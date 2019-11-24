#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include <fstream>
#include <functional>
#include <queue>
#include <set>
#include <iostream>
#include <utility>

#define DIS_INF 2147483647

using namespace std;


class Node;
class Group;
class Compare_node_d;

typedef priority_queue<Node*, vector<Node*>, Compare_node_d> pq_min;


class Edge{

	public:
		Edge(Node *a, Node *b, const int& w);
        Edge(Node *a, Node *b, const int& w, vector < pair < Node*, Node* > > Nv);
		Node* node[2];
		int weight;
        vector < pair <Node*, Node*> > NodePair_vector; // for Prim

		Node *getNeighbor(Node *n);

		bool operator < (const Edge& rhs) const;
        friend ostream& operator << (ostream& os, const Edge& e);
        
};

class Node{

	public:
		Node(const int& i);
		void addEdge(Edge *e);
		void sortEdge();
        friend ostream& operator << (ostream& os, const Node& n);

		int id;
        int color;
		bool traveled;
		vector<Edge *> edge;

		int d;
		Node *prev;///
		//int heap_idx;
};

class Graph{

    friend class Group;

	public:
		Graph(const string& n);
		~Graph();
		
		void addEdge(const int& v1, const int& v2, const int& w); /// "const int&"
        void addEdge_modified(const int& v1, const int& v2, const int& w, vector < pair < Node*, Node* > > Nv);

		void sortEdgesOfNode();
        void sortEdgesByWeight();
        void sortNodesByDegree();
        void sortNodesByID();
		void init();
        
        void addGroup(Group* g) { _groups.push_back(g); }
        void Dijkstra(Node* s);
        void Relax(Node* u, Node* v, int w);
        void initialize_all_nodes();
        void Dijkstra_all_nodes();

        void Prim(Node* r);


        friend ostream& operator << (ostream& os, const Graph& g);
		Node * getNodeById(const int& id);

		map < int, Node * > nodesMap;
        vector< Node * > nodes;
		vector< Edge * > edges;
		string name;
        vector < Group* > _groups;
        Graph* _g;
        vector< pair<Node*, Node* > >Prim_path;
        vector < pair < Node*, Node* > > Prim_path_Cartesian;
};

class Group
{
    public:
        Group() {}
        Group(const int& id): _id(id) {}
        ~Group() {}

        void add(Node* n) { _node_group.push_back(n); }
        void setID(const int& id) { _id = id; }
        int getID() { return _id; }
        vector < Node * > get_node_group() { return _node_group; }
        
    private:
        vector < Node * > _node_group;
        int _id;
};

class Compare_node_d
{
public:
    bool operator ()(Node* a, Node* b) {
        return a->d > b->d;
    }
};

#endif
