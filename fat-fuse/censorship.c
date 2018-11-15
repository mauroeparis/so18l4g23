#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "censorship.h"

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

const int n[] = {9, 9, 9, 16, 16, 16, 9, 9, 9, 9, 9, 9, 10, 10, 10, 11, 11,
11, 12, 12, 12, 7, 7, 7, 9, 9, 9, 6, 6, 6, 4, 4, 4, 7, 7, 7, 7, 7, 7, 10, 10,
10, 7, 7, 7, 3, 3, 3, 3, 3, 3, 5, 5, 5, 10, 10, 10, 13, 13, 13, 7, 7, 7, 8, 8,
8, 9, 9, 9, 11, 11, 11, 11, 11, 11, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9, 9, 9, 6, 6,
6, 9, 9, 9, 9, 9, 9, 10, 10, 10, 7, 7, 7, 5, 5, 5, 5, 5, 5, 4, 4, 4, 13, 13, 13
};

const int fw_size = 114;


void censorship(char *buf, size_t size){
  /*Function that change*/

  int start_w = 0;

  for(int i = 0; i < size; i++) {
    if(is_punt(buf[i])){
      int len = i - start_w;
      char *word = malloc(len * sizeof(char));
      for(int j = 0; j < len; j++) word[j] = buf[j + start_w];

      for(int j = 0; j < fw_size; j++){
        if(len != n[j])
          continue;
        if(memcmp(word, forbidden_words[j], len) == 0){
          memset(buf + start_w, 'x', len);
          break;
        }
      }
      free(word);
      start_w = i + 1;
    }
  }

}

bool is_punt(char b) {
  return b == ' ' || b == ',' || b == '.' || b == '\n';
}
