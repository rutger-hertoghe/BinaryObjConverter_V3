#include "BinaryReader.h"
#include "BinaryWriter.h"

int main()
{
    BinaryWriter("low_poly_stanford_bunny.obj", "outputFile.bobj");

    BinaryReader("outputFile.bobj", "reinterpreted.obj");
}

