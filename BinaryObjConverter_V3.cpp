#include "BinaryReader.h"
#include "BinaryWriter.h"

int main()
{
    BinaryWriter("low_poly_stanford_bunny.obj", "outputFile.bobj");

    BinaryReader reader("outputFile.bobj", "reinterpreted.obj");
    reader.WriteObj();
}

