# Blue Burst Patch Project
A collection of patches for the Phantasy Star Online Blue Burst client.

## Patches
Some patches must be enabled at compile time via preprocessor definitions and some patches may be enabled and disabled via the Windows Registry.

### Disable GameGuard `[COMPILED:DO_NOT_PATCH_DISABLE_GAMEGUARD]`
Enables GameGuard.  
GameGuard is disabled by default, use this flag to skip that patch.  
Note: Some GameGuard code was previously disabled in the client binary.

### Unsellable rare items `[COMPILED:PATCH_SHOP - REGISTRY:TODO]`
Prevents you from selling rare items to vendors.

### Early walk fix `[COMPILED:PATCH_EARLY_WALK_FIX]`
Fixes the notorious "early walk" bug which is when enemies sometimes skip their battle cry animation and start moving immediately after spawning.

### Slow Gibbles fix `[COMPILED:PATCH_SLOW_GIBBLES_FIX]`
Fixes the "slow Gibbles" bug which causes the spawning animation of Gibbles to be permanently slow after they do their jump attack.
The purpose of the patch is not to change game mechanics but to avoid having to restart the client to fix the bug thus the fix is only applied when a new party is created.

## Installation
Use the psobb.exe bundled with this project. That client is modified to automatically load bbpp.dll. Place bbpp.dll in your game directory.

## Building
### Windows
Use Visual Studio.

### Linux
It is possible to build the project on Linux by using Wine to run MSVC.
Install MSVC ([recommended Dockerfile](https://github.com/mstorsjo/msvc-wine)) and run wine-build.sh.
