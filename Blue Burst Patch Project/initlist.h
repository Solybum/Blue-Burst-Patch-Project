#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "psobb.h"

/// The idea here is to allow initlists to be modified safely from multiple different places
class InitList
{
public:
    typedef void (__cdecl *Function)(void);

#pragma pack(push, 1)
    /// The components of an InitList are FunctionPairs.
    /// The components of a FunctionPair are pointers to functions that take no arguments and return nothing.
    /// The first function is called when resources should be created and the second function is called when those resources should be freed.
    /// In most cases the second function is not required, but the first one is required.
    struct FunctionPair
    {
        Function init;
        Function uninit;

        FunctionPair(Function init, Function uninit);
        FunctionPair(size_t init, size_t uninit);
        FunctionPair();
        bool operator<(const FunctionPair& other) const;
    };
#pragma pack(pop)

    // The instruction operands we patch are often byte-sized, so that is the maximum size we can use. This could be made configurable if needed.
    typedef uint8_t SizeRefValueType;
    static const size_t MAX_FN_PAIR_COUNT = sizeof(SizeRefValueType) * 0xff / sizeof(FunctionPair);

private:
    friend void PSOBB();

    static std::map<const FunctionPair*, std::unique_ptr<InitList>> initLists;
    static bool patchApplied;
    static const FunctionPair nullTerminator;

    const FunctionPair* origList;
    size_t origPairCount;
    std::set<FunctionPair**> listReferenceAddresses;
    std::set<SizeRefValueType*> sizeReferenceAddresses;
    // It is important to maintain the original ordering of the initlist
    std::vector<FunctionPair> functionPairs;
    bool nullTerminated;

    static void PatchAllInitLists();

    InitList(const FunctionPair* listStart, const FunctionPair* listEnd);
    InitList(const FunctionPair* listStart);


public:
    void Patch();

    /// Add addresses that reference the size of the initlist.
    /// These values will be rewritten to the new size of the initlist.
    void AddListReferenceAddress(size_t addr);
    void AddListReferenceAddress(FunctionPair** addr);
    void AddListReferenceAddress(std::initializer_list<size_t> addrs);
    void AddListReferenceAddress(std::initializer_list<FunctionPair**> addrs);

    /// Add addresses that reference the initlist's address.
    /// These pointers will be rewritten to point to the new location of the initlist.
    void AddSizeReferenceAddress(size_t addr);
    void AddSizeReferenceAddress(SizeRefValueType* addr);
    void AddSizeReferenceAddress(std::initializer_list<size_t> addrs);
    void AddSizeReferenceAddress(std::initializer_list<SizeRefValueType*> addrs);

    /// The pair will not be added if the first function of the pair already exists in the list.
    void AddFunctionPair(const FunctionPair& funcPair);
    void AddFunctionPair(const FunctionPair&& funcPair);

    /// Should terminate list with a null element?
    void SetNullTerminated(bool);

    /// Appends the init function pairs from another initlist.
    void Append(const InitList& other);

    /// Removes all init function pairs.
    void Clear();
    
    std::vector<FunctionPair> GetFunctions() const;
    void SetFunctions(const std::vector<FunctionPair>&);

    static InitList& GetInitList(size_t listStartAddr, size_t listEndAddr);
    static InitList& GetInitList(const FunctionPair* listStart, const FunctionPair* listEnd);
    static InitList& GetNullTerminatedInitList(size_t listStartAddr);
    static InitList& GetNullTerminatedInitList(const FunctionPair* listStart);

    std::string toString();
};

