/*********
 * Name: VY PHAM
 * ID: vyapham
 * Student ID #: 1593394
 * Assignment name: pa5
 ********/

#include<stdio.h>
#include<stdlib.h>
#include "Graph.h"

#define WHITE 0
#define GRAY 1
#define BLACK 2

typedef struct GraphObj {
	List* L;		// contains neighbors of vertex i
	int* color;		// white = 0; gray = 1; black = 2;
	int* parent;	// parents of vertex i
	int* discover;	// discover time
	int* finish;	// finish time
	int order;		// number of vertices
	int size;		// number of edges
	int time; 
} GraphObj;

/* Constructors-Destructors */
Graph newGraph(int n) {
	Graph G = malloc(sizeof(struct GraphObj));

	G -> L = calloc(n + 1, sizeof(List));
	G -> color = calloc(n + 1, sizeof(int));
	G -> parent = calloc(n + 1, sizeof(int));
	G -> discover = calloc(n + 1, sizeof(int));
	G -> finish = calloc(n + 1, sizeof(int));

	for (int i = 1; i < n + 1; i++) {
		G -> L[i] = newList();
		G -> color[i] = WHITE;
		G -> parent[i] = NIL;
		G -> discover[i] = INF;
		G -> finish[i] = INF;
	}

	G -> order = n;
	G -> size = 0;

	return G;
}

void freeGraph(Graph* pG) {
	for (int i = 1; i < getOrder(*pG) + 1; i++) {
		freeList(&(*pG) -> L[i]);
	}
	free((*pG) -> L);
	free((*pG) -> color);
	free((*pG) -> parent);
	free((*pG) -> discover);
	free((*pG) -> finish);
	*pG = NULL;
	return;
}

/* Access functions */
int getOrder(Graph G) {
	return G -> order;
}

int getSize(Graph G) {
	return G -> size;
}

int getParent(Graph G, int u) {
	if (!(1 <= u && u <= getOrder(G))) {
		exit(1);
	}
	return G -> parent[u];
} /* Pre: 1<=u<=n=getOrder(G) */

int getDiscover(Graph G, int u) {
	if (!(1 <= u && u <= getOrder(G))) {
		exit(1);
	}
	return G -> discover[u];
} /* Pre: 1<=u<=n=getOrder(G) */

int getFinish(Graph G, int u) {
	if (!(1 <= u && u <= getOrder(G))) {
		exit(1);
	}
	return G -> finish[u];
} /* Pre: 1<=u<=n=getOrder(G) */

/* Manipulation procedures */
void insertion(List L, int n) {
	if (length(L) == 0) {
		append(L, n);
		return;
	}
	if (front(L) > n) {
		prepend(L, n);
		return;
	}
	if (back(L) < n) {
		append(L, n);
		return;
	}
	moveFront(L);
	while (index(L) != -1) {
		if (n >= get(L)) {
			moveNext(L);
		}
		else {
			insertBefore(L, n);
			break;
		}
	}
	return;
}

void addArc(Graph G, int u, int v) {
	if (!(1 <= u && u <= getOrder(G)) || !(1 <= v && v <= getOrder(G))) {
		printf("Error: addEdge() called on invalid vertex number");
		exit(1);
	}
	insertion(G -> L[u], v);

	G -> size ++;
	return;
} /* Pre: 1<=u<=n, 1<=v<=n */

void addEdge(Graph G, int u, int v) {
	if (!(1 <= u && u <= getOrder(G)) || !(1 <= v && v <= getOrder(G))) {
		printf("Error: addArc() called on invalid vertex number");
		exit(1);
	}
	insertion(G -> L[u], v);
	insertion(G -> L[v], u);	
	G -> size ++;
	return;
} /* Pre: 1<=u<=n, 1<=v<=n */

void Visit(Graph G, List S, int x) {
	G -> color[x] = GRAY;
	G -> discover[x] = ++ G -> time;
	moveNext(G -> L[x]);
	while(index(G -> L[x] != -1)) {
		if (G -> color[get(G -> L[x])] == WHITE) {
			G -> parent[get(G -> L[x])] = x;
			Visit(G, S, get(G -> L[x]));
		}
	}
	G -> color[x] = BLACK;
	G -> finish[x] = ++ G -> time;
	prepend(S, x);
}

void DFS(Graph G, List S) {
	if (length(S) != getOrder(G)) {
		exit(1);
	}
	for (int i = 1; i < getOrder(G) + 1; i++) {
		G -> color[i] = WHITE;
		G -> parent[i] = NIL;
	}
	G -> time = 0;
	
	List tempS = copyList(S);
	clear(S);

	moveFront(tempS);
	while(index(tempS) != -1) {
		if (G -> color[get(tempS)] == WHITE) {
			Visit(G, S, get(tempS));
		}
		moveNext(tempS);
	}
	
	freeList(&tempS);
} /* Pre: length(S)==getOrder(G) */

/* Other Functions */
Graph transpose(Graph G) {
	if (G == NULL) {
		exit(1);
	}
	
	Graph tempG = newGraph(getOrder(G));

	for (int i = 1; i < getOrder(G) + 1; i++) {
		if (length(G -> L[i]) != 0) {
			moveFront(G -> L[i]);
			while (index(G -> L[i]) != -1) {
				addArc(tempG, get(G -> L[i]), i);
				moveNext(G -> L[i]);
			}
		}
	}

	return tempG;
}

Graph copyGraph(Graph G) {
	if (G == NULL) {
		exit(1);
	}

	Graph tempG = newGraph(getOrder(G));

	for (int i = 1; i < getOrder(G) + 1; i++) {
		tempG -> L[i] = copyList(G -> L[i]);
	}

	tempG -> size = getSize(G);

	return tempG;
}

void printGraph(FILE* out , Graph G) {
	for (int i = 1; i < getOrder(G) + 1; i++) {
		fprintf(out, "%i: ", i);
		printList(out, G -> L[i]);
		fprintf(out, "\n");
	}
	return;
}
