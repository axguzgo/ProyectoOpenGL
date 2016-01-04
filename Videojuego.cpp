//Axel Guzman Godia

#include <iostream>
#include <sstream>
#include <GL/freeglut.h>
#include "Utilidades.h"
#include <GL/FreeImage.h>

#define tasaFPS 30

//Variables de modo
int modo=1;			//Si modo=-1 se pinta todo en modelo alambrico
int niebla =1;		//Si niebla=-1; niebla desactivada
int luces =-1;		//Si luces=-1; modo diurno
int largas = -1;
double zsuelo;
double metros=0;
//Variable velocimetro
static double anguloVelocidad=0;//Posicion inicial en 0 km/h
GLUquadric* cilindro;
static int distancia;
//Inicializacion variables
static float velocidad = 0.0;
float angulo ;
static float acc=0;
//vector texturas
static GLuint tex[30];
//Lista cartel
static GLuint cartel;

//Variables de la camara
float mirar[3]={0,0,-1};
static GLfloat posicion[3]={0,1,0};
//Orientacion y posicion de las farolas
static GLfloat F1[] = { 0 , 4, 0, 1 };
static GLfloat F2[] = { 0 , 4, 0, 1 };
static GLfloat F3[] = { 0 , 4, 0, 1 };
static GLfloat F4[] = { 0 , 4, 0, 1 };
static GLfloat orientacionF[] = { 0 , -1.0,-0.2};
//Geometría objetos farola
static GLfloat S1[] = { 0 , 0, 0, 0 };
static GLfloat S2[] = { 0 , 0, 0, 0 };
static int posFarola1=10;
static int posFarola2=30;
//Vectores para la posicion de los anuncios
static GLfloat C1[] = { 0 , 0, 0, 0 };
static GLfloat C2[] = { 0 , 0, 0, 0 };
static int posCartel1=21;
static GLfloat C3[] = { 0 , 0, 0, 0 };
static GLfloat C4[] = { 0 , 0, 0, 0 };
static int posCartel2=41;

//Calculamos la normal para cada punto
void normal(float x, float y, float z){
                float vectorx = y * posicion[2] - z;
                float vectory = z * posicion[0] - x * posicion[2];        
                float vectorz = x - y *posicion[0];
                float l = sqrt(vectorx*vectorx+vectory*vectory+vectorz*vectorz );
                float normalVX= vectorx / l;
                float normalVY= vectory / l;
                float normalVZ= vectorz / l;
                glNormal3f(normalVX,normalVY,normalVZ);
}


void loadImgFile(char* nombre)
// Uso de FreeImage para cargar la imagen en cualquier formato
// nombre: nombre del fichero con extensión en el mismo directorio que el proyecto
// o con su path completo
{
// Detección del formato, lectura y conversion a BGRA
 FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(nombre,0);
FIBITMAP* imagen = FreeImage_Load(formato, nombre);
FIBITMAP* imagen32b = FreeImage_ConvertTo32Bits(imagen);
// Lectura de dimensiones y colores
int w = FreeImage_GetWidth(imagen32b);
int h = FreeImage_GetHeight(imagen32b);
GLubyte* texeles = FreeImage_GetBits(imagen32b);
// Carga como textura actual
glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, texeles);
// Liberar recursos
FreeImage_Unload(imagen);
FreeImage_Unload(imagen32b);
}


