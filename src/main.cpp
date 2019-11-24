/***********************************************************************
 * FileName     [ main.cpp ]
 * Synopsis     [ Define main() ]
 * Author       [ Yasheed Chen ]
 **********************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#ifndef SHAPEMGR_H
#include "shapeMgr.h"
#endif
#include "../lib/util.h"

using namespace std;

ShapeMgr* shapeMgr = new ShapeMgr();

static void
usage()
{
    cout << "usage: net_open_finder <input.txt> <output.txt>" << endl;
}

static void
myexit()
{
    usage();
    exit(-1);
}
int main(int argc, char** argv)
{
    //read in the file
    if(argc == 3) {
        if(!shapeMgr->readObjects(argv[1])){
            cerr << "Error: read failed!! " << endl;
            myexit();
        }
    } 
    else {
        cerr << "Error: illegal number of argument (" << argc << ")!!\n";
        myexit();
    }
    cout<<"Finish reading files!"<<endl;
    
    for(int i = 0; i < shapeMgr->get_number_layers(); ++i)
    {
        shapeMgr->sort_one_layer_shapes_y(i+1);
        shapeMgr->sort_one_layer_shapes_x(i+1);
        
        shapeMgr->sort_one_layer_obstacles_y(i+1);
        shapeMgr->sort_one_layer_obstacles_x(i+1);
    }
    cout<<"check sorting"<<endl;
    
   for(int i=0; i < shapeMgr->get_number_layers(); ++i)
    {
        cout<<"LayerShapes "<<i+1<<" start"<<endl;
        for (int j=0; j< shapeMgr->get_layer_number_shapes(i+1) ; ++j)
        {
            
            cout<<shapeMgr->get_layer_objects(i)->get_shapes_xl(j)<<"  "<<shapeMgr->get_layer_objects(i)->get_shapes_yd(j)<<endl;
        }
        
        cout<<"LayerShapes "<<i+1<<" end"<<endl;
        
        
        
    }
    cout<<"Finish routedshapes sorting!"<<endl;
    
     
    
    for(int i=0; i < shapeMgr->get_number_layers() ; ++i)
    {
        cout<<"LayerObstacle "<<i+1<<" start"<<endl;
        for (int j=0; j< shapeMgr->get_layer_number_obstacles(i+1); ++j)
        {
            
            cout<<shapeMgr->get_layer_objects(i)->get_obstacles_xl(j)<<"  "<<shapeMgr->get_layer_objects(i)->get_obstacles_yd(j)<<endl;
            
        }
        cout<<"LayerObstacle "<<i+1<<" end"<<endl;
        
    }
    cout<<"Finish obstacle sorting!"<<endl;
    shapeMgr->construct_vias();    
    for(int i=0; i < shapeMgr->get_number_layers(); ++i)
    {
        cout<<"Layer Number: "<< i+1 <<endl;
        shapeMgr->get_connected_components_shapes(i);
        shapeMgr->get_connected_components_obstacles(i);
        shapeMgr->construct_graph(i);
    }

    shapeMgr->construct_node_groups();
    shapeMgr->construct_MST();
    shapeMgr->MST_Prim(argv[2]);
    return 0;
}



