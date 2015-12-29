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
double zsuelo;
//
GLUquadric* cilindro;
//Inicializacion variables
static float velocidad = 0.0;
float angulo ;

//vector texturas
static GLuint tex[6];
//Lista cartel
static GLuint cartel;

//Variables de la camara
float mirar[3]={0,0,-1};
static GLfloat posicion[3]={0,1,0};
//Orientacion y posicion de las farolas
static GLfloat F1[] = { 0 , 2.5, 0, 1 };
static GLfloat F2[] = { 0 , 2.5, 0, 1 };
static GLfloat F3[] = { 0 , 2.5, 0, 1 };
static GLfloat F4[] = { 0 , 2.5, 0, 1 };

static GLfloat orientacionF[] = { 0 , -1.0, 0};
//Vectores para la posicion de los anuncios
static GLfloat C1[] = { 0 , 0, 0, 0 };
static GLfloat C2[] = { 0 , 0, 0, 0 };
static int posCartel1=21;
static GLfloat C3[] = { 0 , 0, 0, 0 };
static GLfloat C4[] = { 0 , 0, 0, 0 };
static int posCartel2=39;

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
// nombre: nombre del fichero con extensi�n en el mismo directorio que el proyecto
// o con su path completo
{
// Detecci�n del formato, lectura y conversion a BGRA
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
glGenTextures(6,tex);
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
    // Inicializaci�n de las luces
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

    /***LUZ DEL VEHICULO***/
    GLfloat Al1[]={0.2,0.1,0.2,0.2};//ambiental
    GLfloat Dl1[]={1.0,1.0,1.0,1.0};//difusa
    GLfloat Sl1[]={0.3,0.3,0.3,0.3};//especular

    //CARACTERIZAMOS LA LUZ
    glLightfv(GL_LIGHT1, GL_AMBIENT, Al1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, Dl1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, Sl1);

    //Definimos el angulo y el exponente
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);//angulo
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 15.0);//exponente
    glEnable(GL_LIGHT1);

    /***FAROLAS***/
    GLfloat Af[]={0.0,0.0,0.0};//ambiental
    GLfloat Df[]={0.5,0.5,0.2};//difusa
    GLfloat Sf[]={0.0,0.0,0.0};//especular

    //FAROLA 1 (light3)
    glLightfv(GL_LIGHT3, GL_AMBIENT, Af);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, Df);
    glLightfv(GL_LIGHT3, GL_SPECULAR, Sf);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 5.0);
    glEnable(GL_LIGHT3);

    //FAROLA 2 (light4)
    glLightfv(GL_LIGHT4, GL_AMBIENT, Af); 
    glLightfv(GL_LIGHT4, GL_DIFFUSE, Df);
    glLightfv(GL_LIGHT4, GL_SPECULAR, Sf);
    glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 5.0);
    glEnable(GL_LIGHT4);

    //FAROLA 3 (light5)
    glLightfv(GL_LIGHT5, GL_AMBIENT, Af); 
    glLightfv(GL_LIGHT5, GL_DIFFUSE, Df);
    glLightfv(GL_LIGHT5, GL_SPECULAR, Sf);
    glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 5.0);
    glEnable(GL_LIGHT5);

    //FAROLA 4 (light6)
    glLightfv(GL_LIGHT6, GL_AMBIENT, Af); 
    glLightfv(GL_LIGHT6, GL_DIFFUSE, Df);
    glLightfv(GL_LIGHT6, GL_SPECULAR, Sf);
    glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 45.0);
    glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 5.0);
    glEnable(GL_LIGHT6);

    //calculamos la posicion inicial de los carteles
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
	glBindTexture(GL_TEXTURE_2D,textura);
	//2b. Definir como se aplicar� la textura en ese objeto
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//2c. Definir la forma de combinar
	if(luces<0){glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);}
	else{glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);}
}

void calcular_cartel(GLfloat C1[],GLfloat C2[],double zcartel){
	double posicionZ=(int)posicion[2];
	    //Calculo de la normal en cada punto (tz,tx)
        GLfloat tx;
        GLfloat tz=-1;
		double i =zcartel;

        double x = 5*sin((i)*(PI/25));
        tx = (PI/5)*cos(-1*i*(PI/25.0));

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

	//Seleccionar textura cartel 1
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
	calcular_cartel(C1,C2,20);
	calcular_cartel(C3,C4,40);		           
}


//Mostrar datos de velocidad en el titulo
void setTitulo(){
        stringstream vel;
        vel << "Velocidad " << velocidad << " m/s";
        string str(vel.str());
        const char* tituloVelocidad = str.c_str();
        glutSetWindowTitle(tituloVelocidad);
}

