#include <iostream>
#include "Utils.hpp"
#include "PolygonalMesh.hpp"
#include <limits>

using namespace std;
using namespace MeshLibrary;


int main()
{
    PolygonalMesh mesh;
    string path = "PolygonalMesh";
    double tol = 1e-6;

    if(!importMesh(path, mesh, tol))
        return 1;

    return 0;
}

