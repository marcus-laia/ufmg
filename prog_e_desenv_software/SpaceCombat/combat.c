#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>
#include <time.h>

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
//AUDIOS
ALLEGRO_SAMPLE *backgroundMusic = NULL;
ALLEGRO_SAMPLE *gameMusic = NULL;
ALLEGRO_SAMPLE *shooting = NULL;
ALLEGRO_SAMPLE *colisaoGenerica = NULL;
ALLEGRO_SAMPLE *colisaoTanques = NULL;
ALLEGRO_SAMPLE *colisaoTiroTanque = NULL;
ALLEGRO_SAMPLE *optionsNavigation = NULL;
ALLEGRO_SAMPLE *openPause = NULL;
ALLEGRO_SAMPLE *closePause = NULL;
ALLEGRO_SAMPLE *openEndGame = NULL;
ALLEGRO_SAMPLE *closeEndGame = NULL;
//INSTANCIAS DE AUDIO
ALLEGRO_SAMPLE_INSTANCE *inst_backgroundMusic = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_gameMusic = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_shooting = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_colisaoGenerica = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_colisaoTanques = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_colisaoTiroTanque = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_optionsNavigation = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_openPause = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_closePause = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_openEndGame = NULL;
ALLEGRO_SAMPLE_INSTANCE *inst_closeEndGame = NULL;
//FONTES
ALLEGRO_FONT *originTech25 = NULL;
ALLEGRO_FONT *spaceAge25 = NULL;
ALLEGRO_FONT *spaceAge40 = NULL;
//IMAGENS
ALLEGRO_BITMAP *menu = NULL;
ALLEGRO_BITMAP *how_to_play = NULL;
ALLEGRO_BITMAP *choose = NULL;
ALLEGRO_BITMAP *pause = NULL;
ALLEGRO_BITMAP *end = NULL;
ALLEGRO_BITMAP *exitConfirm = NULL;
ALLEGRO_BITMAP *pointer = NULL;
ALLEGRO_BITMAP *selectorP1 = NULL;
ALLEGRO_BITMAP *selectorP2 = NULL;
ALLEGRO_BITMAP *selectorMap = NULL;
ALLEGRO_BITMAP *background = NULL;
ALLEGRO_BITMAP *background2 = NULL;
ALLEGRO_BITMAP *nave1 = NULL;
ALLEGRO_BITMAP *nave2 = NULL;
ALLEGRO_BITMAP *nave3 = NULL;
ALLEGRO_BITMAP *nave4 = NULL;
ALLEGRO_BITMAP *nave5 = NULL;
ALLEGRO_BITMAP *nave6 = NULL;
ALLEGRO_BITMAP *nave7 = NULL;
ALLEGRO_BITMAP *nave8 = NULL;
ALLEGRO_BITMAP *nave9 = NULL;
ALLEGRO_BITMAP *shot = NULL;

const float FPS = 100;
const int SCREEN_W = 960;
const int SCREEN_H = 540;

const float VEL_TANQUE = 2.5;
const float PASSO_ANGULO = M_PI/90;

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;

const float RAIO_CAMPO_FORCA = 40;
const float VEL_TIRO = 8;
const float RAIO_TIRO = 6;

const float THETA = M_PI/4;

int soundON = 1;
int musicON = 1;

typedef struct Ponto
{
	float x, y;
} Ponto;

typedef struct Circulo
{
	Ponto centro;
	float raio;
} Circulo;

typedef struct Tiro 
{
	Circulo corpo;
	int tipo;
	float vel;
	float vel_x, vel_y;
	ALLEGRO_COLOR cor;
	int ativo;
} Tiro;

typedef struct Tanque 
{
	Ponto centro;
	Ponto A, B, C;
	ALLEGRO_COLOR cor;
	ALLEGRO_BITMAP *spaceship;

	float vel;
	float angulo;
	float x_comp, y_comp;
	float vel_angular;

	Circulo campoForca;
	Tiro s;

} Tanque;

typedef struct Bloco
{
	Ponto top_left;
	Ponto bottom_right;
} Bloco;

typedef struct Map
{
	int id;
	Bloco central;
	Bloco extra1;
	Bloco extra2;
	Bloco extra3;
	Bloco extra4;
} Map;

typedef struct Player
{
	int id;
	Tanque tanque;
	int pontos;
} Player;

void soundHandle(int condition, ALLEGRO_SAMPLE_INSTANCE *instance){
	if(condition){
		al_stop_sample_instance(instance);
		al_play_sample_instance(instance);
	}
}

