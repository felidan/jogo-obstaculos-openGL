// @Felipe Dantas (felidan)

#include "stdafx.h"

using namespace cv;
using namespace std;

#define VELOCIDADE_ANIMACAO 10
#define TEXTO_POSITION_X 490
#define TEXTO_POSITION_Y 290

int TRANSLADA_MURO;
int TRANSLADA_BOLA;

char texto1[60];
char texto2[30];
char texto3[30];
int level = 1;

#pragma region Variaveis

GLfloat angulo, observadorX, observadorY, fAspect;
int moveX, moveY;
float x, y;
bool startJogo, viraDireita, viraEsquerda, viraCima, viraBaixo;

#pragma endregion

#pragma region variaveis-objetos

int quad1_cen1 = 20, quad2_cen1 = 20;
int transladaX_quad1 = -100, transladaY_quad1 = -100, transladaZ_quad1;
int rotacionaMesa;
int mesaX = 180, mesaY = 300;

int cen1_transX_muro1, cen1_transX_muro2, cen1_transX_muro3;
int muro1_cen1_X1, muro1_cen1_X2;
int muro2_cen1_X1, muro2_cen1_X2;
int muro3_cen1_X1, muro3_cen1_X2;

int cen2_transX_muro1, cen2_transX_muro2, cen2_transX_muro3;
int muro1_cen2_X1, muro1_cen2_X2;
int muro2_cen2_X1, muro2_cen2_X2;
int muro3_cen2_X1, muro3_cen2_X2;

int cen3_transX_muro1, cen3_transX_muro2, cen3_transX_muro3;
int muro1_cen3_X1, muro1_cen3_X2;
int muro2_cen3_X1, muro2_cen3_X2;
int muro3_cen3_X1, muro3_cen3_X2;

bool direita_m1_cen1 = true;
bool direita_m2_cen1 = true;
bool direita_m3_cen1 = false;

bool direita_cen2 = true;

bool direita_m1_cen3 = true;
bool direita_m2_cen3 = false;
bool direita_m3_cen3 = true;

GLfloat raioBola;

#pragma endregion

#pragma region variaveis-textura

#define texturaSolo_Width 512
#define texturaSolo_Height 512

#define texturaMuro_Width 256
#define texturaMuro_Height 256

static GLubyte texMuro[texturaMuro_Height][texturaMuro_Width][4];
static GLubyte texSolo[texturaSolo_Height][texturaSolo_Width][4];
static GLubyte texPisoSala[texturaSolo_Height][texturaSolo_Width][4];

GLuint tex[5];

#pragma endregion

void SobeNivel(void) {
	TRANSLADA_MURO += 2;
	level++;
	sprintf(texto2, "LEVEL: %d", level);
	sprintf(texto3, "VELOCIDADE X %d", level);
}

void Perdeu(void) {
	TRANSLADA_MURO = 6;
	level = 1;
	sprintf(texto2, "LEVEL: %d", level);
	sprintf(texto3, "VELOCIDADE X %d", level);
}

void ResetJogo(void) {
	x = 0;
	y = -280 + raioBola;

	startJogo = false;
	viraDireita = false;
	viraEsquerda = false;
	viraCima = false;
	viraBaixo = false;
}

