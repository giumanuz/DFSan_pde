#include <llvm/Transforms/Instrumentation/DataFlowSanitizer.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "sanitizer/dfsan_interface.h"
//#include <iostream>

using namespace std;
//clang -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING -fsanitize=dataflow -o esempio esempio.cpp

void iferror(int ris){
  if (ris!=0) return;
  fprintf (stderr, "Errore\n");
  exit(EXIT_FAILURE);
}

int main(void) {
  int vet[]= {100, 200, 300};

  dfsan_label label[]= {1, 2, 4};

  for (int i=0; i<sizeof(vet); i++)
    dfsan_set_label(label[i], &vet[i], sizeof(int));
  
  dfsan_label ij_label = dfsan_get_label(vet[0]+ vet[1]);

  iferror((dfsan_has_label(ij_label, label[0])));
  iferror((dfsan_has_label(ij_label, label[1])));
  iferror(!(dfsan_has_label(ij_label, label[2])));

  dfsan_label ijk_label = dfsan_get_label(vet[0]+ vet[1]+ vet[2]);

  iferror((dfsan_has_label(ijk_label, label[0])));
  iferror((dfsan_has_label(ijk_label, label[1])));
  iferror((dfsan_has_label(ijk_label, label[2])));
  
  printf ("EXECUTE\n");

  return 0;
}