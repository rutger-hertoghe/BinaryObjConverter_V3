#include "BinaryReader.h"
#include "Structs.h"

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

BinaryReader::BinaryReader(const std::string& _inputFile, const std::string& _outputFile)
{
    const char nullTerminator{ 0 };

    // BACK TRANSLATION START
    std::ifstream fileToReinterpret{ _inputFile, std::ios::binary };
    std::ofstream backTranslation{ _outputFile };

    // DECODE HEADER
    // Read header to map
    std::unordered_map<char, std::string> charTypeMap;
    char currentTypeChar{ 1 };
    while (currentTypeChar != nullTerminator)
    {
        // Read type char
        char typeChar{};
        fileToReinterpret.read(&typeChar, sizeof(typeChar));
        if (typeChar == nullTerminator) break;

        // Read size of string
        char stringSize{};
        fileToReinterpret.read(&stringSize, sizeof(stringSize));
        std::string typeString{};
        // Ensure destination string has same length
        typeString.resize(stringSize);
        // Read string
        fileToReinterpret.read(typeString.data(), stringSize);

        // Add string to map
        charTypeMap[typeChar] = typeString;
    }

    // Verify map
    for (const auto& typePair : charTypeMap)
    {
        const auto typeChar{ typePair.first };
        const auto& typeString{ typePair.second };
        std::cout << static_cast<int>(typeChar) << " : " << typeString << '\n';
    }

    // READ DATA BLOCKS
    while (!fileToReinterpret.eof())
    {
        char type{};
        fileToReinterpret.read(&type, sizeof(type));
        size_t blockSize{};
        fileToReinterpret.read((char*)&blockSize, sizeof(blockSize));
        if (charTypeMap[type] == "v")
        {
            for (size_t i{ 0 }; i < blockSize; ++i)
            {
                Vertex v{};
                fileToReinterpret.read((char*)&v, sizeof(v));
                backTranslation << charTypeMap[type] << " " << v.x << " " << v.y << " " << v.z << '\n';
            }
        }
        else if (charTypeMap[type] == "f")
        {
            for (size_t i(0); i < blockSize; ++i)
            {
                Face f{};
                fileToReinterpret.read((char*)&f, sizeof(f));
                backTranslation << charTypeMap[type] << " " << f.v0 << " " << f.v1 << " " << f.v2 << '\n';
            }
        }
        // Verify end of block
        char blockTerminator{};
        fileToReinterpret.read(&blockTerminator, sizeof(blockTerminator));
        if (blockTerminator != 0)
        {
            std::cout << "block was not terminated by null terminator!\n";
            return;
        }
    }
}
