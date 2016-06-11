#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

#define lobit(x) ((x)&-(x))
#define rm_lobit(x) ((x)&=~lobit(x))
#define debug() \
do { \
  fprintf( stderr, "LINE: %d\ncounter: %d\n", __LINE__, counter ); \
  ++counter; \
} while(0)

typedef unsigned char Uint8;
typedef unsigned short Uint16;
typedef unsigned int Uint32;
typedef vector< Uint8 > V8;
typedef vector< V8 > VV8;

Uint8 tab[512];
Uint8 suma[512];
Uint16 ab[46][10][12];
Uint8 abk[46][10];
Uint32 counter = 0;

void precalculate()
{
  Uint16 a;
  for( Uint16 k = 1; k < 512; ++k )
  {
    if( k == lobit(k) ) // k == 1, 2, 4, ...
      a = k;
    tab[k] = tab[k-a] + 1; // they repeat with just +1
  }

  for( Uint16 k = 0; k < 512; ++k )
  {
    Uint8 sum = 0;
    for( Uint8 j = 1; j <= 9; ++j )
    {
      if( ((k>>(j-1))&1) )
      {
        sum += j;
      }
    }
    suma[k] = sum;
  }

  for( Uint16 k = 0; k < 512; ++k )
  {
    int i = abk[suma[k]][tab[k]]++;
    ab[suma[k]][tab[k]][i] = k;
  }
}

Uint8 max_u_b( Uint8 b )
{
  return (9*10-(9-b)*(10-b))/2;
}

Uint8 min_u_b( Uint8 b )
{
  return (b*(b+1))/2;
}

int main()
{
  precalculate();
  Uint8 a, b;
  scanf( "%hhd%hhd", &a, &b );
  for( Uint8 k = 0; k < abk[a][b]; ++k )
  {
    for( Uint8 j = 1; j <= 9; ++j )
    {
      if( ab[a][b][k] & (1<<(j-1)) )
      {
        printf( "%d ", j );
      }
    }
    printf( "\n" );
  }
}
