#include <ruby.h>

#include <iostream>

#include "./RxReader.hpp"
#include "./It_Color.hpp"
#include "./It_Table.hpp"

// const char *inputPath = "/home/gabriel/gabrielmartins.dev/dev/cpp/orm-xp/projects/Project1/Data/Map002.rxdata";
// const char *inputPath = "/home/gabriel/gabrielmartins.dev/dev/cpp/orm-xp/projects/Project1/Data/Scripts.rxdata";
// const char *inputPath = "/home/gabriel/gabrielmartins.dev/dev/cpp/orm-xp/projects/Project1/Data/Map001.rxdata";
const char *inputPath = "/home/gabriel/gabrielmartins.dev/dev/cpp/orm-xp/projects/Project1/Data/Tilesets.rxdata";

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

  ruby_init();

  const char *scriptsDirPath = "/home/gabriel/gabrielmartins.dev/dev/aeon/rx-io/assets";
  VALUE loadPathArray = rb_gv_get("$LOAD_PATH");
  rb_funcall(loadPathArray, rb_intern("<<"), 1, rb_str_new2(scriptsDirPath));

  It::Color::integrate();
  It::Table::integrate();

  RxReader::require("RGSS-BuiltIn.rb");

  RxReader::read(inputPath);

  ruby_cleanup(0);

  return 0;
}