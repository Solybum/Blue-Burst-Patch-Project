#pragma once

#include <cstdint>
#include <vector>

namespace CustomizeMenu
{
    enum class ActionCategory : uint16_t
    {
        Blank = 0x0,
        Attack = 0x2,
        Technique = 0x3,
        ToolItem = 0x4,
        PhotonBlast = 0x5,
        Interaction = 0x7,
        ListTerminator = 0x8
    };

    enum class BlankAction : uint16_t
    {
        Blank = 0x0
    };

    enum class AttackAction : uint16_t
    {
        NormalAttack = 0x0,
        HeavyAttack = 0x1,
        SpecialAttack = 0x2
    };

    enum class TechniqueAction : uint16_t
    {
        Foie = 0x0,
        Gifoie = 0x1,
        Rafoie = 0x2,
        Barta = 0x3,
        Gibarta = 0x4,
        Rabarta = 0x5,
        Zonde = 0x6,
        Gizonde = 0x7,
        Razonde = 0x8,
        Grants = 0x9,
        Deband = 0xa,
        Jellen = 0xb,
        Zalure = 0xc,
        Shifta = 0xd,
        Ryuker = 0xe,
        Resta = 0xf,
        Anti = 0x10,
        Reverser = 0x11,
        Megid = 0x12
    };

    enum class ToolItemAction : uint16_t
    {
        Monomate = 0x0,
        Dimate = 0x1,
        Trimate = 0x2,
        Monofluid = 0x3,
        Difluid = 0x4,
        Trifluid = 0x5,
        Antidote = 0x6,
        Antiparalysis = 0x7,
        SolAtomizer = 0x8,
        MoonAtomizer = 0x9,
        StarAtomizer = 0xa,
        Telepipe = 0xb,
        TrapVision = 0xc,
        FireTrap = 0xd,
        FreezeTrap = 0xe,
        SlowTrap = 0xf,
        ConfusionTrap = 0x10
    };

    enum class PhotonBlastAction : uint16_t
    {
        Farlla = 0x0,
        Estlla = 0x1,
        Golla = 0x2,
        Pilla = 0x3,
        Leilla = 0x4,
        MyllaYoulla = 0x5
    };

    enum class InteractionAction : uint16_t
    {
        TalkToCapsule = 0x1,
        TalkToNPC = 0x2,
        PressButton = 0x3,
        PickupItem = 0x4,
        EnterWarp1 = 0x5,
        EnterWarp2 = 0x6,
        Unknown1 = 0x7,
        WordSelect = 0x8,
        QuickMenu = 0x9,
        Unknown2 = 0xa,
        Unknown3 = 0xb
    };

    enum class ListTerminatorAction : uint16_t
    {
        ListTerminator = 0x0
    };

#pragma pack(push, 1)
    typedef union
    {
        BlankAction blankAction;
        AttackAction attackAction;
        TechniqueAction techniqueAction;
        ToolItemAction toolItemAction;
        PhotonBlastAction photonBlastAction;
        InteractionAction interactionAction;
        ListTerminatorAction listTerminatorAction;
    } ActionType;

    typedef struct
    {
        ActionCategory actionCategory;
        ActionType actionType;
    } ActionListEntry;
#pragma pack(pop)

    typedef std::vector<ActionListEntry> ActionList;

    // Presets
    const ActionListEntry actionListTerminator = []()
    {
        ActionListEntry terminator;
        terminator.actionCategory = ActionCategory::ListTerminator;
        terminator.actionType.listTerminatorAction = ListTerminatorAction::ListTerminator;
        return terminator;
    }();

    const ActionListEntry blankActionListEntry = []()
    {
        ActionListEntry blank;
        blank.actionCategory = ActionCategory::Blank;
        blank.actionType.blankAction = BlankAction::Blank;
        return blank;
    }();

    ActionList& GetFleshieActionList();
    ActionList& GetAndroidActionList();
    void ApplyActionListPatch();
};
