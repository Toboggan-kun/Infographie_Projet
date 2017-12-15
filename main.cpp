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
/*
//RGB PALETTE
#define RED 255
#define GREEN 255
#define BLUE 255*/

//DECLARATION DES VARIABLES
    //MENU
    int menu;
    int menu2;
    int menuColor;
    int color;
    int window;
    int windowColor;
    int numberMenu;
    int value = 1;
    int counter = 0;
    int change = 0;
    int drawSegment = 0;
    int changefenetrage = 0;

    int state;
int button;

    //CLIC SOURIS
    int x0, y0;
    int cnt = 0;
    //COORDONNEES DU SEGMENT [AB]
    double xa = 0;
    double ya = 0;
    double xb, yb;
    int dx, dy;
    int incrE, incrNE, incrX, incrY, di;

    //COORDONNEES DU CERCLE
    int xc, yc; //POINTS ORIGINE
    //POINTS DESTINATION
    int rx = 0;
    int ry = 0;
    int r; //RAYON
    int dp;
    int octantX;
    int octantY;

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
    int indexPolygone = 0;

    //FENETRAGE
    typedef struct{
        int somme;
        int gauche;
        int droite;
        int bas;
        int haut;
    }CodeSegment;


    double fenetrage_xa;
    double fenetrage_xb;
    double fenetrage_ya;
    double fenetrage_yb;

    CodeSegment codeA;
    CodeSegment codeB;
    CodeSegment codeExt;
    CodeSegment codeXY;
    int accepte = 0;
    int fini = 0;
    int fenetrageTester = 0;

    double Xmin, Xmax, Ymin, Ymax; //COORDONNEES DE LA FENETRE
    //VARIABLES INTERMEDIAIRES
    int p;
    double m;
    double x = 0;
    double y = 0;
    int c; //COULEUR

    //COULEUR
    int i, j;
    int R[255];
    int G[255];
    int B[255];
    int palette[1280][3];
    int index = 0;


    double windows_xa;
    double windows_ya;
    double windows_xb;
    double windows_yb;


    double red;
    double green;
    double blue;


//PROTOTYPES DE FONCTIONS
void menuInterface(int num);
void clearConsole(void);
//void pickColor(int x0, int y0, int c);
//void menuColor(int numberMenu);
void pickColor(void);
void mouse(int button, int state, int x0, int y0);
void affichePixel(double x, double y);
void affichage(void);
void bresemham_cercle(int xc, int yc, int r);
void bresemham_segment(double xa, double xb, double ya, double yb);
void calculCode(void);
void fenetrage(double, double, double, double, double, double, double, double);
int compareStructure(CodeSegment, CodeSegment);
void polygon_trace(int xa, int xb, int ya, int yb);
void keyboard(unsigned char touch, int x, int y);
void myLastPoint(int destinationPointX, int destinationPointY);


typedef struct Color Color;
struct Color
{
    int r;
    int g;
    int b;
};


// DEFINITION DE L'ELEMENT DE LA LISTE CHAINE DES SEGMENTS
typedef struct Segment Segment;
struct Segment
{
    double xa;
    double ya;
    double xb;
    double yb;
    Segment *next;
};


// DEFINITION DE LA LISTE CHAINE DES SEGMENTS
typedef struct SegmentList SegmentList;
struct SegmentList
{
    Segment *first;
};


//INITIALISATION DE LA LISTE CHAINE
SegmentList *initialisationSegment(){
    SegmentList *listsegment = (SegmentList *)malloc(sizeof(*listsegment));

    if(listsegment == NULL){
        exit(EXIT_FAILURE);
    }

    listsegment->first = NULL;

    return listsegment;
}

