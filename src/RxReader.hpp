#pragma once

#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/variable.h>

#include <iostream>
#include <stdexcept>
#include <string>

#include "./FileIO.hpp"

class RxReader {

 public:

  static void read(mrb_state* mrb, const char* filePath)
  {
    // std::cout << "Reading file..." << std::endl;
    std::string content = FileIO::readFile(filePath);

    // const char* ruby_code = "Marshal.load($data)";

    mrb_sym varName = mrb_intern_lit(mrb, "$data");
    mrb_value varContent = mrb_str_new_cstr(mrb, content.c_str());
    mrb_gv_set(mrb, varName, varContent);

    // mrb_load_string(mrb, "puts $data");

    // mrb_gv_set(mrb, mrb_intern_cstr(mrb, "$data"), ruby_string);

    const char* ruby_code = "Marshal.load($data)";
    mrb_value result = mrb_load_string(mrb, ruby_code);

    std::cout << "exc: " << mrb->exc << std::endl;
    if (mrb->exc) {
      mrb_print_error(mrb);
      throw std::runtime_error("Mrb Error!");
    }

    // mrb_p(mrb, ruby_string);
  }

  static void marshalLoad(const std::string& content)
  {
    mrb_state* mrb = mrb_open();
    if (!mrb) { /* handle error */
    }

    mrb_value ruby_string = mrb_str_new_cstr(mrb, content.c_str());

    const char* ruby_code = "Marshal.load($data)";

    // mrb_gv_set(mrb, mrb_intern_cstr(mrb, "$data"), ruby_string);

    // mrb_value result = mrb_load_string(mrb, ruby_code);

    // const char* ruby_code = "Marshal.load($data)";
  }
};