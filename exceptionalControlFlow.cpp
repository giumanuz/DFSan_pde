#include "sanitizer/dfsan_interface.h"
#include <assert.h>
#include <llvm/Transforms/Instrumentation/DataFlowSanitizer.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
//#include <iostream>

using namespace std;

// clang -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING -fsanitize=dataflow -o
// prova1 prova1.cpp

void iferror(int r, const char* msg) {
  if (r != -1)
    return;
  perror(msg);
  exit(EXIT_FAILURE);
}

void printTaint(dfsan_label la, dfsan_label lb, dfsan_label lc, dfsan_label ld,
                dfsan_label le) {

  dfsan_label arr[] = {la, lb, lc, ld, le};

  for (int i = 3; i < 5; i++) {
    for (int j = 0; j < 3; j++) {
      if (i != j && dfsan_has_label(arr[i], arr[j]) && arr[j]!=0) {
        char primo, secondo;
        switch (i) {
        case 0:
          primo = 'a';
          break;
        case 1:
          primo = 'b';
          break;
        case 2:
          primo = 'c';
          break;
        case 3:
          primo = 'd';
          break;
        case 4:
          primo = 'e';
          break;
        default:
          break;
        }
        switch (j) {
        case 0:
          secondo = 'a';
          break;
        case 1:
          secondo = 'b';
          break;
        case 2:
          secondo = 'c';
          break;
        case 3:
          secondo = 'd';
          break;
        case 4:
          secondo = 'e';
          break;
        default:
          break;
        }
        printf("Il label %c è influenzato dal label %c\n", primo, secondo);
      }
    }
  }
}

int main(void) {
  int a = 3;
  dfsan_label la = 1;
  dfsan_set_label(la, &a, 4);

  int b = 5;
  dfsan_label lb = 2;
  dfsan_set_label(lb, &b, 4);

  int c = 8;
  dfsan_label lc = 4;
  dfsan_set_label(lc, &c, 4);

  int d = c - 2 * a;
  dfsan_label ld = dfsan_get_label(d);

  int e = 4;
  dfsan_label le=0;

  printf("PRIMA\n");
  printf("a=%d %d, b=%d %d, c=%d %d, d=%d %d, e=%d %d\n", a, la, b, lb, c, lc, d, ld, e, le);
  printTaint(la, lb, lc, ld, le);

  pid_t pid = fork();
  iferror(pid, "fork");
  if (pid == 0) {
    d += b;
    e = c - b;
    dfsan_label ld2= dfsan_get_label(d);
    dfsan_label le2=dfsan_get_label(e);
    printf("\nDENTRO\n");
    printf("a=%d %d, b=%d %d, c=%d %d, d=%d %d, e=%d %d\n", a, la, b, lb, c, lc, d, ld2, e, le2);
    printTaint(la, lb, lc, ld2, le2);
    _exit(EXIT_SUCCESS);
  }

  int status;
  pid = wait(&status);
  iferror(pid, "wait");
  if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
    printf("Corretto\n");
  else
    printf("Non corretto");


  dfsan_label ld3 = dfsan_get_label(d);
  dfsan_label le3 = dfsan_get_label(e);
  printf("\nDOPO\n");
  printf("a=%d %d, b=%d %d, c=%d %d, d=%d %d, e=%d %d\n", a, la, b, lb, c, lc, d, ld3, e, le3);
  printTaint(la, lb, lc, ld3, le3);
}
