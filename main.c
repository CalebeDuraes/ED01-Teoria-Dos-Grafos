#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
    #define LIMPAR_TELA() system("cls")
#else
    #define LIMPAR_TELA() system("clear")
#endif

unsigned long hash(const char *str) {
    unsigned long h = 5381;
    int c;
    while ((c = *str++)) {
        h = ((h << 5) + h) + c;
    }
    return h;
}

typedef struct {
    int identificador;
    int tamanho;
    char nome[50];
}arquivo;

typedef struct Node {
    arquivo* valor;
    int altura;
    struct Node* esquerda;
    struct Node* direita;
} Nodo;

Nodo* criarNo(arquivo* arc) {
    Nodo* novoNo = (Nodo*)malloc(sizeof(Nodo));
    novoNo->valor = arc;
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

Nodo* inserirAVL(Nodo* raiz, arquivo* arc) {
    if (raiz == NULL) {
        return criarNo(arc);
    }

    if (arc->identificador < raiz->valor->identificador) {
        raiz->esquerda = inserirAVL(raiz->esquerda, arc);
    } else if (arc->identificador > raiz->valor->identificador) {
        raiz->direita = inserirAVL(raiz->direita, arc);
    } else {
        return NULL;
    }

    atualizarAltura(raiz);

    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && arc->identificador < raiz->esquerda->valor->identificador) {
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && arc->identificador > raiz->direita->valor->identificador) {
        return rotacaoEsquerda(raiz);
    }

    if (fb > 1 && arc->identificador > raiz->esquerda->valor->identificador) {
        raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
        return rotacaoDireita(raiz);
    }

    if (fb < -1 && arc->identificador < raiz->direita->valor->identificador) {
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

Nodo* removerAVL(Nodo* raiz, int identificador) {
    if (raiz == NULL) return raiz;

    if (identificador < raiz->valor->identificador) {
        raiz->esquerda = removerAVL(raiz->esquerda, identificador);
    } else if (identificador > raiz->valor->identificador) {
        raiz->direita = removerAVL(raiz->direita, identificador);
    } else {
        if (raiz->esquerda == NULL || raiz->direita == NULL) {
            Nodo* filho = (raiz->esquerda) ? raiz->esquerda : raiz->direita;
            free(raiz);
            return filho;
        } else {
            Nodo* sucessor = encontrarMinimo(raiz->direita);
            raiz->valor = sucessor->valor;
            raiz->direita = removerAVL(raiz->direita, sucessor->valor->identificador);
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

Nodo* buscar(Nodo* raiz, int identificador) {
    Nodo* atual = raiz;
    while (atual != NULL) {
        if (identificador == atual->valor->identificador) {
            return atual;
        } else if (identificador < atual->valor->identificador) {
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
    free(raiz->valor);
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
    printf("%s\n", raiz->valor->nome);

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
    printf("%s (ID: %d)\n", raiz->valor->nome, raiz->valor->identificador);
    emOrdem(raiz->direita);
}

void pausar() {
    printf("\nPressione Enter para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}


void exibirMenu() {
    printf("========================================\n");
    printf("   Logitechi Gerenciamento de arquivos\n");
    printf("========================================\n");
    printf("1. Inserir arquivo\n");
    printf("2. Remover arquivo\n");
    printf("3. Buscar arquivo\n");
    printf("4. Exibir arvore (desenho)\n");
    printf("5. Exibir arvore em ordem\n");
    printf("0. Sair\n");
    printf("========================================\n");
    printf("Escolha uma opcao: ");
}

int lerInteiro() {
    int valor;
    while (scanf("%d", &valor) != 1) {
        printf("Entrada inválida. Digite um número inteiro: ");
        while (getchar() != '\n');
    }
    return valor;
}

arquivo* lerArquivo(arquivo* arc) {
    char *extensao;

    int correto = 0;
    while (!correto) {
        printf("Digite o nome do arquivo (PDF ou DOCX): ");
        scanf("%s", arc->nome);

        extensao = strrchr(arc->nome, '.');
        if (extensao != NULL) {
            extensao++;

            if (strcmp(extensao, "pdf") == 0) {
                arc->tamanho = 40 + rand() % (4000 - 40 + 1);
                correto = 1;
            } else if (strcmp(extensao, "docx") == 0) {
                arc->tamanho = 20 + rand() % (2000 - 20 + 1);
                correto = 1;
            } else {
                printf("Os arquivos precisam ser PDF ou DOCX! (Ex: 'nome_arquivo.pdf').\n");
            }
        } else {
            printf("Digite tambem a extensao do arquivo! (Ex: 'nome_arquivo.pdf').\n");
        }

    }

    arc->identificador = (int)hash(arc->nome);

    return arc;
}

int main() {
    Nodo* raiz = NULL;
    Nodo* encontrado;
    int opcao, identificador;
    char excluir[50];
    char busca[50];
    srand(time(NULL));

    do {
        LIMPAR_TELA();
        exibirMenu();
        opcao = lerInteiro();

        switch (opcao) {
            case 1:
                arquivo* arc = (arquivo*)malloc(sizeof(arquivo));
                arc = lerArquivo(arc);
                Nodo* novo = inserirAVL(raiz, arc);
                LIMPAR_TELA();
                if (novo != NULL) {
                    raiz = novo;
                    printf("%s inserido com sucesso! (ID: %d | Tamanho: %dKB)\n", arc->nome, arc->identificador, arc->tamanho);
                } else {
                    printf("Arquivo ja existe!\n");
                }
                exibirArvore(raiz);
                pausar();
                break;

            case 2:
                LIMPAR_TELA();
                printf("Estrutura atual da arvore:\n");
                exibirArvore(raiz);
                printf("Digite o nome do arquivo a remover: ");
                scanf("%s", excluir);
                identificador = (int)hash(excluir);
                LIMPAR_TELA();
                if (buscar(raiz, identificador) == NULL) {
                    printf("Identificador %d não encontrado na arvore.\n", identificador);
                } else {
                    raiz = removerAVL(raiz, identificador);
                    printf("Identificador %d removido com sucesso!\n", identificador);
                    exibirArvore(raiz);
                }
                pausar();
                break;

            case 3:
                printf("Digite o nome do arquivo a buscar: ");
                scanf("%s", busca);
                identificador = (int)hash(busca);
                LIMPAR_TELA();
                encontrado = buscar(raiz, identificador);
                if (encontrado != NULL) {
                    printf("Arquivo \"%s\" está presente na árvore! (ID: %d | Tamanho: %dKB)\n", busca, identificador, encontrado->valor->tamanho);
                } else {
                    printf("Arquivo \"%s\" não está presente na árvore.\n", busca);
                }
                pausar();
                break;

            case 4:
                LIMPAR_TELA();
                printf("Estrutura atual da arvore:\n");
                exibirArvore(raiz);
                pausar();
                break;

            case 5:
                LIMPAR_TELA();
                printf("Caminho em ordem crescente:\n");
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