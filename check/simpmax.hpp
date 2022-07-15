//
//  simpmax.hpp
//  
//
//  (C) Alexander A. Gaifullin, 2022
//

#ifndef simpmax_hpp
#define simpmax_hpp


#include <stdio.h>
#include <set>
#include <fstream>

using namespace std;


class SimpMax {
public:
    set <unsigned long int> max_simplices; // set of maximal simplices of the complex
    
    int max_num_vert;                      // number m such that all vertices of the complex
                                           // are among 0, ..., m-1.
    
    SimpMax() {};                          // constructor for the empty complex
    
    SimpMax(int max_num_vert);             // constructor for the empty complex
                                           // with the given max_num_vert,
                                           // all vertices are ghost
    
    SimpMax (int max_num_vert, unsigned long int s); // constructor for the simplicial complex
                                                     // with the given max_num_vert
                                                     // containing a unique maximal simplex s
    
    SimpMax link (int v) const;   // Finds the link of the vertex v
    
    SimpMax costar (int v) const; // Finds the contrastar of the vertex v
    
    bool is_simplex () const;     // Checks if the simplicial complex is a nonempty simplex
    
    bool empty () const;          // Checks if the simplicial complex is empty
    
    bool contains (unsigned long int tau) const; // Checks if the simplicial complex
                                                 // contains the simplex tau
    
    void add (unsigned long int tau);            // Adds simplex tau to the simplicial complex
    
    void add (const SimpMax & another_complex);  // Adds to the simplicial complex all
                                                 // simplices of another_complex
    
    bool nonevasive () const;         // checks if the simplicial complex is nonevasive
};

#endif /* simpmax_hpp */
