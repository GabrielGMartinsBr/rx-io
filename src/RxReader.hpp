#pragma once

#include <ruby.h>

#include <iostream>
#include <ostream>
#include <string>

#include "./FileIO.hpp"
#include "./It_Table.hpp"

typedef VALUE (*Cb)(VALUE);

class RxReader {

 public:

  static void read(const char* filePath)
  {
    // std::cout << "Reading file..." << std::endl;

    std::string content = FileIO::readFile(filePath);
    VALUE rb_content = rb_str_new(content.c_str(), content.size());

    VALUE rb_result = marshalLoadProtect(rb_content);
    // VALUE rb_result = marshalLoad(rb_content);
    // rb_p(rb_result);

    VALUE varsArr = rb_obj_instance_variables(rb_result);
    rb_p(varsArr);

    VALUE rb_data = rb_iv_get(rb_result, "@data");
    Eng::Table* t = It::Table::getObjectValue(rb_data);

    Log::out() << t->getValue(1);
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