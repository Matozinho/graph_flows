#pragma once

#include <benchmark/benchmark.h>

#include <utils/bench.hpp>

#include "ford_fulkerson.cpp"
#include "push_relabel.cpp"

BENCHMARK(BM_Ford_Fulkerson)->Apply(utils::bench::custom_arguments)->Complexity();
BENCHMARK(BM_Push_Relable)->Apply(utils::bench::custom_arguments)->Complexity();

BENCHMARK_MAIN();
