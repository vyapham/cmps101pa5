/*********
* Name: VY PHAM
* ID: vyapham
* Student ID #: 1593394
* Assignment name: pa5
********/

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#include "List.h"

int main(int argc, char* argv[]) {
  FILE *in, *out;
  int n = 0;

  if (argc != 3) {
    printf("Usage: %s <input file> <output file>\n", argv[0]);
    exit(1);
  }
  
  in = fopen(argv[1], "r");
  
  if(in == NULL) {
    printf("Unable to open file %s for reading\n", argv[1]);
    exit(1);
  }
  
  out = fopen(argv[2], "w");
  
  if(out == NULL) {
    printf("Unable to open file %s for writing\n", argv[2]);
    exit(1);
  }
  
  fscanf(in, "%d", &n);
  Graph G = newGraph(n);
  int v1 = 0;
  int v2 = 0;
	
  while (fgetc(in) != EOF)  {
    fscanf(in, "%d", &v1);
    fscanf(in, "%d", &v2);     
    if (v1 == 0 && v2 == 0) {
      break;
    }
    addArc(G, v1, v2);
  }
  
  printGraph(out, G);
  fprintf(out, "\n");

  List S = newList();
  for (int i = 1; i <= n; i++) {
    append(S, i);
  }

  Graph GT = transpose(G);
  DFS(G, S);
  DFS(GT, S);

  int components = 0;     // number of strong components
  moveFront(S);
  while (index(S) != -1) {
    if (getParent(GT, get(S)) == NIL) {
      components ++;
    }
    moveNext(S);
  }

  fprintf(out, "G contains %d strongly connected components:\n", components);

  List tempL = newList();
  int count = 0;
  moveBack(S);
  while (index(S) != -1) {
    prepend(tempL, get(S));
    if (getParent(GT, get(S)) == NIL) {
         fprintf(out, "Component %d: ", ++count);
         printList(out, tempL);
         fprintf(out, "\n");
         clear(tempL);
    }
    movePrev(S);
  }

  fclose(in);
  fclose(out);
  
  freeList(&S);
  freeList(&tempL);
  freeGraph(&GT);
  freeGraph(&G);
  
  return 0;
  
}
