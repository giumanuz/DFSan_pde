#include "sanitizer/dfsan_interface.h"
#include <assert.h>
#include <llvm/Transforms/Instrumentation/DataFlowSanitizer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <iostream>

using namespace std;
// clang -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING -fsanitize=dataflow -o passgen2 passwgen2.cpp

void iferror(int ris, const char *msg) {
  if (ris != 0)
    return;
  fprintf(stderr, "%s\n", msg);
  exit(EXIT_FAILURE);
}

void generaPass(const char *nome, const char *cognome, int giorno, int mese,
                int anno, dfsan_label label_nome, dfsan_label label_cognome,
                dfsan_label label_giorno, dfsan_label label_mese,
                dfsan_label label_anno, dfsan_label *labelPassw, char *pass) {
  srand(time(NULL));

  pass[3] = rand() % (122 + 1 - 97) + 97;
  pass[5] = rand() % (96 + 1 - 91) + 91;
  pass[8] = rand() % (90 + 1 - 65) + 65;
  pass[11] = rand() % (96 + 1 - 91) + 91;
  pass[12] = rand() % (122 + 1 - 97) + 97;
  pass[4] = '-';
  pass[9] = '-';

  if (nome[1] > 100 && nome[1] < 120) 
    pass[0] = 'G';
  else
    pass[0] = 'H';
  labelPassw[0] = dfsan_union(labelPassw[0], label_nome);
  
  pass[1] = (anno % giorno) % 10 + '0';
  labelPassw[1] = dfsan_union(labelPassw[1], label_anno);
  labelPassw[1] = dfsan_union(labelPassw[1], label_giorno);
  pass[2] = (anno) % 10 + '0';
  labelPassw[2] = dfsan_union(labelPassw[2], label_anno);
  pass[6] = cognome[0];
  labelPassw[6] = dfsan_union(labelPassw[6], label_cognome);
  pass[7] = nome[0] + 13;
  labelPassw[7] = dfsan_union(labelPassw[7], label_nome);
  pass[10] = (mese % giorno) % 10 + '0';
  labelPassw[10] = dfsan_union(labelPassw[10], label_mese);
  labelPassw[10] = dfsan_union(labelPassw[10], label_giorno);
  pass[13] = '}';
  printf("%s\n", pass);
}


int notLabel(dfsan_label label, dfsan_label *lista, int size) {
  for (int i = 0; i < size; i++)
    if (dfsan_has_label(label, lista[i])) {
      printf("Errore size %d \n", size);
      return 1;
    }
  return 0;
}

int checkTaint(dfsan_label *labelPass, dfsan_label label_nome,
               dfsan_label label_cognome, dfsan_label label_giorno,
               dfsan_label label_mese, dfsan_label label_anno) {

  if (!dfsan_has_label(labelPass[0], label_nome) ||
      !dfsan_has_label(labelPass[1], label_giorno) ||
      !dfsan_has_label(labelPass[1], label_anno) ||
      !dfsan_has_label(labelPass[2], label_anno) ||
      !dfsan_has_label(labelPass[6], label_cognome) ||
      !dfsan_has_label(labelPass[7], label_nome) ||
      !dfsan_has_label(labelPass[10], label_mese) ||
      !dfsan_has_label(labelPass[10], label_giorno))
    return 0;

  dfsan_label arr0[] = {label_cognome, label_giorno, label_mese, label_anno};
  dfsan_label arr1[] = {label_cognome, label_nome, label_mese};
  dfsan_label arr2[] = {label_cognome, label_nome, label_mese, label_giorno};
  dfsan_label arr3[] = {label_cognome, label_nome, label_mese, label_giorno,
                        label_anno};
  dfsan_label arr6[] = {label_giorno, label_nome, label_mese, label_anno};
  dfsan_label arr10[] = {label_cognome, label_nome, label_anno};

  // if (notLabel(labelPass[0], arr0, 4) || notLabel(labelPass[3], arr3, 5) )
  //       notLabel(labelPass[4], arr3, 5) || notLabel(labelPass[5], arr3, 5) ||
  //       notLabel(labelPass[6], arr6, 4) || notLabel(labelPass[7], arr0, 4) ||
  //       notLabel(labelPass[8], arr3, 5) || notLabel(labelPass[9], arr3, 5) ||
  //       notLabel(labelPass[10], arr10, 3) || notLabel(labelPass[11], arr3, 5)
  //       || notLabel(labelPass[12], arr3, 5) || notLabel(labelPass[13], arr3,
  //       5) || notLabel(labelPass[1], arr1, 3) || notLabel(labelPass[2], arr2,
  //       4))
  //    return 0;

  return 1;
}

int main(void) {
  char nome[6] = "Mario";
  char cognome[6] = "Rossi";
  char birthday[11]= "12/12/2012";
  int giorno = 12;
  int mese = 12;
  int anno = 2012;
  char *res;


  dfsan_label label_nome = 1;
  dfsan_label label_cognome = 2;
  dfsan_label label_giorno = 3;
  dfsan_label label_mese = 4;
  dfsan_label label_anno = 5;

  dfsan_set_label(label_nome, &nome, strlen(nome));
  dfsan_set_label(label_cognome, &cognome, strlen(cognome));
  dfsan_set_label(label_giorno, &giorno, sizeof(giorno));
  dfsan_set_label(label_mese, &mese, sizeof(mese));
  dfsan_set_label(label_anno, &anno, sizeof(anno));
  dfsan_label labelPassw[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

  char *risultato = (char *)malloc(14);
  for (int i = 0; i < 14; i++)
    dfsan_set_label(labelPassw[i], &risultato[i], sizeof(int));

  generaPass(nome, cognome, giorno, mese, anno, label_nome, label_cognome,
             label_giorno, label_mese, label_anno, labelPassw, risultato);



  dfsan_label arr[] = {label_nome, label_cognome, label_giorno, label_mese, label_anno};
  for (int i = 0; i < 14; i++){
    for (int j = 0; j < 5; j++){
     if (dfsan_has_label(labelPassw[i], arr[j])) 
        printf ("Il label %d è presente nel carattere %d \n", j, i);
    }
    printf("\n");
  }




//   if (checkTaint(labelPassw, label_nome, label_cognome, label_giorno,
//                  label_mese, label_anno))
//     printf("Marcato correttamente\n");
//   else
//     printf("Marcato non correttamente\n");

//   printf("La password è: %s\n", risultato);
}