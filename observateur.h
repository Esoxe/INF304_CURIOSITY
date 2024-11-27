#ifndef _OBSERVATEUR_H_
#define _OBSERVATEUR_H_

typedef enum {A,G,D,M} Alphabet;

typedef enum {Init,Mes,Erreur,Move} Etat;

Etat etat_initial();

Etat fct_transition(Etat e , Alphabet c);

Etat fct2_transition(Etat e , Alphabet C);

int est_final(Etat e);

#endif