# This macro defines a cmake option that can be used to enable a patch.
# ARGN can be used for additional defines.
macro(define_optional_patch patch_name)
    option(${patch_name} "" OFF)

    if(${patch_name})
        add_compile_definitions(${patch_name})

        foreach(additional_define ${ARGN})
            set(${additional_define} ON)
            add_compile_definitions(${additional_define})
        endforeach()
    endif()
endmacro()

# Define all patches
define_optional_patch(PATCH_CUSTOMIZE_MENU)
define_optional_patch(PATCH_DISABLE_GAMEGUARD)
define_optional_patch(PATCH_EARLY_WALK_FIX)
define_optional_patch(PATCH_FASTWARP)
define_optional_patch(PATCH_IME)
define_optional_patch(PATCH_KEYBOARD_ALTERNATE_PALETTE)
define_optional_patch(PATCH_LARGE_ASSETS)
define_optional_patch(PATCH_NEWENEMY USE_NEWGFX PATCH_INITLISTS PATCH_ENEMY_CONSTRUCTOR_LISTS)
define_optional_patch(PATCH_OMNISPAWN PATCH_INITLISTS PATCH_ENEMY_CONSTRUCTOR_LISTS)
define_optional_patch(PATCH_SKIP_INTRO_CREDITS)
define_optional_patch(PATCH_SLOW_GIBBLES_FIX PATCH_INITLISTS)
define_optional_patch(PATCH_UNSELLABLE_RARES)
define_optional_patch(PATCH_HOOKS)
define_optional_patch(PATCH_KEYBOARD_HOOKS PATCH_HOOKS)
define_optional_patch(PATCH_EDITORS PATCH_KEYBOARD_HOOKS)
define_optional_patch(PATCH_NEWMAP PATCH_MAP_OBJECT_CONSTRUCTOR_LISTS PATCH_ENEMY_CONSTRUCTOR_LISTS PATCH_INITLISTS)
define_optional_patch(ENABLE_DEVMOD)

# This makes the lpVtbl field of COM objects accessible in Wine's headers
add_compile_definitions(CINTERFACE)

# List all source files here
set(source_files
    battleparam.cpp
    common.cpp
    customize_menu.cpp
    earlywalk.cpp
    editors.cpp
    editors/TCameraEditor.cpp
    editors/TFreeCamera.cpp
    editors/TGroupEnemySetEditor.cpp
    editors/TGroupSetEditor.cpp
    editors/TParticleEditor.cpp
    editors/TQuestScriptChecker.cpp
    editors/TSetEvtScriptTest.cpp
    enemy.cpp
    entity.cpp
    entitylist.cpp
    fastwarp.cpp
    helpers.cpp
    hooking.cpp
    ime.cpp
    initlist.cpp
    keyboard.cpp
    large_assets.cpp
    map.cpp
    mathutil.cpp
    newenemy.cpp
    object_extension.cpp
    object_wrapper.cpp
    object.cpp
    omnispawn.cpp
    omnispawn_particles.cpp
    omnispawn_sound_effects.cpp
    palette.cpp
    patching.cpp
    psobb_functions.cpp
    shop.cpp
    slow_gibbles.cpp
    map_object.cpp
    newmap/newmap.cpp
    newmap/map_object_cloud.cpp
    newmap/map_object_snowfall.cpp
    newmap/map_object_newdoor.cpp
    newmap/slbgm.cpp
    newmap/snow_map.cpp
    newmap/fog.cpp
    newmap/sunlight.cpp
    newmap/maploader.cpp
    newmap/setdata.cpp
    newmap/enemies_and_objects.cpp
    newmap/sound_effects.cpp
    newmap/particle_effects.cpp
    newmap/enemy_icecube.cpp
    newmap/footsteps.cpp
    newmap/map_object_payload.cpp
    newmap/map_object_turret.cpp
    newmap/map_object_slippery.cpp
    newmap/map_object_snowwisp.cpp
    devmod_interface.h
    ninja.cpp
    quest.cpp
    player.cpp
    
    newgfx/animation.cpp
    newgfx/bone.cpp
    newgfx/mesh.cpp
    newgfx/model.cpp)

add_library(${PROJECT_NAME} SHARED dllmain.cpp psobb.cpp ${source_files})

# Link with assimp if using newgfx
if(USE_NEWGFX)
    target_link_libraries(${PROJECT_NAME} assimp)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Remove "lib" prefix from output file
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

# Enable warnings
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/devmod")

# Force 32-bit target
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -m32 LINK_FLAGS "-m32 -static -static-libgcc -static-libstdc++")

# Define DEBUG in debug mode
target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

# Enable WIN32_LEAN_AND_MEAN
target_compile_definitions(${PROJECT_NAME} PUBLIC "WIN32_LEAN_AND_MEAN")

if (NOT MSVC)
    # Use intel asm syntax
    add_compile_options(-masm=intel)
endif()

# Create a separate dll from files inside devmod dir
if (ENABLE_DEVMOD)
    file(GLOB devmod_source_files "${CMAKE_CURRENT_SOURCE_DIR}/devmod/*.cpp")
    add_library(devmod SHARED ${source_files} ${devmod_source_files})
    set_target_properties(devmod PROPERTIES PREFIX "")
    target_include_directories(devmod PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    set_target_properties(devmod PROPERTIES COMPILE_FLAGS -m32 LINK_FLAGS "-m32 -static -static-libgcc -static-libstdc++")
    target_compile_definitions(devmod PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
    target_compile_definitions(devmod PUBLIC "WIN32_LEAN_AND_MEAN")
endif()
