//
//  triple_flip_graph_random.cpp
//  
//
//  (C) Alexander A. Gaifullin, 2023
//


using namespace std;

#include <iostream>
#include "../common/permutation.hpp"
#include "../common/myiofunctions.hpp"
#include "../common/triang.hpp"
#include <cstdlib>
#include <time.h>

#define ifile_name_group "symmetry_group.dat"
#define ifile_name_triang "triang.dat"
#define ofile_name "result.dat"


set <Permutation> group_elements;
//set <unsigned long int> orbit_rep;
//set <unsigned long int> all_max_simp;

vector<set <unsigned long long int> > certificates;
/* Found certificates of triangulations by the order of symmetry group
 * certificates[0] corresponds to |Sym(K)|=1
 * certificates[1] corresponds to |Sym(K)|=2
 * certificates[2] corresponds to |Sym(K)|=3
 * certificates[3] corresponds to |Sym(K)|=5
 * certificates[4] corresponds to |Sym(K)|=6
 * certificates[5] corresponds to |Sym(K)|=7
 * certificates[6] corresponds to |Sym(K)|=12
 * certificates[7] corresponds to |Sym(K)|=60
 */


unsigned long long int number_of_steps = 0;




int main() {
    certificates.resize(8);
    srand(time(0));

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
    ofstream ofile_seq ("sequence.dat", ios::out);
    ofstream ofile1 ("cert1.dat", ios::out);
    ofstream ofile2 ("cert2.dat", ios::out);
    ofstream ofile3 ("cert3.dat", ios::out);
    ofstream ofile5 ("cert5.dat", ios::out);
    ofstream ofile6 ("cert6.dat", ios::out);
    ofstream ofile7 ("cert7.dat", ios::out);
    ofstream ofile12 ("cert12.dat", ios::out);
    ofstream ofile60 ("cert60.dat", ios::out);



    // Adding initial triangulation

    Triang current_triang (all_max_simp);
    set <Triple> initial_triples = current_triang.compute_triples(group_elements);
    if (initial_triples.size() == 0) {
        cout << "No admissible distinguished triples" << endl;
        return 0;
    }


    for (;;number_of_steps++) {

        switch (current_triang.symm_group_order()) {
            case 1:
                if (certificates[0].count(current_triang.cert) == 0) {
                    certificates[0].insert(current_triang.cert);
                    ofile1 << current_triang.cert << endl;
                }
                break;
            case 2:
                if (certificates[1].count(current_triang.cert) == 0) {
                    certificates[1].insert(current_triang.cert);
                    ofile2 << current_triang.cert << endl;
                }
                break;
            case 3:
                if (certificates[2].count(current_triang.cert) == 0) {
                    certificates[2].insert(current_triang.cert);
                    ofile3 << current_triang.cert << endl;
                }
                break;
            case 5:
                if (certificates[3].count(current_triang.cert) == 0) {
                    certificates[3].insert(current_triang.cert);
                    ofile5 << current_triang.cert << endl;
                }
                break;
            case 6:
                if (certificates[4].count(current_triang.cert) == 0) {
                    certificates[4].insert(current_triang.cert);
                    ofile6 << current_triang.cert << endl;
                }
                break;
            case 7:
                if (certificates[5].count(current_triang.cert) == 0) {
                    certificates[5].insert(current_triang.cert);
                    ofile7 << current_triang.cert << endl;
                }
                break;
            case 12:
                if (certificates[6].count(current_triang.cert) == 0) {
                    certificates[6].insert(current_triang.cert);
                    ofile12 << current_triang.cert << endl;
                }
                break;
            case 60:
                if (certificates[7].count(current_triang.cert) == 0) {
                    certificates[7].insert(current_triang.cert);
                    ofile60 << current_triang.cert << endl;
                }
                break;
            default: // This case should never occur
                cout << "??? Symmetry group of order " << current_triang.symm_group_order() << endl;
                return 0;
        }

        set <Triple> triples = current_triang.compute_triples(group_elements);
        int k = rand() % triples.size();
        int i = 0;
        for (auto t : triples) {
            if (i == k) {
                current_triang.apply_triple_flip (group_elements, t);
                ofile_seq << simplex_to_string (15, t.delta[0]) << endl;
                break;
            } else {
                i++;
            }
        }
    }


    ofile_seq.close();
    ofile1.close();
    ofile2.close();
    ofile3.close();
    ofile5.close();
    ofile6.close();
    ofile7.close();
    ofile12.close();
    ofile60.close();
    
    return 0;
}
 

