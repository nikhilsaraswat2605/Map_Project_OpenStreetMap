/*
    Name: NIKHIL SARASWAT
    Roll No.: 20CS10039
*/

#include <iostream>
#include <bits/stdc++.h>
#include <cstdio>
#include <fstream>
#include <vector>
#include "rapidxml.hpp"

#define int long long
#define float long double
#define endl '\n'

using namespace std;
using namespace rapidxml;

class Node // Class for Node
{
public:
    int id;
    string name;
    float lat;
    float lon;
    Node()
    {
        this->name = "unknown";
    }
};
class Way // Class for Way
{
public:
    int id;
    vector<int> nodes;
};

class OpenStreetMap // Class for OpenStreetMap (This is used for Main functioning of program)
{
private:
    vector<Node> nodeArray;                   // data structure for storing the data of node
    vector<Way> wayArray;                     // data structure for storing the data of ways
    map<int, int> nodeidhash;                 // data structure for hashing node id and index of node in nodeArray vector
    map<int, int> touch;                      // data structure to strore path between two nodes
    map<int, vector<pair<int, float>>> graph; // data structure for graph (Adjacency List)
public:
    // This function is to parshing the <map.osm>
    void parsing()
    {
        xml_document<> doc;
        xml_node<> *root_node = NULL;
        // cout << "\nParsing map data (map.osm)....." << endl;

        // Read the map.osm file
        ifstream theFile("map.osm");
        vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
        buffer.push_back('\0');

        // Parse the buffer
        doc.parse<0>(&buffer[0]);

        // Find out the root node
        root_node = doc.first_node("osm");

        // Iterate over the All nodes of the map.osm
        for (xml_node<> *student_node = root_node->first_node("node"); student_node; student_node = student_node->next_sibling())
        {
            string name = student_node->name();
            if (name == "node")
            {
                Node tempnode;
                string tempstring = (student_node->first_attribute("id")->value());
                tempnode.id = stoll(tempstring);
                tempstring = (student_node->first_attribute("lat")->value());
                tempnode.lat = stold(tempstring);
                tempstring = (student_node->first_attribute("lon")->value());
                tempnode.lon = stold(tempstring);

                // Interate over the node tags with 'tag'
                for (xml_node<> *student_name_node = student_node->first_node("tag"); student_name_node; student_name_node = student_name_node->next_sibling())
                {
                    string nameattribute = student_name_node->first_attribute("k")->value();
                    if (nameattribute == "name:en")
                    {
                        tempnode.name = student_name_node->first_attribute("v")->value();
                    }
                    else if (nameattribute == "name")
                    {
                        tempnode.name = student_name_node->first_attribute("v")->value();
                    }
                }
                this->nodeArray.push_back(tempnode);
            }
            else if (name == "way")
            {
                Way tempway;
                string tempstring = (student_node->first_attribute("id")->value());
                tempway.id = stoll(tempstring);

                // Interate over the nodes under way with 'nd' tag
                for (xml_node<> *student_name_node = student_node->first_node("nd"); student_name_node; student_name_node = student_name_node->next_sibling())
                {
                    string name = student_name_node->name();
                    if (name == "nd")
                    {
                        string nameattribute = student_name_node->first_attribute("ref")->value();
                        tempway.nodes.push_back(stoll(nameattribute));
                    }
                }
                this->wayArray.push_back(tempway);
            }
        }
    }
    // This function is to check the validity of node id
    inline int checknodeID(int ID)
    {
        for (int i = 0; i < nodeArray.size(); i++)
        {
            if (this->nodeArray[i].id == ID)
                return i;
        }
        return -1; // if node id is not present then returning -1
    }
    // This function is for converting the angle from degrees to radians
    inline float deg2rad(float deg)
    {
        return deg * (M_PI / 180); // since we know that, PI radians = 180 degrees
    }
    // This function is for finding crow fly distance between two nodes
    inline float crowflyDistance(float latX, float lonX, float latY, float lonY)
    {
        /*
        The formulae which I have used is called the haversine formula,
        which calculates the distance between two points on a sphere as the crow flies.
        reference :-
        https://en.wikipedia.org/wiki/Haversine_formula
        */
        float RadiusEarth = 6371;          // Radius of the earth in km
        float dLat = deg2rad(latY - latX); // deg2rad below
        float dLon = deg2rad(lonY - lonX);
        float a = sin(dLat / 2) * sin(dLat / 2) + cos(deg2rad(latX)) * cos(deg2rad(latY)) * sin(dLon / 2) * sin(dLon / 2);
        float c = 2 * atan2(sqrt(a), sqrt(1 - a));
        float ans = RadiusEarth * c; // Distance in km
        return ans;
    }
    // This function is for searching a substring in the graph
    void findelement(string &pat)
    {
        int position = 0;
        int index_str;
        string str = pat;
        bool flag = 0;
        int cnt = 1;
        for (auto node : this->nodeArray)
        {
            string name = node.name;
            // below transformations are for case in-sensitiveness
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            transform(pat.begin(), pat.end(), pat.begin(), ::tolower);
            if ((index_str = name.find(pat, position)) != string::npos) // using buit in find function which gives index at which substr was found
            {
                if (node.name == "unknown")
                    continue;
                else
                {
                    if (flag == 0)
                    {
                        flag = 1;
                        cout << "\n\tMost relevent results of the given substring: \"" << str << "\" are as following :-\n"
                             << endl;
                    }
                    cout << setprecision(8) << fixed << "\t" << cnt << ". node id: " << node.id << " ; node name: " << node.name << " ; latitude: " << node.lat << " ; longitude: " << node.lon << endl;
                    cnt++;
                }
            }
        }
        if (flag == 0)
        {
            cout << "Your search - \"" << pat << "\" ... - did not match any node!" << endl;
        }
    }
    // This function is for creating a hash between node id and it index in the nodeArray vector
    void hashing()
    {
        for (int i = 0; i < this->nodeArray.size(); i++)
        {
            nodeidhash[nodeArray[i].id] = i;
        }
    }
    // This function is for finding k closest nodes of a particular node
    void Kclosestnode(int id, float latPoint, float lonPoint, int k)
    {
        set<pair<float, pair<int, string>>> knearestSet;

        if (k >= this->nodeArray.size())
        {
            cout << "\n\t\tSorry, the k value (" << k << ") is larger than total number of nearest nodes of this node in the map\n"
                 << endl;
            return;
        }
        for (int i = 0; i < this->nodeArray.size(); i++)
        {
            if (id == this->nodeArray[i].id)
                continue;
            float distance = crowflyDistance(latPoint, lonPoint, this->nodeArray[i].lat, this->nodeArray[i].lon);
            knearestSet.insert(make_pair(distance, make_pair(this->nodeArray[i].id, this->nodeArray[i].name)));
        }
        cout << k << "\n nearest nodes of the given node id : \'" << id << "\' are as following :-\n"
             << endl;
        int cnt = 1;
        for (auto node : knearestSet)
        {
            cout << setprecision(8) << fixed << "\t\t" << cnt << ". node id: " << node.second.first << " ; node name: " << node.second.second << " ; distance: " << node.first << " kms." << endl;
            if (cnt >= k)
                break;
            cnt++;
        }
    }
    // This function is for creating a graph among nodes
    void createGraph()
    {
        // creating a graph with adjacency list
        for (auto way : this->wayArray)
        {
            float distance = 0.0;
            int size = way.nodes.size();
            for (int i = 0; i < size; i++)
            {
                if (i == 0)
                {
                    if (size > 1)
                    {
                        distance = crowflyDistance(this->nodeArray[nodeidhash[way.nodes[i]]].lat,
                                                   this->nodeArray[nodeidhash[way.nodes[i]]].lon,
                                                   this->nodeArray[nodeidhash[way.nodes[i + 1]]].lat,
                                                   this->nodeArray[nodeidhash[way.nodes[i + 1]]].lon);

                        graph[way.nodes[i]].push_back({way.nodes[i + 1], distance});
                    }
                    continue;
                }
                if (i == size - 1)
                {

                    distance = crowflyDistance(this->nodeArray[nodeidhash[way.nodes[i]]].lat,
                                               this->nodeArray[nodeidhash[way.nodes[i]]].lon,
                                               this->nodeArray[nodeidhash[way.nodes[i - 1]]].lat,
                                               this->nodeArray[nodeidhash[way.nodes[i - 1]]].lon);

                    graph[way.nodes[i]].push_back({way.nodes[i - 1], distance});

                    continue;
                }
                distance = crowflyDistance(this->nodeArray[nodeidhash[way.nodes[i]]].lat,
                                           this->nodeArray[nodeidhash[way.nodes[i]]].lon,
                                           this->nodeArray[nodeidhash[way.nodes[i + 1]]].lat,
                                           this->nodeArray[nodeidhash[way.nodes[i + 1]]].lon);

                graph[way.nodes[i]].push_back({way.nodes[i + 1], distance});

                distance = crowflyDistance(this->nodeArray[nodeidhash[way.nodes[i]]].lat,
                                           this->nodeArray[nodeidhash[way.nodes[i]]].lon,
                                           this->nodeArray[nodeidhash[way.nodes[i - 1]]].lat,
                                           this->nodeArray[nodeidhash[way.nodes[i - 1]]].lon);

                graph[way.nodes[i]].push_back({way.nodes[i - 1], distance});
            }
        }
    }
    // This function is for finding shortest path between two nodes, this is Djikstra algorithm
    float dijkstra(int srcid, int destid)
    {
        // Time Complexity - O(E logV)
        int size = this->nodeArray.size();
        map<int, float> length;
        for (int i = 0; i < size; i++)
        {
            length[this->nodeArray[i].id] = 10000007.0;
            touch[this->nodeArray[i].id] = srcid;
        }
        touch[srcid] = -1;
        length[srcid] = 0.0;

        // priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> pq;    // we could also use priority queue for the same purpose
        set<pair<float, int>> pq;

        pq.insert({0.0, srcid});
        while (!pq.empty())
        {
            auto vnear = pq.begin();
            pair<float, int> vnearest = {vnear->first, vnear->second};
            pq.erase(vnear);

            if (vnearest.second == destid)
            {
                return vnearest.first;
            }
            for (auto neighbor : this->graph[vnearest.second])
            {
                if (length[vnearest.second] + neighbor.second < length[neighbor.first])
                {
                    touch[neighbor.first] = vnearest.second;
                    pq.erase({length[neighbor.first], neighbor.first});
                    length[neighbor.first] = length[vnearest.second] + neighbor.second;
                    pq.insert({length[neighbor.first], neighbor.first});
                }
            }
        }
        return -1;
    }
    // This function is for creating a prompt message for input, it will be used in user interface
    void prompt()
    {
        cout << endl;
        cout << "---------------------------------------------------------------------------------------------" << endl;
        cout << "\tType 1: for printing total number of nodes and ways - (task # 1) Part-1" << endl;
        cout << "\tType 2: for search for a particular element - (task # 1) Part-2" << endl;
        cout << "\tType 3: for finding k nearest nodes of a particular node - (task # 2)" << endl;
        cout << "\tType 4: for finding shortest path between two nodes - A and B - (task # 3)" << endl;
        cout << "\tType any other number for QUIT" << endl;
        cout << "---------------------------------------------------------------------------------------------" << endl;
    }
    // This function is for creating an User Interface to provide better experience
    void userInterface()
    {

        cout << "\n\n-------------------------------- WELCOME TO OPEN STREET MAP! --------------------------------" << endl;
        while (true)
        {
            prompt();
            cout << "Type here: \t";
            int type;
            cin >> type;
            if (type == 1) // Task #1 Part-1
            {
                cout << "\n---------------------------------------------------------------------------------------------" << endl;
                cout << "\n\tTotal number of nodes discovered in this map: " << nodeArray.size() << endl;
                cout << "\tTotal number of ways discovered in this map: " << wayArray.size() << endl;
            }
            else if (type == 2) // Task #1 Part-2
            {
                cout << "\nEnter the string which you want to search: ";
                string str;
                getline(cin, str);
                getline(cin, str);
                this->findelement(str);
            }
            else if (type == 3) // Task #2
            {
                cout << "\nEnter the ID of node: ";
                int ID, k;
                cin >> ID;
                int index = this->checknodeID(ID);
                if (index < 0)
                {
                    cout << "\n\t\tSorry, This ID is not valid!\n"
                         << endl;
                    continue;
                }
                cout << "\nEnter k (number of nearest nodes): ";
                cin >> k;
                this->Kclosestnode(nodeArray[index].id, nodeArray[index].lat, nodeArray[index].lon, k);
            }
            else if (type == 4) // Task #3
            {
                cout << "\nEnter the ID of node A: ";
                int IDa;
                cin >> IDa;
                if (this->checknodeID(IDa) < 0)
                {
                    cout << "\n\t\tSorry, This ID is not valid!\n"
                         << endl;
                    continue;
                }
                cout << "\nEnter the ID of node B: ";
                int IDb;
                cin >> IDb;
                if (this->checknodeID(IDb) < 0)
                {
                    cout << "\n\t\tSorry, This ID is not valid!\n"
                         << endl;
                    continue;
                }
                float ShortestPathLength = this->dijkstra(IDa, IDb);
                if (ShortestPathLength < 0)
                {
                    cout << "\n\t\tThese two nodes are not connected by any way!\n"
                         << endl;
                    continue;
                }
                cout << setprecision(8) << fixed << "\n\n\tShortest path length between A and B: " << ShortestPathLength << " kms.\n"
                     << endl;
                cout << "\nThe path is as following :- \n"
                     << endl;
                int node = IDb;
                cout << "\tB = " << node;
                while (this->touch[node] > 0)
                {
                    cout << " <- " << this->touch[node];
                    node = this->touch[node];
                }
                cout << " = A" << endl;
            }
            else
            {
                cout << "\n\t\t\tThank You!\n"
                     << endl;
                return;
            }
        }
    }
};

signed main()
{
    // Delaring an object of class OpenStreetMap
    OpenStreetMap OSM;

    OSM.parsing();     // Calling for parsing of map.osm
    OSM.hashing();     // Creating a hash between node id and index in the node Array
    OSM.createGraph(); // Creating a graph using Adjacency List

    OSM.userInterface(); // Creating a User Interface to operate the program

    return 0;
}
