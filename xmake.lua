set_project("ShadowEditor")

-- version
set_version("0.0.1", { build = "%Y%m%d%H%M" })

-- set xmake min version
set_xmakever("2.9.3")

-- set warning all as error
--set_warnings("all", "error")

set_languages("c99", "c++20")

set_encodings("utf-8")

includes("shadow/**/xmake.lua", "lib/**/xmake.lua")
