#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 30
#define RED 0
#define BLACK 1

#define debug 0

typedef struct n{
    char key[LENGTH];
    int color;
    struct n *p;
    struct n *left;
    struct n *right;
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

node* RBinsert(RB *T, char* key){
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
        if(strcmp(z->key,x->key)==0)     return x;
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

    return z;
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
    //free(x);
}

typedef struct{
    int index;
    char ch_s;
    char symb;
}filter_element;

typedef struct f{
    filter_element * element;
    struct f *left;
    struct f *right;
}filtro;

void init_filtro(filtro *f) {
    f->element = NULL;
    f->left = f->right = NULL;
}

typedef struct node_l {
    node *data;
    struct node_l *prev;
    struct node_l *next;
} element;

typedef struct {
    // tiene traccia di quanti nodi sono presenti all'interno della lista
    int count;
    // puntatore al primo nodo della lista
    element *header;
    // puntatore all'ultimo nodo della lista
    element *tailer;
} list;

void init_list(list *l) {
    l->count = 0;
    l->header = l->tailer = NULL;
}

void list_insert(list *l, node *new_data){
    element *new_node;
    new_node = malloc(sizeof(element));

    new_node->data = new_data;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (l->count == 0) {
        new_node->prev = NULL;
        l->header = l->tailer = new_node;
    } else {
        new_node->prev = l->tailer;
        l->tailer->next = new_node;
        l->tailer = new_node;
    }

    l->count++;
}

void delete_node_l(list *l, element *n){
    if(l->header == n)
        l->header = n->next;

    if(l->tailer == n)
        l->tailer = n->prev;

    n->prev->next = n->next;
    if (n->next != NULL)
        n->next->prev = n->prev;

    free(n);
    l->count--;
}

void RBcopy( node *source, RB *Dest, list *l){
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

        node *z = RBinsert(Dest, source->key);
        list_insert(l, z);
        RBcopy(source->left, Dest, l);
        RBcopy(source->right, Dest, l);
    }
}

filtro *genera_filtro (char* s, char* r, int k){
    filtro *f = malloc(sizeof(filtro));
    filtro *previous = malloc(sizeof(filtro));

    for(int i=0; i<k; i++){
        filtro* current = malloc(sizeof(filtro));
        filter_element *e = malloc(sizeof(filter_element));
        e->index = i;
        e->ch_s = s[i];

        // logica per calcolare symb
        int countCharR = 0, countMatchedS = 0, countNotMatchedBeforeS = 0;

        for (int j = 0; j < k; j++) {
            if (r[j] == s[i])
                countCharR++;
            if (s[j] == r[j] && s[j] == s[i]) {
                countMatchedS++;
            }
            if (s[i] != r[j] && s[j] == s[i] && j < i)
                countNotMatchedBeforeS++;
        }

        if (s[i] == r[i])
            e->symb = '+';
        else {
            if (countCharR == 0 || (countNotMatchedBeforeS >= countCharR - countMatchedS))
                e->symb ='/';
            else
                e->symb ='|';
        }

        current->element = e;
        current->right = NULL;
        current->left = NULL;
        if(i == 0){
            f = current;
        } else {
            previous->right = current;
            current->left = previous;
        }
        previous = current;
    }
    return f;
}

void stampa_filtro(filtro *f){
    filtro *current=f;
    while(current->right != NULL){
        printf("%c", current->element->symb);
        current = current->right;
    }
    printf("\n");
}

int eliminaFilt_pipe(int index, char ch_s, char *s, filtro *f_header, int k){
    if(s[index] == ch_s)    return 1;

    filtro *fe = f_header;
    int cont_occ=0;
    for(int i=0; i<k; i++){
            if(s[i] == ch_s) {
                if(!(fe->element->symb == '+' && fe->element->ch_s == ch_s ))
                    cont_occ ++;
            }
            fe = fe->right;
    }
    if(cont_occ == 0)   return 1;
    return 0;
}

int eliminaFilt_slash(int index, char ch_s, char *s, filtro *f_header, int k){
    int cont_occ_s=0, cont_occ_f=0;
    filtro *fe = f_header;

    for(int i=0; i<k; i++){    //veroficare < <=
        if(s[i] == ch_s) {
            cont_occ_s ++;
            if(fe->element->symb == '+' && fe->element->ch_s == ch_s )
                cont_occ_f ++;
            if(fe->element->symb == '|' && fe->element->ch_s == ch_s )
                cont_occ_f ++;
        }
        fe = fe->right;
    }
    if(cont_occ_s >0 && cont_occ_s > cont_occ_f)    return 1;
    return 0;
}

void filtra_dic( RB* filtered, list *l_filtered, filtro *f, int k){
    filtro *f_header = f;
    filtro *current = f;

    while(current->right != NULL){
        element *cur_node = l_filtered->header;
        while(cur_node != NULL){
            if(current->element->symb == '+'){
                if(cur_node->data->key[current->element->index] != current->element->ch_s){
                    element *nodeToRemove = cur_node;
                    cur_node = cur_node->next;

                    RBdelete(filtered, nodeToRemove->data);
                    delete_node_l(l_filtered, nodeToRemove);
                    continue;
                }
            }
            else if(current->element->symb == '/') {
                if (eliminaFilt_slash(current->element->index, current->element->ch_s, cur_node->data->key, f_header, k)) {
                    element *nodeToRemove = cur_node;
                    cur_node = cur_node->next;

                    RBdelete(filtered, nodeToRemove->data);
                    delete_node_l(l_filtered, nodeToRemove);
                    continue;
                }
            }
            else if(current->element->symb == '|') {
                if (eliminaFilt_pipe(current->element->index, current->element->ch_s, cur_node->data->key, f_header, k)) {
                    element *nodeToRemove = cur_node;
                    cur_node = cur_node->next;

                    RBdelete(filtered, nodeToRemove->data);
                    delete_node_l(l_filtered, nodeToRemove);
                    continue;
                }
            }

            cur_node = cur_node->next;
        }
        current = current->right;
    }
}

int play(RB *dic, int n, int k, int *found, char *r){
    char s[LENGTH];
    list *l_filtered = malloc(sizeof(list));
    init_list(l_filtered);
    RB *filtered = malloc(sizeof(RB));
    init(filtered);

    RBcopy(dic->root, filtered, l_filtered);

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
                node *z = RBinsert(filtered, s);
                list_insert(l_filtered, z);
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
                } else {
                    filtro *f = genera_filtro(s, r, k);
                    stampa_filtro(f);

                    filtra_dic(filtered, l_filtered, f, k);

                    /*Inoltre, dopo ogni confronto, il programma deve stampare in output il numero di parole ammissibili ancora compatibili
 * con i vincoli appresi tranne nel caso di un confronto con esito “not_exists”*/
                    printf("\n%d", l_filtered->count);
                    free(f);
                }
            }
        }
    }

    free(filtered);
    free(l_filtered);
    return 0;
}

int main() {
    NIL = malloc(sizeof(node));
    NIL->color = BLACK;

    int k, found=0;
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

        if(play(dic, n, k, &found, r)!=0)
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
                printf("\nStringa in ingresso: %s\n", s);
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
                }

                if (debug) {
                    printf("\nParole ammissibili\n");
                    inorderWalk(dic->root);
                    printf("\n");
                }
            }
            else if(strcmp(s, "+nuova_partita") == 0){
                found=0;
                if (debug)
                    printf("\nNuova partita\n");

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
    return 0;
}