//Inicializaci�n propia
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

	 //Inicializaci�n de luces
     cargaLuces();

     //Calcular posicion de las cuatro farolas
     calcularFarolas();
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
		//Poligono para pegar en el la textura dle coche
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
	
	/*
	glBegin(GL_QUADS);
			glTexCoord2f(100,100);
			glVertex3f(x-100,ysuelo,zsuelo+100);
			glTexCoord2f(0,0);
			glVertex3f(x-100,ysuelo,zsuelo-100);
			glTexCoord2f(100,0);
			glVertex3f(x+100,ysuelo,zsuelo-100);
			glTexCoord2f(0,100);
			glVertex3f(x+100,ysuelo,zsuelo+100);	
			glEnd();
	glBegin(GL_QUADS);
			glTexCoord2f(100,100);
			glVertex3f(x-100,ysuelo,zsuelo+200);
			glTexCoord2f(0,0);
			glVertex3f(x-100,ysuelo,zsuelo+200);
			glTexCoord2f(100,0);
			glVertex3f(x+100,ysuelo,zsuelo+200);
			glTexCoord2f(0,100);
			glVertex3f(x+100,ysuelo,zsuelo+200);	
		glEnd();*/
	}
void recalcular_suelo(){
	if (zsuelo-50>posicion[2])zsuelo=zsuelo-150;
}
void display(){
    // Borrado de la pantalla
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	//Calculamos la posicion del foco dependiente de la camara
    static const GLfloat PL1[] = {0, 0.3, -1, 1 }; 
    glLightfv(GL_LIGHT1, GL_POSITION, PL1); 
    GLfloat dir_central[]={0.0,-1.0, -2.0};
	glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,dir_central);
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

	glDisable(GL_TEXTURE);
	glPushMatrix();
	
	glTranslatef(-5*sin(posicion[2]*(PI/25)),1,-10);
	glutSolidCube(1);
	glPopMatrix();
	glEnable(GL_TEXTURE);
	
	// Dibujar carretera
	//Seleccionar textura carretera
	seleccionar_textura(tex[0]);
    carretera();
	//Carteles	
    recalcularCarteles();
	colocarCarteles();

    //Mostrar velocidad
    setTitulo();

	//colocar el vehiculo
	colocar_vehiculo(tex[4]);

	recalcular_suelo();
	colocar_suelo();
	//LUNA 
    //posicion de la luna
    GLfloat Pl0[]={0,10,0,0};
    glLightfv(GL_LIGHT0, GL_POSITION, Pl0);

	

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

//Funci�n de atenci�n a las teclas "flecha"
void onSpecialKey(int specialKey, int x, int y)
{
    switch (specialKey) {
            //Giro izq
            case GLUT_KEY_LEFT :
                    angulo -= rad(1);
                    mirar[0] = sin(angulo);
                    mirar[2] = -cos(angulo);
                    break;
			//Giro der
            case GLUT_KEY_RIGHT :
                    angulo += rad(1);
                    mirar[0] = sin(angulo);
                    mirar[2] = -cos(angulo);
                    break;
            //Aumentar velocidad
            case GLUT_KEY_UP :
                    if(velocidad < 9)
                            velocidad += 0.1;
                    if(velocidad > 9) velocidad = 9;
                    break;
			//Disminuir velocidad
            case GLUT_KEY_DOWN :
                    if( velocidad>0 )
                            velocidad -= 0.1;
                    if( velocidad<0 ) velocidad = 0;
                    break;
    }

}

void onKey(unsigned char tecla , int x , int y){
	//Funci�n de atenci�n al pulsado de teclas
	switch(tecla){
	case 's': modo = modo*(-1);
		if(modo<0){glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_FOG);}
		else{ glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glEnable(GL_TEXTURE_2D);}
		break;

	case 'n': niebla = niebla*(-1);
		if(niebla<0){						//Configuraci�n niebla
			glEnable(GL_FOG);				//Activar Niebla
			glFogfv(GL_FOG_COLOR,BLANCO);	//Configurar color niebla
			glFogf(GL_FOG_DENSITY, 0.1);	//COnfigurar densidad de la niebla
			}
		else{ glDisable(GL_FOG);}
		break;
	case 'l': luces =luces*(-1);
		if(luces<0){glDisable(GL_LIGHTING);
		}
		else{glEnable(GL_LIGHTING);}
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

    //Movimiento de la c�mara
    posicion[0] += (velocidad*tiempo)*sin(angulo);
    posicion[2] += (velocidad*tiempo)*-cos(angulo);

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


    //Configuraci�n color
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