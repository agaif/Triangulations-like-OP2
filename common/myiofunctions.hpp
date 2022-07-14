//
//  myiofunctions.hpp
//  
//
//  Created by Александр on 08.06.22.
//

#ifndef myiofunctions_hpp
#define myiofunctions_hpp

#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include "permutation.hpp"

using namespace std;

bool generate_group (ifstream & file, int & degree, int & group_order,
                     vector <Permutation> & group_elements);

void print_simplices (ofstream & file, int number_of_vertices,
                      const set <unsigned long int> & simplices);

unsigned long int string_to_simplex (int & num, const string & str);
unsigned long int string_to_simplex (const string & str);

string simplex_to_string (int number_of_vertices, unsigned long int s);

int num_vert (int number_of_vertices, unsigned long int s);

bool read_triang (ifstream & file, int number_of_vertices,
                  int & number_of_vertices_in_simplex,
                  int & number_of_orbits, set <unsigned long int> & orbit_rep);

bool read_triang (ifstream & file, int number_of_vertices,
                  set <unsigned long int> & orbit_rep);


#endif /* myiofunctions_hpp */
