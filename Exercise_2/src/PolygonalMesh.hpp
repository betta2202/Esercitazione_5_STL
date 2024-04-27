#pragma once

#include <vector>
#include <Eigen/Eigen>


using namespace Eigen;
using namespace std;

namespace MeshLibrary{

struct PolygonalMesh
{
    // Cell0D
    unsigned int NumberOfCell0Ds = 0;
    vector<unsigned int> IdCell0Ds = {};
    vector<Vector2d> CoordinatesCell0Ds;
    map<unsigned int, list<unsigned int>> Cell0DMarker = {}; // map<marker, list id>

    // Cell1D
    unsigned int NumberOfCell1Ds = 0;
    vector<unsigned int> IdCell1Ds = {};
    vector<Vector2i> VerticesCell1Ds = {};
    map<unsigned int, list<unsigned int>> Cell1DMarker = {}; // id univoco solo per le cell0d, cell1d... l'intersezione non è nulla

    // Cell2D
    unsigned int NumberOfCell2Ds = 0;
    vector<unsigned int> IdCell20Ds = {};
    vector<vector<unsigned int>> VerticesCell2Ds = {};
    vector<vector<unsigned int>> EdgesCell2Ds = {};

};

}