void CalculaInterseccao(void) {
	// intersecção laterais da mesa
	if (x + (raioBola * 2) >= 170 || x - (raioBola * 2) <= -170) {
		Perdeu();
		ResetJogo();
	}

	// intersecção dos muros do cenario 1
	// primeiro muro
	if ((y + (raioBola * 2) >= -208 && y + (raioBola * 2) <= -200)) {
		if (x + (raioBola * 2) >= muro2_cen1_X2 + cen1_transX_muro2 && x + (raioBola * 2) <= muro2_cen1_X1 + cen1_transX_muro2) {
			Perdeu();
			ResetJogo();
		}
	}
	// segundo muro
	if ((y + (raioBola * 2) >= -158 && y + (raioBola * 2) <= -150)) {
		if (x + (raioBola * 2) <= (muro1_cen1_X2* -1) + cen1_transX_muro1 && x + (raioBola * 2) >= (muro1_cen1_X1 * -1) + cen1_transX_muro1) {
			Perdeu();
			ResetJogo();
		}
	}
	// terceiro muro
	if ((y + (raioBola * 2) >= -108 && y + (raioBola * 2) <= -100)) {
		if (x + (raioBola * 2) >= muro3_cen1_X2 + cen1_transX_muro3 && x + (raioBola * 2) <= muro3_cen1_X1 + cen1_transX_muro3) {
			Perdeu();
			ResetJogo();
		}
	}

	// intersecção dos muros do cenario 2
	// primeiro muro
	if ((y + (raioBola * 2) >= -50 && y + (raioBola * 2) <= -42)) {
		if (x + (raioBola * 2) >= muro2_cen2_X2 + cen2_transX_muro2 && x + (raioBola * 2) <= muro2_cen2_X1 + cen2_transX_muro2) {
			Perdeu();
			ResetJogo();
		}
	}
	// segundo muro
	if ((y + (raioBola * 2) >= 0 && y + (raioBola * 2) <= 8)) {
		if (x + (raioBola * 2) <= (muro1_cen2_X2* -1) + cen2_transX_muro1 && x + (raioBola * 2) >= (muro1_cen2_X1 * -1) + cen2_transX_muro1) {
			Perdeu();
			ResetJogo();
		}
	}
	// terceiro muro
	if ((y + (raioBola * 2) >= 50 && y + (raioBola * 2) <= 58)) {
		if (x + (raioBola * 2) >= muro3_cen2_X2 + cen2_transX_muro3 && x + (raioBola * 2) <= muro3_cen2_X1 + cen2_transX_muro3) {
			Perdeu();
			ResetJogo();
		}
	}

	// intersecção dos muros do cenario 3
	// primeiro muro
	if ((y + (raioBola * 2) >= 200 && y + (raioBola * 2) <= 208)) {
		if (x + (raioBola * 2) >= muro2_cen3_X2 + cen3_transX_muro2 && x + (raioBola * 2) <= muro2_cen3_X1 + cen3_transX_muro2) {
			Perdeu();
			ResetJogo();
		}
	}
	// segundo muro
	if ((y + (raioBola * 2) >= 150 && y + (raioBola * 2) <= 158)) {
		if (x + (raioBola * 2) <= (muro1_cen3_X2* -1) + cen3_transX_muro1 && x + (raioBola * 2) >= (muro1_cen3_X1 * -1) + cen3_transX_muro1) {
			Perdeu();
			ResetJogo();
		}
	}
	// terceiro muro
	if ((y + (raioBola * 2) >= 100 && y + (raioBola * 2) <= 108)) {
		if (x + (raioBola * 2) >= muro3_cen3_X2 + cen3_transX_muro3 && x + (raioBola * 2) <= muro3_cen3_X1 + cen3_transX_muro3) {
			Perdeu();
			ResetJogo();
		}
	}

	if (y + (raioBola * 2) >= 300) {
		SobeNivel();
		ResetJogo();
	}
}

void DesenhaTextoLinha1(char *string)
{
	glPushMatrix();
	// Posição no universo onde o texto será colocado          
	glRasterPos2f(-TEXTO_POSITION_X, TEXTO_POSITION_Y - (TEXTO_POSITION_Y * 0.08));
	// Exibe caracter a caracter
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *string++);
	glPopMatrix();
}

void DesenhaTextoLinha2(char *string)
{
	glPushMatrix();
	// Posição no universo onde o texto será colocado          
	glRasterPos2f(-TEXTO_POSITION_X, TEXTO_POSITION_Y-15 - (TEXTO_POSITION_Y * 0.08));
	// Exibe caracter a caracter
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *string++);
	glPopMatrix();
}

void DesenhaTextoLinha3(char *string)
{
	glPushMatrix();
	// Posição no universo onde o texto será colocado          
	glRasterPos2f(-TEXTO_POSITION_X, TEXTO_POSITION_Y - 30 - (TEXTO_POSITION_Y * 0.08));
	// Exibe caracter a caracter
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *string++);
	glPopMatrix();
}

#pragma region desenha

void placar(void) {

	glColor3f(0.3f, 0.3f, 0.3f);

	glBegin(GL_QUADS);
	glVertex3f(-980, 300, -700);
	glVertex3f(-500, 300, -700);
	glVertex3f(-500, 550, -700);
	glVertex3f(-980, 550, -700);
	glEnd();
}

void sala(void) {

	glColor3f(0.5f, 0.5f, 1.0f);

	glBegin(GL_QUADS);
	glVertex3f(-400, -200, -700);
	glVertex3f(400, -200, -700);
	glVertex3f(400, 550, -700);
	glVertex3f(-400, 550, -700);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[2]);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-400, -200, -700);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(400, -200, -700);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(400, -500, 700);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-400, -500, 700);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Bola(void) {

	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(raioBola, 20, 20);

}

