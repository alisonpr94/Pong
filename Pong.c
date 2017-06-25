#include <stdio.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "arquivosPong.h"

#define FONTSIZE 15 //tamanho da fonte
#define SCREEN_W 640 //tamanho da janela que sera criada
#define SCREEN_H 480

TTF_Font *my_font;

int main(){
	srand(time(NULL));	
	FILE *arq;
	
	int salvarJogo,jogando,controle,voltaJogo,jogarOuSair, i;
	char salvar,pontosc1[4],pontosc2[4], sair[10];
	JOGADORES aux;
	jogando = 0;

	printf("\t\t\tBEM-VINDO AO PONG\n\n");
	printf("\t\t\t\tMENU\n\n");
	printf("1 - Novo Jogo\n");
	printf("2 - Mostrar Pontuações\n");
	printf("-> ");
	scanf("%d", &controle);

	if(controle == 1){
		lerNomes();
		jogando = 1;	
	}
	
	if(controle == 2){		
		mostrarTodosOsPlacares(arq);//mostra a pontuação de todos os jogadores e nomes	
		printf("\nJogar ou sair");
		printf("\nInforme 1 para jogar ou 0 para sair");
		printf("\n-> ");
		scanf("%d",&jogarOuSair);
		
		if(jogarOuSair == 1){
			lerNomes();		
			jogando = 1;		
		}	
	}

	SDL_Surface* screen; //superficie que representa a tela do computador
    	SDL_Event event; //um evento enviado pela SDL	
	SDL_Init(SDL_INIT_VIDEO); //inicializar a SDL
	TTF_Init();
	my_font = TTF_OpenFont("fonte.ttf", FONTSIZE);	
	Uint8 *keystate;
	
	if(jogando == 1){//Não criar a janela se o jogador sair     	
		screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 16,SDL_SWSURFACE);//criar uma janela 	640x480x16bits
		iniciaJogo(screen);//inicializa as variáveis
		keystate = 0;	
	}
	
	while(jogando)//rodar enquanto nao for para encerrar :)
    {
		    	
		//checar eventos
   	 	SDL_PumpEvents();
   	 	keystate = SDL_GetKeyState(NULL);
            			
		if(keystate[SDLK_UP])
            moveRaquete(1,1);		
        if (keystate[SDLK_DOWN])
            moveRaquete(1,0);
		if(keystate[ SDLK_w])
			moveRaquete(0,1);			
		if(keystate[SDLK_s])
			moveRaquete(0,0);
		if(keystate[SDLK_ESCAPE]){

			jogando = 0;
			printf("\nDigite 1 se deseja salvar ou 0 para sair.\n");
			printf("->  ");
			scanf("%d",&salvarJogo);

			if(salvarJogo){
				salvarNovoJogo(arq);//salvar jogo, nome e pontuação 			
			}							
		}

		moveBola(screen);//movimenta a bola e trata colisões
		atualiza(screen);//atualiza o fundo
		desenhaLinha(screen); //função que cria linha na tela

		sprintf(pontosc1, "  %s: %d",jogadores.nome1, jogadores.ponto1);			
		sprintf(pontosc2,"%s: %d",jogadores.nome2, jogadores.ponto2);
		sprintf(sair, "%s", "ESC para sair!");
		desenha_texto(pontosc1, screen, 25, 20);
		desenha_texto(pontosc2, screen, 440, 20);
		desenha_texto(sair, screen, 225, 448);

		desenhaRaquete(screen);//desenha a raquete
		desenhaBola(screen);//desenha a bola
    	SDL_Flip(screen); //atualizar a tela	
	}	
	
	fim();		
		
    return 0;
}

//Funcao que desenha na tela o nome e a pontuação dos jogadores
void desenha_texto(char* texto, SDL_Surface* dst, int x, int y){
	
	SDL_Color cor = {255, 255, 255}; //vermelho
  	SDL_Surface* src = TTF_RenderText_Blended(my_font, texto, cor);
 	SDL_Rect dst_rect = {x, y, 0, 0};
  
 	SDL_BlitSurface(src, NULL, dst, &dst_rect);
  
 	SDL_FreeSurface(src);
}

//Função que os nomes dos jogadores
void imprimir(){
	printf("\nJogador1: %s\t\t",jogadores.nome1);
	printf("Jogador2: %s\n",jogadores.nome2);	
}

//Função que lê os nomes dos jogadores
void lerNomes(){
	int i;

	printf("Informe o nome do primeiro jogador: ");
	scanf("%s",jogadores.nome1);
	printf("Informe o nome do segundo jogador: ");
	scanf("%s",jogadores.nome2);

	for(i = 0; i < strlen(jogadores.nome1); i++){
		jogadores.nome1[i] = toupper(jogadores.nome1[i]);
	}

	for(i = 0; i < strlen(jogadores.nome2); i++){
		jogadores.nome2[i] = toupper(jogadores.nome2[i]);
	}
}

//Função que salva o novo jogo em arquivo binário
void salvarNovoJogo(FILE *arq){
	JOGADORES aux1;
	strcpy(aux1.nome1,jogadores.nome1);
	strcpy(aux1.nome2,jogadores.nome2);
	aux1.ponto1 = jogadores.ponto1;
	aux1.ponto2 = jogadores.ponto2;
		
	arq = fopen("dados.bin", "a+b");
	if(!arq){
		printf("\nNao foi possivel abrir o arquivo!\n");
		exit(1);
	}

	fwrite(&aux1, sizeof(JOGADORES), 1, arq);

	fclose(arq);
}

