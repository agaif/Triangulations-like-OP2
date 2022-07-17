//
//  fvect.cpp
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
#define ofile_name "fvect.dat"

int group_order;
int number_of_vertices;
int number_of_vertices_in_simplex;
int number_of_orbits;
vector <Permutation> group_elements;
set <unsigned long int> orbit_rep;


// changes a subsimplex t of s to the next subsimplex of s
// returns s+1 if t were equal to s
void change_to_next (unsigned long int & t, const unsigned long int & s) {
    if (t == s) {
        t++;
        return;
    }
    for (int i = 0; true; i++) {
        if ((s&(1UL<<i)) == 0) continue; // skip vertices not belonging to s
        if ((t&(1UL<<i)) != 0) {
            t ^= 1UL<<i;
        } else {
            t |= 1UL<<i;
            return;
        }
    }
}

// the function computing the dimension of simplex
int dimen (const unsigned long int t) {
    int res = -1;
    for (int i = 0; i < number_of_vertices; i++) {
        if ((t&(1UL<<i)) != 0) res++;
    }
    return res;
}

/********************************/

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
    
    // Initializing f-vector with 0's
    vector <int> fvect;
    fvect.resize(number_of_vertices_in_simplex);
    for (int i = 0; i < number_of_vertices_in_simplex; i++) {
        fvect[i] = 0;
    }
    
    // Computing f-vector
    set <unsigned long int> already_considered; // the set of the smallest representatives
                                                // of orbits that has already been counted
    int number_orbits_counted = 0;
    for (auto s : orbit_rep) {
        for (unsigned long int t = 0; t <= s; change_to_next (t, s)) {
            if (t == 0 || already_considered.count(t) != 0) continue;
            unsigned long int tmin = t;
            set <unsigned long int> torb;
            for (auto g : group_elements) {
                unsigned long int tcur = g*t;
                if (tcur < tmin) tmin = tcur;
                torb.insert(tcur);
            }
            for (auto q : torb) already_considered.insert(q);
        
            fvect[dimen(tmin)] += torb.size();
            number_orbits_counted++;
            
            /*** writing log ***/
            if (number_orbits_counted % 10000 == 0) {
                cout << number_orbits_counted << " orbits counted\n";
            }
            /*** end writing log ***/
        }
    }
    
    
    // Writing the result
    ofstream ofile (ofile_name, ios::out);
    for (int i = 0; i < number_of_vertices_in_simplex ; i++) {
        ofile << fvect[i] << endl;
    }
    ofile.close();
    
    
    return 0;
}
 

