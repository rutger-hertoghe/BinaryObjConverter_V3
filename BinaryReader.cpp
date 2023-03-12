#include "BinaryReader.h"
#include "Structs.h"

#include <iostream>
#include <string>

BinaryReader::BinaryReader(const std::string& inputFile, const std::string& outputFile)
	: m_InputFile{inputFile, std::ios::binary}
	, m_OutputFile{outputFile}
{
    if (!m_InputFile.is_open())
    {
        std::cout << "Input file not found!\n";
        return;
    }
    DecodeHeader();
    VerifyHeader();
}

void BinaryReader::DecodeHeader()
{
    constexpr char nullTerminator{ 0 };
    //  1. Read in type char
    //  2. Read in length of associated type string
    //  3. Read in type string
    // Do this as long as header terminator is not found as type char


    while (m_InputFile.good())
    {
        char typeChar{};
        m_InputFile.read(&typeChar, sizeof(typeChar));

        if (typeChar == nullTerminator)
        {
            break;
        }

        char stringSize{};
        m_InputFile.read(&stringSize, sizeof(stringSize));

        // Resizing required to ensure string contains correct information
        std::string typeString{};
        typeString.resize(stringSize);
        m_InputFile.read(typeString.data(), stringSize);

        m_TypeMap[typeChar] = typeString;
    }

}

void BinaryReader::VerifyHeader()
{
    for (const auto& typePair : m_TypeMap)
    {
        const auto typeChar{ typePair.first };
        const auto& typeString{ typePair.second };
        std::cout << static_cast<int>(typeChar) << " : " << typeString << '\n';
    }
}

void BinaryReader::WriteObj()
{
    // READ DATA BLOCKS
    while (!m_InputFile.eof())
    {
        char type{};
        m_InputFile.read(&type, sizeof(type));

        if (m_TypeMap[type] == "v")
        {
            size_t blockSize{};
            m_InputFile.read((char*)&blockSize, sizeof(blockSize));
            for (size_t i{ 0 }; i < blockSize; ++i)
            {
                WriteVertex();
            }
        }
        else if (m_TypeMap[type] == "vn")
        {
            size_t blockSize{};
            m_InputFile.read((char*)&blockSize, sizeof(blockSize));
            for (size_t i{ 0 }; i < blockSize; ++i)
            {
                WriteNormal();
            }
        }
        else if (m_TypeMap[type] == "f")
        {
            bool useCompactFaces;
            m_InputFile.read((char*)&useCompactFaces, sizeof(useCompactFaces));

            size_t blockSize{};
            m_InputFile.read((char*)&blockSize, sizeof(blockSize));

            for (size_t i(0); i < blockSize; ++i)
            {
                WriteFace(useCompactFaces);
            }
        }
        else if(m_TypeMap[type] == "#")
        {
	        
        }

        // Verify end of block
        char blockTerminator{};
        m_InputFile.read(&blockTerminator, sizeof(blockTerminator));
        if (blockTerminator != 0)
        {
            std::cout << "block was not terminated by null terminator!\n";
            return;
        }
    }
}

void BinaryReader::WriteVertex()
{
    Vertex v{};
    m_InputFile.read((char*)&v, sizeof(v));
    m_OutputFile << "v " << v.x << " " << v.y << " " << v.z << '\n';
}

void BinaryReader::WriteNormal()
{
    Vertex v{};
    m_InputFile.read((char*)&v, sizeof(v));
    m_OutputFile << "vn " << v.x << " " << v.y << " " << v.z << '\n';
}

void BinaryReader::WriteFace(bool isCompact)
{
    if(isCompact)
    {
        CompactFace f{};
        m_InputFile.read((char*)&f, sizeof(f));
        m_OutputFile << "f " << f.v0 << " " << f.v1 << " " << f.v2 << '\n';
    }
    else
    {
        Face f{};
        m_InputFile.read((char*)&f, sizeof(f));
        m_OutputFile << "f " << f.v0 << " " << f.v1 << " " << f.v2 << '\n';
    }
}
