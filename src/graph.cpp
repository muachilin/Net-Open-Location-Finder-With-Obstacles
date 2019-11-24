#include "graph.h"


Edge::Edge(Node *a, Node *b, const int& w)
{
	if ( a->id <= b->id ) { node[0] = a; node[1] = b; }
	else { node[0] = b; node[1] = a; }
	weight = w;
}

Edge::Edge(Node *a, Node *b, const int& w, vector < pair < Node*, Node* > > Nv)
{
	if ( a->id <= b->id ) { node[0] = a; node[1] = b; }
	else { node[0] = b; node[1] = a; }
	weight = w;
    NodePair_vector = Nv;
}


bool Edge::operator < (const Edge& rhs) const{

	int id1a = node[0]->id;
	int id1b = node[1]->id;

	int id2a = rhs.node[0]->id;
	int id2b = rhs.node[1]->id;


	if ( id1a == id2a ) return id1b < id2b;
	if ( id1a == id2b ) return id1b < id2a;
	if ( id1b == id2a ) return id1a < id2b;
	if ( id1b == id2b ) return id1a < id2a;

	return weight < rhs.weight;
}
Node * Edge::getNeighbor(Node *n)
{
	if ( node[0] == n ) return node[1];
	if ( node[1] == n ) return node[0];

	return 0;	
}


Node::Node(const int& i)
{
	id = i;
	traveled = false;
    color = -1;
	d = DIS_INF;
	prev = 0;
}

void Node::addEdge(Edge *e)
{
	edge.push_back(e);
}

bool edgeComp( const Edge* A, const Edge* B ){
	if ( (*A) < (*B) ) return true;
	return false;
}


void Node::sortEdge()
{
    sort(edge.begin(), edge.end(), edgeComp);
}

Graph::Graph(const string& n)
{
	name = n;
    //_g = new Graph(n);
}

Graph::~Graph()
{
	vector<Edge *>::iterator itE;
	for ( itE = edges.begin() ; itE != edges.end() ; itE++ )
	{
		delete (*itE);
		(*itE) = 0;
	}
	
	map<int, Node *>::iterator itN;
	for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
	{
		delete (*itN).second;
		(*itN).second = 0;
	}

    /*vector<Node *>::iterator itN2;
	for ( itN2 = nodes.begin() ; itN2 != nodes.end() ; itN2++ )
	{
		(*itN2) = 0;
	}*/
}
		
void Graph::addEdge(const int& v1, const int& v2, const int& w)
{
	Node *a, *b;
	map<int, Node *>::iterator it = nodesMap.find(v1);
	if ( it != nodesMap.end() )
		a = (*it).second;
	else
	{
		a = new Node(v1);
		nodesMap[v1] = a;
        nodes.push_back(a);
	}

	it = nodesMap.find(v2);
	if ( it != nodesMap.end() )
		b = (*it).second;
	else
	{
		b = new Node(v2);
		nodesMap[v2] = b;
        nodes.push_back(b);
	}

	Edge *e = new Edge(a, b, w);
	edges.push_back(e);

	a->edge.push_back(e);
	b->edge.push_back(e);

}

void Graph::addEdge_modified(const int& v1, const int& v2, const int& w, vector < pair < Node*, Node* > > Nv)
{
	Node *a, *b;
	map<int, Node *>::iterator it = nodesMap.find(v1);
	if ( it != nodesMap.end() )
		a = (*it).second;
	else
	{
		a = new Node(v1);
		nodesMap[v1] = a;
        nodes.push_back(a);
	}

	it = nodesMap.find(v2);
	if ( it != nodesMap.end() )
		b = (*it).second;
	else
	{
		b = new Node(v2);
		nodesMap[v2] = b;
        nodes.push_back(b);
	}

	Edge *e = new Edge(a, b, w, Nv); //
	edges.push_back(e);

	a->edge.push_back(e);
	b->edge.push_back(e);

}

	
void Graph::sortEdgesOfNode()
{
	map<int, Node *>::iterator it;
	for ( it = nodesMap.begin() ; it != nodesMap.end() ; it++ )
	{
		Node *node = (*it).second;
		node->sortEdge();
	}

}

bool edgeCompByW( const Edge* A, const Edge* B ){
	if ( A->weight < B->weight ) return true;
	return false;
}

void Graph::sortEdgesByWeight()
{
    sort(edges.begin(), edges.end(), edgeCompByW);
}


bool NodeCompByD( const Node* A, const Node* B ){
	if ( A->edge.size() > B->edge.size() ) 
        return true;
    else if (A->edge.size() == B->edge.size()) {
        if (A->id < B->id) 
            return true;
        else 
            return false;
    }
    else
        return false;
}

