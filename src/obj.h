/***********************************************************************
 * FileName     [ obj.h ]
 * Synopsis     [ Define objects ]
 * Author       [ Yasheed Chen ]
 **********************************************************************/

#ifndef OBJ_H
#define OBJ_H

#include <vector>
#include <map>
#include <list>
#include <set>
#include "graph.h"
using namespace std;

class Pin
{
public:
    Pin(){}
    ~Pin(){}
    Pin(int i, int j, int idofpin,int k) :  _block(0),_id(idofpin),_x(i),_y(j), _type(k){}
    int get_X(){return _x;}
    int get_Y(){return _y;}
    int get_type(){return _type;}
    int get_id(){return _id;}
    int _block; //0 is unblocked 1 is blocked
private:
    int _id; //id of the pin on the graph, which is also the id of the node
    int _x;
    int _y;
    int _type; //1 is routed shape 2 is obstacle 3 is vias

};
class Connected_Pins
{
public:
    Connected_Pins(){}
    ~Connected_Pins(){}
    void add_vector_id(vector<int> connected_id_input){connected_id = connected_id_input;}
//private:
    vector<int> connected_id; //id of pins that are originally connected

};

class Point
{
public:
    Point() {}
    Point(int x, int y) : _x(x), _y(y) {}
    ~Point() {}
    int getX() { return _x; }
    int getY() { return _y; }
private:
    int _x;
    int _y;
};


class Shape
{
public:
    Shape() {}
    Shape(int l, int id, Point LD, Point RU) : _l(l), _id(id), _LDcorner(LD), _RUcorner(RU) 
    {
        _LUcorner = Point(_LDcorner.getX(), _RUcorner.getY());
        _RDcorner = Point(_RUcorner.getX(), _LDcorner.getY());
    }
    ~Shape() {}

    Point* get_ld(){return &_LDcorner;}
    Point* get_lu(){return &_LUcorner;}
    Point* get_rd(){return &_RDcorner;}
    Point* get_ru(){return &_RUcorner;}
    int getLayer() { return _l; }
    int getID() { return _id; }
    int getX_l(){ return _LDcorner.getX();}
    int getX_r(){ return _RDcorner.getX();}
    int getY_d(){ return _RDcorner.getY();}
    int getY_u(){ return _RUcorner.getY();}
    //int getLorR;
    /*int LorR(Shape* s, int x)
    {
        if ( x == (s->getX_l()) ){return 1;}//left
        else if ( x == (s->getX_r()) ) {return 2;}//right
        else {return 0;}//this point does not belongs to  this shape
    }
     */
    int LorR(int x)
    {
        if ( x == (_LDcorner.getX()) ){return 1;}//left
        else if ( x == ( _RUcorner.getX() ) ) {return 2;}//right
        else {return 0;}//this point does not belongs to  this shape
    }
    bool point_on_the_shape(Point* i)
    {
        if ( (i->getX()>= _LDcorner.getX()) &&( i->getX()<= _RUcorner.getX())&&  (i->getY()>= _RDcorner.getY() )&& (i->getY()<= _LUcorner.getY()) )
        {
            return true;
        }
        else { return false; }
    }

    
private:
    int _l;
    int _id;
    Point _LDcorner;
    Point _RUcorner;
    Point _LUcorner;
    Point _RDcorner;
   
};

class Via
{
public:
    Via(){}
    Via(int l, int id, Point P) : _l(l), _id(id), _viapoint(P){}
    ~Via() {}
    int getLayer(){return _l;}
    int getID(){return _id;}
    Point get_point(){return _viapoint;}
private:
    int _l;
    int _id;
    Point _viapoint;
};

class Obstacle
{
public:
    Obstacle() {}
    Obstacle(int l, int id, Point LD, Point RU) :  _l(l), _id(id), _LDcorner(LD), _RUcorner(RU)
    {
        _LUcorner = Point(_LDcorner.getX(), _RUcorner.getY());
        _RDcorner = Point(_RUcorner.getX(), _LDcorner.getY());
    }
    ~Obstacle() {}
    Point* get_ld(){return &_LDcorner;}
    Point* get_lu(){return &_LUcorner;}
    Point* get_rd(){return &_RDcorner;}
    Point* get_ru(){return &_RUcorner;}

