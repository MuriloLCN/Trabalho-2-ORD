/*
    Segundo trabalho avaliativo
    Disciplina 6897 - Organização e recuperação de dados
    Professora: Valeria Delisandra Feltrim
    Alunos:
        Leonardo Venâncio Correia (RA: 129266)
        Murilo Luis Calvo Neves (RA: 129037)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM_ARVORE 5
#define NULO -1
#define TAMANHO_MAXIMO_BUFFER 500
#define TAMANHO_CABECALHO sizeof(int)

typedef struct registro {
    char identificador[64];
    int byte_offset;
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

// Essas funções foram baseadas na biblioteca utilizada em sala, vistas nos slides e algumas foram recicladas do primeiro trabalho
int gerar_novo_rrn(FILE* arvore_b);
void inicializar_pagina(pagina* nova_pagina, registro* registro_vazio);
void inserir_elemento_em_pagina(registro chave_inserida, int rrn_filho_dir_chave, registro chaves[], int filhos[], int* num_chaves);
void divide_pagina(registro chave_inserida, int rrn_inserido, pagina *pagina_original, registro* chave_promovida, 
                   int* filho_direito_promovido, pagina* nova_pagina_gerada, FILE* arvore_b);
void modulo_criar_indice();
void modulo_imprimir_arvore_b();
void modulo_realizar_operacoes();
void modulo_insercao();
void modulo_busca();
void ler_identificador_registro(char *registro, char *nome);
status_insercao insere_chave(int rrn_atual, registro chave, int* filho_direito_promovido, registro* chave_promovida, FILE* arvore_b);
status_operacao busca_chave_em_pagina(registro chave, pagina pagina_alvo, int* posicao_encontrada);
status_operacao busca_chave(int rrn, registro chave, int* rrn_encontrado, int* posicao_encontrada, FILE* arvore_b);
status_operacao imprime_pagina(int rrn_alvo, FILE* arvore_b);
status_operacao imprime_arvore(FILE* arvore_b);
status_operacao le_pagina(int rrn, pagina* pagina_de_destino, FILE* arvore_b);
status_operacao escreve_pagina(int rrn, pagina* pagina_de_origem, FILE* arvore_b);

void libera_memoria_pagina(pagina* pagina_finalizada);

int converte_rrn_para_offset(int rrn)
{
    /*
        Converte um valor de rrn para o byte offset onde a página está armazenada no arquivo arvb
        Entradas:
            int rrn: O RRN a ser convertido
    */
    return (rrn * sizeof(pagina)) + TAMANHO_CABECALHO;
}

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
    int offset = converte_rrn_para_offset(rrn);
    fseek(arvore_b, offset, SEEK_SET);
    int lido = fread(pagina_de_destino, sizeof(pagina), 1, arvore_b);

    if (lido == 1)
    {
        return SUCESSO;
    }

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
    int offset = converte_rrn_para_offset(rrn);
    fseek(arvore_b, offset, SEEK_SET);
    int escrito = fwrite(pagina_de_origem, sizeof(pagina), 1, arvore_b);

    if (escrito == 1)
    {
        return SUCESSO;
    }
    
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
    
    int tamanho_pagina = sizeof(pagina);
    int tamanho_cabecalho = sizeof(int);

    fseek(arvore_b, 0, SEEK_END);
    int offset = ftell(arvore_b);

    return (offset - tamanho_cabecalho) / tamanho_pagina;
}

void inicializar_pagina(pagina* nova_pagina, registro* registro_vazio)
{
    /*
        Inicializa os valores de uma nova página criada
        Entrada:
            pagina* nova_pagina: Um ponteiro para a variável página a ser inicializada
    */
    
    nova_pagina->num_chaves = 0;

    for (int i = 0; i < ORDEM_ARVORE - 1; i++)
    {
        nova_pagina->chaves[i] = *registro_vazio;
        nova_pagina->filhos[i] = -1;
    }
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

    int i = *num_chaves;

    while (i > 0 && strcmp(chave_inserida.identificador, chaves[i].identificador) < 0)
    {
        chaves[i] = chaves[i-1];
        filhos[i+1] = filhos[i];
        i--;
    }

    *num_chaves = *num_chaves + 1;
    chaves[i] = chave_inserida;
    filhos[i+1] = rrn_filho_dir_chave;
}

