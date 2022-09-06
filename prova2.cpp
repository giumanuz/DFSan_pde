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

  int a=0;
  dfsan_label la = 1;                     
  dfsan_set_label(la, &a, sizeof(a));

  int res=3;
  if (a==0)
    res=1;
  else
    res=2;
  dfsan_label lab = dfsan_get_label(res);   

  printf("%d %d\n", res, lab);
  printf("%d\n", dfsan_has_label(lab, la));
 }