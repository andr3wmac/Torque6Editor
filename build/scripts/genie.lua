solution "Torque6Editor"
    -- Settings
    BUILD_DIR           = "../bin"
    PROJECT_DIR         = "../" .. _ACTION .. "/"
    LIB_DIR             = "../../engine/lib/"
    LIB_PROJECT_DIR     = PROJECT_DIR .. "lib"
    LIB_BUILD_DIR       = PROJECT_DIR .. "lib/bin"
    SRC_DIR             = "../../engine/src/"
    PLUGIN_DIR          = "../../engine/plugins/"
    PLUGIN_BUILD_DIR    = "../../engine/projects/shared-modules/"
    PROJECT             = "../../engine/projects/00-RollTheBall/"
    EDITOR_DIR          = "../../"

    -- Solution
    configurations {
        "Release",
        "Debug"
    }
    platforms {
        "x32",
        "x64",
    }
    location (PROJECT_DIR)

    -- Torque 6 App
    dofile ("../../engine/build/scripts/Torque6App.lua")
    Torque6App()

    -- Torque 6 Editor ( Optional )
    dofile ("Torque6Editor.lua")
    Torque6Editor()
    startproject "Torque6Editor"

    -- Torque 6 Launcher ( Optional )
    dofile ("Torque6Launcher.lua")
    Torque6Launcher()

    -- Torque6 DLL
    dofile ("../../engine/build/scripts/Torque6.lua")
    Torque6()
    
    -- Libraries  
    group "Libraries"
    dofile ("../../engine/build/scripts/assimp.lua")
    assimp()
    dofile ("../../engine/build/scripts/bgfx.lua")
    bgfx()
    dofile ("../../engine/build/scripts/bullet.lua")
    bullet()
    dofile ("../../engine/build/scripts/ljpeg.lua")
    ljpeg()
    dofile ("../../engine/build/scripts/lpng.lua")
    lpng()
    dofile ("../../engine/build/scripts/zlib.lua")
    zlib()

    -- Plugins
    group "Plugins"
    dofile ("../../engine/build/scripts/Plugins.lua")

    -- This loads plugins by folder name in the plugins directory.
    -- The name must match a module in the PLUGIN_DIR
    torque6Plugin("Sky")
    torque6Plugin("Terrain")
    torque6Plugin("Particles")
    torque6Plugin("CSG")