void Mesa(void) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[1]);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-mesaX, -mesaY, -10);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-mesaX, mesaY, -10);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(mesaX, mesaY, -10);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(mesaX, -mesaY, -10);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void BordaEsquerdaMesa(void) {
	// Borda esquerda 
	glColor3f(0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);
	// teto
	glVertex3f(-mesaX, -mesaY, 0);
	glVertex3f(-mesaX, mesaY, 0);
	glVertex3f(-170, mesaY, 0);
	glVertex3f(-170, -mesaY, 0);

	glEnd();

	glBegin(GL_QUADS);
	// piso
	glVertex3f(-mesaX, -mesaY, -10);
	glVertex3f(-mesaX, mesaY, -10);
	glVertex3f(-170, mesaY, -10);
	glVertex3f(-170, -mesaY, -10);

	glEnd();

	glBegin(GL_QUADS);
	// externa
	glVertex3f(-mesaX, -mesaY, -10);
	glVertex3f(-mesaX, mesaY, -10);
	glVertex3f(-mesaX, mesaY, 0);
	glVertex3f(-mesaX, -mesaY, 0);

	glEnd();

	glBegin(GL_QUADS);
	// interna
	glVertex3f(-170, -mesaY, -10);
	glVertex3f(-170, mesaY, -10);
	glVertex3f(-170, mesaY, 0);
	glVertex3f(-170, -mesaY, 0);

	glEnd();
}

void BordaDireitaMesa(void) {
	// TODO - DIVIDIR EM DOIS QUADRADOS PARA A PORTA DA BOLA
	// Borda direita 
	glColor3f(0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);
	// teto
	glVertex3f(mesaX, -mesaY, 0);
	glVertex3f(mesaX, mesaY, 0);
	glVertex3f(170, mesaY, 0);
	glVertex3f(170, -mesaY, 0);

	glEnd();

	glBegin(GL_QUADS);
	// piso
	glVertex3f(mesaX, -mesaY, -10);
	glVertex3f(mesaX, mesaY, -10);
	glVertex3f(170, mesaY, -10);
	glVertex3f(170, -mesaY, -10);

	glEnd();

	glBegin(GL_QUADS);
	// externa
	glVertex3f(mesaX, -mesaY, -10);
	glVertex3f(mesaX, mesaY, -10);
	glVertex3f(mesaX, mesaY, 0);
	glVertex3f(mesaX, -mesaY, 0);

	glEnd();

	glBegin(GL_QUADS);
	// interna
	glVertex3f(170, -mesaY, -10);
	glVertex3f(170, mesaY, -10);
	glVertex3f(170, mesaY, 0);
	glVertex3f(170, -mesaY, 0);

	glEnd();
}

void BordaCimaMesa(void) {
	// Borda de cima 
	glColor3f(0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);
	// teto
	glVertex3f(-mesaX, mesaY, 0);
	glVertex3f(-mesaX, 290, 0);
	glVertex3f(mesaX, 290, 0);
	glVertex3f(mesaX, mesaY, 0);
	glEnd();

	glBegin(GL_QUADS);
	// piso
	glVertex3f(-mesaX, mesaY, -10);
	glVertex3f(-mesaX, 290, -10);
	glVertex3f(mesaX, 290, -10);
	glVertex3f(mesaX, mesaY, -10);
	glEnd();

	glBegin(GL_QUADS);
	// interna
	glVertex3f(-mesaX, 290, -10);
	glVertex3f(mesaX, 290, -10);
	glVertex3f(mesaX, 290, 0);
	glVertex3f(-mesaX, 290, 0);
	glEnd();

	glBegin(GL_QUADS);
	// externa
	glVertex3f(-mesaX, mesaY, -10);
	glVertex3f(mesaX, mesaY, -10);
	glVertex3f(mesaX, mesaY, 0);
	glVertex3f(-mesaX, mesaY, 0);
	glEnd();
}

void BordaBaixoMesa(void) {
	// Borda de baixo
	glColor3f(0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);
	// teto
	glVertex3f(-mesaX, -mesaY, 0);
	glVertex3f(-mesaX, -290, 0);
	glVertex3f(mesaX, -290, 0);
	glVertex3f(mesaX, -mesaY, 0);
	glEnd();

	glBegin(GL_QUADS);
	// piso
	glVertex3f(-mesaX, -mesaY, -10);
	glVertex3f(-mesaX, -290, -10);
	glVertex3f(mesaX, -290, -10);
	glVertex3f(mesaX, -mesaY, -10);
	glEnd();

	glBegin(GL_QUADS);
	// interna
	glVertex3f(-mesaX, -290, -10);
	glVertex3f(mesaX, -290, -10);
	glVertex3f(mesaX, -290, 0);
	glVertex3f(-mesaX, -290, 0);
	glEnd();

	glBegin(GL_QUADS);
	// externa
	glVertex3f(-mesaX, -mesaY, -10);
	glVertex3f(mesaX, -mesaY, -10);
	glVertex3f(mesaX, -mesaY, 0);
	glVertex3f(-mesaX, -mesaY, 0);
	glEnd();
}