void Graph::sortNodesByDegree()
{
    sort(nodes.begin(), nodes.end(), NodeCompByD);
}


bool NodeCompByID( const Node* A, const Node* B ){
	if ( A->id < B->id ) return true;
    else return false;
}

void Graph::sortNodesByID()
{
    sort(nodes.begin(), nodes.end(), NodeCompByID);
}
void Graph::init()
{
	map<int, Node *>::iterator itN;
	for ( itN = nodesMap.begin() ; itN != nodesMap.end() ; itN++ )
	{
		Node *node = (*itN).second;
		node->traveled = false;
		node->d = DIS_INF;
		node->prev = 0;
        node->color = -1;
	}
	
}

Node * Graph::getNodeById(const int& id)
{
	return nodesMap[id];
}

ostream&
operator <<(ostream& os, const Edge& e)
{
    string weight = (e.weight == DIS_INF) ? "DIS_INF" : to_string(e.weight);
    os << "edge(" << e.node[0]->id << ", " << e.node[1]->id << ")" << endl;
    os << "weight: " << weight << endl;
    return os;
}

ostream&
operator <<(ostream& os, const Node& n)
{
    os << "======node" << n.id << "======" << endl;
    os << "color: " << n.color << endl;
    os << "edges: " << endl;
    for(auto& e: n.edge)
        os << *e << endl;
    os << endl;
    //os << "d: " << n.d << endl;
    //os << "previous node: " << n.prev->id << endl;
    os << "======node" << n.id << "======" << endl;
    return os;
}

ostream&
operator <<(ostream& os, const Graph& g)
{
    os << "============Graph " << g.name << "============" << endl;
    os << endl;
    os << "nodes: " << endl;
    for(auto& node: g.nodes){
        os << *node << endl;
    }
        os << endl;
    os << "edges: " << endl;

    for(auto& edge: g.edges){
        os << *edge << endl;
    }

    os << "============Graph " << g.name << "============" << endl;
    return os;

}

void
Graph::Dijkstra(Node* s)
{
    //cout << "[Dijkstra] source: " << s->id << endl;
    // initialize source node
    s->d = 0;
    s->prev = s;

    // use priorty queue
    pq_min Q; 
    for(auto& node: nodes)
        Q.push(node);
    
    while(!Q.empty()){
        //cout << "Q's size = " << Q.size() << endl;
        Node* u = Q.top();
        u->traveled = true;
        for(auto& v: u->edge) {
            Relax(u, v->getNeighbor(u), v->weight);
        }
        // done relax
        Q.pop();
    }

    cout << "After Dijkstra: " << endl;
    cout << "print every node's d" << endl;
    for(auto& node: nodes) {
        cout << "node #" << node->id << ": " << node->d << endl;
    }
}

void
Graph::Relax(Node* u, Node* v, int w)
{
    if(u->d == DIS_INF) cout << "u is INF" << endl;
    else if(v->d > u->d + w) {
        v->d = u->d + w;
        v->prev = u;
    }
    //cout << "Relaxing edge(" << u->id << ", " << v->id << ")" << endl;

}

void
Graph::initialize_all_nodes()
{
    // initialize all nodes
    for(auto& node: nodes) {
        cout << "initialing node #" << node->id << endl; 
        node->d = DIS_INF; // every node's d set to inf
        node->traveled = false; // every node not traveled
        node->prev = NULL; // every node's desent
    }
}

