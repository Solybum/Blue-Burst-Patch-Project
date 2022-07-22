#ifdef PATCH_INITLISTS

#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include "initlist.h"

std::map<const InitList::FunctionPair*, std::unique_ptr<InitList>> InitList::initLists;
bool InitList::patchApplied = false;
const InitList::FunctionPair InitList::nullTerminator = FunctionPair(nullptr, nullptr);

InitList::FunctionPair::FunctionPair(InitList::Function init_, InitList::Function uninit_) : init(init_), uninit(uninit_) {}

InitList::FunctionPair::FunctionPair(size_t init_, size_t uninit_) : init(reinterpret_cast<InitList::Function>(init_)), uninit(reinterpret_cast<InitList::Function>(uninit_)) {}

InitList::FunctionPair::FunctionPair() : init(nullptr), uninit(nullptr) {}

bool InitList::FunctionPair::operator<(const FunctionPair& other) const
{
    return ((size_t) init) < ((size_t) other.init);
}

InitList::InitList(const FunctionPair* listStart, const FunctionPair* listEnd) :
    origList(listStart),
    origPairCount((listEnd - listStart) / sizeof(FunctionPair)),
    nullTerminated(false)
{
    const FunctionPair* current = listStart;

    // Read function pairs into vector
    while (current < listEnd)
    {
        AddFunctionPair(*current);
        current++;
    }
}

InitList::InitList(const FunctionPair* listStart) :
    origList(listStart),
    nullTerminated(true)
{
    const FunctionPair* current = listStart;

    // Read function pairs into vector
    while (current->init != nullptr)
    {
        AddFunctionPair(*current);
        current++;
    }

    origPairCount = (current - listStart) / sizeof(FunctionPair);
}

InitList& InitList::GetInitList(const FunctionPair* listStart, const FunctionPair* listEnd)
{
    auto entry = initLists.find(listStart);

    if (entry == initLists.end())
    {
        // No existing object found, create new
        initLists[listStart] = std::unique_ptr<InitList>(new InitList(listStart, listEnd));
    }

    return *initLists[listStart];
}

InitList& InitList::GetInitList(size_t listStartAddr, size_t listEndAddr)
{
    return GetInitList(reinterpret_cast<const FunctionPair*>(listStartAddr), reinterpret_cast<const FunctionPair*>(listEndAddr));
}

InitList& InitList::GetNullTerminatedInitList(const FunctionPair* listStart)
{
    auto entry = initLists.find(listStart);

    if (entry == initLists.end())
    {
        // No existing object found, create new
        initLists[listStart] = std::unique_ptr<InitList>(new InitList(listStart));
    }

    return *initLists[listStart];
}

InitList& InitList::GetNullTerminatedInitList(size_t listStartAddr)
{
    return GetNullTerminatedInitList(reinterpret_cast<const FunctionPair*>(listStartAddr));
}

void InitList::AddListReferenceAddress(FunctionPair** addr)
{
    listReferenceAddresses.insert(addr);
}

void InitList::AddListReferenceAddress(size_t addr)
{
    AddListReferenceAddress(reinterpret_cast<FunctionPair**>(addr));
}

void InitList::AddListReferenceAddress(std::initializer_list<FunctionPair**> addrs)
{
    listReferenceAddresses.insert(addrs);
}

void InitList::AddListReferenceAddress(std::initializer_list<size_t> addrs)
{
    for (size_t addr : addrs)
    {
        AddListReferenceAddress(reinterpret_cast<FunctionPair**>(addr));
    }
}

void InitList::AddSizeReferenceAddress(SizeRefValueType* addr)
{
    sizeReferenceAddresses.insert(addr);
}

void InitList::AddSizeReferenceAddress(size_t addr)
{
    AddSizeReferenceAddress(reinterpret_cast<SizeRefValueType*>(addr));
}

void InitList::AddSizeReferenceAddress(std::initializer_list<SizeRefValueType*> addrs)
{
    sizeReferenceAddresses.insert(addrs);
}

void InitList::AddSizeReferenceAddress(std::initializer_list<size_t> addrs)
{
    for (size_t addr : addrs)
    {
        AddSizeReferenceAddress(reinterpret_cast<SizeRefValueType*>(addr));
    }
}

void InitList::AddFunctionPair(const FunctionPair& funcPair)
{
    // Check duplicate
    for (const FunctionPair& existing : functionPairs)
    {
        if (existing.init == funcPair.init) return;
    }

    functionPairs.push_back(funcPair);
}

void InitList::AddFunctionPair(const FunctionPair&& funcPair)
{
    AddFunctionPair(funcPair);
}

void InitList::SetNullTerminated(bool option)
{
    nullTerminated = option;
}

bool InitList::HasChanged()
{
    return functionPairs.size() != origPairCount;
}

void InitList::Patch()
{
    if (!HasChanged())
    {
        // Nothing to do
        return;
    }

    // This whole thing won't do anything if there isn't at least one list reference
    if (listReferenceAddresses.empty())
    {
        std::runtime_error(toString() + ": At least one list reference must be defined");
    }

    // Check initlist size
    size_t pairCount = functionPairs.size();
    if (pairCount > MAX_FN_PAIR_COUNT)
    {
        std::runtime_error(toString() + ": Too many function pairs (was: " +
            std::to_string(pairCount) + ", max: " + std::to_string(MAX_FN_PAIR_COUNT) + ")");
    }

    // Rewrite initlist size references
    size_t byteSize = pairCount * sizeof(FunctionPair);
    for (SizeRefValueType* address : sizeReferenceAddresses)
    {
        *address = (SizeRefValueType) byteSize;
    }

    size_t newListLength = functionPairs.size();
    if (nullTerminated)
    {
        // Add space for null terminator
        newListLength += 1;
    }

    // Copy the new list to an array on the heap which will be deliberately leaked
    // because we need the initlists to stay allocated for the entire lifetime of the program anyway
    // and we'll just let the system clean them up after exiting
    FunctionPair* listCopy = new FunctionPair[newListLength];
    std::copy(functionPairs.begin(), functionPairs.end(), listCopy);

    if (nullTerminated)
    {
        // Append null terminator
        listCopy[functionPairs.size()] = InitList::nullTerminator;
    }

    // Rewrite initlist references
    for (FunctionPair** address : listReferenceAddresses)
    {
        *address = listCopy;
    }
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
        InitList& lst = *entry.second;
        lst.Patch();
    }

    patchApplied = true;
}

std::string InitList::toString()
{
    std::stringstream sstream;
    sstream << "InitList(" << std::hex << (size_t) origList << ")";
    return sstream.str();
}

#endif // PATCH_INITLISTS
