#pragma once

#include <vector>
#include <string>

bool LoadBinaryFileToBuffer(const std::string& filePath, std::vector<unsigned char>& out_buffer);
bool SaveBufferToBinaryFile(const std::string& filePath, const std::vector<unsigned char>& buffer);
