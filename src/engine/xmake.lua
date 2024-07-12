set_project("Engine")

add_configfiles("config.h.in")

-- version
set_version("0.0.1", { build = "%Y%m%d%H%M" })

-- set xmake min version
set_xmakever("2.9.3")

-- set warning all as error
--set_warnings("all", "error")

set_languages("c99", "c++20")

add_rules("mode.debug", "mode.release")

if is_mode("release") then
    set_optimize("smallest")
    if is_plat("windows") then
        add_ldflags("/LTCG")
    end
end

add_requires("spdlog")

target("Engine", function()
    set_kind("binary")

    includes("lib/**/xmake.lua")

    add_includedirs("include")

    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)")
    add_configfiles("engine.config.h.in")
    add_includedirs("$(buildir)/$(plat)/$(arch)/$(mode)", { public = true })

    add_files("src/*.cpp", "src/*.cppm")

    add_defines("SHARED_LIB")
    if is_plat("windows") then
        add_defines("WIN32", "_WIN32", "DLLEXPORT")
    end

    add_packages("spdlog")

    add_deps("BehaviorTree")
    add_deps("JSEngine", { public = true })

    after_build(function(target)

    end)
end)
