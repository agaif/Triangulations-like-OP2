//
//  myiofunctions.hpp
//  
//
//  (C) Alexander A. Gaifullin, 2022
//

#ifndef myiofunctions_hpp
#define myiofunctions_hpp

#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include "permutation.hpp"

using namespace std;

// function for reading from file and generating the symmetry group
bool generate_group (ifstream & file, int & degree, int & group_order,
                     vector <Permutation> & group_elements);
bool generate_group (ifstream & file, int & degree, int & group_order,
                     set <Permutation> & group_elements);

// functions for writing simplices to file
void print_simplices (ofstream & file, int number_of_vertices,
                      const set <unsigned long int> & simplices);
void print_simplices (ofstream & file, int number_of_vertices,
                      const vector <unsigned long int> & simplices);

// functions for transforming string of 0's and 1's to unsigned long int encoding a simplex
unsigned long int string_to_simplex (int & num, const string & str);
unsigned long int string_to_simplex (const string & str);

// function that transforms simplex encoded in 'unsigned long int'
// to string of 0's and 1's
string simplex_to_string (int number_of_vertices, unsigned long int s);

// function computing the number of vertices in the simplex
int num_vert (int number_of_vertices, unsigned long int s);

// functions for reading a triangulation from file
bool read_triang (ifstream & file, int number_of_vertices,
                  int & number_of_vertices_in_simplex,
                  int & number_of_orbits, set <unsigned long int> & orbit_rep);
bool read_triang (ifstream & file, int number_of_vertices,
                  set <unsigned long int> & orbit_rep);
bool read_triang (ifstream & file, int number_of_vertices,
                  int & number_of_vertices_in_simplex,
                  int & number_of_orbits, vector <unsigned long int> & orbit_rep);
bool read_triang (ifstream & file, int number_of_vertices,
                  vector <unsigned long int> & orbit_rep);


#endif /* myiofunctions_hpp */
