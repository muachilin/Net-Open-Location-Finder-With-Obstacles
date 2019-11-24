/***********************************************************************
 * FileName     [ shapeMgr.cpp ]
 * Synopsis     [ Define shape manager functions ]
 * Author       [ Yasheed Chen ]
 **********************************************************************/

#include "shapeMgr.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <queue>
#include "../lib/tm_usage.h"
#ifndef GRAPH_H
#include<graph.h>
#endif

using namespace std;

/***********************************************/
/*   class shapeMgr about graph construction   */
/***********************************************/

// read in the file
bool ShapeMgr::readObjects(const string& fileName)
{
    
    string buffer;
    ifstream file;
    file.open(fileName, ifstream::in);
    file>>buffer; file>>buffer;  file>>buffer;    _via_cost = stoi(buffer);
    cout<<_via_cost<<endl;
    
    getline(file, buffer);
    file>>buffer; file>>buffer;  file>>buffer;    _spacing = stoi(buffer);
    cout<<_spacing<<endl;
    getline(file, buffer);
    file>>buffer; file>>buffer;  file>>buffer;
    int bx1,by1,bx2,by2;
    int bdot1=buffer.find(",");
    bx1= stoi(buffer.substr(1));
    by1= stoi(buffer.substr(bdot1+1));
    _boundary_lower = Point (bx1,by1);
    file>>buffer;
    int bdot2=buffer.find(",");
    bx2= stoi(buffer.substr(1));
    by2= stoi(buffer.substr(bdot2+1));
    _boundary_upper = Point (bx2,by2);
    
    getline(file, buffer);
    file>>buffer; file>>buffer;  file>>buffer;   _n_metal_layers = stoi(buffer);
    for(int i = 0; i < _n_metal_layers; i++)
    {
        LayerObject* lay = new LayerObject();
        _layer_objects.push_back(lay);
    }
    cout<<"finish constructing layer"<<endl;
    getline(file, buffer);
    file>>buffer; file>>buffer;  file>>buffer;   _n_routed_shapes = stoi(buffer);
    
    getline(file, buffer);
    file>>buffer; file>>buffer;  file>>buffer;   _n_routed_vias = stoi(buffer);
    
    getline(file, buffer);
    file>>buffer; file>>buffer;  file>>buffer;   _n_obstacles = stoi(buffer);
    cout<<_n_metal_layers<<' '<<_n_routed_shapes<<' '<<_n_routed_vias<<' '<<_n_obstacles<<endl;
    //Shape
    int _shapeid=1;
    for (size_t i=0; i < _n_routed_shapes ; ++i )
    {
        getline(file, buffer);
        file>>buffer; file>>buffer;
        int _layerid;    _layerid = stoi(buffer.substr(1));//M"X"
        file>>buffer;//read coordinate1
        int x1,y1;
        int dot1=buffer.find(",");
        x1=stoi(buffer.substr(1));
        y1=stoi(buffer.substr(dot1+1));
        file>>buffer;//read coordinate2
        int x2,y2;
        int dot2=buffer.find(",");
        x2=stoi(buffer.substr(1));
        y2=stoi(buffer.substr(dot2+1));
        
        Point *point1;
        Point *point2;
        point1 = new Point(x1,y1);
        point2 = new Point(x2,y2);
        
        Shape *routedshape;
        routedshape = new Shape( _layerid, _shapeid, *point1, *point2);
        _shapes.push_back(routedshape);//vector of shapes
        _layer_objects[_layerid-1]->add_shapes(routedshape);
        _shapeid++;
        
    }
    cout<<"shape finished"<<endl;
    //Vias
    int _viaid = 1;
    for (size_t i=0; i < _n_routed_vias; ++i)
    {
        getline(file, buffer);
        file>>buffer; file>>buffer;
        int _layerid;
        _layerid = stoi(buffer.substr(1));
        file>>buffer;
        int x1,y1;
        int dot1 = buffer.find(",");
        x1 = stoi(buffer.substr(1));
        y1 = stoi(buffer.substr(dot1+1));
        
        Point *point1;
        point1 = new Point(x1,y1);
        Via *via;
        via = new Via(_layerid,_viaid,*point1);
        _vias.push_back(via);
        _viaid++;
    }
    //Obstacle
    int _obstacleid=1;
    for (size_t i=0; i < _n_obstacles ; ++i )
    {
        getline(file, buffer);
        file>>buffer;  file>>buffer;
        int _layerid;
        _layerid = stoi(buffer.substr(1));//M"X"
        file>>buffer;//read coordinate1
        int x1,y1;
        int dot1=buffer.find(",");
        x1=stoi(buffer.substr(1));
        y1=stoi(buffer.substr(dot1+1));
        
        file>>buffer;//read coordinate2
        int x2,y2;
        int dot2=buffer.find(",");
        x2=stoi(buffer.substr(1));
        y2=stoi(buffer.substr(dot2+1));
        
        Point *point1;
        Point *point2;
        
        point1 = new Point(x1-_spacing,y1-_spacing);
        point2 = new Point(x2+_spacing,y2+_spacing);
        
        Obstacle *obstacle;

        obstacle= new Obstacle( _layerid, _obstacleid, *point1, *point2);
        _obstacles.push_back(obstacle);//vector of obstacles
        _layer_objects[_layerid-1]->add_obstacles(obstacle);
        _obstacleid++;
    }
    return true;
    cout<<1<<endl;
}



