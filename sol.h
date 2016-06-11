#ifndef _SOL_H
#define _SOL_H

#define lobit(x) ((x)&-(x))
#define rm_lobit(x) ((x)&=~lobit(x))
#define debug() \
do { \
  fprintf( stderr, "FILE: %s\nLINE: %d\ncounter: %d\n", __FILE__, __LINE__, counter ); \
  ++counter; \
} while(0)
#define bit(x) (1<<((x)-1))

typedef unsigned char Uint8;
typedef unsigned short Uint16;
typedef unsigned int Uint32;
const Uint8 MAX_ROWS = 7; // 7, 7
const Uint8 MAX_COLS = 7;

struct square {
  Uint8 flags;
  union {
    struct { Uint8 right_sum, down_sum; };
    Uint16 x;
  };
  static const Uint8 mask_black = 0x01;
  static const Uint8 value_black = 0x00;
  static const Uint8 mask_white = 0x01;
  static const Uint8 value_white = 0x01;
  static const Uint8 mask_running = 0x02;
  static const Uint8 value_running = 0x00;
  static const Uint8 mask_done = 0x02;
  static const Uint8 value_done = 0x02;

  static const Uint8 white_flags = 0x01;
  static const Uint16 white_x = 0x1ff;
  static const Uint8 black_flags_done = 0x02;
  static const Uint8 black_flags_running = 0x00;
  static const Uint8 black_right_sum = 0;
  static const Uint8 black_down_sum = 0;

//  square() : flags( _default.flags ), right_sum( _default.right_sum ), down_sum( _default.down_sum ), x( _default.x ) {}
  bool is_black() { return ((flags & mask_black) == value_black); }
  bool is_white() { return ((flags & mask_white) == value_white); }
  bool is_running() { return ((flags & mask_running) == value_running); }
  bool is_done() { return ((flags & mask_done) == value_done); }
  bool has_down_sum() { return down_sum > 0; }
  bool has_right_sum() { return right_sum > 0; }
  void set_white() { flags = white_flags; x = white_x; }
  void set_black_numbers( const Uint8& a, const Uint8& b ) { flags = black_flags_running; right_sum = a; down_sum = b; }
  void set_black() { flags = black_flags_done; right_sum = black_right_sum; down_sum = black_down_sum; }
  void set_bit( const Uint8& a ) { x |= bit(a); }
  void remove_bit( const Uint8& a ) { x &= ~bit(a); }
  void flip_bit( const Uint8& a ) { x ^= bit(a); }
  Uint16 get_bit( const Uint8& a ) { return ((x>>(a-1))&1);}
  void set_done() { flags |= mask_done; }
  void set_running() { flags &= ~mask_running; }
};

extern Uint8 tab[512];
extern Uint8 suma[512];
extern Uint16 ab[46][10][12];
extern Uint8 abk[46][10];
extern Uint32 counter;
extern square p[MAX_ROWS][MAX_COLS];

void precalculate();
void solve_kakuro();

#endif /* _SOL_H */
