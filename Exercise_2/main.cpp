#include <iostream>
#include "Utils.hpp"
#include "PolygonalMesh.hpp"

using namespace std;
using namespace MeshLibrary;


int main()
{
    PolygonalMesh mesh;
    string path = "PolygonalMesh";
    unsigned int tol = 1e-1;

    if(!importMesh(path, mesh, tol))
        return 1;

    return 0;
}

