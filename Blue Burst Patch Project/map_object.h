#pragma once

#include <cstdint>
#include <unordered_map>
#include <windows.h>
#include "enemy.h"
#include "map.h"
#include "mathutil.h"
#include "object_extension.h"

namespace MapObject
{
#pragma pack(push, 1)

    enum class MapObjectType : uint16_t
    {
        PlayerSet1 = 0,
        PlayerSet2 = 0,
        PlayerSet3 = 0,
        PlayerSet4 = 0,
        Particle = 1,
        Teleporter = 2,
        Warp = 3,
        LightCollision = 4,
        Item = 5,
        EnvSound = 6,
        FogCollision = 7,
        EventCollision = 8,
        CharaCollision = 9,
        ElementalTrap = 10,
        StatusTrap = 11,
        HealTrap = 12,
        LargeElementalTrap = 13,
        ObjRoomID = 14,
        Sensor = 15,
        UnknownItem_16 = 16,
        Lensflare = 17,
        ScriptCollision = 18,
        HealRing = 19,
        MapCollision = 20,
        ScriptCollisionA = 21,
        ItemLight = 22,
        RadarCollision = 23,
        FogCollisionSW = 24,
        BossTeleporter = 25,
        ImageBoard = 26,
        AreaWarp_Ep1_excludingWarpSound = 27,
        Epilogue = 28,
        UnknownItem_29 = 29,
        UnknownItem_30 = 30,
        UnknownItem_31 = 31,
        BoxDetectObject = 32,
        SymbolChatObject = 33,
        TouchPlateObject = 34,
        TargetableObject = 35,
        EffectObject = 36,
        CountDownObject = 37,
        UnknownItem_38 = 38,
        UnknownItem_39 = 39,
        UnknownItem_40 = 40,
        UnknownItem_41 = 41,
        MenuActivation = 64,
        TelepipeLocation = 65,
        BGMCollision = 66,
        MainRagolTeleporter = 67,
        LobbyTeleporter = 68,
        PrincipalWarp = 69,
        PrincipalWarpB = 69,
        ShopDoor = 70,
        HuntersGuildDoor = 71,
        TeleporterDoor = 72,
        MedicalCenterDoor = 73,
        Elevator = 74,
        EasterEgg = 75,
        ValentinesHeart = 76,
        ChristmasTree = 77,
        ChristmasWreath = 78,
        HalloweenPumpkin = 79,
        TwentyFirstCentury = 80,
        DrRobotnic = 81,
        Sonic = 81,
        Knux = 81,
        Tails = 81,
        WelcomeBoard = 82,
        Firework = 83,
        LobbyScreenDoor = 84,
        MainRagolTeleporter_BattleInNextArea = 85,
        LabTeleporterDoor = 86,
        Pioneer2InvisibleTouchplate = 87,
        ForestDoor = 128,
        ForestSwitch = 129,
        LaserFence = 130,
        LaserSquareFence = 131,
        ForestLaserFenceSwitch = 132,
        LightRays = 133,
        BlueButterfly = 134,
        CrashedProbe = 135,
        Probe = 135,
        RandomTypeBox1 = 136,
        ForestWeatherStation = 137,
        Battery = 138,
        ForestConsole1 = 139,
        ForestConsole2 = 139,
        BlackSlidingDoor = 140,
        RicoMessagePod = 141,
        EnergyBarrier = 142,
        ForestRisingBridge = 143,
        Switch_noneDoor = 144,
        EnemyBox_Grey = 145,
        FixedTypeBox = 146,
        EnemyBox_Brown = 147,
        EmptyTypeBox = 149,
        LaserFenseEx = 150,
        LaserSquareFenceEx = 151,
        FloorPanel1 = 192,
        Caves4ButtonDoor = 193,
        CavesNormalDoor = 194,
        CavesSmashingPillar = 195,
        CavesSign1 = 196,
        CavesSign2 = 197,
        CavesSign3 = 198,
        HexagalTank = 199,
        BrownPlatform = 200,
        WarningLightObject = 201,
        Rainbow = 203,
        FloatingJelifish = 204,
        FloatingDragonfly = 205,
        CavesSwitchDoor = 206,
        RobotRechargeStation = 207,
        CavesCakeShop = 208,
        Caves1SmallRedRock = 209,
        Caves1MediumRedRock = 210,
        Caves1LargeRedRock = 211,
        Caves2SmallRock1 = 212,
        Caves2MediumRock1 = 213,
        Caves2LargeRock1 = 214,
        Caves2SmallRock2 = 215,
        Caves2MediumRock2 = 216,
        Caves2LargeRock2 = 217,
        Caves3SmallRock = 218,
        Caves3MediumRock = 219,
        Caves3LargeRock = 220,
        FloorPanel2 = 222,
        DestructableRock_Caves1 = 223,
        DestructableRock_Caves2 = 224,
        DestructableRock_Caves3 = 225,
        MinesDoor = 256,
        FloorPanel3 = 257,
        MinesSwitchDoor = 258,
        LargeCryoTube = 259,
        Computer_likeCalus = 260,
        GreenScreenOpeningAndClosing = 261,
        FloatingRobot = 262,
        FloatingBlueLight = 263,
        SelfDestructingObject1 = 264,
        SelfDestructingObject2 = 265,
        SelfDestructingObject3 = 266,
        SparkMachine = 267,
        MinesLargeFlashingCrate = 268,
        RuinsSeal = 304,
        RuinsTeleporter = 320,
        RuinsWarp_SiteToSite = 321,
        RuinsSwitch = 322,
        FloorPanel4 = 323,
        Ruins1Door = 324,
        Ruins3Door = 325,
        Ruins2Door = 326,
        Ruins11ButtonDoor = 327,
        Ruins21ButtonDoor = 328,
        Ruins31ButtonDoor = 329,
        Ruins4ButtonDoor = 330,
        Ruins2ButtonDoor = 331,
        RuinsSensor = 332,
        RuinsFenceSwitch = 333,
        RuinsLaserFence4x2 = 334,
        RuinsLaserFence6x2 = 335,
        RuinsLaserFence4x4 = 336,
        RuinsLaserFence6x4 = 337,
        RuinsPoisonBlob = 338,
        RuinsPilarTrap = 339,
        PopupTrap_NoTech = 340,
        RuinsCrystal = 341,
        Monument = 342,
        RuinsRock1 = 345,
        RuinsRock2 = 346,
        RuinsRock3 = 347,
        RuinsRock4 = 348,
        RuinsRock5 = 349,
        RuinsRock6 = 350,
        RuinsRock7 = 351,
        Poison = 352,
        FixedBoxType_Ruins = 353,
        RandomBoxType_Ruins = 354,
        EnemyTypeBox_Yellow = 355,
        EnemyTypeBox_Blue = 356,
        EmptyTypeBox_Blue = 357,
        DestructableRock = 358,
        PopupTraps_techs = 359,
        FlyingWhiteBird = 368,
        Tower = 369,
        FloatingRocks = 370,
        FloatingSoul = 371,
        Butterfly = 372,
        LobbyGameMenu = 384,
        LobbyWarpObject = 385,
        Lobby1EventObject_DefaultTree = 386,
        UnknownItem_387 = 387,
        UnknownItem_388 = 388,
        UnknownItem_389 = 389,
        LobbyEventObject_StaticPumpkin = 390,
        LobbyEventObject_3ChristmasWindows = 391,
        LobbyEventObject_RedAndWhiteCurtain = 392,
        UnknownItem_393 = 393,
        UnknownItem_394 = 394,
        LobbyFishTank = 395,
        LobbyEventObject_Butterflies = 396,
        UnknownItem_400 = 400,
        GreyWall_low_Squares = 401,
        SpaceshipDoor = 402,
        GreyWall_high_HorizontalLines = 403,
        TempleNormalDoor = 416,
        BreakableWallWall_ButUnbreakable = 417,
        BrokenCilinderAndRubble = 418,
        ThreeBrokenWallPiecesOnFloor = 419,
        HighBrickCilinder = 420,
        LyingCilinder = 421,
        BrickConeWithFlatTop = 422,
        BreakableTempleWall = 423,
        TempleMapDetect = 424,
        SmallBrownBrickRisingBridge = 425,
        LongRisingBridge_withPinkHighEdges = 426,
        FourSwitchTempleDoor = 427,
        FourButtonSpaceshipDoor = 448,
        ItemBoxCca = 512,
        Teleporter_Ep2 = 513,
        CCADoor = 514,
        SpecialBoxCCA = 515,
        BigCCADoor = 516,
        BigCCADoorSwitch = 517,
        LittleRock = 518,
        Little3StoneWall = 519,
        Medium3StoneWall = 520,
        SpiderPlant = 521,
        UnknownItem_522 = 522,
        UnknownItem_523 = 523,
        UnknownItem_524 = 524,
        UnknownItem_525 = 525,
        ShortSaw = 527,
        LongSaw = 527,
        LaserDetect = 528,
        LargeLaserDetect = 528,
        UnknownItem_529 = 529,
        UnknownItem_530 = 530,
        Seagull = 531,
        Fish = 544,
        SeabedDoor_withBlueEdges = 545,
        SeabedDoor_alwaysOpen_NonTriggerable = 546,
        LittleCryotube = 547,
        Contaner = 547,
        WideGlassWall_breakable = 548,
        BlueFloatingRobot = 549,
        RedFloatingRobot = 550,
        Dolphin = 551,
        CaptureTrap = 552,
        VRLink = 553,
        UnknownItem_576 = 576,
        WarpInBarbaRayRoom = 640,
        UnknownItem_672 = 672,
        GeeNest = 688,
        LabComputerConsole = 689,
        LabComputerConsole_GreenScreen = 690,
        Chair,YelllowPillow = 691,
        OrangeWallWithHoleInMiddle = 692,
        GreyWallWithHoleInMiddle = 693,
        LongTable = 694,
        GBAStation = 695,
        Talk_linkToSupport = 696,
        InstaWarp = 697,
        LabInvisibleObject = 698,
        LabGlassWindowDoor = 699,
        UnknownItem_700 = 700,
        LabCelingWarp = 701,
        Ep4LightSource = 768,
        LongCacti = 769,
        RoundCacti = 769,
        SetOfCacti = 769,
        BigBrownRockMedium1 = 770,
        BrownRockMedium2 = 770,
        BrownRockSmall = 770,
        BreakableBrownRock = 771,
        UnknownItem_832 = 832,
        UnknownItem_833 = 833,
        PoisonPlant = 896,
        UnknownItem_897 = 897,
        UnknownItem_898 = 898,
        OozingDesertPlant = 899,
        UnknownItem_901 = 901,
        BigBlackRocks_Wilds_Crater = 902,
        MediumBlackRocks_Wilds_Crater = 902,
        SmallBlackRocks_Wilds_Crater = 902,
        UnknownItem_903 = 903,
        UnknownItem_904 = 904,
        UnknownItem_905 = 905,
        UnknownItem_906 = 906,
        FallingRock = 907,
        DesertPlant_hasCollision = 908,
        DesertFixedTypeBox_BreakableCrystals = 909,
        UnknownItem_910 = 910,
        BeeHive1 = 911,
        BeeHive2 = 911,
        UnknownItem_912 = 912,
        Heat = 913,
        TopOfSaintMillionEgg = 960,
        UnknownItem_961 = 961,

    };

