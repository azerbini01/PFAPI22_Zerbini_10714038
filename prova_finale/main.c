#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 128
#define ENTRIES 600000
#define ERROR 0.0005

// BloomFilter (definizione: https://en.wikipedia.org/wiki/Bloom_filter) --> utilizzato per memorizzare tutte le parole ammissibili
typedef struct {
    //numero massimo di parole inseribili
    int entries;
    //probabilità di avere un falso positivo
    double error;
    //numero di bits del bit array
    int bits;
    int bytes;
    //numero di funzioni di hash
    int hashes;
    //bits per elemento
    double bpe;
    //puntatore al bit array
    unsigned char * bf;
}BloomFilter;

BloomFilter * bloom_init(int entries, double error){
    BloomFilter *bloom = malloc(sizeof(BloomFilter));
    bloom->entries = entries;
    bloom->error = error;

    double num = log(bloom->error);
    double denom = 0.480453013918201; // ln(2)^2
    bloom->bpe = -(num / denom);

    double dentries = (double)entries;
    bloom->bits = (int)(dentries * bloom->bpe);

    if (bloom->bits % 8) {
        bloom->bytes = (bloom->bits / 8) + 1;
    } else {
        bloom->bytes = bloom->bits / 8;
    }

    bloom->hashes = (int)ceil(0.693147180559945 * bloom->bpe);  // ln(2)

    bloom->bf = (unsigned char *)calloc(bloom->bytes, sizeof(unsigned char));
    if (bloom->bf == NULL) {
        return NULL;   //init del bit array fallita
    }

    // tutti i bit del bit array vengono settati a 0
    memset(bloom->bf, 0, bloom->bytes);

    return bloom;   //successo
}

