#pragma once
#include <string>
#include <fstream>
#include <unordered_map>

class BinaryWriter final
{
public:
	explicit BinaryWriter(const std::string& inputFile, const std::string& outputFile);

	void WriteBobj();

private:
	std::ifstream m_InputFile;
	std::ofstream m_OutputFile;

	std::unordered_map<std::string, char> m_TypeMap;
	void EncodeHeader();
	void InitializeTypeMap();
	void WriteHeader();

	void WriteNullTerminator();

	void WriteBody();

	void WriteBlock(const std::string& currentType, const std::vector<std::string>& blockData);
	void WriteDataBlockInfo(const std::string& currentType, const std::vector<std::string>& blockData);
	void WriteVertexBlock(const std::vector<std::string>& blockData);
	void WriteFaceBlock(const std::vector<std::string>& blockData);
};

