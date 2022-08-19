#include <stdio.h>
#include <stdlib.h>
//#include <string.h>

#define MAX_LENGTH 1024
#define RED 0
#define BLACK 1

static unsigned char partita;
static int count;
static int countFilt;

static inline char* strcpy(char* destination, const char* source){
    /* return if no memory is allocated to the destination
    if (destination == NULL) {
        return NULL;
    } */

    // take a pointer pointing to the beginning of the destination string
    char *ptr = destination;

    // copy the C-string pointed by source into the array pointed by destination
    while (*source != '\0'){
        *destination = *source;
        destination++;
        source++;
    }

    // include the terminating null character
    *destination = '\0';

    // the destination is returned by standard `strcpy()`
    return ptr;
}

static inline int strcmp(const char *X, const char *Y){
    while (*X){
        // se i caratteri differiscono o viene raggiunta la fine della seconda stringa
        if (*X != *Y) {
            break;
        }

        // passa alla coppia di caratteri successiva
        X++;
        Y++;
    }
    // restituisce la differenza ASCII dopo aver convertito `char*` in `unsigned char*`
    return *(const unsigned char*)X - *(const unsigned char*)Y;
}

typedef struct n{
    char *key;
    char color;
    unsigned char partita;
    struct n *p;
    struct n *left;
    struct n *right;
}node;

typedef struct{
    node *root;
    node *NIL;
}RB;

