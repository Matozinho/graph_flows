#pragma once

#include <fmt/core.h>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <graph/graph.hpp>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>
#include <vector>
#include "graph/push_relabel_graph.hpp"

namespace utils::assets {
  inline std::vector<int32_t> get_size_of_assets(const std::string& path) {
    std::vector<int32_t> sizes;

    // Read the name of the files from path
    std::filesystem::path p(path);
    for (const auto& entry : std::filesystem::directory_iterator(p)) {
      // Extract only the number from the name of the file
      std::string name = entry.path().filename().string();
      std::regex re("[0-9]+");
      std::smatch match;
      std::regex_search(name, match, re);

      if (match.empty()) continue;

      // Convert the number to int32_t and add it to the vector
      int32_t size = std::stoi(match[0]);
      sizes.push_back(size);
    }

    std::ranges::sort(sizes);

    return sizes;
  }

  inline Graph* get_graph_from_file(std::string fileName) {
    std::ifstream inputFile(fileName);

    if (!inputFile.is_open()) throw std::runtime_error("Unable to open file");

    std::string graph_size;
    getline(inputFile, graph_size);

    std::string line;
    std::vector<std::string> numbers;

    while (getline(inputFile, line)) {
      numbers.push_back(line);
    }

    inputFile.close();

    // Create a graph with the size read from the file
    Graph* g = new Graph(std::stoi(graph_size));

    for (int i = 0; i < numbers.size(); i++) {
      std::istringstream iss(numbers[i]);
      std::string number;
      int j = 0;

      while (iss >> number) {
        int value = std::stoi(number);
        if (value > 0) {
          g->addEdge(i, j, value);
        }
        j++;
      }
    }

    return g;
  }

  inline PushRelabelGraph* get_push_relabel_graph_from_file(std::string fileName) {
    std::ifstream inputFile(fileName);

    if (!inputFile.is_open()) throw std::runtime_error("Unable to open file");

    std::string graph_size;
    getline(inputFile, graph_size);

    std::string line;
    std::vector<std::string> numbers;

    while (getline(inputFile, line)) {
      numbers.push_back(line);
    }

    inputFile.close();

    // Create a graph with the size read from the file
    PushRelabelGraph* g = new PushRelabelGraph(std::stoi(graph_size));

    for (int i = 0; i < numbers.size(); i++) {
      std::istringstream iss(numbers[i]);
      std::string number;
      int j = 0;

      while (iss >> number) {
        int value = std::stoi(number);
        if (value > 0) {
          g->addEdge(i, j, value);
        }
        j++;
      }
    }

    return g;
  }

  template <typename R, typename... Args> std::function<R(Args...)> memo(R (*fn)(Args...)) {
    std::map<std::tuple<Args...>, R> table;
    return [fn, table](Args... args) mutable -> R {
      auto argt = std::make_tuple(args...);
      auto memoized = table.find(argt);
      if (memoized == table.end()) {
        auto result = fn(args...);
        table[argt] = result;
        return result;
      } else {
        return memoized->second;
      }
    };
  }

  // inline std::vector<int32_t> get_params_from_file(std::string fileName) {
  //   std::ifstream inputFile(fileName);
  //
  //   if (!inputFile.is_open()) throw std::runtime_error("Unable to open file");
  //
  //   std::string line;
  //   std::vector<int32_t> numbers;
  //
  //   while (getline(inputFile, line)) {
  //     std::istringstream lineStream(line);
  //     int number;
  //     while (lineStream >> number) {
  //       numbers.push_back(number);
  //     }
  //   }
  //
  //   inputFile.close();
  //
  //   return numbers;
  // }
}  // namespace utils::assets