void initMapa(Map *mapa)
{
	switch (mapa->id)
	{
	case 1:
		mapa->central.top_left.x = 380;
		mapa->central.top_left.y = 120;
		mapa->central.bottom_right.x = 580;
		mapa->central.bottom_right.y = 420;		
		break;
	case 2:
		mapa->central.top_left.x = 380;
		mapa->central.top_left.y = 170;
		mapa->central.bottom_right.x = 580;
		mapa->central.bottom_right.y = 370;
		//Esq
		mapa->extra1.top_left.x = 180;
		mapa->extra1.top_left.y = 230;
		mapa->extra1.bottom_right.x = 260;
		mapa->extra1.bottom_right.y = 310;
		//Dir
		mapa->extra2.top_left.x = 700;
		mapa->extra2.top_left.y = 230;
		mapa->extra2.bottom_right.x = 780;
		mapa->extra2.bottom_right.y = 310;
			
		break;
	case 3:
		mapa->central.top_left.x = 405;
		mapa->central.top_left.y = 195;
		mapa->central.bottom_right.x = 555;
		mapa->central.bottom_right.y = 345;
		//supEsq
		mapa->extra1.top_left.x = 140;
		mapa->extra1.top_left.y = 100;
		mapa->extra1.bottom_right.x = 220;
		mapa->extra1.bottom_right.y = 180;			
		//supDir
		mapa->extra2.top_left.x = 740;
		mapa->extra2.top_left.y = 100;
		mapa->extra2.bottom_right.x = 820;
		mapa->extra2.bottom_right.y = 180;			
		//infEsq
		mapa->extra3.top_left.x = 140;
		mapa->extra3.top_left.y = 360;
		mapa->extra3.bottom_right.x = 220;
		mapa->extra3.bottom_right.y = 440;			
		//infDir
		mapa->extra4.top_left.x = 740;
		mapa->extra4.top_left.y = 360;
		mapa->extra4.bottom_right.x = 820;
		mapa->extra4.bottom_right.y = 440;			
		break;
	case 4:
		mapa->central.top_left.x = 400;
		mapa->central.top_left.y = 220;
		mapa->central.bottom_right.x = 560;
		mapa->central.bottom_right.y = 320;
		//esq
		mapa->extra1.top_left.x = 190;
		mapa->extra1.top_left.y = 170;
		mapa->extra1.bottom_right.x = 230;
		mapa->extra1.bottom_right.y = 370;
		//sup
		mapa->extra2.top_left.x = 380;
		mapa->extra2.top_left.y = 25;
		mapa->extra2.bottom_right.x = 580;
		mapa->extra2.bottom_right.y = 65;
		//dir
		mapa->extra3.top_left.x = 730;
		mapa->extra3.top_left.y = 170;
		mapa->extra3.bottom_right.x = 770;
		mapa->extra3.bottom_right.y = 370;
		//inf
		mapa->extra4.top_left.x = 380;
		mapa->extra4.top_left.y = 475;
		mapa->extra4.bottom_right.x = 580;
		mapa->extra4.bottom_right.y = 515;		
		break;
	}
}

void desenhaCenario(Map mapa)
{

	al_draw_filled_rectangle(mapa.central.top_left.x, mapa.central.top_left.y,
							mapa.central.bottom_right.x, mapa.central.bottom_right.y,
							al_map_rgb(255, 255, 255));

	switch (mapa.id)
	{
	case 1:	
		break;
	case 2:		
		al_draw_filled_rectangle(mapa.extra1.top_left.x, mapa.extra1.top_left.y,
							mapa.extra1.bottom_right.x, mapa.extra1.bottom_right.y,
							al_map_rgb(255, 255, 255));

		al_draw_filled_rectangle(mapa.extra2.top_left.x, mapa.extra2.top_left.y,
							mapa.extra2.bottom_right.x, mapa.extra2.bottom_right.y,
							al_map_rgb(255, 255, 255));
			
		break;
	case 3:
		al_draw_filled_rectangle(mapa.extra1.top_left.x, mapa.extra1.top_left.y,
							mapa.extra1.bottom_right.x, mapa.extra1.bottom_right.y,
							al_map_rgb(255, 255, 255));			

		al_draw_filled_rectangle(mapa.extra2.top_left.x, mapa.extra2.top_left.y,
							mapa.extra2.bottom_right.x, mapa.extra2.bottom_right.y,
							al_map_rgb(255, 255, 255));

		al_draw_filled_rectangle(mapa.extra3.top_left.x, mapa.extra3.top_left.y,
							mapa.extra3.bottom_right.x, mapa.extra3.bottom_right.y,
							al_map_rgb(255, 255, 255));

		al_draw_filled_rectangle(mapa.extra4.top_left.x, mapa.extra4.top_left.y,
							mapa.extra4.bottom_right.x, mapa.extra4.bottom_right.y,
							al_map_rgb(255, 255, 255));	
		break;
	case 4:
		al_draw_filled_rectangle(mapa.extra1.top_left.x, mapa.extra1.top_left.y,
							mapa.extra1.bottom_right.x, mapa.extra1.bottom_right.y,
							al_map_rgb(255, 255, 255));		

		al_draw_filled_rectangle(mapa.extra2.top_left.x, mapa.extra2.top_left.y,
							mapa.extra2.bottom_right.x, mapa.extra2.bottom_right.y,
							al_map_rgb(255, 255, 255));

		al_draw_filled_rectangle(mapa.extra3.top_left.x, mapa.extra3.top_left.y,
							mapa.extra3.bottom_right.x, mapa.extra3.bottom_right.y,
							al_map_rgb(255, 255, 255));		

		al_draw_filled_rectangle(mapa.extra4.top_left.x, mapa.extra4.top_left.y,
							mapa.extra4.bottom_right.x, mapa.extra4.bottom_right.y,
							al_map_rgb(255, 255, 255));			
		break;
	}
}

void desenhaTanque(Tanque t) 
{
	al_draw_scaled_rotated_bitmap(t.spaceship,
   								al_get_bitmap_width(t.spaceship)/2, 
								al_get_bitmap_height(t.spaceship)/2,
								t.centro.x, t.centro.y, 
								2*t.campoForca.raio/(al_get_bitmap_width(t.spaceship)-20), 
								2*t.campoForca.raio/(al_get_bitmap_height(t.spaceship)-20),
								t.angulo-M_PI/2, 0);
	
	/* al_draw_scaled_bitmap(t.spaceship,
					0, 0,                                		// source origin
					al_get_bitmap_width(t.spaceship),     		// source width
					al_get_bitmap_height(t.spaceship),    		// source height
					t.centro.x-t.campoForca.raio, t.centro.y-t.campoForca.raio,				// target origin
					2*t.campoForca.raio, 2*t.campoForca.raio,        		// target dimensions
					0                                    		// flags
				); */
	/* al_draw_circle(t.centro.x, t.centro.y, t.campoForca.raio, t.cor, 1);
	al_draw_filled_triangle(t.A.x + t.centro.x, t.A.y + t.centro.y,
							t.B.x + t.centro.x, t.B.y + t.centro.y,
							t.C.x + t.centro.x, t.C.y + t.centro.y,
							t.cor); */
	
	if(t.s.ativo){		
		al_draw_filled_circle(t.s.corpo.centro.x, t.s.corpo.centro.y, t.s.corpo.raio, t.s.cor);
		al_draw_scaled_bitmap(shot,
					0, 0,                                									// source origin
					al_get_bitmap_width(shot),     											// source width
					al_get_bitmap_height(shot),    											// source height
					t.s.corpo.centro.x-t.s.corpo.raio, t.s.corpo.centro.y-t.s.corpo.raio,	// target origin
					2*t.s.corpo.raio, 2*t.s.corpo.raio,        								// target dimensions
					0                                    									// flags
				);
	}
}