//Sort the routed shapes by x
//void ShapeMgr::sort_shapes_x()
//{
//    stable_sort(_shapes.begin(),_shapes.end(),compare_shape_x);
//}


//void ShapeMgr::sort_shapes_y()
//{
//    stable_sort(_shapes.begin(),_shapes.end(),compare_shape_y);
//}


void ShapeMgr::construct_vias()
{
    int _n_via_layers[_n_metal_layers - 1];
    for(int i = 0; i < _n_metal_layers - 1; i++)
    {
        _n_via_layers[i] = 0;
    }
    for(int i = 0; i < _n_routed_vias; i++)//check the layer that doesnot have vias
    {
        int layer = _vias[i]->getLayer();
        _n_via_layers[layer-1] = 1;
    }
    for(int i = 0; i < _n_metal_layers - 1; i++)
    {
        if(_n_via_layers[i] == 0)//no vias at this layer
        {
            for(int j = 0; j < _layer_objects[i]->layer_object_shape_number();j++)
            {
                int x = _layer_objects[i]->get_shapes_xl(j);
                int y = _layer_objects[i]->get_shapes_yd(j);//left down corner of this shape
                int _not_covered = 0;
                for(int k = 0; k < _layer_objects[i]->layer_object_shape_number();k++)
                {
                    if(k!=j)
                    {
                        if(x > _layer_objects[i]->get_shapes_xl(k) && x < _layer_objects[i]->get_shapes_xr(k) &&y > _layer_objects[i]->get_shapes_yd(k)&&y < _layer_objects[i]->get_shapes_yu(k))
                        {
                            _not_covered = 1;
                            cout<<"covered by shape in the same layer"<<endl;
                            break;
                        }
                    }
                }
                if(_not_covered == 1) continue;
                for(int k = 0; k < _layer_objects[i]->layer_object_obstacle_number();k++)
                {
                    if(k!=j)
                    {
                        if(x > _layer_objects[i]->get_obstacles_xl(k) && x < _layer_objects[i]->get_obstacles_xr(k) &&y > _layer_objects[i]->get_obstacles_yd(k)&&y < _layer_objects[i]->get_obstacles_yu(k))
                        {
                            _not_covered = 1;
                            cout<<"covered"<<endl;
                            break;
                        }
                    }
                }
                if(_not_covered == 1) continue;
                for(int k = 0; k < _layer_objects[i+1]->layer_object_shape_number();k++)
                {
                    
                    if(x > _layer_objects[i+1]->get_shapes_xl(k) && x < _layer_objects[i+1]->get_shapes_xr(k) &&y > _layer_objects[i+1]->get_shapes_yd(k)&&y < _layer_objects[i+1]->get_shapes_yu(k))
                    {
                        _not_covered = 1;
                        cout<<"covered by shape at other layer"<<endl;
                        break;
                    }
                    
                }
                if(_not_covered == 1) continue;
                for(int k = 0; k < _layer_objects[i+1]->layer_object_obstacle_number();k++)
                {
                    if(x > _layer_objects[i+1]->get_obstacles_xl(k) && x < _layer_objects[i+1]->get_obstacles_xr(k) &&y > _layer_objects[i+1]->get_obstacles_yd(k)&&y < _layer_objects[i+1]->get_obstacles_yu(k))
                    {
                        _not_covered = 1;
                        cout<<"covered by obstacle at other layer"<<endl;
                        break;
                    }
                }
                if(_not_covered == 1) continue;
                
                if(_not_covered == 0)
                {
                    Point p = Point(x,y);
                    Via* via_pushback = new Via(i+1,_output_vias.size(),p);
                    _output_vias.push_back(via_pushback);
                    cout<<"finish construct vias for a layer"<<endl;
                    break; //go to next layer
                }
            }
        }
    }
    
}


