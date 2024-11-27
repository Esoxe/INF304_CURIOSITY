#include "generation_terrains.h"
#include "interprete.h"
#include <stdio.h>
#include <stdlib.h>

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
  float total_pas=0;
  float nb_s=0,nb_b=0,nb_obs=0,nb_pas=0;
  erreur_programme errp;
  Environnement envt;
  etat_inter etat;
  Programme prog;
  resultat_inter res;
  erreur_terrain errt;
  int N = strtol(argv[2], NULL, 10);
  int L = strtol(argv[3], NULL, 10);
  int H = strtol(argv[4], NULL, 10);
  float d = strtof(argv[5], NULL);
  int graine = strtol(argv[6], NULL, 10);
  int nb_step_max = strtol(argv[7], NULL, 10);
  float dens_th=0;
  FILE *res_f=fopen(argv[8],"w");
  Terrain T;
  FILE *terrain_genere;
  srand(graine);
  res=OK_ROBOT;
  if (argc < 9) {
    printf(
      "Usage: %s <fichier_programme> <N> <L> <H> <d> <graine> <nb_step_max> <fichier_res> \n",
      argv[0]);
    return 1;
  }
  for(int j=0;j<N;j++){
    terrain_genere=fopen("terrain_genere.txt","w+");
    do
    {
      generation_aleatoire(&T,L,H,d,&dens_th);
    } while (!existe_chemin_vers_sortie(&T));
    fprintf(terrain_genere,"%d\n%d\n",H,L);
    ecrire_terrain(terrain_genere,&T,H/2,L/2);
    fclose(terrain_genere);
    errt=initialise_environnement(&envt,"terrain_genere.txt");
    gestion_erreur_terrain(errt);
    errp=lire_programme(&prog,argv[1]);
    gestion_erreur_programme(errp);
    init_etat(&etat);
    res=OK_ROBOT;
    for(int i=0;i<nb_step_max && res==OK_ROBOT;i++){
      res=exec_pas(&prog,&envt,&etat);
      nb_pas++;
    }
    switch (res)
    {   
    case OK_ROBOT:
        fprintf(res_f,"%d\n",-1);
        nb_b++;
      break;
    case PLOUF_ROBOT:
      fprintf(res_f,"%d\n",-2);
      nb_obs++;
      break;
    case CRASH_ROBOT:
      fprintf(res_f,"%d\n",-3);
      nb_obs++;
      break;
    case SORTIE_ROBOT:
        fprintf(res_f,"%f\n",nb_pas+1);
        total_pas+=nb_pas+1;
        nb_s++;
    default:
      break;
    }
    nb_pas=0;
    }

  //Ecriture resultat
  printf("Nombre de terrain ou le robot est sorti : %.0f \n",nb_s);
  printf("Pourentage de terrain ou le robot est sorti : %.2f %%\n",nb_s/N*100);
  printf("Nombre de terrain ou le robot est resté bloqué : %.2f \n",nb_b);
  printf("Pourentage de terrain ou le robot est resté bloqué : %.2f %%\n",nb_b/N*100);
  printf("Nombre de terrain ou le robot est rentré dans un obstacle : %.2f \n",nb_obs);
  printf("Pourentage de terrain ou le robot est rentré dans un obstacle : %.2f %%\n",nb_obs/N*100);
  if(nb_s!=0)
  printf("Nombre de pas moyen effectués pour les sorties : %.2f \n",total_pas/nb_s);
}