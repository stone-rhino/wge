#pragma once

struct Response {
  std::unordered_multimap<std::string, std::string> headers_{
      {"content-type", "text/html; charset=UTF-8"},
      {"content-length", "11"},
      {"set-cookie", "c1=v1;c2=v2"},
      {"set-cookie", "c3=v4"}};

  std::vector<std::string_view> body_{{"hello world"}};
};