    int getLayer() { return _l; }
    int getID() { return _id; }
    int getX_l(){ return _LDcorner.getX();}
    int getX_r(){ return _RDcorner.getX();}
    int getY_d(){ return _RDcorner.getY();}
    int getY_u(){ return _RUcorner.getY();}
    
    int LorR(int x)
    {
        if ( x == (_LDcorner.getX()) ){return 1;}//left
        else if ( x == ( _RUcorner.getX() ) ) {return 2;}//right
        else {return 0;}//this point does not belongs to  this shape
    }
    bool point_on_the_obstacle(Point* i)
    {
        if ( (i->getX()>= _LDcorner.getX()) &&( i->getX()<= _RUcorner.getX())&&  (i->getY()>= _RDcorner.getY() )&& (i->getY()<= _LUcorner.getY()) )
        {
            return true;
        }
        else { return false; }
    }

private:
    int _l;
    int _id;
    Point _LDcorner;
    Point _RUcorner;
    Point _LUcorner;
    Point _RDcorner;
};

class Diff_place
{
public:
    Diff_place() {}
    ~Diff_place() {}
    
private:
    vector <Obstacle*> _obstacles;
    vector <Shape*> _shapes;
    // int id;
};


class LayerObject
{
public:
    LayerObject() {}
    ~LayerObject() {}
    void add_shapes(Shape* j){_shapes.push_back(j);}
    void add_obstacles(Obstacle* j){_obstacles.push_back(j);}
    
    Shape* get_shapes(int i) {return _shapes[i];}
    Obstacle* get_obstacles(int i){return _obstacles[i];}
    
    int get_shapes_xl(int i){return _shapes[i]->getX_l();}
    int get_shapes_xr(int i){return _shapes[i]->getX_r();}
    int get_shapes_yd(int i){return _shapes[i]->getY_d();}
    int get_shapes_yu(int i){return _shapes[i]->getY_u();}
    int get_obstacles_xl(int i){return _obstacles[i]->getX_l();}
    int get_obstacles_xr(int i){return _obstacles[i]->getX_r();}
    int get_obstacles_yd(int i){return _obstacles[i]->getY_d();}
    int get_obstacles_yu(int i){return _obstacles[i]->getY_u();}
    
    int layer_object_shape_number(){return _shapes.size();}
    int layer_object_obstacle_number(){return _obstacles.size();}
    
    void sort_layer_shapes_x(){stable_sort(_shapes.begin(),_shapes.end(),compare_shape_x);}
    void sort_layer_shapes_y(){stable_sort(_shapes.begin(),_shapes.end(),compare_shape_y);}
    
    void sort_layer_obstacles_x(){stable_sort(_obstacles.begin(),_obstacles.end(),compare_obstacle_x);}
    void sort_layer_obstacles_y(){stable_sort(_obstacles.begin(),_obstacles.end(),compare_obstacle_y);}
   // <vector <Shape*> >  get_vector_shapes{return _shapes;}
    
    
private:
    vector <Shape*> _shapes;
    vector <Obstacle*> _obstacles;
    //comparison function
    static bool compare_shape_x( Shape* i,  Shape* j){return i->getX_l() < j->getX_l();}
    static bool compare_shape_y( Shape* i,  Shape* j){return i->getY_d() < j->getY_d();}
    
    static bool compare_obstacle_x( Obstacle* i,  Obstacle* j){return i->getX_l() < j->getX_l();}
    static bool compare_obstacle_y( Obstacle* i,  Obstacle* j){return i->getY_d() < j->getY_d();}
    
    vector < vector < Diff_place* > > _diff_places;
    
    //vector <Connected_Component> _connected_components;
};

class Polygon
{
public:
    Polygon();
    Polygon(int i, Point* p)
    {
        if(i==1){point_of_shapes_polygon.push_back(p);}
        else if(i==2) {point_of_obstacles_polygon.push_back(p);}
        
    }
    ~Polygon();
    void polygonShapes(Point* p){point_of_shapes_polygon . push_back(p) ;}
    void polygonObstacles(Point* p){point_of_obstacles_polygon.push_back(p) ;}
    void add_points_shapes_polygon(Point* p){ point_of_shapes_polygon.push_back(p);}
    void add_points_obstacles_polygon(Point* p){ point_of_obstacles_polygon . push_back(p);}
    Point* getpoint_of_obstacles(int i){return point_of_obstacles_polygon[i];}
    vector <Point*> get_shape_point_polygon(){return point_of_shapes_polygon;}
    vector <Point*> get_obstacle_point_polygon(){return point_of_obstacles_polygon;}
    
private:
    vector <Point*> point_of_obstacles_polygon;
    vector <Point*> point_of_shapes_polygon;
    
};


