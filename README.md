# Blue Burst Patch Project
A collection of patches for the Phantasy Star Online Blue Burst client.

## Patches
Some patches must be enabled at compile time via preprocessor definitions and some patches may be enabled and disabled via the Windows Registry.

### Disable GameGuard `[COMPILED:PATCH_DISABLE_GAMEGUARD]`
Disables GameGuard.

### Unsellable rare items `[REGISTRY:TODO]`
Prevents you from selling rare items to vendors.

### Early walk fix `[COMPILED:PATCH_EARLY_WALK_FIX]`
Fixes the notorious "early walk" bug which is when enemies sometimes skip their battle cry animation and start moving immediately after spawning.

## Installation
Use the psobb.exe bundled with this project. That client is modified to automatically load bbpp.dll. Place bbpp.dll in your game directory.

## Building
Use Visual Studio.
