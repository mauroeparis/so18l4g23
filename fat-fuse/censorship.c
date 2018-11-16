#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "censorship.h"

#define FW_SIZE 114

char *forbidden_words[] = {"bourgeois", "BOURGEOIS", "Bourgeois",
"bourgeoisieclass","BOURGEOISIECLASS", "Bourgeoisieclass", "communism",
"COMMUNISM", "Communism", "communist", "COMMUNIST", "Communist", "communists",
"COMMUNISTS", "Communists", "communistic", "COMMUNISTIC", "Communistic",
"conservative", "CONSERVATIVE", "Conservative", "country", "COUNTRY",
"Country", "countries", "COUNTRIES", "Countries", "family", "FAMILY",
"Family", "free", "FREE", "Free", "freedom", "FREEDOM", "Freedom",
"history", "HISTORY", "History", "historical", "HISTORICAL",
"Historical", "justice", "JUSTICE", "Justice", "man", "MAN", "Man",
"men", "MEN", "Men", "moral", "MORAL", "Moral", "philosophy",
"PHILOSOPHY", "Philosophy", "philosophical", "PHILOSOPHICAL",
"Philosophical", "politic", "POLITIC", "Politic", "politics",
"POLITICS", "Politics", "political", "POLITICAL", "Political",
"proletarian", "PROLETARIAN", "Proletarian", "proletariat",
"PROLETARIAT", "Proletariat", "radical", "RADICAL", "Radical",
"religion", "RELIGION", "Religion", "religions", "RELIGIONS",
"Religions", "religious", "RELIGIOUS", "Religious", "social", "SOCIAL",
"Social", "socialism", "SOCIALISM", "Socialism", "socialist",
"SOCIALIST", "Socialist", "socialists", "SOCIALISTS", "Socialists",
"society", "SOCIETY", "Society", "value", "VALUE", "Value", "world",
"WORLD", "World", "work", "WORK", "Work", "working-class",
"WORKING-CLASS", "Working-class"};

void censorship(char *buf, size_t size){
  /*Function that change*/

  int start_w = 0;

  for(int i = 0; i < size; i++){
    if(is_punt(buf[i])){
      int len = i - start_w;
      for(int j = 0; j < FW_SIZE; j++){
        char *fw = forbidden_words[j];
        if(len == strlen(fw)){
          if(memcmp(buf + start_w, fw, len) == 0){
            memset(buf + start_w, 'x', len);
            break;
          }
        }
      }

      start_w = i + 1;
    }
  }

}

bool is_punt(char b) {
  return b == ' ' || b == ',' || b == '.' || b == '\n';
}
