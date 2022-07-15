# Triangulations of manifolds like the octonionic projective plane

**Alexander A. Gaifullin**

This repository contains C++ programs implementing algorithms for the paper 

Alexander A. Gaifullin, "634 vertex-transitive and more than $10^{103}$ non-vertex-transitive 27-vertex triangulations of manifolds like the octonionic projective plane", arXiv???

which is further referred to as [G].

This README file contains only technical information on the realizations of the algorithms. For the aims and descriptions of the algorithms, and also for mathematical terminology used below, see the quoted paper [G].

## General information

Each directory, except for the directory **common**, contains a separate program. Below is the list of all these programs with their short descriptions. The directory **common** contains two libraries which are used by all the programs. Namely, **myiofunctions.cpp / myiofunctions.hpp** contain input/output functions and **permutation.cpp / permutation.hpp** contain the class **Permutation**, which allows working with elements of permutation groups. The description of these libraries is given below.

**List of programs:**

1. **find**:<br/> 
   Finds all weak pseudo-manifolds $K$ with the prescribed vertex set $V$, dimension $d$, and symmetry group $G$ that have at least $N$ maximal simplices and do not contain a pair of simplices $\sigma$ and $\tau$ such that $\sigma\cup\tau$ is the whole vertex set $V$. Implements the algorithm described in Section 4 of [G].
2. **check**: <br/> 
Checks that the given $G$-invariant simplicial complex $K$ is a combinatorial manifold. (A positive answer guarantees that $K$ is a combinatorial manifold but a negative answer does not guarantee that $K$ is not.) Implements the algorithm described in Section 5 of [G].
3. **permute**: <br/>
 This program, for a given $G$-invariant simplicial complex $K$ and an element $h$ in the normalizer of $G$, produces the $G$-invariant simplicial complex $h(K)$. This simple script was conveniently used to decompose the list of 24 combinatorial manifolds from Theorem 2.2 of [G] (which is produced by **find**) into 4 orbits with respect to the group $C_2\times C_3$ generated by the sign reversal and the Frobenius automorphism.
4. **num_neighbors**: <br/>
This program, for a given $d$-dimensional simplicial complex $K$, computes the distribution of the numbers $s(\rho)$ for $(d-2)$-simplices $\rho$, where $s(\rho)$ is the number of $d$-simplices $\sigma\in K$ containing $\rho$. It was used to produce Table 4 in [G]. 
5. **num_neighbors_pairs**: <br/>
This program, for a given $d$-dimensional simplicial complex $K$ and two given vertices $u$ and $v$ of it, computes the matrix $(N_{pq})$, where $N_{pq}$ is the number of $(d-1)$-simplices $\tau\in K$ such that $u,v\in\tau$, $s(\tau\setminus \\{u\\})=p$, and $s(\tau\setminus \\{v\\})=q$. It was used to produce Tables 6-10 in [G]. 
6. **triples**: <br/>
 This program computes the $\nu$-parameters of simplices of a pure $d$-dimensional simplicial complex. Besides, when $K$ is a combinatorial manifold, it finds all distinguished subcomplexes, that is, subcomplexes of the form $(\Delta_1*\partial\Delta_2)\cup(\Delta_2*\partial\Delta_3)\cup(\Delta_3*\partial\Delta_1)$, where $\Delta_1$, $\Delta_2$, and $\Delta_3$ are $(d/2)$-simplices. Computations by means of this program yield Propositions 8.2 and 8.4 in [G].
7. **allsimp**: <br/>
This auxiliary script produces the list of all simplices in some $G$-invariant set from a given symmetry group $G$ and a list of representatives of $G$-orbits of the simplices in this set. 
8. **operations**: <br/>
This simple script performs basic operations (compairing, union, intersection, difference) for any set of strings. It can be conveniently used to check whether two simplicial complexes coincide, find a common part of two simplicial complexes, etc. In particular, it was used to produce Table 3 and Tables 11-20 in [G].   

Among these programs, the only two non-trivial are the programs **find** and **check**. For them, we will produce detailed descriptions below. The other programs are very simple. They do not require any special explanation.

## Input/output format

The files with the input data are to be put in the directory of the program (for instance, **find**). The output files also are written to the same directory. Below, we first describe the standard formats that are used for main types of data (symmetry group, simplex, simplicial complex), and then the input/output data for all the 8 programs listed above. 