    struct MapObjectBase; // Forward declaration for InitData

    union InitParam
    {
        float asFloat;
        uint32_t asDword;
        struct
        {
            uint16_t word1;
            uint16_t word2;
        } asWords;
    };

    /**
     * @brief Data loaded from the quest's .dat file is passed to the object's constructor
     */
    struct InitData
    {
        int16_t skin;
        uint16_t flags;
        struct InnerData
        {
            int16_t indexSmall;
            uint16_t unk1;
            int16_t indexLarge;
            int16_t group;
            int16_t mapSection;
            int16_t unk2;
            Vec3f position;
            Vec3<int32_t> rotation;
            InitParam param1;
            InitParam param2;
            InitParam param3;
            InitParam param4;
            InitParam param5;
            InitParam param6;
            MapObjectBase* spawnedInstance;
        } data;
    };
    
    struct SpawnableDefinition
    {
        uint16_t id = 0;
        void (__cdecl *LoadAssets)(void) = nullptr;
        void (__cdecl *UnloadAssets)(void) = nullptr;
        void* (__cdecl *Create)(InitData::InnerData* initData) = nullptr;
        
        constexpr SpawnableDefinition() = default;
        SpawnableDefinition(MapObjectType id, size_t loadAssetsAddr, size_t unloadAssetsAddr, size_t createAddr);
        SpawnableDefinition(uint16_t id,
            decltype(LoadAssets) LoadAssets,
            decltype(UnloadAssets) UnloadAssets,
            decltype(Create) Create);
    };
    
