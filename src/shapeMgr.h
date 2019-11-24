/***********************************************************************
 * FileName     [ shapeMgr.h ]
 * Synopsis     [ Define shape manager ]
 * Author       [ Yasheed Chen ]
 **********************************************************************/

#ifndef SHAPEMGR_H
#define SHAPEMGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#ifndef OBJ_H
#include "obj.h"
#endif
#ifndef GRAPH_H
#include "graph.h"
#endif

using namespace std;

class Range
{
public:
    Range(){}
    Range(int i, int j){ ymin = i; ymax = j; }
    ~Range(){}
    int get_ymin(){ return ymin; }
    int get_ymax(){ return ymax; }
private:
    int ymin;
    int ymax;
};

class ShapeMgr
{
public:
    ShapeMgr() {}
    ~ShapeMgr() {}
    // member functions about graph construction
    bool readObjects(const string&); // if read fails, return false
    // add member functions here
//    void sort_shapes_x();
//    void sort_shapes_y();
    void sort_one_layer_shapes_x(int i)
    { 
        _layer_objects[i-1]->sort_layer_shapes_x(); 
    } 
    void sort_one_layer_shapes_y(int i)
    { 
        _layer_objects[i-1]->sort_layer_shapes_y(); 
    }
    
    void sort_one_layer_obstacles_x(int i)
    {
        _layer_objects[i-1]->sort_layer_obstacles_x();
    }
    void sort_one_layer_obstacles_y(int i)
    {
        _layer_objects[i-1]->sort_layer_obstacles_y();
    }
    //void connect(int i){_layer_objects[i]->}
    
    int get_number_layers(){ return _n_metal_layers; }
    int get_number_shapes(){ return _n_routed_shapes; }
    int get_number_obstacles(){ return _n_obstacles; }
    
    int get_layer_number_shapes(int i)
    {
        return _layer_objects[i-1]->layer_object_shape_number();
    }
    
    int get_layer_number_obstacles(int i)
    {
        return _layer_objects[i-1]->layer_object_obstacle_number();
    }
    
    int get_xl_shapes(int i){ return _shapes[i]->getX_l(); }
    
    LayerObject*  get_layer_objects(int i) { return _layer_objects[i]; }
    
    
    void  get_connected_components_shapes(int i)
    { 
        Connected_Component_Shapes* j = new Connected_Component_Shapes(); 
        _connected_components_shapes.push_back(j);  
        _connected_components_shapes[i]->bbstconstruct(_layer_objects[i]);
    }
    void  get_connected_components_obstacles(int i)
    { 
        Connected_Component_Obstacles* j = new Connected_Component_Obstacles(); 
        _connected_components_obstacles.push_back(j);  
        _connected_components_obstacles[i]->bbstconstruct(_layer_objects[i]);
    }
    vector<Polygon*> get_shapes_polygons_layer(int i)
    { 
        return _connected_components_shapes[i]->get_polygons(); 
    }//get i layer's polygons
    vector<Polygon*> get_obstacles_polygons_layer(int i)
    { 
        return _connected_components_obstacles[i]->get_polygons(); 
    }//get i layer's polygons
    void construct_graph(int i);
    void sort_pins(int i){stable_sort(_pins[i].begin(),_pins[i].end(),compare_x);};
    void sort_pins_by_id(int i) { stable_sort(_pins[i].begin(), _pins[i].end(), compare_by_id); }
    void construct_vias();
    // member function about MST
    void construct_node_groups(); // convert connected_pins id to node groups
    void construct_MST(); // use Dikstra's Algorithm
    void MST_Prim(const string& filename);

    void getPinById();
    void Print_answer_by_node_pairs(Node* a, Node* b, const int count);
    void write_answer(Node* a, Node* b, const int count, const string& filename);
    
private:
    vector <Graph*> _graphs;
    vector <Shape*> _shapes;
    vector <Obstacle*> _obstacles;
    vector <Via*> _vias;
    vector <LayerObject*> _layer_objects; //vector of objects on different layers
    vector <Connected_Component_Shapes*> _connected_components_shapes;
    vector <Connected_Component_Obstacles*> _connected_components_obstacles;
    vector <map <int, vector<Range> > > _x_value;
    vector <vector <Pin>> _pins; //first vector means layer, second means the pin on that layer
    vector <vector<Connected_Pins>> _connected_pins;//store the connected pins that
    vector <Via*> _output_vias;
    //
    int _via_cost;
    int _spacing;
    Point _boundary_lower;
    Point _boundary_upper;
    int _n_metal_layers; // # of metal layers
    int _n_routed_shapes; // # of routed shapes
    int _n_routed_vias; // # of routed vias
    int _n_obstacles; // # of obstacles
    static bool compare_x(Pin pin1, Pin pin2)
    {
        return(pin1.get_X()<pin2.get_X());
    }
    static bool compare_id(int x, int y){ return (x > y); }
    static bool compare_by_id(Pin pin1, Pin pin2)
    {
        return(pin1.get_id() < pin2.get_id());
    }

};

#endif