The main object we are working with is a pure $d$-dimensional simplicial complex with $n$ vertices numbered from $1$ to $n$ with the given symmetry group $G$. In the most important for us case, $d=16$, $n=27$, and $|G|=351$. 

### Standard format for a group $G$

The following description of the symmetry $G\subset S_n$ should be provided in the file **symmetry_group.dat**. The first line of the file contains the degree $n$ of the permutation group. Further lines contain generators of $G$, one generator per line. Each generator is given in cycle notation, cycles are surrounded by round brackets, vertices in each cycle are separated by spaces. Additional spaces are irrelevant. Empty lines are not allowed.  For instance, the $351$-element group $G_{351}\subset S_{27}$ is given by the following file: 
```
27
(1 2 3 4 5 6 7 8 9 10 11 12 13)(14 15 16 17 18 19 20 21 22 23 24 25 26)
(1 14 27)(2 4 10)(3 22 13)(5 6 21)(7 25 11)(8 19 18)(9 16 26)(12 20 24)(15 23 17)
```

### Standard format for a simplex

Every simplex is encoded by a row of $n$ binary digits. The $i^{th}$ from the left digit is 1 whenever $i$ is a vertex of the simplex, and is 0 otherwise. For instance, the row 
```
111010100000000
``` 
encodes the simplex $\\{1,2,3,5,7\\}$ in a simplicial complex with 15 vertices. We conveniently interpret every such 
row as a reversed binary notation for a number. For instance, the above row corresponds to the binary number
$1010111_2=87$. We order the simplices according to the corresponding numbers from smallest to largest in all lists we use.

### Standard format for a simplicial complex

We typically work with rather large $G$-invariant simplicial complexes.

The file describing a simplicial complex is usually called **triang.dat**.

The first line of the description of a simplicial complex contains the number $k$ of $G$-orbits of maximal simplices. The consecutive $k$ lines contain rows of 0's and 1's encoding representatives of $G$-orbits of maximal simplices as described above. The smallest representative must be chosen in each orbit and these representatives must be ordered from smallest to largest (w.r.t. the ordering described above). For instance, a file describing a simplicial complex can look like that:
```
286
111111111111110111000000000
111111111111011111000000000
111111101111111111000000000
. . . . .
110111110010111001111110000
```

The file **triang.dat** does not contain a description of the group $G$. So, in fact, a simplicial complex is described only by the two files **symmetry_group.dat** and **triang.dat** together. 

### Program "find"

**Input data:**

+ A file **symmetry_group.dat** describing the symmetry group $G$ in the standard format.
+ A file **dimnum.dat** containing two numbers $d$ and $N$ separated by a space (or several spaces); the first number $d$ is the dimension of weak pseudomanifolds $K$ we are looking for, and the second number $N$ is the smallest number of $d$-simplices in $K$, for instance,  
`16 100386` <br/> 
     

**Output data:**

+ A file **triangulations.dat** containing the list of all weak pseudomanifolds found. The information about each pseudomanifold starts with the line of the form <br/> <br/> 
` *** <number of the pseudomanifold> ` <br/><br/>
after which follows the description of the pseudomanifold in the standard format. Different pseudomanifolds are separated by empty lines. The numbering of pseudomanifolds starts from 1. So the file  **triangulations.dat** looks like that 
    ```
    *** 1
    286
    111111111111110111000000000
    . . . . . 
    110111110010111001111110000

    *** 2
    286
    111111111111111110000000000
    . . . . . 
    001111110110111010111110000

    . . . . . 
    ```

+ A file **triangulations_with_sizes.dat** containing the same list with the additionally information on the cardinalities of the $G$-orbits of maximal simplices. The cardinality of each $G$-orbit is written directly after the representative of this orbit (separated by space). For instance,
    ```
    *** 1
    286
    111111111111110111000000000 351
    . . . . . 
    110111110010111001111110000 351

    *** 2
    286
    111111111111111110000000000 351
    . . . . . 
    001111110110111010111110000 351

    . . . . . 
    ```
### Program "check"

**Input data:**

+ A file **symmetry_group.dat** describing the symmetry group $G$ in the standard format.
+ A file **triang.dat** describing a pure simplicial complex $K$ in the standard format.

**Output data:**