    const std::unordered_map<MapObjectType, SpawnableDefinition>& GetMapObjectDefinitions();
    const SpawnableDefinition& GetMapObjectDefinition(MapObjectType);

    struct MapObjectBase
    {
        struct Vtable {
            union {
                void (__thiscall *Destruct)(void* self, BOOL freeMemory);
                DEFINE_FIELD(0x4, void (__thiscall *Update)(void* self));
                DEFINE_FIELD(0x8, void (__thiscall *Render)(void* self));
                DEFINE_FIELD(0xc, void (__thiscall *RenderShadow)(void* self));
                DEFINE_FIELD(0x14, void (__thiscall *ApplyInitData)(void* self, InitData::InnerData* initData));
                DEFINE_FIELD(0x64, void (__thiscall *HandleUnlockEvent)(void* self, void* packet));
            };
        };

        typedef void* (__thiscall *ConstructorFunction)(void* self, void* parentObject);
        static const ConstructorFunction Constructor;
        static const Vtable* origVtable;

        union
        {
            Vtable* vtable;

            union {
                DEFINE_FIELD(0x4, void* typeId);
                DEFINE_FIELD(0x1c, uint16_t entityIndex);
                DEFINE_FIELD(0x28, int16_t mapSection);
                DEFINE_FIELD(0x38, Vec3f xyz1);
                DEFINE_FIELD(0x44, Vec3f xyz5);
                DEFINE_FIELD(0x50, Vec3f xyz2);
                DEFINE_FIELD(0x5c, Vec3<int32_t> rotation);
                DEFINE_FIELD(0x68, InitParam initParam1);
                DEFINE_FIELD(0x6c, InitParam initParam2);
                DEFINE_FIELD(0x70, InitParam initParam3);
                DEFINE_FIELD(0x84, InitParam initParam4);
                DEFINE_FIELD(0x88, InitParam initParam5);
                DEFINE_FIELD(0x8c, InitParam initParam6);
                DEFINE_FIELD(0xa4, Enemy::CollisionBox* collisionBoxes);
                DEFINE_FIELD(0xa8, uint32_t collisionBoxCount);
            };

            // Ensure object's size is at least the same as its superclass
            uint8_t _padding[0x37c];
        };

        MapObjectBase(void* parentObject);

        void Destruct(bool32 freeMemory);
    };

    using CreationFunction = void* (__cdecl *)(InitData::InnerData*);

    struct TaggedMapObjectConstructor
    {
        uint16_t objectType;
        uint16_t unknown1;
        CreationFunction createObject;
        float constructionDistance;
        uint32_t unknown2;

        TaggedMapObjectConstructor(uint16_t type, CreationFunction createFun);
        TaggedMapObjectConstructor();
    };
#pragma pack(pop)

    extern void** rootMapObject;

    std::vector<TaggedMapObjectConstructor>& GetMapObjectConstructorList(Map::MapType map);

    void PatchMapObjectConstructorLists();
    
    extern InitData* (__cdecl *GetFloorInitDataArray)(uint32_t floor);
    extern uint32_t (__cdecl *GetFloorObjectCount)(uint32_t floor);
};
