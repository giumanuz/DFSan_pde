#include <llvm/Transforms/Instrumentation/DataFlowSanitizer.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "sanitizer/dfsan_interface.h"

using namespace std;
//clang -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING -fsanitize=dataflow -o esempio esempio.cpp

void iferror(int ris){
  if (ris!=0) return;
  fprintf (stderr, "Errore\n");
  exit(EXIT_FAILURE);
}

int main(void) {
  int i = 100;
  int j = 200;
  int k = 300;
  
  dfsan_label i_label = 1;
  dfsan_label j_label = 2;
  dfsan_label k_label = 4;

  dfsan_set_label(i_label, &i, sizeof(i));
  dfsan_set_label(j_label, &j, sizeof(j));
  dfsan_set_label(k_label, &k, sizeof(k));

  dfsan_label ij_label = dfsan_get_label(i + j);

  iferror((ij_label & i_label));                                                  // ij_label has i_label
  iferror((ij_label & j_label));                                                  // ij_label has j_label
  iferror(!(ij_label & k_label));                                                   // ij_label doesn't have k_label
  iferror((ij_label == 3));                                                   // Verifies all of the above

  // Or, equivalently:
  iferror((dfsan_has_label(ij_label, i_label)));
  iferror((dfsan_has_label(ij_label, j_label)));
  iferror(!(dfsan_has_label(ij_label, k_label)));

  dfsan_label ijk_label = dfsan_get_label(i + j + k);

  iferror((ijk_label & i_label));                                                   // ijk_label has i_label
  iferror((ijk_label & j_label));                                                   // ijk_label has j_label
  iferror((ijk_label & k_label));                                                   // ijk_label has k_label
  iferror((ijk_label == 7));                                                  // Verifies all of the above

  // Or, equivalently:
  iferror((dfsan_has_label(ijk_label, i_label)));
  iferror((dfsan_has_label(ijk_label, j_label)));
  iferror((dfsan_has_label(ijk_label, k_label)));
  
  printf ("Execute\n");

  return 0;
}
