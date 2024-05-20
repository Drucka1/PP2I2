#include "../../include/game/rendertext.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#define COUPAGE 50

// Générer du texte instantanément

void text_display(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                  int x, int y) {

  SDL_Surface *surface_text;
  SDL_Texture *texture_text;
  SDL_Color color_text = {0, 255, 0};

  surface_text = TTF_RenderText_Blended(font, text, color_text);
  if (!surface_text) {
    fprintf(stderr, "Erreur lors de la création de surface de texte : %s\n", TTF_GetError());
    return;
  }

  texture_text = SDL_CreateTextureFromSurface(renderer, surface_text);
  if (!texture_text) {
    fprintf(stderr, "Erreur lors de la création de texture de texte : %s\n", SDL_GetError());
    SDL_FreeSurface(surface_text); //en plus
    return;
  }

  SDL_Rect rectangle = {x, y, surface_text->w, surface_text->h}; // w : largeur ; h : hauteur ; x et y coordonnées du coin supérieur gauche du rectangle
  SDL_RenderCopy(renderer, texture_text, NULL, &rectangle);
  SDL_FreeSurface(surface_text);
  SDL_DestroyTexture(texture_text);
}

/*

//Générer du texte lettre par lettre

void display_text_letter_by_letter(SDL_Renderer* renderer, TTF_Font* font, const
char* text, int x, int y){ int length_text = strlen(text); for (int i = 0;
i<length_text; i++){ SDL_SetRenderDrawColor(renderer, 0,0,0,255); //fond noir
        SDL_RenderClear(renderer);

        char current_text[i+2]; // lettre actuelle + '\0'
        strncpy(current_text, text, i+1);
        current_text[i+1] = '\0';
        SDL_Color color_texte = {255,255,255}; //blanc
        SDL_Surface* surface = TTF_RenderText_Solid(font, current_text,
color_texte); SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,
surface); SDL_Rect rectangle = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rectangle);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        SDL_RenderPresent(texture);
        SDL_Delay(100); //délai de 100ms avant d'afficher la lettre suivante
    }
}

*/

// Couper une chaîne selon un nombre de caractères

char *cut_string(char *str_1) {
  int len_str = strlen(str_1);
  char str[10000];
  for (int i = 0; i < len_str; i++) {
    str[i] = *(str_1 + i);
  }
  str[len_str] = '\0';

  char text[10000] = "";
  int coupage = COUPAGE;
  int len = 0;
  const char *separator = " ";

  // Pour le premier mot
  char *strToken = strtok(str, separator);
  len = strlen(strToken);
  if (len <= coupage) {
    strcat(text, strToken);
  }

  // Pour les autres mots
  while (strToken != NULL) {
    strToken = strtok(NULL, separator);
    if (strToken == NULL) {
      break;
    }
    len += strlen(strToken) + 1;
    if (len <= coupage) {
      strcat(text, " ");
      strcat(text, strToken);
    }
  }
  char *res = copy_string(text);
  return res;
}

// Après avoir coupé la chaine au bon endroit, pouvoir récupérer le reste de la
// chaine
char *difference_str(char *big_str, char *small_str) {
  int len_big_str = strlen(big_str);
  int len_small_str = strlen(small_str);
  int len_res = len_big_str - len_small_str - 1;

  if (len_res <= 0) {
    return NULL;
  }

  char res[len_res + 1];
  for (int i = len_small_str + 1; i < len_big_str; i++) {
    res[i - len_small_str - 1] = big_str[i];
  }
  res[len_res] = '\0';
  char *res2 = copy_string(res);
  return res2;
}

// Copier une chaine de caractère

char *copy_string(char *str) {
  char *res = malloc(strlen(str) + 1);
  char *p = res;
  while (*str != '\0') {
    *p = *str;
    p++;
    str++;
  }
  *p = '\0';
  return res;
}
