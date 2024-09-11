set_project("jsengine")

-- version
set_version("0.0.1", { build = "%Y%m%d%H%M" })

-- set xmake min version
set_xmakever("2.9.3")

-- set warning all as error
--set_warnings("all", "error")

set_languages("c99", "c++20")

add_rules("mode.debug", "mode.release", "mode.valgrind")

if is_mode("release") then
    --set_optimize("smallest")
    if is_plat("windows") then
        add_ldflags("/LTCG")
    end
end

add_requires("quickjs", { configs = { shared = true } })

target("jsengine", function()
    set_kind("$(kind)")

    add_includedirs("include", { public = true })
    add_headerfiles("include/jsengine/*.hpp", "include/jsengine/**/*.hpp")

    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)")
    add_configfiles("jsengine.config.h.in")
    add_includedirs("$(buildir)/$(plat)/$(arch)/$(mode)", { public = true })

    add_files("src/*.cpp", "src/**/*.cpp")
    add_files("include/jsengine/*.ixx", "include/jsengine/**/*.ixx", { public = true })

    if is_plat("windows") then
        add_defines("WIN32", "_WIN32")

        --if is_kind("shared") then
        --    add_defines("DLLEXPORT")
        --    add_rules("utils.symbols.export_all", { export_classes = true })
        --end
    end

    if is_kind("shared") then
        add_defines("JSENGINE_SHARED_LIB", "JSENGINE_EXPORTS", { public = true })
    end

    add_packages("quickjs", { public = true })

    add_deps("common", { public = true, configs = { shared = true } })

    after_build(function(target)

    end)
end)