void initTiro(Tanque *t, int tipo)
{
	t->s.tipo = tipo;
	switch (t->s.tipo){
		case 1:
			t->s.corpo.raio = RAIO_TIRO;
			t->s.vel = VEL_TIRO;
			t->s.cor = al_map_rgb(rand()%256, rand()%256, rand()%256);
			t->s.ativo = 0;
		break;
	}
}

void initTanque(Player *p)
{
	printf("centro init (%f, %f)", p->tanque.centro.x, p->tanque.centro.y);

	switch (p->id){
		case 1:
			p->tanque.centro.x = SCREEN_W/8;
			p->tanque.centro.y = SCREEN_H/2;
		break;
		case 2:
			p->tanque.centro.x = 7*SCREEN_W/8;
			p->tanque.centro.y = SCREEN_H/2;
		break;
	}
	

	p->tanque.cor = al_map_rgb(rand()%256, rand()%256, rand()%256);

	float h = RAIO_CAMPO_FORCA * cos(THETA);
	float w = RAIO_CAMPO_FORCA * sin(THETA);
	
	p->tanque.A.x = 0;
	p->tanque.A.y = -RAIO_CAMPO_FORCA;
	p->tanque.B.x = -w;
	p->tanque.B.y = h;
	p->tanque.C.x = w;
	p->tanque.C.y = h;

	p->tanque.vel = 0;
	p->tanque. angulo = M_PI/2;
	p->tanque.x_comp = cos(p->tanque.angulo);
	p->tanque.y_comp = sin(p->tanque.angulo);
	p->tanque.vel_angular = 0;

	p->tanque.campoForca.centro = p->tanque.centro;
	p->tanque.campoForca.raio = RAIO_CAMPO_FORCA;

	initTiro(&p->tanque, 1);
}

void initPlayer(Player *p){
	/* switch (p->id){
		case 1: */
			p->pontos=0;
			initTanque(p);
	/*	break;
	} */
}

float distPontos(Ponto a, Ponto b){
    float dist=0;
    dist = (pow((pow(a.x-b.x,2))+(pow(a.y-b.y,2)),0.5));
    return dist;
}

int colisaoCirculos(Circulo c1, Circulo c2){
	if (distPontos(c1.centro,c2.centro)<(c1.raio+c2.raio)){
		return 1;
	}
	return 0;
}

int colisaoCirculoTela(Circulo *c){
	int colisao = 0;
	if (c->centro.y > SCREEN_H - c->raio) {
		c->centro.y = SCREEN_H - c->raio;
		colisao = 1;
	} else if (c->centro.y < c->raio) {
		c->centro.y = c->raio;
		colisao = 1;
	}
	if (c->centro.x > SCREEN_W - c->raio){
		c->centro.x = SCREEN_W - c->raio;
		colisao = 1;
	} else if (c->centro.x < c->raio){
		c->centro.x = c->raio;		
		colisao = 1;
	}

	if (colisao){
		soundHandle(soundON, inst_colisaoGenerica);		
	}

	return colisao;
}

int colisaoBlocoPonto(Bloco b, Ponto p){
    if (p.x>=b.top_left.x && p.x<=b.bottom_right.x && p.y>=b.top_left.y && p.y<=b.bottom_right.y){
        return 1;
    } else {
        return 0;
    }
}

int colisaoCirculoBloco(Bloco b, Circulo *c){
	int colisao = 0;

	Ponto dir, esq, sup, inf;

	dir.x = c->centro.x + c->raio;
	dir.y = c->centro.y;
	esq.x = c->centro.x - c->raio;
	esq.y = c->centro.y;

	sup.x = c->centro.x;
	sup.y = c->centro.y - c->raio;
	inf.x = c->centro.x;
	inf.y = c->centro.y + c->raio;

	if (colisaoBlocoPonto(b,dir)){
		c->centro.x = b.top_left.x - c->raio;
		colisao = 1;
	}
	if (colisaoBlocoPonto(b,esq)){
		c->centro.x = b.bottom_right.x + c->raio;
		colisao = 1;
	}
	if (colisaoBlocoPonto(b,inf)){
		c->centro.y = b.top_left.y - c->raio;
		colisao = 1;
	}
	if (colisaoBlocoPonto(b,sup)){
		c->centro.y = b.bottom_right.y + c->raio;
		colisao = 1;
    }

	Ponto top_right, bottom_left;

	top_right.x = b.bottom_right.x;
    top_right.y = b.top_left.y;
    bottom_left.x = b.top_left.x;
    bottom_left.y = b.bottom_right.y;

	if (
        distPontos(c->centro,b.bottom_right)<=c->raio ||
        distPontos(c->centro,bottom_left)<=c->raio ||
        distPontos(c->centro,top_right)<=c->raio ||
        distPontos(c->centro,b.top_left)<=c->raio
     ){
        colisao = -1;
    }

	if (colisao!=0){
		soundHandle(soundON, inst_colisaoGenerica);
	}

	return colisao;
}

void atira(Tanque *t)
{
	t->s.corpo.centro.x = t->A.x + t->centro.x;
	t->s.corpo.centro.y = t->A.y + t->centro.y;
	//if (!t->s.ativo){
		t->s.vel_x = cos(t->angulo);
		t->s.vel_y = sin(t->angulo);
		t->s.ativo = 1;
	//}
}

