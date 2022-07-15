//
//  allsimp.cpp
//  
//
//  (C) Alexander A. Gaifullin, 2022
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>


using namespace std;


#include "../common/permutation.hpp"
#include "../common/myiofunctions.hpp"

#define ifile_name_group "symmetry_group.dat"
#define ifile_name_triang "triang.dat"
#define ofile_name_all "triang_all.dat"

int main() {
    
    int group_order;
    int number_of_vertices;
    int number_of_vertices_in_simplex;
    int number_of_orbits;
    vector <Permutation> group_elements;
    set <unsigned long int> orbit_rep;
    set <unsigned long int> all_simp;

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
    
    // Reading the representatives of orbits
    ifstream file_triang (ifile_name_triang, ios::in);
    if ( !read_triang(file_triang, number_of_vertices, number_of_vertices_in_simplex,
                      number_of_orbits, orbit_rep) ) {
        cout << "INCORRECT INITIAL DATA\n";
        file_triang.close();
        return 0;
    }
    cout << orbit_rep.size() << " orbits of simplices read\n";
    file_triang.close();
    
    // Generating the set of all simplices
    for (auto s : orbit_rep) {
        for (auto g : group_elements) {
            all_simp.insert(g * s);
        }
    }
    
    // Writing the result
    ofstream ofile (ofile_name_all, ios::out);
    print_simplices (ofile, number_of_vertices, all_simp);
    ofile.close();
    cout << all_simp.size() << " simplices written\n";

    return 0;
}
 