void ShapeMgr::construct_graph(int i)
{
    map<int, vector<Range>> x;
    _x_value.push_back(x);
    cout<<"Start Graph Construction"<<endl;
    cout<<"Start Finding the existing x coordinates"<<endl;
    LayerObject* layer_object = get_layer_objects(i);
    int shape_number = layer_object->layer_object_shape_number();
    for(int j=0; j <  shape_number; j++)
    {
        Shape* shape = layer_object->get_shapes(j);
        if  ( _x_value[i].count( shape->getX_l() )== false )
        {
            vector<Range> r;
            _x_value[i].insert (  pair< int, vector<Range> >( shape->getX_l(), r) );
        }
        if  ( _x_value[i].count( shape->getX_r() )== false )
        {
            vector<Range> r;
            _x_value[i].insert (  pair< int, vector<Range> >( shape->getX_r(), r) );
        }
    }
    int obstacle_number = layer_object->layer_object_obstacle_number();
    for(int j=0; j <  obstacle_number; j++)
    {
        Obstacle* obstacle = layer_object->get_obstacles(j);
        if  ( _x_value[i].count( obstacle->getX_l() )== false )
        {
            vector<Range> r;
            _x_value[i].insert (  pair< int, vector<Range> >( obstacle->getX_l(), r) );
        }
        if  ( _x_value[i].count( obstacle->getX_r() )== false )
        {
            vector<Range> r;
            _x_value[i].insert (  pair< int, vector<Range> >( obstacle->getX_r(), r) );
        }
    }
    
    cout<<"Start Constructing the range can't be passed"<<endl;
    for(int j=0; j <  shape_number; j++)
    {
        Shape* shape = layer_object->get_shapes(j);
        int yu = shape->getY_u();
        int yd = shape->getY_d();
        int xl = shape->getX_l();
        int xr = shape->getX_r();
        for ( auto iterator = _x_value[i].begin(); iterator!=_x_value[i].end(); ++iterator )
        {
            if(iterator->first >= xl && iterator->first < xr)
            {
                Range* r = new Range(yd,yu);
                iterator->second.push_back(*r);
            }
        }
    }
    
    for(int j=0; j <  obstacle_number; j++)
    {
        Obstacle* obstacle = layer_object->get_obstacles(j);
        int yu = obstacle->getY_u();
        int yd = obstacle->getY_d();
        int xl = obstacle->getX_l();
        int xr = obstacle->getX_r();
        for ( auto iterator = _x_value[i].begin(); iterator!=_x_value[i].end(); ++iterator )
        {
            if(iterator->first >= xl && iterator->first < xr)
            {
                Range* r = new Range(yd,yu);
                iterator->second.push_back(*r);
            }
        }
    }
    
    int real_yminimum = _boundary_lower.getY()+_spacing;
    int yminimum = _boundary_lower.getY();
    int real_ymaximum = _boundary_upper.getY()-_spacing;
    int ymaximum = _boundary_upper.getY();
    int real_xminimum = _boundary_lower.getX()+_spacing;
    int xminimum = _boundary_lower.getX();
    int real_xmaximum = _boundary_upper.getX()-_spacing;
    int xmaximum = _boundary_upper.getX();
    for ( auto iterator = _x_value[i].begin(); iterator!=_x_value[i].end(); ++iterator )
    {
        if(iterator->first >= (_boundary_lower.getX() + _spacing) && iterator->first <= (_boundary_upper.getX() - _spacing))
        {
            Range* r = new Range(yminimum,real_yminimum);
            iterator->second.push_back(*r);
            Range* r1 = new Range(ymaximum,real_ymaximum);
            iterator->second.push_back(*r1);
        }
    }
    
    for ( auto iterator = _x_value[i].begin(); iterator!=_x_value[i].end(); ++iterator )
    {
        if(iterator->first <= (_boundary_lower.getX() + _spacing) || iterator->first >= (_boundary_upper.getX() - _spacing))
        {
            Range* r = new Range(yminimum,ymaximum);
            iterator->second.push_back(*r);
        }
    }
    cout<<"Finished Constructing the range that can't be passed."<< endl;
    
    cout<<"Start Creating the Pins"<<endl;
    vector<Pin> pushbackneed;
    _pins.push_back(pushbackneed);
    vector<Connected_Pins> pushbackneed_2;
    _connected_pins.push_back(pushbackneed_2);

    vector<Polygon*> shapes_connected = get_shapes_polygons_layer(i);
    int number_connected_shapes = shapes_connected.size();
    cout<<"number of connected shapes are "<<number_connected_shapes<<endl;
    int idofpin = 0;
    for(int j = 0; j < number_connected_shapes ; j++)
    {
        vector <Point*> points_need_add = shapes_connected[j]->get_shape_point_polygon();
        int number_points = points_need_add.size();
        vector<int> connected_inputs;
        for(int k = 0; k < number_points; k++)
        {
            if((points_need_add[k]->getX()>=xminimum)&&(points_need_add[k]->getY()>=yminimum)&&(points_need_add[k]->getX()<=xmaximum)&&(points_need_add[k]->getY()<=ymaximum))
            {
                idofpin++;
                Pin push_back_pin = Pin(points_need_add[k]->getX(),points_need_add[k]->getY(),idofpin,1);
                _pins[i].push_back(push_back_pin);
                connected_inputs.push_back(idofpin);
            }
        }
        Connected_Pins push_back_connected;
        push_back_connected.add_vector_id(connected_inputs);
        _connected_pins[i].push_back(push_back_connected);
    }
    
    vector<Polygon*> obstacle_connected = get_obstacles_polygons_layer(i);
    int number_connected_obstacles = obstacle_connected.size();
    cout<<"number of connected obstacles are "<<number_connected_obstacles<<endl;
    for(int j = 0; j < number_connected_obstacles ; j++)
    {
        vector <Point*> points_need_add = obstacle_connected[j]->get_obstacle_point_polygon();
        int number_points = points_need_add.size();
        for(int k = 0; k < number_points; k++)
        {
            if((points_need_add[k]->getX()>=xminimum)&&(points_need_add[k]->getY()>=yminimum)&&(points_need_add[k]->getX()<=xmaximum)&&(points_need_add[k]->getY()<=ymaximum))
            {
                idofpin++;
                Pin push_back_pin = Pin(points_need_add[k]->getX(),points_need_add[k]->getY(),idofpin,2);
                _pins[i].push_back(push_back_pin);
            }
        }
    }
    
    int hihi = 0;
    for(int j = 0; j < _n_routed_vias; j++){
        if(_vias[j]->getLayer() == i || _vias[j]->getLayer() == (i+1)){
            hihi++;
            idofpin++;
            Pin push_back_pin = Pin(_vias[j]->get_point().getX(),_vias[j]->get_point().getY(),idofpin,3);
            _pins[i].push_back(push_back_pin);
        }
    }
    for(int j = 0; j < _output_vias.size(); j++)
    {
        if(_output_vias[j]->getLayer() == i || _output_vias[j]->getLayer() == (i+1)){
            hihi++;
            idofpin++;
            Pin push_back_pin = Pin(_output_vias[j]->get_point().getX(),_output_vias[j]->get_point().getY(),idofpin,3);
            _pins[i].push_back(push_back_pin);
        }
    }
    cout<<"number of vias:"<<hihi<<endl;
    
    Graph* j = new Graph("haha");
    _graphs.push_back(j);
    sort_pins(i);
    cout<<"Number of Pins are "<<_pins[i].size()<<endl;
    for(int j = 0; j < _pins[i].size(); j++)
    {
        int pin_x_value = _pins[i][j].get_X();
        int pin_y_value = _pins[i][j].get_Y();
        cout<<"Producing Edges for "<<_pins[i][j].get_id()<<" , the x of this point is "<<_pins[i][j].get_X()<<", the y of this point is "<<_pins[i][j].get_Y()<<endl;
        vector<Pin*> L_point_1;
        vector<Pin*> L_point_2;
        int now_x_1 = 0;
        int pre_x_1 = 0;
        int now_x_2 = 0;
        int pre_x_2 = 0;
        
        for(int k = 0; k < _pins[i].size(); k++)
        {
        
            
        
            if (_pins[i][k].get_X() >= pin_x_value)
            {
                if(_x_value[i].count(pre_x_1)==true)
                {
                    vector<Range> x_range = _x_value[i].find(pre_x_1)->second;
                    now_x_1 = _pins[i][k].get_X();
                    now_x_2 = _pins[i][k].get_X();
                    
                    for(int m = 0; m < x_range.size(); m++)
                    {
                        vector<int>points_need_to_erase_index;
                        for(int n = 0; n < L_point_1.size(); n++)
                        {
                            if((L_point_1[n]->get_Y() > x_range[m].get_ymin()&& x_range[m].get_ymin() > _pins[i][j].get_Y()) || (L_point_1[n]->get_Y() > x_range[m].get_ymin()&& x_range[m].get_ymax() > _pins[i][j].get_Y())){points_need_to_erase_index.push_back(n);}
                        }
                        sort(points_need_to_erase_index.begin(),points_need_to_erase_index.end(),compare_id);
                        for(int n = 0; n < points_need_to_erase_index.size(); n++){L_point_1.erase(L_point_1.begin()+points_need_to_erase_index[n]);}
                        
                    }
                    
                    
                    for(int m = 0; m < x_range.size(); m++)
                    {
                        vector<int>points_need_to_erase_index;
                        for(int n = 0; n < L_point_2.size(); n++)
                        {
                            if((L_point_2[n]->get_Y() < x_range[m].get_ymax()&& x_range[m].get_ymin() < _pins[i][j].get_Y()) || (L_point_2[n]->get_Y() < x_range[m].get_ymax()&& x_range[m].get_ymax() < _pins[i][j].get_Y())){points_need_to_erase_index.push_back(n);}
                        }
                        sort(points_need_to_erase_index.begin(),points_need_to_erase_index.end(),compare_id);
                        for(int n = 0; n < points_need_to_erase_index.size(); n++){L_point_2.erase(L_point_2.begin()+points_need_to_erase_index[n]);}
                    }
                    
                }

                break;
            }

            else
            {
                if(_pins[i][k]._block == 1)
                {
                    if(_x_value[i].count(pre_x_1)==true && pre_x_1 != now_x_1)
                    {
                        vector<Range> x_range = _x_value[i].find(pre_x_1)->second;
                        now_x_1 = _pins[i][k].get_X();
                        now_x_2 = _pins[i][k].get_X();
                        
                        for(int m = 0; m < x_range.size(); m++)
                        {
                            vector<int>points_need_to_erase_index;
                            for(int n = 0; n < L_point_1.size(); n++)
                            {
                                if((L_point_1[n]->get_Y() > x_range[m].get_ymin()&& x_range[m].get_ymin() > _pins[i][j].get_Y()) || (L_point_1[n]->get_Y() > x_range[m].get_ymin()&& x_range[m].get_ymax() > _pins[i][j].get_Y())){points_need_to_erase_index.push_back(n);}
                            }
                            sort(points_need_to_erase_index.begin(),points_need_to_erase_index.end(),compare_id);
                            for(int n = 0; n < points_need_to_erase_index.size(); n++){L_point_1.erase(L_point_1.begin()+points_need_to_erase_index[n]);}
                            
                        }
                        
                        
                        for(int m = 0; m < x_range.size(); m++)
                        {
                            vector<int>points_need_to_erase_index;
                            for(int n = 0; n < L_point_2.size(); n++)
                            {
                                if((L_point_2[n]->get_Y() < x_range[m].get_ymax()&& x_range[m].get_ymin() < _pins[i][j].get_Y()) || (L_point_2[n]->get_Y() < x_range[m].get_ymax()&& x_range[m].get_ymax() < _pins[i][j].get_Y())){points_need_to_erase_index.push_back(n);}
                            }
                            sort(points_need_to_erase_index.begin(),points_need_to_erase_index.end(),compare_id);
                            for(int n = 0; n < points_need_to_erase_index.size(); n++){L_point_2.erase(L_point_2.begin()+points_need_to_erase_index[n]);}
                    
                    }
                    pre_x_1 = _pins[i][k].get_X();
                    pre_x_2 = _pins[i][k].get_X();
                }
            }
                
                else if(_pins[i][k]._block == 0)
                {
                    if(_x_value[i].count(pre_x_1)==true)
                    {
                        vector<Range> x_range = _x_value[i].find(pre_x_1)->second;
                        now_x_1 = _pins[i][k].get_X();
                        now_x_2 = _pins[i][k].get_X();
                        
                        for(int m = 0; m < x_range.size(); m++)
                        {
                            vector<int>points_need_to_erase_index;
                            for(int n = 0; n < L_point_1.size(); n++)
                            {
                                if((L_point_1[n]->get_Y() > x_range[m].get_ymin()&& x_range[m].get_ymin() > _pins[i][j].get_Y()) || (L_point_1[n]->get_Y() > x_range[m].get_ymin()&& x_range[m].get_ymax() > _pins[i][j].get_Y())){points_need_to_erase_index.push_back(n);}
                                if(L_point_1[n]->get_Y() > x_range[m].get_ymin()&& L_point_1[n]->get_Y() < x_range[m].get_ymax()){L_point_1[n]->_block = 1;}
                            }
                            sort(points_need_to_erase_index.begin(),points_need_to_erase_index.end(),compare_id);
                            for(int n = 0; n < points_need_to_erase_index.size(); n++){L_point_1.erase(L_point_1.begin()+points_need_to_erase_index[n]);}
                            
                        }
                        
                        
                        for(int m = 0; m < x_range.size(); m++)
                        {
                            vector<int>points_need_to_erase_index;
                            for(int n = 0; n < L_point_2.size(); n++)
                            {
                                if((L_point_2[n]->get_Y() < x_range[m].get_ymax()&& x_range[m].get_ymin() < _pins[i][j].get_Y()) || (L_point_2[n]->get_Y() < x_range[m].get_ymax()&& x_range[m].get_ymax() < _pins[i][j].get_Y())){points_need_to_erase_index.push_back(n);}
                                if(L_point_2[n]->get_Y() > x_range[m].get_ymin()&& L_point_2[n]->get_Y() < x_range[m].get_ymax()){L_point_2[n]->_block = 1;}
                            }
                            sort(points_need_to_erase_index.begin(),points_need_to_erase_index.end(),compare_id);
                            for(int n = 0; n < points_need_to_erase_index.size(); n++){L_point_2.erase(L_point_2.begin()+points_need_to_erase_index[n]);}
                        }
                        
                    }
                    if (_pins[i][k].get_Y() <= pin_y_value){
                        L_point_2.push_back(&_pins[i][k]);
                    }
                    else if (_pins[i][k].get_Y() > pin_y_value){
                        L_point_1.push_back(&_pins[i][k]);
                    }
                    pre_x_1 = _pins[i][k].get_X();
                    pre_x_2 = _pins[i][k].get_X();
                }
                

            }


        }

	cout<<"Finish finding the possible connecting points"<<endl;
        for(int k = 0; k < L_point_1.size(); k++)
        {
            int weight = abs(_pins[i][j].get_X() - L_point_1[k]->get_X()) + abs(_pins[i][j].get_Y() -  L_point_1[k]->get_Y());
            _graphs[i]->addEdge(_pins[i][j].get_id(),L_point_1[k]->get_id(),weight);
            cout<<"create edge with "<<L_point_1[k]->get_id()<<" , the x of this point is "<<L_point_1[k]->get_X()<<", the y of this point is "<<L_point_1[k]->get_Y()<<endl;
        }
        for(int k = 0; k < L_point_2.size(); k++)
        {
            int weight = abs(_pins[i][j].get_X() - L_point_2[k]->get_X()) + abs(_pins[i][j].get_Y() -  L_point_2[k]->get_Y());
            _graphs[i]->addEdge(_pins[i][j].get_id(),L_point_2[k]->get_id(),weight);
            cout<<"create edge with "<<L_point_2[k]->get_id()<<" , the x of this point is "<<L_point_2[k]->get_X()<<", the y of this point is "<<L_point_2[k]->get_Y()<<endl;
        }
    }
    
    _graphs[i];
    cout<<"Finish Graph Construction"<<endl;

}

