//
//  num_neighbors_pairs.cpp
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
    
    // Reading an edge
    ifstream file_edge (ifile_name_edge, ios::in);
    int v_beg, v_end;
    file_edge >> v_beg;
    file_edge >> v_end;
    --v_beg;      // exterior numbering of vertices starts from 1
    --v_end;      // while the intrinsic numbering stars from 0
    file_edge.close();
    
    // Generating all maximal simplices
    generate_all_max_simp ();
    
    // Producing the set of all codim 1 simplices tau that contain the given edge
    set <unsigned long int> codim_one_faces_containing_edge;
    for (auto s : all_max_simp) {
        if  ( ( s & (1UL<<v_beg) ) == 0 || ( s & (1UL<<v_end) ) == 0 ) continue;
        for (int i = 0; i < number_of_vertices; i++) {
            if ( i == v_beg || i == v_end ) continue;
            if ( ( s & (1UL<<i) ) != 0 ) {
                codim_one_faces_containing_edge.insert (s^(1UL<<i));
            }
        }
    }
    
    int number_pairs_occurrences[number_of_vertices][number_of_vertices];
    /* number_occurrences[s] is the number of found
     * codim 1 simplices tau with given s(tau-v_beg) and s(tau-v_end) */
    for (int i = 0; i < number_of_vertices; i++) {
        for (int j = 0; j < number_of_vertices; j++) {
            number_pairs_occurrences[i][j] = 0;
        }
    }
    
    // Computing the required matrix N_{pq}
    for (auto t : codim_one_faces_containing_edge) {
        ++ number_pairs_occurrences [ num_containing_codim_two (t^(1UL<<v_beg))]
        [ num_containing_codim_two (t^(1UL<<v_end))];
    }
    
    // Writing the result
    ofstream ofile (ofile_name, ios::out);
    ofile << max_number << endl;
    for (int i = 1; i <= max_number ; i++) {
        for (int j = 1; j <= max_number ; j++) {
            ofile << number_pairs_occurrences [i][j] << " ";
        }
        ofile << endl;
    }
    ofile.close();

    return 0;
}
 

