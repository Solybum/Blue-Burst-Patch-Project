# Blue Burst Patch Project
A collection of patches for the Phantasy Star Online Blue Burst client.

## Patches
Some patches must be enabled at compile time via preprocessor definitions and some patches may be enabled and disabled via the Windows Registry.

### Disable GameGuard `[COMPILED:DO_NOT_PATCH_DISABLE_GAMEGUARD]`
Enables GameGuard.  
GameGuard is disabled by default, use this flag to skip that patch.  
Note: Some GameGuard code was previously disabled in the client binary.

### Unsellable rare items `[COMPILED:PATCH_UNSELLABLE_RARES]`
Prevents you from selling rare items to vendors.

### Enable IME `[COMPILED:PATCH_IME]`
Enables the use of IME.

### Early walk fix `[COMPILED:PATCH_EARLY_WALK_FIX]`
Fixes the notorious "early walk" bug which is when enemies sometimes skip their battle cry animation and start moving immediately after spawning.

### Slow Gibbles fix `[COMPILED:PATCH_SLOW_GIBBLES_FIX]`
Fixes the "slow Gibbles" bug which causes the spawning animation of Gibbles to be permanently slow after they do their jump attack.
The purpose of the patch is not to change game mechanics but to avoid having to restart the client to fix the bug thus the fix is only applied when a new party is created.

### Fastwarp `[COMPILED:PATCH_FASTWARP]`
This patch improves all loading times by allowing the game to load assets as fast as possible during loading screens. Can be disabled during quests with FASTWARP_NO_QUEST.

### Omnispawn `[COMPILED:PATCH_OMNISPAWN]`
Allows spawning any enemy in any area with some exceptions.
* Del Lily may not be spawned outside of Tower and other Lilies may not be spawned in Tower.
* Sinow Zoa may not be spawned outside of Seabed and Epsilon may not be spawned in Seabed.
* Only the Rappies that normally appear in an episode maybe be spawned in that episode.
* Enemies that appear in both E1 and E2 have the current episode's stats.
* E4 enemies have Crater stats in E1 and Desert stats in E2.
* Enemies that appear in both E1 and E2 have E1 stats in Crater and E2 stats in Desert.

**Note for quest makers**: QEdit can be configured to allow creating Omnispawn quests via the FloorSet.ini configuration file. See the QEdit wiki for details.

### Large assets patch `[COMPILED:PATCH_LARGE_ASSETS]`
Allows loading of larger asset files.

### New Enemy `[COMPILED:PATCH_NEWENEMY]`
A demonstration of how a new enemy may be implemented using the object extension framework.

### Intro credits skip `[COMPILED:PATCH_SKIP_INTRO_CREDITS]`
Skips the credits screen when the game is launched.

### Debug menus `[COMPILED:PATCH_EDITORS]`
This patch restores various debug editors and menus used by the original developers.

## Installation
Use the psobb.exe bundled with this project. That client is modified to automatically load bbpp.dll. Place bbpp.dll in your game directory.

## Building
### Windows
Use Visual Studio.

### Linux
Use MinGW and CMake.

```
mkdir build && cd build
CMAKE_TOOLCHAIN_FILE="../CMakeLists-mingw.txt" cmake -DPATCH_XYZ=ON .. && cmake --build .
```

It is also possible to build the project on Linux by using Wine to run MSVC.
Install MSVC ([recommended Dockerfile](https://github.com/mstorsjo/msvc-wine)) and run wine-build.sh.

## License
Blue Burst Patch Project is licensed under the MIT license.
This product contains unmodified and modified subcomponents with separate copyright notices and license terms.
* [Learn OpenGL](https://learnopengl.com/Guest-Articles/2020/Skeletal-Animation) ([CC BY 4.0](https://creativecommons.org/licenses/by/4.0/legalcode)): Newgfx library is derived from Learn OpenGL
* [Wine](https://source.winehq.org/source/) ([LGPL-2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html)): Newgfx library depends on Wine's header files
* Thanks to VincentNL for their help with developing the debug editor restoration patch.
