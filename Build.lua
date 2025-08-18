-- premake5.lua
workspace "Thorn"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

include "Engine/Build-Engine.lua"

group "Vendor"
	include "Vendor/Modules/glfw/Build.lua"
group ""

include "Editor/Build-Editor.lua"