void atualizaTiro(Player *p, Map mapa)
{
	if (p->tanque.s.ativo)
	{
		p->tanque.s.corpo.centro.y -= p->tanque.s.vel*p->tanque.s.vel_y;
		p->tanque.s.corpo.centro.x -= p->tanque.s.vel*p->tanque.s.vel_x;

		if (colisaoCirculoTela(&p->tanque.s.corpo) || 
			colisaoCirculoBloco(mapa.central, &p->tanque.s.corpo)!=0)
		{
			p->tanque.s.ativo = 0;
		} 
		else if(mapa.id==2){
			if (colisaoCirculoBloco(mapa.extra1, &p->tanque.s.corpo)!=0 ||
				colisaoCirculoBloco(mapa.extra2, &p->tanque.s.corpo)!=0 )
			{
				p->tanque.s.ativo = 0;
			}
		}
		else if(mapa.id==3 || mapa.id==4){
			if (colisaoCirculoBloco(mapa.extra1, &p->tanque.s.corpo)!=0 ||
				colisaoCirculoBloco(mapa.extra2, &p->tanque.s.corpo)!=0 ||
				colisaoCirculoBloco(mapa.extra3, &p->tanque.s.corpo)!=0 ||
				colisaoCirculoBloco(mapa.extra4, &p->tanque.s.corpo)!=0 )
			{
				p->tanque.s.ativo = 0;
			}
		}
	} else {
		p->tanque.s.corpo.centro.x = p->tanque.A.x + p->tanque.centro.x;
		p->tanque.s.corpo.centro.y = p->tanque.A.y + p->tanque.centro.y;
	}
}

void Rotate(Ponto *P, float Angle){
	float x=P->x,y=P->y;

	P->x=(x*cos(Angle))-(y*sin(Angle));
	P->y=(y*cos(Angle))+(x*sin(Angle));
}

void rotacionaTanque(Tanque *t)
{
	if (t->vel_angular != 0)
	{
		Rotate(&t->A, t->vel_angular);
		Rotate(&t->B, t->vel_angular);
		Rotate(&t->C, t->vel_angular);

		t->angulo += t->vel_angular;
		t->x_comp = cos(t->angulo);
		t->y_comp = sin(t->angulo);
	}
}

void atualizaTanque(Player *p1, Player *p2, Map mapa)
{
	rotacionaTanque(&p1->tanque);

	p1->tanque.campoForca.centro.y += p1->tanque.vel*p1->tanque.y_comp;
	p1->tanque.campoForca.centro.x += p1->tanque.vel*p1->tanque.x_comp;

	colisaoCirculoTela(&p1->tanque.campoForca);

	if (colisaoCirculos(p1->tanque.campoForca,p2->tanque.campoForca))
	{
		soundHandle(soundON, inst_colisaoTanques);
		p1->tanque.campoForca.centro = p1->tanque.centro;
	} else if (colisaoCirculoBloco(mapa.central,&p1->tanque.campoForca) == -1)
	{
		p1->tanque.campoForca.centro = p1->tanque.centro;
	} 
	else if (mapa.id==2) {
		if (colisaoCirculoBloco(mapa.extra1,&p1->tanque.campoForca) == -1 || 
			colisaoCirculoBloco(mapa.extra2,&p1->tanque.campoForca) == -1)
		{
			p1->tanque.campoForca.centro = p1->tanque.centro;
		}
	} 
	else if (mapa.id==3 || mapa.id==4) {
		if (colisaoCirculoBloco(mapa.extra1,&p1->tanque.campoForca) == -1 || 
			colisaoCirculoBloco(mapa.extra2,&p1->tanque.campoForca) == -1 ||
			colisaoCirculoBloco(mapa.extra3,&p1->tanque.campoForca) == -1 || 
			colisaoCirculoBloco(mapa.extra4,&p1->tanque.campoForca) == -1)
		{
			p1->tanque.campoForca.centro = p1->tanque.centro;
		}
	}
	
	p1->tanque.centro = p1->tanque.campoForca.centro;

	atualizaTiro(p1, mapa);

	if(p2->tanque.s.ativo){
		if (colisaoCirculos(p1->tanque.campoForca,p2->tanque.s.corpo)){
			soundHandle(soundON, inst_colisaoTiroTanque);
			p2->tanque.s.ativo = 0;
			p2->pontos++;
		}
	}
}

int randomEntreNums(int menor, int maior){
	return (menor + rand()%(1+maior-menor));
}
//funcoes de menu
void posicionaPointer(int *pointerPos, int *pointer_y){
	if(*pointerPos>3){
		*pointerPos = 1;
	} else if(*pointerPos<1){
		*pointerPos = 3;
	}

	*pointer_y = (*pointerPos)*100 + 18;
}

void posicionaPointerPause(int *pointerPos, int *pointer_y){
	if(*pointerPos>4){
		*pointerPos = 1;
	} else if(*pointerPos<1){
		*pointerPos = 4;
	}

	*pointer_y = ((*pointerPos)-1)*52 + 145;
}

void posicionaSelector(int *pos, int *selectorX, int id){

	if (id!=3){		
		if(*pos>10){
			*pos = 1;
		} else if(*pos<1){
			*pos = 10;
		}
	} else {
		if(*pos>4){
			*pos = 1;
		} else if(*pos<1){
			*pos = 4;
		}
	}

	if (id==1){
		*selectorX = ((*pos)-1)*90 + 24;
	} 
	else if (id==2){
		*selectorX = ((*pos)-1)*90 + 25;
	}
	else if (id==3){
		*selectorX = ((*pos)-1)*200 + 79;
	}
}

