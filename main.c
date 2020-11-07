#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <limits.h>
#include <SDL2/SDL.h>

const int x = 100;
const int y = 100;
const int w = 1000;
const int h = 800;

typedef struct rect
{
    int x;
    int y;
    SDL_Rect *rect;
    struct rect *next;
} rectangleElement;

typedef struct line
{
    int x1;
    int x2;
    int y1;
    int y2;
    struct line *next;
} lineElement;

rectangleElement *createRectangleElement(SDL_Rect *rect)
{
    rectangleElement *newElement;
    newElement = (rectangleElement *)malloc(sizeof(rectangleElement));
    newElement->next = NULL;
    newElement->rect = rect;
    return newElement;
}

void insertBeginingRec(rectangleElement **head, SDL_Rect *rect)
{
    rectangleElement *newElement;
    newElement = createRectangleElement(rect);
    newElement->next = (*head);
    *head = newElement;
}

lineElement *createLineElement(int x1, int y1, int x2, int y2)
{
    lineElement *l;
    l = (lineElement *)malloc(sizeof(lineElement));
    l->x1 = x1;
    l->y1 = y1;
    l->x2 = x2;
    l->y2 = y2;
    l->next = NULL;
    return l;
}

void insertBeginingLine(lineElement **head, int x1, int y1, int x2, int y2)
{
    lineElement *elem = createLineElement(x1, y1, x2, y2);
    elem->next = (*head);
    (*head) = elem;
}

void drawLines(SDL_Renderer *renderer, lineElement *head)
{
    lineElement *p = head;
    while (p)
    {
        SDL_RenderDrawLine(renderer, p->x1 + 25, p->y1 + 25, p->x2 + 25, p->y2 + 25);
        p = p->next;
    }
}

void drawRectangles(SDL_Renderer *renderer, rectangleElement *head)
{
    rectangleElement *p;
    for (p = head; p != NULL; p = p->next)
        SDL_RenderFillRect(renderer, p->rect);
}

typedef struct nd
{
    int x;
    int y;
    int nbr_dest;
    struct nd **dest;
    int *arc;
    int index;
} node;

typedef struct
{
    node **nd;
    int nbr_nd;
} graph;

node *Create_Node(int x, int y)
{
    node *nd;
    nd = (node *)malloc(sizeof(node));
    nd->x = x;
    nd->y = y;
    nd->dest = NULL;
    nd->arc = NULL;
    nd->nbr_dest = 0;
    return nd;
}

graph *Create_Graph()
{
    graph *g;
    g = (graph *)malloc(sizeof(graph));
    g->nd = NULL;
    g->nbr_nd = 0;
    return g;
}

void Update_Graph(graph *Graph, int x, int y)
{
    Graph->nbr_nd++;
    Graph->nd = (node **)realloc(Graph->nd, Graph->nbr_nd * sizeof(node *));
    Graph->nd[Graph->nbr_nd - 1] = Create_Node(x, y);
    Graph->nd[Graph->nbr_nd - 1]->index = Graph->nbr_nd - 1;
}

void Display_Node(node *n)
{
    printf("-----------\n");
    if (n)
    {
        printf("x = %d\n", n->x);
        printf("y = %d\n", n->y);
        printf("index = %d\n", n->index);
        printf("nbr_dest = %d\n", n->nbr_dest);
    }
    printf("-----------\n");
}

void Ori_Linking_Nodes(node *n1, node *n2, int cout)
{
    n1->nbr_dest++;
    n1->dest = (node **)realloc(n1->dest, n1->nbr_dest * sizeof(node *));
    n1->dest[n1->nbr_dest - 1] = n2;
    n1->arc = (int *)realloc(n1->arc, n1->nbr_dest * sizeof(int *));
    n1->arc[n1->nbr_dest - 1] = cout;
}

void None_Ori_Linking_Nodes(node *n1, node *n2, int cout)
{
    n1->nbr_dest++;
    n2->nbr_dest++;
    n1->dest = (node **)realloc(n1->dest, n1->nbr_dest * sizeof(node *));
    n2->dest = (node **)realloc(n2->dest, n2->nbr_dest * sizeof(node *));
    n1->dest[n1->nbr_dest - 1] = n2;
    n2->dest[n2->nbr_dest - 1] = n1;
    n1->arc = (int *)realloc(n1->arc, n1->nbr_dest * sizeof(int *));
    n2->arc = (int *)realloc(n2->arc, n2->nbr_dest * sizeof(int *));
    n1->arc[n1->nbr_dest - 1] = cout;
    n2->arc[n2->nbr_dest - 1] = cout;
}

