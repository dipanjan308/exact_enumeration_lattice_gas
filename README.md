# Exact enumeration of lattice gas model
These codes enumerate all configurations for the system of hard particles (dimer,  mixture of dimers and 2 X 2 squares, 1-NN particles) on square lattice with periodic boundary conditions along both directions, using "breadth-first" and "depth-first" algorithm. Codes for dimer model and 1-NN model uses only "depth-first" algorithm. 1-NN is the nearest neighbour exclusion model in which lattice sites upto 1st nearest neighbour are excluded from being occupied by the particles. The codes can be generalised to the lattice models of any particle shape.

Breadth-first algorithm needs the information about all the configurations in previous level to enumerate the configurations in the current level. Here the level 
denote the total number of particles and current level configurations mean the configurations with on added particle in the previous level configurations. Because 
of this breadth-first algorithm is constrained by the available memory to store all the configurations in a given level. On the other hand depth-first algorithm 
does not have such drawback and should be able to enumerate all the configurations for any system sizes. However total number of configurations increases drastically
with increasing system size and the algorithm takes a lot of time to enumerate all such configurations. For example total number of configurations in 1-NN model of
system size 4, 5, 6 and 8 are respectively 743, 25531, 2406862 and 213256442503. Data files obtained from the codes are also attached for both 1-NN and dimer
lattice-gas models.

Both algorithms are obtained from the book [Statistical Mechanics: Algorithms and computations](https://global.oup.com/ukhe/product/statistical-mechanics-algorithms-and-computations-9780198515364?cc=gb&lang=en&), by Werner Krauth.
