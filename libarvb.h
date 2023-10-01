#ifndef ARVB_H_INCLUDED
#define ARVB_H_INCLUDED

#include <stdio.h>

// ATENCAO: Qualquer alteração nas definições exige a compilação da biblioteca 

// Declarações
#define ORDEM 5
#define NULO -1
#define TAMBUFFER 500

typedef struct _cabecalho {
    int rrn_raiz;
} CABECALHO;

typedef struct _pagina {
    int num_chaves;
    int chaves[ORDEM-1];
    int filhos[ORDEM];
} PAGINA;

// possiveis retornos das funções relacionadas a insercao
enum {SEM_PROMOCAO, PROMOCAO, ERRO};

// possiveis retornos das funções relacionadas a busca
enum {NAO_ENCONTRADO, ENCONTRADO};

// ponteiro do arquivo btree.dat
extern FILE * arvb;

// Funções
int le_pagina(int rrn, PAGINA *pag);
/**
    Função que le uma pagina do arquivo arvb.
    
    Entradas:
    => int rrn = RRN da pagina a ser lida
    => PAGINA * pag = ponteiro para a página que armazenará a página lida 
    
    Saídas:
    => 0 = A página não foi lida
    => 1 = A página foi lida
**/

int escreve_pagina(int rrn, PAGINA *pag);
/**
    Função que escreve uma pagina do arquivo arvb.
    
    Entradas:
    => int rrn = RRN onde a pagina será escrita
    => PAGINA * pag = ponteiro para a página que será escrita 
    
    Saídas:
    => 0 = A página não foi escrita
    => 1 = A página foi escrita
**/

int novo_rrn();
/**
    Função que calcula o RRN de uma nova página do arquivo arvb.
       
    Saída:
    => Um número inteiro que corresponde ao RRN da uma nova página
**/

void inicializa_pagina(PAGINA *pag);
/**
    Função que inicializa uma pagina.
    Todas as posições dos campos chaves e filhos serão inicializadas com NULO (-1).
    O campo num_chaves será inicializado com zero.

    Entrada:
    => PAGINA * pag = ponteiro para a página que será inicializada 
**/

void insere_na_pagina(int chv_pro, int rrn_pro, int chaves[], int filhos[], int *num_chaves);
/**
    Função que insere uma chave e seu filho direito em uma página.
    
    Entradas:
    => int chv_pro = chave a ser inserida
    => int rrn_pro = filho direito da chave a ser inserida
    => int chaves[] = campo chaves da página
    => int filhos[] = campo filhos da página
    => int *num_chaves = ponteiro para o campo num_chaves
**/

void divide(int chave_i, int rrn_i, PAGINA *pag, int *chave_pro, int *filho_d_pro, PAGINA *novapag);
/**
    Função que insere uma chave e seu filho direito em uma página que precisará ser dividida.
    Cria uma nova página. 
    Distribui as chaves entre a página atual e a nova página.
    Determina qual chave promover (juntamente com seu filho direito).
    
    Entradas:
    => int chave_i = chave a ser inserida
    => int rrn_i = filho direito da chave a ser inserida
    => PAGINA *pag = página onde será feita a inserção
    => int *chave_pro = chave promovida após a divisao de pag
    => int *filho_d_pro = filho direito da chave promovida (rrn da nova página)
    => PAGINA *novapag = nova página criada pela divisão
**/

int insere(int rrn_atual, int chave, int *filho_d_pro, int *chave_pro);
/**
    Função que insere uma chave em arvb
    
    Entradas:
    => int rrn_atual = rrn da página raiz
    => int chave = chave a ser inserida
    => int *filho_d_pro = filho direito da chave promovida caso o retorno seja PROMOCAO
    => int *chave_pro = chave promovida caso o retorno seja PROMOCAO

    Saídas:
    => SEM_PROMOCAO = a chave foi inserida e não houve divisão da raiz
    => PROMOCAO = a chave foi inserida e houve divisão da raiz
    => ERRO = a chave não foi inserida por já estar na árvore-b
    
**/


int busca_na_pagina(int chave, PAGINA pag, int *pos);
/**
    Função que busca por uma chave em uma página
    
    Entradas:
    => int chave = chave a ser buscada
    => PAGINA pag = página onde a busca será realizada
    => int *pos = arg de retorno - posição em que a chave está (se estiver na página) ou deveria estar (se não estiver na página)

    Saídas:
    => NAO_ENCOTRADO = a chave não está na página
    => ENCONTRADO = a chave está na página 
    
**/

int busca(int rrn, int chave, int *rrn_encontrado, int *pos_encontrada);
/**
    Função que busca por uma chave em arvb
    
    Entradas:
    => int rrn = RRN da página raiz
    => int chave = chave a ser buscada
    => int *rrn_encontrado = arg de retorno - caso a chave seja encontrada, conterá o RRN da página em que ela está
    => int *pos_encontrada = arg de retorno - caso a chave seja encontrada, conterá a posição da chave na página de rrn_encontrada

    Saídas:
    => NAO_ENCOTRADO = a chave não foi encontrada
    => ENCONTRADO = a chave foi encontrada
    
**/

int imprime_pagina(int rrn);
/**
    Função que imprime o conteúdo de uma página de arvb
    
    Entradas:
    => int rrn = RRN da página a ser impressa

    Saídas:
    => 0 = a página de rrn não pode ser lida
    => 1 = impressao feita com sucesso
    
**/

int imprime_arvore();
/**
    Função que imprime o conteúdo de arvb

    Saídas:
    => 0 = o arquivo arvb não está aberto
    => 1 = impressao feita com sucesso
    
**/

int leia_chave(FILE * chaves);
/**
    Função que le uma chave (número inteiro) de um arquivo texto contendo uma chave por linha

    Saídas:
    => número inteiro correspondente à chave lida
    => -1 = não foi possível ler uma chave, possivelmente por ser o fim de arquivo
    
**/

#endif // ARVB_H_INCLUDED