class Connected_Component_Shapes  // Construct a vector of this, then
{
public:
    Connected_Component_Shapes() {}
    ~Connected_Component_Shapes() {}
    /*vector<Shape*> getMin(map<int, vector<Shape*> > mymap)
    {
        pair<int, vector<Shape*> > min = *min_element(mymap.begin(), mymap.end(), compare);//min is an iterator
        return min.second;
    }
    */
    
    void bbstconstruct(LayerObject* l)
    {
        //step1
        for (int i=0; i < (l->layer_object_shape_number()); ++i)
        {
            
            if  ( bbst.count( l->get_shapes_xl(i) )== false )
            {
                vector<Shape*> s;
                s.push_back(l->get_shapes(i));
                bbst.insert (  pair< int, vector<Shape*> >( l->get_shapes_xl(i), s) );
            }
            else if ( bbst.count( l->get_shapes_xl(i) )== true )
            {
                (    bbst.find(l->get_shapes_xl(i))  ->second ).push_back(l->get_shapes(i));
            }
            if  ( bbst.count( l->get_shapes_xr(i) )== false)
            {
                vector<Shape*> s;
                s.push_back(l->get_shapes(i));
                bbst.insert (  pair< int, vector<Shape*> >( l->get_shapes_xr(i),  s ) );
            }
            else if ( bbst.count( l->get_shapes_xr(i) )== true )
            {
                (  bbst.find(l->get_shapes_xr(i))->second) .push_back(l->get_shapes(i));
            }
        }
        for(auto iterator = bbst.begin(); iterator != bbst.end(); iterator++)
        {
            cout<< "now is x = " << iterator->first <<endl;
            for(int j = 0; j < iterator->second.size();j++)
            {
                cout<<"the shape is "<<iterator->second[j]->getID()<<endl;
            }
        }
        cout<<"OK1"<<endl;
        
        //step2
       
        
        int size_bbst = bbst.size();
        for (int i=0; i < size_bbst; ++i)
        {
            cout<<i<<"th iteration"<<endl;
        
            set <Shape*> need_to_delete;
            
            auto it = *min_element( bbst.begin(), bbst.end(),
                                   [](pair<int, vector<Shape*> > l, pair<int, vector<Shape*> > r) -> bool { return l.first < r.first; });
            //vector <Shape*> x = getMin(bbst);
            cout<<"it -> x is "<<it.first<<endl;
            vector <Shape*> x =it.second ;
            bbst.erase( min_element(bbst.begin(), bbst.end(), compare)); //extract min
            x_coord=it.first;
            
            for (int j=0; j < x.size(); ++j)
            {
                if(   x[j] -> LorR ( x_coord )== 1)// 1:left
                {
                    
                    b1.insert (   x[j] );
                    set <Shape*> s= { x[j] };
                    
                    
                    // set<Shape*> ::iterator it;
                    // s.insert(it, x[j]);
                    set <Shape*>  ::iterator it0;
                    for (it0 = b1.begin(); it0!=b1.end(); ++it0)
                    {
                        
                        if( (*it0) != x[j])  //for object_inter in B1/Rk
                        {
                            if( (x[j]->getY_d() <= ((*it0)->getY_u())) && (x[j]->getY_d() >= ((*it0)->getY_d())) )
                            {
                                
                                for (int m=0; m<A.size(); ++m)  //search for "c" that Ac contains object in b1
                                {
                                    set<Shape*>::iterator it1;
                                    it1= A[m].find(*it0);
                                    if ((*it1)==(*it0))
                                    {   A_c=m; cout<<"A_c ="<<A_c<<endl; break;}
                                }
                                set <Shape*> t1=A[A_c];  t1.insert(s.begin(), s.end() ); //union
                                set <Shape*> possible_delete_set = s;
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                A[A_c]=t1;  s=A[A_c];
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                if(possible_delete_set.size()!=1 && s.size() != possible_delete_set.size())
                                {
                                    vector<set<Shape*>>::iterator it2;
                                    it2 = find (A.begin(), A.end(), possible_delete_set);
                                    cout<<"after find"<<endl;
                                    A.erase(it2);
                                    cout<<"erase set"<<endl;
                                }
                            }
                            
                            else if( (x[j]->getY_u() >= ((*it0)->getY_d())) && (x[j]->getY_u() <= ((*it0)->getY_u())) )
                            {
                                
                                for (int m=0; m<A.size(); ++m)  //search for "c" that Ac contains object in b1
                                {
                                    set<Shape*>::iterator it1;
                                    it1= A[m].find(*it0);
                                    if ((*it1)==(*it0))
                                    {   A_c=m; cout<<"A_c = "<<A_c<<endl; break; }
                                }
                                set <Shape*> t2=A[A_c];  t2.insert(s.begin(), s.end() );//union
                                set <Shape*> possible_delete_set = s;
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                A[A_c]=t2;  s=A[A_c];
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                if(possible_delete_set.size()!=1 && s.size() != possible_delete_set.size())
                                {
                                    vector<set<Shape*>>::iterator it2;
                                    it2 = find (A.begin(), A.end(), possible_delete_set);
                                    cout<<"after find"<<endl;
                                    A.erase(it2);
                                    cout<<"erase set"<<endl;
                                }
                            }
                            
                            else if( (x[j]->getY_u() >= ((*it0)->getY_u())) && (x[j]->getY_d() <= ((*it0)->getY_d())) )
                            {
                                
                                for (int m=0; m<A.size(); ++m)  //search for "c" that Ac contains object in b1
                                {
                                    set<Shape*>::iterator it1;
                                    it1= A[m].find(*it0);
                                    if ((*it1)==(*it0))
                                    {   A_c=m; cout<<"A_c = "<<A_c<<endl; break; }
                                }
                                set <Shape*> t2=A[A_c];  t2.insert(s.begin(), s.end() );//union
                                set <Shape*> possible_delete_set = s;
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                A[A_c]=t2;  s=A[A_c];
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                if(possible_delete_set.size()!=1 && s.size() != possible_delete_set.size())
                                {
                                    vector<set<Shape*>>::iterator it2;
                                    it2 = find (A.begin(), A.end(), possible_delete_set);
                                    cout<<"after find"<<endl;
                                    A.erase(it2);
                                    cout<<"erase set"<<endl;
                                }
                            }
                            
                            else if( (x[j]->getY_u() <= ((*it0)->getY_u())) && (x[j]->getY_d() >= ((*it0)->getY_d())) )
                            {
                                
                                for (int m=0; m<A.size(); ++m)  //search for "c" that Ac contains object in b1
                                {
                                    set<Shape*>::iterator it1;
                                    it1= A[m].find(*it0);
                                    if ((*it1)==(*it0))
                                    {   A_c=m; cout<<"A_c = "<<A_c<<endl; break; }
                                }
                                set <Shape*> t2=A[A_c];  t2.insert(s.begin(), s.end() );//union
                                set <Shape*> possible_delete_set = s;
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                A[A_c]=t2;  s=A[A_c];
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                if(possible_delete_set.size()!=1 && s.size() != possible_delete_set.size())
                                {
                                    vector<set<Shape*>>::iterator it2;
                                    it2 = find (A.begin(), A.end(), possible_delete_set);
                                    cout<<"after find"<<endl;
                                    A.erase(it2);
                                    cout<<"erase set"<<endl;
                                }
                            }
                        }
                    }
                    if(s.size()==1)  {  A.push_back(s); cout<<"create set"<<endl; }
                    cout<<"insert OK"<<endl;
                }
                else if( x[j] -> LorR (x_coord )== 2 )//2:right
                {
                    need_to_delete.insert ( x[j] );
                    cout<<"need to delete"<<endl;
                }
                
            }
            //for objects in need_to_delete
            set <Shape*>  ::iterator it3;
            for (it3 = need_to_delete.begin(); it3!=need_to_delete.end(); ++it3)
            {  b1.erase(*it3) ; }
            
        }
        
        cout<<"Connected Components are"<<endl;
        for(int i = 0 ; i < A.size(); i++)
        {
            cout<<"connected "<<i<<endl;
            set<Shape*>::iterator it;
            for(it=A[i].begin(); it!=A[i].end(); ++it)
            {
                cout<<"The shape id is "<<(*it)->getID()<<endl;
            }
        }
        //step3
        
        number_of_sets=A.size();
        cout<<"number_of_sets: "<<number_of_sets<<endl;
        
        for (int i=0; i < number_of_sets ; ++i)
        {
            vector <Shape*> l;
            
            Polygon *poly;
            
            vector <Point*> poi;
            
            Shape* ii1 ;
            
                for (set<Shape*>::iterator it1=A[i].begin(); it1!=A[i].end(); ++it1)
                {
                    
                    int x[4];
                    for(int j = 0; j< 4; j++){x[j] = 0;}
                    
                    if( l.size()==0 )
                    {
                        poi.push_back((*it1)->get_ld());
                        poi.push_back((*it1)->get_lu());
                        poi.push_back((*it1)->get_rd());
                        poi.push_back((*it1)->get_ru());
                        l.push_back(*it1);
                        continue;
                    }
                   Shape* ii = *it1;
                    ii1=ii;
                    for (int k=0; k< l.size(); ++k)
                    {
                        
                        if(  ( l[k]->point_on_the_shape)( (ii)->get_ld()) ==true )
                        { x[0]=1; }
                        if(  (l[k]->point_on_the_shape)( (ii)->get_lu()) == true )
                        { x[1]=1; }
                        if(  (l[k]->point_on_the_shape)( (ii)->get_rd()) == true )
                        { x[2]=1; }
                        if(  (l[k]->point_on_the_shape)( (ii)->get_ru()) == true )
                        { x[3]=1; }
                        
                        if( ii-> point_on_the_shape (l[k]->get_lu())==true)
                        {
                            
                            for (vector <Point*>::iterator q=poi.begin(); q != poi.end(); ++q)
                            {
                                if(  (*q) == (l[k]->get_lu())  )
                                {
                                    poi.erase(q);
                                    break;
                                }
                            }
                        }
                        if( ii -> point_on_the_shape (l[k]->get_ld())==true)
                        {
                            for (vector <Point*>::iterator q=poi.begin(); q != poi.end(); ++q)
                            {
                                if(  (*q) == (l[k]->get_ld())  )
                                {
                                    poi.erase(q);
                                    break;
                                }
                            }
                        }
                        if( ii -> point_on_the_shape (l[k]->get_ru())==true)
                        {
                            for (vector <Point*>::iterator q=poi.begin(); q != poi.end(); ++q)
                            {
                                if(  (*q) == (l[k]->get_ru())  )
                                {
                                    poi.erase(q);
                                    break;
                                }
                            }
                        
                        }
                        if( ii -> point_on_the_shape (l[k]->get_rd())==true)
                        {
                            for (vector <Point*>::iterator q=poi.begin(); q != poi.end(); ++q)
                            {
                                if(  (*q) == (l[k]->get_rd())  )
                                {
                                    poi.erase(q);
                                    break;
                                }
                            }
                         }
                    
                       }//small for
                     if(x[0]==0){poi.push_back((ii)->get_ld());}
                     if(x[1]==0){poi.push_back((ii)->get_lu());}
                     if(x[2]==0){poi.push_back((ii)->get_rd());}
                     if(x[3]==0){poi.push_back((ii)->get_ru());}
                    
                    l.push_back(ii1);
                    
                  }
            poly= new Polygon (1,poi[0]);
            cout<<"poi size : "<<poi.size()<<endl;
            for(int m=1; m < poi.size(); ++m)
            {
                poly->add_points_shapes_polygon (poi[m]);
            }
            polygon_shapes . push_back(poly);
        }
        cout<<polygon_shapes.size()<<endl<<endl;
        
    }
    vector<Polygon*> get_polygons(){return polygon_shapes;}
private:
    int number_of_sets;
    int A_c;
    int x_coord;
    map <int, vector<Shape*> > bbst;
    set <Shape* > b1;
    vector < set <Shape*> > A;
    vector <Polygon*> polygon_shapes;
    
