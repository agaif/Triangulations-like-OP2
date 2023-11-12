//
//  triple_flip_graph.cpp
//  
//
//  (C) Alexander A. Gaifullin, 2023
//


using namespace std;

#include <iostream>
#include "../common/permutation.hpp"
#include "../common/myiofunctions.hpp"
#include "../common/triang.hpp"

#define ifile_name_group "symmetry_group.dat"
#define ifile_name_triang "triang.dat"
#define ofile_name_vert "vertices.dat"
#define ofile_name_details "vertices_details.dat"
#define ofile_name_edges "edges.dat"

// Output file streams
ofstream ofile_vert;
ofstream ofile_details;
ofstream ofile_edges;

set <Permutation> group_elements;
/* The set of elements of the group G
 * for which the G-equivariant triple flip graph is studied
 */

vector <Triang> triangulations;
/* Vector of all generated triangulations */
int number_of_triangulations = 0;
/* Number of generated triangulations */

map <unsigned long int, set <int> > triang_by_cert;
/* Triangulations organized by their certificates:
 * triang_by_cert[cert] is the set of all numbers of
 * triangulations (in vector 'triangulations') \
 * with the given certificate cert.
 */


/* The function that adds a new triangulation to the vector 'triangulations',
 * changing appropriately 'triang_by_cert'
 * and writes information to the output files.
 */
void add_triang(const Triang & a) {
    triangulations.push_back(a);
    ++ number_of_triangulations;
    if (triang_by_cert.count(a.cert) == 0) {
        triang_by_cert[a.cert] = {number_of_triangulations-1};
    } else {
        triang_by_cert[a.cert].insert(number_of_triangulations-1);
    }

    ofile_vert << "#" << number_of_triangulations << endl;
    ofile_vert << "*" << a.symm_group_order() << endl;
    ofile_vert << a.cert << endl << endl;

    ofile_details << "#" << number_of_triangulations << endl;
    ofile_details << "*" << a.symm_group_order() << endl;
    ofile_details << a.cert << endl << "=" << endl;

    vector <unsigned int> distrib = a.distrib();
    for (int i = 0; i < 6; i++) {
        ofile_details << distrib[i] << endl;
    }
    ofile_details << "=" << endl;
    print_simplices(ofile_details, 15, a.simp);
    ofile_details << endl;
}


/* The function checking whether a traingulation is isomorphic to
 * one of the already generated ones.
 * If 'yes', then the function returns the number of those old triangulation
 * to which a new one is isomorphic. Moreover, an isomorphism (any, if several)
 * is written to the variable 'isomorphism'.
 * If 'no', then the function returns -1.
 */
int num_triang (const Triang & a, Permutation & isomorphism) {
    if (triang_by_cert.count(a.cert) == 0) return -1;
    for (auto i : triang_by_cert[a.cert]) {
        if (a.isomorph(triangulations[i], group_elements, isomorphism)) return i;
    }
    return -1;
}

/* The function finding (and writing to file)
 * the multiset of all edges (in G-equivariant triple flip graph)
 * from the triangultion with number 'curr_num'
 * in the vector 'triangulations'.
 * If an edge leads to a triangulation that is not yet in the list,
 * then the function 'add_triang' is called to add this triangulation.
 * If an edge is a loop, then it is also checked whether this loop is self-inverse.
 */
void find_edges(int curr_num) {
    // Computing the set of representatives of orbits of all admissible triples
    set <Triple> triples = triangulations[curr_num].compute_triples(group_elements);

    ofile_edges << curr_num+1 << ": ";

    multiset <int> edges;
    /* The required multiset. It consists of all numbers n to which
     * an edge leads from the given triangulation.
     * Self-inverse loops are indicated by number -2.
     * Non-self-inverse loops are indicated by number -1.
     */

    // The cycle over all possible equivariant triple flips
    for (auto t : triples) {
        // Creating a copy of the set of all maximal simplices
        set <unsigned long int> new_simp = triangulations[curr_num].simp;

        // Performing an equivariant triple flip, i.e.,
        // the triple flips for all distinguished triples in the given G-orbit
        for (auto g : group_elements) {
            Triple t_curr = g * t;

            for (int i = 0; i < 15; i++) {
                int k = 0;
                while ((t_curr.delta[k]&(1UL<<i)) == 0) ++k;
                int l = (k+1)%3;
                int m = (k+2)%3;
                new_simp.erase(t_curr.delta[m]|(t_curr.delta[k]^(1UL<<i)));
                new_simp.insert(t_curr.delta[l]|(t_curr.delta[k]^(1UL<<i)));
            }
        }
        Triang new_triang(new_simp); // Creating new triangulation

        // Checking whether the new triangulation is already in the list
        Permutation isomorphism(15);
        int n = num_triang(new_triang, isomorphism);
        if (n != -1) {
            if (n != curr_num) {
                edges.insert(n);
            } else {
                // Checking whether the loop is self-inverse
                Triple t_inv = isomorphism * t.opp();
                bool self_inverse = false;
                for (auto g : triangulations[curr_num].equiv_symm_group(group_elements)) {
                    if (t_inv == g * t) {
                        self_inverse = true;
                        break;
                    }
                }
                if (self_inverse) {
                    edges.insert(-2);
                } else {
                    edges.insert(-1);
                }
            }
        } else {
            add_triang(new_triang);    // Adding a new triangulation
            edges.insert(number_of_triangulations-1);
        }
    }
    for (auto e : edges) {
        if (e == -2) {
            ofile_edges << curr_num+1 << "(si) ";
        } else if (e == -1) {
            ofile_edges  << curr_num+1 << " ";
        } else {
            ofile_edges << e + 1<< " ";
        }
    }
    ofile_edges << endl;
}



int main() {

    // Reading and generating the group G
    ifstream file_group (ifile_name_group, ios::in);
    int group_order;
    int number_of_vertices;
    if ( generate_group (file_group, number_of_vertices, group_order, group_elements) &&        number_of_vertices == 15) {
        cout << "Group of order " << group_order << " generated" << endl;
    } else {
        cout << "INCORRECT INITIAL DATA : Symmetry group" << endl;
        file_group.close();
        return 0;
    }
    file_group.close();
    
    // Reading the triangulation (list of all max simplices)
    ifstream file_triang (ifile_name_triang, ios::in);
    int number_of_vertices_in_simplex, number_of_simplices;
    set <unsigned long int> all_max_simp;
    if ( !read_triang(file_triang, number_of_vertices, number_of_vertices_in_simplex,
                      number_of_simplices, all_max_simp) || number_of_vertices_in_simplex != 9 || number_of_simplices != 490) {
        cout << "INCORRECT INITIAL DATA : Triangulation" << endl;
        file_triang.close();
        return 0;
    }
    cout << "Triangulation read\n";
    file_triang.close ();
    
    // Opening output files
    ofile_vert.open (ofile_name_vert, ios::out);
    ofile_details.open (ofile_name_details, ios::out);
    ofile_edges.open (ofile_name_edges, ios::out);
    
    // Adding initial triangulation
    add_triang(Triang(all_max_simp));

    // Finding edges
    for (int curr_num = 0; curr_num < triangulations.size(); ++ curr_num){
        find_edges(curr_num);
    }


    // Closing output files
    ofile_vert.close();
    ofile_details.close();
    ofile_edges.close();
    
    return 0;
}
 

