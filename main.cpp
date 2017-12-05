/*
Groupe:
    DESMONTS Julien
    FERNANDEZ Benjamin
    TANG SONG Caroline
Date: 09/11/2017

1ERE PARTIE:
    Consiste à tracer des segments et des cercles suivants différents axes.
    - méthode interactive de tracés avec la souris
    - utilisation de l'algorithme de BRESENHAM pour les deux primitives graphiques
SOURIS:
    - utilisation de couleurs interactives
    BRESENHAM:
        - tracés de segments
        - tracés d'ellipses
        - tracés de cercles quelconques

*/
#include<windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>


//DECLARATION DES VARIABLES
    //CLIC SOURIS
    int x0, y0;
    //COORDONNEES DU SEGMENT [AB]
    int xa = 0;
    int ya = 0;
    int xb, yb;
    int dx, dy;
    int incrE, incrNE, incrX, incrY, di;

    //COORDONNEES DU CERCLE
    int xc, yc; //POINTS ORIGINE
    //POINTS DESTINATION
    int rx = 0;
    int ry = 0;
    int r; //RAYON
    int c_incrE, c_incrSE; //INCREMENTATION EST ET SUD EST
    int dp;
    //VARIABLES INTERMEDIAIRES
    int m, p;
    int x = 0;
    int y = 0;


//PROTOTYPES DE FONCTIONS
void mouse_segment(int button,int state,int x0,int y0);
void mouse_cercle(int button, int state, int x0, int y0);
void clavier(unsigned char touche,int x,int y);
void affichePixel(int x, int y);
void affichage(void);
void bresemham_cercle(int xc, int yc, int r);
void bresemham_segment(int xa, int xb, int ya, int yb);
void bloc_couleur(void);


int main(int argc, char **argv){
    printf("Commandes disponibles: \n\n");
    printf(" - 'c' pour activer le mode: trace de cercle.\n");
    printf(" - 'e' pour activer le mode: trace d'ellipse.\n");
    printf(" - 's' pour activer le mode: trace de segment.\n");
    printf(" - clique droit pour nettoyer la fenetre.\n");
    printf(" - 'q' pour quitter le programme.");

	//INITIALISATION DE GLUT ET CREATION DE LA FENETRE
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

        //FENETRE PRINCIPALE
        glutInitWindowSize(500, 500); //DIMENSION DE LA FENETRE
        glutInitWindowPosition (100, 100); //POSITION HAUT/GAUCHE
        glutCreateWindow("PROJET"); //NOM DE LA FENETRE
        //REPERE 2D DELIMITANT LES ABSCISSES ET LES ORDONNEES
        gluOrtho2D(-250.0, 250.0, -250.0, 250.0);
        //INITIALISATION D'OPENGL
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glColor3f(1.0, 1.0, 1.0); //COULEUR: BLANC
        glPointSize(2.0); //TAILLE D'UN POINT: 2px
        //ENREGISTREMENT DES FONCTIONS D'APPELS
        glutDisplayFunc(affichage);
        glutMouseFunc(mouse_cercle);
        //glutMouseFunc(mouse_segment);
        glutKeyboardFunc(clavier);

        //FENETRE DE COULEUR
        glutInitWindowSize(500, 500); //DIMENSION DE LA FENETRE
        glutInitWindowPosition (100, 100); //POSITION HAUT/GAUCHE
        glutCreateWindow("DONNEZ DE LA COULEUR A VOS TRACES!"); //NOM DE LA FENETRE
        //REPERE 2D DELIMITANT LES ABSCISSES ET LES ORDONNEES
        gluOrtho2D(-250.0, 250.0, -250.0, 250.0);
        //INITIALISATION D'OPENGL
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glColor3f(1.0, 1.0, 1.0); //COULEUR: BLANC
        glPointSize(2.0); //TAILLE D'UN POINT: 2px
        //ENREGISTREMENT DES FONCTIONS D'APPELS
        glutDisplayFunc(bloc_couleur);


    glutMainLoop();
    return 0;
}
int arrondi(double x){
    return x + 0.5;
}
//POUR LE CHOIX DE COULEUR
void bloc_couleur(){
    glClear(GL_COLOR_BUFFER_BIT);
    //DESSIN DU CARRE DE COULEUR
    glBegin(GL_POLYGON);

        glColor3f(1.0,0.0,0.0); //ROUGE
        glVertex2f(-250, -250);
        glColor3f(0.0,1.0,0.0); //VERT
        glVertex2f(-250, 250);
        glColor3f(0.0,0.0,1.0); //BLEU
        glVertex2f(250, 250);
        glColor3f(1.0,1.0,1.0); //BLANC
        glVertex2f(250, -250);

    glEnd();
    glFlush();
}
void affichage(){

    //TRACE DE CERCLES
    x = 0;
    y = r;
    printf("\nRayon = %d", r);
    dp = 5 - 4 * r;
    affichePixel(x + xc, y + yc); //PREMIER PIXEL ALLUME A PARTIR DE L'OCTANT 2
    affichePixel(y + xc, x + yc);
    affichePixel(x + xc, -y + yc);
    affichePixel(-y + xc, x + yc);

    while(y > x){
        if(dp <= 0){
            dp = dp + 8 * x + 12;
        }else{
            dp = dp + 8 * (x - y) + 20;
            y -= 1;
        }
        x += 1;
        affichePixel(x + xc, y + yc);
        affichePixel(y + xc, x + yc);
        affichePixel(y + xc, -x + yc);
        affichePixel(x + xc, -y + yc);
        affichePixel(-x + xc, -y + yc);
        affichePixel(-y + xc, -x + yc);
        affichePixel(-y + xc, x + yc);
        affichePixel(-x + xc, y + yc);
    }
    glFlush();
}