    static bool compare(pair<int, vector<Shape*> > i, pair<int, vector<Shape*> > j) { return i.first < j.first; }
    
};

class Connected_Component_Obstacles  // Construct a vector of this, then
{
public:
    Connected_Component_Obstacles() {}
    ~Connected_Component_Obstacles() {}
   
    
    void bbstconstruct(LayerObject* l)
    {
        //step1
        for (int i=0; i < (l->layer_object_obstacle_number()); ++i)
        {
            
            if  ( bbst.count( l->get_obstacles_xl(i) )== false )
            {
                vector<Obstacle*> s;
                s.push_back(l->get_obstacles(i));
                bbst.insert (  pair< int, vector<Obstacle*> >( l->get_obstacles_xl(i), s) );
            }
            else if ( bbst.count( l->get_obstacles_xl(i) )== true )
            {
                (    bbst.find(l->get_obstacles_xl(i))  ->second ).push_back(l->get_obstacles(i));
            }
            if  ( bbst.count( l->get_obstacles_xr(i) )== false)
            {
                vector<Obstacle*> s;
                s.push_back(l->get_obstacles(i));
                bbst.insert (  pair< int, vector<Obstacle*> >( l->get_obstacles_xr(i),  s ) );
            }
            else if ( bbst.count( l->get_obstacles_xr(i) )== true )
            {
                (  bbst.find(l->get_obstacles_xr(i))->second) .push_back(l->get_obstacles(i));
            }
        }
        for(auto iterator = bbst.begin(); iterator != bbst.end(); iterator++)
        {
            cout<< "now is x = " << iterator->first <<endl;
            for(int j = 0; j < iterator->second.size();j++)
            {
                cout<<"the obstacle is "<<iterator->second[j]->getID()<<endl;
            }
        }
       
        
        //step2
        
        int size_bbst = bbst.size();
        for (int i=0; i < size_bbst; ++i)
        {
            cout<<i<<"th iteration"<<endl;
            
            set <Obstacle*> need_to_delete;
            
            auto it = *min_element( bbst.begin(), bbst.end(),
                                   [](pair<int, vector<Obstacle*> > l, pair<int, vector<Obstacle*> > r) -> bool { return l.first < r.first; });
            //vector <Shape*> x = getMin(bbst);
            cout<<"it -> x is "<<it.first<<endl;
            vector <Obstacle*> x =it.second ;
            bbst.erase( min_element(bbst.begin(), bbst.end(), compare)); //extract min
            x_coord=it.first;
            
            for (int j=0; j < x.size(); ++j)
            {
                if(   x[j] -> LorR ( x_coord )== 1)// 1:left
                {
                    
                    b1.insert (   x[j] );
                    set <Obstacle*> s= { x[j] };
                    
                    
                    // set<Shape*> ::iterator it;
                    // s.insert(it, x[j]);
                    set <Obstacle*>  ::iterator it0;
                    for (it0 = b1.begin(); it0!=b1.end(); ++it0)
                    {
                        
                        if( (*it0) != x[j])  //for object_inter in B1/Rk
                        {
                            if( (x[j]->getY_d() < ((*it0)->getY_u())) && (x[j]->getY_d() > ((*it0)->getY_d())) )
                            {
                                
                                for (int m=0; m<A.size(); ++m)  //search for "c" that Ac contains object in b1
                                {
                                    set<Obstacle*>::iterator it1;
                                    it1= A[m].find(*it0);
                                    if ((*it1)==(*it0))
                                    {   A_c=m; cout<<"A_c ="<<A_c<<endl; break;}
                                }
                                set <Obstacle*> t1=A[A_c];  t1.insert(s.begin(), s.end() ); //union
                                set <Obstacle*> possible_delete_set = s;
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                A[A_c]=t1;  s=A[A_c];
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                if(possible_delete_set.size()!=1 && s.size() != possible_delete_set.size())
                                {
                                    vector<set<Obstacle*> >::iterator it2;
                                    it2 = find (A.begin(), A.end(), possible_delete_set);
                                    cout<<"after find"<<endl;
                                    A.erase(it2);
                                    cout<<"erase set"<<endl;
                                }
                            }
                            
                            else if( (x[j]->getY_u() > ((*it0)->getY_d())) && (x[j]->getY_u() < ((*it0)->getY_u())) )
                            {
                                
                                for (int m=0; m<A.size(); ++m)  //search for "c" that Ac contains object in b1
                                {
                                    set<Obstacle*>::iterator it1;
                                    it1= A[m].find(*it0);
                                    if ((*it1)==(*it0))
                                    {   A_c=m; cout<<"A_c = "<<A_c<<endl; break; }
                                }
                                set <Obstacle*> t2=A[A_c];  t2.insert(s.begin(), s.end() );//union
                                set <Obstacle*> possible_delete_set = s;
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                A[A_c]=t2;  s=A[A_c];
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                if(possible_delete_set.size()!=1 && s.size() != possible_delete_set.size())
                                {
                                    vector<set<Obstacle*> >::iterator it2;
                                    it2 = find (A.begin(), A.end(), possible_delete_set);
                                    cout<<"after find"<<endl;
                                    A.erase(it2);
                                    cout<<"erase set"<<endl;
                                }
                            }
                            else if( (x[j]->getY_u() >= ((*it0)->getY_u())) && (x[j]->getY_d() <= ((*it0)->getY_d())) )
                            {
                                
                                for (int m=0; m<A.size(); ++m)  //search for "c" that Ac contains object in b1
                                {
                                    set<Obstacle*>::iterator it1;
                                    it1= A[m].find(*it0);
                                    if ((*it1)==(*it0))
                                    {   A_c=m; cout<<"A_c = "<<A_c<<endl; break; }
                                }
                                set <Obstacle*> t2=A[A_c];  t2.insert(s.begin(), s.end() );//union
                                set <Obstacle*> possible_delete_set = s;
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                A[A_c]=t2;  s=A[A_c];
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                if(possible_delete_set.size()!=1 && s.size() != possible_delete_set.size())
                                {
                                    vector<set<Obstacle*> >::iterator it2;
                                    it2 = find (A.begin(), A.end(), possible_delete_set);
                                    cout<<"after find"<<endl;
                                    A.erase(it2);
                                    cout<<"erase set"<<endl;
                                }
                            }
                            else if( (x[j]->getY_u() <= ((*it0)->getY_u())) && (x[j]->getY_d() >= ((*it0)->getY_d())) )
                            {
                                
                                for (int m=0; m<A.size(); ++m)  //search for "c" that Ac contains object in b1
                                {
                                    set<Obstacle*>::iterator it1;
                                    it1= A[m].find(*it0);
                                    if ((*it1)==(*it0))
                                    {   A_c=m; cout<<"A_c = "<<A_c<<endl; break; }
                                }
                                set <Obstacle*> t2=A[A_c];  t2.insert(s.begin(), s.end() );//union
                                set <Obstacle*> possible_delete_set = s;
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                A[A_c]=t2;  s=A[A_c];
                                cout<<"size "<<s.size()<<" "<<possible_delete_set.size()<<endl;
                                if(possible_delete_set.size()!=1 && s.size() != possible_delete_set.size())
                                {
                                    vector<set<Obstacle*> >::iterator it2;
                                    it2 = find (A.begin(), A.end(), possible_delete_set);
                                    cout<<"after find"<<endl;
                                    A.erase(it2);
                                    cout<<"erase set"<<endl;
                                }
                            }
                        }
                    }
                    if(s.size()==1)  {  A.push_back(s); cout<<"create set"<<endl; }
                    cout<<"insert OK"<<endl;
                }
                else if( x[j] -> LorR (x_coord )== 2 )//2:right
                {
                    need_to_delete.insert ( x[j] );
                    cout<<"need to delete"<<endl;
                }
                
            }
            
            set <Obstacle*>  ::iterator it3;
            for (it3 = need_to_delete.begin(); it3!=need_to_delete.end(); ++it3)
            {  b1.erase(*it3) ; }
            
        }
        
        cout<<"Connected Components are"<<endl;
        for(int i = 0 ; i < A.size(); i++)
        {
            cout<<"connected "<<i<<endl;
            set<Obstacle*>::iterator it;
            for(it=A[i].begin(); it!=A[i].end(); ++it)
            {
                cout<<"The shape id is "<<(*it)->getID()<<endl;
            }
        }
        //step3
        
        number_of_sets=A.size();
        cout<<"number_of_sets: "<<number_of_sets<<endl;
        
        for (int i=0; i < number_of_sets ; ++i)
        {
            vector <Obstacle*> l;
            
            Polygon *poly;
            
            vector <Point*> poi;
            
            Obstacle* ii1 ;
            
            for (set<Obstacle*>::iterator it1=A[i].begin(); it1!=A[i].end(); ++it1)
            {
                
                int x[4];
                for(int j = 0; j< 4; j++){x[j] = 0;}
                
                if( l.size()==0 )
                {
                    poi.push_back((*it1)->get_ld());cout<<"push back"<<endl;
                    poi.push_back((*it1)->get_lu());cout<<"push back"<<endl;
                    poi.push_back((*it1)->get_rd());cout<<"push back"<<endl;
                    poi.push_back((*it1)->get_ru());cout<<"push back"<<endl;
                    l.push_back(*it1);
                    cout<<"Continue"<<endl;
                    continue;
                }
                Obstacle* ii = *it1;
                ii1=ii;
                cout<<"size over zero"<<endl;
                for (int k=0; k< l.size(); ++k)
                {
                    
                    if(  ( l[k]->point_on_the_obstacle)( (ii)->get_ld()) ==true )
                    { x[0]=1; }
                    if(  (l[k]->point_on_the_obstacle)( (ii)->get_lu()) == true )
                    { x[1]=1; }
                    if(  (l[k]->point_on_the_obstacle)( (ii)->get_rd()) == true )
                    { x[2]=1; }
                    if(  (l[k]->point_on_the_obstacle)( (ii)->get_ru()) == true )
                    { x[3]=1; }
                    
                    if( ii-> point_on_the_obstacle (l[k]->get_lu())==true)
                    {
                        
                        for (vector <Point*>::iterator q=poi.begin(); q != poi.end(); ++q)
                        {
                            if(  (*q) == (l[k]->get_lu())  )
                            {
                                poi.erase(q);cout<<"erase point"<<endl;
                                break;
                            }
                        }
                    }
                    if( ii -> point_on_the_obstacle (l[k]->get_ld())==true)
                    {
                        for (vector <Point*>::iterator q=poi.begin(); q != poi.end(); ++q)
                        {
                            if(  (*q) == (l[k]->get_ld())  )
                            {
                                poi.erase(q);cout<<"erase point"<<endl;
                                break;
                            }
                        }
                    }
                    if( ii -> point_on_the_obstacle (l[k]->get_ru())==true)
                    {
                        for (vector <Point*>::iterator q=poi.begin(); q != poi.end(); ++q)
                        {
                            if(  (*q) == (l[k]->get_ru())  )
                            {
                                poi.erase(q);cout<<"erase point"<<endl;
                                break;
                            }
                        }
                        
                    }
                    if( ii -> point_on_the_obstacle (l[k]->get_rd())==true)
                    {
                        for (vector <Point*>::iterator q=poi.begin(); q != poi.end(); ++q)
                        {
                            if(  (*q) == (l[k]->get_rd())  )
                            {
                                poi.erase(q);cout<<"erase point"<<endl;
                                break;
                            }
                        }
                    }
                    
                }
                if(x[0]==0){poi.push_back((ii)->get_ld());cout<<"push back"<<endl;}
                if(x[1]==0){poi.push_back((ii)->get_lu());cout<<"push back"<<endl;}
                if(x[2]==0){poi.push_back((ii)->get_rd());cout<<"push back"<<endl;}
                if(x[3]==0){poi.push_back((ii)->get_ru());cout<<"push back"<<endl;}
                
                l.push_back(ii1);
                
            }
            poly= new Polygon(2,poi[0]);
            cout<<"poi size : "<<poi.size()<<endl;
            for(int m=1; m < poi.size(); ++m)
            {
                poly->add_points_obstacles_polygon (poi[m]);
                cout<<poi[m]->getX()<<endl;
            }
            polygon_obstacles . push_back(poly);
        }
        cout<<polygon_obstacles.size()<<endl<<endl;
        
    }
    vector<Polygon*> get_polygons(){return polygon_obstacles;}
private:
    int number_of_sets;
    int A_c;
    int x_coord;
    map <int, vector<Obstacle*> > bbst;
    set <Obstacle* > b1;
    vector < set <Obstacle*> > A;
    vector <Polygon*> polygon_obstacles;
    
    static bool compare(pair<int, vector<Obstacle*> > i, pair<int, vector<Obstacle*> > j) { return i.first < j.first; }
    
};

#endif
