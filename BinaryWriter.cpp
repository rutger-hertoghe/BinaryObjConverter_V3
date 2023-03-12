#include "BinaryWriter.h"
#include "Structs.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

BinaryWriter::BinaryWriter(const std::string& inputFile, const std::string& outputFile)
    : m_InputFile(inputFile)
    , m_OutputFile(outputFile, std::ios::binary)
{
    if (!m_InputFile.is_open())
    {
        std::cout << "Input file not found!\n";
        return;
    }
}

void BinaryWriter::WriteBobj()
{
    EncodeHeader();
    WriteBody();
    m_OutputFile.close();
}

void BinaryWriter::EncodeHeader()
{
    InitializeTypeMap();
    WriteHeader();
    WriteNullTerminator();
}

void BinaryWriter::InitializeTypeMap()
{
    char typeIdx{ 1 };

    // Read through the file and add the first string find for each line to the typemap
    std::string readLine;
    while (std::getline(m_InputFile, readLine))
    {
        std::stringstream line{ readLine };
        std::string type;
        line >> type;
        auto it{ m_TypeMap.find(type) };
        if (it == m_TypeMap.end())
        {
            m_TypeMap[type] = typeIdx++;
        }
    }

    // Go back to the beginning of the file
    m_InputFile.clear();
    m_InputFile.seekg(0);
}

void BinaryWriter::WriteHeader()
{
    // NOTE: if more than 255 "types" were detected in the file, this code will break. This should not be the case for obj files.
    for (const auto& typePair : m_TypeMap)
    {
        const auto& typeString{ typePair.first };
        const auto& typeChar{ typePair.second };
        // Write char associated with type string
        m_OutputFile.write(&typeChar, sizeof(typeChar));
        // Write length of type string
        char stringSize{ static_cast<char>(typeString.size()) };
        m_OutputFile.write(&stringSize, sizeof(stringSize));
        // Write contents of string
        m_OutputFile.write(typeString.data(), stringSize);
    }
}

void BinaryWriter::WriteNullTerminator()
{
    static const char nullTerminator{ 0 };
    m_OutputFile.write(&nullTerminator, sizeof(nullTerminator));
}

void BinaryWriter::WriteBody()
{
	// COMPOSE DATA BODY
    std::string currentLine;
    std::string currentType;
    std::vector<std::string> blockData;

    while (std::getline(m_InputFile, currentLine))
    {
        // Get type of new line
        std::stringstream line{ currentLine };
        std::string lineType;
        line >> lineType;

        // If line of different type, write block
        if (lineType != currentType && !blockData.empty())
        {
            WriteBlock(currentType, blockData);
            blockData.clear();
            currentType = lineType;
        }
        blockData.push_back(line.str());
    }

    // Write final block
    if (!blockData.empty())
    {
        WriteBlock(currentType, blockData);
        blockData.clear();
    }
}

void BinaryWriter::WriteBlock(const std::string& currentType, const std::vector<std::string>& blockData)
{
    if (currentType == "v" || currentType == "f")
    {
        WriteDataBlockInfo(currentType, blockData);
    }

    // Write block
    if (currentType == "v")
    {
        WriteVertexBlock(blockData);
        WriteNullTerminator();
    }
    else if (currentType == "f")
    {
        WriteFaceBlock(blockData);
        WriteNullTerminator();
    }

}

void BinaryWriter::WriteDataBlockInfo(const std::string& currentType, const std::vector<std::string>& blockData)
{
    // Write type
    char type{ m_TypeMap[currentType] };
    m_OutputFile.write(&type, sizeof(type));

    // Write blocklength (block.size() * relevant fields)
    size_t blockLength{ blockData.size() };
    m_OutputFile.write((const char*)&blockLength, sizeof(blockLength));
}

void BinaryWriter::WriteVertexBlock(const std::vector<std::string>& blockData)
{
    for (const auto& vString : blockData)
    {
        std::stringstream vertexString{ vString };
        std::string type;
        Vertex v;
        vertexString >> type >> v.x >> v.y >> v.z;
        m_OutputFile.write((const char*)&v, sizeof(v));
    }
}

void BinaryWriter::WriteFaceBlock(const std::vector<std::string>& blockData)
{
    for (const auto& fString : blockData)
    {
        std::stringstream faceString{ fString };
        std::string type;
        Face f;
        faceString >> type >> f.v0 >> f.v1 >> f.v2;
        m_OutputFile.write((const char*)&f, sizeof(f));
    }
}