// FONCTION D'AJOUT D'UN ELEMENT DE LA LISTE CHAINEE
void addSegment(SegmentList *listsegment, double xa, double ya, double xb, double yb){
    /* On crée un nouvel élément */
    Segment* newSegment = (Segment *)malloc(sizeof(*newSegment));

    /* On assigne la valeur au nouvel élément */
    newSegment->xa = xa;
    newSegment->ya = ya;
    newSegment->xb = xb;
    newSegment->yb = yb;

    /* On ajoute en fin, donc aucun élément ne va suivre */
    newSegment->next = NULL;

    if(listsegment->first == NULL){
        /* Si la liste est videé il suffit de renvoyer l'élément créé */
        listsegment->first = newSegment;
    }else{
        /* Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
        indique que le dernier élément de la liste est relié au nouvel élément */
        Segment* temp = listsegment->first;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = newSegment;
    }
}

// AFFICHAGE DES ELEMENTS DE LA LISTE CHAINEE DES SEGMENTS
void displaySegmentList(SegmentList *listsegment){
    if(listsegment == NULL){
        exit(EXIT_FAILURE);
    }
    Segment *actual = listsegment->first;

    while (actual != NULL){

        printf("xa: %.2lf, ya: %.2lf, xb: %.2lf, yb: %.2lf -> ", actual->xa, actual->ya, actual->xb, actual->yb);
        actual = actual->next;
    }
    printf("NULL\n");
}


// DEFINITION DE L'ELEMENT DE LA LISTE CHAINE DES SEGMENTS
typedef struct Circle Circle;
struct Circle{
    int x;
    int y;
    int r;
    Circle *next;
};


// DEFINITION DE LA LISTE CHAINE DES SEGMENTS
typedef struct CircleList CircleList;
struct CircleList{
    Circle *first;
};
//INITIALISATION DE LA LISTE CHAINE
CircleList *initialisationCircle(){
    CircleList *listcircle = (CircleList *)malloc(sizeof(*listcircle));

    if(listcircle == NULL){
        exit(EXIT_FAILURE);
    }
    listcircle->first = NULL;

    return listcircle;
}

// FONCTION D'AJOUT D'UN ELEMENT DE LA LISTE CHAINEE
void addCircle(CircleList *listcircle, int x, int y, int r){
    /* On crée un nouvel élément */
    Circle* newCircle = (Circle *)malloc(sizeof(*newCircle));

    /* On assigne la valeur au nouvel élément */
    newCircle->x = x;

    /* On ajoute en fin, donc aucun élément ne va suivre */
    newCircle->y = y;
    newCircle->r = r;
    newCircle->next = NULL;

    if(listcircle->first == NULL){
        /* Si la liste est videé il suffit de renvoyer l'élément créé */
        listcircle->first = newCircle;
    }else{
        /* Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
        indique que le dernier élément de la liste est relié au nouvel élément */
        Circle* temp = listcircle->first;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = newCircle;
    }
}


// AFFICHAGE DES ELEMENTS DE LA LISTE CHAINEE
void displayCircleList(CircleList *listcircle){
    if (listcircle == NULL){
        exit(EXIT_FAILURE);
    }

    Circle *actual = listcircle->first;

    while (actual != NULL){
        printf("x: %d, y: %d, r: %d -> ", actual->x, actual->y, actual->r);
        actual = actual->next;
    }
    printf("NULL\n");
}





Color *initialisationColor(){
    Color *listcolor = (Color *)malloc(sizeof(*listcolor));

    if(listcolor == NULL){
        exit(EXIT_FAILURE);
    }
    listcolor->r = 255;
    listcolor->g = 255;
    listcolor->b = 255;

    return listcolor;
}




typedef struct Polygone Polygone;
struct Polygone{
    int xa;
    int ya;
    int xb;
    int yb;
    int polygonIdentifier;
    Polygone *next;
};


typedef struct PolygonList PolygonList;
struct PolygonList
{
    Polygone *first;
};


//INITIALISATION DE LA LISTE CHAINE
PolygonList *initialisationPolygon(){
    PolygonList *listpolygon = (PolygonList *)malloc(sizeof(*listpolygon));

    if(listpolygon == NULL){
        exit(EXIT_FAILURE);
    }

    listpolygon->first = NULL;

    return listpolygon;
}


