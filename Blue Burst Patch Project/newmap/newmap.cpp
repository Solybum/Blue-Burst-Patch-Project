#ifdef PATCH_NEWMAP

#include <array>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include "newmap.h"
#include "keyboard.h"
#include "map_object.h"
#include "map_object_cloud.h"
#include "object_extension.h"
#include "player.h"
#include "helpers.h"
#include "mathutil.h"
#include "quest.h"

#pragma pack(push, 1)
struct MapDesignation
{
    uint32_t map_id;
    uint32_t map_variant;
    uint32_t object_set;
};

struct MapAssetPrefixes
{
    struct Prefixes {
        const char* basename;
        const char* variant_names[];
    }* prefixes;
    uint32_t variant_count;
};

struct MapLoadingHandler
{
    const char* name;
    uint32_t (__cdecl *init)();
    void* (__cdecl *uninit)();
};
#pragma pack(pop)

const size_t VANILLA_FLOOR_COUNT = 18;
const size_t VANILLA_MAP_COUNT = 47;
auto vanillaFloorMapDesignations = reinterpret_cast<MapDesignation*>(0x00aafce0);
auto vanillaMapAssetPrefixes = reinterpret_cast<MapAssetPrefixes*>(0x00a116e0);
auto vanillaUltMapAssetPrefixes = reinterpret_cast<MapAssetPrefixes*>(0x00a114e0);
auto mapLoadingHandlers = reinterpret_cast<MapLoadingHandler*>(0x00a160c0);

MapAssetPrefixes::Prefixes snowMapAssetPrefixStrings = {
    "map_snow01",
    "map_snow01"
};
std::array<MapAssetPrefixes, 1> customMaps = {
    MapAssetPrefixes { &snowMapAssetPrefixStrings, 1 }
};
std::unordered_map<uint32_t, uint32_t> currentMapSubtitutions; // (Original map index, custom map index)

/**
 * @brief Custom maps will use the objects and enemies from the matching vanilla maps, but can have custom map geometry files.
 */
const MapAssetPrefixes::Prefixes* __cdecl GetMapAssetPrefixes(uint32_t map)
{
    if (!currentMapSubtitutions.count(map))
    {
        // Do vanilla behavior
        if (IsUltEp1()) return vanillaUltMapAssetPrefixes[map].prefixes;
        return vanillaMapAssetPrefixes[map].prefixes;
    }

    return customMaps[currentMapSubtitutions[map]].prefixes;
}

__attribute__((regparm(1))) // Take argument in EAX
uint32_t __cdecl Before_InitEpisodeMaps(uint32_t episode)
{
    // This gets called when entering or leaving a game or the lobby and from set_episode opcode
    // (but not when going to main menu, but will get called when entering lobby again).
    // Seems like a good place to reset the map subtitutions.
    currentMapSubtitutions.clear();
    // Code we overwrote
    *reinterpret_cast<uint32_t*>(0x00aafdb8) = episode;
    // Return to original code
    return episode;
}

void __cdecl NewOpcodeDesignateCustomMap(uint8_t origMap, uint8_t newMap)
{
    currentMapSubtitutions.insert({origMap, newMap});

    // TODO: Replace area initializer
}

void PatchMapDesignateOpcode()
{
    PatchJMP(0x0080bee8, 0x0080bf11, (int) GetMapAssetPrefixes);
    PatchCALL(0x0080c7a0, 0x0080c7a5, (int) Before_InitEpisodeMaps);
    Quest::SetOpcode(0xf962, Quest::SetupOpcodeOperand11, (void*) NewOpcodeDesignateCustomMap);
}

#pragma pack(push, 1)
struct SlbgmTransition
{
    uint16_t start_index;
    uint16_t length;
    uint16_t transition_in;
    uint16_t transition_out;
    uint16_t next_part;
};
struct SlbgmDef
{
    char name[32];
    uint32_t count;
    uint32_t unk1;
    uint32_t unk2;
    SlbgmTransition* track1;
    SlbgmTransition* track2;
};
#pragma pack(pop)

void ApplyNewMapPatch()
{
    PatchMapDesignateOpcode();

    EnableMapObjectCloud();

    Keyboard::onKeyDown(Keyboard::Keycode::H, []() {
        auto player = GetPlayer(0);

        MapObject::InitData::InnerData initData = {
            10000,
            0,
            10000+4096,
            0,
            player->room,
            0,
            Vec3f {player->position.x, player->position.y, player->position.z},
            Vec3<int32_t> { 0, 0, 0},
            0,
            0,
            0,
            0,
            0,
            0,
            0
        };
        MapObjectCloud::Create(&initData);
    });
}

#endif // PATCH_NEWMAP
