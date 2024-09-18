set_project("text")

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

target("text", function()
    set_kind("$(kind)")

    add_includedirs("include", { public = true })
    add_headerfiles("include/text/*.hpp", "include/text/**/*.hpp")

    add_files("src/*.cpp", "src/**/*.cpp")
    add_files("src/*.cppm", "src/**/*.cppm", { public = true })

    set_configdir("$(buildir)/$(plat)/$(arch)/$(mode)")
    add_configfiles("text.config.cppm.in")
    add_files("$(buildir)/$(plat)/$(arch)/$(mode)/text.*.cppm", { public = true })

    if is_plat("windows") then
        add_defines("WIN32", "_WIN32")

        --if is_kind("shared") then
        --    add_rules("utils.symbols.export_all", { export_classes = true })
        --end
    end

    if is_kind("shared") then
        add_defines("TEXT_SHARED_LIB", "TEXT_EXPORTS", { public = true })
    end

    add_deps("common", { public = true, configs = { shared = true } })

    after_build(function(target)

    end)
end)