void loadTexture()
// Funcion de carga de texturas e inicializacion
{
//1a. Generar un objeto textura
glGenTextures(30,tex);
//1b. Activar el objeto textura
glBindTexture(GL_TEXTURE_2D,tex[0]);
//1c. Cargar la imagen que servira de textura
loadImageFile("carretera.jpe");
//1b. Activar el objeto textura
glBindTexture(GL_TEXTURE_2D,tex[1]);
//1c. Cargar la imagen que servira de textura
loadImageFile("Paisaje3.jpg");
//Activar textura 3
glBindTexture(GL_TEXTURE_2D,tex[2]);
//1c. Cargar la imagen que servira de textura
loadImageFile("DOMINATOR.gif");
//Activar textura 4
glBindTexture(GL_TEXTURE_2D,tex[3]);
//1c. Cargar la imagen que servira de textura
loadImageFile("defqon1.jpg");
//Activar textura 5
glBindTexture(GL_TEXTURE_2D,tex[4]);
//1c. Cargar la imagen que servira de textura
loadImageFile("aston-martin.gif");
//Activar textura 6
glBindTexture(GL_TEXTURE_2D,tex[5]);
//1c. Cargar la imagen que servira de textura
loadImageFile("grass.jpg");
//Activar textura 7
glBindTexture(GL_TEXTURE_2D,tex[6]);
//1c. Cargar la imagen que servira de textura
loadImageFile("velocimetroDia.png");
glBindTexture(GL_TEXTURE_2D,tex[7]);
//1c. Cargar la imagen que servira de textura
loadImageFile("AgujaDia.png");
glBindTexture(GL_TEXTURE_2D,tex[8]);
//1c. Cargar la imagen que servira de textura
loadImageFile("velocimetroNoche.png");
glBindTexture(GL_TEXTURE_2D,tex[9]);
//1c. Cargar la imagen que servira de textura
loadImageFile("AgujaNoche.png");


//

//1d. Habilitar las texturas
glEnable(GL_TEXTURE_2D);
}

//FAROLAS
void calcularFarolas(){
    float aux;
        for(int i= 0; i<4; i++)
        {
                aux = 5*sin(((i*10))*(PI/25));
                switch(i){
                        case 0:
                                F1[0] = aux;        
                                F1[2]=-i*10;
                                break;
                        case 1:
                                F2[0] = aux;        
                                F2[2]=-i*10;
                                break;
                        case 2:
                                F3[0] = aux;
                                F3[2]=-i*10;
                                break;
                        case 3:
                                F4[0] = aux;
                                F4[2]=-i*10;
                                break;
                }
        }
}

void colocarFarolas(){
        glLightfv(GL_LIGHT3, GL_POSITION, F1);
        glLightfv(GL_LIGHT4, GL_POSITION, F2);
        glLightfv(GL_LIGHT5, GL_POSITION, F3);
        glLightfv(GL_LIGHT6, GL_POSITION, F4);
        
		
		

        glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, orientacionF);
        glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, orientacionF);
        glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, orientacionF);
        glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, orientacionF);
}

void recalcularFarolas(){
        if(posicion[2] < F2[2]){
                for(int indice = 0 ; indice<4; indice++){                        
                        F1[indice]=F2[indice]; 
                        F2[indice]=F3[indice]; 
                        F3[indice]=F4[indice];
                }
                F4[0]=5*sin(((30+posicion[2]*-1))*(PI/25));
                F4[2]=posicion[2]-30;
        }
}

