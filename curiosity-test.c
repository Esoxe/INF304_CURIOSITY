
#include "terrain.h"
#include "robot.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curiosity.c>



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
    int nb_pas;
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

}