void Cenario1(void) {
	glPushMatrix(); // muro 1
	glTranslated(cen1_transX_muro2, 0, 0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[0]);

	glBegin(GL_QUADS); // fundo
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen1_X1, -200, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen1_X2, -200, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen1_X2, -200, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen1_X1, -200, 10.0);
	glEnd();

	glBegin(GL_QUADS); // frontal
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen1_X1, -208, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen1_X2, -208, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen1_X2, -208, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen1_X1, -208, 10.0);
	glEnd();

	glBegin(GL_QUADS); // teto
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen1_X1, -200, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen1_X2, -200, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen1_X2, -208, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen1_X1, -208, 10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral direita
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen1_X1, -200, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen1_X1, -208, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen1_X1, -208, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen1_X1, -200, -10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral esquerda
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen1_X2, -200, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen1_X2, -208, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen1_X2, -208, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen1_X2, -200, -10.0);
	glEnd();

	glPopMatrix();

	glPushMatrix(); // muro 2
	glTranslated(cen1_transX_muro1, 0, 0);

	glBegin(GL_QUADS); // frontal
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen1_X1, -150, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen1_X2, -150, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen1_X2, -150, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen1_X1, -150, 10.0);
	glEnd();

	glBegin(GL_QUADS); // fundo
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen1_X1, -158, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen1_X2, -158, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen1_X2, -158, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen1_X1, -158, 10.0);
	glEnd();

	glBegin(GL_QUADS); // teto
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen1_X1, -150, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen1_X2, -150, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen1_X2, -158, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen1_X1, -158, 10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral direita
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen1_X1, -158, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen1_X1, -150, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen1_X1, -150, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen1_X1, -158, -10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral esquerda
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen1_X2, -158, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen1_X2, -150, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen1_X2, -150, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen1_X2, -158, -10.0);
	glEnd();

	glPopMatrix();

	glPushMatrix(); // muro 3
	glTranslated(cen1_transX_muro3, 0, 0);

	glBegin(GL_QUADS); // frontal
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen1_X1, -100, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen1_X2, -100, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen1_X2, -100, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen1_X1, -100, 10.0);
	glEnd();

	glBegin(GL_QUADS); // fundo
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen1_X1, -108, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen1_X2, -108, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen1_X2, -108, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen1_X1, -108, 10.0);
	glEnd();

	glBegin(GL_QUADS); // teto
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen1_X1, -100, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen1_X2, -100, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen1_X2, -108, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen1_X1, -108, 10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral direita
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen1_X1, -100, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen1_X1, -108, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen1_X1, -108, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen1_X1, -100, -10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral esquerda
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen1_X2, -100, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen1_X2, -108, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen1_X2, -108, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen1_X2, -100, -10.0);
	glEnd();


	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void Cenario2(void) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[0]);

	glPushMatrix(); // muro 1
	glTranslated(cen2_transX_muro2, 0, 0);

	glBegin(GL_QUADS); // fundo
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen2_X1, -50, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen2_X2, -50, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen2_X2, -50, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen2_X1, -50, 10.0);
	glEnd();

	glBegin(GL_QUADS); // frontal
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen2_X1, -42, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen2_X2, -42, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen2_X2, -42, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen2_X1, -42, 10.0);
	glEnd();

	glBegin(GL_QUADS); // teto
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen2_X1, -50, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen2_X2, -50, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen2_X2, -42, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen2_X1, -42, 10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral direita
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen2_X1, -50, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen2_X1, -42, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen2_X1, -42, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen2_X1, -50, -10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral esquerda
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen2_X2, -50, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen2_X2, -42, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen2_X2, -42, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen2_X2, -50, -10.0);
	glEnd();

	glPopMatrix();

	glPushMatrix(); // muro 2
	glTranslated(cen2_transX_muro1, 0, 0);

	glBegin(GL_QUADS); // frontal
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen2_X1, 0, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen2_X2, 0, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen2_X2, 0, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen2_X1, 0, 10.0);
	glEnd();

	glBegin(GL_QUADS); // fundo
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen2_X1, 8, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen2_X2, 8, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen2_X2, 8, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen2_X1, 8, 10.0);
	glEnd();

	glBegin(GL_QUADS); // teto
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen2_X1, 0, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen2_X2, 0, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen2_X2, 8, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen2_X1, 8, 10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral direita
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen2_X1, 8, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen2_X1, 0, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen2_X1, 0, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen2_X1, 8, -10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral esquerda
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen2_X2, 8, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen2_X2, 0, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen2_X2, 0, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen2_X2, 8, -10.0);
	glEnd();

	glPopMatrix();

	glPushMatrix(); // muro 3
	glTranslated(cen2_transX_muro3, 0, 0);

	glBegin(GL_QUADS); // frontal
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen2_X1, 50, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen2_X2, 50, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen2_X2, 50, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen2_X1, 50, 10.0);
	glEnd();

	glBegin(GL_QUADS); // fundo
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen2_X1, 58, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen2_X2, 58, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen2_X2, 58, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen2_X1, 58, 10.0);
	glEnd();

	glBegin(GL_QUADS); // teto
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen2_X1, 50, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen2_X2, 50, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen2_X2, 58, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen2_X1, 58, 10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral direita
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen2_X1, 50, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen2_X1, 58, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen2_X1, 58, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen2_X1, 50, -10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral esquerda
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen2_X2, 50, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen2_X2, 58, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen2_X2, 58, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen2_X2, 50, -10.0);
	glEnd();

	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
}