// FONCTION D'AJOUT D'UN ELEMENT DE LA LISTE CHAINEE
void addPolygon(PolygonList *listpolygon, int xa, int ya, int xb, int yb,int index){
    /* On crée un nouvel élément */
    Polygone* newPolygon = (Polygone *)malloc(sizeof(*newPolygon));

    /* On assigne la valeur au nouvel élément */
    newPolygon->xa = xa;
    newPolygon->ya = ya;
    newPolygon->xb = xb;
    newPolygon->yb = yb;
    newPolygon->polygonIdentifier = index;

    /* On ajoute en fin, donc aucun élément ne va suivre */
    newPolygon->next = NULL;

    if(listpolygon->first == NULL){
        /* Si la liste est videé il suffit de renvoyer l'élément créé */
        listpolygon->first = newPolygon;
    }else{
        /* Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
        indique que le dernier élément de la liste est relié au nouvel élément */
        Polygone* temp = listpolygon->first;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = newPolygon;
    }
}




// AFFICHAGE DES ELEMENTS DE LA LISTE CHAINEE DES SEGMENTS
void displayPolygonList(PolygonList *listpolygon){
    if(listpolygon == NULL){
        exit(EXIT_FAILURE);
    }
    Polygone *actual = listpolygon->first;

    while (actual != NULL){
        printf("xa: %d, ya: %d, xb: %d, yb: %d, index : %d -> ", actual->xa, actual->ya, actual->xb, actual->yb, actual->polygonIdentifier);
        actual = actual->next;
    }
    printf("NULL\n");
}




