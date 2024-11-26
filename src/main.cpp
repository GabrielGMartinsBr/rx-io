#include <mruby.h>
#include <mruby/compile.h>
#include <stdio.h>

#include <iostream>

int main()
{
  std::cout << "It works!" << std::endl;

  mrb_state *mrb = mrb_open();
  if (!mrb) { /* handle error */
  }
  puts("Executing Ruby code from C!");
  mrb_load_string(mrb, "p 'hello world!'");
  mrb_close(mrb);
  return 0;

  return 0;
}