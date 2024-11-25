#include "environnement.h"
#include "interprete.h"
#include "programme.h"

#include <stdio.h>
#include <stdlib.h>

char manage_orient(Orientation o){
    switch (o)
    {
    case Nord:
        return 'N';
        break;
    case Sud:
        return 'S';
        break;
    case Est:
        return 'E';
        break;
    case Ouest:
        return 'O';
        break;
    default:
        break;
    }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for (i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf(
        "Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
    case ERREUR_NULL:
    printf(
        "ERREUR TERRAIN NULL\n");
    exit(1);
     case ERREUR_LIGNE_ENPLUS:
    printf(
        "Erreur trop de lignes dans le fichier\n");
    exit(1);
  }
}

void gestion_erreur_programme(erreur_programme e) {
  switch (e.type_err) {
  case OK_PROGRAMME:
    break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}


int main(int argc,char **argv){
    Environnement envt;
    Programme prog;
    erreur_terrain errt;
    erreur_programme errp;
    etat_inter etat;
    resultat_inter res;
    FILE *test=fopen(argv[1],"r");
    int nb_pas;
    char terrain_nom[DIM_MAX];
    char prog_nom [DIM_MAX];
    char e;
    int x=0,y=0,i=0;
    char o='S';
    char ligne[DIM_MAX];
    fscanf(test,"%s\n",terrain_nom);
    fscanf(test,"%s\n",prog_nom);
    fscanf(test,"%s\n",ligne);
    nb_pas=atoi(ligne);
    e=fgetc(test);
    fgetc(test);
    if(e=='N'|| e=='F'){
        fscanf(test, "%d %d \n", &x,&y);
        fscanf(test, "%c", &o);
    }
    errt=initialise_environnement(&envt,terrain_nom);
    gestion_erreur_terrain(errt);
    errp=lire_programme(&prog,prog_nom);
    gestion_erreur_programme(errp);
    init_etat(&etat);
    res=OK_ROBOT;
    for(i;i<nb_pas && res==OK_ROBOT;i++){
        res=exec_pas(&prog,&envt,&etat);
        afficher_envt(&envt);
    }
    switch (res) {
        case OK_ROBOT:
            if(i==nb_pas){
                if(e=='N')
                {
                    if(envt.r.x==x && envt.r.y==y){
                        if(manage_orient(envt.r.o)==o){
                            printf("OK\n");
                            return 0;
                        }
                        else {
                            printf("nombre de pas finit robot a la bonne case mais pas la bonne orientation orientation voulue: %c  orientation reel %c\n",o,manage_orient(envt.r.o));
                            return 1;
                            }
                    }
                    else {
                        printf("nomnbre de pas finit mais le robot n'est pas sur la bonne case: case voulue : %d %d  case du robot: %d %d\n",x,y,envt.r.x,envt.r.y);
                        return 1;
                    }
                }
                printf("nombre de pas finit le robot est sur le terrain mais le programme devait faire l'event %c\n",e);
                return 1;
                }
            else {
                printf("Robot sur une case sur le terrain mais le robot n'a pas fait le nb de pas demander\n");
                return 1;
            }
                break;
        case SORTIE_ROBOT:
            if(e=='S'){
                printf("OK\n");
                return 0;
            }
            else {
                printf("Robot sortie mais le robot aurait du faire l'event %c\n",e);
                return 1;
            }
                break;
        case ARRET_ROBOT:
            if(e=='F'){
                if(envt.r.x==x && envt.r.y==y){
                    if(manage_orient(envt.r.o)==o){
                        printf("OK\n");
                        return 0;
                    }
                    else {
                    printf("Programme finit robot a la bonne case mais pas la bonne orientation orientation voulue: %c  orientation reel %c\n",o,manage_orient(envt.r.o));
                    return 1;
                    }
                }
                else {
                printf("Programme finit mais le robot n'est pas sur la bonne case: case voulue : %d %d  case du robot: %d %d\n",x,y,envt.r.x,envt.r.y);
                return 1;
                }
            }
            printf("Programme finit le robot est sur le terrain mais le programme devait faire l'event %c\n",e);
            return 1;
            break;
        case PLOUF_ROBOT:
            if(e=='P'){
                printf("OK\n");
                return 0;
            }
            else {
            printf("Robot tomber a l'eau mais le robot aurait du faire l'event %c\n",e);
            return 1;
            }
            break;
        case CRASH_ROBOT:
            if(e=='O'){
                printf("OK\n");
                return 0;
            }
            else {
            printf("Le robot s'est pris un rocher alors qu'il devait faire l'event %c\n",e);
            return 1;
            }
            break;
        
        case ERREUR_PILE_VIDE:
            printf("ERREUR : pile vide\n");
            return 1;
            break;
        case ERREUR_ADRESSAGE:
            printf("ERREUR : erreur d'adressage\n");
            return 1;
            break;
        case ERREUR_DIVISION_PAR_ZERO:
            printf("ERREUR : division par 0\n");
            return 1;
            break;
  }
}