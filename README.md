# Net Open Location Finder With Obstacles

Introduction
-------------
    
    Finding the shortest path for several terminals is a well known unsolvable problem. For example, problem such as "Steiner Tree Problem", the decision variant of the problem in graphs is NP-complete (which implies that the optimization variant is NP-hard). In our problem, we have several layers rectangle routed net shapes that need to be connected, and some rectangle obstacles that can’t be crossed. This should also not be able to solved by polynomial time. Thus, the only way we can process this is to use some heuristics to approach the minimum path. There are some previous works which give some good approach to the problems such as obstacle-avoiding RMST(Rectilinear Steiner minimal tree) problem and multilayer OARSMT(Obstacle avoiding rectilinear Steiner minimal tree). These past approaches give us some idea to handle this problem.

![image](https://github.com/muachilin/Net-Open-Location-Finder-With-Obstacles/blob/master/problem_example.png)
