function Torque6Launcher()
    project "Torque6Launcher"
        targetname "Torque6Launcher"
        language "C++"
        kind "WindowedApp"
        debugdir (EDITOR_DIR)
        targetdir (BUILD_DIR)

        includedirs {
            SRC_DIR,
            EDITOR_DIR,
            path.join(LIB_DIR, "assimp/include"),
            path.join(LIB_DIR, "bgfx/include"),
            path.join(LIB_DIR, "bgfx/common"),
            path.join(LIB_DIR, "openal/win32"),
        }

        files {
            path.join(EDITOR_DIR, "launcher/**.cpp"),
            path.join(EDITOR_DIR, "launcher/**.h"),
        }

        links {
            "Torque6"
        }

        defines {
            "TORQUE_PLUGIN"
        }

        configuration "vs2015"
            windowstargetplatformversion "10.0.10240.0"

        configuration "Release"
            defines     { "TORQUE_ENABLE_PROFILER" }

        configuration "Debug"
            targetname "Torque6Launcher_DEBUG"
            defines     { "TORQUE_DEBUG", "TORQUE_ENABLE_PROFILER" }
            flags       { "Symbols" }

        configuration "vs*"
            defines     { "_CRT_SECURE_NO_WARNINGS" }

        configuration "windows"
            flags { "WinMain" }
            targetdir   "../bin/windows"
            links { "ole32",
                    "winmm",
                    "comctl32",
                    "rpcrt4",
                    "wsock32",
                    "wininet", 
                  }
            includedirs { path.join(EDITOR_DIR, "wxWidgets/include"),
                          path.join(EDITOR_DIR, "wxWidgets/src/stc/scintilla/include"),
                          path.join(EDITOR_DIR, "wxWidgets/src/stc/scintilla/lexlib"),
                          path.join(EDITOR_DIR, "wxWidgets/src/stc/scintilla/src"),
                        }
            
        configuration { "windows", "x32", "Release" }
            includedirs { path.join(EDITOR_DIR, "wxWidgets/lib/vc_lib/mswu") }
            libdirs { path.join(EDITOR_DIR, "wxWidgets/lib/vc_lib") }
            targetdir (BUILD_DIR .. "/windows.x32.release")

        configuration { "windows", "x32", "Debug" }
            includedirs { path.join(EDITOR_DIR, "wxWidgets/lib/vc_lib/mswu") }
            libdirs { path.join(EDITOR_DIR, "wxWidgets/lib/vc_lib") }
            targetdir (BUILD_DIR .. "/windows.x32.debug")

        configuration { "windows", "x64", "Release" }
            includedirs { path.join(EDITOR_DIR, "wxWidgets/lib/vc_lib/mswu") }
            libdirs { path.join(EDITOR_DIR, "wxWidgets/lib/vc_x64_lib") }
            targetdir (BUILD_DIR .. "/windows.x64.release")

        configuration { "windows", "x64", "Debug" }
            includedirs { path.join(EDITOR_DIR, "wxWidgets/lib/vc_lib/mswud") }
            libdirs { path.join(EDITOR_DIR, "wxWidgets/lib/vc_x64_lib") }
            targetdir (BUILD_DIR .. "/windows.x64.debug")

        configuration { "windows", "Release" }
            links { "wxmsw31u_adv",
                    "wxmsw31u_aui",
                    "wxmsw31u_core",
                    "wxmsw31u_propgrid",
                    "wxmsw31u_stc",
                    "wxmsw31u_webview",
                    "wxbase31u",
                    "wxbase31u_net",
                    "wxscintilla",
                    "wxtiff",
                    "wxjpeg",
                    "wxpng",
                    "wxzlib",
                    "wxregexu",
                    "wxexpat",
                  }

        configuration { "windows", "Debug" }
            links { "wxmsw31ud_adv",
                    "wxmsw31ud_aui",
                    "wxmsw31ud_core",
                    "wxmsw31ud_propgrid",
                    "wxmsw31ud_stc",
                    "wxmsw31ud_webview",
                    "wxbase31ud",
                    "wxbase31ud_net",
                    "wxscintillad",
                    "wxtiffd",
                    "wxjpegd",
                    "wxpngd",
                    "wxzlibd",
                    "wxregexud",
                    "wxexpatd",
                  }

        configuration "linux"
            targetdir   "../bin/linux"
            links       { "dl" }
            linkoptions { "-rdynamic" }

        configuration "bsd"
            targetdir   "../bin/bsd"

        configuration "linux or bsd"
            links       { "m" }
            linkoptions { "-rdynamic" }

        configuration "macosx"
            targetdir   "../bin/darwin"
            links       { "CoreServices.framework" }

        configuration { "macosx", "gmake" }
            buildoptions { "-mmacosx-version-min=10.4" }
            linkoptions  { "-mmacosx-version-min=10.4" }
end
