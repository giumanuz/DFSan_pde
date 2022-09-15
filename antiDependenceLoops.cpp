#include "sanitizer/dfsan_interface.h"
#include <assert.h>
#include <llvm/Transforms/Instrumentation/DataFlowSanitizer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <iostream>

using namespace std;
// clang -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING -fsanitize=dataflow -o prova1 prova1.cpp

void setLabel(int* vet, dfsan_label* label, int len){
    for (int i=0; i<len; i++)
        dfsan_set_label(label[i], &vet[i], 4);
}
void func(int* vet, int len){
    for (int i=0; i<len-1; i++)
        vet[i]=vet[i+1];
}

void checkResult(int * vet, dfsan_label* label, int len){
    for (int i=0; i<len; i++)
        label[i]=dfsan_get_label(vet[i]);
}

void printTaint(dfsan_label* dest, dfsan_label* src, int len) {
  for (int i = 0; i < len; i++) {
    for (int j = 0; j < len; j++) {
      if (dfsan_has_label(dest[i], src[j]))
            printf("L'indice %d è influenzato dall'indice %d\n", i, j);
    }
  }
}

int main(void) {
    int vet[]={1, 2, 3, 4};
    dfsan_label label[]={1, 2, 4, 8};
    dfsan_label ris[4];
    setLabel(vet, label, 4);

    func(vet, 4);

    checkResult(vet, ris, 4);
    printTaint(ris, label, 4);

    // PRINT SULLA CONSOLE
    // L'indice 0 è influenzato dall'indice 1
    // L'indice 1 è influenzato dall'indice 2
    // L'indice 2 è influenzato dall'indice 3
    // L'indice 3 è influenzato dall'indice 3
 }