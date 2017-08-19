structure = require "structure"

include "boost.lua"
include "gmock.lua"

workspace "dormouse-engine"

	configurations { "DebugStatic", "ReleaseStatic" }
	
	platforms { "Win64" }
	
	language "C++"

	-- Configuration specific settings
	
	filter "configurations:Debug*"
		symbols "On"
		
	filter "configurations:Release*"
		defines { "NDEBUG" }
		optimize "On"
	
	-- Platform specific settings
	
	filter "platforms:Win32"
		system "Windows"
		architecture "x32"

	filter "platforms:Win64"
		system "Windows"
		architecture "x64"
		
	filter {}
	
	-- Global includes
	
	includedirs { BOOST_INCLUDE_DIR }
	
	-- Dependency libraries
	
	libdirs { BOOST_LIB_DIR }
	
	-- Warnings
	
	warnings "Extra"
	flags { "FatalWarnings" }
	
	filter "action:vs*"
		defines { "_SCL_SECURE_NO_WARNINGS" }
		buildoptions { "/std:c++latest" }
		defines { "_HAS_AUTO_PTR_ETC=1" } -- otherwise boost fails to build with c++latest
		defines { "GTEST_LANG_CXX11=1" } -- otherwise gmock fails to compile with c++latest
	filter {}

	include "foundation"
		
	structure.create_install_project()
	structure.create_run_tests_project()
	