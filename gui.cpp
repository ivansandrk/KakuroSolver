#include <stdio.h>
#include <string>
#include <assert.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "gui.h"
#include "sol.h"

typedef Uint32 uint32;
typedef Uint16 uint16;
typedef Uint8  uint8;

const Uint32 RED        = 0xFF0000;
const Uint32 GREEN      = 0x00FF00;
const Uint32 BLUE       = 0x0000FF;
const Uint32 YELLOW     = 0xFFFF00;
const Uint32 MAGENTA    = 0xFF00FF;
const Uint32 CYAN       = 0x00FFFF;
const Uint32 WHITE      = 0xFFFFFF;
const Uint32 BLACK      = 0x000000;
const Uint32 GRAY       = 0x7F7F7F;
const Uint32 LIGHT_BLUE = 0x7F7FFF;
const Uint32 BROWN      = 0xFF994D;
const Uint32 LIGHT_PINK = 0xFFAFAF;

const uint32 boje[] = {RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, WHITE, BLACK,
                       GRAY, LIGHT_BLUE, BROWN, LIGHT_PINK};

const int TILE_SIZE = 60;
const int FONT_SMALL_SIZE = 20;
const int FONT_BIG_SIZE = 30;
int SCREEN_WIDTH;
int SCREEN_HEIGHT;

bool quit = false;
bool solve_again = false;
int brojic = 0;
int mouse_x = -1, mouse_y = -1;
SDL_Window* g_window = NULL;
//SDL_Surface* g_screen = NULL;
SDL_Renderer* g_renderer = NULL;
TTF_Font* g_font_small = NULL;
TTF_Font* g_font_big = NULL;

