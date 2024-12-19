#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

HINSTANCE handle;

typedef int (*AbreConexaoImpressora_t)(int, const char *, const char *, int);
typedef int (*FechaConexaoImpressora_t)();
typedef int (*ImpressaoTexto_t)(const char *, int, int, int);
typedef int (*Corte_t)(int);
typedef int (*ImpressaoQRCode_t)(const char *, int, int);
typedef int (*ImpressaoCodigoBarras_t)(int, const char *, int, int, int);
typedef int (*StatusImpressora_t)(int);
typedef int (*AvancaPapel_t)(int);
typedef int (*AbreGavetaElgin_t)();
typedef int (*AbreGaveta_t)(int, int, int);
typedef int (*ImprimeXMLSAT_t)(const char *, int);
typedef int (*ImprimeXMLCancelamentoSAT_t)(const char *, const char *, int);
typedef int (*SinalSonoro_t)(int, int, int);


AbreConexaoImpressora_t AbreConexaoImpressora;
FechaConexaoImpressora_t FechaConexaoImpressora;
ImpressaoTexto_t ImpressaoTexto;
Corte_t Corte;
ImpressaoQRCode_t ImpressaoQRCode;
ImpressaoCodigoBarras_t ImpressaoCodigoBarras;
StatusImpressora_t StatusImpressora;
AvancaPapel_t AvancaPapel;
ImprimeXMLSAT_t ImprimeXMLSAT;
ImprimeXMLCancelamentoSAT_t ImprimeXMLCancelamentoSAT;
AbreGaveta_t AbreGaveta;
AbreGavetaElgin_t AbreGavetaElgin;
SinalSonoro_t SinalSonoro;

int carregarFuncoes() {
    handle = LoadLibrary("E1_Impressora01.dll");
    if (!handle) {
        fprintf(stderr, "Erro ao carregar a biblioteca.\n");
        return 0;
    }

    AbreConexaoImpressora = (AbreConexaoImpressora_t)GetProcAddress(handle, "AbreConexaoImpressora");
    FechaConexaoImpressora = (FechaConexaoImpressora_t)GetProcAddress(handle, "FechaConexaoImpressora");
    ImpressaoTexto = (ImpressaoTexto_t)GetProcAddress(handle, "ImpressaoTexto");
    Corte = (Corte_t)GetProcAddress(handle, "Corte");
    ImpressaoQRCode = (ImpressaoQRCode_t)GetProcAddress(handle, "ImpressaoQRCode");
    ImpressaoCodigoBarras = (ImpressaoCodigoBarras_t)GetProcAddress(handle, "ImpressaoCodigoBarras");
    StatusImpressora = (StatusImpressora_t)GetProcAddress(handle, "StatusImpressora");
    AvancaPapel = (AvancaPapel_t)GetProcAddress(handle, "AvancaPapel");
    
    ImprimeXMLSAT = (ImprimeXMLSAT_t)GetProcAddress(handle, "ImprimeXMLSAT");
    ImprimeXMLCancelamentoSAT = (ImprimeXMLCancelamentoSAT_t)GetProcAddress(handle, "ImprimeXMLCancelamentoSAT");
    AbreGaveta = (AbreGaveta_t)GetProcAddress(handle, "AbreGaveta");
    AbreGavetaElgin = (AbreGavetaElgin_t)GetProcAddress(handle, "AbreGavetaElgin");
    SinalSonoro = (SinalSonoro_t)GetProcAddress(handle, "SinalSonoro");

    if (!AbreConexaoImpressora || !FechaConexaoImpressora || !ImpressaoTexto || !Corte ||
        !ImpressaoQRCode || !ImpressaoCodigoBarras || !StatusImpressora || !AvancaPapel ||
		 !ImprimeXMLSAT || !ImprimeXMLCancelamentoSAT || !AbreGaveta || !AbreGavetaElgin || !SinalSonoro) {
        fprintf(stderr, "Erro ao carregar as funcoes da biblioteca.\n");
        return 0;
    }
    return 1;
}

void liberarBiblioteca() {
    if (handle) {
        FreeLibrary(handle);
    }
}

