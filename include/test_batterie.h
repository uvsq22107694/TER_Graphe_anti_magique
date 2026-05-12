#ifndef TEST_BATTERIE_H
#define TEST_BATTERIE_H

/*
 * Teste la propriété antimagique sur un graphe biparti K_{m,n}.
 */
void tester_biparti(int m, int n);

/*
 * Lance une série de tests sur différents graphes bipartis non triviaux
 * selon les contraintes de N. Alon (m <= n et n >= 4).
 */
void lancer_batterie_biparti();

/*
 * Teste la propriété antimagique sur un graphe k-parti.
 */
void tester_kparti(int* tailles, int k);


/*
 * Lance une série de tests sur différents hypergraphes k-partis.
 */
void lancer_batterie_hyper_kparti();

/*
 * Lance une série de tests sur différents graphes k-partis.
 */
void lancer_batterie_kparti();

/*
 * Lance une série de tests empiriques sur des hypergraphes (calculs et swaps).
 */
void lancer_batterie_hypergraphe();

#endif // TEST_BATTERIE_H
