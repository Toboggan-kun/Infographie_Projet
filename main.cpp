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
    //MENU
    int menu;
    int window;
    int value = 1;
    int counter = 0;
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
    int dp;

    //ELLIPSE
    int r1; //REGION/RAYON 1
    int r2; //REGION/RAYON 2
    int twoRX, twoRY;
    int point;

    //POLYGON
    int nbCote = 0;
    int destinationPointX, destinationPointY;
    int clicPolygon = 0;
    int clicEnd;

    //FENETRAGE
    typedef struct{
        int somme;
        int gauche;
        int droite;
        int bas;
        int haut;
    }CodeSegment;

    CodeSegment codeA;
    CodeSegment codeB;
    CodeSegment codeExt;
    CodeSegment codeXY;

    int Xmin, Xmax, Ymin, Ymax; //COORDONNEES DE LA FENETRE
    //VARIABLES INTERMEDIAIRES
    int m, p;
    int x = 0;
    int y = 0;
    int c; //COULEUR

    //COULEUR
    int i, j;
    int R, G, B;



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
void calculCode(void);
int fenetrage(int, int, int, int, int, int, int, int);
int compareStructure(CodeSegment, CodeSegment);
void polygon_trace(int xa, int xb, int ya, int yb);

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
        if(clicEnd == 0){
        glutAddMenuEntry("Segment", 1);
        glutAddMenuEntry("Cercle", 2);
        glutAddMenuEntry("Ellipse", 3);
        glutAddMenuEntry("Polygone", 9);
        glutAddMenuEntry("Fenêtrage", 4);
        glutAddMenuEntry("Effacer", 5);
        glutAddMenuEntry("Quitter le programme", 6);

            glutAttachMenu(GLUT_RIGHT_BUTTON); //AU CLIC DROIT
        }


        //FENETRE DE COULEUR
        glutInitWindowSize(500, 500); //DIMENSION DE LA FENETRE
        glutInitWindowPosition (100, 100); //POSITION HAUT/GAUCHE
        glutCreateWindow("DONNEZ DE LA COULEUR A VOS TRACES!"); //NOM DE LA FENETRE
        //REPERE 2D DELIMITANT LES ABSCISSES ET LES ORDONNEES
        gluOrtho2D(-255.0, 255.0, -255.0, 255.0);
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

void menuInterface(int num){
    if(menu == 0){
        glutDestroyWindow(window);
        exit(0);
    }else{
        value = num;
    }
    glutPostRedisplay();
}

