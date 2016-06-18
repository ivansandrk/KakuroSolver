#include "sol.h"
#include <cstdio>

int g_rows;
int g_cols;

square p[MAX_ROWS][MAX_COLS]; // polje koje zivot znaci

static Uint8 tab[512]; // tab mi govori koliko 1(jedinica) ima broj, npr. 0x3f ima 6 jedinica
static Uint8 suma[512]; // suma broja, npr. suma za 0x58 je 7+5+4 = 16
static Uint16 ab[46][10][12]; // 16 u 4 polja: 2+3+4+7=16, 1+3+4+8=16, ...
static Uint8 abk[46][10]; // koliko ima za svaki ovaj gore

void solve_kakuro()
{
  for (int k = 0; k < g_rows; ++k) {
    for (int i = 0; i < g_cols; ++i) {
      square &r = p[k][i];
      if (r.is_running() && r.is_black()) {
        r.set_done();
        if (r.has_right_sum()) {
          Uint16 f = 0;
          Uint8 j = 0;
          for (Uint8 jj = 0; i+jj+1 < g_cols && p[k][i+jj+1].is_white() ; ++jj) {
            if (p[k][i+jj+1].is_running()) {
              ++j;
            }
          }
          for (Uint8 l = 0; l < abk[r.right_sum_][j]; ++l) {
            f |= ab[r.right_sum_][j][l];
          }
          for (Uint8 l = 0; i+l+1 < g_cols && p[k][i+l+1].is_white(); ++l) {
            int K = k, I = i+l+1;
            if (p[K][I].is_running())
              p[K][I].x_ &= f;
          }
        }
        if (r.has_down_sum()) {
          Uint16 f = 0;
          Uint8 j = 0;
          for (Uint8 jj = 0; k+jj+1 < g_rows && p[k+jj+1][i].is_white() ; ++jj) {
            if (p[k+jj+1][i].is_running()) {
              ++j;
            }
          }
          for (Uint8 l = 0; l < abk[r.down_sum_][j]; ++l) {
            f |= ab[r.down_sum_][j][l];
          }
          for (Uint8 l = 0; k+l+1 < g_rows && p[k+l+1][i].is_white(); ++l) {
            int K = k+l+1, I = i;
            if (p[K][I].is_running())
              p[K][I].x_ &= f;
          }
        }
      }
      if (r.is_running() && r.is_white() && r.is_single()) {
        square &q = p[k][i];
        q.set_done();
        Uint8 j;
        for (j = 1; i-j >= 0 && p[k][i-j].is_white(); ++j)
          if (p[k][i-j].is_running())
            p[k][i-j].x_ &= ~q.x_;
        p[k][i-j].right_sum_ -= suma[q.x_];
        p[k][i-j].set_running();
        
        for (j = 1; i+j < g_cols && p[k][i+j].is_white(); ++j)
          if (p[k][i+j].is_running())
            p[k][i+j].x_ &= ~q.x_;
        
        for (j = 1; k-j >= 0 && p[k-j][i].is_white(); ++j)
          if (p[k-j][i].is_running())
            p[k-j][i].x_ &= ~q.x_;
        p[k-j][i].down_sum_ -= suma[q.x_];
        p[k-j][i].set_running();
        
        for (j = 1; k+j < g_rows && p[k+j][i].is_white(); ++j)
          if (p[k+j][i].is_running())
            p[k+j][i].x_ &= ~q.x_;
      }
    }
  }
}

void precalculate()
{
  Uint16 a;
  for (Uint16 k = 1; k < 512; ++k) {
    if (k == lobit(k)) {// k == 1, 2, 4, ...
      a = k;
    }
    tab[k] = tab[k-a] + 1; // they repeat with just +1
  }

  for (Uint16 k = 0; k < 512; ++k) {
    Uint8 sum = 0;
    for (Uint8 j = 1; j <= 9; ++j) {
      if ((k >> (j - 1)) & 1) {
        sum += j;
      }
    }
    suma[k] = sum;
  }

  for (Uint16 k = 0; k < 512; ++k) {
    int i = abk[suma[k]][tab[k]]++;
    ab[suma[k]][tab[k]][i] = k;
  }
}

square::square() : orig_right_sum_(0), right_sum_(0),
                   orig_down_sum_(0), down_sum_(0),
                   x_(0), white_(false), running_(true) {}

bool square::is_white() {
  return white_;
}

void square::set_white() {
  white_ = true;
  x_ = 0x1FF;
}

bool square::is_black() {
  return !is_white();
}

void square::set_black(Uint8 a, Uint8 b) {
  white_ = false;
  orig_right_sum_ = right_sum_ = a;
  orig_down_sum_ = down_sum_ = b;
}

bool square::is_running() {
  return running_;
}

void square::set_running() {
  running_ = true;
}

bool square::is_done() {
  return !is_running();
}

void square::set_done() {
  running_ = false;
}

bool square::has_down_sum() {
  return orig_down_sum_ > 0;
}

bool square::has_right_sum() {
  return orig_right_sum_ > 0;
}

void square::set_bit(Uint8 a) {
  x_ |= bit(a);
}

void square::remove_bit(Uint8 a) {
  x_ &= ~bit(a);
}

void square::flip_bit(Uint8 a) {
  x_ ^= bit(a);
}

Uint16 square::get_bit(Uint8 a) {
  return (x_ >> (a - 1)) & 1;
}

int possibilities(Uint16 x) {
  return tab[x];
}

bool is_single(Uint16 x) {
  int p = possibilities(x);
  assert(p > 0);
  return p == 1;
}

int square::possibilities() {
  assert(is_white());
  return ::possibilities(x_);
}

bool square::is_single() {
  assert(is_white());
  return ::is_single(x_);
}

void square::remove_possibilities(Uint16 x) {
  assert(is_white());
  if (!is_running() || is_single()) return;
  x_ &= ~x;
}
