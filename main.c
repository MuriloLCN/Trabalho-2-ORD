/*
    Segundo trabalho avaliativo
    Disciplina 6897 - Organização e recuperação de dados
    Professora: Valeria Delisandra Feltrim
    Alunos:
        Leonardo Venâncio Correia (RA: )  // <------  A COLOCAR
        Murilo Luis Calvo Neves (RA: 129037)
*/

#include <stdio.h>
#include <stdlib.h>

#define ORDEM_ARVORE 5
#define NULO -1
#define TAMANHO_MAXIMO_BUFFER 500

typedef struct registro {
    char* identificador;
    int* byte_offset;
} registro;

typedef struct pagina {
    int num_chaves;
    registro chaves[ORDEM_ARVORE - 1];
    offsets[ORDEM_ARVORE - 1];
    int filhos[ORDEM_ARVORE];
} pagina;

// typedef enum {False, True} booleano;

typedef enum {SEM_PROMOCAO, PROMOCAO, ERRO} status_insercao;

typedef enum {SUCESSO, FALHA} status_operacao;

// Cabeçalho de todas as funções para não existir problemas de ordem

// Essas funções foram baseadas na biblioteca utilizada em sala e vistas nos slides
int gerar_novo_rrn(FILE* arvore_b);
void inicializar_pagina(pagina* nova_pagina);
void inserir_elemento_em_pagina(registro chave_inserida, int rrn_filho_dir_chave, registro chaves[], int filhos[], int* num_chaves);
void divide_pagina(registro chave_inserida, int rrn_inserido, pagina *pagina_original, registro* chave_promovida, int* filho_direito_promovido, pagina* nova_pagina_gerada);
status_insercao insere_chave(int rrn_atual, registro chave, int* filho_direito_promovido, registro* chave_promovida, FILE* arvore_b);
status_operacao busca_chave_em_pagina(registro chave, pagina pagina_alvo, int* posicao_encontrada);
status_operacao busca_chave(int rrn, registro chave, int* rrn_encontrado, int* posicao_encontrada, FILE* arvore_b);
status_operacao imprime_pagina(int rrn_alvo, FILE* arvore_b);
status_operacao imprime_arvore(FILE* arvore_b);
status_operacao le_pagina(int rrn, pagina* pagina_de_destino, FILE* arvore_b);
status_operacao escreve_pagina(int rrn, pagina* pagina_de_origem, FILE* arvore_b);


status_operacao le_pagina(int rrn, pagina* pagina_de_destino, FILE* arvore_b)
{   
    /*
        Lê uma página armazenada no arquivo arvb para a memória
        Entradas:
            int rrn: RRN da página a ser lida
            pagina* pagina_de_destino: Variável página passada por referência para receber os dados lidos
            FILE* arvore_b: Descritor do arquivo contendo a árvore B
        Retorno:
            FALHA se a página não pode ser lida
            SUCESSO se a página foi lida com sucesso
    */
    return FALHA;
}

status_operacao escreve_pagina(int rrn, pagina* pagina_de_origem, FILE* arvore_b)
{
    /*
        Escreve uma página da memória no arquivo arvb
        Entradas:
            int rrn: RRN da página a ser escrita
            pagina* pagina_de_destino: Ponteiro para a página a ser escrita
            FILE* arvore_b: Descritor do arquivo contendo a árvore B
        Retorno:
            FALHA se a página não pode ser escrita
            SUCESSO se a página foi escrita com sucesso
    */
    return FALHA;
}

int gerar_novo_rrn(FILE* arvore_b)
{
    /*
        Gera um novo valor de RRN para novas páginas do arquivo arvb
        Entradas:
            FILE* arvore_b: Descritor do arquivo contendo a árvore B
        Retorno:
            O valor de RRN gerado
    */
    return 0;
}

void inicializar_pagina(pagina* nova_pagina)
{
    /*
        Inicializa os valores de uma nova página criada
        Entrada:
            pagina* nova_pagina: Um ponteiro para a variável página a ser inicializada
    */
}

