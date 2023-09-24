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
void inicializar_pagina(pagina* nova_pagina);
void inserir_elemento_em_pagina(registro chave_inserida, int rrn_filho_dir_chave, registro chaves[], int filhos[], int* num_chaves);
void divide_pagina(registro chave_inserida, int rrn_inserido, pagina *pagina_original, registro* chave_promovida, int* filho_direito_promovido, pagina* nova_pagina_gerada);
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

    // faça TAMANHOPAG receber o tamanho em bytes de uma página
    // faça TAMANHOCAB receber o tamanho em bytes do cabeçalho
    // faça BYTEOFFSET receber o byte-offset do fim do arquivo
    // retorne (BYTEOFFSET – TAMANHOCAB)/TAMANHOPAG
    return 0;
}

void inicializar_pagina(pagina* nova_pagina)
{
    /*
        Inicializa os valores de uma nova página criada
        Entrada:
            pagina* nova_pagina: Um ponteiro para a variável página a ser inicializada
    */

    // PAG.NUM_CHAVES = 0
    // para i de 0 até ORDEM-1 faça
    // PAG.CHAVES[i] = NULO
    // PAG.FILHOS[i] = NULO
    // PAG.FILHOS[i] = NULO
    // fim FUNÇÃO
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

    // faça i receber NUM_CHAVES
    // enquanto i > 0 e CHAVE < CHAVES[i-1] faça
    // CHAVES[i] = CHAVES[i-1];
    // FILHOS[i+1] = FILHOS[i];
    // decremente i
    // fim enquanto
    // incremente NUM_CHAVES
    // faça CHAVES[i] receber CHAVE
    // faça FILHOS[i+1] receber FILHO_D
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

    // copie PAG para PAGAUX
    // insira CHAVE e FILHO_D na PAGAUX
    // faça MEIO receber ORDEM/2
    // faça CHAVE_PRO receber PAGAUX.CHAVES[MEIO]
    // faça FILHO_D_PRO receber o RRN que o NOVAPAG terá no arquivo
    // árvore-b
    // /* Copie as chaves e ponteiros que vêm antes de CHAVE_PRO
    // para PAG */
    // inicialize PAG
    // faça i receber 0
    // enquanto i < MEIO faça
    // PAG.CHAVES[i] = PAGAUX.CHAVES[i]
    // PAG.FILHOS[i] = PAGAUX.FILHOS[i]
    // incremente PAG.NUM_CHAVES
    // incremente i
    // fim enquanto
    // PAG.FILHOS[i] = PAGAUX.FILHOS[i]
    // /* Copie as chaves e ponteiros que vêm depois de CHAVE_PRO para
    // NOVAPAG */
    // inicialize NOVAPAG
    // faça i receber MEIO + 1
    // enquanto i < ORDEM faça
    // NOVAPAG.CHAVES[NOVAPAG.NUM_CHAVES] = PAGAUX.CHAVES[i]
    // NOVAPAG.FILHOS[NOVAPAG.NUM_CHAVES] = PAGAUX.FILHOS[i]
    // incremente NOVAPAG.NUM_CHAVES
    // incremente i
    // fim enquanto
    // NOVAPAG.FILHOS[NOVAPAG.NUM_CHAVES] = PAGAUX.FILHOS[i]
    // fim FUNÇÃO
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

    pagina nova_pagina;
    inicializar_pagina(&nova_pagina);

    divide_pagina(chave_pro, rrn_pro, &pagina_atual, chave_promovida, filho_direito_promovido, &nova_pagina);
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

    // faça i receber 0
    // enquanto i < PAG.NUM_CHAVES e CHAVE > PAG.CHAVE[i] faça
    // incremente i
    // faça POS receber i
    // se POS < PAG.NUM_CHAVES e CHAVE é igual a PAG.CHAVE[POS] então
    // retorne ENCONTRADO
    // senão
    // retorne NAO_ENCONTRADO
    // fim FUNÇÃO
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

    // se RRN == NULO então /* condição de parada */
    // retorne NAO_ENCONTRADO
    // senão
    // leia a página armazenada no RRN para PAG
    // ACHOU = busca_na_pagina(CHAVE, PAG, POS)
    // /* POS recebe a posição em que CHAVE ocorre
    // ou deveria ocorrer se estivesse em PAG */
    // se ACHOU então
    // RRN_ENCONTRADO := RRN /* RRN da página que contém a chave */
    // POS_ENCONTRADO := POS /* posição da chave na página*/
    // retorne ENCONTRADO
    // senão /* siga o ponteiro para a próxima página da busca */
    // retorne(busca(PAG.FILHOS[POS], CHAVE, RRN_ENCONTRADO, POS_ENCONTRADA))
    // fim se
    // fim se
    // fim FUNÇÃO
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