void copiar_pagina(pagina* pagina_origem, pagina* pagina_destino)
{
    /*
        Copia os dados de uma página de origem para uma página de destino
        Entradas:
            pagina* pagina_origem: A página de origem dos dados
            pagina* pagina_destinho: A página de destino dos dados
    */
}

void divide_pagina(registro chave_inserida, int rrn_inserido, pagina *pagina_original, registro* chave_promovida,
                   int* filho_direito_promovido, pagina* nova_pagina_gerada, FILE* arvore_b)
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
    pagina pagina_auxiliar;
    copiar_pagina(pagina_original, &pagina_auxiliar);
    inserir_elemento_em_pagina(chave_inserida, rrn_inserido, pagina_original->chaves, pagina_original->filhos, pagina_original->num_chaves);

    int meio = ORDEM_ARVORE / 2;

    *chave_promovida = pagina_auxiliar.chaves[meio];
    *filho_direito_promovido = gerar_novo_rrn(arvore_b);

    registro registro_vazio;
    registro_vazio.byte_offset = -1;
    strcpy(registro_vazio.identificador, "\0");

    inicializar_pagina(pagina_original, &registro_vazio);
    
    int i = 0;
    while (i < meio)
    {
        pagina_original->chaves[i] = pagina_auxiliar.chaves[i];
        pagina_original->filhos[i] = pagina_auxiliar.filhos[i];
        pagina_original->num_chaves = pagina_original->num_chaves + 1;
        i += 1;
    }

    pagina_original->filhos[i] = pagina_auxiliar.filhos[i];

    inicializar_pagina(nova_pagina_gerada, &registro_vazio);
    
    int i = meio + 1;
    while (i < ORDEM_ARVORE)
    {
        nova_pagina_gerada->chaves[nova_pagina_gerada->num_chaves] = pagina_auxiliar.chaves[i];
        nova_pagina_gerada->filhos[nova_pagina_gerada->num_chaves] = pagina_auxiliar.filhos[i];
        nova_pagina_gerada->num_chaves = nova_pagina_gerada->num_chaves + 1;
        i += 1;
    }

    nova_pagina_gerada->filhos[nova_pagina_gerada->num_chaves] = pagina_auxiliar.filhos[i];
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

    if (rrn_atual == NULO)
    {
        *chave_promovida = chave;
        *filho_direito_promovido = NULO;
        return PROMOCAO;
    }
    
    pagina pagina_atual;

    le_pagina(rrn_atual, &pagina_atual, arvore_b);
    
    int posicao_encontrada;
    status_operacao achou = busca_chave_em_pagina(chave, pagina_atual, &posicao_encontrada);
    
    if (achou == SUCESSO)
    {
        printf("\nErro: Chave duplicada!");
        return ERRO;
    }

    int rrn_pro;
    registro chave_pro;

    status_operacao retorno = insere_chave(pagina_atual.filhos[posicao_encontrada], chave, &rrn_pro, &chave_pro, arvore_b);

    if (retorno == SEM_PROMOCAO || retorno == ERRO)
    {
        return retorno;
    }

    if (pagina_atual.num_chaves < ORDEM_ARVORE - 1)
    {
        inserir_elemento_em_pagina(chave_pro, rrn_pro, pagina_atual.chaves, pagina_atual.filhos, &(pagina_atual.num_chaves));
        escreve_pagina(rrn_atual, &pagina_atual, arvore_b);
        return SEM_PROMOCAO;       
    }

    registro registro_vazio;
    registro_vazio.byte_offset = -1;
    strcpy(registro_vazio.identificador, "\0");

    pagina nova_pagina;
    inicializar_pagina(&nova_pagina, &registro_vazio);

    divide_pagina(chave_pro, rrn_pro, &pagina_atual, chave_promovida, filho_direito_promovido, &nova_pagina, arvore_b);
    escreve_pagina(rrn_atual, &pagina_atual, arvore_b);
    escreve_pagina(*filho_direito_promovido, &nova_pagina, arvore_b);

    return PROMOCAO;
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
    
    int i = 0;
    while (i < pagina_alvo.num_chaves && strcmp(chave.identificador, pagina_alvo.chaves[i].identificador) > 0)
    {
        i += 1;
        *posicao_encontrada = i;
        
    }

    if (*posicao_encontrada < pagina_alvo.num_chaves && strcmp(chave.identificador, pagina_alvo.chaves[i].identificador) == 0)
    {
        return SUCESSO;
    }
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

    if (rrn == -1)
    {
        return FALHA;
    }

    pagina pagina_atual;

    le_pagina(rrn, &pagina_atual, arvore_b);
    status_operacao achou = busca_chave_em_pagina(chave, pagina_atual, posicao_encontrada);
    
    if (achou == SUCESSO)
    {
        *rrn_encontrado = rrn;
        // pOS_ENCONTRADO := POS  ??
        return SUCESSO;
    }

    return busca_chave(pagina_atual.filhos[*posicao_encontrada], chave, rrn_encontrado, posicao_encontrada, arvore_b);
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