void cargaLuces(){
    // Inicialización de las luces
    glEnable(GL_LIGHTING);

    /***LUNA***/
    GLfloat Al0[]={0.05,0.05,0.05,0.5};//ambiental
    GLfloat Dl0[]={0.05,0.05,0.05,0.05};//difusa
    GLfloat Sl0[]={0.0,0.0,0.0,0.0};//especular

    //CARACTERIZAMOS LA LUZ
    glLightfv(GL_LIGHT0, GL_AMBIENT, Al0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Dl0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Sl0); 
    glEnable(GL_LIGHT0);           

    /***LUCES DEL VEHICULO***/
	
	//Luz izquierda
    GLfloat Al1[]={0.2,0.1,0.2,0.2};//ambiental
    GLfloat Dl1[]={1.0,1.0,1.0,1.0};//difusa
    GLfloat Sl1[]={0.3,0.3,0.3,0.3};//especular

    //CARACTERIZAMOS LA LUZ
    glLightfv(GL_LIGHT1, GL_AMBIENT, Al1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, Dl1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, Sl1);

    //Definimos el angulo y el exponente
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0);//angulo
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);//exponente
    glEnable(GL_LIGHT1);

    //Luz derecha
    glLightfv(GL_LIGHT7, GL_AMBIENT, Al1);
    glLightfv(GL_LIGHT7, GL_DIFFUSE, Dl1);
    glLightfv(GL_LIGHT7, GL_SPECULAR, Sl1);

    //Definimos el angulo y el exponente
    glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 25.0);//angulo
    glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 20.0);//exponente
    glEnable(GL_LIGHT7);

    /***FAROLAS***/
    GLfloat Af[]={0.0,0.0,0.0};//ambiental
    GLfloat Df[]={0.5,0.5,0.2};//difusa
    GLfloat Sf[]={0.0,0.0,0.0};//especular

    //FAROLA 1 (light3)
    glLightfv(GL_LIGHT3, GL_AMBIENT, Af);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, Df);
    glLightfv(GL_LIGHT3, GL_SPECULAR, Sf);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10.0);
    glEnable(GL_LIGHT3);

    //FAROLA 2 (light4)
    glLightfv(GL_LIGHT4, GL_AMBIENT, Af); 
    glLightfv(GL_LIGHT4, GL_DIFFUSE, Df);
    glLightfv(GL_LIGHT4, GL_SPECULAR, Sf);
    glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 10.0);
    glEnable(GL_LIGHT4);

    //FAROLA 3 (light5)
    glLightfv(GL_LIGHT5, GL_AMBIENT, Af); 
    glLightfv(GL_LIGHT5, GL_DIFFUSE, Df);
    glLightfv(GL_LIGHT5, GL_SPECULAR, Sf);
    glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 10.0);
    glEnable(GL_LIGHT5);

    //FAROLA 4 (light6)
    glLightfv(GL_LIGHT6, GL_AMBIENT, Af); 
    glLightfv(GL_LIGHT6, GL_DIFFUSE, Df);
    glLightfv(GL_LIGHT6, GL_SPECULAR, Sf);
    glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 10.0);
    glEnable(GL_LIGHT6);

    //calculamos la posicion inicial de las farolas
    calcularFarolas();
}

//CARRETERA
void carretera(){
	
        //Posicion en el eje de las y
        GLint posicionZ = (int) posicion[2];
        //Calculo del primer punto del camino
        GLfloat p1 = 5*sin(((-1*posicionZ))*(PI/25));
        
        //Vertices que usaremos en la version quad {x,y,z}
        GLfloat pos1[3];
        GLfloat pos2[3];
        GLfloat pos3[3];
        GLfloat pos4[3];

        //Calculo de la normal en cada punto (tz,tx)
        GLfloat tx = (PI/5)*cos(-1*posicionZ*(PI/25));
        GLfloat tz = -1;
        
        //Definimos los valores dentro de pos1
        pos1[0]=p1+tz*2;
        pos1[1]=0;
        pos1[2]=-(-1+((posicionZ*-1))+tx*2);
        //Definimos los valores dentro de pos2
        pos2[0]=p1-tz*2;
        pos2[1]=0;
        pos2[2]=-(-1+((posicionZ*-1))-tx*2);

        GLfloat x;
        for(int i=0;i<50; i++){
                x = 5*sin((i+(posicionZ*-1))*(PI/25));
                tx = (PI/5)*cos(-1*posicionZ*(PI/25));

                /*CARRETERA*/
                //Definimos los valores dentro de pos3                
                pos3[0]=x-tz*2;
                pos3[1]=0;
                pos3[2]=-((i+(posicionZ*-1))-tx*2);
                //Definimos los valores dentro de pos4
                pos4[0]=x+tz*2;
                pos4[1]=0;
                pos4[2]=-((i+(posicionZ*-1))+tx*2);


				

                quad(pos1,pos2,pos3,pos4,20,20);
				
                //donde estaba pos4 ahora tendremos a pos1 y donde estaba pos3 ahora estara pos2
                pos1[0]=pos4[0];
                pos1[1]=pos4[1];
                pos1[2]=pos4[2];

                pos2[0]=pos3[0];
                pos2[1]=pos3[1];
                pos2[2]=pos3[2];
        }
}

