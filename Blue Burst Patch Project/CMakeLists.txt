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
define_optional_patch(PATCH_SLOW_GIBBLES_FIX)
define_optional_patch(PATCH_UNSELLABLE_RARES)

# This makes the lpVtbl field of COM objects accessible in Wine's headers
add_compile_definitions(CINTERFACE)

# List all source files here
add_library(${PROJECT_NAME} SHARED
    battleparam.cpp
    common.cpp
    customize_menu.cpp
    dllmain.cpp
    earlywalk.cpp
    enemy.cpp
    entitylist.cpp
    fastwarp.cpp
    helpers.cpp
    ime.cpp
    initlist.cpp
    large_assets.cpp
    map.cpp
    mathutil.cpp
    newenemy.cpp
    object.cpp
    object_extension.cpp
    object_wrapper.cpp
    omnispawn.cpp
    palette.cpp
    pch.cpp
    psobb.cpp
    shop.cpp
    slow_gibbles.cpp
    
    newgfx/animation.cpp
    newgfx/bone.cpp
    newgfx/mesh.cpp
    newgfx/model.cpp)

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

# Force 32-bit target
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -m32 LINK_FLAGS -m32)