void Display_Linked_Node(node *n)
{
    if (n && n->nbr_dest > 0)
    {
        printf("les noeuds liees au noeud (%d , %d) \n", n->x, n->y);
        for (int i = 0; i < n->nbr_dest; i++)
        {
            Display_Node(n->dest[i]);
            printf("Le cout est : %d\n", n->arc[i]);
        }
    }
}

//affiche les sommets liees entre eux
void Display_Linked_Graph(graph *Graph)
{
    for (int i = 0; i < Graph->nbr_nd; i++)
    {
        Display_Linked_Node(Graph->nd[i]);
    }
}

//affiche tous les sommets du graph
void Display_Graph(graph *Graph)
{
    printf("----------------------------------les noeuds du graphe----------------------------------\n");
    for (int i = 0; i < Graph->nbr_nd; i++)
        Display_Node(Graph->nd[i]);
    printf("\n---------------------------------------------------------------------------------------\n\n");
}

void Init_Array(int **t, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            t[i][j] = INT_MAX - 100;
        }
    }
}

int min(int a, int b)
{
    if (a < b)
        return a;
    return b;
}

int Bellman_Ford_Iter(graph *Graph, int dep, int arr)
{
    int **plusCourChemin;
    plusCourChemin = (int **)malloc(Graph->nbr_nd * sizeof(int *));
    for (int i = 0; i < Graph->nbr_nd; i++)
    {
        plusCourChemin[i] = (int *)malloc(Graph->nbr_nd * sizeof(int));
    }
    Init_Array(plusCourChemin, Graph->nbr_nd);

    int cheminMin, cheminVoisin;
    plusCourChemin[0][arr] = 0;
    for (int i = 1; i < Graph->nbr_nd; i++)
    {
        for (int j = 0; j < Graph->nbr_nd; j++)
        {
            cheminMin = plusCourChemin[i - 1][j];
            for (int k = 0; k < Graph->nd[j]->nbr_dest; k++)
            {
                cheminVoisin = Graph->nd[j]->arc[k] + plusCourChemin[i - 1][Graph->nd[j]->dest[k]->index];
                cheminMin = min(cheminMin, cheminVoisin);
            }
            plusCourChemin[i][j] = cheminMin;
        }
    }
    return plusCourChemin[Graph->nbr_nd - 1][dep];
}

int Get_Index_By_Coordonnee(graph *Graph, int x, int y)
{
    for (int i = 0; i < Graph->nbr_nd; i++)
    {
        if (Graph->nd[i]->x == x && Graph->nd[i]->y == y)
            return Graph->nd[i]->index;
    }
    return -1;
}

void Add_Arc(graph *Graph, char p, lineElement **head)
{
    int x1, x2, y1, y2, cout;
    int index_1, index_2;
    printf("Entrez le coordonnee du predecesseur : ");
    scanf("%d %d", &x1, &y1);
    index_1 = Get_Index_By_Coordonnee(Graph, x1, y1);
    while (index_1 == -1)
    {
        printf("Veuillez saisir des coordonnees qui existent !!!\n");
        printf("Entrez les coordonnees du source : ");
        scanf("%d %d", &x1, &y1);
        index_1 = Get_Index_By_Coordonnee(Graph, x1, y1);
    }
    printf("Entrez le coordonnee du successeur : ");
    scanf("%d %d", &x2, &y2);
    index_2 = Get_Index_By_Coordonnee(Graph, x2, y2);
    while (index_2 == -1)
    {
        printf("Veuillez saisir des coordonnees qui existent !!!\n");
        printf("Entrez les coordonnees du noeud d arrivee : ");
        scanf("%d %d", &x2, &y2);
        index_2 = Get_Index_By_Coordonnee(Graph, x2, y2);
    }
    insertBeginingLine(head, x1, y1, x2, y2);
    printf("entrer le cout : ");
    scanf("%d", &cout);
    if (p == 'o')
        Ori_Linking_Nodes(Graph->nd[index_1], Graph->nd[index_2], cout);
    else
        None_Ori_Linking_Nodes(Graph->nd[index_1], Graph->nd[index_2], cout);
}