SegmentList *listOfSegment = initialisationSegment();
SegmentList *tempSegmentList = initialisationSegment();
CircleList *listOfCircle = initialisationCircle();
PolygonList *listOfPolygon = initialisationPolygon();
Color *couleur = initialisationColor();

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
        glutKeyboardFunc(keyboard); // FERMETURE DU POLYGONE GRACE AU CLAVIER
        //ENREGISTREMENT DES FONCTIONS D'APPELS
        glutDisplayFunc(affichage);
        glutMouseFunc(mouse);

        //GESTION DU MENU
        menu = glutCreateMenu(menuInterface);
        if(clicEnd == 0){

            glutAddMenuEntry("Segment", 1);
            glutAddMenuEntry("Cercle", 2);
            glutAddMenuEntry("Ellipse", 3);
            glutAddMenuEntry("Fenêtrage", 4);
            glutAddMenuEntry("Effacer la console", 5);
            glutAddMenuEntry("Effacer un segment", 6);
            glutAddMenuEntry("Effacer un cercle", 7);
            glutAddMenuEntry("Arc de cercle", 10);
            glutAddMenuEntry("Polygone", 9);
            glutAddMenuEntry("Quitter le programme", 8);

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
        //glutMouseFunc(mouseColor);
        //ENREGISTREMENT DES FONCTIONS D'APPELS
        glutDisplayFunc(pickColor);

        /*menu2 = glutCreateMenu(menuColor);
        glutAddMenuEntry("Couleurs", menuColor);
        glutAddSubMenu("Rouge", 11);
        glutAddSubMenu("Fuchsia", 12);
        glutAddSubMenu("Bleu Foncé", 13);
        glutAddSubMenu("Bleu Clair", 14);
        glutAddSubMenu("Vert", 15);
        glutAddSubMenu("Jaune", 16);*/

        //glutMouseFunc(pickColor);
        /*//INITIALISATION DES TABLEAUX R G B
        for(i = 0; i <= 255; i++){
            R[i] = i;
            G[i] = i;
            B[i] = i;
        }*/

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
/*void menuColor(int numberMenu){
    if(menu2 == 0){
        glutDestroyWindow(windowColor);
        exit(0);
    }else{
        color = numberMenu;
    }
    glutPostRedisplay();
}*/
//POUR LE CHOIX DE COULEUR
void pickColor(){

    //red = arrondi(color->r * 250);

    glClear(GL_COLOR_BUFFER_BIT);



    glBegin(GL_POLYGON);

            /* Spectre couleur ROUGE */
            glColor3f(1.0,0.0,0.0); //ROUGE
            glVertex2f(-250, -250);
            glColor3f(1.0,0.0,0.0); //ROUGE
            glVertex2f(-150, -250);
            glColor3f(1.0,1.0,1.0); //ROUGE
            glVertex2f(-150, 250);
            glColor3f(1.0,1.0,1.0); //ROUGE
            glVertex2f(-250, 250);
glFlush();
glEnd();

glBegin(GL_POLYGON);

            /* Spectre couleur VERT */
            glColor3f(1.0,1.0,1.0); //VERT
            glVertex2f(-100.0, -250.0);
            glColor3f(1.0,1.0,1.0); //VERT
            glVertex2f(0.0, -250.0);
            glColor3f(0.0,1.0,0.0); //VERT
            glVertex2f(0.0, 250.0);
            glColor3f(0.0,1.0,0.0); //VERT
            glVertex2f(-100.0, 250.0);

glFlush();
glEnd();

glBegin(GL_POLYGON);

            /* Spectre couleur VERT */
            glColor3f(1.0,1.0,1.0); //VERT
            glVertex2f(50.0, -250.0);
            glColor3f(1.0,1.0,1.0); //VERT
            glVertex2f(150.0, -250.0);
            glColor3f(0.0,0.0,1.0); //VERT
            glVertex2f(150.0, 250.0);
            glColor3f(0.0,0.0,1.0); //VERT
            glVertex2f(50.0, 250.0);
         /*   glColor3f(0.0,0.0,1.0); //BLEU
            glVertex2f(-250.0, 250.0);
            glColor3f(1.0,0.0,0.0); //ROUGE
            glVertex2f(250.0, 250.0);*/
    /*switch(color){
    case 10: //ROUGE

            glBegin(GL_POLYGON);
            glColor3f(0.0,0.0,0.0); //NOIR
            glVertex2f(-250.0, -250.0);
            glColor3f(0.0,0.0,0.0); //NOIR
            glVertex2f(-250.0, 250.0);
            glColor3f(1.0,0.0,0.0); //ROUGE
            glVertex2f(250.0, 250.0);
            glColor3f(1.0,1.0,1.0); //BLANC
            glVertex2f(250.0, -250.0);

        break;
    case 11: //FUCHSIA
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_POLYGON);
            glColor3f(0.0,0.0,0.0); //NOIR
            glVertex2f(-250.0, -250.0);
            glColor3f(0.0,0.0,0.0); //NOIR
            glVertex2f(-250.0, 250.0);
            glColor3f(1.0,0.0,1.0); //ROUGE
            glVertex2f(250.0, 250.0);
            glColor3f(1.0,1.0,1.0); //BLANC
            glVertex2f(250.0, -250.0);

        break;

    }
*/
    glEnd();
    glFlush();
}


void affichage(){

    if(value == 1 && change == 1){ //SEGMENT
        bresemham_segment(xa, xb, ya, yb);
        addSegment(listOfSegment,xa,ya,xb,yb);
        displaySegmentList(listOfSegment);

    }else if(value == 2){ //CERCLE
        bresemham_cercle(xc, yc, r);
        addCircle(listOfCircle,xc,yc,r);
        xc = 0;
        yc = 0;
        r = 0;

    }else if(value == 3){

    }else if(value == 4 && changefenetrage == 1){ //DECOUPAGE



        fenetrageTester = 1;

        Segment *actualSegment = listOfSegment->first;

        while (actualSegment != NULL){
            fenetrage_xa = actualSegment->xa;
            fenetrage_ya = actualSegment->ya;
            fenetrage_xb = actualSegment->xb;
            fenetrage_yb = actualSegment->yb;



            fenetrage(fenetrage_xa, fenetrage_xb, fenetrage_ya, fenetrage_yb, windows_xa, windows_xb, windows_yb, windows_ya);





            actualSegment = actualSegment->next;
        }


        fenetrageTester = 0;

    }else if(value == 5){ //EFFACER CONSOLE
        clearConsole();
        counter = 0;
        value = 0;
        change = 0;
        listOfSegment = initialisationSegment();
        listOfCircle = initialisationCircle();
        listOfPolygon = initialisationPolygon();
        indexPolygone = 0;

    }else if(value == 6){ //EFFACER SEGMENT
        exit(0);
    }else if(value == 7){ //EFFACER CERCLE
        exit(0);

    }else if(value == 8){ //QUITTER LE PROGRAMME
        exit(0);

    }else if(value == 9){ // POLYGONE

        clicEnd = 1;
        if(drawSegment >= 2){
            bresemham_segment(xa, xb, ya, yb);
            addPolygon(listOfPolygon,xa,ya,xb,yb, indexPolygone);
            displayPolygonList(listOfPolygon);
        }


    }else if(value == 10){

        r = 0;
    }else if(value == 1 && change == 0){
        change = 1;

    }else if(value == 4 && changefenetrage == 0){
        changefenetrage = 1;

    }

    glFlush();

}
void clearConsole(){
    glClear(GL_COLOR_BUFFER_BIT);
}
void affichePixel(double x, double y){
    //glClear(GL_COLOR_BUFFER_BIT);




        if (fenetrageTester == 1){
            glColor3f(1.0,0.0,0.0); //ROUGE
            glPointSize(2.5); //TAILLE D'UN POINT: 2px
        } else {
            glColor3f(1.0,1.0,1.0); //BLANC
            glPointSize(2.0); //TAILLE D'UN POINT: 2px
        }

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

void bresemham_segment(double xa, double xb, double ya, double yb){
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
    //REGION 1
    x = 0;
    y = ry;
    while(y <= 0){
        //PREMIER CADRAN
        affichePixel(x + xc, y + yc); //POINT 1: OCTANT 1
        affichePixel(-x + xc, y + yc); //POINT 2: OCTANT 4
        affichePixel(-x + xc, -y + yc); //POINT 3: OCTANT 5
        affichePixel(x + xc, -y + yc); //POINT 4: OCTANT 8
    }



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
                    //printf("\noctant 5");
                    r = rx; //OCTANT 5
                }else{
                    //printf("\noctant 6");
                    r = ry; //OCTANT 6
                }

            }
            //OCTANTS 7/8
            else if(rx >= 0 && ry <= 0){
                ry = -ry;
                if(rx >= ry){
                    //printf("\noctant 8");
                    r = rx; //OCTANT 8
                }else if(ry >= rx){
                    //printf("\noctant 7");
                    r = ry; //OCTANT 7
                }

            }
            //OCTANTS 3/4
            else if(rx <= 0 && ry >= 0){
                rx = -rx;
                if(rx >= ry){
                    //printf("\noctant 4");
                    r = rx; //OCTANT 4
                }else{
                    //printf("\noctant 3");
                    r = ry; //OCTANT 3
                }
            }
            //OCTANTS 1/2
             else if(rx >= 0 && ry >= 0){
                if(rx >= ry){
                    //printf("\noctant 1");
                    r = rx; //OCTANT 1
                }else{
                    //printf("\noctant 2");
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
                windows_xa = x0 - 250;
                windows_ya = -y0 + 250;
        }
        if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
                windows_xb = x0 - 250;
                windows_yb = -y0 + 250;
                glBegin(GL_LINES);

                bresemham_segment(windows_xa, windows_xb, windows_ya, windows_ya);
                bresemham_segment(windows_xb, windows_xb, windows_ya, windows_yb);
                bresemham_segment(windows_xb, windows_xa, windows_yb, windows_yb);
                bresemham_segment(windows_xa, windows_xa, windows_yb, windows_ya);
                affichage();



        }

    }else if(value == 9){
        if(clicEnd == 1){
            //TRACE DE SEGMENT AVEC DEUX CLICS SOURIS
            //ENREGISTREMENT DU PREMIER POINT
            while(clicPolygon == 0){
                //printf("\n1- valeur de clic = %d", clicPolygon);
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
                }
                if(cnt % 4 != 0){
                    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                        xb = x0 - 250;
                        yb = -y0 + 250;

                        affichage();

                    }

                }

                cnt++;
                clicPolygon = 1;
            }
            //ON PART A PARTIR DU DERNIER POINT TRACE
            if(clicPolygon != 0){
                if(cnt % 4 != 0){
                    xa = xb;
                    ya = yb;

                    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                        xb = x0 - 250;
                        yb = -y0 + 250;
                        drawSegment++;
                        affichage();

                    }
                }
                cnt++;
                clicPolygon = 0;
            }

        }

    }else if(value == 10){
        if(counter % 2 == 0){
            if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                printf("\nPOINT ORIGINE: ");
                xc = x0 - 250;
                printf("\nxc = %d", xc);
                yc = -y0 + 250;
                printf("\nyc = %d", yc);
                printf("\ncounter = %d", counter);
            }
        }
        if(counter % 4 == 0){

            if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
                rx = 0;
                ry = 0;
                printf("\nPOINT D'ARRIVEE/RAYON: ");
                //MOUVEMENT GAUCHE-DROITE
                rx = (x0 - xc) - 250;
                //printf("\nx0 = %d", x0);
                printf("\nrx = %d", rx);
                ry = (-y0 - yc) + 250;
                //printf("\ny0 = %d", y0);
                printf("\nry = %d", ry);
                printf("\ncounter2 = %d", counter);

                //DETERMINER LE RAYON DU CERCLE/ARC DE CERCLE
                //OCTANTS 5/6
                if(rx <= 0 && ry <= 0){
                    rx = -rx;
                    ry = -ry;
                    if(rx >= ry){
                        //printf("\noctant 5");
                        r = rx; //OCTANT 5
                    }else{
                        //printf("\noctant 6");
                        r = ry; //OCTANT 6
                    }

                }
                //OCTANTS 7/8
                else if(rx >= 0 && ry <= 0){
                    ry = -ry;
                    if(rx >= ry){
                        //printf("\noctant 8");
                        r = rx; //OCTANT 8
                    }else if(ry >= rx){
                        //printf("\noctant 7");
                        r = ry; //OCTANT 7
                    }

                }
                //OCTANTS 3/4
                else if(rx <= 0 && ry >= 0){
                    rx = -rx;
                    if(rx >= ry){
                        //printf("\noctant 4");
                        r = rx; //OCTANT 4
                    }else{
                        //printf("\noctant 3");
                        r = ry; //OCTANT 3
                    }
                }
                //OCTANTS 1/2
                 else if(rx >= 0 && ry >= 0){
                    if(rx >= ry){
                        //printf("\noctant 1");
                        r = rx; //OCTANT 1
                    }else{
                        //printf("\noctant 2");
                        r = ry; //OCTANT 2
                    }
                }
            }
        }


        if(counter % 6 == 0){
            if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                printf("\nSUR QUEL OCTANT TRACER: \n");
                octantX = x0 - 250;
                octantY = -y0 + 250;
                printf("\noctantX = %d octantY = %d", octantX, octantY);

                //OCTANTS 5/6
                if(octantX <= 0 && octantY <= 0){
                    octantX = -octantX;
                    octantY = -octantY;
                    if(octantX >= octantY){
                        printf("\noctant 5");

                    }else{
                        printf("\noctant 6");
                    }

                }
                //OCTANTS 7/8
                else if(octantX >= 0 && octantY <= 0){
                    ry = -ry;
                    if(octantX >= octantY){
                        printf("\noctant 8");

                    }else if(octantY >= octantX){
                        printf("\noctant 7");

                    }

                }
                //OCTANTS 3/4
                else if(octantX <= 0 && octantY >= 0){
                    octantX = -octantX;
                    if(octantX >= octantY){
                        printf("\noctant 4");

                    }else{
                        printf("\noctant 3");
                    }
                }
                //OCTANTS 1/2
                 else if(rx >= 0 && octantY >= 0){
                    if(rx >= octantY){
                        printf("\noctant 1");
                    }else{
                        printf("\noctant 2");
                    }
                 }
            }
        }
        counter++;
    }

}




