#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #define LIMPAR_TELA() system("cls")
#else
    #define LIMPAR_TELA() system("clear")
#endif

typedef struct Node {
    int valor;
    int altura;
    struct Node* esquerda;
    struct Node* direita;
} Nodo;

Nodo* criarNo(int valor) {
    Nodo* novoNo = (Nodo*)malloc(sizeof(Nodo));
    novoNo->valor = valor;
    novoNo->altura = 1;
    novoNo->esquerda = novoNo->direita = NULL;
    return novoNo;
}

int altura(Nodo* no) {
    if (no == NULL) return 0;
    return no->altura;
}

int maximo(int a, int b) {
    return (a > b) ? a : b;
}

void atualizarAltura(Nodo* no) {
    no->altura = 1 + maximo(altura(no->esquerda), altura(no->direita));
}

int fatorBalanceamento(Nodo* no) {
    if (no == NULL) return 0;
    return altura(no->esquerda) - altura(no->direita);
}

Nodo* rotacaoDireita(Nodo* y) {
    Nodo* x = y->esquerda;
    Nodo* T2 = x->direita;

    x->direita = y;
    y->esquerda = T2;

    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}

Nodo* rotacaoEsquerda(Nodo* x) {
    Nodo* y = x->direita;
    Nodo* T2 = y->esquerda;

    y->esquerda = x;
    x->direita = T2;

    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}

Nodo* inserirAVL(Nodo* raiz, int valor) {
    if (raiz == NULL) {
        return criarNo(valor);
    }

    if (valor < raiz->valor) {
        raiz->esquerda = inserirAVL(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = inserirAVL(raiz->direita, valor);
    } else {
        return raiz;
    }

    atualizarAltura(raiz);

    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && valor < raiz->esquerda->valor) {
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && valor > raiz->direita->valor) {
        return rotacaoEsquerda(raiz);
    }

    if (fb > 1 && valor > raiz->esquerda->valor) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && valor < raiz->direita->valor) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

Nodo* encontrarMinimo(Nodo* no) {
    Nodo* atual = no;
    while (atual && atual->esquerda != NULL)
        atual = atual->esquerda;
    return atual;
}

Nodo* removerAVL(Nodo* raiz, int valor) {
    if (raiz == NULL) return raiz;

    if (valor < raiz->valor) {
        raiz->esquerda = removerAVL(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = removerAVL(raiz->direita, valor);
    } else {
        if (raiz->esquerda == NULL || raiz->direita == NULL) {
            Nodo* filho = (raiz->esquerda) ? raiz->esquerda : raiz->direita;
            free(raiz);
            return filho;
        } else {
            Nodo* sucessor = encontrarMinimo(raiz->direita);
            raiz->valor = sucessor->valor;
            raiz->direita = removerAVL(raiz->direita, sucessor->valor);
        }
    }

    atualizarAltura(raiz);

    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esquerda) >= 0) {
        return rotacaoDireita(raiz);
    }

    if (fb > 1 && fatorBalanceamento(raiz->esquerda) < 0) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && fatorBalanceamento(raiz->direita) <= 0) {
        return rotacaoEsquerda(raiz);
    }

    if (fb < -1 && fatorBalanceamento(raiz->direita) > 0) {
        raiz->direita = rotacaoDireita(raiz->direita);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

Nodo* buscar(Nodo* raiz, int valor) {
    Nodo* atual = raiz;
    while (atual != NULL) {
        if (valor == atual->valor) {
            return atual;
        } else if (valor < atual->valor) {
            atual = atual->esquerda;
        } else {
            atual = atual->direita;
        }
    }
    return NULL;
}

void liberarArvore(Nodo* raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

void desenharArvore(Nodo* raiz, int espaco, char prefixo) {
    if (raiz == NULL) return;

    int novoEspaco = espaco + 8;

    desenharArvore(raiz->direita, novoEspaco, '/');

    printf("%*s", espaco, "");
    if (espaco > 0) {
        printf("%c-- ", prefixo);
    } else {
        printf("--- ");
    }
    printf("%d\n", raiz->valor);

    desenharArvore(raiz->esquerda, novoEspaco, '\\');
}

void exibirArvore(Nodo* raiz) {
    if (raiz == NULL) {
        printf("\n(Árvore vazia)\n\n");
        return;
    }
    printf("\n");
    desenharArvore(raiz, 0, ' ');
    printf("\n");
}

void emOrdem(Nodo* raiz) {
    if (raiz == NULL) return;
    emOrdem(raiz->esquerda);
    printf("%d ", raiz->valor);
    emOrdem(raiz->direita);
}

void pausar() {
    printf("\nPressione Enter para continuar...");
    while (getchar() != '\n');
    getchar();
}

void exibirMenu() {
    printf("========================================\n");
    printf("   Logitechi Gerenciamento de arquivos\n");
    printf("========================================\n");
    printf("1. Inserir valor\n");
    printf("2. Remover valor\n");
    printf("3. Buscar valor\n");
    printf("4. Exibir árvore (desenho)\n");
    printf("5. Exibir árvore em ordem\n");
    printf("0. Sair\n");
    printf("========================================\n");
    printf("Escolha uma opção: ");
}

int lerInteiro() {
    int valor;
    while (scanf("%d", &valor) != 1) {
        printf("Entrada inválida. Digite um número inteiro: ");
        while (getchar() != '\n');
    }
    return valor;
}

int main() {
    Nodo* raiz = NULL;
    int opcao, valor;
    Nodo* encontrado;

    do {
        LIMPAR_TELA();
        exibirMenu();
        opcao = lerInteiro();

        switch (opcao) {
            case 1:
                printf("Digite o valor a inserir: ");
                valor = lerInteiro();
                raiz = inserirAVL(raiz, valor);
                LIMPAR_TELA();
                printf("Valor %d inserido com sucesso!\n", valor);
                exibirArvore(raiz);
                pausar();
                break;

            case 2:
                printf("Digite o valor a remover: ");
                valor = lerInteiro();
                LIMPAR_TELA();
                if (buscar(raiz, valor) == NULL) {
                    printf("Valor %d não encontrado na árvore.\n", valor);
                } else {
                    raiz = removerAVL(raiz, valor);
                    printf("Valor %d removido com sucesso!\n", valor);
                    exibirArvore(raiz);
                }
                pausar();
                break;

            case 3:
                printf("Digite o valor a buscar: ");
                valor = lerInteiro();
                LIMPAR_TELA();
                encontrado = buscar(raiz, valor);
                if (encontrado != NULL) {
                    printf("Valor %d está presente na árvore!\n", valor);
                } else {
                    printf("Valor %d não está presente na árvore.\n", valor);
                }
                pausar();
                break;

            case 4:
                LIMPAR_TELA();
                printf("Estrutura atual da árvore:\nPS: Tá lindona! :D\n");
                exibirArvore(raiz);
                pausar();
                break;

            case 5:
                LIMPAR_TELA();
                printf("Caminho em ordem crescente: ");
                emOrdem(raiz);
                printf("\n\n");
                pausar();
                break;

            case 0:
                LIMPAR_TELA();
                printf("Programa encerrado...\n");
                break;

            default:
                printf("Opção inválida!\n");
                pausar();
        }

    } while (opcao != 0);

    liberarArvore(raiz);
    return 0;
}