//
//  isomorphism_groups.cpp
//  
//
//  (C) Alexander A. Gaifullin, 2023
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>

#include "../common/permutation.hpp"
#include "../common/myiofunctions.hpp"
#include "../common/triang.hpp"

using namespace std;

#define ifile_name_group "symmetry_group.dat"
#define ifile_name_triangulations "triangulations.dat"
#define ofile_name "result.dat"


set <Permutation> group_elements; // All elements of the symmetry group
int group_order;                     // The order of the symmetry group

vector < set <unsigned long int> >  triang_orbit_rep;
/* triang_orbit_rep[i] is the set of representatives of G-orbits
 * of maximal simplices of the ith triangulation
 */

vector < Triang > triang; /* The vector of all triangulations read */
int number_of_triangulations; /* Number of triangulations read */

vector < vector<int> > isomorphism_groups;
/* The vector encoding the groups of isomorphic triangulations:
 * For each i,
 * isomorphism_groups[i] is the vector consisting of all numbers
 * of triangulations that belong to the same (i th) isomorphism group.
 */


vector  <Permutation> add_permut;


// The function reading the representatives of G-orbits for all triangulations
void read_triangulations () {
    ifstream file (ifile_name_triangulations, ios::in);
    string str;
    
    while (!file.eof()) {
        getline(file, str);
        if (file.eof()) break;
        if (str[0] != '*') continue;
        
        set <unsigned long int> new_triang;
        read_triang (file, 15, new_triang);
        triang_orbit_rep.push_back (new_triang);
    }
    
    file.close();
    number_of_triangulations = triang_orbit_rep.size();
}


// The function computing the smallest subset in a G-orbit
unsigned long int smallest_subset (unsigned long int s) {
    unsigned long int t = s;
    for (auto g : group_elements) {
        unsigned long int r = g * s;
        if (t > r) t = r;
    }
    return t;
}


int main () {
    
    // Reading and generating the symmetry group
    ifstream file_group (ifile_name_group, ios::in);
    int number_of_vertices;
    if ( generate_group (file_group, number_of_vertices, group_order, group_elements) &&        number_of_vertices == 15) {
        cout << "Group of order " << group_order << " generated" << endl;
    } else {
        cout << "INCORRECT INITIAL DATA : Symmetry group" << endl;
        file_group.close();
        return 0;
    }
    file_group.close();

    // Reading the triangulations
    read_triangulations ();
    cout << number_of_triangulations << " triangulations read\n";

    // Generating objects of type Triang for all triangulations read
    for (auto t : triang_orbit_rep) {
        set <unsigned long int> all_simp;
        for (auto s : t) {
            for (auto g : group_elements) {
                all_simp.insert(g * s);
            }
        }
        triang.push_back(Triang(all_simp));
    }

    // Generating isomorphism groups
    for (int i = 0; i < number_of_triangulations; i++) {
        bool found = false;
        for (int j = 0; j < isomorphism_groups.size(); j++) {
            if (triang[i].isomorph(triang[isomorphism_groups[j][0]], group_elements)) {
                found = true;
                isomorphism_groups[j].push_back(i);
                break;
            }
        }
        if (!found) {
            vector <int> new_vect = {i};
            isomorphism_groups.push_back(new_vect);
        }
    }


    // Writing ismomorphism groups to file
    ofstream file (ofile_name, ios::out);
    for (int i = 0; i < isomorphism_groups.size(); i++) {
        vector <unsigned int> vec = triang[isomorphism_groups[i][0]].distrib();
        for (auto x : vec) {file << x << endl;}
        file << "=" << endl;
        for (int j = 0; j < isomorphism_groups[i].size(); j++) {
            file << isomorphism_groups[i][j]+1 << " ";

        }
        file << endl << endl;
    }
    file.close();
}



