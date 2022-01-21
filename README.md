# Map_Project_OpenStreetMap
This was the CPP Project Of Software Engineering Lab at IIT Kharagpur


Problem Statement:-

Openstreetmap (www.openstreetmap.org) is a free and open resource which provides high
quality map data for all areas in the world. They provide the data in the OSM format which can
be downloaded from their website. OSM is an xml-based format, which has various elements and
the attributes for those elements. The purpose of this assignment is to develop a software which
reads data about various artifacts on the map, and provide a text searchable interface to the
map.<br/>
An OSM file of the Kharagpur area is given to you for reference.<br/><br/>
Two particular elements of the OSM format, are of interest.<br/>
&emsp;&emsp; • “node”: This represents a particular place on the map, e.g. a shop, building, etc. It has an
id, latitude, and longitude. Additionally, some node elements have a “name” attribute
which describes the place.<br/>
&emsp;&emsp; • “way”: This represents a path or a line through the map, and is denoted by a sequence of
node elements. This element also has an id, and optionally a name describing the way <br/><br/>

**Use case 1:**<br/>
Write a C++ program which parses a given OSM file and extracts the node and way elements from
the file along with their attributes. It should print the total number nodes and ways discovered
in the given file. It allow a user to search for a particular element by matching input string with
substring of the name. You can use a xml parsing library to parse the file.<br/><br/>

**Use case 2:**<br/>
Given an input OSM write a C++ program which can find the k-closest nodes to a given node using
the crow fly distance. Here k is a user input. You can use the latitude, longitude information of
nodes to calculate the distance.<br/><br/>

**Use case 3:**<br/>
Given an input OSM file, write a C++ program which calculates the shortest path between two
node elements, through the way elements. The distance on a way is the sum of distances
between consecutive node elements in the way. You can follow the following steps:<br/>
1. Create an incidence data structure which stores whether ith way contains jth node.<br/>
2. Create a graph between the nodes and calculate the shortest “1-hop” distance between
the nodes.<br/>
3. Run a shortest path algorithm on this graph.<br/>