void Cenario3(void) {

	glPushMatrix(); // muro 1
	glTranslated(cen3_transX_muro2, 0, 0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex[0]);

	glBegin(GL_QUADS); // fundo
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen3_X1, 200, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen3_X2, 200, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen3_X2, 200, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen3_X1, 200, 10.0);
	glEnd();

	glBegin(GL_QUADS); // frontal
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen3_X1, 208, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen3_X2, 208, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen3_X2, 208, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen3_X1, 208, 10.0);
	glEnd();

	glBegin(GL_QUADS); // teto
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen3_X1, 200, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen3_X2, 200, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen3_X2, 208, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen3_X1, 208, 10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral direita
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen3_X1, 200, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen3_X1, 208, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen3_X1, 208, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen3_X1, 200, -10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral esquerda
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro2_cen3_X2, 200, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro2_cen3_X2, 208, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro2_cen3_X2, 208, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro2_cen3_X2, 200, -10.0);
	glEnd();

	glPopMatrix();

	glPushMatrix(); // muro 2
	glTranslated(cen3_transX_muro1, 0, 0);

	glBegin(GL_QUADS); // frontal
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen3_X1, 150, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen3_X2, 150, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen3_X2, 150, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen3_X1, 150, 10.0);
	glEnd();

	glBegin(GL_QUADS); // fundo
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen3_X1, 158, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen3_X2, 158, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen3_X2, 158, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen3_X1, 158, 10.0);
	glEnd();

	glBegin(GL_QUADS); // teto
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen3_X1, 150, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen3_X2, 150, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen3_X2, 158, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen3_X1, 158, 10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral direita
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen3_X1, 158, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen3_X1, 150, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen3_X1, 150, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen3_X1, 158, -10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral esquerda
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-muro1_cen3_X2, 158, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-muro1_cen3_X2, 150, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-muro1_cen3_X2, 150, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-muro1_cen3_X2, 158, -10.0);
	glEnd();

	glPopMatrix();

	glPushMatrix(); // muro 3
	glTranslated(cen3_transX_muro3, 0, 0);

	glBegin(GL_QUADS); // frontal
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen3_X1, 100, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen3_X2, 100, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen3_X2, 100, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen3_X1, 100, 10.0);
	glEnd();

	glBegin(GL_QUADS); // fundo
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen3_X1, 108, -10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen3_X2, 108, -10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen3_X2, 108, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen3_X1, 108, 10.0);
	glEnd();

	glBegin(GL_QUADS); // teto
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen3_X1, 100, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen3_X2, 100, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen3_X2, 108, 10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen3_X1, 108, 10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral direita
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen3_X1, 100, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen3_X1, 108, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen3_X1, 108, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen3_X1, 100, -10.0);
	glEnd();

	glBegin(GL_QUADS); // lateral esquerda
	glTexCoord2f(0.0, 0.0);
	glVertex3f(muro3_cen3_X2, 100, 10.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(muro3_cen3_X2, 108, 10.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(muro3_cen3_X2, 108, -10.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(muro3_cen3_X2, 100, -10.0);
	glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void Desenha(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	placar();
	sala();

	glPushMatrix();
	
		glRotated(rotacionaMesa, 1, 0, 0);

		Mesa();

		BordaEsquerdaMesa();
		BordaCimaMesa();
		BordaBaixoMesa();
		BordaDireitaMesa();

		Cenario1();
		Cenario2();
		Cenario3();

		glPushMatrix();

			glTranslatef(x, y, 0);
			CalculaInterseccao();

			Bola();

		glPopMatrix();
	
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);

	DesenhaTextoLinha1(texto1);
	DesenhaTextoLinha2(texto2);
	//DesenhaTextoLinha3(texto3);

	glutSwapBuffers();
}

#pragma endregion

#pragma region textura

void processaTexturaMuro(void) {
	int i, j;
	uchar r, g, b;
	Vec3b imagem;

	Mat img = imread("muro3.png", CV_LOAD_IMAGE_COLOR);
	if (img.empty()) {
		printf("Falha ao carregar o arquivo do disco!\n");
	}

	for (i = 0; i < texturaMuro_Height; i++)
	{
		for (j = 0; j < texturaMuro_Width; j++)
		{
			imagem = img.at<Vec3b>(i, j);

			texMuro[i][j][0] = (GLubyte)imagem.val[2];
			texMuro[i][j][1] = (GLubyte)imagem.val[1];
			texMuro[i][j][2] = (GLubyte)imagem.val[0];
			texMuro[i][j][3] = (GLubyte)255;
		}
	}
}

void processaTexturaSolo(void) {
	int i, j;
	uchar r, g, b;
	Vec3b imagem;

	Mat img = imread("chao.jpg", CV_LOAD_IMAGE_COLOR);
	if (img.empty()) {
		printf("Falha ao carregar o arquivo do disco!\n");
	}

	for (i = 0; i < texturaSolo_Height; i++)
	{
		for (j = 0; j < texturaSolo_Width; j++)
		{
			imagem = img.at<Vec3b>(i, j);

			texSolo[i][j][0] = (GLubyte)imagem.val[2];
			texSolo[i][j][1] = (GLubyte)imagem.val[1];
			texSolo[i][j][2] = (GLubyte)imagem.val[0];
			texSolo[i][j][3] = (GLubyte)255;
		}
	}
}

void processaTexturaQuadriculado(void) {
	int i, j;
	uchar r, g, b;
	Vec3b imagem;

	Mat img = imread("quadriculado.jpg", CV_LOAD_IMAGE_COLOR);
	if (img.empty()) {
		printf("Falha ao carregar o arquivo do disco!\n");
	}

	for (i = 0; i < texturaSolo_Height; i++)
	{
		for (j = 0; j < texturaSolo_Width; j++)
		{
			imagem = img.at<Vec3b>(i, j);

			texPisoSala[i][j][0] = (GLubyte)imagem.val[2];
			texPisoSala[i][j][1] = (GLubyte)imagem.val[1];
			texPisoSala[i][j][2] = (GLubyte)imagem.val[0];
			texPisoSala[i][j][3] = (GLubyte)255;
		}
	}
}

void textura(void) {

	processaTexturaMuro();
	processaTexturaSolo();
	processaTexturaQuadriculado();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(4, tex);


	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texturaMuro_Width, texturaMuro_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texMuro);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texturaSolo_Width, texturaSolo_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texSolo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, tex[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texturaSolo_Width, texturaSolo_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texPisoSala);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

#pragma endregion

#pragma region iluminação

void iluminacao(void) {
	GLfloat luzAmbiente[4] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat luzDifusa[4] = { 1.0, 1.0, 1.0, 1.0 }; // cor
	GLfloat luzEspecular[4] = { 1.0, 1.0, 1.0, 1.0 }; // brilho

	GLfloat posicaoLuz1[4] = { 130, 250, 100.0, 1.0 };
	GLfloat posicaoLuz2[4] = { -130, 250, 100.0, 1.0 };
	GLfloat posicaoLuz3[4] = { -130, 250, -100.0, 1.0 };
	GLfloat posicaoLuz4[4] = { 130, 250, -100.0, 1.0 };

	GLfloat especularidade[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLint especMaterial = 60;

	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
	glMateriali(GL_FRONT, GL_SHININESS, especMaterial);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz1);

	glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT1, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz2);

	glLightfv(GL_LIGHT2, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT2, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT2, GL_POSITION, posicaoLuz3);

	glLightfv(GL_LIGHT3, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT3, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT3, GL_POSITION, posicaoLuz4);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);

	// habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);
}