void plus_Court_Chemin(graph *Graph)
{
    printf("\n\n----- Calcul du plus court chemin -----\n");
    int index_1, index_2;
    int x1, x2, y1, y2;
    char c;
    do
    {
        printf("Entrez c pour calculer un plus court chemin et x pour sortir : ");
        scanf(" %c", &c);
        if (c == 'c')
        {
            printf("Entrez les coordonnees du source : ");
            scanf("%d %d", &x1, &y1);
            index_1 = Get_Index_By_Coordonnee(Graph, x1, y1);
            while (index_1 == -1)
            {
                printf("Veuillez saisir des coordonnees qui existent !!!\n");
                printf("Entrez les coordonnees du source : ");
                scanf("%d %d", &x1, &y1);
                index_1 = Get_Index_By_Coordonnee(Graph, x1, y1);
            }
            printf("Entrez les coordonnees du noeud d arriver : ");
            scanf("%d %d", &x2, &y2);
            index_2 = Get_Index_By_Coordonnee(Graph, x2, y2);
            while (index_2 == -1)
            {
                printf("Veuillez saisir des coordonnees qui existent !!!\n");
                printf("Entrez les coordonnees du noeud d arriver : ");
                scanf("%d %d", &x2, &y2);
                index_2 = Get_Index_By_Coordonnee(Graph, x2, y2);
            }
            printf("le cout du plus court chemin est : %d\n", Bellman_Ford_Iter(Graph, index_1, index_2));
        }
        if (c == 'x')
            exit(1);
    } while (1);
}

void Add_Arcs(graph *Graph, char p, lineElement **headLine)
{
    char c;
    printf("Entrez a pour ajouter un arc et c pour calculer un plus court chemin: ");
    scanf(" %c", &c);
    if (c == 'a')
    {
        Add_Arc(Graph, p, headLine);
    }
    if (c == 'c')
        plus_Court_Chemin(Graph);
}

int main(int argc, char *argv[])
{
    graph *Graph;
    Graph = Create_Graph();
    rectangleElement *headRectangles = NULL;
    lineElement *headLine = NULL;
    char c, p;

    printf("Si le graphe est oriente entrez o sinon entrez n : ");
    scanf(" %c", &p);
    getchar();

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Unable to initialize SDL: %s", SDL_GetError());
        exit(-1);
    }

    SDL_Window *window;
    window = SDL_CreateWindow("Hamza", x, y, w, h, SDL_WINDOW_OPENGL);
    if (window == NULL)
    {
        printf("error");
        exit(-1);
    }

    SDL_Event windowEvent;

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    while (1)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
                break;
            if (windowEvent.type == SDL_MOUSEBUTTONDOWN)
            {
                if (windowEvent.button.button == SDL_BUTTON_LEFT)
                {

                    SDL_Rect *rect;
                    rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
                    rect->x = windowEvent.button.x;
                    rect->y = windowEvent.button.y;
                    rect->h = 50;
                    rect->w = 50;
                    Update_Graph(Graph, rect->x, rect->y);
                    insertBeginingRec(&headRectangles, rect);
                    printf("( %d , %d )\n", rect->x, rect->y);
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 51, 146, 255, 255);
        drawRectangles(renderer, headRectangles);
        SDL_RenderPresent(renderer);
    }

    if (Graph->nbr_nd == 0)
    {
        printf("Empty gaph !!!\n");
        return;
    }
    Display_Graph(Graph);

    SDL_DestroyWindow(window);

    window = SDL_CreateWindow("Hamza", x, y, w, h, SDL_WINDOW_OPENGL);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    while (1)
    {
        if (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT)
                break;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        drawLines(renderer, headLine);
        SDL_SetRenderDrawColor(renderer, 51, 146, 255, 255);
        drawRectangles(renderer, headRectangles);
        SDL_RenderPresent(renderer);
        Add_Arcs(Graph, p, &headLine);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    Display_Graph(Graph);

    system("pause");
    return 0;
}
