/// proiect PC2
/// Sofronea Georgian, Grupa 163
/// problema 4 - arbore binar / arbore binar de cautare

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

struct binary_tree {
    int vf;
    struct binary_tree *left;
    struct binary_tree *right;
};

typedef struct binary_tree binary_tree;

FILE *file;

///preordine - RSD

int minimum(int a,int b) {
    return a<b ? a:b;
}

int maximum(int a, int b) {
    return a>b ? a:b;
}

bool CitirePreordine(binary_tree *root) {
    char s[6];
    fscanf(file,"%s",s);
    if(strcmp(s,"null") == 0 || strcmp(s, "NULL") == 0) {
        free(root);
        return 1;
    }
    root->vf = atoi(s);
    root->left = (binary_tree*) malloc(sizeof(binary_tree));
    root->right = (binary_tree*) malloc(sizeof(binary_tree));
    if(CitirePreordine(root->left) == 1)
        root->left = 0;
    if(CitirePreordine(root->right) == 1)
        root->right = 0;
    return 0;
}

/*void AfisarePreordine(binary_tree *root) {
    if(!root) {printf("null "); return;}
    printf("%d ",root->vf);
    AfisarePreordine(root->left);
    AfisarePreordine(root->right);
}*/

bool VerificareArboreDeCautare(binary_tree *root, int low, int high) {
    bool rezultat_stanga = true;
    bool rezultat_dreapta = true; 

    if(root->left) {/// pas la stanga 
        rezultat_stanga = root->left->vf < root->vf && VerificareArboreDeCautare(root->left, low, minimum(high,root->vf));
    }
    if(root->right) /// pas la dreapta
        rezultat_dreapta = root->right->vf > root->vf && VerificareArboreDeCautare(root->right, maximum(low,root->vf), high);

    bool rezultat_ordine = true;
    if(root->vf)
        rezultat_ordine = root->vf > low && root->vf < high;
    if(rezultat_ordine == false)
        printf("[node value]:%d [lower lim]: %d [higher lim]: %d\n", root->vf, low, high);
    return rezultat_stanga && rezultat_dreapta && rezultat_ordine;
}

void TreeDestructor(binary_tree *root) {
    if(root->left == 0 && root->right == 0) {
        free(root);
        return;
    }
    if(root->left)
        TreeDestructor(root->left);
    if(root->right)
        TreeDestructor(root->right);
    free(root);
}

int main() {
    binary_tree *tree;
    file = fopen("Date.in","r");
    tree = (binary_tree*) malloc(sizeof(binary_tree));
    CitirePreordine(tree);
    /// AfisarePreordine(tree);

    /// VERIFICARE ARBORE DE CAUTARE
    FILE *outf = fopen("Date.out","w");
    if(!tree) {
        fprintf(outf,"da\n"); /// arborele vid este BST
        return 0;
    }
    fprintf(outf,"%s",VerificareArboreDeCautare(tree,INT_MIN, INT_MAX) ? "da\n" : "nu\n");
    TreeDestructor(tree);
    return 0;
}