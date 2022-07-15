//
//  permute.cpp
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
#define ifile_name_permut "permutation.dat"
#define ofile_name "triang_new.dat"


int group_order;
int number_of_vertices;
int number_of_vertices_in_simplex;
int number_of_orbits;
vector <Permutation> group_elements;
set <unsigned long int> orbit_rep;
set <unsigned long int> orbit_rep_new;

// function that returns the smallest subset in the orbit of s
unsigned long int smallest_subset (unsigned long int s) {
    unsigned long int t = s;
    for (auto g : group_elements) {
        unsigned long int r = g * s;
        if (t > r) t = r;
    }
    return t;
}

//////////////////////

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
    
    // Generating the set of group elements
    set <Permutation> set_group_elements;
    for (auto g : group_elements) set_group_elements.insert(g);
    
    // Reading a permutation h
    ifstream ifile_permut (ifile_name_permut, ios::in);
    string str;
    getline(ifile_permut,str);
    ifile_permut.close();
    Permutation h (number_of_vertices, str);
    if ( ! h.is_well_defined() ) {
        cout << "INCORRECT INITIAL DATA" << endl;
        return 0;
    }
    Permutation h_inv = h.inv();  // the inverse permutation
    
    
    // Checking if h normalizes the symmetry group
    for (auto g : group_elements) {
        if (set_group_elements.count(h * g * h_inv) == 0) {
            cout << "The permutation does not belong to the normalizer of the group\n";
            return 0;
        }
    }
    
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
        orbit_rep_new.insert (smallest_subset(h * s));
    }
    
    // Writing the result
    ofstream ofile (ofile_name, ios::out);
    print_simplices (ofile, number_of_vertices, orbit_rep_new);
    ofile.close();
    cout << orbit_rep_new.size() << " orbits written\n";

    return 0;
}
 

