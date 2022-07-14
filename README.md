# Triangulations of manifolds like the octonionic projective plane

This repository contains C++ programs implementing algorithms for the paper A.A. Gaifullin, "634 vertex-transitive and more than $10^{103}$ non-vertex-transitive 27-vertex triangulations of manifolds like the octonionic projective plane", arXiv???

This README file contains only technical information on the realizations of the algorithms. For the aims and descriptions of the algorithms, and also for mathematical terminology used below, see the quoted paper.

## General information

Each directory, except for the directory **common**, contains a separate program. Below is the list of all these programs with their short descriptions. The directory **common** contains two **.cpp** files (together with the corresponding **.hpp** files) which are used by all the programs. Namely, **myiofunctions.cpp / myiofunctions.hpp** contain input/output functions and **permutation.cpp / permutation.hpp** contain the class **Permutation**, which allows working with elements of permutation groups.

**List of programs:**

1. **find** - finds all weak pseudo-manifolds $K$ with the prescribed vertex set $V$, dimension $d$, and symmetry group $G$ that have at least $N$ maximal simplices and do not contain a pair of simplices $\sigma$ and $\tau$ such that $\sigma\cup\tau$ is the whole vertex set $V$. 
2. **check** - checks that the given simplicial complex $K$ is a combinatorial manifold. (A positive answer guarantees that $K$ is a combinatorial manifold but a negative answer does not guarantee that $K$ is not.)
3. **num_neighbors** - for a given $d$-dimensional simplicial complex $K$, computes the distribution of the numbers $n(\rho)$ over $(d-2)$-simplices $\rho$, where $n(\rho)$ is the number of $d$-simplices $\sigma\in K$ containing $\rho$. 
4. **num_neighbors_pairs** - for a given $d$-dimensional simplicial complex $K$ and two given vertices $u$ and $v$ of it, computes the matrix $(N_{pq})$, where $N_{pq}$ is the number of $(d-1)$-simplices $\tau\in K$ such that $u,v\in\tau$, $n(\tau\setminus \\{u\\})=p$, and $n(\tau\setminus \\{v\\})=q$. 
5. **triple** - finds all distinguished subcomplexes of a combinatorial $d$-manifold $K$, that is, subcomplexes of the form $(\Delta_1*\partial\Delta_2)\cup(\Delta_2*\partial\Delta_3)\cup(\Delta_3*\partial\Delta_1)$, where $\Delta_1$, $\Delta_2$, and $\Delta_3$ are $(d/2)$-simplices.
6. **nu_param** - computes the $\nu$-parameters of simplices of a pure $d$-dimensional simplicial complex.
7. **allsimp** - given a symmetry group $G$ and a list of representatives of $G$-orbits of simplices produces the list of all simplices in these orbits.
8. **operations** - performs basic operations (compairing, union, intersection, difference) for simplicial complexes.  

Among these programs, the only two non-trivial are the programs **find** and **check**. For them, we will produce detailed descriptions below. The other programs are very simple. They do not require any special explanation.

## Input/output format

The main object we are working with is a pure $d$-dimensional simplicial complex with $n$ vertices numbered from $1$ to $n$ with the given symmetry group $G$. In the most important for us case, $d=16$, $n=27$, and $|G|=351$. 

### Format for the group $G$

The following description of the symmetry $G\subset S_n$ should be provided in the file **symmetry_group.dat**. The first line of the file contains the degree $n$ of the permutation group. Further lines contain generators of $G$, one generator per line. Each generator is given in cycle notation, cycles are surrounded by round brackets, vertices in each cycle are separated by spaces. Additional spaces are irrelevant. Empty lines are not allowed.  For instance, the $351$-element group $G_{351}\subset S_{27}$ is given by the following file:

>27
>
>(1 2 3 4 5 6 7 8 9 10 11 12 13)(14 15 16 17 18 19 20 21 22 23 24 25 26)
>
>(1 14 27)(2 4 10)(3 22 13)(5 6 21)(7 25 11)(8 19 18)(9 16 26)(12 20 24)(15 23 17)

### Format for a simplex

Every simplex is encoded by a row of $n$ binary digits. The $i$ th from the left digit is 1 whenever $i$ is a vertex of the simplex, and is 0 otherwise. For instance, the row

>111010100000000

encodes the simplex $\\{1,2,3,5,7\\}$ in a simplicial complex with 15 vertices. We conveniently interpret every such 
row as a reversed binary notation for a number. For instance, the above row corresponds to the number

> $1010111_2=87$ 

