
#include "terrain.h"
#include "robot.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "curiosity.c"



int main(int argc,char **argv){
    Environnement envt;
    Programme prog;
    erreur_terrain errt;
    erreur_programme errp;
    etat_inter etat;
    resultat_inter res;
    FILE *test=fopen(argv[1],"r");
    FILE *terrain;
    FILE *Programme;
    int nb_pas,i=0;
    char e;
    int x,y;
    char o;
    char ligne[DIM_MAX];
    fgets(ligne,DIM_MAX,test);
    terrain=fopen(ligne,"r");
    if(terrain==NULL){
        return ERREUR_FICHIER;
    }
    fgets(ligne,DIM_MAX,test);
    Programme=fopen(ligne,"r");
    if(terrain==NULL){
        return ERREUR_FICHIER_PROGRAMME;
    }
    fgets(ligne,DIM_MAX,test);
    nb_pas=atoi(ligne);
    e=fgetc(test);
    if(e=='N'|| e=='F'){
        x=fgetc(test);
        y=fgetc(test);
        o=fgetc(test);
    }   
    errt=initialise_environnement(&envt,terrain);
    gestion_erreur_terrain(errt);
    errp=lire_programme(&prog,Programme);
    gestion_erreur_programme(errp);
    init_etat(&etat);
      init_etat(&etat);
    do {
    res = exec_pas(&prog, &envt, &etat);
    /* Affichage du terrain et du robot */
    i++;
    afficher_envt(&envt);
    } while (res == OK_ROBOT && i<nb_pas);
    switch (res) {
        case OK_ROBOT:
            if(i+1==nb_pas){
                if(e=='N')
                {
                    if(envt.r.x==x && envt.r.y==y){
                        if(envt.r.o==o){
                        printf("OK fin du nombre de pas et rebot sur la position attendu avec la bonne orientation");
                        }
                        else printf("nombre de pas finit robot a la bonne case mais pas la bonne orientation orientation voulue: %c  orientation reel %c",o,envt.r.o);
                    }
                    else printf("nomnbre de pas finit mais le robot n'est pas sur la bonne case: case voulue : %d %d  case du robot: %d %d",x,y,envt.r.x,envt.r.y);
                }
        printf("nombre de pas finit le robot est sur le terrain mais le programme devait faire l'event %c",e);
        }
        else 
            printf("Robot sur une case sur le terrain mais le robot n'a pas fait le nb de pas demander");
        break;
    case SORTIE_ROBOT:
        if(e=='S'){
            printf("OK");
        }
        else printf("Robot sortie mais le robot aurait du faire l'event %c",e);
        break;
    case ARRET_ROBOT:
        if(e=='F'){
            if(envt.r.x==x && envt.r.y==y){
                if(envt.r.o==o){
                    printf("OK");
                }
                else printf("Programme finit robot a la bonne case mais pas la bonne orientation orientation voulue: %c  orientation reel %c",o,envt.r.o);
            }
            else printf("Programme finit mais le robot n'est pas sur la bonne case: case voulue : %d %d  case du robot: %d %d",x,y,envt.r.x,envt.r.y);
        }
        printf("Programme finit le robot est sur le terrain mais le programme devait faire l'event %c",e);
        break;
    case PLOUF_ROBOT:
        if(e=='P'){
            printf("OK");
        }
        else printf("Robot tomber a l'eau mais le robot aurait du faire l'event %c",e);
        break;
    case CRASH_ROBOT:
        if(e=='O'){
            printf("OK");
        }
        else printf("Le robot s'est pris un rocher alors qu'il devait faire l'event %c",e);
        break;
    
    case ERREUR_PILE_VIDE:
        printf("ERREUR : pile vide\n");
        break;
    case ERREUR_ADRESSAGE:
        printf("ERREUR : erreur d'adressage\n");
        break;
    case ERREUR_DIVISION_PAR_ZERO:
        printf("ERREUR : division par 0\n");
        break;
  }
}