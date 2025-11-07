# Process Sim Project
I am trying to write a semiconductor process simulator similar to Suprem-IV.

Original Software: Suprem-4 is a two-dimensional process-simulation program for silicon integrated circuits. 
https://github.com/cogenda/Suprem4
Yes, the Suprem4 code published on GitHub by cogenda is open source under a permissive, non-commercial license.

This the broad plan
1. Use Eigen C++ solver and Gmsh meshing system
First get the 1D simulator going on Eigen and solve a toy problem (diffusion equation) - Done

2. Clone the Suprem codebase and run the examples to compare our results with

3. Implement a simple 1D problem (diffusion, say) and compare results with the Suprem.