#pragma endregion

void Inicializa(void) {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	#pragma region set-variaveis

	angulo = 45;
	observadorX = 0;
	observadorY = 0;
	rotacionaMesa = -45;
	raioBola = 5;
	x = 0;
	y = -280 + raioBola;

	muro1_cen1_X1 = 120;
	muro1_cen1_X2 = 20;
	muro2_cen1_X1 = 140;
	muro2_cen1_X2 = 40;
	muro3_cen1_X1 = 100;
	muro3_cen1_X2 = 0;

	muro1_cen2_X1 = 170;
	muro1_cen2_X2 = 70;
	muro2_cen2_X1 = 170;
	muro2_cen2_X2 = 70;
	muro3_cen2_X1 = 170;
	muro3_cen2_X2 = 70;


	muro1_cen3_X1 = 170;
	muro1_cen3_X2 = 70;
	muro2_cen3_X1 = 50;
	muro2_cen3_X2 = -50;
	muro3_cen3_X1 = 110;
	muro3_cen3_X2 = 10;

	cen1_transX_muro1 = 0;
	cen1_transX_muro2 = 0;
	cen1_transX_muro3 = 0;

	cen2_transX_muro1 = 0;
	cen2_transX_muro2 = 0;
	cen2_transX_muro3 = 0;

	cen3_transX_muro1 = 0;
	cen3_transX_muro2 = 0;
	cen3_transX_muro3 = 0;

	startJogo = false;
	viraDireita = false;
	viraEsquerda = false;
	viraCima = false;
	viraBaixo = false;
	
	#pragma endregion

	TRANSLADA_MURO = 6;
	TRANSLADA_BOLA = 6;

	strcpy(texto1, "LABIRINTO - FELIPE  DANTAS    @CG - FEI");
	strcpy(texto2, "LEVEL: 1");
	strcpy(texto3, "VELOCIDADE X 1");

	textura();
	iluminacao();

}

