set_project("graphs-flow")
set_version("1.0.0")
set_xmakever("2.7.0")

set_languages("c++20")
add_rules("mode.debug", "mode.release")

local libs = { "fmt", "gtest", "benchmark" }

add_includedirs("include")
add_requires(table.unpack(libs))

target("graphs-flow-lib")
  set_kind("static")
  add_files("src/**/*.cpp")
  add_packages(table.unpack(libs))

target("graphs-flow")
  set_kind("binary")
  add_files("standalone/main.cpp")
  add_packages(table.unpack(libs))
  add_deps("graphs-flow-lib")

target("test")
  set_kind("binary")
  add_files("test/*.cpp")
  add_packages(table.unpack(libs))
  add_deps("graphs-flow-lib")

target("bench")
  set_kind("binary")
  add_files("benchmark/main.cpp")
  add_deps("graphs-flow-lib")
  add_packages(table.unpack(libs))

add_installfiles("(include/**)", {prefixdir = ""})

-- Post build copy assets folder
after_build(function(target)
    os.cp("assets", target:targetdir())
end)
