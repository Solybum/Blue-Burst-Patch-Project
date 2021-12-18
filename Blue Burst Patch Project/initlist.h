#include <cstddef>
#include <memory>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "psobb.h"

// Automatically enable patch if header is used
#define PATCH_INITLISTS

/// The idea here is to allow initlists to be modified safely from multiple different places
class InitList
{
public:
    typedef void (__cdecl *Function)(void);

#pragma pack(push, 1)
    struct FunctionPair
    {
        Function init;
        Function uninit;

        FunctionPair(Function init, Function uninit);
        FunctionPair();
    };
#pragma pack(pop)

private:
    friend void PSOBB();

    static std::map<size_t, std::unique_ptr<InitList>> initLists;
    static bool patchApplied;

    FunctionPair* origList;
    size_t origPairCount;
    std::set<size_t> listReferenceAddresses;
    std::set<size_t> sizeReferenceAddresses;
    std::vector<FunctionPair> functionPairs;

    static void PatchAllInitLists();

    InitList(FunctionPair* listStart, FunctionPair* listEnd);

public:
    /// The total byte size of an initlist must fit within a byte
    static const size_t MAX_FN_PAIR_COUNT = 0xff / sizeof(FunctionPair);

    /// Add addresses that reference the size of the initlist.
    /// These values will be rewritten to the new size of the initlist.
    void AddListReferenceAddress(std::initializer_list<size_t> addrs);

    /// Add addresses that reference the initlist's address.
    /// These pointers will be rewritten to point to the new location of the initlist.
    void AddSizeReferenceAddress(std::initializer_list<size_t> addrs);

    void AddFunctionPair(const FunctionPair& funcPair);
    void AddFunctionPair(const FunctionPair&& funcPair);

    // size_t because it's probably easier to use than casting to a pointer
    static InitList& GetInitList(size_t listStartAddr, size_t listEndAddr);

    std::string toString();
};

