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

  int a=20;
  dfsan_label la = 1;                     
  dfsan_set_label(la, &a, sizeof(a));

  int b=10;
  dfsan_label lb = 2;                     
  dfsan_set_label(lb, &b, sizeof(b));

  int c=30;
  dfsan_label lc = 4;                     
  dfsan_set_label(lc, &c, sizeof(c));

  int res=3;
  if (a==20)
    res=b;
  else
    res=c;
  dfsan_label lab = dfsan_get_label(res);   

  printf("%d %d\n", res, lab);
  printf("%d\n", dfsan_has_label(lab, la));
  printf("%d\n", dfsan_has_label(lab, lb));
  printf("%d\n", dfsan_has_label(lab, lc));
 }