void seleccionar_textura(GLuint textura){
	if(modo<0){glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_FOG);}

	else{ glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		  glEnable(GL_TEXTURE_2D);


	glBindTexture(GL_TEXTURE_2D,textura);
	//2b. Definir como se aplicará la textura en ese objeto
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	if(luces<0){glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);}
	else{glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);}}
}
//Carteles
void calcular_cartel(GLfloat C1[],GLfloat C2[],double zcartel){
	double posicionZ=(int)posicion[2];
	    //Calculo de la normal en cada punto (tz,tx)
        GLfloat tx;
        GLfloat tz=-1;
		double i =zcartel;

        double x = 5*sin((i)*(PI/25.0));
        tx = (PI/5.0)*cos(-1*i*(PI/25.0));

        //Definimos la posicion de los soportes             
        C1[0]=x-tz*2;
        C1[1]=0;
        C1[2]=-((i+(posicionZ*-1))-tx*2);
        ////Definimos la posicion de los soportes
        C2[0]=x+tz*2;
        C2[1]=0;
        C2[2]=-((i+(posicionZ*-1))+tx*2);

};
void colocar_cartel(GLfloat C1[],GLfloat C2[],GLuint textura){
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.0,0.0,0.0);
	//Soportes cartel 1
	glPushMatrix();
		glTranslatef(C1[0],C1[1],C1[2]);
		glRotatef(90,-1,0,0);
		glutSolidCylinder(0.1,3,12,12);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(C2[0],C2[1],C2[2]);
		glRotatef(90,-1,0,0);
		glutSolidCylinder(0.1,3,12,12);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	//Seleccionar textura cartel 
	seleccionar_textura(textura);

	//Vertices para generar cartel
	GLfloat a[3];
	GLfloat b[3];
	GLfloat c[3];
	GLfloat d[3];

	//Dar valor a los vertices
	a[0]=C2[0];
	a[1]=2;
	a[2]=C2[2];
	
	b[0]=C1[0];
	b[1]=2;
	b[2]=C1[2];
	
	c[0]=C1[0];
	c[1]=3;
	c[2]=C1[2];
	
	d[0]=C2[0];
	d[1]=3;
	d[2]=C2[2];

	//Pintar cartel con resolucion 20X20
	quad(a,b,c,d,20,20);
	
	
	
}
void colocarCarteles(){

	colocar_cartel(C1,C2,tex[2]);
	colocar_cartel(C3,C4,tex[3]);

}

void recalcularCarteles(){
	double x,tx;
	GLfloat tz;
        if(posicion[2] < -posCartel1){
				posCartel1=posCartel1+40;
				tz=-1;
				x = 5*sin((posCartel1)*(PI/25));
				tx = (PI/5)*cos(-1*posCartel1*(PI/25.0));
                C1[0]=x-tz*2;
                C1[2]=-((posCartel1)-tx*2);
				C2[0]=x+tz*2;
				C2[2]=-((posCartel1)+tx*2);
				}
		if(posicion[2] < -posCartel2){
				posCartel2=posCartel2+40;
				tz=-1;
				x = 5*sin((posCartel2)*(PI/25));
				tx = (PI/5)*cos(-1*posCartel2*(PI/25.0));
                C3[0]=x-tz*2;
                C3[2]=-((posCartel2)-tx*2);
				C4[0]=x+tz*2;
                C4[2]=-((posCartel2)+tx*2);

        }
}
//Calcula la posicion inicial de los carteles
void calcularCarteles(){
	calcular_cartel(C1,C2,21);
	calcular_cartel(C3,C4,41);		           
}

