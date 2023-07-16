#pragma once

#include <benchmark/benchmark.h>
#include <fmt/core.h>

#include <string>
#include <utils/assets.hpp>

namespace utils::bench {
  static std::string filePath = "assets";

  static void custom_arguments(benchmark::internal::Benchmark *b) {
    auto elements = utils::assets::get_size_of_assets(fmt::format("{}", filePath));

    for (auto size : elements) b->Args({size});
  }
}  // namespace utils::bench
