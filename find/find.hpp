//
//  find.hpp
//  
//
//  (C) Alexander A. Gaifullin, 2022
//

#ifndef find_hpp
#define find_hpp

#define int_level signed char
/* Data type used for levels and status of orbits and pairs.
 * Must be signed.
 * Change it to `signed short' if you expect level > 127
 * This will increase considerably the amount of required memory */

#define int_orbit unsigned short int
/* Data type used for numbers of orbits.
 * Change it to `unsigned int' if you expect more than 65536 orbits.
 * This will increase the amount of required memory */

/* Uncomment exactly one of the following three lines to set the type of logs:
 * 'f' - full logs
 * 's' - standard logs
 * 'n' - no logs
 */
//#define type_log 'f'
#define type_log 's'
//#define type_log 'n'

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>

using namespace std;


// Functions for the action on subsets

set <unsigned long int> minimize_simplices (set <unsigned long int> & simplices);
bool ifsmallest_subset (unsigned long int );
unsigned long int next_subset (unsigned long int);

// Functions for initial generating the orbits of maximal simplices

void try_to_add_orbit (unsigned long int s,
                       map <unsigned long int, pair <set <int_orbit>,
                       set <int_orbit> > > & all_face_orbits);
void generate_orbits (map <unsigned long int, pair <set <int_orbit>,
                      set <int_orbit> > > & all_face_orbits);
void initialize_relative_status ();
void add_adjacency_group (set <int_orbit> & orbits_once_adjacent,
                          set <int_orbit> & orbits_twice_adjacent);
void generate_adjacency_groups (map <unsigned long int, pair <set <int_orbit>,
                     set <int_orbit> > > & all_face_orbits);
void generate_orbits_and_adjacency_groups ();


// Functions for taking/removing/requiring/forbidding orbits of simplices

bool take_one_orbit (int_orbit number);
bool remove_one_orbit (int_orbit number);
void require_one_pair (int_orbit m, int_orbit n);
void prohibit_one_pair (int_orbit m, int_orbit n);
bool take (int_orbit number);
bool remove (int_orbit number);
bool require (int_orbit m, int_orbit n);
bool prohibit (int_orbit m, int_orbit n);


// Functions for updating groups of simplices corresponding to codimension 1 faces

void put_to_waiting_list (int agnum);
bool examine_with_taken_two (int agnum);
bool examine_with_taken_one (int agnum);
bool examine_with_taken_zero_step1 (int agnum);
bool examine_with_taken_zero_step2 (int agnum);
bool examine_with_taken_zero_step3 (int agnum);
bool examine_with_taken_zero (int agnum);
bool examine (int agnum);


// The functions for eliminating the orbits

int_orbit choose_orbit ( );
bool increase_level();
bool decrease_one_level();
void decrease_level();
int number_of_prohibited_simplices (int_orbit n);
bool eliminate_by_possible_number_of_simplices ();
bool take_initial_orbits ();
void selection ();

// Output functions

void triangulation_found ();


#endif /* find_hpp */