void 
ShapeMgr::construct_node_groups()
{
    cerr << "Constructing node groups..." << endl;
    for(int i = 0; i < _graphs.size(); ++i){ // through every layer
        for(int j = 0; j < _connected_pins[i].size(); ++j){ // for every connected_pins, add to group
            Group* g = new Group(j);
            cerr << "==new Group #" << j << endl;
            for(auto& id: _connected_pins[i][j].connected_id){ 
                g->add(_graphs[i]->getNodeById(id));
                cerr << "====Group #" << j << " add node #" << id << endl;
            }
            _graphs[i]->addGroup(g);
            cerr << "Graph #" << i << " add Group #" << g->getID() << endl;
        }
    }
}

void
ShapeMgr::construct_MST()
{
    for(auto& graph: _graphs) {
        graph->Dijkstra_all_nodes();
    }
    cout << "after constructing MST" << endl;
    for(auto& graph: _graphs) {
        cout << *graph->_g << endl;
    }
}

void
ShapeMgr::MST_Prim(const string& filename)
{
    cerr << "Primming..." << endl;
    for(auto& graph: _graphs) {
        (graph->_g)->Prim(graph->nodes[0]);
    }
    cerr << "after Primming" << endl;
    int count = 0;
    for(auto& graph: _graphs) {
        sort_pins_by_id(count);
        //cerr << *graph->_g << endl;
        cerr << "Prim Path: " << endl;
        for(auto& pair: graph->_g->Prim_path) {
            if(pair.first == NULL) cout << "NULL!!!" << endl;
            else
                cerr << "node #" << pair.first->id << " -> " << "node #" 
                    << pair.second->id << endl;
        }
        cerr << endl;
        cerr << "=============================================" << endl;
        /*cerr << "Prim Path cartesian" << endl;
        for(auto& pair: graph->_g->Prim_path_Cartesian) {
            if(pair.first == NULL) cout << "NULL!!!" << endl;
            else
                cerr << "(" << pair.first->id << ", " << 
                     pair.second->id << ")" << endl;
        }*/
        for(auto& pair: graph->_g->Prim_path_Cartesian) {
            Node* a = pair.first;
            Node* b = pair.second;
            if(a == NULL || b == NULL) cerr << "NULL!!!" << endl;
            else {
                Print_answer_by_node_pairs(a, b, count);
                write_answer(a, b, count, filename);
            }
        }
        ++count;
        
    }
}

