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
    double epsilon_double = numeric_limits<double>::epsilon();
    //double tol = 1e-8;
    double tol = epsilon_double;

    if(!importMesh(path, mesh, tol))
        return 1;

    return 0;
}