void mouseColor(int button, int state, int x0, int y0){

}










int arrondi(int e){
    e = e + 0.5;
    return e;
}

CodeSegment calculCode(double x, double y, double Xmin, double Xmax, double Ymin, double Ymax){
    codeXY.somme = 0;
    codeXY.haut = 0;
    codeXY.bas = 0;
    codeXY.gauche = 0;
    codeXY.droite = 0;



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
    }else if (x < Xmin){
        codeXY.gauche = 1;
        codeXY.somme++;
    }




    return codeXY;
}


void creerFenetre(int Xmin, int Ymin, int Xmax, int Ymax){
    bresemham_segment(Xmin, Xmax, Ymin, Ymin);
    bresemham_segment(Xmin, Xmin, Ymin, Ymax);
    bresemham_segment(Xmin, Xmax, Ymax, Ymax);
    bresemham_segment(Xmax, Xmax, Ymin, Ymax);



}


void fenetrage(double xa, double xb, double ya, double yb, double Xmin, double Xmax, double Ymin, double Ymax){

    printf("\n\nCode A");

    codeA = calculCode(xa, ya, Xmin, Xmax, Ymin, Ymax);
    printf("\n\nCode B");
    codeB = calculCode(xb, yb, Xmin, Xmax, Ymin, Ymax);
    m = (double)(yb - ya) / (xb - xa);

    accepte = 0;
    fini = 0;


    fenetrageTester = 1;



    do{
        if(codeA.somme == 0 && codeB.somme == 0){
            accepte = 1;
            fini = 1;
        }else{
            if ( (codeA.haut == 1 && codeB.haut == 1) || (codeA.bas == 1 && codeB.bas == 1) || (codeA.gauche == 1 && codeB.gauche == 1) || (codeA.droite == 1 && codeB.droite == 1) ) {
                fini = 1;
            } else {
                codeExt = codeA;
                if (codeA.somme == 0) {
                    codeExt = codeB;
                }

                if (codeExt.haut == 1) {
                    y = Ymax;
                    x = xa + (Ymax - ya) / m;
                } else if (codeExt.bas == 1){
                    y = Ymin;
                    x = xa + (Ymin - ya) / m;
                } else if (codeExt.droite == 1){
                    x = Xmax;
                    y = ya + (Xmax - xa) * m;
                } else if (codeExt.gauche == 1){
                    x = Xmin;
                    y = ya + (Xmin - xa) * m;

                }

                if ( (codeExt.haut == codeA.haut) && (codeExt.bas == codeA.bas) && (codeExt.gauche == codeA.gauche) && (codeExt.droite == codeA.droite) ){
                    xa = x;
                    ya = y;
                    codeA = calculCode(x,y,Xmin,Xmax,Ymin,Ymax);
                } else {
                    xb = x;
                    yb = y;
                    codeB = calculCode(x,y,Xmin,Xmax,Ymin,Ymax);
                }
            }


        }


    }while(fini != 1);
    if(accepte == 1){

        printf("\n\nXa = %d",xa);
    printf("\nYa = %d",ya);
    printf("\nXb = %d",xb);
    printf("\nYb = %d",yb);
        bresemham_segment(xa, xb, ya, yb);


    }

    fenetrageTester = 0;
}





void arcSegment(int xc, int yc, int r){
    if(counter != 0){

        x = 0;
        y = 30;
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






void myLastPoint(int destinationPointX, int destinationPointY){
    if(value == 9){

        xa = xb;
        ya = yb;
        xb = destinationPointX;
        yb = destinationPointY;
        affichage();
        drawSegment = 0;
        counter = 0;
        indexPolygone++;
    }

}
void keyboard(unsigned char touch, int x, int y){
    switch(touch){
    case 'c':
        myLastPoint(destinationPointX, destinationPointY);
        break;

    }
}
