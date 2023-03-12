#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

struct Vertex
{
    float x, y, z;
};

int main()
{
    // Initialize files
    std::ifstream inputFile{ "low_poly_stanford_bunny.obj" };
    std::ofstream outputFile{ "outputFile.bobj", std::ios::binary };

    if(!inputFile.is_open())
    {
        std::cout << "Input file not found!\n";
        return -1;
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
        if(it == typeMap.end()) 
        {
            typeMap[type] = typeIdx++;
        }
    }
    // Go back to the beginning
    inputFile.clear();
    inputFile.seekg(0);

    // NOTE: if more than 255 "types" were detected in the file, this code will break. This should not be the case for obj files.
    // Write header
    for(const auto& typePair : typeMap)
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
    while(std::getline(inputFile, currentLine))
    {
        // Get type of new line
        std::stringstream line{ currentLine };
        std::string lineType;
        line >> lineType;

        // If line of different type, write block
        if(lineType != currentType && !blockData.empty())
        {
            if(currentType == "v" || currentType == "f")
            {
            	// Write type
                char type{ typeMap[currentType] };
                outputFile.write(&type, sizeof(type));
                // Write blocklength (block.size() * relevant fields)
                size_t blockLength{ blockData.size() };
                outputFile.write((const char*)&blockLength, sizeof(blockLength));
            }
            
            // Write block
            if(currentType == "v")
            {
	            for(const auto& vertex : blockData)
	            {
                    std::stringstream vertexString{ vertex };
                    std::string type;
                    Vertex v;
                    vertexString >> type >> v.x >> v.y >> v.z;
                    outputFile.write((const char*)&v, sizeof(v));
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






    // BACK TRANSLATION START
    outputFile.close();

    std::ifstream fileToReinterpret{ "outputFile.bobj", std::ios::binary };
    std::ofstream backTranslation{ "retranslatedOutput.obj" };

    // DECODE HEADER
    // Read header to map
    std::unordered_map<char, std::string> charTypeMap;
    char currentTypeChar{ 1 };
    while(currentTypeChar != nullTerminator)
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
    for(const auto& typePair : charTypeMap)
    {
        const auto typeChar{ typePair.first };
        const auto& typeString{ typePair.second };
        std::cout << static_cast<int>(typeChar) << " : " << typeString << '\n';
    }

    // READ DATA BLOCKS
    while(!fileToReinterpret.eof())
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
                backTranslation << charTypeMap[type] << v.x << v.y << v.z << '\n';
            }
        }
        // Verify end of block
        char blockTerminator{};
        fileToReinterpret.read(&blockTerminator, sizeof(blockTerminator));
        if (blockTerminator != 0)
        {
            std::cout << "block was not terminated by null terminator!\n";
            return -1;
        }
    }
}