static inline void init(RB* T){
    T->NIL = malloc(sizeof (node));
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

void stampaFiltrate(RB* T, node *x){
    if(x != T->NIL){
        stampaFiltrate(T, x->left);
        if(x->partita <= partita)
            printf("%s\n", x->key);
        stampaFiltrate(T, x->right);
    }
}

/* void RBfree(RB* T, node *x){
    if(x != T->NIL){
        RBfree(T, x->left);
        RBfree(T, x->right);
        free(x);
    }
} */

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

node* RBinsert(RB *T, char* key, int k){
    node *z, *x, *y;
    z = malloc(sizeof(node));
    z->key = malloc(sizeof(char)*(k+1));

    strcpy(z->key,key);   //z->key = key
    z->partita = partita;
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

typedef struct f{
    //filter_element * element;
    int index;
    char ch_s;
    char symb;
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

static inline void filt_insert(headerFilt *hf, filtro *new_data){
    listaFiltri *new_node = malloc(sizeof(listaFiltri));

    new_node->data = new_data;
    new_node->next = hf->header;
    hf->header = new_node;
}

static inline filtro *genera_filtro (char* s, char* r, int k){
    filtro *f = malloc(sizeof(filtro));
    filtro *previous=f /*= malloc(sizeof(filtro))*/;

    for(int i=0; i<k; i++){
        filtro* current = malloc(sizeof(filtro));
        current->index = i;
        current->ch_s = s[i];

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
            current->symb = '+';
        else {
            if (countCharR == 0 || (countNotMatchedBeforeS >= countCharR - countMatchedS))
                current->symb ='/';
            else
                current->symb ='|';
        }

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

static inline void stampa_filtro(filtro *f){
    filtro *current=f;
    while(current != NULL){
        //printf("%c", current->element->symb);
        putc(current->symb, stdout);
        current = current->right;
    }
    printf("\n");
}

static inline int eliminaFilt_pipe(int index, char ch_s, char *s, filtro *f_header, int k){
    if(s[index] == ch_s)    return 1;

    filtro *fe = f_header;
    int cont_occ=0;
    for(int i=0; i<k; i++){
        if(s[i] == ch_s) {
            if(fe->ch_s != ch_s)
                cont_occ ++;
        }
        fe = fe->right;
    }
    if(cont_occ == 0)   return 1;
    return 0;
}

static inline int eliminaFilt_slash(int index, char ch_s, char *s, filtro *f_header, int k){
    if(s[index] == ch_s)    return 1;

    int cont_occ_s=0, cont_occ_f=0;
    filtro *fe = f_header;

    for(int i=0; i<k; i++){
        if(s[i] == ch_s)
            cont_occ_s ++;
        if(fe->ch_s == ch_s && fe->symb == '+')
            cont_occ_f ++;
        if(fe->ch_s == ch_s && fe->symb == '|')
            cont_occ_f ++;

        fe = fe->right;
    }
    if(cont_occ_s >0 && cont_occ_s > cont_occ_f)    return 1;
    return 0;
}

static inline void filtra_dic( RB* T, node *x, filtro *f, int k){

    if(x != T->NIL){
        filtra_dic(T, x->left, f, k);
        if(x->partita <= partita) {
            filtro *current = f;
            while (current != NULL) {
                if (current->symb == '+') {
                    if (x->key[current->index] != current->ch_s) {
                        x->partita = (char)(partita+1);
                        countFilt--;
                        break;
                    }
                } else if (current->symb == '/') {
                    if (eliminaFilt_slash(current->index, current->ch_s, x->key, f, k)) {
                        x->partita = (char)(partita+1);
                        countFilt--;
                        break;
                    }
                } else if (current->symb == '|') {
                    if (eliminaFilt_pipe(current->index, current->ch_s, x->key, f, k)) {
                        x->partita = (char)(partita+1);
                        countFilt--;
                        break;
                    }
                }
                current = current->right;
            }
        }
        filtra_dic(T, x->right, f, k);
    }
}

static inline int filtra_el(char *data, filtro *f, int k){
    filtro *current = f;

    while(current != NULL){
        if(current->symb == '+'){
            if(data[current->index] != current->ch_s){
                return 1;
            }
        }
        else if(current->symb == '/') {
            if (eliminaFilt_slash(current->index,current->ch_s, data, f, k)) {
                return 1;
            }
        }
        else if(current->symb == '|') {
            if (eliminaFilt_pipe(current->index, current->ch_s, data, f, k)) {
                return 1;
            }
        }
        current = current->right;
    }
    return 0;
}

static inline void passaFiltri(node *new, headerFilt *f_head, int k){
    listaFiltri *curr = f_head->header;

    while(curr != NULL){
        if(filtra_el(new->key, curr->data, k)) {    //non devo inserire un nuovo nodo in filtrate
            new->partita = (unsigned char) (partita + 1);
            return;
        }
        curr = curr->next;
    }
    //nuovo nodo da inserire
    countFilt++;
}

void free_filtro(filtro *f){
    filtro *current=f, *next;

    while(current != NULL){
        next = current->right;
        free(current);
        current = next;
    }
}

void free_lfiltri(headerFilt *headerF){
    listaFiltri *current=headerF->header, *next;

    while(current != NULL){
        next = current->next;
        free_filtro(current->data);
        free(current);
        current = next;
    }
    free(headerF);
}

int play(RB *dic, int n, int k, int *found, char *r){
    countFilt = count;
    char s[MAX_LENGTH];
    headerFilt *f_head = malloc(sizeof(headerFilt));
    f_head->header = NULL;

    while (n > 0) {
        if (fscanf(stdin, "%s", s) == 0) return -1;

        if (strcmp(s, "+stampa_filtrate") == 0) {
            stampaFiltrate(dic, dic->root);
        }
        else if (strcmp(s, "+inserisci_inizio") == 0) {
            while (1) {
                if (fscanf(stdin, "%s", s) == 0) return -1;
                if (strcmp(s, "+inserisci_fine") == 0) break;
                if (strcmp(s, "+stampa_filtrate") == 0) {
                    stampaFiltrate(dic, dic->root);
                    continue;
                }

                node *new = RBinsert(dic, s, k);
                count++;
                passaFiltri(new, f_head, k);
            }
        }
        else if(strcmp(s, "+nuova_partita")==0) {
            *found = 0;
            partita++;
            free_lfiltri(f_head);

            if( fscanf (stdin, "%s",r)==0 )    return -1;
            if( fscanf (stdin, "%d",&n)==0 )    return -1;

            countFilt = count;
            f_head = malloc(sizeof(headerFilt));
            f_head->header = NULL;
            continue;
        }
        else {
            if (RBsearch(dic, s) == dic->NIL) {
                fprintf(stdout, "%s", "not_exists\n");
                //continue;
            }
            else {
                n = n-1;

                if (strcmp(s, r) == 0) {
                    fprintf(stdout, "%s", "ok\n");
                    *found = 1;
                    break;
                } else {
                    filtro *f = genera_filtro(s, r, k);
                    filt_insert(f_head, f);
                    stampa_filtro(f);

                    filtra_dic(dic, dic->root, f, k);
                    printf("%d\n", countFilt);
                }
            }
        }
    }

    partita++;
    free_lfiltri(f_head);
    return 0;
}

int main() {
    partita = (unsigned char)0;
    count = 0;
    int k, found=0;
    char s[MAX_LENGTH];
    RB *dic = malloc(sizeof(RB));
    init(dic);

    //leggo k = lunghezza delle parole
    if(fscanf(stdin, "%d", &k)==0)    return -1;


    //il sistema legge una sequenza (di lunghezza arbitraria) di parole, ognuna di lunghezza k, che costituisce l'insieme delle parole ammissibili
    while(1){
        if (fscanf(stdin, "%s", s) == EOF){
            exit(0);
        }
        if(strcmp(s, "+nuova_partita")==0)  break;
        if (strcmp(s, "+inserisci_inizio") == 0) {
            while (1) {
                if (fscanf(stdin, "%s", s) == 0) return -1;
                if (strcmp(s, "+inserisci_fine") == 0) break;
                if (strcmp(s, "+stampa_filtrate") == 0) {
                    inorderWalk(dic, dic->root);
                    continue;
                }

                RBinsert(dic, s, k);
                count++;
            }
            continue;
        }
        if (strcmp(s, "+stampa_filtrate") == 0) {
            inorderWalk(dic, dic->root);
            continue;
        }

        RBinsert(dic, s, k);
        count++;
    }

    //r = parola di riferimento (di lunghezza k caratteri)
    char r[k+1];
    if( fscanf (stdin, "%s",r)==0 )    return -1;
    //numero n massimo di parole da confrontare con la parola di riferimento
    int n;
    if( fscanf (stdin, "%d",&n)==0 )    return -1;

    while(n!=0) {
        if(play(dic, n, k, &found, r)!=0)
            printf("\nERRORE!");

        if (found == 0)
            fprintf(stdout, "%s", "ko\n");

        //una partita è stata finita
        while(1){
            if (fscanf(stdin, "%s", s) == EOF){
                exit(0);
            }

            if (strcmp(s, "+inserisci_inizio") == 0) {
                while (1) {
                    if (fscanf(stdin, "%s", s) == 0) return -1;
                    if (strcmp(s, "+inserisci_fine") == 0) break;
                    if (strcmp(s, "+stampa_filtrate") == 0) {
                        inorderWalk(dic, dic->root);
                        continue;
                    }

                    RBinsert(dic, s, k);
                    count++;
                }
            }
            else if (strcmp(s, "+stampa_filtrate") == 0) {
                inorderWalk(dic, dic->root);
            }
            else if(strcmp(s, "+nuova_partita") == 0){
                found=0;

                if( fscanf (stdin, "%s",r)==0 )    return -1;
                if( fscanf (stdin, "%d",&n)==0 )    return -1;

                break;
            }
            else{
                exit(0);
            }
        }
    }

    return 0;
}


