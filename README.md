# Triangulations of manifolds like the octonionic projective plane

This repository contains C++ programs that implement algorithms for the paper A.A. Gaifullin, "634 vertex-transitive and more than $10^{103}$ non-vertex-transitive 27-vertex triangulations of manifolds like the octonionic projective plane", arXiv???

This README file contains only technical information on the realizations of the algorithms. For the aims and descriptions of the algorithms, see the quoted paper.

## General information

Each directory, except for the directory **common**, contains a separate program. Below is the list of all these programs with their short descriptions. The directory **common** contains two **.cpp** files (together with the corresponding **.hpp** files) that are used by all the programs. Namely, **myiofunctions.cpp / myiofunctions.hpp** contains input/output functions and **permutation.cpp / permutation.hpp** contains the class **Permutation** that allows working with elements of permutation groups.
