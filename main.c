/*
 * O NÓ DEVE TER:
 *   - Identificador único para arquivo
 *   - Nome do arquivo armazenado no nó
 *
 * FUNÇÕES ÁRVORE AVL:
 *   - Inicializar (C/ alocação dinâmica de memória)
 *   - Inserir -> FEITO
 *   - Remover
 *   - Buscar
 *   - Rotações (Simples e duplas)
 *   - Algoritimo de percurso (pré-ordem ou em-ordem ou pós-ordem)
 *
 * INTERFACE DEVE TER:
 *   - Armazenar arquivo
 *   - Buscar arquivo
 *   - Remover Arquivo
 *   - Mostrar toda a árvore de arquivos armazenados
 */


#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int valor;
    struct Node* esquerda;
    struct Node* direita;
}Nodo;

Nodo* criarNo(int valor) {
    Nodo* novoNo = (Nodo*)malloc(sizeof(Nodo));
    novoNo->valor = valor;
    novoNo->esquerda=NULL;
    novoNo->direita=NULL;

    return novoNo;
}

Nodo* inseir(Nodo* raiz, int valor) {
    Nodo* novoNo = criarNo(valor);
    if (raiz == NULL) {
        return novoNo;
    }

    Nodo *atual = raiz;
    Nodo *pai = NULL;

    while (atual != NULL) {
        pai = atual;
        if (valor < atual->valor) {
            atual = atual->esquerda;
        } else if (valor > atual->valor) {
            atual = atual->direita;
        } else {
            free(novoNo);
            return raiz;
        }
    }

    if (valor < pai->valor) {
        pai->esquerda = novoNo;
    } else {
        pai->direita = novoNo;
    }

    return raiz;
}

Nodo* encontrarMinimo(Nodo* no) {
    Nodo *atual = no;
    while (atual && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

int main() {
    printf("Hello, World!\n");
    return 0;
}