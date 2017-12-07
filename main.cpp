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
#define fini FALSE
#define accept FALSE


//DECLARATION DES VARIABLES
    //MENU
    int menu;
    int window;
    int value = 1;
    //CLIC SOURIS
    int x0, y0;
    int cnt = 0;
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

    //FENETRAGE
    int Xmin, Xmax, Ymin, Ymax; //COORDONNEES DE LA FENETRE
    int codeA, codeB, codeExt; //CODE SEGMENT
    int somme, gauche, droite, haut, bas;
    int segmentCode;
    //VARIABLES INTERMEDIAIRES
    int m, p;
    int x = 0;
    int y = 0;
    int c; //COULEUR


//PROTOTYPES DE FONCTIONS
void menuInterface(int num);
void clearConsole(void);
//void pickColor(int x0, int y0, int c);
void menuInterface(int menu);
void mouse(int button, int state, int x0, int y0);
void affichePixel(int x, int y);
void affichage(void);
void bresemham_cercle(int xc, int yc, int r);
void bresemham_segment(int xa, int xb, int ya, int yb);
void bloc_couleur(void);


int main(int argc, char **argv){

	//INITIALISATION DE GLUT ET CREATION DE LA FENETRE
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

        //FENETRE PRINCIPALE
        glutInitWindowSize(500, 500); //DIMENSION DE LA FENETRE
        glutInitWindowPosition (100, 100); //POSITION HAUT/GAUCHE
        window = glutCreateWindow("PROJET"); //NOM DE LA FENETRE
        //REPERE 2D DELIMITANT LES ABSCISSES ET LES ORDONNEES
        gluOrtho2D(-250.0, 250.0, -250.0, 250.0);
        //INITIALISATION D'OPENGL
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glColor3f(0.0, 0.0, 0.0); //COULEUR: BLANC
        glPointSize(2.0); //TAILLE D'UN POINT: 2px
        //ENREGISTREMENT DES FONCTIONS D'APPELS
        glutDisplayFunc(affichage);
        glutMouseFunc(mouse);

        //GESTION DU MENU
        menu = glutCreateMenu(menuInterface);

        glutAddMenuEntry("Segment", 1);
        glutAddMenuEntry("Cercle", 2);
        glutAddMenuEntry("Ellipse", 3);
        glutAddMenuEntry("Découpage", 9);
        glutAddMenuEntry("Effacer", 4);
        glutAddMenuEntry("Quitter le programme", 5);
        glutAttachMenu(GLUT_RIGHT_BUTTON); //AU CLIC DROIT

        //FENETRE DE COULEUR
        glutInitWindowSize(500, 500); //DIMENSION DE LA FENETRE
        glutInitWindowPosition (100, 100); //POSITION HAUT/GAUCHE
        glutCreateWindow("DONNEZ DE LA COULEUR A VOS TRACES!"); //NOM DE LA FENETRE
        //REPERE 2D DELIMITANT LES ABSCISSES ET LES ORDONNEES
        gluOrtho2D(-250.0, 250.0, -250.0, 250.0);
        //INITIALISATION D'OPENGL
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glColor3f(0.0, 0.0, 0.0); //COULEUR: NOIR
        glPointSize(2.0); //TAILLE D'UN POINT: 2px
        //ENREGISTREMENT DES FONCTIONS D'APPELS
        glutDisplayFunc(bloc_couleur);
        //glutMouseFunc(pickColor);

    glutMainLoop();
    return 0;
}
/*void pickColor(int button, int state, int x0, int y0, int c){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        x = x0 - 250;
        y = -y0 + 250;

        c =
    }
}*/
//POUR LE CHOIX DE COULEUR
void menuInterface(int num){
    if(menu == 0){
        glutDestroyWindow(window);
        exit(0);
    }else{
        value = num;
    }
    glutPostRedisplay();
}
void bloc_couleur(){
    glClear(GL_COLOR_BUFFER_BIT);
    //DESSIN DU CARRE DE COULEUR
    glBegin(GL_POLYGON);

        glColor3f(1.0,0.0,0.0); //ROUGE
        glVertex2f(-250.0, -250.0);
        glColor3f(0.0,1.0,0.0); //VERT
        glVertex2f(-250, 250);
        glColor3f(0.0,0.0,1.0); //BLEU
        glVertex2f(250.0, 250.0);
        glColor3f(1.0,1.0,1.0); //BLANC
        glVertex2f(250.0, -250.0);

    glEnd();
    glFlush();
}
void affichage(){

    if(value == 1){

        bresemham_segment(xa, xb, ya, yb);

    }else if(value == 2){

        bresemham_cercle(xc, yc, r);

    }else if(value == 3){

    }else if(value == 4){
        clearConsole();
        value = 0;

    }else if(value == 5){
        exit(0);
    }
    glFlush();

}
void clearConsole(){
    glClear(GL_COLOR_BUFFER_BIT);
}
void affichePixel(int x, int y){
    //glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0,1.0,1.0); //BLANC
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
void fenetrage(){

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
void bresemham_ellipse(int xc, int yc, int r){
    if(y >= x){

        x = 0;
        y = r;

    }
}
void mouse(int button, int state, int x0, int y0){

    if(value == 2){
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
    }else if(value == 1){
        //TRACE DE SEGMENT AVEC DEUX CLICS SOURIS
        if(cnt % 4 != 0){
            if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                xb = x0 - 250;
                printf("\nxb = %d", xb);
                yb = -y0 + 250;
                printf("\nyb = %d\ncnt = %d", yb, cnt);
                affichage();
            }
        }
        if(cnt % 2 == 0){
            if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                xa = x0 - 250;
                printf("\nxa = %d", xa);
                ya = -y0 + 250;
                printf("\nxy = %d", ya);
            }
        }

        cnt++;
    }

}

int calculCode(int x, int y, int Xmin, int Xmax, int Ymin, int Ymax){

}