//Objetos farola (Geometría)
void calcular_Farola(){
		GLfloat tx;
		GLfloat tz=-1;
		double i = posFarola1;
		

        double x = 5*sin((i)*(PI/25.0));
        tx = (PI/5.0)*cos(-1*i*(PI/25.0));

        //Definimos la posicion de la primera farola             
        S1[0]=x-tz*2;
        S1[1]=0;
        S1[2]=-((posFarola1)-tx*2);
		

		i=posFarola2;
		x = 5*sin((i)*(PI/25.0));
        tx = (PI/5.0)*cos(-1*i*(PI/25.0));
		
        //Definimos la posicion de la segunda farola 

        S2[0]=x+tz*2;
        S2[1]=0;
        S2[2]=-((posFarola2)+tx*2);
		

};
void colocar_objetoFarola(){
	
	
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.5,0.5,0.5);
	double i =posFarola1;	
	double x = 5*sin((i)*(PI/25));
	
	//Geometría farola 1 (derecha)
	glPushMatrix();
		glTranslatef(S1[0],S1[1],S1[2]);
		glRotatef(90,-1,0,0);
		glutSolidCylinder(0.1,4,12,12);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(S1[0],4,S1[2]);
		glRotatef(90,0,-1,0);
		glutSolidCylinder(0.1,2.25,12,12);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0,-0.5,0);
		glTranslatef(x,4,S1[2]);
		glRotatef(90,-1,0,0);
		glColor3f(1.0,1.0,0.0);
		//Aspecto de bombilla luminosa
		glPushAttrib(GL_LIGHTING_BIT);
		glMaterialfv( GL_FRONT , GL_EMISSION , AMARILLO );
		glutSolidSphere(0.15,15,15);
		glPopAttrib();
		glColor3f(0.5,0.5,0.5);
		glutSolidCone(0.5,0.5,12,12);
	glPopMatrix();

	
	 i =posFarola2;	
	 x = 5*sin((i)*(PI/25));
	
	//Geometría farola 2 (izquierda)
	glColor3f(0.5,0.5,0.5);
	glPushMatrix();
		glTranslatef(S2[0],S2[1],S2[2]);
		glRotatef(90,-1,0,0);
		glutSolidCylinder(0.1,4,12,12);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(S2[0],4,S2[2]);
		glRotatef(90,0,1,0);
		glutSolidCylinder(0.1,2.25,12,12);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0,-0.5,0);
		glTranslatef(x,4,S2[2]);
		glRotatef(90,-1,0,0);
		glColor3f(1.0,1.0,0.0);
		//Aspecto de bombilla luminosa
		glPushAttrib(GL_LIGHTING_BIT);
		glMaterialfv( GL_FRONT , GL_EMISSION , AMARILLO );
		glutSolidSphere(0.15,15,15);
		glPopAttrib();
		glColor3f(0.5,0.5,0.5);
		glutSolidCone(0.5,0.5,12,12);
	glPopMatrix();

	glDisable(GL_COLOR_MATERIAL);
}
void recalcularobjetosFarola(){
	double x,tx;
	GLfloat tz;
        if(posicion[2] < -posFarola1){
				posFarola1=posFarola1+40;
				tz=-1;
				x = 5*sin((posFarola1)*(PI/25.0));
				tx = (PI/5.0)*cos(-1*posFarola1*(PI/25.0));
				S1[0]=x-tz*2;
				S1[1]=0;
				S1[2]=-((posFarola1)-tx*2);
				}
		if(posicion[2] < -posFarola2){
				posFarola2=posFarola2+40;
				tz=-1;
				x = 5*sin((posFarola2)*(PI/25.0));
				tx = (PI/5.0)*cos(-1*posFarola2*(PI/25.0));
				S2[0]=x+tz*2;
				S2[1]=0;
				S2[2]=-((posFarola2)+tx*2);
	
        }
}