void EspecificaParametrosVisualizacao(void) {
	// sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// projeção de perspectiva
	gluPerspective(angulo, fAspect, 0.1, 5000);

	// sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Onde estou, olhando para onde
	gluLookAt(observadorX, observadorY, 750, 0, 0, 0, 0, 1, 0);
}

void AjustaTamanhoJanela(GLsizei w, GLsizei h) {
	if (h == 0) h = 1;

	// tamanho da viewport
	glViewport(0, 0, w, h);

	// correção de aspecto
	fAspect = (GLfloat)w / (GLfloat)h;

	EspecificaParametrosVisualizacao();
}

void TeclasEspeciais(int key, int x, int y)
{
	printf("#Teclas especiais\n");

	if (key == GLUT_KEY_RIGHT) {
		printf("   #RIGHT\n\n");
		viraCima = false;
		viraBaixo = false;
		viraDireita = true;
		viraEsquerda = false;
	}
	if (key == GLUT_KEY_LEFT) {
		printf("   #RIGHT\n\n");
		viraCima = false;
		viraBaixo = false;
		viraDireita = false;
		viraEsquerda = true;
	}
	if (key == GLUT_KEY_UP) {
		printf("   #UP\n\n");
		viraCima = true;
		viraBaixo = false;
		viraDireita = false;
		viraEsquerda = false;
	}
	if (key == GLUT_KEY_DOWN) {
		printf("   #DOWN\n\n");
		viraCima = false;
		viraBaixo = true;
		viraDireita = false;
		viraEsquerda = false;
	}

	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
}

