#pragma once
#include <string>

class ArgumentInterpreter
{
public:
	explicit ArgumentInterpreter(int argc, char* argv[]);

	bool IsInputValid();
	bool IsOutputValid();

	const std::string& GetInputFile();
	const std::string& GetOutputFile();

	const std::string& GetInputExtension();

private:
	std::string m_InputFile;
	std::string m_OutputFile;

	std::string m_InputExtension;
	std::string m_OutputExtension;

	std::string ParseExtension(const std::string& file);
};