//Metodo para mostrar información de usuario (HUD)
void mostrarVelocidad(){
	if(luces ==-1)seleccionar_textura(tex[6]);
	else seleccionar_textura(tex[8]);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//Usamos esto para que no le afecte la iluminacion


	
	glPushMatrix();
		glTranslatef(0.95,-0.85,-0.5);
		glScalef(0.4,0.6,0.5);
		//Cuadrado para alojar le velocimetro
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(-1,0,0);
			glTexCoord2f(0,1);
			glVertex3f(-1,1,0);
			glTexCoord2f(1,1);
			glVertex3f(1,1,0);
			glTexCoord2f(1,0);
			glVertex3f(1,0,0);	
		glEnd();

	//Aguja indicadora del velocimetro 
	glPopMatrix();

	if(luces ==-1)seleccionar_textura(tex[7]);
	else seleccionar_textura(tex[9]);
	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);//Usamos esto para que no le afecte la iluminacion
	glPushMatrix();
		glTranslatef(0.92,-0.85,-0.49);

		glScalef(0.45,0.55,0.5);
		glTranslatef(0,0.5,0);
		glRotatef(anguloVelocidad,0,0,-1);//
		glRotatef(115,0,0,1);//Esto coloca la aguja apuntando a cero 
		glTranslatef(0,-0.5,0);
		//Cuadrado para alojar la aguja
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3f(-1,0,0);
			glTexCoord2f(0,1);
			glVertex3f(-1,1,0);
			glTexCoord2f(1,1);
			glVertex3f(1,1,0);
			glTexCoord2f(1,0);
			glVertex3f(1,0,0);	
		glEnd();

		
	glPopMatrix();
	//Fin pruebas
	
}


void DrawHud()
{
	
    // Desactivar iluminación
	glDisable( GL_LIGHTING );
    glDisable( GL_TEXTURE_2D );
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
        glLoadIdentity();
		//Para colocar elementos 2D se usa la proyección otrográfica
        glOrtho( -100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f );

        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
		 glColor4f( 1.0, 1.0f, 1.0f,0.5f );
            glLoadIdentity();
            glTranslatef( -90.0f, -90.0f, 0.0f );
            glScalef( 0.1f, 0.1f, 0.1f );

			int aux=metros;
			char str[256];
			sprintf(str, "%d.%.3d KM", distancia,aux);
            char *message = str;
            int index = 0;
            while( *( message + index++ ) != '\0' )
                glutStrokeCharacter( GLUT_STROKE_ROMAN, *( message + index -1 ));
            
        glPopMatrix();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    // Reenable lighting
    glColor4f( 0.0, 0.0f, 0.0f,1.0f );
	glEnable( GL_TEXTURE_2D );
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	
}


//Mostrar datos de velocidad en el titulo
void setTitulo(){
        stringstream vel;
        vel << "Velocidad " << velocidad << " m/s";
        string str(vel.str());
        const char* tituloVelocidad = str.c_str();
        glutSetWindowTitle(tituloVelocidad);
}
void luz_vehiculo(){if(largas==-1){
			//Nos aseguramos de volver a las luces cortas
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0);//angulo
			glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 20.0);//exponente
			glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 25.0);//angulo
			glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 20.0);//exponente
			//Calculamos la posicion del foco dependiente de la camara (Luz vehiculo) *Mover a colocar_vehiculo
			//Luz izq
			static const GLfloat PL1[] = {-0.9, 0.2,-1, 1 }; 
			glLightfv(GL_LIGHT1, GL_POSITION, PL1); 
			GLfloat dir_central[]={0.0,-1.0, -2.0};
			glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,dir_central);
			//Lus der
			static const GLfloat PL2[] = {0.9, 0.2, -1, 1 }; 
			glLightfv(GL_LIGHT7, GL_POSITION, PL2); 
			glLightfv(GL_LIGHT7,GL_SPOT_DIRECTION,dir_central);
			
			}

	else{ 
		//Con esto creamos efecto de luces largas
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);//angulo
		  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 10.0);//exponente
		  glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 45.0);//angulo
		  glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 10.0);//exponente
	}}