int init()
{
  input_kakuro();
  precalculate(); // has to come before solve_kakuro()
  
  if (SDL_Init(SDL_INIT_VIDEO) == -1) {
    fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
    exit(1);
  }

  SCREEN_WIDTH = TILE_SIZE * g_cols;
  SCREEN_HEIGHT = TILE_SIZE * g_rows;

  g_window = SDL_CreateWindow("title",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
  if (!g_window) {
    fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
    exit(1);
  }
  //g_screen = SDL_GetWindowSurface(g_window);
  g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
  if (!g_renderer) {
    fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_SetRenderDrawColor(g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  
  if (TTF_Init() == -1) {
    fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
    exit(1);
  }
  // "/usr/share/fonts/truetype/msttcorefonts/arial.ttf"
  g_font_small = TTF_OpenFont("lazy.ttf", FONT_SMALL_SIZE);
  g_font_big   = TTF_OpenFont("lazy.ttf", FONT_BIG_SIZE);
  if (!g_font_small || !g_font_big) {
    fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
    exit(1);
  }
  return 0;
}

int clean_up()
{
  if (g_renderer) {
    SDL_DestroyRenderer(g_renderer);
    g_renderer = NULL;
  }
  if (g_window) {
    SDL_DestroyWindow(g_window);
    g_window = NULL;
  }

  SDL_Quit();
//  output_kakuro();

  return 0;
}

int get_input()
{
  SDL_Event event;
  SDL_WaitEvent(&event);
  do {
    switch (event.type) {
    case SDL_QUIT: quit = true; break;
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym) {
      case SDLK_ESCAPE: quit = true; break;
      case SDLK_0: brojic = 0; break;
      case SDLK_1: brojic = 1; break;
      case SDLK_s: solve_again = true; break;
      }
      break;
    case SDL_KEYUP: break;
    case SDL_MOUSEMOTION: break;
    case SDL_MOUSEBUTTONDOWN:
      mouse_x = event.button.x;
      mouse_y = event.button.y;
      break;
    case SDL_MOUSEBUTTONUP: break;
    }
  } while (SDL_PollEvent(&event));
  return 0;
}

int process()
{
  if (solve_again) {
    solve_kakuro();
    solve_again = false;
  }
  if (mouse_x > 0 && mouse_y > 0) {
    int K = mouse_y / TILE_SIZE, I = mouse_x / TILE_SIZE;
    if (p[K][I].is_white() && brojic > 0 && !p[K][I].is_single()) {
      mouse_x -= I * TILE_SIZE;
      mouse_y -= K * TILE_SIZE;
      Uint8 br = mouse_x/(TILE_SIZE/3) + (mouse_y/(TILE_SIZE/3))*3 + 1;
      p[K][I].flip_bit(br);
    }
    mouse_x = -1; mouse_y = -1;
  }
  return 0;
}

void set_color(uint32 color)
{
  uint8 r = (color >> 16) & 0xFF;
  uint8 g = (color >>  8) & 0xFF;
  uint8 b = (color >>  0) & 0xFF;
  SDL_SetRenderDrawColor(g_renderer, r, g, b, 0xFF);
}

SDL_Point g_offset = {0, 0};

void draw_rect(uint16 x, uint16 y, uint16 w, uint16 h)
{
  SDL_Rect rect = {x + g_offset.x, SCREEN_HEIGHT - y - g_offset.y - h, w, h};
  SDL_RenderFillRect(g_renderer, &rect);
}

void draw_line(uint16 x0, uint16 y0, uint16 x1, uint16 y1)
{
  x0 = x0 + g_offset.x;
  y0 = SCREEN_HEIGHT - y0 - g_offset.y;
  x1 = x1 + g_offset.x;
  y1 = SCREEN_HEIGHT - y1 - g_offset.y;
  SDL_RenderDrawLine(g_renderer, x0, y0, x1, y1);
}

void print(TTF_Font* font, uint16 x, uint16 y, Uint8 num)
{
  std::string text = std::to_string(num);
  SDL_Color color = {0, 0, 0, 0};
  SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, surface);
  int w = surface->w;
  int h = surface->h;
  SDL_FreeSurface(surface);
  x = x + g_offset.x;
  y = SCREEN_HEIGHT - y - g_offset.y - h;
  SDL_Rect rect = {x, y, w, h};
  SDL_RenderCopy(g_renderer, texture, NULL, &rect);
  SDL_DestroyTexture(texture);
}

int draw()
{
  set_color(WHITE);
  SDL_RenderClear(g_renderer);
  
  for (int k = 0; k < g_rows; ++k) {
    for (int i = 0; i < g_cols; ++i) {
      const square& q = p[k][i];
      set_color(q.is_black() ? BROWN : WHITE);
      g_offset = {TILE_SIZE * i, TILE_SIZE * (g_rows-k-1)};
      draw_rect(0, 0, TILE_SIZE, TILE_SIZE);

      if (q.is_white() && q.is_single()) {
        set_color(LIGHT_PINK);
        draw_rect(0, 0, TILE_SIZE, TILE_SIZE);
        print(g_font_big, TILE_SIZE * 0.35, TILE_SIZE * 0.25, q.value());
      }
      if (q.is_white() && !q.is_single()) {
        for (int j = 0; j < 9; ++j) {
          if (q.get_bit(j+1)) {
            set_color(boje[j]);
            //draw_rect((j%3)*TILE_SIZE/3, (2-j/3)*TILE_SIZE/3, TILE_SIZE/3, TILE_SIZE/3);
            print(g_font_small, (j%3)*TILE_SIZE/3, (2-j/3)*TILE_SIZE/3, j+1);
          }
        }
      }
      if (q.is_black() && (q.has_right_sum() || q.has_down_sum())) {
        set_color(WHITE);
        draw_line(0, TILE_SIZE, TILE_SIZE, 0);
        if (q.has_right_sum()) {
          print(g_font_small, TILE_SIZE/2, TILE_SIZE/2, q.orig_right_sum_);
        }
        if (q.has_down_sum()) {
          print(g_font_small, 0, 0, q.orig_down_sum_);
        }
      }
      g_offset = {0, 0};
    }
  }
  
  set_color(BLACK);
  for (int k = 1; k < g_rows; ++k) {
    draw_line(0, k * TILE_SIZE, SCREEN_WIDTH, k * TILE_SIZE);
  }
  for (int k = 1; k < g_cols; ++k) {
    draw_line(k * TILE_SIZE, 0, k * TILE_SIZE, SCREEN_HEIGHT);
  }

  SDL_RenderPresent(g_renderer);

  return 0;
}

void input_kakuro()
{
  scanf("%d%d\n", &g_rows, &g_cols);
  assert(g_rows <= MAX_ROWS);
  assert(g_cols <= MAX_COLS);
  for (Uint8 k = 0; k < g_rows; ++k) {
    for (Uint8 i = 0; i < g_cols; ++i) {
      Uint8 a = getchar();
      if (a == 'b') { // black
        p[k][i].set_black();
      }
      else if (a == 'w') { // white
        p[k][i].set_white();
      }
      else { // error
        fprintf(stderr, "input_kakuro(%d, %d): color is b or w [char = 0x%X]\n", k, i, a);
        exit(1);
      }
    }
    while (getchar() != '\n') continue;
  }
  Uint32 k, i, right, down;
  while (scanf("%d%d%d%d", &k, &i, &right, &down) != EOF) {
    p[--k][--i].set_black(right, down);
  }
}

void output_kakuro()
{
  for (int k = 0; k < g_rows; ++k) {
    for (int i = 0; i < g_cols; ++i) {
      putchar(p[k][i].is_black() ? 'b' : 'w');
    }
    putchar('\n');
  }
  for (int k = 0; k < g_rows; ++k) {
    for (int i = 0; i < g_cols; ++i) {
      if (p[k][i].is_black() && (p[k][i].has_right_sum() || p[k][i].has_down_sum())) {
        printf("right: %d  down: %d\n", p[k][i].orig_right_sum_, p[k][i].orig_down_sum_);
      }
    }
  }
}
