//
//  num_neighbors.cpp
//  
//
//  (C) Alexander A. Gaifullin, 2022
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>

using namespace std;


#include "../common/permutation.hpp"
#include "../common/myiofunctions.hpp"

#define ifile_name_group "symmetry_group.dat"
#define ifile_name_triang "triang.dat"
#define ifile_name_edge "edge.dat"
#define ofile_name "result.dat"

int group_order;
int number_of_vertices;
int number_of_vertices_in_simplex;
int number_of_orbits;
vector <Permutation> group_elements;
set <unsigned long int> orbit_rep;
set <unsigned long int> all_max_simp;
int max_number = 0;

// function that generates the set of all maximal simplices of the triangulation
void generate_all_max_simp () {
    for (auto s : orbit_rep) {
        for (auto g : group_elements) {
            all_max_simp.insert(g * s);
        }
    }
}

// function that computes the number of maximal simplices containing the given simplex t
int num_containing_codim_two (unsigned long int t) {
    int number = 0;
    for (int i = 0; i < number_of_vertices; i++) {
        if ( ( t & (1UL<<i) ) != 0 ) continue;
        for (int j = i + 1; j < number_of_vertices; j++) {
            if ( ( t & (1UL<<j) ) != 0 ) continue;
            if (  all_max_simp.count(t | (1UL<<i) | (1UL<<j)) != 0 ) number++;
        }
    }
    if (number > max_number) max_number = number; /* max_number stores the max value
                                                   * of number ever occurred */
    return number;
}



int main() {

    // Reading and generating the symmetry group
    ifstream file_group (ifile_name_group, ios::in);
    if ( generate_group (file_group, number_of_vertices, group_order, group_elements) ) {
        cout << "Group of order " << group_order << " generated" << endl;
    } else {
        cout << "INCORRECT INITIAL DATA" << endl;
        file_group.close();
        return 0;
    }
    file_group.close();
    
    // Reading the triangulation
    ifstream file_triang (ifile_name_triang, ios::in);
    if ( !read_triang(file_triang, number_of_vertices, number_of_vertices_in_simplex,
                      number_of_orbits, orbit_rep) ) {
        cout << "INCORRECT INITIAL DATA\n";
        file_triang.close();
        return 0;
    }
    cout << orbit_rep.size() << " orbits of simplices read\n";
    file_triang.close();
    
    // Generating all maximal simplices
    generate_all_max_simp ();
    
    set <unsigned long int> codim_two_faces; // codimension 2 faces already considered
    
    int number_occurrences[number_of_vertices];
       /* number_occurrences[s] is the number of found
        * codim 2 simplices rho with given s(rho) */
    for (int i = 0; i < number_of_vertices; i++) {
        number_occurrences[i] = 0;
    }
    
    for (auto s : orbit_rep) {      // cycle over all representatives of max simplices
        for (int i = 0; i < number_of_vertices; i++) {
            if ( ( s & (1UL<<i) ) == 0 ) continue;
            for (int j = i + 1; j < number_of_vertices; j++) {
                if ( ( s & (1UL<<j) ) == 0 ) continue;
                
                unsigned long int q = s^(1UL<<i)^(1UL<<j);   // codim 2 face
                if (codim_two_faces.count(q) != 0) continue; // skip if already considered
                
                int stab_order = 0;   // order of stabilizer of q
                for (auto g : group_elements) {
                    unsigned long int r = g * q;
                    codim_two_faces.insert(r);
                    if (r == q) stab_order++;
                }
                number_occurrences [ num_containing_codim_two (q)] += group_order / stab_order;
                // adding the size of the orbit of q
            }
        }
    }
    
    
    // Writing the result
    ofstream ofile (ofile_name, ios::out);
    for (int i = 0; i <= max_number ; i++) {
        if (number_occurrences[i] != 0) {
            ofile << i << " " << number_occurrences [i] << endl;
        }
    }

    return 0;
}
 

