typedef struct{
	float x,y;
	float w,h; 
	float dx,dy;
}BOLA;

typedef struct{
	int x,y;
	int w,h;
}RAQUETE;

typedef struct{
	char nome1[20];
	int ponto1;
	char nome2[20];	
	int ponto2;
}JOGADORES;

BOLA bola;		
RAQUETE raquete[2];
JOGADORES jogadores;

//##########Funções do programa###########
void iniciaJogo(SDL_Surface* screen);
void desenhaRaquete(SDL_Surface* screen);
void desenhaBola(SDL_Surface* screen);
void fim();
void moveRaquete(int, int);
void atualiza(SDL_Surface* screen);
void moveBola(SDL_Surface* screen);
void salvarNovoJogo(FILE*arq);
void mostrarTodosOsPlacares(FILE*arq);
void delay();
void lerNomes();
void desenha_texto(char* texto, SDL_Surface* dst, int x, int y);
void desenhaLinha(SDL_Surface*screen);
//######################################
