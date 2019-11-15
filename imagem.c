#include "imagem.h"
#include "struct.h"
#include "func.h"
#include "linha.h"

/****************************************************
Função: criarImagem
Parâmetros: nenhum
Retorno: estrutura Imagem

Descrição: serve para inicializar uma nova imagem, recebendo as entradas necessárias
e retornando os valores atribuídos em uma estrutura do tipo Imagem.
*****************************************************/
Imagem criarImagem(int *imagemAberta, int lar, int alt){
	Imagem imagem;

	/* atribuindo configurações padrão */
	strcpy(imagem.id, "P3");
	imagem.max = 255;

	imagem.lar = lar;
	imagem.alt = alt;
	imagem.numDePixels = imagem.lar * imagem.alt;

	/* alocação dinâmica da matriz pixels */
	imagem.pixels = (Pixel **) safeMalloc(imagem.alt * sizeof(Pixel *));
	for (int i = 0; i < imagem.alt; i++){
		imagem.pixels[i] = (Pixel *) safeCalloc(imagem.lar, sizeof(Pixel));

		/* a imagem será criada com a cor branca */
		for (int j = 0; j < imagem.lar; j++){
			imagem.pixels[i][j].r = 255;
			imagem.pixels[i][j].g = 255;
			imagem.pixels[i][j].b = 255;
		}
	}

	/* atribuindo a cor preta ao pincel */
	imagem.cor.r = 0;
	imagem.cor.g = 0;
	imagem.cor.b = 0;

	/* inicia uma estrutura de desenho */
	imagem.desenho = criarDesenho();

	*imagemAberta = 1;

	return imagem;
}

/****************************************************
Função: lerImagem
Parâmetros: nenhum
Retorno: tipo Imagem

Descrição: lê um arquivo ppm, atribui a uma estrutura Imagem e 
retorna essa estrutura.
*****************************************************/
Imagem abrirImagem(int *imagemAberta, char caminho[100]){
	/* removendo o '\n' da string caminho */
	/*for (int i = 0; i < strlen(caminho); ++i){
		if (caminho[i] == '\n')
			caminho[i] = '\0';
	}*/
	caminho[strlen(caminho) - 1] = '\0';

	Imagem imagem;
	imagem.arquivo = fopen(caminho, "r");
	if (imagem.arquivo == NULL){
		printf("%s: NULL\n", caminho);
		*imagemAberta = 0;
		return imagem;
	}

	/* leitura de conteudo */
	fscanf(imagem.arquivo, "%s\n", imagem.id);
	fscanf(imagem.arquivo, "%d %d", &imagem.lar, &imagem.alt);
	imagem.numDePixels = imagem.lar * imagem.alt;
	fscanf(imagem.arquivo, "%d", &imagem.max);

	/* alocação dinâmica da matriz pixels */
	imagem.pixels = (Pixel **) safeMalloc(imagem.alt * sizeof(Pixel *));
	for (int i = 0; i < imagem.alt; i++){
		imagem.pixels[i] = (Pixel *) safeCalloc(imagem.lar, sizeof(Pixel));
	}

	/* lendo pixels da imagem e atribuindo à matriz */
	for (int i = 0; i < imagem.alt; ++i){
		for (int j = 0; j < imagem.lar; ++j){
			fscanf(imagem.arquivo, "%d %d %d\n",
				&imagem.pixels[i][j].r,
				&imagem.pixels[i][j].g,
				&imagem.pixels[i][j].b);
		}
	}

	/* atribuindo a cor preta ao pincel */
	imagem.cor.r = 0;
	imagem.cor.g = 0;
	imagem.cor.b = 0;

	/* inicia uma estrutura de desenho */
	imagem.desenho = criarDesenho();

	fclose(imagem.arquivo);

	*imagemAberta = 1;

	return imagem;
}

/****************************************************
Função: salvarImagem
Parâmetros: ponteiro tipo Imagem
Retorno: nenhum

Descrição: cria um novo arquivo ppm e escreve todas as informações
da imagem dentro dele.
*****************************************************/
void salvarImagem(Imagem *imagem){
	/*  */
	char nomeArq[50] = "./galeria/";
	strcat(nomeArq, imagem->nomeDoArquivo);
	strcpy(imagem->nomeDoArquivo, nomeArq);

	/* criando novo arquivo */
	imagem->arquivo = fopen(imagem->nomeDoArquivo, "w");

	/* inserindo desenhos na imagem */
	inserirDesenhos(imagem);

	/* inserindo informações de id, dimensões e valor de cor máximo */
	fprintf(imagem->arquivo, "%s\n", imagem->id);
	fprintf(imagem->arquivo, "%d %d\n", imagem->lar, imagem->alt);
	fprintf(imagem->arquivo, "%d\n", imagem->max);

	/* escrevendo matriz de pixels no arquivo */
	for (int x = 0; x < imagem->alt; x++){
		for (int y = 0; y < imagem->lar; y++){
			fprintf(imagem->arquivo, "%d %d %d\n",
				imagem->pixels[x][y].r,
				imagem->pixels[x][y].g,
				imagem->pixels[x][y].b);
		}
	}

	/* fechando arquivo */
	fclose(imagem->arquivo);
}

/****************************************************
Função: limparImagem
Parâmetros: ponteiro tipo Imagem
Retorno: nenhum

Descrição: limpa toda a imagem para uma cor especificada
*****************************************************/
void limparImagem(Imagem *imagem, int r, int g, int b){
	for (int i = 0; i < imagem->alt; ++i)
		for (int j = 0; j < imagem->lar; ++j){
			imagem->pixels[i][j].r = r;
			imagem->pixels[i][j].g = g;
			imagem->pixels[i][j].b = b;
		}

	/* todos os desenhos serão apagados */
	imagem->desenho = criarDesenho();
}

/****************************************************
Função: criarDesenho
Parâmetros: nenhum
Retorno: estrutura Desenho

Descrição: responsável por criar uma estrutura Desenho e retorná-la
*****************************************************/
Desenho criarDesenho(void){
	Desenho d;

	d.numLinhas = 0;

	return d;
};

/****************************************************
Função: listarDesenhos
Parâmetros: tipo Desenho
Retorno: nenhum

Descrição: imprime na tela todos os desenhos feitos na imagem
*****************************************************/
void listarDesenhos(Desenho d){
	/* listar linhas */
	for (int i = 0; i < d.numLinhas; ++i)
		printf("linha %02d - p1 (%d,%d), p2 (%d,%d)\n", 
			i+1, 
			d.linhas[i].inicio.x,
			d.linhas[i].inicio.y,
			d.linhas[i].fim.x,
			d.linhas[i].fim.y);
}

/****************************************************
Função: inserirDesenhos
Parâmetros: nenhum
Retorno: ponteiro tipo Imagem

Descrição: aplica todos os desenhos na matriz de pixel, essa função é
chamada apenas na etapa de salvamento do arquivo ppm
*****************************************************/
void inserirDesenhos(Imagem *imagem){
	/* inserindo linhas */
	for (int i = 0; i < imagem->desenho.numLinhas; ++i){
		inserirLinha(imagem->desenho.linhas[i], imagem);
	}
}