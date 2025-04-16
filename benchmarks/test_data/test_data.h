#pragma once

#include <vector>
#include <string>

#include "http_info.h"

class TestData {
public:
  enum class Type {
    White,
    Black,
  };
public:
  TestData(Type type);

public:
  const std::vector<HttpInfo>& getHttpInfos() const { return http_infos_; }

private:
  std::vector<HttpInfo> http_infos_;
  std::string buffer_;
};