void inserir_elemento_em_pagina(registro chave_inserida, int rrn_filho_dir_chave, registro chaves[], int filhos[], int* num_chaves)
{
    /*
        Insere uma chave e seu filho à direita em uma página
        Entradas:
            registro chave_inserida: Chave a ser inserida na página
            int rrn_filho_dir_chave: RRN do filho direito da chave a ser inserida
            registro chaves[]: Campo vetor de chaves da página 
            int filhos[]: Campo vetor de filhos da página
            int* num_chaves: Campo número de chaves da página
    */
}

void divide_pagina(registro chave_inserida, int rrn_inserido, pagina *pagina_original, registro* chave_promovida,
                   int* filho_direito_promovido, pagina* nova_pagina_gerada)
{
    /*
        Insere uma chave em uma página que precisa ser dividida, cria uma nova página, realiza a distribuição e promoção de chaves
        Entradas:
            registro chave_inserida: A chave a ser inserida na página
            int rrn_inserido: O RRN do filho direito da chave inserida
            pagina* pagina_original: A página onde se realiza a inserção
            registro* chave_promovida: Variável para receber a chave a ser promovida após a divisão
            int* filho_direito_promovido: Variável para receber o filho direito da chave promovida (RRN da nova página)
            pagina* nova_pagina: Variável para receber a nova página gerada
    */
}

status_insercao insere_chave(int rrn_atual, registro chave, int* filho_direito_promovido, registro* chave_promovida, FILE* arvore_b)
{
    /*
        Insere uma chave na árvore B
        Entradas:
            int rrn_atual: RRN da página raiz da árvore
            registro chave: Chave a ser inserida na árvore
            int* filho_direito_promovido: Variável para receber o filho direito da chave promovida (caso status seja PROMOCAO)
            registro* chave_promovida: Variável para receber a chave promovida (caso o status seja PROMOCAO)
            FILE* arvore_b: Descritor do arquivo com a árvore B
        Retorno:
            Status SEM_PROMOCAO caso a chave seja inserida e não seja necessária promoções
            Status PROMOCAO caso a chave seja inserida e exija promoçõe
            Status ERRO caso a chave não possa ser inserida
    */
    return ERRO;
}

status_operacao busca_chave_em_pagina(registro chave, pagina pagina_alvo, int* posicao_encontrada)
{
    /*
        Realiza a busca por uma chave em uma página
        Entradas:
            registro chave: A chave a ser procurada
            pagina pagina_alvo: A página na qual será procurada a chave
            int* posicao_encontrada: Variável para receber a posição achada, caso não ache retorna a posição em que se esperaria achá-la
        Retorno:
            Status FALHA caso a chave não pertença à página
            Status SUCESSO caso a chave esteja na página
    */
    return FALHA;
}

status_operacao busca_chave(int rrn, registro chave, int* rrn_encontrado, int* posicao_encontrada, FILE* arvore_b)
{
    /*
        Busca por uma chave no arquivo árvore B
        Entradas:
            int rrn: RRN da raiz da árvore 
            registro chave: Chave a qual se irá buscar na árvore
            int* rrn_encontrado: Variável para receber o RRN da página que contém a chave (caso ela seja encontrada)
            int* posicao_encontrada: Variável para receber a posição da chave no vetor da página (caso ela seja encontrada)
            FILE* arvore_b: Descritor do arquivo contendo a árvore B
        Retorno:
            Status SUCESSO caso a chave seja encontrada no arquivo
            Status FALHA caso a chave não seja encontrada no arquivo
    */
}

status_operacao imprime_pagina(int rrn_alvo, FILE* arvore_b)
{
    /*
        Imprime o conteúdo de uma página do arquivo arvb na saída padrão
        Entradas:
            int rrn_alvo: O RRN da página que deverá ser impressa
            FILE* arvore_b: O descritor do arquivo contendo a árvore B
        Retorno:
            Status SUCESSO caso a impressão seja feita com sucesso
            Status FALHA caso a página não possa ser lida (RRN inválido)
    */
    return FALHA;
}

status_operacao imprime_arvore(FILE* arvore_b)
{
    /*
        Imprime o conteúdo do arquivo arvb
        Entrada:
            FILE* arvore_b: O descritor do arquivo contendo a árvore B
        Retorno:
            Status FALHA se a árvore não possa ser impressa (arquivo não aberto)
            Status SUCESSO se a árvore foi impressa com sucesso
    */
}

