#include "sanitizer/dfsan_interface.h"
#include <assert.h>
#include <llvm/Transforms/Instrumentation/DataFlowSanitizer.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <iostream>

using namespace std;
// clang -DLLVM_DISABLE_ABI_BREAKING_CHECKS_ENFORCING -fsanitize=dataflow -o
// passGen passwordGenerator.cpp

void iferror(int ris, const char *msg) {
  if (ris != 0)
    return;
  fprintf(stderr, "%s\n", msg);
  exit(EXIT_FAILURE);
}

char * generaPass(const char *nome, const char *cognome, int giorno, int mese,
                 int anno) {
  srand(time(NULL));

  char *pass = (char*) malloc(8);

  pass[3] = rand() % (122 + 1 - 97) + 97;
  pass[5] = rand() % (96 + 1 - 91) + 91;
  pass[4] = '-';
  if (nome[1] > 100 && nome[1] < 120)
    pass[0] = 'G';
  else
    pass[0] = 'H';
  
  pass[1] = (anno % giorno) % 10 + '0';
  pass[2] = (anno) % 10 + '0';
  pass[6] = cognome[0];
  return pass;
}

int checkWord(char *stringa) {
  while (*stringa) {
    if (!isspace(*stringa))
      return 0;
    stringa++;
  }
  return 1;
}

int checkData(int giorno, int mese, int anno) {
  if (anno <= 0 || mese <= 0 || giorno <= 0)
    return 0;
  if (anno < 1920)
    return 0;
  if (mese > 12)
    return 0;
  switch (mese) {
  case 1:
  case 3:
  case 5:
  case 7:
  case 8:
  case 10:
  case 12:
    if (giorno > 31)
      return 0;
    break;
  case 4:
  case 6:
  case 9:
  case 11:
    if (giorno > 30)
      return 0;
  default:
    break;
  }
  if (!(anno % 400) || (!(anno % 4) && anno % 100)) { //è bisestile
    if (mese == 2 && giorno > 29)
      return 0;
  } else {
    if (mese == 2 && giorno > 28)
      return 0;
  }
  return 1;
}

int checkTaint(dfsan_label *labelPass, dfsan_label label_nome,
               dfsan_label label_cognome, dfsan_label label_giorno,
               dfsan_label label_mese, dfsan_label label_anno) {

  dfsan_label arr0[] = {label_nome, label_cognome, label_giorno, label_mese,
                        label_anno};

  for (int i = 0; i < 7; i++) {
    for (int j = 0; j < 5; j++) {
      if (dfsan_has_label(labelPass[i], arr0[j])){
        switch (j)
        {
        case 0:
            printf("Il carattere con indice %d è influenzato dal nome\n", i);
            break;
        case 1:
            printf("Il carattere con indice %d è influenzato dal cognome\n", i);
            break;
        case 2:
            printf("Il carattere con indice %d è influenzato dal giorno\n", i);
            break;
        case 3:
            printf("Il carattere con indice %d è influenzato dal mese\n", i);
            break;
        case 4:
            printf("Il carattere con indice %d è influenzato dal anno\n", i);
            break;
        
        default:
            break;
        }
      }
    }
  }

  return 1;
}

int main(void) {
  char *nome = (char *)malloc(64);
  char cognome[64];
  char birthday[11];
  int giorno = 0;
  int mese = 0;
  int anno = 0;
  char *res;

  do {
    printf("Inserisci il nome: \n");
    res = fgets(nome, 64, stdin);
    if (res == NULL)
      iferror(0, "gets");
  } while (checkWord(nome));

  do {
    printf("Inserisci il cognome: \n");
    res = fgets(cognome, 64, stdin);
    if (res == NULL)
      iferror(0, "gets");
  } while (checkWord(cognome));

  do {
    giorno = mese = anno = 0;
    printf("Inserisci il giorno di nascita dd/mm/aaaa: \n");
    res = fgets(birthday, 11, stdin);
    if (res == NULL)
      iferror(0, "gets");
    char *token = strtok(birthday, "/-");
    if (token != NULL)
      giorno = atoi(token);
    token = strtok(NULL, "/-");
    if (token != NULL)
      mese = atoi(token);
    token = strtok(NULL, "/-");
    if (token != NULL)
      anno = atoi(token);
  } while (!checkData(giorno, mese, anno));

  dfsan_label label_nome = 1;
  dfsan_label label_cognome = 2;
  dfsan_label label_giorno = 4;
  dfsan_label label_mese = 8;
  dfsan_label label_anno = 16;

  dfsan_set_label(label_nome, nome, strlen(nome));
  dfsan_set_label(label_cognome, cognome, strlen(cognome));
  dfsan_set_label(label_giorno, &giorno, sizeof(giorno));
  dfsan_set_label(label_mese, &mese, sizeof(mese));
  dfsan_set_label(label_anno, &anno, sizeof(anno));

  char *risultato = generaPass(nome, cognome, giorno, mese, anno);


  dfsan_label labelPassw[] = {0b00100000, 0b01000000, 0b01100000, 0b10000000,
                               0b10100000, 0b11000000, 0b11100000};

  for (int i = 0; i < 7; i++)
    labelPassw[i]=dfsan_get_label(risultato[i]);
  
  checkTaint(labelPassw, label_nome, label_cognome, label_giorno,
                 label_mese, label_anno);
  printf("La password è: %s\n", risultato);
}