#include <iostream>
#include <algorithm>
#include "customize_menu.h"

namespace CustomizeMenu
{
    ActionList ReadEntriesIntoActionList(size_t entriesAddr)
    {
        ActionListEntry* entries = reinterpret_cast<ActionListEntry*>(entriesAddr);
        ActionList actionList;

        while (entries->actionCategory != ActionCategory::ListTerminator)
        {
            actionList.push_back(*entries);
            entries++;
        }

        return actionList;
    }

    // Original action lists.
    ActionList fleshieActionList = ReadEntriesIntoActionList(0x009737e0);
    ActionList androidActionList = ReadEntriesIntoActionList(0x00973880);

    // Addresses of code that refer to the action lists.
    ActionListEntry** fleshieActionListPatchAddr = reinterpret_cast<ActionListEntry**>(0x0074a2b1);
    ActionListEntry** androidActionListPatchAddr = reinterpret_cast<ActionListEntry**>(0x0074a2a4);

    // Expose action lists for modification by other patches.
    ActionList& GetFleshieActionList() { return fleshieActionList; }
    ActionList& GetAndroidActionList() { return androidActionList; }

    // Ensures the action list is correctly structured.
    void FixupActionList(ActionList& actionList)
    {
        // Action list must be padded with blank entries to align with the row length.
        // Action list must have a terminator at the end.
        const size_t rowLength = 3;
        bool hasTerminator = false;
        bool isAligned = true;

        if (!actionList.empty()) {
            hasTerminator = actionList.back().actionCategory == ActionCategory::ListTerminator;

            size_t numEntries = actionList.size();

            if (hasTerminator) {
                numEntries--;
            }

            isAligned = numEntries % rowLength == 0;
        }

        if (hasTerminator && !isAligned) {
            // Remove terminator(s) and retry
            actionList.pop_back();
            FixupActionList(actionList);
            return;
        }

        if (!isAligned) {
            // Add padding
            size_t numToAdd = rowLength - actionList.size() % rowLength;
            for (size_t i = 0; i < numToAdd; i++) {
                actionList.push_back(blankActionListEntry);
            }
        }

        if (!hasTerminator) {
            // Add terminator
            actionList.push_back(actionListTerminator);
        }
    }

    ActionListEntry* CopyActionListToHeap(const ActionList& actionList)
    {
        ActionListEntry* copy = new ActionListEntry[actionList.size()];
        std::copy(actionList.begin(), actionList.end(), copy);
        return copy;
    }

    bool patchApplied = false;

    void ApplyActionListPatch()
    {
        if (patchApplied) {
            return;
        }

        FixupActionList(fleshieActionList);
        FixupActionList(androidActionList);

        // Replace references with new action lists.
        // Probably no need to keep track of these buffers.
        *fleshieActionListPatchAddr = CopyActionListToHeap(fleshieActionList);
        *androidActionListPatchAddr = CopyActionListToHeap(androidActionList);

        patchApplied = true;
    }
};
