#include "main.h"

int main()
{
  init();

  while( !quit ) {
    get_input();

    process();

    draw();
  }

  clean_up();
}
