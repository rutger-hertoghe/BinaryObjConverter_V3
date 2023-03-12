#include <iostream>

#include "ArgumentInterpreter.h"
#include "BinaryReader.h"
#include "BinaryWriter.h"

int main(int argc, char* argv[])
{
    ArgumentInterpreter argInterpreter{ argc, argv };
    if(!argInterpreter.IsInputValid())
    {
        std::cout << "Supplied input file was invalid!\n";
    	std::cout << "Verify whether it exists & whether it has either the 'obj' or 'bobj' extension\n";
        return -1;
    }

    if(!argInterpreter.IsOutputValid())
    {
        std::cout << "Supplied output file was invalid!\n";
    	std::cout << "Verify that it has either the 'obj' or 'bobj' extension & that it does not have the same extension as the input file\n";
        return -1;
    }

    if(argInterpreter.GetInputExtension() == "obj")
    {
        BinaryWriter writer(argInterpreter.GetInputFile(), argInterpreter.GetOutputFile());
        writer.WriteBobj();
    }
    else if(argInterpreter.GetInputExtension() == "bobj")
    {
        BinaryReader reader(argInterpreter.GetInputFile(), argInterpreter.GetOutputFile());
        reader.WriteObj();
    }
}

