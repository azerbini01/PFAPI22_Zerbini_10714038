/*Lo scopo di questo task è impratichirsi con l'interfaccia del
sistema di verifica.
L'input è costituito da due numeri interi, l' output atteso è
la loro somma.*/
#include <stdlib.h>
#include <stdio.h>

int main() {
    int x,y;
    int c;

    if(scanf("%d", &x)==0 ) return -1;
    if(scanf("%d", &y)==0 ) return -1;

    c = printf("%d", x+y);
    if(c<0) return -1;

    return 0;
}