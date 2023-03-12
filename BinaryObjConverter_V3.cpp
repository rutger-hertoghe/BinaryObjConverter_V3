#include "BinaryReader.h"
#include "BinaryWriter.h"

int main()
{
    BinaryWriter writer("low_poly_stanford_bunny.obj", "outputFile.bobj");
    writer.WriteBobj();

    BinaryReader reader("outputFile.bobj", "reinterpreted.obj");
    reader.WriteObj();
}