//Função que mostra todos os placares
void mostrarTodosOsPlacares(FILE *arq){
	JOGADORES aux;
		
	arq = fopen("dados.bin", "rb");
	if(!arq){
		printf("Nao foi possivel salvar seu jogo!\n");
		exit(1);
	}

	fread(&aux,sizeof(JOGADORES),1,arq);

	while(!feof(arq)){
		printf("\nJogo\n");		
		printf("#########################\n");		
		printf(" Jogador1: %s\n", aux.nome1);		
		printf(" Potuação: %d\n", aux.ponto1);
		printf(" Jogador2: %s\n", aux.nome2);
		printf(" Pontuação: %d\n", aux.ponto2);
		printf("########################");
		printf("\n\n");		
		fread(&aux,sizeof(JOGADORES),1,arq);
	}

	fclose(arq);
}

//Função que inicia os valores
void iniciaJogo(SDL_Surface* screen){
	
	bola.x = (screen->w / 2) - 10;
	bola.y = (screen->h / 2) - 10;
	bola.w = 10;
	bola.h = 10;
	bola.dy = 1.0;
	bola.dx = 1.0;
	
	raquete[0].x = 10;
	raquete[0].y = 200;
	raquete[0].w = 20;
	raquete[0].h = 100;

	raquete[1].x = 610;
	raquete[1].y = 200;
	raquete[1].w = 20;
	raquete[1].h = 100;
}

//Função que desenha as raquetes
void desenhaRaquete(SDL_Surface* screen){
	int i;
	SDL_Rect rect; 
	
	for(i = 0; i < 2; i++){
		rect.x = raquete[i].x;
		rect.y = raquete[i].y;
		rect.w = raquete[i].w;
		rect.h = raquete[i].h;
		SDL_FillRect(screen,&rect,SDL_MapRGB(screen->format,255,255,255));
	}	
}

//Função que desenha linha na tela
void desenhaLinha(SDL_Surface *screen){
	
	char linha[1];
	int i, y = 0;

	for(i = 0; i < 420; i++){
		sprintf(linha, "%s", "|");
		desenha_texto(linha, screen, 310, y);
		y = y + 1;
	}
}

//Função que desenha a bola
void desenhaBola(SDL_Surface* screen){
	SDL_Rect rect3;
	
	rect3.x = bola.x;
	rect3.y = bola.y;
	rect3.w = bola.w;
	rect3.h = bola.h;
	SDL_FillRect(screen,&rect3,SDL_MapRGB(screen->format,255,0,0));

}

//Função que move as raquetes
void moveRaquete(int posRaquete, int cimaOUbaixo){
	
	if(posRaquete == 1 && cimaOUbaixo == 0){
		if(raquete[1].y > 480 - raquete[1].h){
			raquete[1].y = 480 - raquete[1].h;
		} 
		else{ 
			raquete[1].y += 3;
		}
	}

	if(posRaquete == 1 && cimaOUbaixo == 1){
		if(raquete[1].y < 0 ){
			raquete[1].y = 0;
		} 
		else{ 
			raquete[1].y -= 3;
		}
	}

	if(posRaquete == 0 && cimaOUbaixo == 0){
		if(raquete[0].y > 480 - raquete[0].h){
			raquete[0].y = 480 - raquete[0].h;
		} 
		else{ 
			raquete[0].y += 3;
		}
	}	
	
	if(posRaquete == 0 && cimaOUbaixo == 1){

		if(raquete[0].y < 0){
			raquete[0].y = 0;
		} 
		else{ 
			raquete[0].y -= 3;
		}
	}	

}

//Função que atualia a tela de jogo
void atualiza(SDL_Surface* screen){
	SDL_Rect src;
	
	src.x = 0;
	src.y = 0;
	src.w = screen->w;
	src.h = screen->h;
	
	int r = SDL_FillRect(screen,&src,0x0);
}

//Função que move a bola
void moveBola(SDL_Surface* screen){
	 
	bola.x += bola.dx;
	bola.y += bola.dy;

	if (bola.x < 28) {	
		jogadores.ponto2 += 1;
		iniciaJogo(screen);
	}

	if (bola.x > 612 ) { 	
		jogadores.ponto1 += 1;
		iniciaJogo(screen);
	}

	if (bola.y < 0 || bola.y > screen->h - 10) {
		
		bola.dy = -bola.dy;
	}
	
	//bater na raquete	
	int i;
	for (i = 0; i < 2; i++) {
		
		int c;
		c = 1;			
		if (bola.x > raquete[i].x + raquete[i].w) {
			c = 0;
		}

		if (bola.x + bola.w < raquete[i].x) {
			c = 0;
		}

		if (bola.y > raquete[i].y + raquete[i].h) {
			c = 0;
		}

		if (bola.y + bola.h < raquete[i].y) {
			c = 0;
		}
			
		//colisão
		if (c == 1){
			bola.dx = - bola.dx;
		
			//bola move para esquerda
			if (bola.dx < 0){
				bola.dx -= 0.5;
			}else{ //bola move para direita	
				bola.dx += 0.5;
			}
					
		}
	}
}

//Função que finaliza
void fim(){
	SDL_Quit(); //encerrar a SDL
	TTF_Quit();	
}
