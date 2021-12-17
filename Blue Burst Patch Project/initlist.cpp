#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include "initlist.h"

std::map<size_t, std::unique_ptr<InitList>> InitList::initLists;
bool InitList::patchApplied = false;

InitList::FunctionPair::FunctionPair(InitList::Function init_, InitList::Function uninit_) : init(init_), uninit(uninit_) {}
InitList::FunctionPair::FunctionPair() : init(nullptr), uninit(nullptr) {}

InitList::InitList(FunctionPair* listStart, FunctionPair* listEnd) :
    origList(listStart),
    origPairCount((listEnd - listStart) / sizeof(FunctionPair))
{
    FunctionPair* current = listStart;

    // Read function pairs into vector
    while (current < listEnd)
    {
        functionPairs.push_back(*current);
        current++;
    }
}

InitList& InitList::GetInitList(size_t listStartAddr, size_t listEndAddr)
{
    auto entry = initLists.find(listStartAddr);

    if (entry == initLists.end())
    {
        // No existing object found, create new
        initLists[listStartAddr] = std::unique_ptr<InitList>(
            new InitList(reinterpret_cast<FunctionPair*>(listStartAddr),
                         reinterpret_cast<FunctionPair*>(listEndAddr)));
    }

    return *initLists[listStartAddr];
}

void InitList::AddListReferenceAddress(std::initializer_list<size_t> addrs)
{
    listReferenceAddresses.insert(addrs);
}

void InitList::AddSizeReferenceAddress(std::initializer_list<size_t> addrs)
{
    sizeReferenceAddresses.insert(addrs);
}

void InitList::AddFunctionPair(const FunctionPair& funcPair)
{
    functionPairs.push_back(funcPair);
}

void InitList::AddFunctionPair(const FunctionPair&& funcPair)
{
    functionPairs.push_back(funcPair);
}

void InitList::PatchAllInitLists()
{
    if (patchApplied)
    {
        // Patching twice could cause problems if initlists were modified in between
        // This should never happen anyway, but just in case
        std::runtime_error("InitList: Tried to apply patch twice");
    }

    for (const auto& entry : initLists)
    {
        InitList& newList = *entry.second;

        // Check initlist size
        size_t pairCount = newList.functionPairs.size();
        if (pairCount > MAX_FN_PAIR_COUNT)
        {
            std::runtime_error(newList.toString() + ": Too many function pairs (was: " +
                std::to_string(pairCount) + ", max: " + std::to_string(MAX_FN_PAIR_COUNT) + ")");
        }

        // Rewrite initlist size references
        size_t byteSize = pairCount * sizeof(FunctionPair);
        for (size_t address : newList.sizeReferenceAddresses)
        {
            *(uint8_t*) address = (uint8_t) byteSize;
        }

        // This whole thing won't do anything if there isn't at least one list reference
        if (newList.listReferenceAddresses.empty())
        {
            std::runtime_error(newList.toString() + ": At least one list reference must be defined");
        }

        // Copy the new list to an array on the heap which will be deliberately leaked
        // because we need the initlists to stay allocated for the entire lifetime of the program anyway
        // and we'll just let the system clean them up after exiting
        FunctionPair* listCopy = new FunctionPair[newList.functionPairs.size()];
        std::copy(newList.functionPairs.begin(), newList.functionPairs.end(), listCopy);

        // Rewrite initlist references
        for (size_t address : newList.listReferenceAddresses)
        {
            *(FunctionPair**) address = listCopy;
        }
    }

    patchApplied = true;
}

std::string InitList::toString()
{
    std::stringstream sstream;
    sstream << "InitList(" << std::hex << (size_t) origList << ")";
    return sstream.str();
}