The program **check** says whether it has succeeded to check that $K$ is a combinatorial manifold. So there are two possibilities:

+ If the program succeeds to check that $K$ is a combinatorial manifold, then the output is just the line
    ``` 
    This is a combinatorial manifold
    ```

+ If the program does not succeed to check that $K$ is a combinatorial manifold, then it writes the smallest representatives of $G$-orbits of simplices for which it has failed to check that their links are combinatorial spheres to the file **bad_simplices.dat** in the standard format. Besides, the program produces the line 
     ```
     There are <number of orbits of bad simplices> bad simplices
     ```
     

**Remark.** If a simplex is put to the list of *bad simplices*, it does not follow that its link is not a combinatorial sphere. A positive result guarantees that $K$ is a combinatorial manifold but a negative result does not guarantee that $K$ is not.

### Program "permute"

**Input data:**

+ A file **symmetry_group.dat** describing the symmetry group $G$ in the standard format.
+ A file **triang.dat** describing a pure simplicial complex $K$ in the standard format.
+ A file **permutation.dat** consisting of a single line that contains a permutation $h$ in cycle notation. (Cycles are surrounded by round brackets, vertices in each cycle are separated by spaces.)

**Output data:**

First, the program finds out whether the permutation $h$ normalizes the group $G$. If $h$ does not normalize $G$, then the only output is the line

```
The permutation does not belong to the normalizer of the group
```

Second, if $h$ belongs to the normalizer of $G$, then $h(K)$ is again a $G$-invariant simplicial complex. Then the output is a file **triang_new.dat** containing $h(K)$ in the standard format.


### Program "num_neighbors"

**Input data:**

+ A file **symmetry_group.dat** describing the symmetry group $G$ in the standard format.
+ A file **triang.dat** describing a $d$-dimensional pure simplicial complex $K$ in the standard format.

**Output data:**

+ A file **result.dat**. Each line of this file contains two numbers $s$ and $N_s$ separated by a space, where $N_s$ is the number of $(d-2)$-simplices $\rho\in K$ with $s(\rho)=s$. Recall that $s(\rho)$ is the number of $d$-simplices $\sigma\in K$ containing $\rho.$ For instance, the file **result.dat** can look like that: 

    ```
    3 849771
    4 1509651
    5 697788
    6 201942
    7 40716
    8 9477
    9 351
    ```
      
### Program "num_neighbors_pairs"

**Input data:**

+ A file **symmetry_group.dat** describing the symmetry group $G$ in the standard format.
+ A file **triang.dat** describing a $d$-dimensional pure simplicial complex $K$ in the standard format.
+ A file **edge.dat** containing two (different) numbers $u$ and $v$ separated by a space. These are numbers of vertices, so they must be in the range  from 1 to the total number of vertices $n$. 

**Output data:**

+ A file **result.dat**. The first line of this file contains a single number $s_{max}$ that is equal to the maximal among all numbers $s(\tau\setminus\\{u\\})$ and $s(\tau\setminus\\{v\\})$, where $\tau$ runs over all $(d-1)$-dimensional simplices $\tau\in K$ containing both $u$ and $v$. The next $s_{max}$ lines contain the $s_{max}\times s_{max}$ matrix $N_{pq}$, where $N_{pq}$ is the number of $(d-1)$-simplices $\tau\in K$ such that $u,v\in\tau$, $s(\tau\setminus \\{u\\})=p$, and $s(\tau\setminus \\{v\\})=q$. (The entries of each row of the matrix are separated by spaces.)


### Program "triples"

**Input data:**

+ A file **symmetry_group.dat** describing the symmetry group $G$ in the standard format.
+ A file **triang.dat** describing a $d$-dimensional pure simplicial complex $K$ in the standard format.
+ The program asks the user whether to look for distinguished triples or not. <br/>
**Remark.** The reason for this request is that the procedure for finding the distinguished triples works correctly only when $K$ is a combinatorial manifold and $n=3d/2+3$. Nevertheless, computing the $\nu$-parameters may be useful in other situations as well.
 
**Output data:** 

