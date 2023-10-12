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

#define NOME_ARQUIVO_DADOS "dados.dat"
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
    //offsets[ORDEM_ARVORE - 1];
    int filhos[ORDEM_ARVORE];
} pagina;

// typedef enum {False, True} booleano;

typedef enum {SEM_PROMOCAO, PROMOCAO, ERRO} status_insercao;

typedef enum {SUCESSO, FALHA} status_operacao;

// Protótipos
status_operacao busca_chave_em_pagina(registro chave, pagina pagina_alvo, int* posicao_encontrada);
int rrn_raiz(FILE* arvore_b);
void ler_identificador_registro(char *registro, char *nome);
void modulo_insercao(FILE* arvore_b, FILE* arquivo_de_dados, char* registro_em_string);
void modulo_busca(FILE* arvore_b, char* identificador_registro, FILE* arquivo_de_dados);


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


void inicializar_pagina(pagina* nova_pagina)
{
    /*
        Inicializa os valores de uma nova página criada
        Entrada:
            pagina* nova_pagina: Um ponteiro para a variável página a ser inicializada
    */
    
    nova_pagina->num_chaves = 0;

    for (int i = 0; i < ORDEM_ARVORE - 1; i++)
    {
        nova_pagina->chaves[i].byte_offset = -1;
        strcpy(nova_pagina->chaves[i].identificador, "\0");
    }

    for (int j = 0; j < ORDEM_ARVORE; j++)
    {
        nova_pagina->filhos[j] = -1;
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
    pagina_destino->num_chaves = pagina_origem->num_chaves;
    
    for (int i = 0; i < pagina_destino->num_chaves; i++)
    {
        pagina_destino->chaves[i].byte_offset = pagina_origem->chaves[i].byte_offset;
        strcpy(pagina_destino->chaves[i].identificador, pagina_origem->chaves[i].identificador);
    }

    for (int j = 0; j <= pagina_destino->num_chaves; j++)
    {
        pagina_destino->filhos[j] = pagina_origem->filhos[j];
    }
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
    inserir_elemento_em_pagina(chave_inserida, rrn_inserido, (*pagina_original).chaves, (*pagina_original).filhos, &(*pagina_original).num_chaves);

    int meio = ORDEM_ARVORE / 2;

    *chave_promovida = pagina_auxiliar.chaves[meio];
    *filho_direito_promovido = gerar_novo_rrn(arvore_b);

    inicializar_pagina(pagina_original);
    
    int i = 0;
    while (i < meio)
    {
        pagina_original->chaves[i] = pagina_auxiliar.chaves[i];
        pagina_original->filhos[i] = pagina_auxiliar.filhos[i];
        pagina_original->num_chaves = pagina_original->num_chaves + 1;
        i += 1;
    }

    pagina_original->filhos[i] = pagina_auxiliar.filhos[i];

    inicializar_pagina(nova_pagina_gerada);
    
    i = meio + 1;
    while (i < ORDEM_ARVORE)
    {
        nova_pagina_gerada->chaves[nova_pagina_gerada->num_chaves] = pagina_auxiliar.chaves[i];
        nova_pagina_gerada->filhos[nova_pagina_gerada->num_chaves] = pagina_auxiliar.filhos[i];
        nova_pagina_gerada->num_chaves = nova_pagina_gerada->num_chaves + 1;
        i += 1;
    }

    nova_pagina_gerada->filhos[nova_pagina_gerada->num_chaves] = pagina_auxiliar.filhos[i];
}


void imprpag(pagina pag)
{
    printf("\nNumero de chaves: %d\n", pag.num_chaves);
    for (int i = 0; i < ORDEM_ARVORE - 1; i++)
    {
        printf("[%s, %d] ", pag.chaves[i].identificador, pag.chaves[i].byte_offset);
    }
    printf("\n");
    for (int i = 0; i < ORDEM_ARVORE; i++)
    {
        printf("[%d] ", pag.filhos[i]);
    }
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

    status_operacao res = le_pagina(rrn_atual, &pagina_atual, arvore_b);
    
    //printf("\nPagina lida: %d", res);
    imprpag(pagina_atual);

    int posicao_encontrada = 0;
    status_operacao achou = busca_chave_em_pagina(chave, pagina_atual, &posicao_encontrada);
    
    //printf("\nElemento '%s' deve ser inserido na posicao %d", chave.identificador, posicao_encontrada);
    if (achou == SUCESSO)
    {
        printf("\nErro: Chave duplicada!");
        return ERRO;
    }

    int rrn_pro;
    registro chave_pro;

    //printf("\nChamando recursivamente:\ninsere_chave(%d, [%s, %d])", pagina_atual.filhos[posicao_encontrada], chave.identificador, chave.byte_offset);
    status_insercao retorno = insere_chave(pagina_atual.filhos[posicao_encontrada], chave, &rrn_pro, &chave_pro, arvore_b);

    //status_insercao retorno = SEM_PROMOCAO;

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

    divide_pagina(chave_pro, rrn_pro, &pagina_atual, chave_promovida, filho_direito_promovido, &nova_pagina, arvore_b);
    escreve_pagina(rrn_atual, &pagina_atual, arvore_b);
    escreve_pagina(*filho_direito_promovido, &nova_pagina, arvore_b);
    // deu erro entre o 9 e aqui

    return PROMOCAO;
}


void insere_registro_arquivo_de_dados(FILE* arquivo_de_dados, char* string_registro, int* posicao_inserida)
{
    /*
        Insere um registro no final do arquivo de dados
        Entradas:
            FILE* arquivo_de_dados: O descritor do arquivo contendo os dados dos jogos
            char* string_registro: A string contendo os dados do registro a ser inserido
            int* posicao_inserida: Variável para receber o offset no qual o registro foi escrito
    */

    fseek(arquivo_de_dados, 0, SEEK_END);
    int posicao = ftell(arquivo_de_dados);

    short tamanho_registro = strlen(string_registro);
    fwrite(&tamanho_registro, sizeof(short), 1, arquivo_de_dados);

    fwrite(string_registro, sizeof(char), tamanho_registro, arquivo_de_dados);
    *posicao_inserida = posicao;
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
    int posicao;

    le_pagina(rrn, &pagina_atual, arvore_b);
    status_operacao achou = busca_chave_em_pagina(chave, pagina_atual, &posicao);
    
    if (achou == SUCESSO)
    {
        *rrn_encontrado = rrn;
        *posicao_encontrada = posicao;
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
    fseek(arvore_b, sizeof(pagina) * rrn_alvo + TAMANHO_CABECALHO, SEEK_SET);
    pagina pagina_lida;
    int res = fread(&pagina_lida, sizeof(pagina), 1, arvore_b);

    if (res <= 0)
    {
        return FALHA;
    }

    printf("\nPagina %d", rrn_alvo);
    printf("\nChaves: ");

    for (int i = 0; i < pagina_lida.num_chaves; i++)
    {
        printf("%s |", pagina_lida.chaves[i].identificador);
    }

    printf("\nOffsets: ");
    for (int i = 0; i < pagina_lida.num_chaves; i++)
    {
        printf("%d |", pagina_lida.chaves[i].byte_offset);
    }

    printf("\nFilhos: ");
    for (int i = 0; i <= pagina_lida.num_chaves; i++)
    {
        printf("%d |", pagina_lida.filhos[i]);
    }

    return SUCESSO;
}


void modulo_insercao_arvb_apenas(FILE* arvore_b, registro novo_registro)
{
    /*
        Insere um registro no arquivo arvore b sem inserí-lo no arquivo de dados
        Função usada na criação do índice
        Entradas:
            FILE* arvore_b: Descritor do arquivo contendo a arvore b
            registro novo_registro: Registro contendo a informação a ser inserida
    */

   
    printf("\nInsercao do registro de chave %s e offset %d: ", novo_registro.identificador, novo_registro.byte_offset);
     // printf("\nInsercao do registro de chave \"%s\"", nome);

    status_insercao op;
    int raiz = rrn_raiz(arvore_b);

    int filho_d_pro = -1;

    registro chave_pro;

    chave_pro.byte_offset = -1;

    strcpy(chave_pro.identificador, "\0");

    // printf("\n%s (%d bytes - offset %d)", registro_em_string, strlen(registro_em_string), posicao);

    if (insere_chave(raiz, novo_registro, &filho_d_pro, &chave_pro, arvore_b) == PROMOCAO)
    {
        printf("\nPrecisou criar uma nova raiz");
        pagina nova_pagina;

        inicializar_pagina(&nova_pagina);

        printf("\nNova pagina raiz inicializada");

        nova_pagina.chaves[0] = chave_pro;
        nova_pagina.filhos[0] = raiz;
        nova_pagina.filhos[1] = filho_d_pro;
        nova_pagina.num_chaves += 1;
        
        printf("\nPagina gerada: \n");
        imprpag(nova_pagina);

        int rrn = gerar_novo_rrn(arvore_b);
        
        printf("\nRRN gerado para a nova pagina: %d", rrn);
        escreve_pagina(rrn, &nova_pagina, arvore_b);
        printf("\nEscrevendo a nova pagina no rrn");
        raiz = rrn;
    }

    fseek(arvore_b, 0, SEEK_SET);
    fwrite(&raiz, sizeof(int), 1, arvore_b);
}


void modulo_criar_indice(FILE* arvore_b, FILE* arquivo_de_dados)
{
    /*
        Procedimento que monta o índice no arquivo arvb vazio
        Entrada:
            FILE* arvore_b: Descritor do arquivo vazio para a criação do índice
            FILE* arquivo_de_dados: Descritor do arquivo contendo os dados dos jogos
        
        [Função reutilizada em grande parte do trabalho 1]
    */

    char identificador_atual[TAMANHO_MAXIMO_BUFFER];
    char buffer[TAMANHO_MAXIMO_BUFFER];

    int rrn_inicial = 0;
    fwrite(&rrn_inicial, sizeof(int), 1, arvore_b);
    pagina pagina_inicial;
    inicializar_pagina(&pagina_inicial);
    fwrite(&pagina_inicial, sizeof(pagina), 1, arvore_b);

    fseek(arquivo_de_dados, sizeof(int), SEEK_SET);

    short tamanho_registro;

    int res = 1;

    int posicao_do_ponteiro_de_leitura = sizeof(int);
    do
    {
        fread(&tamanho_registro, sizeof(short), 1, arquivo_de_dados);

        fread(buffer, sizeof(char), tamanho_registro, arquivo_de_dados);

        ler_identificador_registro(buffer, identificador_atual);

        registro novo_registro;
        novo_registro.byte_offset = posicao_do_ponteiro_de_leitura;
        strcpy(novo_registro.identificador, identificador_atual);

        modulo_insercao_arvb_apenas(arvore_b, novo_registro);

        posicao_do_ponteiro_de_leitura += sizeof(short) + tamanho_registro;

    } while (0 == feof(arquivo_de_dados));
}


void modulo_imprimir_arvore_b(FILE* arvore_b)
{
    /*
        Procedimento para imprimir as páginas do arquivo arvb na saída padrão
        Entrada:
            FILE* arvore_b: O arquivo contendo a estrutura da árvore a ser impressa
    */

    fseek(arvore_b, 0, SEEK_SET);

    int rrn_raiz;
    fread(&rrn_raiz, sizeof(int), 1, arvore_b);

    int rrn_max = gerar_novo_rrn(arvore_b);

    for (int i = 0; i < rrn_max; i++)
    {
        if (i == rrn_raiz)
        {
            printf("\n - - - - - - RAIZ - - - - - - ");
            imprime_pagina(i, arvore_b);
            printf("\n - - - - - - - - - - - - - - -\n");        
        }
        else
        {
            imprime_pagina(i, arvore_b);
            printf("\n");
        }
    }
}


void modulo_realizar_operacoes(FILE* arvore_b, FILE* arquivo_de_dados, FILE* arquivo_de_operacoes)
{
    /*
        Procedimento que abre o arquivo de operações e realiza as operações descritas nela, chamando modulo_insercao() ou modulo_busca()
        conforme necessário.

        Parâmetros:
        FILE* arvore_b: O descritor do arquivo contento os dados do registros sobre os quais as operações serão realizadas
        FILE* arquivo_de_operacoes: O descritor do arquivo contendo as operações a serem realizadas junto com os seus argumentos.

    */
   char comando;
   char parametro[TAMANHO_MAXIMO_BUFFER];
   int tamanho_parametro;
   char identificador[TAMANHO_MAXIMO_BUFFER];

   while (comando = fgetc(arquivo_de_operacoes) != EOF) {
        fseek(arquivo_de_operacoes, 1, SEEK_CUR);
        fgets(parametro, TAMANHO_MAXIMO_BUFFER, arquivo_de_operacoes);

        tamanho_parametro = strlen(parametro);

        if (parametro[tamanho_parametro - 1] == '\n') {
            parametro[tamanho_parametro - 1] = '\0';
        }

        switch(comando) {
            case 'b':
                printf("\n");
                ler_identificador_registro(parametro, identificador);
                modulo_busca(arvore_b, identificador, arquivo_de_dados);
                break;
            case 'i':
                printf("\n");
                modulo_insercao(arvore_b, arquivo_de_dados, parametro);
                break;
            default:
                printf("\n");
                printf("\nA operacao '%c' nao e uma operacao valida.", comando);
                break;
        }
   }

}


void modulo_insercao(FILE* arvore_b, FILE* arquivo_de_dados, char* registro_em_string)
{
    /*
        Realiza a operação de inserção no arquivo arvb e no arquivo de dados, imprime os resultados na saída padrão
        Entradas:
            FILE* arvore_b: O descritor do arquivo para se realizar a busca
            FILE* arquivo_de_dados: O descritor do arquivo contendo os dados dos jogos
            char* registro_em_string: A string contendo os dados do registro a ser inserido
    */

    int posicao;
    insere_registro_arquivo_de_dados(arquivo_de_dados, registro_em_string, &posicao);
    char nome[TAMANHO_MAXIMO_BUFFER];
    ler_identificador_registro(registro_em_string, nome);

    printf("\nInsercao do registro de chave \"%s\"", nome);

    registro chave;
    chave.byte_offset = posicao;
    strcpy(chave.identificador, nome);

    status_insercao op;
    int raiz = rrn_raiz(arvore_b);
    int filho_d_pro = -1;
    registro chave_pro;

    printf("\n%s (%d bytes - offset %d)", registro_em_string, strlen(registro_em_string), posicao);

    if (insere_chave(raiz, chave, &filho_d_pro, &chave_pro, arvore_b) == PROMOCAO)
    {
        pagina nova_pagina;

        inicializar_pagina(&nova_pagina);
        nova_pagina.chaves[0] = chave_pro;
        nova_pagina.filhos[0] = raiz;
        nova_pagina.filhos[1] = filho_d_pro;
        nova_pagina.num_chaves += 1;
        int rrn = gerar_novo_rrn(arvore_b);
        escreve_pagina(rrn, &nova_pagina, arvore_b);
        raiz = rrn;
    }

    fseek(arvore_b, 0, SEEK_SET);
    fwrite(&raiz, sizeof(int), 1, arvore_b);

}


int rrn_raiz(FILE* arvore_b)
{
    /*
        Pega o RRN da raíz da árvore no cabeçalho do arquivo
        Entrada:
            FILE* arvore_b: O arquivo contendo a árvore B
        Retorno:
            O inteiro representando o RRN da raiz
    */
    fseek(arvore_b, 0, SEEK_SET);
    int rrn;
    fread(&rrn, sizeof(int), 1, arvore_b);
    return rrn;
}


void modulo_busca(FILE* arvore_b, char* identificador_registro, FILE* arquivo_de_dados)
{
    /*
        Realiza a operação de busca no arquivo arvb e imprime os resultados na saída padrão
        Entradas:
            FILE* arvore_b: O descritor do arquivo para se realizar a busca
            FILE* arquivo_de_dados: O descritor do aquivo contendo os dados dos jogos
            char* identificador_registro: O identificador do registro a ser procurado
    */

    registro chave;
    chave.byte_offset = -1;
    strcpy(chave.identificador, identificador_registro);

    printf("\nBusca pelo registro de chave \"%s\"", chave.identificador);
    
    int rrn_encontrado;
    int posicao_encontrada;
    int raiz;

    raiz = rrn_raiz(arvore_b);
    status_operacao op;
    op = busca_chave(raiz, chave, &rrn_encontrado, &posicao_encontrada, arvore_b);

    int offset_pagina = TAMANHO_CABECALHO + sizeof(registro) * rrn_encontrado;
    pagina pagina_encontrada;
    fread(&pagina_encontrada, sizeof(pagina), 1, arvore_b);

    int offset_no_arquivo = pagina_encontrada.chaves[posicao_encontrada].byte_offset;

    fseek(arquivo_de_dados, offset_no_arquivo, SEEK_SET);

    char dados_do_jogo[TAMANHO_MAXIMO_BUFFER];
    int tamanho_do_registro;
    fread(&tamanho_do_registro, sizeof(short), 1, arquivo_de_dados);
    fread(dados_do_jogo, sizeof(char), tamanho_do_registro, arquivo_de_dados);

    printf("\n%s (%d bytes - offset %d)", dados_do_jogo, tamanho_do_registro, offset_no_arquivo);

}

void ler_identificador_registro(char *registro, char *nome)
{
    /*
    Recupera a string que serve como identificador de um registro
    [Função reutilizada do trabalho 1]

    Parâmetros:
        char* registro: String contendo o registro a ser lido
        char* nome: String que irá receber o identificador do registro
    */

    // if (registro[0] == '*')
    // {
    //     nome[0] = '\0';
    // }

    int i = 0;
    while (registro[i] != '|' && registro[i] != '\0')
    {
        nome[i] = registro[i];
        i++;
    }
    nome[i] = '\0';
}


int main(int argc, char *argv[]) 
{
    FILE *arquivo_de_dados = fopen(NOME_ARQUIVO_DADOS, "rb+");
    // FILE *btree = fopen("arvb.dat", "rb+");

    if (arquivo_de_dados == NULL)
    {
        fprintf(stderr, "\nNao foi encontrado o arquivo de dados para leitura");
        exit(EXIT_FAILURE);
    }
    
    if (argc == 2 && strcmp(argv[1], "-c") == 0)
    {
        printf("Modo de criacao do indice ativado... "); 
        FILE* arvore_b = fopen("arvb.dat", "w+b");
        modulo_criar_indice(arvore_b, arquivo_de_dados); 
        fclose(arvore_b);
    }
    else if (argc == 3 && strcmp(argv[1], "-e") == 0)
    {
    	printf("Modo de realizar operacoes ativado... "); 
        FILE *arquivo_de_operacoes = fopen(argv[2], "rb");
        FILE* arvore_b = fopen("arvb.dat", "r+b");

        if (arvore_b == NULL)
        {
            printf("\nErro: Arquivo \"arvb.dat\" nao foi encontrado");
            return 0;
        }

        modulo_realizar_operacoes(arvore_b, arquivo_de_dados, arquivo_de_operacoes);
        fclose(arvore_b);
    }
    else if (argc == 2 && strcmp(argv[1], "-p") == 0)
    {
    	printf("Modo de impressao da arvore ativado... "); 

        FILE* arvore_b = fopen("arvb.dat", "r+b");

        if (arvore_b == NULL)
        {
            printf("\nErro: Arquivo \"arvb.dat\" nao foi encontrado");
            return 0;
        }
        modulo_imprimir_arvore_b(arvore_b); 
    }
    else 
    {
        fprintf(stderr, "Argumentos incorretos!\n");
        fprintf(stderr, "Modo de uso:\n");
        // fprintf(stderr, "$ %s -e nome_arquivo\n", argv[0]); // Colocar comandos formatadinhos aqui <-----
        // fprintf(stderr, "$ %s -p\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    fclose(arquivo_de_dados);
    return 0;
}