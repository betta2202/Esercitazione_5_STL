#pragma once

#include<iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace MeshLibrary{

bool importMesh(const string& path,
                PolygonalMesh& mesh,
                double& tol);

bool importCell0D(const string& fileName,
                  PolygonalMesh& mesh);

bool importCell1D(const string& fileName,
                  PolygonalMesh& mesh);

bool importCell2D(const string& fileName,
                  PolygonalMesh& mesh);

}

