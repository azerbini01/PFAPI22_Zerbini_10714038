#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 0
#define BLACK 1

#define debug 0

typedef struct d{
    int key;
    int color;
    struct d *p;
    struct d *left;
    struct d *right;
}node_int;

typedef struct{
    node_int* root;
}RB_int;

node_int *NIL_int;

void init_int(RB_int* T){
    T->root = NIL_int;
    T->root->p = NIL_int;
    T->root->right = NIL_int;
    T->root->left = NIL_int;
}

void left_rotate_int(RB_int* T, node_int *x){
    node_int *y;

    y = x->right;
    x->right = y->left;
    if(y->left != NIL_int)
        y->left->p = x;

    y->p = x->p;
    if(y->p == NIL_int)
        T->root = y;
    else if(x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->left = x;
    x->p = y;
}

void right_rotate_int(RB_int* T, node_int *x){
    node_int *y;

    y = x->left;
    x->left = y->right;
    if(y->right != NIL_int)
        y->right->p = x;

    y->p = x->p;
    if(y->p == NIL_int)
        T->root = y;
    else if(x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->right = x;
    x->p = y;
}

void inorderWalk_int(node_int *x){
    if(x != NIL_int){
        inorderWalk_int(x->left);
        printf("%c\n", x->key);
        inorderWalk_int(x->right);
    }
}

node_int *RBsearch_int(RB_int* T, int key){
    node_int *x = T->root;

    while(x != NIL_int && x->key != key ){
        if(key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}

void RBinsert_fixup_int(RB_int *T, node_int *z){
    while(z->p->color == RED){

        if(z->p == z->p->p->left){

            if(z->p->p->right->color == RED){
                z->p->color = BLACK;
                z->p->p->right->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }

            else{
                if(z == z->p->right){
                    z = z->p;
                    left_rotate_int(T, z);
                }

                z->p->color = BLACK;
                z->p->p->color = RED;
                right_rotate_int(T, z->p->p);
            }
        }

        else{
            if(z->p->p->left->color == RED){
                z->p->color = BLACK;
                z->p->p->left->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }

            else{
                if(z == z->p->left){
                    z = z->p;
                    right_rotate_int(T, z);
                }

                z->p->color = BLACK;
                z->p->p->color = RED;
                left_rotate_int(T, z->p->p);
            }
        }
    }

    T->root->color = BLACK;
}

void RBinsert_int(RB_int *T, char key){
    node_int *z, *x, *y;
    z = malloc(sizeof(node_int));

    z->key = key;
    z->color = RED;
    z->left = NIL_int;
    z->right = NIL_int;

    x = T->root;
    y = NIL_int;

    while(x != NIL_int){
        y = x;
        if(z->key == x->key)     return;
        else if(z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    if(y == NIL_int)
        T->root = z;
    else if(z->key <= y->key)
        y->left = z;
    else
        y->right = z;

    z->p = y;

    RBinsert_fixup_int(T, z);
}

void RBcopy_int( node_int *source, RB_int *Dest){
    if(source != NIL_int){

        RBinsert_int(Dest, source->key);
        RBcopy_int(source->left, Dest);
        RBcopy_int(source->right, Dest);
    }
}

typedef struct n{
    char key;
    int countCharR;
    int countMatchedS;
    RB_int *pos;
    int color;
    struct n *p;
    struct n *left;
    struct n *right;
}node_char;

typedef struct{
    node_char* root;
}RB_char;

node_char *NIL_char;

void init_char(RB_char* T){
    T->root = NIL_char;
    T->root->p = NIL_char;
    T->root->right = NIL_char;
    T->root->left = NIL_char;
}

void left_rotate_char(RB_char* T, node_char *x){
    node_char *y;

    y = x->right;
    x->right = y->left;
    if(y->left != NIL_char)
        y->left->p = x;

    y->p = x->p;
    if(y->p == NIL_char)
        T->root = y;
    else if(x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->left = x;
    x->p = y;
}

void right_rotate_char(RB_char* T, node_char *x){
    node_char *y;

    y = x->left;
    x->left = y->right;
    if(y->right != NIL_char)
        y->right->p = x;

    y->p = x->p;
    if(y->p == NIL_char)
        T->root = y;
    else if(x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->right = x;
    x->p = y;
}

void inorderWalk_char(node_char *x){
    if(x != NIL_char){
        inorderWalk_char(x->left);
        printf("%c\n", x->key);
        inorderWalk_char(x->right);
    }
}

node_char *RBsearch_char(RB_char* T, char key){
    node_char *x = T->root;

    while(x != NIL_char && x->key != key ){
        if(key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    return x;
}

void RBinsert_fixup_char(RB_char *T, node_char *z){
    while(z->p->color == RED){

        if(z->p == z->p->p->left){

            if(z->p->p->right->color == RED){
                z->p->color = BLACK;
                z->p->p->right->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }

            else{
                if(z == z->p->right){
                    z = z->p;
                    left_rotate_char(T, z);
                }

                z->p->color = BLACK;
                z->p->p->color = RED;
                right_rotate_char(T, z->p->p);
            }
        }

        else{
            if(z->p->p->left->color == RED){
                z->p->color = BLACK;
                z->p->p->left->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }

            else{
                if(z == z->p->left){
                    z = z->p;
                    right_rotate_char(T, z);
                }

                z->p->color = BLACK;
                z->p->p->color = RED;
                left_rotate_char(T, z->p->p);
            }
        }
    }

    T->root->color = BLACK;
}

void RBinsert_char(RB_char *T, char key, RB_int *pos, int countCharR, int countMatchedS){
    node_char *z, *x, *y;
    z = malloc(sizeof(node_char));

    z->key = key;
    RBcopy_int(pos->root, z->pos);
    z->countCharR = countCharR;
    z->countMatchedS = countMatchedS;
    z->color = RED;
    z->left = NIL_char;
    z->right = NIL_char;

    x = T->root;
    y = NIL_char;

    while(x != NIL_char){
        y = x;
        if(z->key == x->key)     return;
        else if(z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    if(y == NIL_char)
        T->root = z;
    else if(z->key <= y->key)
        y->left = z;
    else
        y->right = z;

    z->p = y;

    RBinsert_fixup_char(T, z);
}


typedef struct m{
    char key[30];
    int color;
    struct m *p;
    struct m *left;
    struct m *right;
}node;

typedef struct{
    node* root;
}RB;

node *NIL;

void init(RB* T){
    T->root = NIL;
    T->root->p = NIL;
    T->root->right = NIL;
    T->root->left = NIL;
}

void left_rotate(RB* T, node *x){
    node *y;

    y = x->right;
    x->right = y->left;
    if(y->left != NIL)
        y->left->p = x;

    y->p = x->p;
    if(y->p == NIL)
        T->root = y;
    else if(x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->left = x;
    x->p = y;
}

void right_rotate(RB* T, node *x){
    node *y;

    y = x->left;
    x->left = y->right;
    if(y->right != NIL)
        y->right->p = x;

    y->p = x->p;
    if(y->p == NIL)
        T->root = y;
    else if(x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->right = x;
    x->p = y;
}

void inorderWalk(node *x){
    if(x != NIL){
        inorderWalk(x->left);
        printf("%s\n", x->key);
        inorderWalk(x->right);
    }
}

node *RB_minimum(node *x){
    while(x->left != NIL){
        x = x->left;
    }
    return x;
}

node *RBsearch(RB* T, char* key){
    node *x = T->root;

    while(x != NIL && strcmp(x->key, key)!=0){  //x->key != key
        if(strcmp(x->key, key)>0)    //key < x->key
            x = x->left;
        else
            x = x->right;
    }
    return x;
}

void RBinsert_fixup(RB *T, node *z){
    while(z->p->color == RED){

        if(z->p == z->p->p->left){

            if(z->p->p->right->color == RED){
                z->p->color = BLACK;
                z->p->p->right->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }

            else{
                if(z == z->p->right){
                    z = z->p;
                    left_rotate(T, z);
                }

                z->p->color = BLACK;
                z->p->p->color = RED;
                right_rotate(T, z->p->p);
            }
        }

        else{
            if(z->p->p->left->color == RED){
                z->p->color = BLACK;
                z->p->p->left->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }

            else{
                if(z == z->p->left){
                    z = z->p;
                    right_rotate(T, z);
                }

                z->p->color = BLACK;
                z->p->p->color = RED;
                left_rotate(T, z->p->p);
            }
        }
    }

    T->root->color = BLACK;
}

void RBinsert(RB *T, char* key){
    node *z, *x, *y;
    z = malloc(sizeof(node));

    if(strcpy(z->key,key)==NULL)   //z->key = key
        exit(0) ;
    z->color = RED;
    z->left = NIL;
    z->right = NIL;

    x = T->root;
    y = NIL;

    while(x != NIL){
        y = x;
        if(strcmp(z->key,x->key)==0)     return;
        else if(strcmp(z->key,x->key)<0) //z->key < x->key
            x = x->left;
        else
            x = x->right;
    }

    if(y == NIL)
        T->root = z;
    else if(strcmp(z->key,y->key)<=0) //z->key <= y->key
        y->left = z;
    else
        y->right = z;

    z->p = y;

    RBinsert_fixup(T, z);
}

void RBdelete_fixup(RB *T, node *x){
    node *w;

    while(x != T->root && x->color == BLACK){

        if(x == x->p->left){
            w = x->p->right;

            if(w->color == RED){
                w->color = BLACK;
                x->p->color = RED;
                left_rotate(T, x->p);
                w = x->p->right;
            }

            if(w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x->p->color = BLACK;
                x = x->p;
            }
            else{

                if(w->right->color == BLACK){
                    w->color = RED;
                    w->left->color = BLACK;
                    right_rotate(T, w);
                    w = x->p->right;
                }

                w->color = x->p->color;
                x->p->color = BLACK;
                x->right->color = BLACK;
                left_rotate(T, x->p);
                x = T->root;

            }

        }
        else{
            w = x->p->left;

            if(w->color == RED){
                w->color = BLACK;
                x->p->color = BLACK;
                right_rotate(T, x->p);
                w = x->p->left;
            }

            if(w->left->color == BLACK && w->right->color == BLACK){
                w->color = RED;
                x->p->color = BLACK;
                x = x->p;
            }
            else{

                if(w->left->color == BLACK){
                    w->color = RED;
                    w->right->color = BLACK;
                    left_rotate(T, w);
                    w = x->p->left;
                }

                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                right_rotate(T, x->p);
                x = T->root;

            }
        }

    }

    x->color = BLACK;
}

void replace(RB* T, node *u, node *v){  /* replace node u with node v */
    if(u->p == NIL)
        T->root = v;

    else if(u == u->p->left)
        u->p->left = v;

    else
        u->p->right = v;

    v->p = u->p;
}

void RBdelete(RB *T, node *z){
    node *y, *x;
    int yOriginalColor;

    y = z;
    yOriginalColor = y->color;

    if(z->left == NIL){
        x = z->right;
        replace(T, z, z->right);
    }
    else if(z->right == NIL){
        x = z->left;
        replace(T, z, z->left);
    }
    else{
        y = RB_minimum(z->right);
        yOriginalColor = y->color;

        x = y->right;

        if(y->p == z){
            x->p = y;
        }
        else{
            replace(T, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }

        replace(T, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }

    if(yOriginalColor == BLACK){
        RBdelete_fixup(T, x);
    }
}

void mod_filtrate(node *x, RB* filtered, RB_char* symbols, int *num_filt, char *r, char *s, char *o, int k){
    if(x != NIL){
        mod_filtrate(x->left, filtered, symbols, num_filt, r, s, o, k);

        /*controllo filtrate
         Si noti che i vincoli appresi riguardano, per ogni simbolo:
1.
se il simbolo non appartiene a r
2.
posti in cui quel simbolo deve comparire in r
3.
posti in cui quel simbolo non può comparire in r
4.
numero minimo di volte che il simbolo compare in r
5.
numero esatto di volte che il simbolo compare in r
•
si noti che il vincolo 5 è più forte del vincolo 4
•
l'ordine dei simboli (usato per stabilire l'ordine lessicografico delle parole) è quello specificato dallo standard ASCII*/
        for(int i=0; i<k; i++){
            if(s[i] != x->key[i] && o[i] == '+'){
                RBdelete(filtered, x);
                (*num_filt) = (*num_filt) - 1;
            }
            else {
                !!!
                int countCharK = 0, countMatchedK = 0, countNotMatchedBeforeK = 0;
                for (int j = 0; j < k; j++) {
                    if (x->key[j] == s[i])
                        countCharK++;
                    if (x->key[j] == r[j] && x->key[j] == s[i])
                        countMatchedK++;

                    if (s[i] == x->key[i]) {
                        if (x->key[i] != r[j] && x->key[j] == x->key[i] && j < i)
                            countNotMatchedBeforeK++;
                    }
                }

                node_char *x = RBsearch_char(symbols, s[i]);
                if(x==NIL_char) {
                    printf("ERRORE");
                    return;
                }

                if (/*(s[i] == x->key[i] && (countNotMatchedBeforeK >= countCharK - countMatchedK) && o[i] == '|')  ||*/
               (countCharK != x->countCharR) || (countCharK < x->countMatchedS)){
                    RBdelete(filtered, x);
                    (*num_filt) = (*num_filt) - 1;
                }
            }
        }

        mod_filtrate(x->right, filtered, symbols, num_filt, r, s, o, k);
    }

}

void RBcopy( node *source, RB *Dest){
    //node *z;
    //z = malloc(sizeof(node));

    if(source != NIL){
        /*if(strcpy(z->key,source->key)==NULL)
            exit(0) ;
        z->color = source->color;
        z->left = NIL;
        z->left->p = z;
        z->right = NIL;
        z->right->p = z;
        z->p = dest->p;

        dest = z;
        */
        RBinsert(Dest, source->key);
        RBcopy(source->left, Dest);
        RBcopy(source->right, Dest);
    }
}

int play(RB *dic, int n, int k, int *num_words, int *found, char *r){
    int i, j, flag, num_filt = (*num_words);
    char s[30], *o, temp;

    RB_int *pos;
    RB_char * symbols = malloc(sizeof(RB_char));
    init_char(symbols);
    RB * filtered = malloc(sizeof(RB));
    init(filtered);

    RBcopy(dic->root, filtered);
    if (debug) {
        printf("\nFiltered initialized\n");
        inorderWalk(filtered->root);
        printf("\n");
    }

    while (n > 0) {
        if (fscanf(stdin, "%s", s) == 0) return -1;

        if (strcmp(s, "+stampa_filtrate") == 0) {
            if (debug) {
                printf("Stampa filtrate\n");
            }
            /*Quando, durante una partita, da input si legge il comando +stampa_filtrate, il programma deve produrre in output,
              in ordine lessicografico, l'insieme delle parole ammissibili che sono compatibili con i vincoli appresi fino a quel momento nella partita,
              scritte una per riga.
*/          inorderWalk(filtered->root);

        }
        else if (strcmp(s, "+inserisci_inizio") == 0) {
            if (debug) {
                printf("\nNuove parole ammissibili\n");
            }
            while (1) {
                if (fscanf(stdin, "%s", s) == 0) return -1;
                if (strcmp(s, "+inserisci_fine") == 0) break;
                if (strlen(s) != k) return -1;

                RBinsert(dic, s);
                RBinsert(filtered, s);
                (*num_words) = (*num_words) +1;
                num_filt = num_filt+1;
            }

            if (debug) {
                printf("\nParole ammissibili\n");
                inorderWalk(dic->root);
                printf("\n");
            }
        }
        else {
            if (debug) {
                printf("\nStampo output\n");
            }

            if (strlen(s) != k) return -1;

            if (RBsearch(dic, s) == NIL) {
                fprintf(stdout, "%s", "not_exists\n");
                //continue;
            }
            else {
                n = n-1;
                if (debug) {
                    printf("\nn=%d\n", n);
                }

                if (strcmp(s, r) == 0) {
                    fprintf(stdout, "%s", "ok\n");
                    *found = 1;
                    //break;
                }
                else{
                    /*Per ogni 1 ≤ i ≤ k, si ha che
•
res[i] è il carattere + se l'i-esimo carattere di p è uguale all'i-esimo carattere di r
•
cioè se vale che p[i] = r[i], quindi p[i] è "in posizione corretta"
•
res[i] è il carattere / se l'i-esimo carattere di p non compare da nessuna parte in r
•
res[i] è il carattere | se l'i-esimo carattere di p (indicato nel seguito come p[i]) compare in r, ma non in posizione
            i-esima; tuttavia, se in r compaiono ni istanze di p[i], se ci è il numero di istanze del carattere p[i]
             che sono in posizione corretta (chiaramente deve valere che ci ≤ ni) e se ci sono prima del carattere i-esimo
             in p almeno ni-ci caratteri uguali a p[i] che sono in posizione scorretta, allora res[i] deve essere / invece che |*/
                    int countCharR = 0, countMatchedS = 0, countNotMatchedBeforeS = 0;
                    o = malloc(k* sizeof(char));

                    for (i = 0; i < k; i++) {
                        temp = s[i];
                        node_char *x = RBsearch_char(symbols, temp);
                        if(x==NIL_char) {
                            flag = 0;
                            pos = malloc(sizeof(RB_int));
                            init_int(pos);
                        }
                        else flag=1;

                            for (j = 0; j < k; j++) {
                                if (r[j] == s[i])
                                    countCharR++;
                                if (s[j] == r[j] && s[j] == s[i]) {
                                    if(!flag)   RBinsert_int(pos, i);
                                    countMatchedS++;
                                }
                                if (s[i] != r[j] && s[j] == s[i] && j < i)
                                    countNotMatchedBeforeS++;
                            }
                        if(!flag) {
                            RBinsert_char(symbols, temp, pos, countCharR, countMatchedS);
                            free(pos);
                        }


                        if (s[i] == r[i])
                            o[i] = '+';
                        else {
                            if (countCharR == 0 || (countNotMatchedBeforeS >= countCharR - countMatchedS))
                                o[i]='/';
                            else
                                o[i]='|';
                        }
                    }
                    o[k]='\0';
                    fprintf(stdout, "%s", o);
                    /*Inoltre, dopo ogni confronto, il programma deve stampare in output il numero di parole ammissibili ancora compatibili
 * con i vincoli appresi tranne nel caso di un confronto con esito “not_exists”*/

                    //elimina parole da filtrate
                    mod_filtrate(filtered->root, filtered, symbols, &num_filt, r, s, o, k);
                    fprintf(stdout, "\n%d\n", num_filt);
                    free(o);
                }
            }
        }
    }

    free(filtered);
    free(symbols);
    return 0;
}

int main() {
    NIL = malloc(sizeof(node));
    NIL->color = BLACK;
    NIL_char = malloc(sizeof(node_char));
    NIL_char->color = BLACK;

    int k, found=0, num_words=0;
    char s[30];
    RB * dic = malloc(sizeof(RB));
    init(dic);

    //leggo k = lunghezza delle parole
    if(fscanf(stdin, "%d", &k)==0)    return -1;

    //il sistema legge una sequenza (di lunghezza arbitraria) di parole, ognuna di lunghezza k, che costituisce l'insieme delle parole ammissibili
    while(1){
        if( fscanf (stdin, "%s",s)==0 )    return -1;
        if(strcmp(s, "+nuova_partita")==0)  break;
        if(strlen(s)!=k)    return -1;

        RBinsert(dic, s);
        num_words ++;
    }

    if(debug){
        printf("\nParole ammissibili\n");
        inorderWalk(dic->root);
        printf("\n");
    }

    //r = parola di riferimento (di lunghezza k caratteri)
    char r[30];
    if( fscanf (stdin, "%s",r)==0 )    return -1;
    //numero n massimo di parole da confrontare con la parola di riferimento
    int n;
    if( fscanf (stdin, "%d",&n)==0 )    return -1;

    //sequenza di parole (ognuna di k caratteri) da confrontare con la parola di riferimento
    //Ogni tanto, nella sequenza di stringhe in input, può comparire il comando +stampa_filtrate
    /*Inoltre, sia durante una partita, che tra una partita e l'altra, possono comparire i comandi +inserisci_inizio e +inserisci_fine
     che racchiudono tra di loro una sequenza di nuove parole da aggiungere all'insieme delle parole ammissibili*/
    while(n!=0) {

        if(play(dic, n, k, &num_words, &found, r)!=0)
            printf("\nERRORE!");

        if (debug) {
            printf("\nFine partita\n");
        }

        if (found == 0)
            fprintf(stdout, "%s", "ko\n");
/*Dopo che la partita è finita:
•
Non ci possono essere altre parole da confrontare (ma ci potrebbe essere l'inserimento di nuove parole ammissibili)
•
Se in input c'è il comando +nuova_partita, ha inizio una nuova partita*/
            //una partita è stata finita
        while(1){
            if (fscanf(stdin, "%s", s) == EOF){
                n=0;
                break;
            }
            if (debug) {
                printf("\ns: %s\n", s);
            }

            if (strcmp(s, "+inserisci_inizio") == 0) {
                if (debug) {
                    printf("\nNuove parole ammissibili\n");
                 }
                while (1) {
                    if (fscanf(stdin, "%s", s) == 0) return -1;
                    if (strcmp(s, "+inserisci_fine") == 0) break;
                    if (strlen(s) != k) return -1;

                    RBinsert(dic, s);
                    num_words++;
                }

                if (debug) {
                    printf("\nParole ammissibili\n");
                    inorderWalk(dic->root);
                    printf("\n");
                }
            }
            else if(strcmp(s, "+nuova_partita") == 0){
                found=0;

                if( fscanf (stdin, "%s",r)==0 )    return -1;
                if( fscanf (stdin, "%d",&n)==0 )    return -1;

                break;
            }
            else{
                n=0;
                if (debug) {
                    printf("\ns prima break: %s\n", s);
                }
                break;
            }
        }
    }

    free(dic);
    free(NIL);
    free(NIL_char);
    return 0;
}
