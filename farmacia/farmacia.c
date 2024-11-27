#include <iostream>
#include <string.h>
#include <Windows.h>
#include <locale>

struct reg
{
    char nome[20];
    char preco[20];
    char quantidade[20];
    char status;
};
//fx q manipulam aqr para salvar, consultar ou excluir informações.
int tamanho(FILE*);
void cadastrar(FILE*);
void consultar(FILE*);
void geraarqtxt(FILE*);
void excluir(FILE*);

int main()
{
    setlocale(LC_ALL, "portuguese");
    int op;
    FILE* arq;
    //o código tenta abrir o arquivo "farmacia.dat" no modo "rb+",
    if ((arq = fopen("farmacia.dat", "rb+")) == NULL)
    {
        if ((arq = fopen("farmacia.dat", "wb+")) == NULL)//abre/cria arq p escrita
        {
            printf("Falha ao abrir o arquivo!\n"); //Se o arquivo não puder ser aberto em nenhum dos dois modos
            system("pause");
        }
    }

    do
    {
        system("CLS");
        printf("\n======= FARMÁCIA ======= \n");
        printf("1. Cadastrar Medicamento\n");
        printf("2. Consultar por Código\n");
        printf("3. Gerar Relatório\n");
        printf("4. Excluir Registro\n");
        printf("5. Sair\n");
        printf("=========== Medicamentos: %d ============\n", tamanho(arq));
        printf("Opção: ");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            cadastrar(arq);
            break;
        case 2:
            consultar(arq);
            break;
        case 3:
            geraarqtxt(arq);
            break;
        case 4:
            excluir(arq);
            break;
        case 5:
            fclose(arq);
        }
    } while (op != 5);
}

void cadastrar(FILE* arq)
{
    reg medicamento;
    char confirma;
    medicamento.status = ' ';
    fflush(stdin); //limpar o buffer de entrada, removendo quaisquer caracteres remanescentes.

    printf("Cadastrando novo registro:\n");
    printf("\nNúmero do registro: %d\n", tamanho(arq) + 1); //facilitando a consulta
    printf("Nome do Medicamento: ");
    gets(medicamento.nome);
    printf("Preço do Medicamento: ");
    gets(medicamento.preco);
    printf("Quantidade em Estoque: ");
    gets(medicamento.quantidade);
    printf("\nConfirma <s/n>: ");
    fflush(stdin);
    scanf("%c", &confirma); //usamos %c pq eh um caracter q vms armz

    if (toupper(confirma) == 'S')
    {
        printf("\nGravando...\n\n");
        fseek(arq, 0, SEEK_END); //usada p mover o ponteiro de leitura ou gravação
        //arq ponteiro para o arquivo q foi aberto
        fwrite(&medicamento, sizeof(reg), 1, arq); //escrever dados no arq
        //calcula bytes na memoria
    }
    system("pause");
}

void consultar(FILE* arq)
{
    reg medicamento;
    int nr;
    printf("\nConsulta pelo Código\n");
    printf("\nInforme o Código...: ");
    scanf("%d", &nr);
    if ((nr <= tamanho(arq)) && (nr > 0)) //verifica se o numero do registo
    {
        fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
        fread(&medicamento, sizeof(reg), 1, arq);
        if (medicamento.status == ' ')
        {
            printf("\nNome do Medicamento...: %s", medicamento.nome);
            printf("\nPreço.................: %s", medicamento.preco);
            printf("\nQuantidade............: %s\n\n", medicamento.quantidade);
        }
        else
            printf("\nRegistro excluído! \n");
    }
    else
    {
        printf("\nNúmero de registro inválido!\n");
    }
    system("pause");
}

void geraarqtxt(FILE* arq)
{
    char nomearq[20];
    printf("Nome do arquivo texto: ");
    scanf("%s", nomearq);
    strcat(nomearq, ".txt");
    FILE* arqtxt = fopen(nomearq, "w");
    if (!arqtxt)
    {
        printf("Não foi possível criar esse arquivo!\n");
        system("pause");
    }
    fprintf(arqtxt, "Nome                Preço       Quantidade     Status\n");
    fprintf(arqtxt, "=====================================================\n");
    int nr;
    reg medicamento;
    for (nr = 0; nr < tamanho(arq); nr++)
    {
        fseek(arq, nr * sizeof(reg), SEEK_SET);
        fread(&medicamento, sizeof(reg), 1, arq);
        fprintf(arqtxt, "%-20s%-12s%-15s- %c\n", medicamento.nome, medicamento.preco, medicamento.quantidade, medicamento.status);
    }
    fprintf(arqtxt, "=====================================================\n");
    fclose(arqtxt);
}

void excluir(FILE* arq)
{
    reg medicamento;
    char confirma;
    int nr;

    printf("\nInforme o Código do registro para excluir\n");
    scanf("%d", &nr);
    if ((nr <= tamanho(arq)) && (nr > 0))
    {
        fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
        fread(&medicamento, sizeof(reg), 1, arq);
        if (medicamento.status == ' ')
        {
            printf("\nNome do Medicamento...: %s", medicamento.nome);
            printf("\nPreço.................: %s", medicamento.preco);
            printf("\nQuantidade............: %s\n", medicamento.quantidade);
            printf("\nConfirma a exclusão: <s/n>\n");
            getchar();
            scanf("%c", &confirma);

            if (toupper(confirma) == 'S')
            {
                printf("\nExcluindo...\n\n");
                fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
                medicamento.status = '*';
                fwrite(&medicamento, sizeof(reg), 1, arq);
            }
        }
        else
            printf("Registro inexistente! \n");
    }
    else
    {
        printf("\nNúmero de registro inválido!\n");
    }
    system("pause");
}

int tamanho(FILE* arq)
{
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(reg);
}
