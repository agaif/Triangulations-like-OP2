# Triangulations of manifolds like the octonionic projective plane

This repository contains C++ programs implementing algorithms for the paper A.A. Gaifullin, "634 vertex-transitive and more than $10^{103}$ non-vertex-transitive 27-vertex triangulations of manifolds like the octonionic projective plane", arXiv???

This README file contains only technical information on the realizations of the algorithms. For the aims and descriptions of the algorithms, and also for mathematical terminology used below, see the quoted paper.

## General information

Each directory, except for the directory **common**, contains a separate program. Below is the list of all these programs with their short descriptions. The directory **common** contains two **.cpp** files (together with the corresponding **.hpp** files) which are used by all the programs. Namely, **myiofunctions.cpp / myiofunctions.hpp** contain input/output functions and **permutation.cpp / permutation.hpp** contain the class **Permutation**, which allows working with elements of permutation groups.

**List of programs:**

1. **find** - finds all weak pseudo-manifolds $K$ with the prescribed vertex set $V$, dimension $d$, and symmetry group $G$ that have at least $N$ maximal simplices and do not contain a pair of simplices $\sigma$ and $\tau$ such that $\sigma\cup\tau$ is the whole vertex set $V$. 
2. **check** - checks that the given simplicial complex $K$ is a combinatorial manifold. (A positive answer guarantees that $K$ is a combinatorial manifold but a negative answer does not guarantee that $K$ is not.)
3. **num_neighbors** - for a given $d$-dimensional simplicial complex $K$, computes the distribution of the numbers $n(\rho)$ over $(d-2)$-simplices $\rho$, where $n(\rho)$ is the number of $d$-simplices $\sigma\in K$ containing $\rho$. 
4. **num_neighbors_pairs** - for a given $d$-dimensional simplicial complex $K$ and two given vertices $u$ and $v$ of it, computes the matrix $(N_{pq})$, where $N_{pq}$ is the number of $(d-1)$-simplices $\tau\in K$ such that $u,v\in\tau$, $n(\tau\setminus u)=p$, and $n(\tau\setminus v)=q$. 
5. **triple** - finds all distinguished subcomplexes of a combinatorial $d$-manifold $K$, that is, subcomplexes of the form $(\Delta_1*\partial\Delta_2)\cup(\Delta_2*\partial\Delta_3)\cup(\Delta_3*\partial\Delta_1)$, where $\Delta_1$, $\Delta_2$, and $\Delta_3$ are $(d/2)$-simplices.
6. **nu_param** - computes the $\nu$-parameters of simplices of a pure $d$-dimensional simplicial complex.
7. **allsimp** - given a symmetry group $G$ and a list of representatives of $G$-orbits of simplices produces the list of all simplices in these orbits.
8. **operations** - performs basic operations (compairing, union, intersection, difference) for simplicial complexes.  

Among these programs, the only two non-trivial are the programs **find** and **check**. For them, we will produce detailed descriptions below. The other programs are very simple. They do not require any special explanation.

## Input/output format
