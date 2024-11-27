#include "generation_terrains.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int nb_cases_occupees(Terrain *T) {
  int occupee = 0;
  int x, y;
  for (x = 0; x < largeur(T); x++) {
    for (y = 0; y < hauteur(T); y++) {
      if (T->tab[x][y] != LIBRE)
        occupee++;
    }
  }
  return occupee;
}

// Test de generation aléatoire de terrains
// Le programme génère n terrains de largeur et hauteur fixes
// avec largeur et hauteur impaires et inférieures a dimension_max de terrain.h
// avec densité d'obstacle dObst
// autre que la case centrale soit occupee
// l'appel du programme se fait avec 5 arguments :
// generation_terrains N largeur hauteur dObstacle fichier_res
// la sortie se fait dans le fichier resultat

int main(int argc, char **argv) {
  int N, i, l, h;
  float dObst;
  float total=0;
  float dens_th,dens_total=0.0;
  FILE *resFile;
  Terrain T;

  if (argc < 6) {
    printf(
        "Usage: %s <N> <largeur> <hauteur> <densite_obstacle> <fichier_res> \n",
        argv[0]);
    return 1;
  }

  N = strtol(argv[1], NULL, 10);
  l = strtol(argv[2], NULL, 10);
  h = strtol(argv[3], NULL, 10);
  dObst = strtof(argv[4], NULL);

  // test de l et h
  if (l > DIM_MAX || l % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (h > DIM_MAX || h % 2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((dObst > 1) || (dObst < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }

  // Ouverture du fichier résultat
  resFile = fopen(argv[5], "w");
  // Écriture du nombre de terrains
  fprintf(resFile, "Nombre de terrains : %d\n \n", N);

  // Initialisation de la fonction random
  srand(time(NULL));

  // Génération aléatoire des terrains
    for(i=0;i<N;i++){
      do
      {
        generation_aleatoire(&T,l,h,dObst,&dens_th);
        total++;
      } while (!existe_chemin_vers_sortie(&T));
      ecrire_terrain(resFile,&T,h/2,l/2);   // Écriture des terrains générés dans le fichier resFile
      dens_total+=dens_th;
      fprintf(resFile,"Densité d'obstacle obtenue : %.4f \n \n",dens_th);
      fprintf(resFile,"Densité d'obstacle attendue : %.4f \n \n",dObst);
      fprintf(resFile,"Erreur relative sur la densité d'obstacle : %.2f %%\n \n",(fabs(dens_th-dObst)/dObst)*100);
    }

  // Écriture/Affichage des statistiques
    fprintf(resFile,"Densité d'obstacle moyenne sur les terrains : %.4f  \n",dens_total/N);
    fprintf(resFile,"Pourcentage de terrain valide : %.4f %% \n",N/total*100.0);

  // fermeture des fichiers
  fclose(resFile);
  return 0;
}