void
ShapeMgr::Print_answer_by_node_pairs(Node* a, Node* b, const int count)
{
    Pin p1 = _pins[count][a->id-1];
    Pin p2 = _pins[count][b->id-1];
    int x1 = p1.get_X();
    int y1 = p1.get_Y();
    int x2 = p2.get_X();
    int y2 = p2.get_Y();
    if(y1 != y2)
        cout << "V-line M" << count << " (" << x1 << "," << y1 << ") (" << x1 << "," << y2 << ")" << endl; 
    if(x1 != x2)
        cout << "H-line M" << count << " (" << x1 << "," << y2 << ") (" << x2 << "," << y2 << ")" << endl;

}

void
ShapeMgr::write_answer(Node* a, Node* b, const int count, const string& filename)
{
    fstream fout;
    fout.open(filename, ios::out | ios::app);

    Pin p1 = _pins[count][a->id-1];
    Pin p2 = _pins[count][b->id-1];
    int x1 = p1.get_X();
    int y1 = p1.get_Y();
    int x2 = p2.get_X();
    int y2 = p2.get_Y();
    if(y1 != y2)
        fout << "V-line M" << count+1 << " (" << x1 << "," << y1 << ") (" 
            << x1 << "," << y2 << ")" << endl; 
    if(x1 != x2)
        fout << "H-line M" << count+1 << " (" << x1 << "," << y2 << ") (" 
            << x2 << "," << y2 << ")" << endl;

    // via
    for(auto& via: _output_vias) {
        fout << "Via V" << via->getLayer() << " (" << via->get_point().getX()
            << "," << via->get_point().getY() << ")" << endl;
    }
    
    fout.close();

}