void affichePixel(int x, int y){
    //glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0,0.0,0.0); //ROUGE
        glBegin(GL_POINTS);
            glVertex2f(x, y);
        glEnd();
    glFlush();
}
void bresemham_cercle(int xc, int yc, int r){
    x = 0;
    y = r;
    printf("\nrayon = %d", r);
    dp = 5 - 4 * r;
    affichePixel(x + xc, y + yc); //PREMIER PIXEL ALLUME A PARTIR DE L'OCTANT 2
    affichePixel(y + xc, x + yc);
    affichePixel(x + xc, -y + yc);
    affichePixel(-y + xc, x + yc);

    while(y > x){
        if(dp <= 0){
            dp = dp + 8 * x + 12;
        }else{
            dp = dp + 8 * (x - y) + 20;
            y -= 1;
        }
        x += 1;
        affichePixel(x + xc, y + yc);
        affichePixel(y + xc, x + yc);
        affichePixel(y + xc, -x + yc);
        affichePixel(x + xc, -y + yc);
        affichePixel(-x + xc, -y + yc);
        affichePixel(-y + xc, -x + yc);
        affichePixel(-y + xc, x + yc);
        affichePixel(-x + xc, y + yc);
    }
}
void bresemham_segment(int xa, int xb, int ya, int yb){
    //TRACE DE SEGMENTS
    if(ya <= yb){ //OCTANTS 1, 2, 3 ET 4
        if(xa <= xb){ //OCTANTS 1 ET 2
            dx = xb - xa;
            dy = yb - ya;

            if(dx >= dy){ //OCTANT 1
                di = 2 * dy - dx;
                incrE = 2 * dy;
                incrNE = 2 * (dy - dx);
                y = ya;
                for(x = xa; x < xb; x++){
                    affichePixel(x, y);
                    if(di <= 0){
                        di += incrE; //ON CHOISIT LE PIXEL E
                    }else{
                        y += 1;
                        di += incrNE;
                    }
                }
            }
            if(dx < dy){ //OCTANT 2
                di = 2 * dx - dy;
                incrE = 2 * dx;
                incrNE = 2 * (dx - dy);
                x = xa;
                for(y = ya; y < yb; y++){
                    affichePixel(x, y);
                    if(di <= 0){
                        di += incrE; //ON CHOISIT LE PIXEL E
                    }else{
                        x += 1;
                        di += incrNE;
                    }
                }

            }

        }
        if(xa > xb){
            dx = xa - xb;
            dy = yb - ya;

            if(dx < dy){ //OCTANT 3
                di = 2 * dx - dy;
                incrE = 2 * dx;
                incrNE = 2 * (dx - dy);
                x = xa;
                for(y = ya; y < yb; y++){
                    affichePixel(x, y);
                    if(di <= 0){
                        di += incrE; //ON CHOISIT LE PIXEL E
                    }else{
                        x -= 1; //ON DECREMENTE X
                        di += incrNE;
                    }
                }
            }
            if(dx >= dy){ //OCTANT 4
                di = 2 * dy - dx;
                incrE = 2 * dy;
                incrNE = 2 * (dy - dx);
                y = ya;
                for(x = xa; x > xb; x--){
                    affichePixel(x, y);
                    if(di <= 0){
                        di += incrE; //ON CHOISIT LE PIXEL E

                    }else{
                        y += 1;
                        di += incrNE;
                    }
                }
            }

        }
    }
    if(ya > yb){ //OCTANTS 5, 6, 7 ET 8
        if(xa <= xb){ //OCTANTS 7 ET 8
            dx = xb - xa;
            dy = ya - yb;

            if(dx >= dy){ //OCTANT 8
                di = 2 * dy - dx;
                incrE = 2 * dy;
                incrNE = 2 * (dy - dx);
                y = ya;
                for(x = xa; x < xb; x++){
                    affichePixel(x, y);
                    if(di <= 0){
                        di += incrE; //ON CHOISIT LE PIXEL E
                    }else{
                        y -= 1;
                        di += incrNE;
                    }
                }
            }
            if(dx < dy){ //OCTANT 7
                di = 2 * dx - dy;
                incrE = 2 * dx;
                incrNE = 2 * (dx - dy);
                x = xa;
                for(y = ya; y > yb; y--){
                    affichePixel(x, y);
                    if(di <= 0){
                        di += incrE; //ON CHOISIT LE PIXEL E
                    }else{
                        x += 1;
                        di += incrNE;
                    }
                }

            }

        }
        if(xa > xb){
            dx = xa - xb;
            dy = ya - yb;

            if(dx < dy){ //OCTANT 6
                di = 2 * dx - dy;
                incrE = 2 * dx;
                incrNE = 2 * (dx - dy);
                x = xa;
                for(y = ya; y > yb; y--){
                    affichePixel(x, y);
                    if(di <= 0){
                        di += incrE; //ON CHOISIT LE PIXEL E
                    }else{
                        x -= 1; //ON DECREMENTE X
                        di += incrNE;
                    }
                }
            }
            if(dx >= dy){ //OCTANT 5
                di = 2 * dy - dx;
                incrE = 2 * dy;
                incrNE = 2 * (dy - dx);
                y = ya;
                for(x = xa; x > xb; x--){
                    affichePixel(x, y);
                    if(di <= 0){
                        di += incrE; //ON CHOISIT LE PIXEL E

                    }else{
                        y -= 1;
                        di += incrNE;
                    }
                }
            }

        }
    }
}
void mouse_cercle(int button, int state, int x0, int y0){

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){

        printf("\nPOINT ORIGINE: ");
        xc = x0 - 250;
        printf("\nxc = %d", xc);
        yc = -y0 + 250;
        printf("\nyc = %d", yc);
    }

     if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        rx = 0;
        ry = 0;
        printf("\nPOINT D'ARRIVEE: ");
        //MOUVEMENT GAUCHE-DROITE
        rx = (x0 - xc) - 250;
        //printf("\nx0 = %d", x0);
        printf("\nrx = %d", rx);
        ry = (-y0 - yc) + 250;
        //printf("\ny0 = %d", y0);
        printf("\nry = %d", ry);

        //OCTANTS 5/6
        if(rx <= 0 && ry <= 0){
            if(rx <= ry){
                printf("\noctant 5");
                r = -rx; //OCTANT 5
            }else{
                printf("\noctant 6");
                r = -ry; //OCTANT 6
            }

        }
        //OCTANTS 7/8
        if(rx >= 0 && ry <= 0){
            if(rx >= ry){
                printf("\noctant 8");
                r = rx; //OCTANT 8
            }else{
                printf("\noctant 7");
                r = -ry; //OCTANT 7
            }

        }
        //OCTANTS 3/4
        if(rx <= 0 && ry >= 0){
            if(rx >= ry){
                printf("\noctant 4");
                r = -rx; //OCTANT 4
            }else{
                printf("\noctant 3");
                r = ry; //OCTANT 3
            }
        }
        //OCTANTS 1/2
         if(rx >= 0 && ry >= 0){
            if(rx >= ry){
                printf("\noctant 1");
                r = rx; //OCTANT 1
            }else{
                printf("\noctant 2");
                r = ry; //OCTANT 2
            }
        }
        affichage();
    }

    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
    }
}
void mouse_segment(int button, int state, int x0, int y0){

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        xa = x0 - 250;
        ya = -y0 + 250;

    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        xb = x0 - 250;
        yb = -y0 + 250;

        affichage();
    }
}
//EVENEMENT AU CLAVIER
void clavier(unsigned char touche,int x,int y){
	switch (touche){
        case 'c':

            break;//Tracé de cercle
        case 's':
            break;//Tracé de segment
        case'e':
            break;//Tracé d'ellipse
		case 'q': //Quitter le programme
			exit(0);
	}
}
