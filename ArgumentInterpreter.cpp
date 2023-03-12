#include "ArgumentInterpreter.h"

#include <iostream>
#include <sstream>
#include <fstream>

ArgumentInterpreter::ArgumentInterpreter(int argc, char* argv[])
{
    for (int i{ 0 }; i < argc; ++i)
    {
        std::string argument{ argv[i] };
        int nextArgIdx{ i + 1 };
        if (argument == "-i" && nextArgIdx < argc)
        {
            m_InputFile = argv[nextArgIdx];
        }

        if (argument == "-o" && nextArgIdx)
        {
            m_OutputFile = argv[nextArgIdx];
        }
    }
}

bool ArgumentInterpreter::IsInputValid()
{
    if(m_InputFile.empty())
    {
        return false;
    }
    
    m_InputExtension = ParseExtension(m_InputFile);
    if(m_InputExtension != "obj" && m_InputExtension != "bobj")
    {
        return false;
    }

    std::ifstream inputFile{ m_InputFile };
    if(!inputFile.is_open())
    {
        inputFile.close();
        return false;
    }

    return true;
}

bool ArgumentInterpreter::IsOutputValid()
{
    if(m_OutputFile.empty())
    {
        std::cout << "No output file specified! Using ";
        if(m_InputExtension == "obj")
        {
            m_OutputFile = "outputFile.bobj";
        }
        else
        {
            m_OutputFile = "outputFile.obj";
        }
        std::cout << m_OutputFile << '\n';
    }

    m_OutputExtension = ParseExtension(m_OutputFile);

    if(m_OutputExtension == m_InputExtension)
    {
        return false;
    }

    if(m_OutputExtension !=  "obj" && m_OutputExtension != "bobj")
    {
        return false;
    }

    return true;
}

const std::string& ArgumentInterpreter::GetInputFile()
{
    return m_InputFile;
}

const std::string& ArgumentInterpreter::GetOutputFile()
{
    return m_OutputFile;
}

const std::string& ArgumentInterpreter::GetInputExtension()
{
    return m_InputExtension;
}

std::string ArgumentInterpreter::ParseExtension(const std::string& file)
{
    std::string filename, extension;
    std::stringstream fileSS{ file };
    std::getline(fileSS, filename, '.');
    std::getline(fileSS, extension);

    return extension;
}