void
Graph::Dijkstra_all_nodes()
{
    // construct a new graph, store in _g 
    _g = new Graph(name);
    cout << "Dijkstraing..." << endl;
    Graph::initialize_all_nodes();

    for(auto& group: _groups) {
        cout << "=====Group #" << group->getID() << endl;
        initialize_all_nodes();
        for(auto& node: group->get_node_group()) {
            if(node != NULL) {
                //cout << "Doing node #" << node->id << endl;
                Dijkstra(node);
            }
        } 
        // after Dijkstra
        // find the distance to every other groups
        for(auto& other_group: _groups) {
            if(other_group != group) {
                int wei = DIS_INF; 
                Node* connect_node = NULL;
                for(auto& other_node: other_group->get_node_group()) {
                    if(other_node == NULL) cout << "NULL!!!!" << endl;
                    else {
                        if(other_node->d < wei) {
                            wei = other_node->d;
                            connect_node = other_node;
                        } 
                    }
                }
                if(group->getID() < other_group->getID() && connect_node != NULL) {
                    vector < pair < Node*, Node* > > NodePairVector;

                    //vector<Node*>::iterator it1= group->get_node_group().end();// = find(group->get_node_group().begin(),
                               // group->get_node_group().end(), connect_node->prev);
                    // the node's prev does not in group(this group)
		    int check_id = 0; //not in this group
		    vector<Node*> this_group = group->get_node_group();
		    cout<<this_group.size()<<endl;
		    for(int j = 0; j < this_group.size(); j++){if(this_group[j]!=NULL){cout<<this_group[j]->id<<endl;}}
                    while(check_id == 0) {
			cout<<"into the while"<<endl;
			cout<<connect_node->d<<endl;
                        pair<Node*, Node*> ed = make_pair(connect_node->prev, connect_node);
			cout<<"make pair"<<endl;
                        NodePairVector.push_back(ed);
			cout<<"push back"<<endl;
			if(connect_node->prev != NULL){cout<<"have previous"<<endl;}else {cout<<"GG"<<endl;}
			cout<<"connect node previous d "<<connect_node->prev->d<<" , id is "<<connect_node->prev->id<<endl;
			for(int j = 0; j < this_group.size(); j++){cout<<"checking"<<endl;if(this_group[j]!=NULL){if(this_group[j]->id == connect_node->prev->id){cout<<"find"<<endl;check_id = 1;break;}}}
                        //it1 = find(group->get_node_group().begin(),group->get_node_group().end(), connect_node->prev);
			cout<<"after find"<<endl;

		//	if(it1 == group->get_node_group().end()){cout<<"go to previous"<<endl;}
		//	else{cout<<"find!"<<endl;}
                        connect_node = connect_node->prev;
                    }

                    
                    _g->addEdge_modified(group->getID(), other_group->getID(), wei, 
                            NodePairVector);
                    //cout << "adding Edge (" << group->getID() << ", " 
                        //<< other_group->getID() << ", " << wei << ")" << endl;
                    //cerr << "in NodePairVector: " << endl;
                    //for(auto& i: NodePairVector) {
                    //    cerr << "(" << i.first->id << ", " << i.second->id << ")" << endl;
                    //}
                    /*
                    vector<Node*>::iterator it;
                    // get the edge they connect
                    for(auto& e: connect_node->edge) {
                        // if the connect_node's prev is not in group, 
                        // i.e the prev is a obstacle's pin,
                        // then we need to do it recursively
                        it = find(group->get_node_group().begin(),
                                group->get_node_group().end(), connect_node->prev);

                        while(it != group->get_node_group().end()) {
                            // if the node's prev == its neighbor, then we get the
                            // edge
                            if(e->getNeighbor(connect_node) == connect_node->prev) {
                                ed = make_pair(connect_node->prev, connect_node);
                                e->NodePair_vector.push_back(ed);
                            }
                            connect_node = connect_node->prev;
                            it = find(group->get_node_group().begin(),
                                    group->get_node_group().end(), connect_node->prev);

                        }
                        
                    }*/
                }
            }
        }
        _g->init();
    }
}

void
Graph::Prim(Node* r)
{
    cout << "doing prim..." << endl;
    initialize_all_nodes();

    r->d = 0;
    r->prev = r;
    pq_min Q;
    set<Node*> S;
    set<Node*>::iterator it;
    for(auto& node: nodes){
        Q.push(node);
        S.insert(node);
    }
        
    while(!Q.empty()) {
        Node* u = Q.top();
        if (u != r && u->prev != NULL) {
            Prim_path.push_back(make_pair(u->prev, u));
            // adding the real cartesian coordinate to path
            for(auto& e: u->edge) {
                if(e->getNeighbor(u) == u->prev) { // if got the edge
                    for(auto& pair: e->NodePair_vector) {
                        Prim_path_Cartesian.push_back(pair);
                    }
                }
            }
        }
            
        for(auto& e: u->edge) {
            Node* v = e->getNeighbor(u);
            int w = e->weight;
            it = S.find(v);
            if(it != S.end() && w < v->d) {
                v->prev = u;
                v->d = w;
            }
        }
        Q.pop();
        S.erase(S.find(u));
    }

    cout << "after Prim" << endl;
    cout << "print every node's d" << endl;
    for(auto& node: nodes) {
        cout << "node #" << node->id << ":" << node->d << endl;
    }
    cout << "Prim path: " << endl;
    for(auto& pair: Prim_path) {
        if(pair.first == NULL) cout << "NULL!!!" << endl;
        else
            cout << "node #" << pair.first->id << " -> " << "node #" 
            << pair.second->id << endl;
    }
}

