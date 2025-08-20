-- premake5.lua
workspace "ThornEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

include "Thorn/Build-Thorn.lua"

group "Vendor"
	include "Vendor/Modules/glfw/Build.lua"
	include "Vendor/Modules/imgui/Build.lua"
group ""

include "Rose/Build-Rose.lua"