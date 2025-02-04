#pragma once

#include <ruby.h>

#include "FileIO.hpp"
#include "Log.hpp"
#include "MarshalUtils.hpp"
#include "RbUtils.hpp"
#include "StrUtils.hpp"

struct TilesetReader {
  static void read(const char* filePath)
  {
    Str content = FileIO::readFile(filePath);
    VALUE rbContent = rb_str_new(content.c_str(), content.size());

    VALUE rbResult = MarshalUtils::load(rbContent);

    parse(rbResult);
  }

  static void parse(VALUE rbObj)
  {
    int size = RbUtils::getArraySize(rbObj);
    Log::out() << size;

    for (int i = 0; i < size; i++) {
      VALUE entry = rb_ary_entry(rbObj, i);
      if (entry == Qnil) continue;
      // rb_p(entry);
      // printInstanceVarNames(entry);
      VALUE rbValue = rb_iv_get(entry, "@tileset_name");
      const char* value = Convert::toCStr(rbValue);
      Log::out() << "[" << i << "]: " << value;
    }
  }
};