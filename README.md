# Gonality
This code and its output makes up the data files for a PROMYS 2017 research lab investigating Matthew Baker's conjectures on Brill--Noether existence and gonality on graphs by Vandana Agarwala (Caltech), Soumen Ghosh (Indian Statistical Institute), Erik Jahn (ETH Zurich), Kenz Kallal (Harvard College), and Philip Lamkin (Carnegie Mellon University).

# Description of File Contents
gonality_check.cpp contains our implementation of Dhar's burning algorithm and checks Baker's gonality conjecture for 3-regular graphs. The k in k-regular can be changed by adjusting the macro in the code. After compilation, the proper usage is:
```
./gonality_check n infile outfile
```
This reads n graphs in adjacency-list format from infile and outputs one divisor per graph of degree floor((g+3)/2) and positive rank (or report failure if none is found). 

The files p1.txt, p2.txt, ..., p7.txt contain lists of integer partitions of the integers 1, ..., 7. They are used to avoid computing partitions each time gonality_conjecture is run (which could become more computationally intensive with larger graphs). 

The files cub06.g6, ..., cub18.g6 contain the 3-regular graphs on 6, 8, ..., 18 vertices in graph6 format, and the files cub06.txt, ..., cub18.txt contain the corresponding graphs in adjacency list format. Finally, the files div06.txt, ..., div18.txt contain one divisor of degree and rank prescribed by Baker's gonality conjecture for each graph in the corresponding file, one divisor (from one graph) on each line.

The graph6-format files for all the cubic graphs were generated by snarkhunter using a classical algorithm of Brinkmann, Goedgebeur and McKay. They were obtained from [House of Graphs](https://hog.grinvin.org/Cubic).
