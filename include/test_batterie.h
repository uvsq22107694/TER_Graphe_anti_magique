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

#endif // TEST_BATTERIE_H
