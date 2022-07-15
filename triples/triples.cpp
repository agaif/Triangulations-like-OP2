//
//  triples.cpp
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
#define ofile_name_dist "distinguished.dat"
#define ofile_name_nu "nu_param.dat"

int group_order;
int number_of_vertices;
int number_of_vertices_in_simplex;
int number_of_orbits;
vector <Permutation> group_elements;
set <unsigned long int> orbit_rep;
set <unsigned long int> all_max_simp;
int max_number = 0;

vector <vector <unsigned long int> > triples; /* vector for storing distinguished triples,
                                               * each consisting of 3 simplices */
vector <int> triple_types;   // vector of the types of triples
set <unsigned long int> already_found; /* the set of all (minimal) representatives of orbits
                                        * of (d/2)-dimensional simplices that enter in
                                        * the already found distinguished triples */




// function that generates the set of all maximal simplices of the triangulation
void generate_all_max_simp () {
    for (auto s : orbit_rep) {
        for (auto g : group_elements) {
            all_max_simp.insert(g * s);
        }
    }
}

// function that adds the smallest representative of the orbit of a distinguished
// triple (p,q,r) if it has not been added yet
void add_triple (unsigned long int p, unsigned long int q, unsigned long int r) {
    
    // Computing minimal representatives of the orbits of p, q, and r,
    // and the group elements gp, gq, and gr which take p, q, and r
    // to these representatives, respectively
    Permutation gp (number_of_vertices), gq (number_of_vertices), gr (number_of_vertices);
    unsigned long int pmin = p, qmin = q, rmin = r;
    for (auto g : group_elements) {
        if (g * p < pmin) {
            pmin = g * p;
            gp = g;
        }
        if (g * q < qmin) {
            qmin = g * q;
            gq = g;
        }
        if (g * r < rmin) {
            rmin = g * r;
            gr = g;
        }
    }
    
    // Checking if the triple has already been added
    if (already_found.count(pmin) != 0 || already_found.count(qmin) != 0 ||
        already_found.count(rmin) != 0) return;
    
    // Marking pmin, qmin, rmin already considered
    already_found.insert(pmin);
    already_found.insert(qmin);
    already_found.insert(rmin);
    
    // Computing the type of the distinguished triple
    if (pmin == qmin) {
        triple_types.push_back(1);
    } else {
        triple_types.push_back(2);
    }
    
    
    // Finding and adding the smallest representative for the triple
    if (pmin <= qmin && pmin <= rmin) {
        vector <unsigned long int> new_triple { gp * p, gp * q, gp * r};
        triples.push_back(new_triple);
    } else if (qmin <= rmin) {
        vector <unsigned long int> new_triple { gq * q, gq * r, gq * p};
        triples.push_back(new_triple);
    } else {
        vector <unsigned long int> new_triple { gr * r, gr * p, gr * q};
        triples.push_back(new_triple);
    }
}


/* Function that computes and prints the nu-parameters for all simplices
 * with at least one nu-parameter greater than or equal to the half of the dimension
 * In case if_to_find_triples == true, the function also calls add_triples() to */
void compute_nu_parameters (ofstream & ofile_nu, bool if_to_find_triples) {
    
    for (auto s : orbit_rep) {
        
        vector < set <unsigned long int> > nu_vertices;
        nu_vertices.resize(number_of_vertices);
        /* For each i not in the simplex s,
         * nu_vertices[i] will be the set of all vertices j in s
         * such that (s\cup j)\setminus i is a simplex of K.
         * So nu_vertices[i].size() will be the nu-parameter nu(s,i). */
        
        // Computing the sets nu_vertices
        for (int i = 0; i < number_of_vertices; i++) {
            if ( (s & (1UL<<i)) == 0 ) continue;
            unsigned long int q = s ^ (1UL<<i);
            for (int j = 0; j < number_of_vertices; j++) {
                if ( (q & (1UL<<j)) == 0 && j != i && all_max_simp.count(q|(1UL<<j)) != 0 ) nu_vertices[j].insert(i);
            }
        }
        
        // Finding out whether at least one nu-parameter is large
        bool large_nu_param = false;
        for (int j = 0; j < number_of_vertices; j++) {
            if (nu_vertices[j].size() >= number_of_vertices_in_simplex/2) {
                large_nu_param = true;
                break;
            }
        }
        
        // Printing nu-parameters if at least one is large
        if (large_nu_param) {
            ofile_nu << simplex_to_string(number_of_vertices,s) << " ";
            for (int j = 0; j < number_of_vertices; j++) {
                if ( (s&(1UL<<j)) == 0 ) {
                    ofile_nu << nu_vertices[j].size() << " ";
                }
            }
            ofile_nu << endl;
        }
        
        // Trying to add distinguished triple (if necessary)
        if (large_nu_param && if_to_find_triples) {
            for (int j = 0; j < number_of_vertices; j++) {
                if (nu_vertices[j].size() == number_of_vertices_in_simplex/2) {
                    unsigned long int p = 1UL<<j;
                    for (auto i : nu_vertices[j]) p |= 1UL<<i;
                    unsigned long int r = s & ~p;
                    unsigned long int q = ((1UL<<number_of_vertices) - 1) & ~p & ~r;
                    add_triple (p,q,r);
                }
            }
        }
    }
}


//////////////////////////////////////

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
    
    // Asking whether to find distinguished triples or not
    bool if_to_find_triples;  /* the flag showing whether the program needs to
                               * find the distinguished triples or
                               * to compute the nu-parameters only */
    while (true) {
        cout << "Find distinguished triples? (y/n): ";
        char c;
        cin >> c;
        if (c == 'y' || c == 'Y') {
            if_to_find_triples = true;
            break;
        }
        if (c == 'n' || c == 'N') {
            if_to_find_triples = false;
            break;
        }
    }
    
    // Generating all maximal simplices
    generate_all_max_simp ();
    
    // Computing and printing nu-parameters (and finding distinguished triples if necessary)
    ofstream ofile_nu (ofile_name_nu, ios::out);
    compute_nu_parameters (ofile_nu, if_to_find_triples);
    ofile_nu.close();
    
    // Printing distinguished triples (if necessary)
    if (if_to_find_triples) {
        ofstream ofile_dist (ofile_name_dist, ios::out);
        
        for (int i = 0; i < triples.size(); i++) {
            ofile_dist << simplex_to_string(number_of_vertices, triples[i][0]) << endl;
            ofile_dist << simplex_to_string(number_of_vertices, triples[i][1]) << endl;
            ofile_dist << simplex_to_string(number_of_vertices, triples[i][2]) << endl;
            ofile_dist << "type " << triple_types[i] << endl << endl;
        }
        ofile_dist.close();
        
        if (triples.size() == 0) {
            cout << "No triples\n";
            return 0;
        } else {
            cout << triples.size() << " triples found\n";
        }
    }
    
    return 0;
}
 

