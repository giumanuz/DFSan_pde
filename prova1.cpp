#include "sanitizer/dfsan_interface.h"
#include <assert.h>
#include <llvm/Transforms/Instrumentation/DataFlowSanitizer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <iostream>

using namespace std;
// clang -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING -fsanitize=dataflow -o prova1 prova1.cpp

int main(void) {
  fflush(stdout);

  int a=3;
  dfsan_label la = 1;                     //0001
  dfsan_set_label(la, &a, sizeof(a));

  int b=3;
  dfsan_label lb = 2;                     //0010
  dfsan_set_label(lb, &b, sizeof(b));

  int c=a*3+b;
  dfsan_label lab = dfsan_get_label(c);   //0011  da capire

  int d=5;
  dfsan_label ld = 252;                   //devo fare degli accorgimenti perchè non posso matchare d con 0100 ma con 0010 e 0001 si
  
  printf ("%d %d %d  \n", la, lb, lab);


  printf("%d\n", dfsan_has_label(la, lb));
  printf("%d\n", dfsan_has_label(lab, la));
  printf("%d\n", dfsan_has_label(lab, lb));

  // printf("%d %d\n", lab, lb);

  // printf ("%d\n", sizeof(dfsan_label));


  int e = 10;
  dfsan_label le = dfsan_get_label(e);
  dfsan_label le2 = 4;
  dfsan_set_label(le2, &e, sizeof(e));
  printf("%d %d\n", le, le2);
  ///////////////////////////////////////////////////////////////////////////////////

int vet[]= {10, 20, 30};

dfsan_label label[3]={1, 2, 4};
  
for (int i=0; i<3; i++)
  dfsan_set_label(label[i], &vet[i], sizeof(int));


// for (int i = 0; i < 3; i++) {
//   for (int j = 0; j < 3; j++) {
//     if (dfsan_has_label(label[i], label[j])){
//       if (i == j) 
//         printf("Regolare\n");
//       else
//         printf("Il label %d è presente nel label %d \n", j, i);
//     }
//   }
// }
 }