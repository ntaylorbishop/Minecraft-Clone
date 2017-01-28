#include "Engine/FileUtils.hpp"

bool LoadBinaryFileToBuffer(const std::string& filePath, std::vector<unsigned char>& out_buffer) {
	FILE* file;
	long size;

	int wasSuccesful = fopen_s(&file, filePath.c_str(), "rb");

	if (wasSuccesful == 0) {
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		rewind(file);
		out_buffer.resize(size);
		fread(&out_buffer[0], sizeof(unsigned char), size, file);
		fclose(file);

		return true;
	}
	else
		return false;
}
bool SaveBufferToBinaryFile(const std::string& filePath, const std::vector<unsigned char>& buffer) {
	FILE* file;

	int wasSuccessful = fopen_s(&file, filePath.c_str(), "wb");

	if (wasSuccessful == 0) {
		fwrite(&buffer[0], 1, buffer.size(), file);
		fclose(file);

		return true;
	}
	else
		return false;
}