include "directx-tex.lua"

structure.library_project(
	"graphics",
	function()
		includedirs { directx_tex_include_dir() }
		links { "exceptions", "system" }
		link_directx_tex_libs()
	end
	)

project "graphics-unit-test"
	add_gmock_files()