+ A file **nu_param.dat**. This file lists the (smallest) representatives of all $G$-orbits of $d$-simplices $\sigma\in K$ with at least one $\nu$-parameter greater than or equal to $d/2$. The first line of the file contains the number $q$ of such $G$-orbits. Each of the next $q$ orbits contains a simplex $\sigma$ (in the standard format), and the $\nu$-parameters $\nu_K(\sigma,v_i)$ for $i=1,\ldots,n-d-1$, where $v_1 <\cdots < v_{n-d-1}$ are all the vertices not belonging to $\sigma$. These numbers are separated with spaces, for instance, 

    ```
    110110011101100 1 1 2 0 4 1
    ```

+ If the user has chosen to look for distinguished triples, then the program also produces a file
**distinguished.dat** containing information on representatives of $G$-orbits of distinguished triples. The description of each distinguished triple $(\Delta_1,\Delta_2,\Delta_3)$ consists of 4 lines. The first 3 lines contain the simplices $\Delta_1$, $\Delta_2$, and $\Delta_3$ (in the standard format), and the last line indicates the *type* of the triple. (A distinguished triple has *type 1* if the three simplices $\Delta_1$, $\Delta_2$, and $\Delta_3$ lie in the same $G$-orbit, and has *type 2* otherwise.) Descriptions of different distinguished triples are separated by empty lines. An example of the description of a triple is as follows: 
 
    ```
    001100110001111010000000000
    110011001110000100000000100
    000000000000000001111111011
    type 2
    ```

### Program "allsimp"

**Input data:**

+ A file **symmetry_group.dat** describing the symmetry group $G$ in the standard format.
+ A file **triang.dat** containing the list of representatives of a set of $G$-orbits of simplices. (The first line contains the number of $G$-orbits of $K$; the following lines contain the representatives in the standard format.)

**Output data:**

+ A file **triang_all.dat**. The first line contains the total number of simplices in the given $G$-orbits; the following lines contain the list of all these simplices in the standard format. 

### Program "operations"

**Input data:**

+ Files **triang1.dat** and  **triang2.dat**. The first line of each of these two files contains a single number indicating the number of subsequent lines. Then follows an arbitrary set of strings (one per line) without spaces or tabulations or empty lines.  
+ The user is asked to choose the type of the operation:

    ```
    Input the type of the operation:
      1 compare
      2 union
      3 intersection
      4 difference
    ```
    
**Output data:**

+ For the operation **compare**, the program gives one of the four answers: <br/>
`The complexes coincide` <br/>
`The first complex is contained in the second one` <br/>
`The second complex is contained in the first one` <br/>
`Neither of the complexes is contained in the other one` <br/>

+ For the operation **union**, the program units the two lists of strings (removing repetitions) and writes the result to the file **triang_res.dat**. (Again, the first line contains the number of subsequent strings, and then follows the list of strings.) 

+ Similarly the operations **intersection** and **difference** yield the intersection and the difference (the first minus the second) of the two lists, respectively, which are also written to **triang_res.dat**.

## Common libraries 

### Library permutation.cpp / permutation.hpp

The library consists of a single class **Permutation** whose objects are permutations in the group $S_d$ for certain $d$. Note that inside the program we always interpret $S_d$ as the permutation group of the set $\\{0,\\ldots,d-1\\}$. Nevertheless, in I/O functions we always subtract 1/add 1 to all elements so that to obtain permutations of the set $\\{1,\\ldots,d\\}$.

A permutation $\nu\in S_d$ is stored in the variable 
```cpp
vector <int> seq;
```
where `seq[i]` is the value $\nu(i)$. Both `i` and `seq[i]` vary from $0$ to $d-1$. Besides, the class contains the variable
```cpp
bool well_defined;
```
that points out whether the permutation is well defined.

The class contains three contructors:
```cpp
Permutation (int deg);
Permutation (int deg, string cycle_structure);
Permutation (vector <int> & seq);
```
The first one produces the identity permutation of degree `deg`. 

The second constructor produces the permutation from the cycle notation of it. Note that, in the cycle notation, the elements are numbered from `1` to `deg`, so we subtract 1 from every element. For instance,
```cpp
Permutation nu (3, "(12)");
```
yields the permutation `nu` with `nu.seq[0]`, `nu.seq[1]`, and `nu.seq[2]` equal to $1$, $0$, and $2$, respectively.

The third constructor produces the permutation with the given vector `seq`.

#### Methods of the class Permutation



### Library myiofunctions.cpp / myiofunctions.hpp
