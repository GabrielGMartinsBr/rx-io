#pragma once

#include <fstream>
#include <sstream>
#include <string>

struct FileIO {
  static std::string readFile(const std::string& filePath)
  {
    std::ifstream file(filePath, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
      throw std::ios_base::failure("Failed to open the file.");
    }

    std::ostringstream content;
    constexpr std::streamsize chunkSize = 4096;
    char buffer[chunkSize];

    while (!file.eof()) {
      file.read(buffer, chunkSize);
      content.write(buffer, file.gcount());
    }

    file.close();

    return content.str();
  }
};