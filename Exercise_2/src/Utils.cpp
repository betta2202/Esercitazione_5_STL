#include "Utils.hpp"
#include "PolygonalMesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Eigen/Eigen>

namespace MeshLibrary {


bool importMesh(const string& path,
                PolygonalMesh& mesh,
                double& tol)
{
    if(!importCell0D(path + "/Cell0Ds.csv", mesh))
        return false;
    else
    {
        // TEST 1: checks whether the stored points are correct, printing each marker and the corresponding point
        cout << "TEST 1:" << endl;
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarker.begin(); it != mesh.Cell0DMarker.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }
    cout << endl;

    if(!importCell1D(path + "/Cell1Ds.csv", mesh))
        return false;
    else
    {
        // TEST 2: checks whether the stored points are correct, printing each marker and the corresponding point
        cout << "TEST 2:" << endl;
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarker.begin(); it != mesh.Cell1DMarker.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }
    cout << endl;

    // TEST 3: checks whether the edges of the polygons have non-zero length
    cout << "TEST 3:" << endl;
    cout.flush();
    if(!EdgesTest(mesh, tol)){
        cerr << "The edges of the polygons have zero length"<< endl;
        return false;
    }
    else
        cout << "The edges of the polygons have non-zero length" << endl;
    cout << endl;

    if(!importCell2D(path + "/Cell2Ds.csv", mesh))
        return false;

    // TEST 4: checks whether the area of the polygons is non-zero
    cout << "TEST 4:" << endl;
    cout.flush();
    if(!AreaTest(mesh, tol)){
        cerr << "The area of the polygons is zero" << endl;
        return false;
    }
    else
        cout << "The area of the polygons is non-zero" << endl;
    cout << endl;


    return true;
}

// ********************************************************************************************************************************

bool importCell0D(const string& fileName,
                  PolygonalMesh& mesh)
{
    ifstream file(fileName);

    if(file.fail())
        return false;


    string header; // temporary variable
    getline(file, header);

    list<string> lines;
    string line;

    while(getline(file, line))
    {
        lines.push_back(line);
    }

    mesh.NumberOfCell0Ds = lines.size(); // number of vertices

    if (mesh.NumberOfCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.CoordinatesCell0Ds.reserve(mesh.NumberOfCell0Ds);
    mesh.IdCell0Ds.reserve(mesh.NumberOfCell0Ds);

    for(const string & line : lines)
    {
        istringstream convert(line);

        char delimiter = ';';
        unsigned int id;
        unsigned int marker;
        Vector2d coordinates;

        convert >> id >> delimiter >> marker >> delimiter >> coordinates(0) >> delimiter >> coordinates(1); // 0 -> coordinate x, 1 -> coordinate y

        mesh.IdCell0Ds.push_back((id));
        mesh.CoordinatesCell0Ds.push_back(coordinates);

        if(marker != 0)
        {
            auto ret = mesh.Cell0DMarker.insert({marker, {id}});
            if(!ret.second){
                // if the marker key was already present in the map then the insertion was not carried out, so I insert it manually
                mesh.Cell0DMarker[marker].push_back(id);
            }
        }

    }


    file.close();

    return true;
}

// ********************************************************************************************************************************

bool importCell1D(const string& fileName,
                  PolygonalMesh& mesh)
{
    ifstream file(fileName);

    if(file.fail())
        return false;


    string header; // temporary variable
    getline(file, header);

    list<string> lines;
    string line;

    while(getline(file, line))
    {
        lines.push_back(line);
    }

    mesh.NumberOfCell1Ds = lines.size();

    if (mesh.NumberOfCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.VerticesCell1Ds.reserve(mesh.NumberOfCell1Ds);
    mesh.IdCell1Ds.reserve(mesh.NumberOfCell1Ds);

    for(const string & line : lines)
    {
        istringstream convert(line);

        char delimiter = ';';
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        convert >> id >> delimiter >> marker >> delimiter >> vertices(0) >> delimiter >> vertices(1); // 0 -> coordinate x, 1 -> coordinate y

        mesh.IdCell1Ds.push_back(id);
        mesh.VerticesCell1Ds.push_back(vertices);

        if(marker != 0)
        {
            auto ret = mesh.Cell1DMarker.insert({marker, {id}});
            if(!ret.second)
                mesh.Cell1DMarker[marker].push_back(id);
        }

    }


    file.close();

    return true;
}

// ********************************************************************************************************************************

bool importCell2D(const string& fileName,
                  PolygonalMesh& mesh)
{
    ifstream file;
    file.open(fileName);

    if(file.fail())
        return false;

    list<string> lines;
    string line;
    while (getline(file, line))
        lines.push_back(line);

    lines.pop_front(); // the header is ignored

    mesh.NumberOfCell2Ds = lines.size();

    if (mesh.NumberOfCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.IdCell20Ds.reserve(mesh.NumberOfCell2Ds);
    mesh.VerticesCell2Ds.reserve(mesh.NumberOfCell2Ds);
    mesh.EdgesCell2Ds.reserve(mesh.NumberOfCell2Ds);

    for (const string& line : lines)
    {
        istringstream converter(line);

        char delimiter = ';';
        unsigned int id;
        unsigned int marker;
        unsigned int numVertices;
        unsigned int numEdges;
        vector<unsigned int> vertices;
        vector<unsigned int> edges;

        converter >>  id >> delimiter >> marker >> delimiter >> numVertices;
        for(unsigned int i = 0; i < numVertices; i++)
        {
            unsigned int v;
            converter >> delimiter >> v;
            vertices.push_back(v);
        }

        converter >> numEdges;

        for(unsigned int i = 0; i < numEdges; i++)
        {
            unsigned int e;
            converter >> delimiter >> e;
            edges.push_back(e);
        }

        mesh.IdCell20Ds.push_back(id);
        mesh.VerticesCell2Ds.push_back(vertices);
        mesh.EdgesCell2Ds.push_back(edges);
    }


    file.close();

    return true;
}

// EDGES TEST
bool EdgesTest(PolygonalMesh& mesh, double& in_tol)
{
    double tol = max(in_tol, numeric_limits<double>::epsilon());
    for(const auto& points : mesh.VerticesCell1Ds)
    {
        Vector2d coord1 = mesh.CoordinatesCell0Ds[points(0)];
        Vector2d coord2 = mesh.CoordinatesCell0Ds[points(1)];
        if (abs(coord1(0) - coord2(0)) < tol && abs(coord1(1) - coord2(1)) < tol)
        {
            return false;
            break;
        }
    }
    return true;
}


// AREA TEST
bool AreaTest(PolygonalMesh& mesh, double& in_tol)
{
    double tol = max((sqrt(3)/8)*in_tol*in_tol, numeric_limits<double>::epsilon());
    for(unsigned int c = 0; c < mesh.NumberOfCell2Ds; c++)
    {
        unsigned int n = mesh.VerticesCell2Ds[c].size(); // number of vertices of the polygon
        vector<double> area;
        area.reserve(n-2);
        unsigned int count = 0;
        double totArea = 0;

        while(count < n-2)
        {
            double x1 = mesh.CoordinatesCell0Ds[mesh.VerticesCell2Ds[c][0]](0);
            double y1 = mesh.CoordinatesCell0Ds[mesh.VerticesCell2Ds[c][0]](1);

            double x2 = mesh.CoordinatesCell0Ds[mesh.VerticesCell2Ds[c][count + 1]](0);
            double y2 = mesh.CoordinatesCell0Ds[mesh.VerticesCell2Ds[c][count + 1]](1);

            double x3 = mesh.CoordinatesCell0Ds[mesh.VerticesCell2Ds[c][count + 2]](0);
            double y3 = mesh.CoordinatesCell0Ds[mesh.VerticesCell2Ds[c][count + 2]](1);

            // the next line calculate the area of the triangle using the vector product
            area.push_back(1.0/2.0 * abs(x1*y2 + x3*y1 + x2*y3 - x3*y2 - x1*y3 - x2*y1));

            count ++;
        }

        for(unsigned int i = 0; i < area.size(); i++)
            totArea += area[i];

        if(totArea < tol)
        {
            return false;
            break;
        }

    }
    return true;
}

}