// 
void modulo_criar_indice(FILE* arvore_b)
{
    /*
        Procedimento que monta o índice no arquivo arvb vazio
        Entrada:
            FILE* arvore_b: Descritor do arquivo vazio para a criação do índice
    */
}

void modulo_imprimir_arvore_b(FILE* arvore_b)
{
    /*
        Procedimento para imprimir as páginas do arquivo arvb na saída padrão
        Entrada:
            FILE* arvore_b: O arquivo contendo a estrutura da árvore a ser impressa
    */
}

// 
void modulo_realizar_operacoes(FILE* arvore_b)
{
    /*
        Procedimento que abre o arquivo de operações e realiza as operações descritas nela, chamando modulo_insercao() ou modulo_busca()
        conforme necessário.

    */
}

void modulo_insercao(FILE* arvore_b, registro chave)
{
    /*
        Realiza a operação de inserção no arquivo arvb e imprime os resultados na saída padrão
        Entradas:
            FILE* arvore_b: O descritor do arquivo para se realizar a busca
            registro chave: O registro a ser inserido na árvore
    */
}

void modulo_busca(FILE* arvore_b, registro chave)
{
    /*
        Realiza a operação de busca no arquivo arvb e imprime os resultados na saída padrão
        Entradas:
            FILE* arvore_b: O descritor do arquivo para se realizar a busca
            registro chave: O registro a ser procurado na árvore
    */
}

void ler_identificador_registro(char *registro, char *nome)
{
    /*
    Recupera a string que serve como identificador de um registro

    Parâmetros:
        char* registro: String contendo o registro a ser lido
        char* nome: String que irá receber o identificador do registro
    */

    if (registro[0] == '*')
    {
        nome[0] = '\0';
    }

    int i = 0;
    while (registro[i] != '|' && registro[i] != '\0')
    {
        nome[i] = registro[i];
        i++;
    }
    nome[i] = '\0';
}

// 
int main()
{
    /*
        Ponto principal de entrada do programa
    */
    return 0;   
}


/*
void gerenciador(int* raiz)
{
    FILE* chaves = fopen("chaves.txt", "r");
    if (chaves == NULL)
    {
        printf("\nArquivo de chaves não encontrado");
        return;
    }
    int chave = leia_chave(chaves);
    int filho_d_pro = -1;
    int chave_pro = -1;

    while (chave != -1)
    {
        printf("\nInserindo chave: %d", chave);
        
        if (insere_2(*raiz, chave, &filho_d_pro, &chave_pro) == PROMOCAO)
        {
            printf("\nPrecisou de uma promoção na raiz");
            PAGINA nova_pagina;
            inicializa_pagina(&nova_pagina);
            nova_pagina.chaves[0] = chave_pro;
            nova_pagina.filhos[0] = *raiz;
            nova_pagina.filhos[1] = filho_d_pro;
            nova_pagina.num_chaves += 1;
            int rrn = novo_rrn();
            escreve_pagina(rrn, &nova_pagina);
            *raiz = rrn;
        }

        chave = leia_chave(chaves);
    }

    fclose(chaves);
    fseek(arvb, 0, SEEK_SET);
    fwrite(raiz, sizeof(int), 1, arvb);
}

int main()
*/