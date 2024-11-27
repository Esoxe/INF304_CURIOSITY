#include "observateur.h"
#include <stdio.h>
#include <stdlib.h>

Etat etat_initial(){
    return Init;
}

Etat fct_transition(Etat e , Alphabet c){
    switch (e)
    {
    case Init:
        switch (c)
        {
        case A:
            return Erreur;
            break;
        case G:
        case D:
            return Init;
            break;
        case M:
            return Mes;
            break;
        default:
            return Init;
            break;
        }
        
    break;
    case Mes:
        switch (c)
        {
        case A:
        case G:
        case D:
            return Init;
            break;
        case M:
            return Mes;
            break;
        default:
            return Init;
            break;
        }
    break;
    case Erreur:
        return Erreur;
        break;
    
    default:
        return Init;
        break;
    }
}

// cette fonction verifie que après chaque mouvement il y a une mesure
// 
Etat fct2_transition(Etat e , Alphabet c) {
    switch (e)
    {
    case Init:
        switch (c)
        {
        case A:
        case G:
        case D:
            return Move;
            break;
        case M:
            return Init;
        // default:
        //     return Init;
        //     break;
        }
        break;
    case Move:
        switch (c)
        {
        case A:
        case G:
        case D:
            return Erreur;
            break;
        case M:
            return Init;
            break;
        // default:
        //     return Init;
        //     break;
        }
        break;
    case Erreur:
        return Erreur;
    // default:
    //     return Init;
    //     break;
    }
}

int est_final(Etat e){
    printf("%d\n",e);
    return e != 2 ;
}