//POUR LE CHOIX DE COULEUR
void bloc_couleur(){

    glClear(GL_COLOR_BUFFER_BIT);

    /*glBegin(GL_POLYGON);
        glColor3f(1.0,0.0,0.0); //ROUGE
        glVertex2f(-250.0, -250.0);
        glColor3f(0.0,1.0,0.0); //VERT
        glVertex2f(-250.0, 250.0);
        glColor3f(0.0,0.0,1.0); //BLEU
        glVertex2f(250.0, 250.0);
        glColor3f(1.0,1.0,1.0); //BLANC
        glVertex2f(250.0, -250.0);
    glEnd();*/
    glBegin(GL_POINTS);
    for(i = -255; i < 255; i++){ //for(i = -250; i < 250; i++)
        R = 255;
        G = 0;
        B = 0;
        for(j = -255; j < 255; j++){
            R--;
            glColor3b(R, G, B);
            glVertex2i(i, j);
        }
    }
    glEnd();


    glFlush();
}
void affichage(){

    if(value == 1){ //SEGMENT
        bresemham_segment(xa, xb, ya, yb);
        printf("\nxa, ya = %d %d; xb, yb = %d %d", xa, ya, xb, yb);
        glFlush();
    }else if(value == 2){ //CERCLE
        bresemham_cercle(xc, yc, r);
        xc = 0;
        yc = 0;
        r = 0;
    }else if(value == 3){

    }else if(value == 4){ //DECOUPAGE

        glEnd();
    }else if(value == 5){ //EFFACER CONSOLE
        clearConsole();
        counter = 0;
        value = 0;

    }else if(value == 6){ //QUITTER LE PROGRAMME
        exit(0);
    }else if(value == 9){

        clicEnd = 1;
        bresemham_segment(xa, xb, ya, yb);

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

    if(counter != 0){

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
    counter++;
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
void bresemham_ellipse(int xc, int yc, int rx, int ry){

    r1 = rx * rx;
    r2 = ry * ry;
    x = rx;
    y = 0;
    twoRX = 2 * rx * rx;
    twoRY = 2 * ry * ry;
    //PREMIER CADRAN
    affichePixel(x + xc, y + yc); //POINT 1: OCTANT 1
    affichePixel(-x + xc, y + yc); //POINT 2: OCTANT 4
    affichePixel(-x + xc, -y + yc); //POINT 3: OCTANT 5
    affichePixel(x + xc, -y + yc); //POINT 4: OCTANT 8


}
//ELLIPSE
void premier_cadran(int x, int y){
    affichePixel(x + xc, y + yc); //POINT 1: OCTANT 1
    affichePixel(-x + xc, y + yc); //POINT 2: OCTANT 4
    affichePixel(-x + xc, -y + yc); //POINT 3: OCTANT 5
    affichePixel(x + xc, -y + yc); //POINT 4: OCTANT 8
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
                rx = -rx;
                ry = -ry;
                if(rx >= ry){
                    printf("\noctant 5");
                    r = rx; //OCTANT 5
                }else{
                    printf("\noctant 6");
                    r = ry; //OCTANT 6
                }

            }
            //OCTANTS 7/8
            else if(rx >= 0 && ry <= 0){
                ry = -ry;
                if(rx >= ry){
                    printf("\noctant 8");
                    r = rx; //OCTANT 8
                }else if(ry >= rx){
                    printf("\noctant 7");
                    r = ry; //OCTANT 7
                }

            }
            //OCTANTS 3/4
            else if(rx <= 0 && ry >= 0){
                rx = -rx;
                if(rx >= ry){
                    printf("\noctant 4");
                    r = rx; //OCTANT 4
                }else{
                    printf("\noctant 3");
                    r = ry; //OCTANT 3
                }
            }
            //OCTANTS 1/2
             else if(rx >= 0 && ry >= 0){
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
                yb = -y0 + 250;
                affichage();
            }
        }
        if(cnt % 2 == 0){
            if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                xa = x0 - 250;
                ya = -y0 + 250;
            }
        }

        cnt++;
    }else if(value == 4){

        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                xa = x0 - 250;
                ya = -y0 + 250;
        }
        if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
                xb = x0 - 250;
                yb = -y0 + 250;
                glBegin(GL_LINES);

                glVertex2d(xa, ya);
                glVertex2d(xb, ya);

                glVertex2d(xb, ya);
                glVertex2d(xb, yb);

                glVertex2d(xb, yb);
                glVertex2d(xa, yb);

                glVertex2d(xa, yb);
                glVertex2d(xa, ya);
                affichage();
        }

    }else if(value == 9){

        if(clicEnd == 1){

            printf("\nNombres de cotes: %d", nbCote);
            if(nbCote >= 3){

                if(button == GLUT_RIGHT_BUTTON){
                    printf("\nDERNIERE CONDITION");
                    xa = xb;
                    ya = yb;
                    xb = destinationPointX;
                    yb = destinationPointY;
                    affichage();
                    nbCote = 0;
                    clicEnd = 0;

                }
            }
            //TRACE DE SEGMENT AVEC DEUX CLICS SOURIS
            //ENREGISTREMENT DU PREMIER POINT
            while(clicPolygon == 0){
                printf("\n1- valeur de clic = %d", clicPolygon);
                if(cnt % 2 == 0){
                    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                        xa = x0 - 250;
                        ya = -y0 + 250;
                        if(counter == 0){
                            destinationPointX = xa;
                            destinationPointY = ya;
                            printf("\nPremier point enregistre!\nxa = %d\n ya = %d\n", xa, ya);
                        }
                        counter = 1;

                    }
                    nbCote++;
                }
                if(cnt % 4 != 0){
                    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                        xb = x0 - 250;
                        yb = -y0 + 250;
                        affichage();

                    }
                    nbCote++;
                }


                cnt++;
                clicPolygon = 1;
            }
            //ON PART A PARTIR DU DERNIER POINT TRACE
            if(clicPolygon != 0){
                printf("\n2- valeur de clic = %d", clicPolygon);
                if(cnt % 4 != 0){
                    xa = xb;
                    ya = yb;
                    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                        xb = x0 - 250;
                        yb = -y0 + 250;
                        affichage();


                    }
                }
                nbCote++;
                cnt++;
                clicPolygon = 0;
            }

        }

    }

}
int arrondi(int e){
    e = e + 0.5;
    return e;
}
/*CodeSegment calculCode(int x, int y, int Xmin, int Xmax, int Ymin, int Ymax){

    //CALCULER LE CODE D'UN POINT XY
    codeXY.somme = 0;
    if(y > Ymax){
        codeXY.haut = 1;
        codeXY.somme++;
    }else if(y < Ymin){
        codeXY.bas = 1;
        codeXY.somme++;
    }
    if(x > Xmax){
        codeXY.droite = 1;
        codeXY.somme++;
    }else{
        codeXY.gauche = 1;
        codeXY.somme++;
    }
    return codeXY;
}
int compareStructure(CodeSegment one, CodeSegment two){
    return (one == 1 & two == 1);
}
int fenetrage(int xa, int xb, int ya, int yb, int Xmin, int Xmax, int Ymin, int Ymax){

    int accept = false;
    int fini = false;

    CodeSegment codeA = calculCode(xa, ya, Xmin, Xmax, Ymin, Ymax);
    CodeSegment codeB = calculCode(xb, yb, Xmin, Xmax, Ymin, Ymax);

    m = (yb - ya) / (xb - xa);

    do{
        if((codeA.somme) == 0 && (codeB.somme == 0)){
            accept = true;
            fini = true;
        }else{

            if(codeA & codeB ){
                fini = true;
            }else{
                codeExt = codeA;
                if(codeA.somme == 0){

                }
            }
            //CALCUL COTE HAUT
            if(codeExt.haut = 1){
                y = Ymax;
                x = xa + (Ymax - ya) / m;
            //CALCUL COTE BAS
            }else if(codeExt.bas = 1){
                y = Ymin;
                x = xa + (Ymin - ya) / m;
            //CALCUL COTE DROIT
            }else if(codeExt.droite = 1){
                x = Xmax;
                y = ya + (Xmax - xa) * m;
            //CALCUL COTE GAUCHE
            }else if(codeExt.gauche = 1){
                x = Xmin;
                y = ya + (Xmin - xa) * m;
            }

            //CALCUL DES INTERSECTIONS
            if(codeExt == codeA){
                xa = x;
                ya = y;
                codeA = calculCode(x, y, Xmin, Xmax, Ymin, Ymax);
            }else{
                xb = x;
                yb = y;
                codeB = calculCode(x, y, Xmin, Xmax, Ymin, Ymax);
            }
        }

    }while(fini != true);
    if(accept){
        //afficher segments
    }

}*/
