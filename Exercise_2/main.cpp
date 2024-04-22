#include <iostream>
#include "Utils.hpp"
#include "PolygonalMesh.hpp"

using namespace std;
using namespace MeshLibrary;


int main()
{
    PolygonalMesh mesh;
    string path = "PolygonalMesh";

    if(!importMesh(path, mesh))
        return 1;

    return 0;
}

