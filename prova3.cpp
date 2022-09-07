
#include "sanitizer/dfsan_interface.h"
#include <assert.h>
#include <llvm/Transforms/Instrumentation/DataFlowSanitizer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <iostream>

using namespace std;
// clang -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING -fsanitize=dataflow -dfsan-track-origins=1 -o prova3 prova3.cpp
//clang++ -fsanitize=dataflow -mllvm -dfsan-track-origins=1 -fno-omit-frame-pointer -g -O2 prova3.cpp

int main(int argc, char** argv) {
  int i = 0;
  dfsan_label i_label=1;
  dfsan_set_label(i_label, &i, sizeof(i));
  int j = i + 1;
  dfsan_print_origin_trace(&j, "A flow from i to j");
  return 0;


}