//Funzione di hash presa da http://sites.google.com/site/murmurhash/
unsigned int murmurhash2(const void * key, int len, const unsigned int seed){
    // 'm' e 'r' sono "mixing constants"
    const unsigned int m = 0x5bd1e995;
    const int r = 24;

    // Inizializza l'hash su un valore "casuale"

    unsigned int h = seed ^ len;

    // Mescola 4 byte alla volta nell'hash

    const unsigned char * data = (const unsigned char *)key;

    while(len >= 4){
        unsigned int k = *(unsigned int *)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    // Gestisce gli ultimi byte dell'array di input
    switch(len){
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
            h *= m;
    }

    // Esegue alcuni mix finali dell'hash per assicurare che gli ultimi byte siano ben incorporati
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

inline static int test_bit_set_bit(unsigned char * buf, unsigned int x, int set_bit){
    unsigned int byte = x >> 3;
    unsigned char c = buf[byte];
    unsigned int mask = 1 << (x % 8);

    if (c & mask) {
        return 1;
    } else {
        if (set_bit) {  //bloom_add
            buf[byte] = c | mask;
        }
        return 0;
    }
}

static int bloom_check_add(BloomFilter * bloom, const void * buffer, int len, int add){
    int hits = 0;
    register unsigned int a = murmurhash2(buffer, len, 0x9747b28c);
    register unsigned int b = murmurhash2(buffer, len, a);
    register unsigned int x;
    register unsigned int i;

    for (i = 0; i < bloom->hashes; i++) {
        x = (a + i*b) % bloom->bits;
        if (test_bit_set_bit(bloom->bf, x, add)) {
            hits++;
        } else if (!add) {      //bloom_check
            // Basta un solo bit del bit array che non corrisponde per sapere che un elemento non è presente
            return 0;
        }
    }

    if (hits == bloom->hashes) {
        return 1;                // tutti i bit del bit array corrispondenti alla chiave che si vuole inserire sono già stati settati a 1
    }

    return 0;
}

int bloom_check(BloomFilter * bloom, const void * buffer, int len){
    // se la funzione ritorna
    //       0 -> l'elemento non è presente
    //       1 -> l'elemento è presente (o si è verificata una collisione e quindi si ha un falso positivo)
    return bloom_check_add(bloom, buffer, len, 0);
}


int bloom_add(BloomFilter* bloom, const void * buffer, int len){
    // se la funzione ritorna
    //      0 - l'elemento non era presente ed è stato aggiunto
    //      1 - l'elemento è presente (o si è verificata una collisione e quindi si ha un falso positivo)
    return bloom_check_add(bloom, buffer, len, 1);
}

typedef struct node_l {
    char *data;
    struct node_l *next;
} element;

typedef struct {
    int ordered;
    // tiene traccia di quanti nodi sono presenti all'interno della lista
    int count;
    // puntatore al primo nodo della lista
    element *header;
    // puntatore all'ultimo nodo della lista
    element *tailer;
} list;

static inline void init_list(list *l) {
    l->ordered = 0;
    l->count = 0;
    l->header = l->tailer = NULL;
}

// il nodo last viene posizionato nella posizione corretta
static inline element* parition(element* first, element* last) {
    // il pivot punta inizialmente al primo nodo della sottolista
    char *temp;
    element* pivot = first;
    element* front = first;
    while (front != NULL && front != last) {
        if (strcmp(front->data, last->data) < 0) {   //front->data < last->data
            pivot = first;

            // swap delle chiavi
            temp = first->data;
            first->data = front->data;
            front->data = temp;

            // visita del prossimo nodo
            first = first->next;
        }

        // visita del prossimo nodo
        front = front->next;
    }

    // swap della chiave dell'ultimo nodo con quella del nodo corrente
    temp = first->data;
    first->data = last->data;
    last->data = temp;
    return pivot;
}

// Algoritmo di ordinamento per la lista
static inline void quick_sort(element * first, element * last) {
    if (first == last) {
        return;
    }
    element * pivot = parition(first, last);

    if (pivot != NULL && pivot->next != NULL) {
        quick_sort(pivot->next, last);
    }

    if (pivot != NULL && first != pivot) {
        quick_sort(first, pivot);
    }
}

static inline char* list_insert(list *l, char *new_data, int k){
    element *new_node = malloc(sizeof(element));
    new_node->data = malloc((k+1)*sizeof(char));

    strcpy(new_node->data, new_data);
    new_node->next = NULL;

    if (l->count == 0) {
        l->header = l->tailer = new_node;
    } else {
        l->tailer->next = new_node;
        l->tailer = new_node;
    }

    l->count++;
    return new_node->data;
}

static inline void list_insert_punt(list *l, char *new_data){
    element *new_node = malloc(sizeof(element));

    new_node->data = new_data;
    new_node->next = NULL;

    if (l->count == 0) {
        l->header = l->tailer = new_node;
    } else {
        l->tailer->next = new_node;
        l->tailer = new_node;
    }

    l->count++;
}

static inline void free_list(list *l){
    element *curr = l->header, *next;
    while (curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }
    free(l);
}

static inline void print_list(list *l){
    if(l->ordered == 0){
        quick_sort(l->header, l->tailer);
        l->ordered = 1;
    }

    element *curr = l->header;

    while (curr != NULL){
        //printf("%s\n", curr->data);
        puts(curr->data);
        curr = curr->next;
    }
}

typedef struct f{
    char ch_s;
    char symb;
}filterElement;

typedef  filterElement *Filtro;

typedef struct lf{
    Filtro *data;
    struct lf *next;
} listaFiltri;

typedef struct {
    listaFiltri *header;
} headerFilt;

static inline void filt_insert(headerFilt *hf, Filtro *new_data){
    listaFiltri *new_node = malloc(sizeof(listaFiltri));

    new_node->data = new_data;
    new_node->next = hf->header;
    hf->header = new_node;
}

static inline Filtro *genera_filtro(char *s, char *r, int k) {
    Filtro *f = calloc(k, sizeof(filterElement*));

    for (int i = 0; i < k; i++) {
        f[i] = malloc(sizeof(filterElement));
        // current->index = i;
        f[i]->ch_s = s[i];

        // logica per calcolare symb
        if (s[i] == r[i])
            f[i]->symb = '+';
        else {
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

            if (countCharR == 0 ||
                (countNotMatchedBeforeS >= countCharR - countMatchedS))
                f[i]->symb = '/';
            else
                f[i]->symb = '|';
        }
    }
    return f;
}

static inline void stampa_filtro(Filtro *f, int k){
    for (int j = 0; j < k; j++){
        //printf("%c", current->element->symb);
        putc(f[j]->symb, stdout);
    }
    printf("\n");
}

/*
si noti che i vincoli appresi riguardano, per ogni simbolo:
1. se il simbolo non appartiene a r  -> eliminaFilt_slash
2. posti in cui quel simbolo deve comparire in r -> filtra_el
3. posti in cui quel simbolo non può comparire in r -> eliminaFilt_slash e eliminaFilt_pipe
4. numero minimo di volte che il simbolo compare in r -> eliminaFilt_pipe
5. numero esatto di volte che il simbolo compare in r -> eliminaFilt_slash
• si noti che il vincolo 5 è più forte del vincolo 4
*/

static inline int filtra_el(char *data, Filtro *f, int k){
    for(int i=0; i<k; i++){
        char ch = f[i]->ch_s;
        if(f[i]->symb == '+'){
            // 2. posti in cui quel simbolo deve comparire in r
            if(data[i] != ch) return 1;
        }
        else {  //(f[i]->symb == '/' || f[i]->symb == '|')
            // 3. posti in cui quel simbolo non può comparire in r
            if (data[i] == ch) return 1;

            int cont_occ = 0, cont_occ_f = 0;
            for (int j = 0; j < k; j++) {
                if (data[j] == ch) {
                    cont_occ++;
                }
                if (f[j]->ch_s == ch && (f[j]->symb == '+' || f[j]->symb == '|')) {
                    cont_occ_f++;
                }
            }
            if (f[i]->symb == '|') {
                // 4. numero minimo di volte che il simbolo compare in r
                if (cont_occ < cont_occ_f) return 1;
            }
            else /*if (f[i]->symb == '/')*/ {
                // 1. se il simbolo non appartiene a r
                // 5. numero esatto di volte che il simbolo compare in r
                if (cont_occ != cont_occ_f) return 1;
            }
        }
    }
    return 0;
}

list * filtra_dic( list *l_filtered, Filtro *f, int k){
    list *l_new = malloc(sizeof(list));
    init_list(l_new);
    l_new->ordered = l_filtered->ordered;

    element *cur_node = l_filtered->header;
    while(cur_node != NULL) {
        element *next = cur_node->next;
        if (filtra_el(cur_node->data, f, k)==0) list_insert_punt(l_new, cur_node->data);
        cur_node = next;
    }
    return l_new;
}

static inline int passa_filtri(char *new_data, headerFilt *f_head, int k){
    listaFiltri *curr = f_head->header;

    while(curr != NULL){
        if(filtra_el(new_data, curr->data, k))  return 0;   //non devo inserire un nuovo nodo
        curr = curr->next;
    }
    return 1;   //nuovo nodo da inserire
}

void free_filtro(Filtro *f, int k){
    for(int i=0; i<k; i++){
        free(f[i]);
    }
}

void free_lfiltri(headerFilt *headerF, int k){
    listaFiltri *current=headerF->header, *next;

    while(current != NULL){
        next = current->next;
        free_filtro(current->data, k);
        free(current);
        current = next;
    }
    free(headerF);
}

int play(BloomFilter *dic, int n, int k, int *found, char *r, list *l_initial){
    char s[MAX_LENGTH];
    list *l_filtered = l_initial;
    headerFilt *f_head = malloc(sizeof(headerFilt));
    f_head->header = NULL;

    while (n > 0) {
        if (fscanf(stdin, "%s", s) == 0) return -1;

        if (strcmp(s, "+stampa_filtrate") == 0) {
            print_list(l_filtered);
        }
        else if (strcmp(s, "+inserisci_inizio") == 0) {
            while (1) {
                if (fscanf(stdin, "%s", s) == 0) return -1;
                if (strcmp(s, "+inserisci_fine") == 0) break;
                if (strcmp(s, "+stampa_filtrate") == 0) {
                    print_list(l_filtered);
                    continue;
                }

                bloom_add(dic, s, k);
                char *ret = list_insert(l_initial, s, k);
                if(passa_filtri(s, f_head, k))   {
                    if (l_filtered != l_initial) list_insert_punt(l_filtered, ret);
                }

            }
            l_initial->ordered = 0;
            l_filtered->ordered = 0;
        }
        else if(strcmp(s, "+nuova_partita")==0) {
            *found = 0;
            free_lfiltri(f_head, k);
            if (l_filtered != l_initial){
                free_list(l_filtered);
            }

            if( fscanf (stdin, "%s",r)==0 )    return -1;
            if( fscanf (stdin, "%d",&n)==0 )    return -1;

            l_filtered = l_initial;
            f_head = malloc(sizeof(headerFilt));
            f_head->header = NULL;
            continue;
        }
        else {
            if (bloom_check(dic, s, k) == 0) {
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
                    Filtro *f = genera_filtro(s, r, k);
                    filt_insert(f_head, f);
                    stampa_filtro(f, k);

                    list *to_clean = l_filtered;
                    if (l_filtered == l_initial){
                        to_clean = NULL;
                    }

                    l_filtered = filtra_dic(l_filtered, f, k);

                    if (to_clean != NULL){
                        free_list(to_clean);
                    }

                    printf("%d\n", l_filtered->count);
                }
            }
        }
    }
    if (l_filtered != l_initial){
        free_list(l_filtered);
    }
    free_lfiltri(f_head, k);
    return 0;
}

int main() {
    int k, found=0;
    char s[MAX_LENGTH];
    BloomFilter *dic = bloom_init(ENTRIES, ERROR);
    list *l_filtered = malloc(sizeof(list));
    init_list(l_filtered);

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
                    print_list(l_filtered);
                    continue;
                }

                bloom_add(dic, s, k);
                list_insert(l_filtered, s, k);
            }
            continue;
        }
        if (strcmp(s, "+stampa_filtrate") == 0) {
            print_list(l_filtered);
            continue;
        }

        bloom_add(dic, s, k);
        list_insert(l_filtered, s, k);
    }
    l_filtered->ordered = 0;

    //r = parola di riferimento (di lunghezza k caratteri)
    char r[k+1];
    if( fscanf (stdin, "%s",r)==0 )    return -1;
    //numero n massimo di parole da confrontare con la parola di riferimento
    int n;
    if( fscanf (stdin, "%d",&n)==0 )    return -1;

    while(n!=0) {

        if(play(dic, n, k, &found, r, l_filtered)!=0)
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
                        print_list(l_filtered);
                        continue;
                    }

                    bloom_add(dic, s, k);
                    list_insert(l_filtered, s, k);
                }
                l_filtered->ordered = 0;
            }
            else if (strcmp(s, "+stampa_filtrate") == 0) {
                print_list(l_filtered);
            }
            else if(strcmp(s, "+nuova_partita") == 0){
                found=0;

                if( fscanf (stdin, "%s",r)==0 )    return -1;
                if( fscanf (stdin, "%d",&n)==0 )    return -1;

                break;
            }
            else{
                exit(1);
            }
        }
    }
    free_list(l_filtered);
    return 0;
}

