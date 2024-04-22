#include "Utils.hpp"
#include "PolygonalMesh.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Eigen/Eigen>

namespace MeshLibrary {

bool importMesh(const string& path,
                PolygonalMesh& mesh)
{
    if(!importCell0D(path + "/Cell0Ds.csv", mesh))
        return false;
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarker.begin(); it != mesh.Cell0DMarker.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!importCell1D(path + "/Cell1Ds.csv", mesh))
        return false;
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarker.begin(); it != mesh.Cell1DMarker.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!importCell2D(path + "/Cell2Ds.csv", mesh))
        return false;
    /*else
    {
        // TEST
        for(unsigned int c = 0; c < mesh.NumberOfCell2Ds; c++)
        {
            vector<unsigned int> edges = mesh.EdgesCell2Ds[c];

            for(unsigned int e = 0; e < 3; e++)
            {
                const unsigned int origin = mesh.VerticesCell1Ds[edges[e]][0];
                const unsigned int end = mesh.VerticesCell1Ds[edges[e]][1];

                auto findOrigin = find(mesh.VerticesCell2Ds[c].begin(), mesh.VerticesCell2Ds[c].end(), origin);
                if(findOrigin == mesh.VerticesCell2Ds[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.VerticesCell2Ds[c].begin(), mesh.VerticesCell2Ds[c].end(), end);
                if(findEnd == mesh.VerticesCell2Ds[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }

            }
        }
    }*/

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

    mesh.CoordinatesCell0Ds.reserve(mesh.NumberOfCell0Ds); // gli riservo la quantità di memoria che serve
    mesh.IdCell0Ds.reserve(mesh.NumberOfCell0Ds);

    //mesh.MarkerCell0Ds.reserve(mesh.NumberOfCell0Ds);

    for(const string & line : lines)
    {
        istringstream convert(line);

        char delimiter = ';';
        unsigned int id;
        unsigned int marker;
        Vector2d coordinates;

        convert >> id >> delimiter >> marker >> delimiter >> coordinates(0) >> delimiter >> coordinates(1); // 0 -> coordinata x, 1 -> coordinata y

        mesh.IdCell0Ds.push_back((id));
        //mesh.MarkerCell0Ds.push_back(marker);
        mesh.CoordinatesCell0Ds.push_back(coordinates);

        if(marker != 0)
        {
            auto ret = mesh.Cell0DMarker.insert({marker, {id}});
            if(!ret.second) // se la chiave marker era già presente nella mappa allora non si è effettuato l'inserimento, quindi lo metto manualmente
                mesh.Cell0DMarker[marker].push_back(id);
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

    mesh.VerticesCell1Ds.reserve(mesh.NumberOfCell1Ds); // gli riservo la quantità di memoria che serve
    mesh.IdCell1Ds.reserve(mesh.NumberOfCell1Ds);

    for(const string & line : lines)
    {
        istringstream convert(line);

        char delimiter = ';';
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        convert >> id >> delimiter >> marker >> delimiter >> vertices(0) >> delimiter >> vertices(1); // 0 -> coordinata x, 1 -> coordinata y

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

    lines.pop_front(); // ignoro l'header, questo è un modo più compatto di scriverlo

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


}