void exibirMenu() {
	printf("\n#########################################################################\n");
	printf("############################# MENU IMPRESSORA ############################\n");
	printf("#########################################################################\n");
	printf(" 1  - Abrir Conexao\n");
	printf(" 2  - Impressao Texto\n");
	printf(" 3  - Impressao Codigo de barras\n");
	printf(" 4  - Impressao QR Code\n");
	printf(" 5  - Impressao XML SAT\n");
	printf(" 6  - Impressao XML Cancelamento SAT\n");
	printf(" 7  - Abrir Gaveta\n");
	printf(" 8  - Abrir Gaveta Elgin\n");
	printf(" 9  - Sinal Sonoro\n");
	printf(" 0  - Fechar Conexao\n");
	
}

void configurarConexao(
	int *tipo,
	char * modelo,
	char * conexao,
	int *parametro
) {
	printf("\n#########################################################################\n");
	printf("############################# Configurar conexao #########################\n");
	printf("#########################################################################\n\n\n");
	
    int configMenu = 0;
    
    while (configMenu != 1) {
        printf("Informe o tipo de comunicacao: ");
        scanf("%i", tipo);
        
        printf("Informe o modelo: ");
        scanf("%s", modelo);
        
        printf("Informe o tipo de conexao / detalhe de comunicacao: ");
        scanf("%s", conexao);
        
        printf("Informe o parametro: ");
        scanf("%i", parametro);

        printf("\nTipo: %i\nModelo: %s\nConexao: %s\nParametro: %i\n\n", *tipo, modelo, conexao, *parametro);
        
        printf("Dados corretos?\n");
        printf("1 - Sim\n2 - Nao\n");
        printf("-> ");
        scanf("%i", &configMenu);
        
        switch (configMenu) {
            case 1:
                printf("Configuracao concluida!\n\n\n");
                break;
            case 2:
                printf("\nPor favor, insira novamente os dados.\n\n");
                break;
            default:
                printf("Opcao Invalida. Tente novamente.\n");
                break;
        }
    }
}

int estadoConexao = 1;

int abrirConexao()	{
	int tipo;
    char modelo[50];
    char conexao[50];
    int parametro;
    int conexaoMenu = -1;

    configurarConexao(&tipo, modelo, conexao, &parametro);
    int response = AbreConexaoImpressora(tipo, modelo, conexao, parametro);
    
    printf("\nTipo: %i\nModelo: %s\nConexao: %s\nParametro: %i\n\n", tipo, modelo, conexao, parametro);
    
    
    if(response == 0){
    	printf("conexao aberta com sucesso!");
    	estadoConexao = 0;
	} else {
		while (conexaoMenu != 0) {
			estadoConexao = 1;
			printf("Deseja tentar novamente?\n1 - Sim\n2 - Nao\n");
			printf("-> ");
        	scanf("%i", &conexaoMenu);
        	
        	switch (conexaoMenu) {
        		case 1:
        			configurarConexao(&tipo, modelo, conexao, &parametro);
        			break;
        		case 2:
        			conexaoMenu = 0;
        			break;
        		default:
                printf("Opcao Invalida. Tente novamente.\n");
                break;
        	}
		}
	}

}

void impressaoTexto() {
	printf("\n#########################################################################\n");
	printf("############################# Impressao Texto ############################\n");
	printf("#########################################################################\n\n\n");
	
	char dados[500] = "Richard o melhor professor, for ever";
	int posicao = 1;
	int stilo = 4;
	int tamanho = 0;
   
    int response = ImpressaoTexto(dados, posicao, stilo, tamanho);
    AvancaPapel(5);
    Corte(5);
    
    if (estadoConexao != 0) {
    	printf("Impressora nao conectada!");
    	return;
	}
    
    if(response == 0){
    	printf("sucesso!");
	} else {
		printf("ERRO, Retorno %i\n", response);
	}
}

void impressaoQRCode() {
    printf("\n#########################################################################\n");
	printf("############################# Impressao QR Code ############################\n");
	printf("#########################################################################\n\n\n");
	
	char dados[200] = "Sao paulo futebol clube = gigante, o maior";
	int tamanho = 6;
	int nivelCorrecao = 4;
    
    int response = ImpressaoQRCode(dados, tamanho, nivelCorrecao);
    AvancaPapel(8);
    Corte(6);
    
    if (estadoConexao != 0) {
    	printf("Impressora nao conectada!");
    	return;
	}
    
    if(response == 0){
    	printf("sucesso!");
	} else {
		printf("ERRO, Retorno %i\n", response);
	}
}

