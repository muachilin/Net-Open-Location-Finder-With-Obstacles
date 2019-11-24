# Net Open Location Finder With Obstacles

Problem
--------

    This problem is Problem B in 2017 CAD Contest at ICCAD (International Conference On Computer Aided Design).
    
    
Introduction
-------------
    
    We need to connect all the routed net shapes and the vias while minimizing total wire length cost. 
    There are three kinds of objects in this problem. They are routed net shapes R, routed net vias V, and obstacles O. 
    They are blue, yellow, and grey in the following picture, respectively. 
    There are designed boundary B which is the range where our paths can located, 
    and the minimum spacing S which is the spacing that we need to keep with the obstacles and the boundary. 
    In this problem, there are multiple layers. And the cost which required to cross the layers are Cv. 
    The routing paths have to be horizontal or vertical.

![image](https://github.com/muachilin/Net-Open-Location-Finder-With-Obstacles/blob/master/problem_example.png)

Steps
======

    1. Find the Overlapped Routed Net Shapes and the Obstacles
    2. Construct the Routing Edge
    3. Construct Prim's Minimum Spanning Tree
