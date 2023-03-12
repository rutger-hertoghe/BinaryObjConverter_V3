#pragma once
#include <string>
#include <fstream>
#include <unordered_map>

class BinaryWriter final
{
public:
	explicit BinaryWriter(const std::string& inputFile, const std::string& outputFile);

private:
	std::ifstream m_InputFile;
	std::ofstream m_OutputFile;

	std::unordered_map<std::string, char> m_TypeMap;
};

