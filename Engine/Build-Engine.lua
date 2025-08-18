project "Engine"
   kind "StaticLib"
   language "C++"
   cppdialect "C++latest"
   location "Source/Thorn"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/Thorn/**.h", "Source/Thorn/**.cpp", "../Vendor/Modules/glad/src/glad.c" }

   pchheader "Thorn/pch.h"
   pchsource "Source/Thorn/pch.cpp"

   includedirs
   {
      "Source",
      "../Vendor/Modules/glfw/include",
      "../Vendor/Modules/glad/include",
      "../Vendor/Modules/glm",
      "../Vendor/Modules/stb"
   }

   links
   {
       "GLFW"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter { "files:../Vendor/Modules/glad/src/glad.c" }
       flags { "NoPCH" }

   filter "system:windows"
       systemversion "latest"
       defines { }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"