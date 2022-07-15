//
//  check.cpp
//  
//
//  (C) Alexander A. Gaifullin, 2022
//


#include "../common/permutation.hpp"
#include "../common/myiofunctions.hpp"
#include "simpmax.hpp"
#include <time.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>

#define ifile_name_group "symmetry_group.dat"
#define ifile_name_triang "triang.dat"
#define ofile_name "bad_simplices.dat"


using namespace std;

/* BEGIN of functions declarations */

void generate_all_max_simplices ();
void prev_subset (unsigned long int & q, const vector <int> & pos);
void check_faces (unsigned long int s);
void check ();

/* END of functions declarations */

/* BEGIN of global variables */

int number_of_vertices; /* The number of vertices in the complex */
int number_of_vertices_in_simplex;  /* The number of vertices in a maximal simplex */
vector <Permutation> group_elements; /* The set of all elements of the symmetry group */
int group_order;           /* The order of the symmetry group */
int number_of_orbits;      /* The number of orbits of maximal simplices in the triangulation */
set <unsigned long int> orbit_rep; /* Set of size number_of_orbits consisting
                                    * of the smallest representatives of orbits
                                    * of maximal simplices */
set <unsigned long int> all_max_simplices; /* the set of all maximal simplices of triangulation */
set <unsigned long int> simplices_nonevasive; /* the set of all simplices whose links are already
                                               * checked to be combinatorial spheres
                                               * adding a simplex to this set, we always
                                               * simultaneously add all simplices in the same orbit */
set <unsigned long int> orbits_evasive;       /* the smallest representatives of orbits of simplices
                                               * with links for which we currently fail
                                               * checking that they are combinatorial spheres */

int count_number_of_processed_q = 0; /* the counter for complexes for which we check whether
                                      * they are evasive or not; needed only for writing logs */

/* END of global variables */


// the function that generates the set of all maximal simplices of the triangulation
void generate_all_max_simplices () {
    for (auto s : orbit_rep) {
        for (auto g : group_elements) {
            all_max_simplices.insert(g * s);
        }
    }
}


// the function that turns the subset encoded in the unsigned long int
// to the previous subset with 1's allowed only on positions in pos
void prev_subset (unsigned long int & q, const vector <int> & pos) {
    int i = 0;
    while ( (q & (1UL<<pos[i])) == 0 ) {
        q = q | (1UL<<pos[i]);
        ++i;
    }
    q = q ^ (1UL<<pos[i]);
}


// the function that checks links of all subsimplices of the maximal simplex s
void check_faces (unsigned long int s) {
    map <unsigned long int, SimpMax> link_costar_complex;
    // for each face r of s, link_costar_complex[r] will be the simplicial complex
    // L_{r,s} = cost(s\setminus r, link (r,K))
 
    // creating vector of vertices of the maximal simplex s
    vector <int> s_pos;
    for (int i = 0; i < number_of_vertices; i++) {
        if ( (s & (1UL<<i)) != 0 ) s_pos.push_back(i);
    }
    
    // Starting generating all the complexes L_{r,s} simultaneously
    // Step 1: adding each simplex t\setminus s (where t a maximal simplex)
    //         to the complex L_{s\cap t,s}
    for (auto t : all_max_simplices) {
        unsigned long int r = s & t;
        if (r == 0 || r == s) continue;
        
        if (link_costar_complex.count(r) == 0) {
            link_costar_complex[r] = SimpMax(number_of_vertices, t&~s);
        } else {
            link_costar_complex[r].max_simplices.insert(t&~s);
        }
    }
    
    unsigned long int q = s;
    
    // enumerating the proper faces q of s from largest to smallest
    for ( prev_subset (q, s_pos); q != 0; prev_subset (q, s_pos) ) {
        // Creating the complex L_{q,s} if still not exists
        if (link_costar_complex.count(q) == 0) {
            link_costar_complex[q] = SimpMax(number_of_vertices);
        }
        
        // Step 1 of generating L_{q,s}: adding to it all the simplices that
        // belong to L_{q',s} with q' containing q
        for (auto i : s_pos) {
            if ( (q & (1UL<<i)) != 0 || (q | (1UL<<i)) == s ) continue;
            link_costar_complex[q].add ( link_costar_complex [ q | (1UL<<i) ] );
        }
        
        // Checking if the link of q has already been checked to be good,
        // i.e. to have link PL homeomorphic to the sphere
        if (simplices_nonevasive.count(q) != 0) continue;
        
        // Checking if the complex L_{q,s} is nonevasive
        if (link_costar_complex[q].nonevasive()) {
            unsigned long int qmin = q;
            
            // Adding all simplices in the orbit of q to simplices_nonevasive
            for (auto g : group_elements) {
                unsigned long int r = g * q;
                simplices_nonevasive.insert(r);
                if (qmin > r) qmin = r;
            }
            
            // Erasing qmin from bad simplices
            orbits_evasive.erase(qmin);
        } else {
            // Adding the smallest representative of the orbit of q to bad simplices
            unsigned long int qmin = q;
            for (auto g : group_elements) {
                unsigned long int r = g * q;
                if (qmin > r) qmin = r;
            }
            orbits_evasive.insert(qmin);
        }
        /*****   writing log   ****/
        cout << ++count_number_of_processed_q << " complexes checked; ";
        cout << orbits_evasive.size() << " bad simplices so far" << endl;
        /*****   end writing log   ****/
     }
}


// the function that checks links to be combinatorial spheres
void check () {
    int check_count = 0;
    for (auto s : orbit_rep) {
        check_faces (s);
        /*****   writing log   ****/
        cout << ++check_count << " maximal simplices processed" << endl;
        /*****   end writing log   ****/
    }
}


/******************************/

int main() {
    
    time_t time_beginning = time (NULL);
    
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
    
    generate_all_max_simplices ();  // generating all maximal simplices
    
    cout << "Checking non-evasiveness...\n";
    check();
    
    if (orbits_evasive.empty()) {
        cout << "This is a combinatorial manifold\n";
    } else {
        cout << "There are " << orbits_evasive.size() << " bad simplices\n";
        ofstream ofile (ofile_name, ios::out);
        print_simplices (ofile, number_of_vertices, orbits_evasive);
        ofile.close();
    }
    
    // computing the running time
    time_t time_end = time (NULL);
    double running_time = difftime (time_end, time_beginning);
    
    int int_time = (int) running_time;
    int hours = int_time / 3600;
    int minutes = (int_time % 3600) / 60;
    int seconds = int_time % 60;
    
    cout << "Running time: ";
    if (int_time >= 3600) {
        cout << hours << " h " << minutes << " min " << seconds << " sec" << endl;
    } else if (int_time >= 60) {
        cout << minutes << " min " << seconds << " sec" << endl;
    } else {
        cout << seconds << " sec" << endl;
    }
    
    return 0;
}



