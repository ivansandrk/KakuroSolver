#ifndef _SOL_H
#define _SOL_H

#include <assert.h>

#define lobit(x) ((x)&-(x))
#define rm_lobit(x) ((x)&=~lobit(x))
#define debug() \
do { \
  fprintf(stderr, "FILE: %s\nLINE: %d\ncounter: %d\n", __FILE__, __LINE__, counter); \
  ++counter; \
} while(0)
#define bit(x) (1<<((x)-1))

typedef unsigned char Uint8;
typedef unsigned short Uint16;
typedef unsigned int Uint32;
const Uint8 MAX_ROWS = 10;
const Uint8 MAX_COLS = 10;
extern int g_rows;
extern int g_cols;

extern Uint8 tab[512];
extern Uint8 suma[512];
extern Uint16 ab[46][10][12];
extern Uint8 abk[46][10];
extern Uint32 counter;

struct square {
 public:
  Uint8 right_sum;
  Uint8 down_sum;
  Uint16 x;

 private:
  bool white_;
  bool running_;

 public:
  square() : right_sum(0), down_sum(0), x(0), white_(false), running_(true) {}

  bool is_white() {
    return white_;
  }

  void set_white() {
    white_ = true;
    x = 0x1FF;
  }

  bool is_black() {
    return !is_white();
  }

  void set_black() {
    white_ = false;
    right_sum = down_sum = 0;
  }

  void set_black_numbers(Uint8 a, Uint8 b) {
    white_ = false;
    right_sum = a;
    down_sum = b;
  }

  bool is_running() {
    return running_;
  }

  void set_running() {
    running_ = true;
  }

  bool is_done() {
    return !is_running();
  }

  void set_done() {
    running_ = false;
  }

  bool has_down_sum() {
    return down_sum > 0;
  }

  bool has_right_sum() {
    return right_sum > 0;
  }

  void set_bit(Uint8 a) {
    x |= bit(a);
  }

  void remove_bit(Uint8 a) {
    x &= ~bit(a);
  }

  void flip_bit(Uint8 a) {
    x ^= bit(a);
  }

  Uint16 get_bit(Uint8 a) {
    return (x >> (a - 1)) & 1;
  }

  // Return number of possible values for this field.
  int possibilities() {
    assert(is_white());
    return tab[x];
  }

  // Return true if only one possible value for this field (ie. it's solved).
  bool is_single() {
    return possibilities() == 1;
  }
};

extern square p[MAX_ROWS][MAX_COLS];

void precalculate();
void solve_kakuro();

#endif /* _SOL_H */