int main(int argc, char **argv){
	srand(time(NULL));
	
   
	//----------------------- rotinas de inicializacao ---------------------------------------
    
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
	//inicializa o modulo que permite carregar imagens no jogo
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();
	//inicializa demais addons
	al_init_image_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	al_init_ttf_addon();

	//armazenar as imagens
	menu = al_load_bitmap("images/menu.png");
	how_to_play = al_load_bitmap("images/howtoplay.png");
	choose = al_load_bitmap("images/choose.png");
	pause = al_load_bitmap("images/gamePaused.png");
	end = al_load_bitmap("images/endGame.png");
	exitConfirm = al_load_bitmap("images/exit.png");
	pointer = al_load_bitmap("images/pointer.png");
	selectorP1 = al_load_bitmap("images/selectorP1.png");
	selectorP2 = al_load_bitmap("images/selectorP2.png");
	selectorMap = al_load_bitmap("images/selectorMap.png");
	background = al_load_bitmap("images/background.jpg");
	background2 = al_load_bitmap("images/background2.jpg");
	nave1 = al_load_bitmap("images/nave1.png");
	nave2 = al_load_bitmap("images/nave2.png");
	nave3 = al_load_bitmap("images/nave3.png");
	nave4 = al_load_bitmap("images/nave4.png");
	nave5 = al_load_bitmap("images/nave5.png");
	nave6 = al_load_bitmap("images/nave6.png");
	nave7 = al_load_bitmap("images/nave7.png");
	nave8 = al_load_bitmap("images/nave8.png");
	nave9 = al_load_bitmap("images/nave9.png");
	shot = al_load_bitmap("images/shot.png");
	//armazenas as fontes
	originTech25 = al_load_font("fonts/OriginTech personal use.ttf", 25, 1);
	if(originTech25 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}
	spaceAge40 = al_load_font("fonts/space age.ttf", 40, 1);
	if(spaceAge40 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}
	spaceAge25 = al_load_font("fonts/space age.ttf", 25, 1);
	if(spaceAge25 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *arial18 = al_load_font("fonts/arial.ttf", 18, 1);   
	if(arial18 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}


	//reservar 12 espaços de audio
	al_reserve_samples(22); 
	//armazenar audios
	backgroundMusic = al_load_sample("audios/backgroundMusic.ogg");
	gameMusic = al_load_sample("audios/gameMusic.ogg");
	shooting = al_load_sample("audios/shooting.ogg");
	colisaoGenerica = al_load_sample("audios/colisaoGenerica.ogg");
	colisaoTanques = al_load_sample("audios/colisaoTanques.ogg");
	colisaoTiroTanque = al_load_sample("audios/colisaoTiroTanque.ogg");
	optionsNavigation = al_load_sample("audios/optionsNavigation.ogg");
	openPause = al_load_sample("audios/openPause.ogg");
	closePause = al_load_sample("audios/closePause.ogg");
	openEndGame = al_load_sample("audios/openEndGame.ogg");
	closeEndGame = al_load_sample("audios/closeEndGame.ogg");

	//cria instancias para nao bugar o audio	
	inst_backgroundMusic = al_create_sample_instance(backgroundMusic);
	inst_gameMusic = al_create_sample_instance(gameMusic);
	inst_shooting = al_create_sample_instance(shooting);
	inst_colisaoGenerica = al_create_sample_instance(colisaoGenerica);
	inst_colisaoTanques = al_create_sample_instance(colisaoTanques);
	inst_colisaoTiroTanque = al_create_sample_instance(colisaoTiroTanque);
	inst_optionsNavigation = al_create_sample_instance(optionsNavigation);
	inst_openPause = al_create_sample_instance(openPause);
	inst_closePause = al_create_sample_instance(closePause);
	inst_openEndGame = al_create_sample_instance(openEndGame);
	inst_closeEndGame = al_create_sample_instance(closeEndGame);

	//mixa os audios
	al_attach_sample_instance_to_mixer(inst_backgroundMusic, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_gameMusic, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_shooting, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_colisaoGenerica, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_colisaoTanques, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_colisaoTiroTanque, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_optionsNavigation, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_openPause, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_closePause, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_openEndGame, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(inst_closeEndGame, al_get_default_mixer());

	//coloca em loop ou para tocar apenas uma vez
	al_set_sample_instance_playmode(inst_backgroundMusic, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_playmode(inst_gameMusic, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_playmode(inst_shooting, ALLEGRO_PLAYMODE_ONCE);
	al_set_sample_instance_playmode(inst_colisaoGenerica, ALLEGRO_PLAYMODE_ONCE);
	al_set_sample_instance_playmode(inst_colisaoTanques, ALLEGRO_PLAYMODE_ONCE);
	al_set_sample_instance_playmode(inst_colisaoTiroTanque, ALLEGRO_PLAYMODE_ONCE);
	al_set_sample_instance_playmode(inst_optionsNavigation, ALLEGRO_PLAYMODE_ONCE);
	al_set_sample_instance_playmode(inst_openPause, ALLEGRO_PLAYMODE_ONCE);
	al_set_sample_instance_playmode(inst_closePause, ALLEGRO_PLAYMODE_ONCE);
	al_set_sample_instance_playmode(inst_openEndGame, ALLEGRO_PLAYMODE_ONCE);
	al_set_sample_instance_playmode(inst_closeEndGame, ALLEGRO_PLAYMODE_ONCE);

	//regular volume
	al_set_sample_instance_gain(inst_backgroundMusic, 1.0);
	al_set_sample_instance_gain(inst_gameMusic, 0.4);
	al_set_sample_instance_gain(inst_shooting, 0.8);
	al_set_sample_instance_gain(inst_colisaoGenerica, 0.6);
	al_set_sample_instance_gain(inst_colisaoTanques, 0.8);
	al_set_sample_instance_gain(inst_colisaoTiroTanque, 0.4);
	al_set_sample_instance_gain(inst_optionsNavigation, 0.6);
	al_set_sample_instance_gain(inst_openPause, 1.2);
	al_set_sample_instance_gain(inst_closePause, 1.2);
	al_set_sample_instance_gain(inst_openEndGame, 1.0);
	al_set_sample_instance_gain(inst_closeEndGame, 1.0);

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}

 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//arquivo para salvar vitorias
	FILE *arq;
	int amountP1 = 0, amountP2 = 0;

	arq = fopen("amounts.txt", "r");
	fscanf(arq, "%d %d", &amountP1, &amountP2);
	fclose(arq);

	int pointerPos = 1;
	int pointer_x = 205;
	int pointer_y = 118;

	int pausePointerPos = 1;
	int pausePointer_y = 145;

	int selectorP1pos = 1;
	int selectorP1X = 24;
	int selectorP2pos = 1;
	int selectorP2X = 25;
	int selectorMapPos = 1;
	int selectorMapX = 79;

	int winner = 0;

	int musicHandle_previous = 0;
	int musicHandle = 1;

	Player player_1;
	player_1.id = 1;
	initPlayer(&player_1);
	
	Player player_2;
	player_2.id = 2;
	initPlayer(&player_2);
	
	Map mapa;
	mapa.id = selectorMapPos;
	initMapa(&mapa);
	
	char txtPlacar[15], txtVencedor[10], txtResultado[10];
	char txtTotal1[3], txtTotal2[3], soundState[5], musicState[5];
	char creditos[15], soundControl[25], musicControl[25];
	
	sprintf(creditos, "@marcus.laia");


	//inicia o temporizador
	al_start_timer(timer);

	//1 - Menu
	//2 - Escolha de Naves e Mapa
	//3 - Jogo
	//4 - Jogo Pausado
	//5 - Fim de Jogo
	//6 - Como Jogar
	int playing = 1;
	
	while(playing!=0) {
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);
		
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		else if(ev.type == ALLEGRO_EVENT_TIMER) {

			if (playing == 1){
				
				al_draw_bitmap(menu, 0, 0, 0);
				
				posicionaPointer(&pointerPos, &pointer_y);
				// desenha o ponteiro em escala
				al_draw_scaled_bitmap(pointer,
					0, 0,                                	// source origin
					al_get_bitmap_width(pointer),     		// source width
					al_get_bitmap_height(pointer),    		// source height
					pointer_x, pointer_y,					// target origin
					100, 100,        						// target dimensions
					0                                    	// flags
				);
				
				if(soundON){
					sprintf(soundControl, "Press n to Sound OFF");
				} else {
					sprintf(soundControl, "Press n to Sound ON");
				}
				if(musicON){
					sprintf(musicControl, "Press M to Music OFF");
				} else {
					sprintf(musicControl, "Press M to Music ON");
				}

				al_draw_text(spaceAge25, al_map_rgb(255,255,255), 25, 470, 0, soundControl);
				al_draw_text(spaceAge25, al_map_rgb(255,255,255), 25, 495, 0, musicControl);
				//insere creditos
				al_draw_text(arial18, al_map_rgb(255,255,255), 850, 520, 0, creditos);

				al_flip_display();
			}
			else if(playing==2){
				posicionaSelector(&selectorP1pos, &selectorP1X, 1);
				posicionaSelector(&selectorP2pos, &selectorP2X, 2);
				posicionaSelector(&selectorMapPos, &selectorMapX, 3);

				al_draw_bitmap(choose,0,0,0);
				al_draw_bitmap(selectorP1, selectorP1X,130,0);
				al_draw_bitmap(selectorP2, selectorP2X,419,0);
				al_draw_bitmap(selectorMap,selectorMapX,256,0);

				al_flip_display();
			}
			else if(playing == 3){
				al_draw_bitmap(background, 0, 0, 0);
				desenhaCenario(mapa);
				
				sprintf(txtPlacar, "P1-%d x %d-P2", player_1.pontos, player_2.pontos);
				al_draw_text(originTech25, al_map_rgb(0,0,0), SCREEN_W/2-72, SCREEN_H/2-15, 0, txtPlacar);

				atualizaTanque(&player_1, &player_2, mapa);
				atualizaTanque(&player_2, &player_1, mapa);

				desenhaTanque(player_1.tanque);
				desenhaTanque(player_2.tanque);

				if(player_1.pontos==5){					
					winner = 1;
					amountP1++;
					playing = 5;
					soundHandle(soundON, inst_openEndGame);
				}else if (player_2.pontos==5){
					winner = 2;
					amountP2++;
					playing=5;
					soundHandle(soundON, inst_openEndGame);
				}
				//atualiza a tela (quando houver algo para mostrar)
				al_flip_display();
				
				if(al_get_timer_count(timer)%(int)FPS == 0)
					printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/FPS));
			}
			else if(playing == 4){
				al_draw_bitmap(pause,0,0,0);
				posicionaPointerPause(&pausePointerPos, &pausePointer_y);				
				al_draw_scaled_bitmap(pointer,
					0, 0,                                	// source origin
					al_get_bitmap_width(pointer),     		// source width
					al_get_bitmap_height(pointer),    		// source height
					285, pausePointer_y,					// target origin
					70, 70,        						// target dimensions
					0                                    	// flags
				);
				
				if(soundON){
					sprintf(soundState, "ON");
				} else {
					sprintf(soundState, "OFF");
				}
				if(musicON){
					sprintf(musicState, "ON");
				} else {
					sprintf(musicState, "OFF");
				}				

				al_draw_text(spaceAge40, al_map_rgb(255,255,255), 570, 208, 0, soundState);
				al_draw_text(spaceAge40, al_map_rgb(255,255,255), 550, 265, 0, musicState);

				al_flip_display();
			}
			else if(playing == 5){
				al_draw_bitmap(end,0,0,0);

				sprintf(txtVencedor, "PLAYER %d", winner);
				sprintf(txtResultado, "%d X %d", player_1.pontos, player_2.pontos);
				sprintf(txtTotal1, "%d", amountP1);
				sprintf(txtTotal2, "%d", amountP2);
				
				al_draw_text(spaceAge40, al_map_rgb(255,255,255), 350, 155, 0, txtVencedor);
				al_draw_text(originTech25, al_map_rgb(255,255,255), 425, 215, 0, txtResultado);
				al_draw_text(originTech25, al_map_rgb(255,255,255), 380, 315, 0, txtTotal1);
				al_draw_text(originTech25, al_map_rgb(255,255,255), 380, 350, 0, txtTotal2);

				al_flip_display();
			}
			else if(playing == 6){
				al_draw_bitmap(how_to_play,0,0,0);
				al_flip_display();
			}
			else if(playing == 7){
				al_draw_bitmap(exitConfirm,0,0,0);
				al_flip_display();
			}
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			//imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);

			//criar uma funcao que recebe o tanque e uma variavel do tipo comandos que armazena as teclas do tanque

			switch (ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_W:
					
					if (playing==1){
						break;
					}
					else if(playing==2){
						break;
					} 
					else if(playing==3){
						player_1.tanque.vel -= VEL_TANQUE;
					}

					break;
					
				case ALLEGRO_KEY_S:

					if (playing==1){
						break;
					}
					else if(playing==2){
						break;
					} 
					else if(playing==3){
						player_1.tanque.vel += VEL_TANQUE;
					}

					break;

				case ALLEGRO_KEY_D:

					if (playing==1){
						break;
					}
					else if(playing==2){
						selectorP1pos++;
						soundHandle(soundON, inst_optionsNavigation);
					} 
					else if(playing==3){
						player_1.tanque.vel_angular += PASSO_ANGULO;
					}

					break;

				case ALLEGRO_KEY_A:

					if (playing==1){
						break;
					}
					else if(playing==2){
						selectorP1pos--;
						soundHandle(soundON, inst_optionsNavigation);
					} 
					else if(playing==3){
						player_1.tanque.vel_angular -= PASSO_ANGULO;
					}

					break;

				case ALLEGRO_KEY_Q:

					if (playing==1){
						break;
					}
					else if(playing==2){
						break;
					} 
					else if(playing==3){
						atira(&player_1.tanque);
						soundHandle(soundON, inst_shooting);
					}

					break;
				
				case ALLEGRO_KEY_UP:

					if (playing==1){
						pointerPos--;
						soundHandle(soundON, inst_optionsNavigation);
					}
					else if(playing==2){
						break;
					}
					else if(playing==3){
						player_2.tanque.vel -= VEL_TANQUE;
					}
					else if(playing==4){
						pausePointerPos--;
						soundHandle(soundON, inst_optionsNavigation);
					}


					break;
					
				case ALLEGRO_KEY_DOWN:

					if (playing==1){
						pointerPos++;
						soundHandle(soundON, inst_optionsNavigation);
					}
					else if(playing==2){
						break;
					}
					else if(playing==3){
						player_2.tanque.vel += VEL_TANQUE;
					}
					else if(playing==4){
						pausePointerPos++;
						soundHandle(soundON, inst_optionsNavigation);
					}

					break;

				case ALLEGRO_KEY_RIGHT:

					if (playing==1){
						break;
					}
					else if(playing==2){
						selectorP2pos++;
						soundHandle(soundON, inst_optionsNavigation);
					} 
					else if(playing==3){
						player_2.tanque.vel_angular += PASSO_ANGULO;
					}

					break;

				case ALLEGRO_KEY_LEFT:

					if (playing==1){
						break;
					}
					else if(playing==2){
						selectorP2pos--;
						soundHandle(soundON, inst_optionsNavigation);
					} 
					else if(playing==3){
						player_2.tanque.vel_angular -= PASSO_ANGULO;
					}

					break;

				case ALLEGRO_KEY_SPACE:

					if (playing==1){
						break;
					}
					else if(playing==2){
						selectorMapPos++;
						soundHandle(soundON, inst_optionsNavigation);
						mapa.id = selectorMapPos;
						initMapa(&mapa);
					} 
					else if(playing==3){
						atira(&player_2.tanque);
						soundHandle(soundON, inst_shooting);
					}

					break;

				case ALLEGRO_KEY_ENTER:
									
					if(playing==1){
						switch (pointerPos)
						{
						case 1:
							//reseta a posicao dos selector
							selectorP1pos = 1;
							selectorP2pos = 1;
							selectorMapPos = 1;

							playing=2;
							break;

						case 2:
							playing=6;
							break;

						case 3:
							playing=0;
							break;
						}
					} else if (playing==2){
						//reseta as configuracoes dos players	
						initPlayer(&player_1);
						initPlayer(&player_2);
						//escolha de nave aleatoria
						if (selectorP1pos==10){
							selectorP1pos = randomEntreNums(1,9);
						}	
						if (selectorP2pos==10){
							selectorP2pos = randomEntreNums(1,9);
						}
						//selelciona a nave escolhida por cada player			
						switch (selectorP1pos)
						{
						case 1:
							player_1.tanque.spaceship = nave1;
							break;
						case 2:
							player_1.tanque.spaceship = nave2;
							break;
						case 3:
							player_1.tanque.spaceship = nave3;
							break;
						case 4:
							player_1.tanque.spaceship = nave4;
							break;
						case 5:
							player_1.tanque.spaceship = nave5;
							break;
						case 6:
							player_1.tanque.spaceship = nave6;
							break;
						case 7:
							player_1.tanque.spaceship = nave7;
							break;
						case 8:
							player_1.tanque.spaceship = nave8;
							break;
						case 9:
							player_1.tanque.spaceship = nave9;
							break;
						}

						switch (selectorP2pos)
						{
						case 1:
							player_2.tanque.spaceship = nave1;
							break;
						case 2:
							player_2.tanque.spaceship = nave2;
							break;
						case 3:
							player_2.tanque.spaceship = nave3;
							break;
						case 4:
							player_2.tanque.spaceship = nave4;
							break;
						case 5:
							player_2.tanque.spaceship = nave5;
							break;
						case 6:
							player_2.tanque.spaceship = nave6;
							break;
						case 7:
							player_2.tanque.spaceship = nave7;
							break;
						case 8:
							player_2.tanque.spaceship = nave8;
							break;
						case 9:
							player_2.tanque.spaceship = nave9;
							break;
						}

						playing=3;
					
					} else if(playing==3){
						playing=4;
						soundHandle(soundON, inst_openPause);
					}
					else if(playing==4){
						switch (pausePointerPos)
						{
						case 1:
							playing=3;
							pausePointerPos = 1;
							soundHandle(soundON, inst_closePause);
							break;

						case 2:
							soundON = !soundON;
							break;

						case 3:
							musicON = !musicON;
							break;
						
						case 4:
							playing=1;
							pausePointerPos = 1;
							soundHandle(soundON, inst_closePause);
							break;
						}
					}
					else if(playing==5){
						playing=1;
						soundHandle(soundON, inst_closeEndGame);
					}
					else if(playing==7){
						playing=0;
						soundHandle(soundON, inst_closeEndGame);
					}

					break;

				case ALLEGRO_KEY_R:

					if (playing==2){
						selectorMapPos = randomEntreNums(1,4);
						soundHandle(soundON, inst_optionsNavigation);
						mapa.id = selectorMapPos;
						initMapa(&mapa);			
					}					
					break;

				case ALLEGRO_KEY_ESCAPE:

					if(playing==3){
						playing=4;
						soundHandle(soundON, inst_openPause);
					}
					else if(playing==4){
						playing=3;
						soundHandle(soundON, inst_closePause);
					}
					break;

				case ALLEGRO_KEY_N:

					soundON = !soundON;
					break;

				case ALLEGRO_KEY_M:

					musicON = !musicON;
					break;
			}
		}
		//se o tipo de evento for um soltar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
			//imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);
			
			switch (ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_W:

					if (playing==1){
						break;
					}
					else if(playing==2){
						break;
					} 
					else if(playing==3){
						player_1.tanque.vel += VEL_TANQUE;
					}

					break;
					
				case ALLEGRO_KEY_S:

					if (playing==1){
						break;
					}
					else if(playing==2){
						break;
					} 
					else if(playing==3){
						player_1.tanque.vel -= VEL_TANQUE;
					}

					break;

				case ALLEGRO_KEY_D:

					if (playing==1){
						break;
					}
					else if(playing==2){
						break;
					} 
					else if(playing==3){
						player_1.tanque.vel_angular -= PASSO_ANGULO;
					}

					break;

				case ALLEGRO_KEY_A:

					if (playing==1){
						break;
					}
					else if(playing==2){
						break;
					} 
					else if(playing==3){
						player_1.tanque.vel_angular += PASSO_ANGULO;
					}

					break;

				case ALLEGRO_KEY_UP:

					if (playing==1){
						break;
					}
					else if(playing==2){
						break;
					} 
					else if(playing==3){
						player_2.tanque.vel += VEL_TANQUE;
					}

					break;
					
				case ALLEGRO_KEY_DOWN:

					if (playing==1){
						break;
					}
					else if(playing==2){
						break;
					} 
					else if(playing==3){
						player_2.tanque.vel -= VEL_TANQUE;
					}

					break;

				case ALLEGRO_KEY_RIGHT:

					if (playing==1){
						break;
					}
					else if(playing==2){
						break;
					} 
					else if(playing==3){
						player_2.tanque.vel_angular -= PASSO_ANGULO;
					}

					break;

				case ALLEGRO_KEY_LEFT:

					if (playing==1){
						break;
					}
					else if(playing==2){
						break;
					} 
					else if(playing==3){
						player_2.tanque.vel_angular += PASSO_ANGULO;
					}

					break;

				case ALLEGRO_KEY_ESCAPE:

					if(playing==1){
						playing=7;
						soundHandle(soundON, inst_openEndGame);
					} 
					else if(playing==2){
						playing=1;
					}
					else if(playing==6){
						playing=1;
					} 
					else if(playing==5){
						playing=1;
						soundHandle(soundON, inst_closeEndGame);
					}
					else if(playing==7){
						playing=1;
						soundHandle(soundON, inst_closeEndGame);
					}
					break;

					break;
			}
		}
		if(musicON){
			if(playing == 1 || playing == 2 ||playing ==6){
				al_stop_sample_instance(inst_gameMusic);
				al_play_sample_instance(inst_backgroundMusic);
			}
			else if(playing == 3 || playing == 4 ||playing == 5){
				al_stop_sample_instance(inst_backgroundMusic);
				al_play_sample_instance(inst_gameMusic);
			}
		} else {
			al_stop_sample_instance(inst_gameMusic);
			al_stop_sample_instance(inst_backgroundMusic);
		}	

	} //fim do while

	arq = fopen("amounts.txt", "w");
	fprintf(arq, "%d %d", amountP1, amountP2);
	fclose(arq);
     
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)

	al_destroy_bitmap(menu);
	al_destroy_bitmap(how_to_play);
	al_destroy_bitmap(choose);
	al_destroy_bitmap(pause);
	al_destroy_bitmap(end);
	al_destroy_bitmap(exitConfirm);
	al_destroy_bitmap(pointer);
	al_destroy_bitmap(selectorP1);
	al_destroy_bitmap(selectorP2);
	al_destroy_bitmap(selectorMap);
	al_destroy_bitmap(background);
	al_destroy_bitmap(background2);
	al_destroy_bitmap(nave1);
	al_destroy_bitmap(nave2);
	al_destroy_bitmap(nave3);
	al_destroy_bitmap(nave4);
	al_destroy_bitmap(nave5);
	al_destroy_bitmap(nave6);
	al_destroy_bitmap(nave7);
	al_destroy_bitmap(nave8);
	al_destroy_bitmap(nave9);
	al_destroy_bitmap(shot);

	al_destroy_sample(backgroundMusic);
	al_destroy_sample(gameMusic);
	al_destroy_sample(shooting);
	al_destroy_sample(colisaoGenerica);
	al_destroy_sample(colisaoTanques);
	al_destroy_sample(colisaoTiroTanque);
	al_destroy_sample(optionsNavigation);
	al_destroy_sample(openPause);
	al_destroy_sample(closePause);
	al_destroy_sample(openEndGame);
	al_destroy_sample(closeEndGame);

	al_destroy_sample_instance(inst_backgroundMusic);
	al_destroy_sample_instance(inst_gameMusic);
	al_destroy_sample_instance(inst_shooting);
	al_destroy_sample_instance(inst_colisaoGenerica);
	al_destroy_sample_instance(inst_colisaoTanques);
	al_destroy_sample_instance(inst_colisaoTiroTanque);
	al_destroy_sample_instance(inst_optionsNavigation);
	al_destroy_sample_instance(inst_openPause);
	al_destroy_sample_instance(inst_closePause);
	al_destroy_sample_instance(inst_openEndGame);
	al_destroy_sample_instance(inst_closeEndGame);

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
   
 
	return 0;
}