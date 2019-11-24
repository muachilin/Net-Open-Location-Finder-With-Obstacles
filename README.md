# Net Open Location Finder With Obstacles

Problem Source
--------

    This project is based on the Problem B in 2017 CAD Contest 
    at ICCAD (International Conference On Computer Aided Design).
    
    
Introduction
-------------
    
    We need to connect all net shapes and vias using rectilinear paths while minimizing wire length cost. 
    There are three kinds of objects in this problem. 
    They are routed net shapes R, routed net vias V, and obstacles O. 
    They are blue, yellow, and grey in the following picture, respectively. 
    Besides, boundary B is defined as the range where our paths can located.
    And the minimum spacing S is defined as the required spacing between shapes and the boundary. 
    In this problem, there are also multiple layers.
    Therefore, we have to connect shapes among layers while considering vias costs.

![image](https://github.com/muachilin/Net-Open-Location-Finder-With-Obstacles/blob/master/problem_example.png)

Steps
======

    1. Find the overlapping routing net shapes and the obstacles
    2. Construct the routing edge candidates
    3. Construct Prim's minimum spanning tree