//Inicialización propia
void init(){
 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); 

	cilindro = gluNewQuadric();
	gluQuadricNormals(cilindro, GLU_SMOOTH);

    /*CALCULAR ORIENTACION*/
    //Puntos tangentes al camino
    float tx = (PI/5)*cos(posicion[2]*(PI/25));
    float tz = -1;
        
    float modulopos2=sqrt(pow(mirar[0],2)+pow(mirar[2],2));
    float modulopos3= sqrt(pow(tx,2)+pow(tz,2));
    float productoEscalar = mirar[0]*tx +mirar[2]*tz;

    //Angulo desplazado
    angulo= acos(productoEscalar/(modulopos2*modulopos3));
	
	mirar[0] = sin(angulo);
    mirar[2] = -cos(angulo);

	zsuelo=0;
	// Brillos integrados en la textura
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	//Cargar las texturas
	loadTexture();
	
	//Calcular posicion de los carteles
	calcularCarteles();

	 //Inicialización de luces
     cargaLuces();

     //Calcular posicion de las cuatro farolas
     calcularFarolas();

	 //Objetos farola
	 calcular_Farola();
	
}
void colocar_vehiculo(GLuint textura){
 //Activar Transparencias para modelo de coche
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	seleccionar_textura(textura);

	glPushMatrix();
		glTranslatef(posicion[0], -0.1f,posicion[2]);
		glTranslatef(mirar[0]*0.9, 0.0f,mirar[2]*0.9);
		//*Angulo esta en radianes, se convierte a grados mediate las operaciones angulo*180/PI
		glRotatef(((angulo*180)/PI),0,-1,0);
		glTranslatef(0,1,0);
		glRotatef(180,0,0,1);
		//Poligono para pegar en el la textura del coche
		glBegin(GL_QUADS);
			glTexCoord2f(1,1);
			glVertex3f(-1,0,0);
			glTexCoord2f(1,0);
			glVertex3f(-1,1,0);
			glTexCoord2f(0,0);
			glVertex3f(1,1,0);
			glTexCoord2f(0,1);
			glVertex3f(1,0,0);	
		glEnd();
	

	glPopMatrix();}
void colocar_suelo(){
	//Para este objeto usamos coordenadas de textura autogeneradas
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	// Alineacion de la textura
	GLfloat planoS[] = {0.5,0,0,0};
	GLfloat planoT[] = {0,0,0.5,0};
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, planoS);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, planoT);

	seleccionar_textura(tex[5]);
	double ysuelo;
	double x=posicion[0];
	ysuelo=-0.001;
	//Vertices para generar cartel
	GLfloat a[3];
	GLfloat b[3];
	GLfloat c[3];
	GLfloat d[3];

	//Dar valor a los vertices
	
	a[0]=x-100;
	a[1]=ysuelo;
	a[2]=zsuelo+100;
	
	b[0]=x+100;
	b[1]=ysuelo;
	b[2]=zsuelo+100;
	
	c[0]=x+100;
	c[1]=ysuelo;
	c[2]=zsuelo-100;
	
	d[0]=x-100;
	d[1]=ysuelo;
	d[2]=zsuelo-100;

	//Pintar cartel con resolucion 20X20
	quad(a,b,c,d,400,400);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
	}
void recalcular_suelo(){
	if (zsuelo-50>posicion[2])zsuelo=zsuelo-150;
}
void display(){
    // Borrado de la pantalla
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//colocamos la luz del vehiculo antes que la camara para que sea solidaria a esta 
    luz_vehiculo();

	mostrarVelocidad();
	recalcularobjetosFarola();
	//Camara
    gluLookAt(posicion[0],1.0f,posicion[2],mirar[0]+posicion[0], 1.0f,mirar[2]+posicion[2],0.0f,1.0f,0.0f);
	recalcularFarolas();
    //Colocamos las farolas
    colocarFarolas();
	
	

	//Seleccionar  textura paisaje
	seleccionar_textura(tex[1]);
	
	//Objeto cuadrico para la textura
	gluQuadricTexture(cilindro, true);

	//Colocar el cilindro en posicion 
	glPushMatrix();
	glTranslatef(posicion[0],220,posicion[2]);
	glRotatef(90,1,0,0);
	gluCylinder(cilindro, 50, 50, 360, 30, 30);
	glPopMatrix();
	
	colocarCarteles();
	// Dibujar carretera
	//Seleccionar textura carretera
	seleccionar_textura(tex[0]);
    carretera();
	//Carteles	
    recalcularCarteles();
	
	
    //Mostrar velocidad
    setTitulo();

	//colocar el vehiculo
	colocar_vehiculo(tex[4]);

	//Colocar suelo
	recalcular_suelo();
	colocar_suelo();

	//LUNA 
    //posicion de la luna
    GLfloat Pl0[]={0,10,0,0};
    glLightfv(GL_LIGHT0, GL_POSITION, Pl0);

	//Objeto farola
	colocar_objetoFarola();

	//Añadir HUD
	DrawHud();
	
	

	//Comprobar si la iluminación debe activarse
	if(luces ==-1) glDisable(GL_LIGHTING);
	else glEnable(GL_LIGHTING);

	//Hace un flush implicito
    glutSwapBuffers();
}

