#include "BinaryWriter.h"
#include "Structs.h"

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

BinaryWriter::BinaryWriter(const std::string& _inputFile, const std::string& _outputFile)
{
    std::ifstream inputFile{ _inputFile };
    std::ofstream outputFile{ _outputFile, std::ios::binary };

    if (!inputFile.is_open())
    {
        std::cout << "Input file not found!\n";
        return;
    }

    // COMPOSE BINARY HEADER
    // Set up map containing strings and an index for their type
    std::unordered_map<std::string, char> typeMap;
    char typeIdx{ 1 };

    // Read through the file and add the first string find for each line to the typemap
    std::string readLine;
    while (std::getline(inputFile, readLine))
    {
        std::stringstream line{ readLine };
        std::string type;
        line >> type;
        auto it{ typeMap.find(type) };
        if (it == typeMap.end())
        {
            typeMap[type] = typeIdx++;
        }
    }
    // Go back to the beginning
    inputFile.clear();
    inputFile.seekg(0);

    // NOTE: if more than 255 "types" were detected in the file, this code will break. This should not be the case for obj files.
    // Write header
    for (const auto& typePair : typeMap)
    {
        const auto& typeString{ typePair.first };
        const auto& typeChar{ typePair.second };
        // Write char associated with type string
        outputFile.write(&typeChar, sizeof(typeChar));
        // Write length of type string
        char stringSize{ static_cast<char>(typeString.size()) };
        outputFile.write(&stringSize, sizeof(stringSize));
        // Write contents of string
        outputFile.write(typeString.data(), stringSize);
    }
    // Write null terminator to denote end of header
    const char nullTerminator{ 0 };
    outputFile.write(&nullTerminator, sizeof(nullTerminator));

    // COMPOSE DATA BODY
    std::string currentLine;
    std::string currentType;
    std::vector<std::string> blockData;
    while (std::getline(inputFile, currentLine))
    {
        // Get type of new line
        std::stringstream line{ currentLine };
        std::string lineType;
        line >> lineType;

        // If line of different type, write block
        if (lineType != currentType && !blockData.empty())
        {
            if (currentType == "v" || currentType == "f")
            {
                // Write type
                char type{ typeMap[currentType] };
                outputFile.write(&type, sizeof(type));
                // Write blocklength (block.size() * relevant fields)
                size_t blockLength{ blockData.size() };
                outputFile.write((const char*)&blockLength, sizeof(blockLength));
            }

            // Write block
            if (currentType == "v")
            {
                for (const auto& vString : blockData)
                {
                    std::stringstream vertexString{ vString };
                    std::string type;
                    Vertex v;
                    vertexString >> type >> v.x >> v.y >> v.z;
                    outputFile.write((const char*)&v, sizeof(v));
                }
                outputFile.write(&nullTerminator, sizeof(nullTerminator));
            }
            else if (currentType == "f")
            {
                for (const auto& fString : blockData)
                {
                    std::stringstream faceString{ fString };
                    std::string type;
                    Face f;
                    faceString >> type >> f.v0 >> f.v1 >> f.v2;
                    outputFile.write((const char*)&f, sizeof(f));
                }
                outputFile.write(&nullTerminator, sizeof(nullTerminator));
            }

            // Clear blockData
            blockData.clear();

            // Set current type to line type
            currentType = lineType;
        }
        blockData.push_back(line.str());
    }

    if (!blockData.empty())
    {
        if (currentType == "v" || currentType == "f")
        {
            // Write type
            char type{ typeMap[currentType] };
            outputFile.write(&type, sizeof(type));
            // Write blocklength (block.size() * relevant fields)
            size_t blockLength{ blockData.size() };
            outputFile.write((const char*)&blockLength, sizeof(blockLength));
        }

        // Write block
        if (currentType == "v")
        {
            for (const auto& vString : blockData)
            {
                std::stringstream vertexString{ vString };
                std::string type;
                Vertex v;
                vertexString >> type >> v.x >> v.y >> v.z;
                outputFile.write((const char*)&v, sizeof(v));
            }
            outputFile.write(&nullTerminator, sizeof(nullTerminator));
        }
        else if (currentType == "f")
        {
            for (const auto& fString : blockData)
            {
                std::stringstream faceString{ fString };
                std::string type;
                Face f;
                faceString >> type >> f.v0 >> f.v1 >> f.v2;
                outputFile.write((const char*)&f, sizeof(f));
            }
            outputFile.write(&nullTerminator, sizeof(nullTerminator));
        }

        // Clear blockData
        blockData.clear();
    }

    outputFile.close();
}
