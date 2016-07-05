#ifndef _SOL_H
#define _SOL_H

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
const Uint8 MAX_ROWS = 15;
const Uint8 MAX_COLS = 15;
extern int g_rows;
extern int g_cols;

struct square {
 public:
  Uint8 orig_right_sum_;
  Uint8 right_sum_;
  Uint8 orig_down_sum_;
  Uint8 down_sum_;
  Uint16 x_;

 private:
  bool white_;
  bool running_;

 public:
  square();

  bool is_white() const;
  void set_white();
  bool is_black() const;
  void set_black(Uint8 a = 0, Uint8 b = 0);
  bool is_running() const;
  void set_running();
  bool is_done() const;
  void set_done();
  bool has_down_sum() const;
  bool has_right_sum() const;
  void set_bit(Uint8 a);
  void remove_bit(Uint8 a);
  void flip_bit(Uint8 a);
  Uint16 get_bit(Uint8 a) const;
  // Return number of possible values for this field.
  int possibilities() const;
  // Return true if only one possible value for this field (ie. it's solved).
  bool is_single() const;
  void remove_possibilities(Uint16 x);
  // Return the actual value of a white square (with a single possibility),
  // eg. for x_ = 0x08 -> 4.
  Uint8 value() const;
};

extern square p[MAX_ROWS][MAX_COLS];

void precalculate();
void solve_kakuro();

#endif /* _SOL_H */
