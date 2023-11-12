//
//  triang.hpp
//  
//
//  (C) Alexander A. Gaifullin, 2023
//

#ifndef triang_hpp
#define triang_hpp

#include <stdio.h>
#include <vector>
#include <set>
#include <map>
#include <string>

using namespace std;

#include "permutation.hpp"
#include "myiofunctions.hpp"

/*
Class Triple for distinguished triples.
Applicable to only 15-vertex 8-manifolds.
*/
class Triple {
public:
    unsigned long int delta[3]; /* The three 4-simplices of the distinguished triple
                                 * permuted cyclically so that delta[0]
                                 * is the smallest one
                                 */
    Triple (unsigned long int p, unsigned long int q, unsigned long int r);
    bool operator < (const Triple & t) const;
    bool operator == (const Triple & t) const;
    Triple opp () const;     /* the same triple with the opposite cyclic order */
};

Triple operator* (const Permutation & g, const Triple & t); /* Action of permutation on a triple */


/*
Class Triang for working with 15-vertex triangulations of 8-manifolds
like a projective plane, their cetificates and symmetry groups.
Works correctly only for 15-vertex 8-manifolds like a projective plane!
*/
class Triang {
public:
    set <unsigned long int> simp; /* Set of all 8-simplices of the triangulation */
    unsigned long long int cert;  /* The certificate */

    Triang () { }
    Triang (set <unsigned long int> & simp);

    bool isomorph (const Triang & b, const set <Permutation> & group, Permutation & isomorphism) const;
    bool isomorph (const Triang & b, const set <Permutation> & group) const;
    /* Checks whether there exists a weak G-equivariant isomorphism between
     * two triangulations.
     * `group' should be the set of all elements of G.
     * If a required isomorphism exists, it is written to `isomorphism'.
     * If there are several isomorphisms, only one of them is found.
     * The function works correctly only if both triangulations are G-invariant.
     */

    set <Permutation> symm_group () const;
    /* Returns the set of all symmetries of the given triangulation */

    int symm_group_order() const;
    /* Returns the order of the symmetry group of the given triangulation */

    set <Permutation> equiv_symm_group (const set <Permutation> & group) const;
    /* Returns the set of all weak G-equivariant symmetries (automorphisms)
     * of the given triangulation.
     * `group' should be the set of all elements of G.
     * The function works correctly only if the triangulation is G-invariant.
     */

    vector <unsigned int> distrib() const;
    /* Returns the distribution m(K)=(m3,...,m8) encoded in the certificate */

    set <Triple> compute_triples (const set <Permutation> & group) const;
    /* Returns the set of representatives of G-orbits of distinguished triples of K.
     * `group' should be the set of all elements of G.
     * The function works correctly only if the triangulation is G-invariant.
     */

    void apply_triple_flip (const set <Permutation> & group, const Triple & t);
    /* The function applies the equivariant triple to the triangulation
     * The triple must be admissible!
     */

private:
    void compute_cert();
    /* Computes the certificate of K and writes it to `cert'
     */

    vector <vector <vector <int> > > neigh_matrix () const;
    /* Returns the 15x15x15 3-dim matrix N such that
     * N[i][j][k] is the number of 8-simplices that contain
     * the three vertices i, j, k
     */
};

#endif /* triang_hpp */
