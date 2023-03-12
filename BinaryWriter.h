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

	std::string m_OutputName;

	std::unordered_map<std::string, char> m_TypeMap;

	bool m_UseCompactFaces;

	int CountVertices();

	void EncodeHeader();
	void InitializeTypeMap();
	void WriteHeader();

	void WriteNullTerminator();

	void WriteBody();

	void WriteBlock(const std::string& currentType, const std::vector<std::string>& blockData);

	void WriteVertexBlockInfo(const std::string& currentType, const std::vector<std::string>& blockData);
	void WriteVertexBlock(const std::vector<std::string>& blockData);

	void WriteCommentBlockInfo(const std::string& currentType, const std::vector<std::string>& blockData);
	void WriteCommentBlock(const std::vector<std::string>& blockData);

	void WriteFaceBlockInfo(const std::string& currentType, const std::vector<std::string>& blockData);
	void WriteFaceBlock(const std::vector<std::string>& blockData);
};

