#include "test_data.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include <parser.h>

TestData::TestData(Type type) {
  std::string white_data_path =
      std::filesystem::exists("white.data") ? "white.data" : "benchmarks/test_data/white.data";
  std::string black_data_path =
      std::filesystem::exists("black.data") ? "black.data" : "benchmarks/test_data/black.data";

  std::ifstream ifs(type == Type::White ? white_data_path : black_data_path);
  if (!ifs.is_open()) {
    std::cerr << "Failed to open file: "
              << (type == Type::White ? white_data_path : black_data_path) << std::endl;
    exit(1);
  }

  ifs.seekg(0, std::ios::end);
  size_t file_size = ifs.tellg();
  ifs.seekg(0, std::ios::beg);
  buffer_.resize(file_size);
  ifs.read(buffer_.data(), file_size);
  ifs.close();

  parse(buffer_, http_infos_);
}