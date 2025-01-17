#pragma once

#include <ruby.h>

#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "./FileIO.hpp"
#include "./It_Table.hpp"
#include "./MapParser.hpp"

typedef VALUE (*Cb)(VALUE);

class RxReader {

 public:

  static void read(const char* filePath)
  {
    std::string content = FileIO::readFile(filePath);
    VALUE rbContent = rb_str_new(content.c_str(), content.size());

    VALUE rbResult = marshalLoadProtect(rbContent);
    // VALUE rbResult = marshalLoad(rb_content);

    // rb_p(rbResult);
    MapParser::parserTileset(rbResult);

    // MapParser::readMap(rbResult);
  }

  static VALUE marshalLoad(VALUE str)
  {
    VALUE obj = rb_marshal_load(str);
    VALUE errorMsg = rb_gv_get("$!");
    if (errorMsg != Qnil) {
      rb_p(errorMsg);
      throw std::runtime_error("Marshal load failed.");
    }
    return obj;
  }

  static VALUE marshalLoadProtect(VALUE str)
  {
    int error;
    VALUE result = rb_protect((Cb)RUBY_METHOD_FUNC(rb_marshal_load), str, &error);
    if (error) {
      VALUE error_message = rb_gv_get("$!");
      rb_p(error_message);
      std::exit(1);
    }
    return result;
  }

  static void require(const char* c)
  {
    int error;
    VALUE res = rb_protect((Cb)RUBY_METHOD_FUNC(rb_require), (VALUE)c, &error);
    if (error) {
      VALUE error_message = rb_gv_get("$!");
      rb_p(error_message);
      std::exit(1);
    }
  }
};