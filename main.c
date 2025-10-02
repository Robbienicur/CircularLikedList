#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;            // value stored
    struct Node *next;   // next pointer
} Node;

typedef struct {
    Node *tail;          // we keep tail, head = tail->next
    size_t size;         // how many items
} CList;

// init & checks
void cl_init(CList *L){ L->tail = NULL; L->size = 0; }
bool cl_is_empty(const CList *L){ return L->tail == NULL; }
size_t cl_size(const CList *L){ return L->size; }

// add
bool cl_add_front(CList *L, int x){
    Node *n = (Node*)malloc(sizeof(Node));
    if(!n) return false; // no mem
    n->data = x;
    if (cl_is_empty(L)){
        n->next = n;     // first points to itself
        L->tail = n;
    } else {
        n->next = L->tail->next; // to old head
        L->tail->next = n;       // now n is head
    }
    L->size++;
    return true;
}

bool cl_add_back(CList *L, int x){
    Node *n = (Node*)malloc(sizeof(Node));
    if(!n) return false;
    n->data = x;
    if (cl_is_empty(L)){
        n->next = n;
        L->tail = n;
    } else {
        n->next = L->tail->next; // to head
        L->tail->next = n;
        L->tail = n;             // move tail
    }
    L->size++;
    return true;
}

bool cl_add_at(CList *L, size_t pos, int x){
    if (pos > L->size) return false;
    if (pos == 0) return cl_add_front(L, x);
    if (pos == L->size) return cl_add_back(L, x);

    Node *n = (Node*)malloc(sizeof(Node));
    if(!n) return false;
    n->data = x;

    Node *prev = L->tail->next; // head
    for (size_t i = 0; i < pos - 1; ++i) prev = prev->next; // go before pos
    n->next = prev->next;
    prev->next = n;
    L->size++;
    return true;
}

// remove
bool cl_remove_front(CList *L, int *out){
    if (cl_is_empty(L)) return false;
    Node *head = L->tail->next;
    if (out) *out = head->data;
    if (head == L->tail){
        L->tail = NULL;          // removing last one
    } else {
        L->tail->next = head->next; // jump over head
    }
    free(head);
    L->size--;
    return true;
}

bool cl_remove_back(CList *L, int *out){
    if (cl_is_empty(L)) return false;
    Node *tail = L->tail;
    if (out) *out = tail->data;

    if (tail->next == tail){     // only one
        L->tail = NULL;
    } else {
        Node *prev = tail->next; // start at head
        while (prev->next != tail) prev = prev->next; // find prev
        prev->next = tail->next;
        L->tail = prev;
    }
    free(tail);
    L->size--;
    return true;
}

bool cl_remove_at(CList *L, size_t pos, int *out){
    if (cl_is_empty(L) || pos >= L->size) return false;
    if (pos == 0) return cl_remove_front(L, out);
    if (pos == L->size - 1) return cl_remove_back(L, out);

    Node *prev = L->tail->next; // head
    for (size_t i = 0; i < pos - 1; ++i) prev = prev->next;
    Node *t = prev->next;
    prev->next = t->next;
    if (out) *out = t->data;
    free(t);
    L->size--;
    return true;
}

// search and access
int cl_search(const CList *L, int x){
    if (cl_is_empty(L)) return -1;
    Node *cur = L->tail->next;
    for (size_t i = 0; i < L->size; ++i){
        if (cur->data == x) return (int)i;
        cur = cur->next;
    }
    return -1; // not found
}

bool cl_get_head(const CList *L, int *out){
    if (cl_is_empty(L)) return false;
    if (out) *out = L->tail->next->data;
    return true;
}

bool cl_get_tail(const CList *L, int *out){
    if (cl_is_empty(L)) return false;
    if (out) *out = L->tail->data;
    return true;
}

// traverse, display, clear
void cl_traverse(const CList *L, void (*visit)(int)){
    if (cl_is_empty(L) || !visit) return;
    Node *cur = L->tail->next;
    for (size_t i = 0; i < L->size; ++i){
        visit(cur->data);  // do something with value
        cur = cur->next;
    }
}

void cl_display(const CList *L){
    if (cl_is_empty(L)){
        puts("Lista: []");
        return;
    }
    Node *cur = L->tail->next;
    printf("Lista: [");
    for (size_t i = 0; i < L->size; ++i){
        printf("%d", cur->data);
        if (i + 1 < L->size) printf(" -> ");
        cur = cur->next;
    }
    puts("] (circular)");
}

void cl_clear(CList *L){
    int tmp;
    while (!cl_is_empty(L)) cl_remove_front(L, &tmp); // free all
}

// simple demo menu
static void print_val(int x){ printf("%d ", x); }

static void menu(void){
    puts("\nMENU");
    puts("1 Add front  2 Add back  3 Add at pos");
    puts("4 Rem front  5 Rem back  6 Rem at pos");
    puts("7 Search     8 Display   9 Traverse");
    puts("10 Size      11 IsEmpty  12 GetHead");
    puts("13 GetTail   14 Clear    0 Exit");
    printf("Choice: ");
}

int main(void){
    CList L; cl_init(&L);
    int op;
    while (1){
        menu();
        if (scanf("%d", &op) != 1) break;
        if (op == 0) break;
        int val, out; size_t pos;
        switch(op){
            case 1: printf("Val: "); scanf("%d",&val); cl_add_front(&L,val); break;
            case 2: printf("Val: "); scanf("%d",&val); cl_add_back(&L,val); break;
            case 3: printf("Pos: "); scanf("%zu",&pos); printf("Val: "); scanf("%d",&val);
                    if(!cl_add_at(&L,pos,val)) puts("pos invalida"); break;
            case 4: if(cl_remove_front(&L,&out)) printf("Removed %d\n",out); else puts("vacia"); break;
            case 5: if(cl_remove_back(&L,&out))  printf("Removed %d\n",out); else puts("vacia"); break;
            case 6: printf("Pos: "); scanf("%zu",&pos);
                    if(cl_remove_at(&L,pos,&out)) printf("Removed %d\n",out); else puts("pos invalida"); break;
            case 7: printf("Val: "); scanf("%d",&val); printf("Index: %d\n", cl_search(&L,val)); break;
            case 8: cl_display(&L); break;
            case 9: cl_traverse(&L, print_val); puts(""); break;
            case 10: printf("Size=%zu\n", cl_size(&L)); break;
            case 11: printf("Empty? %s\n", cl_is_empty(&L) ? "yes" : "no"); break;
            case 12: if(cl_get_head(&L,&out)) printf("Head=%d\n",out); else puts("vacia"); break;
            case 13: if(cl_get_tail(&L,&out)) printf("Tail=%d\n",out); else puts("vacia"); break;
            case 14: cl_clear(&L); puts("Cleared"); break;
            default: puts("opcion invalida");
        }
    }
    cl_clear(&L);
    return 0;
}