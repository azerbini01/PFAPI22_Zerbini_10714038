#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 25
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
    node *root;
    node *NIL;
}RB;

void init(RB* T){
    T->NIL->color = BLACK;
    T->root = T->NIL;
    T->root->p = T->NIL;
    T->root->right = T->NIL;
    T->root->left = T->NIL;
}

void left_rotate(RB* T, node *x){
    node *y;

    y = x->right;
    x->right = y->left;
    if(y->left != T->NIL)
        y->left->p = x;

    y->p = x->p;
    if(y->p == T->NIL)
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
    if(y->right != T->NIL)
        y->right->p = x;

    y->p = x->p;
    if(y->p == T->NIL)
        T->root = y;
    else if(x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;

    y->right = x;
    x->p = y;
}

void inorderWalk(RB* T, node *x){
    if(x != T->NIL){
        inorderWalk(T, x->left);
        printf("%s\n", x->key);
        inorderWalk(T, x->right);
    }
}

void RBfree(RB* T, node *x){
    if(x != T->NIL){
        RBfree(T, x->left);
        RBfree(T, x->right);
        free(x);
    }
}

node *RBsearch(RB* T, char* key){
    node *x = T->root;

    while(x != T->NIL && strcmp(x->key, key)!=0){  //x->key != key
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
    z->left = T->NIL;
    z->right = T->NIL;

    x = T->root;
    y = T->NIL;

    while(x != T->NIL){
        y = x;
        if(strcmp(z->key,x->key)==0)     return x;
        else if(strcmp(z->key,x->key)<0) //z->key < x->key
            x = x->left;
        else
            x = x->right;
    }

    if(y == T->NIL)
        T->root = z;
    else if(strcmp(z->key,y->key)<=0) //z->key <= y->key
        y->left = z;
    else
        y->right = z;

    z->p = y;

    RBinsert_fixup(T, z);

    return z;
}

typedef struct{
    int index;
    char ch_s;
    char symb;
}filter_element;

typedef struct f{
    filter_element * element;
    struct f *right;
    //struct f *left;
}filtro;

typedef struct lf{
    filtro *data;
    struct lf *next;
} listaFiltri;

typedef struct {
    listaFiltri *header;
} headerFilt;

void filt_insert(headerFilt *hf, filtro *new_data){
    listaFiltri *new_node = malloc(sizeof(listaFiltri));

    new_node->data = new_data;
    new_node->next = hf->header;
    hf->header = new_node;
}

typedef struct node_l {
    char *data;
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

void list_insert(list *l, char *new_data){
    element *new_node = malloc(sizeof(element));
    new_node->data = malloc(LENGTH*sizeof(char));

    strcpy(new_node->data, new_data);
    //new_node->data = new_data;
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

void list_insert_inOrder(list *l, char *new_data){
    element *curr = l->header;
    element *new_node = malloc(sizeof(element));
    new_node->data = malloc(LENGTH*sizeof(char));

    strcpy(new_node->data, new_data);
    new_node->next = NULL;
    new_node->prev = NULL;

    if (l->count == 0) {
        new_node->prev = NULL;
        l->header = l->tailer = new_node;
        l->count++;
        return;
    }

    while(curr!=NULL && strcmp(curr->data, new_data)<0){
        curr = curr->next;
    }

    if(curr == NULL) {
        new_node->prev = l->tailer;
        l->tailer->next = new_node;
        l->tailer = new_node;
    } else {
        if (curr == l->header) {
            new_node->prev = NULL;
            new_node->next = curr;
            curr->prev = new_node;
            l->header = new_node;
        } else {
            curr->prev->next = new_node;
            new_node->prev = curr->prev;
            new_node->next = curr;
            curr->prev = new_node;
        }
    }

    l->count++;
}

void delete_node_l(list *l, element *n){
    if(l->header == n)
        l->header = n->next;
    if(l->tailer == n)
        l->tailer = n->prev;

    if (n->prev != NULL)
        n->prev->next = n->next;
    if (n->next != NULL)
        n->next->prev = n->prev;

    free(n);
    l->count--;
}

void print_list(list *l){
    element *curr = l->header;

    while (curr != NULL){
        printf("%s\n", curr->data);
        curr = curr->next;
    }
}

void free_list(list *l){
    element *curr = l->header, *next;

    while (curr != NULL){
        next = curr->next;
        //free(curr->data);
        free(curr);
        curr = next;
    }
}

void RBcopy(RB* Ts, node *source, list *l){
    if(source != Ts->NIL){
        RBcopy(Ts, source->left, l);
        list_insert(l, source->key);
        RBcopy(Ts, source->right, l);
    }
}

filtro *genera_filtro (char* s, char* r, int k){
    filtro *f = malloc(sizeof(filtro));
    filtro *previous=f /*= malloc(sizeof(filtro))*/;

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
        //current->left = NULL;
        if(i == 0){
            f = current;
        } else {
            previous->right = current;
            //current->left = previous;
        }
        previous = current;
    }
    return f;
}

void stampa_filtro(filtro *f){
    filtro *current=f;
    while(current != NULL){
        printf("%c", current->element->symb);
        current = current->right;
    }
    printf("\n");

}

void free_filtro(filtro *f){
    filtro *current=f, *next;

    while(current != NULL){
        next = current->right;
        free(current->element);
        free(current);
        current = next;
    }
}

void free_lfiltri(listaFiltri *lf){
    listaFiltri *current=lf, *next;

    while(current != NULL){
        next = current->next;
        free_filtro(current->data);
        free(current);
        current = next;
    }
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

    for(int i=0; i<k; i++){
        if(s[i] == ch_s)
            cont_occ_s ++;
        if(fe->element->symb == '+' && fe->element->ch_s == ch_s )
            cont_occ_f ++;
        if(fe->element->symb == '|' && fe->element->ch_s == ch_s )
            cont_occ_f ++;

        fe = fe->right;
    }
    if(cont_occ_s >0 && cont_occ_s > cont_occ_f)    return 1;
    return 0;
}

void filtra_dic( list *l_filtered, filtro *f, int k){
    filtro *f_header = f;
    filtro *current = f;

    while(current != NULL){
        element *cur_node = l_filtered->header;
        while(cur_node != NULL){
            if(current->element->symb == '+'){
                if(cur_node->data[current->element->index] != current->element->ch_s){
                    element *nodeToRemove = cur_node;
                    cur_node = cur_node->next;

                    delete_node_l(l_filtered, nodeToRemove);
                    continue;
                }
            }
            else if(current->element->symb == '/') {
                if (eliminaFilt_slash(current->element->index, current->element->ch_s, cur_node->data, f_header, k)) {
                    element *nodeToRemove = cur_node;
                    cur_node = cur_node->next;

                    delete_node_l(l_filtered, nodeToRemove);
                    continue;
                }
            }
            else if(current->element->symb == '|') {
                if (eliminaFilt_pipe(current->element->index, current->element->ch_s, cur_node->data, f_header, k)) {
                    element *nodeToRemove = cur_node;
                    cur_node = cur_node->next;

                    delete_node_l(l_filtered, nodeToRemove);
                    continue;
                }
            }

            cur_node = cur_node->next;
        }
        current = current->right;
    }
}

void passaFiltri(list *l_filtered, headerFilt *f_head, int k){
    listaFiltri *curr = f_head->header;

    while(curr != NULL){
        filtra_dic(l_filtered, curr->data, k);
        curr = curr->next;
    }
}

int play(RB *dic, int n, int k, int *found, char *r){
    char s[LENGTH];
    filtro *f;
    list *l_filtered = malloc(sizeof(list));
    init_list(l_filtered);
    headerFilt *f_head = malloc(sizeof(headerFilt));
    f_head->header = NULL;

    RBcopy(dic, dic->root, l_filtered);
    if (debug) {
        printf("Prima stampa filtrate list\n");
        print_list(l_filtered);
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
*/          print_list(l_filtered);

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
                list_insert_inOrder(l_filtered, s);
            }

            if (debug) {
                printf("\nParole ammissibili\n");
                inorderWalk(dic, dic->root);
                printf("\n");
                printf("Parole filtrate\n");
                print_list(l_filtered);
            }
        }
        else {
            if (debug) {
                printf("\nStampo output\n");
            }

            if (strlen(s) != k) return -1;

            if (RBsearch(dic, s) == dic->NIL) {
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
                    break;
                } else {
                    f = genera_filtro(s, r, k);
                    filt_insert(f_head, f);
                    stampa_filtro(f);

                    passaFiltri(l_filtered, f_head, k);

                    /*Inoltre, dopo ogni confronto, il programma deve stampare in output il numero di parole ammissibili ancora compatibili
 * con i vincoli appresi tranne nel caso di un confronto con esito “not_exists”*/
                    printf("%d\n", l_filtered->count);

                    if(debug){
                        printf("\nParole filtrate lista: ");
                        print_list(l_filtered);
                    }
                }
            }
        }
    }

    free_lfiltri(f_head->header);
    free(f_head);
    free_list(l_filtered);
    free(l_filtered);
    return 0;
}

int main() {
    int k, found=0;
    char s[LENGTH];
    RB * dic = malloc(sizeof(RB));
    dic->NIL = malloc(sizeof(node));
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
        inorderWalk(dic, dic->root);
        printf("\n");
    }

    //r = parola di riferimento (di lunghezza k caratteri)
    char r[LENGTH];
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
                    inorderWalk(dic, dic->root);
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

    free(dic->NIL);
    free(dic);
    return 0;
}
