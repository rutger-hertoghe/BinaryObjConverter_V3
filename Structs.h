#pragma once

struct Vertex
{
    float x, y, z;
};

struct Face
{
    int v0, v1, v2;
};

struct CompactFace
{
    short int v0, v1, v2;
};