//funcion de redimensionado
void reshape(GLint w, GLint h){
    //Marco es toda la imagen
    float razon = (float)w/h;
    glViewport(0,0,w,h);
    //Configuro la camara
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Isotropia con camara perspectiva
    gluPerspective(120,razon,0.1,130);    
	
}

//Función de atención a las teclas "flecha"
void onSpecialKey(int specialKey, int x, int y)
{
    switch (specialKey) {
            //Giro izq
            case GLUT_KEY_LEFT :
				if(velocidad>0){
                    angulo -= rad(1);
                    mirar[0] = sin(angulo);
					mirar[2] = -cos(angulo);}
                    break;
			//Giro der
            case GLUT_KEY_RIGHT :
				if(velocidad>0){
                    angulo += rad(1);
                    mirar[0] = sin(angulo);
					mirar[2] = -cos(angulo);}
                    break;
            //Aumentar velocidad
            case GLUT_KEY_UP :
                    if(velocidad < 55){
                            velocidad += 0.1;
							anguloVelocidad+=0.32;}
                    if(velocidad > 55){
						velocidad = 55;
						
					}
                    break;
			//Disminuir velocidad
            case GLUT_KEY_DOWN :
                    if( velocidad>0 ){
                            velocidad -= 0.1;
							anguloVelocidad-=0.32;}
                    if( velocidad<0 ) velocidad = 0;
                    break;
    }

}

void onKey(unsigned char tecla , int x , int y){
	//Función de atención al pulsado de teclas
	switch(tecla){
	case 's': modo = modo*(-1);
		break;

	case 'n': niebla = niebla*(-1);
		if(niebla<0){						//Configuración niebla
			glEnable(GL_FOG);				//Activar Niebla
			glFogfv(GL_FOG_COLOR,BLANCO);	//Configurar color niebla
			glFogf(GL_FOG_DENSITY, 0.1);	//COnfigurar densidad de la niebla
			}
		else{ glDisable(GL_FOG);}
		break;

	case 'l': luces =luces*(-1);
			break;

	case 't': largas = largas*(-1);
		printf("%d",largas);

	glutPostRedisplay();
	}
}

void update(){
        
    //Variable para la hora anterior
    static int hora_anterior = glutGet(GLUT_ELAPSED_TIME);
    //Pido la hora actual
    int hora_actual = glutGet(GLUT_ELAPSED_TIME);
    //Tiempo transcurrido en segundos
    float tiempo = (hora_actual-hora_anterior) / 1000.0; 
    //Movimiento de la cámara
    posicion[0] += (velocidad*tiempo)*sin(angulo);
    posicion[2] += (velocidad*tiempo)*-cos(angulo);
	if(velocidad > 55) velocidad =55;
	metros+=(velocidad*tiempo);
	if (metros>1000){metros-=1000;distancia+=1;}
    //Actualizar tiempo transcurrido (hora actual)
    hora_anterior = hora_actual;
    //Redibujar
    glutPostRedisplay();
}

void onTimer(int valor){
    // Vuelvo a poner en marcha el crono (cuenta atras)
    glutTimerFunc(valor, onTimer, valor);
    //Actualizar variables
    update();
}

void main(int argc, char** argv)
{
    FreeImage_Initialise(); // Inicializacion de FreeImage
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600,400);

    glutCreateWindow("Circuito");


    //Configuración color
    glClearColor(1, 1, 1,0);
    //Registro de callbacks 
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc( onSpecialKey );
	glutKeyboardFunc(onKey);
    glutTimerFunc(1000/tasaFPS,onTimer,1000/tasaFPS );
    init();
    //Bucle del programa principal
    glutMainLoop();
	FreeImage_DeInitialise(); // Cierre de FreeImage
        
}