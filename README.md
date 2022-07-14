# Triangulations of manifolds like the octonionic projective plane

This repository contains C++ programs implementing algorithms for the paper A.A. Gaifullin, "634 vertex-transitive and more than $10^{103}$ non-vertex-transitive 27-vertex triangulations of manifolds like the octonionic projective plane", arXiv???

This README file contains only technical information on the realizations of the algorithms. For the aims and descriptions of the algorithms, see the quoted paper.

## General information

Each directory, except for the directory **common**, contains a separate program. Below is the list of all these programs with their short descriptions. The directory **common** contains two **.cpp** files (together with the corresponding **.hpp** files) which are used by all the programs. Namely, **myiofunctions.cpp / myiofunctions.hpp** contain input/output functions and **permutation.cpp / permutation.hpp** contain the class **Permutation**, which allows working with elements of permutation groups.

**List of programs:**

1. **find** - finds all weak pseudo-manifolds $K$ with the prescribed vertex set $V$, dimension $d$, and symmetry group $G$ that have at least $N$ maximal simplices and do not contain a pair of simplices $\sigma$ and $\tau$ such that $\sigma\cup\tau$ is the whole vertex set $V$. 
2. **check** - checks that the given simplicial complex $K$ is a combinatorial manifold. (A positive answer guarantees that $K$ is a combinatorial manifold but a negative answer does not guarantee that $K$ is not.)
3. **num_neighbors** - for a given $d$-dimensional simplicial complex $K$, computes the distribution of the numbers $n(\rho)$ over $(d-2)$-simplices $\rho$, where $n(\rho)$ is the number of $d$-simplices $\sigma\in K$ containing $\rho$. 
4. **num_neighbors_pairs** - for a given $d$-dimensional simplicial complex $K$ and two given vertices $u$ and $v$ of it, computes the matrix $(N_{pq})$, where $N_{pq}$ is the number of $(d-1)$-simplices $\tau\in K$ such that $u,v\in\tau$, $n(\tau\setminus u)=p$ and $n(\tau\setminus v)=q$. 
