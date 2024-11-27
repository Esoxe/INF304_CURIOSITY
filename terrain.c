
#include "terrain.h"
#include "robot.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


erreur_terrain lire_terrain(FILE *f, Terrain *t, int *x, int *y) {
  int l=0, h=0,n=0;   // Dimensions du terrain
  int i=0;
  if (f == NULL) {
    return ERREUR_FICHIER;
  }

  if(t==NULL){
    return ERREUR_NULL;
  }
  n=fscanf(f,"%d",&l);
  if(n==0){
    return ERREUR_LECTURE_LARGEUR;
    }
  else if((l<0) || (l>DIM_MAX)){
    return ERREUR_LARGEUR_INCORRECTE;
  }
  t->largeur = l;
  n=fscanf(f,"%d",&h);
  if(n==0){
    return ERREUR_LECTURE_HAUTEUR;
  }
  else if((h<0) || (h>DIM_MAX)){
    return ERREUR_HAUTEUR_INCORRECTE;
  }
  t->hauteur=h;
  
  char ligne[DIM_MAX];
  fgets(ligne,DIM_MAX,f);
  for(i = 0;i<h;i++){
    char *c;
    c=fgets(ligne,DIM_MAX,f);
    if(c==NULL){
      return ERREUR_LIGNES_MANQUANTES;
    }
    int tailleLigne = strlen(ligne);
    if(tailleLigne < l ) {
      return ERREUR_LIGNE_TROP_COURTE;
    }
    if(tailleLigne >l+1){
      return ERREUR_LIGNE_TROP_LONGUE;
    }
    for(int j =0;j<l;j++){
      switch (ligne[j])
      {
      case '.':
      t->tab[j][i] = LIBRE;
      break;
      case '~':
      t->tab[j][i] = EAU;
      break;
      case '#':
      t->tab[j][i] = ROCHER;
      break;
      case 'C':
      t->tab[j][i] = LIBRE;
      *x=j;
      *y=i;
      break;
      default:
        return ERREUR_CARACTERE_INCORRECT;
      }
    }
  }
  if (fgets(ligne,DIM_MAX,f)!=NULL)
  {
    return ERREUR_LIGNE_ENPLUS;
  }
  

  return OK;
}

void ecrire_terrain(FILE *f,Terrain *t,int x,int y){
  int l=t->largeur;
  int h=t->hauteur;
  for(int i=0;i<h;i++){
    for(int j =0;j<l;j++){
      if(!(i==x && j ==y)){
        switch (t->tab[j][i])
        {
        case EAU:
          fputc('~',f);
          break;
        case ROCHER:
          fputc('#',f);
          break;
        case LIBRE:
          fputc('.',f);
          break;
        default:
          break;
        }
      }
      else 
        fputc('C',f);
    }
    fputs("\n",f);
  }
}

int largeur(Terrain *t){
  return t->largeur;
}
int hauteur(Terrain *t) {
  return t->hauteur;
}
int est_case_libre(Terrain *t,int x,int y){
  if(x <t->largeur && y <=t->hauteur){
    if(t->tab[x][y]== LIBRE){
      return 1;
    }
  }
  return 0;
}
void afficher_terrain(Terrain *t){
  for(int i=0;i<hauteur(t);i++){
    printf("\n");
    for(int j=0;j<t->largeur;j++){
          switch (t->tab[j][i])
      {
      case LIBRE:
      printf(".");
      break;
      case EAU:
      printf("~");
      break;
      case ROCHER:
      printf("#");
      break;
      default:
        break;
      }
    }

  }
  printf("\n");
}


