#include <mruby.h>
#include <mruby/compile.h>
#include <stdio.h>

#include <iostream>

#include "./RxReader.hpp"

const char *inputPath = "/home/gabriel/gabrielmartins.dev/dev/cpp/orm-xp/projects/Project1/Data/Map002.rxdata";

void log(const char *data)
{
  std::cout << data;
}

void logl(const char *data)
{
  std::cout << data << std::endl;
}

void handleArgs(int argc, char *argv[])
{
  // log("Arg: ");
  // logl(argv[2]);

  // std::string content = FileIO::readFile(inputPath);
  // logl(content.c_str());

  // RxReader::read(inputPath);
}

int main(int argc, char *argv[])
{
  // handleArgs(argc, argv);
  // return 0;

  mrb_state *mrb = mrb_open();
  if (!mrb) { /* handle error */
    std::cerr << "Failed to open mruby." << std::endl;
    return 1;
  }

  


  // puts("Executing Ruby code from C!");
  // mrb_load_string(mrb, "p 'hello world!'");

  // // Create a Ruby string
  // const char *c_string = "Hello, mruby!";
  // mrb_value ruby_string = mrb_str_new_cstr(mrb, c_string);

  // // Print the Ruby string in the C++ context (for debugging)
  // mrb_p(mrb, ruby_string);

  RxReader::read(mrb, inputPath);

  mrb_close(mrb);

  return 0;
}