void impressaoCodigoBarras() {
    printf("\n#########################################################################\n");
	printf("############################# Impressao Codigo de Barras #################\n");
	printf("#########################################################################\n\n\n");
	
	int tipo = 8;
	char dados[500] = "{A012345678912";
	int altura = 100;
	int largura = 2;
	int HRI = 3;
    
    int response = ImpressaoCodigoBarras(tipo, dados, altura, largura, HRI);
    AvancaPapel(5);
    Corte(5);
    
    if (estadoConexao != 0) {
    	printf("Impressora nao conectada!");
    	return;
	}
    
    if(response == 0){
    	printf("sucesso!");
	} else {
		printf("ERRO, Retorno %i\n", response);
	}
}

void impressaoXMLSAT() {
	printf("\n#########################################################################\n");
	printf("############################# Impressao XMLSAT ###########################\n");
	printf("#########################################################################\n\n\n");
	
	char dados[500] = "path=C:\\Users\\felix_muniz\\Downloads\\ProjetoC\\XMLSAT.xml";
    int param = 0;
    
    int response = ImprimeXMLSAT(dados, param);
    AvancaPapel(5);
    Corte(5);
    
    if (estadoConexao != 0) {
    	printf("Impressora nao conectada!");
    	return;
	}
    
    if(response == 0){
    	printf("sucesso!");
	} else {
		printf("ERRO, Retorno %i\n", response);
	}
}

void impressaoXMLCancelamentoSAT() {
	printf("\n#########################################################################\n");
	printf("#################### Impressao XMLSAT CancelamentoSAT #################\n");
	printf("#########################################################################\n\n\n");
	
	int param = 0;
   	char dados[500] = "path=C:\\Users\\felix_muniz\\Downloads\\ProjetoC\\CANC_SAT.xml";
	char assQRCode[500] = "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZjbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNcSdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQEVd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6CYVFCDtYR9Hi5qgdk31v23w==";
    
    int response = ImprimeXMLCancelamentoSAT(dados, assQRCode, param);
    AvancaPapel(5);
    
    if (estadoConexao != 0) {
    	printf("Impressora nao conectada!");
    	return;
	}
    
    if(response == 0){
    	printf("sucesso!");
	} else {
		printf("ERRO, Retorno %i\n", response);
	}
}

void abreGaveta() {
	printf("\n#########################################################################\n");
	printf("#################### Abre gaveta #################\n");
	printf("#########################################################################\n\n\n");	
    

    
    int response = AbreGaveta(1, 11, 10);
    
    if(response == 0){
    	printf("gaveta aberta com sucesso!");
	} else {
		printf("ERRO, Retorno %i\n", response);
	}
}

void abreGavetaElgin(

) {
    printf("\n#########################################################################\n");
	printf("#################### Abre gaveta Elgin #################\n");
	printf("#########################################################################\n\n\n");
    
    int response = AbreGavetaElgin();
    
    if(response == 0){
    	printf("gaveta aberta com sucesso!");
	} else {
		printf("ERRO, Retorno %i\n", response);
	}
}

int main() {
    carregarFuncoes();
    int opcao = -1;
    
    while(opcao != 100){
		exibirMenu();
		printf("\n\nDigite a opcao desejada: ");
		scanf("%d", &opcao);
		
		switch (opcao) {
            case 1:
                abrirConexao();
                break;
            case 2:
            	impressaoTexto();
                break;
            case 3:
            	impressaoCodigoBarras();
            	break;
            case 4:
            	impressaoQRCode();
            	break;
            case 5:
            	impressaoXMLSAT();
            	break;
            case 6:
            	impressaoXMLCancelamentoSAT();
            	break;
            case 7:
            	abreGaveta();
            	break;
			case 8:
				abreGavetaElgin();
				break;
			case 9:
				SinalSonoro(4, 50, 5);
				break;
            case 0:
            	FechaConexaoImpressora();
                printf("Fechar conexao e sair...\n");
                break;
            default:
                printf("Opcao Invalida.\n");
    	}
    }
}