void Teclado(unsigned char key, int x, int y) {

	switch (key)
	{
	case ' ': // start / pause
		startJogo = !startJogo;
		viraCima = !viraCima;
		break;
	case'W':
	case'w':
		observadorY -= 6;
		break;
	case'A':
	case'a':
		observadorX += 6;
		break;
	case'D':
	case'd':
		observadorX -= 6;
		break;
	case'S':
	case's':
		observadorY += 6;
		break;
	case'8':
		rotacionaMesa -= 2;
		break;
	case'2':
		rotacionaMesa += 2;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void jogar(int value) {

	if (startJogo) {
		if (viraDireita)
			x += TRANSLADA_BOLA;
		if (viraEsquerda)
			x -= TRANSLADA_BOLA;
		if (viraCima)
			y += TRANSLADA_BOLA;
		if (viraBaixo)
			y -= TRANSLADA_BOLA;
	}

	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
	glutTimerFunc(VELOCIDADE_ANIMACAO, jogar, 1);
}

void AnimaCenarios(int value) {

	if (direita_cen2) {
		if (cen2_transX_muro1 <= 240 - TRANSLADA_MURO) {
			cen2_transX_muro1 += TRANSLADA_MURO;
			cen2_transX_muro2 -= TRANSLADA_MURO;
			cen2_transX_muro3 -= TRANSLADA_MURO;
		}
		else {
			direita_cen2 = !direita_cen2;
		}
	}
	else {
		if (cen2_transX_muro1 >= 0 + TRANSLADA_MURO) {
			cen2_transX_muro1 -= TRANSLADA_MURO;
			cen2_transX_muro2 += TRANSLADA_MURO;
			cen2_transX_muro3 += TRANSLADA_MURO;
		}
		else {
			direita_cen2 = !direita_cen2;
		}
	}

	if (direita_m1_cen1) {
		if (cen1_transX_muro1 <= 190 - TRANSLADA_MURO) {
			cen1_transX_muro1 += TRANSLADA_MURO;
		}
		else {
			direita_m1_cen1 = !direita_m1_cen1;
		}
	}
	else {
		if (cen1_transX_muro1 >= -50 + TRANSLADA_MURO) {
			cen1_transX_muro1 -= TRANSLADA_MURO;
		}
		else {
			direita_m1_cen1 = !direita_m1_cen1;
		}
	}

	if (direita_m2_cen1) {
		if (cen1_transX_muro2 <= 30 - TRANSLADA_MURO) {
			cen1_transX_muro2 += TRANSLADA_MURO;
		}
		else {
			direita_m2_cen1 = !direita_m2_cen1;
		}
	}
	else {
		if (cen1_transX_muro2 >= -210 + TRANSLADA_MURO) {
			cen1_transX_muro2 -= TRANSLADA_MURO;
		}
		else {
			direita_m2_cen1 = !direita_m2_cen1;
		}
	}

	if (direita_m3_cen1) {
		if (cen1_transX_muro3 <= 70 - TRANSLADA_MURO) {
			cen1_transX_muro3 += TRANSLADA_MURO;
		}
		else {
			direita_m3_cen1 = !direita_m3_cen1;
		}
	}
	else {
		if (cen1_transX_muro3 >= -170 + TRANSLADA_MURO) {
			cen1_transX_muro3 -= TRANSLADA_MURO;
		}
		else {
			direita_m3_cen1 = !direita_m3_cen1;
		}
	}

	if (direita_m1_cen3) {
		if (cen3_transX_muro1 <= 240 - TRANSLADA_MURO) {
			cen3_transX_muro1 += TRANSLADA_MURO;
		}
		else {
			direita_m1_cen3 = !direita_m1_cen3;
		}
	}
	else {
		if (cen3_transX_muro1 >= 0 + TRANSLADA_MURO) {
			cen3_transX_muro1 -= TRANSLADA_MURO;
		}
		else {
			direita_m1_cen3 = !direita_m1_cen3;
		}
	}

	if (direita_m2_cen3) {
		if (cen3_transX_muro2 <= 120 - TRANSLADA_MURO) {
			cen3_transX_muro2 += TRANSLADA_MURO;
		}
		else {
			direita_m2_cen3 = !direita_m2_cen3;
		}
	}
	else {
		if (cen3_transX_muro2 >= -120 + TRANSLADA_MURO) {
			cen3_transX_muro2 -= TRANSLADA_MURO;
		}
		else {
			direita_m2_cen3 = !direita_m2_cen3;
		}
	}

	if (direita_m3_cen3) {
		if (cen3_transX_muro3 <= 60 - TRANSLADA_MURO) {
			cen3_transX_muro3 += TRANSLADA_MURO;
		}
		else {
			direita_m3_cen3 = !direita_m3_cen3;
		}
	}
	else {
		if (cen3_transX_muro3 >= -180 + TRANSLADA_MURO) {
			cen3_transX_muro3 -= TRANSLADA_MURO;
		}
		else {
			direita_m3_cen3 = !direita_m3_cen3;
		}
	}

	EspecificaParametrosVisualizacao();
	glutPostRedisplay();
	glutTimerFunc(VELOCIDADE_ANIMACAO, AnimaCenarios, 1);

}

int main(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 700);
	glutCreateWindow("FELIPE - PROJETO CG");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AjustaTamanhoJanela);
	glutSpecialFunc(TeclasEspeciais);
	glutKeyboardFunc(Teclado);
	glutTimerFunc(VELOCIDADE_ANIMACAO, jogar, 1);
	glutTimerFunc(VELOCIDADE_ANIMACAO, AnimaCenarios, 1);
	Inicializa();
	glutMainLoop();
}