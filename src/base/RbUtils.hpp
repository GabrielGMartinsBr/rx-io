#pragma once

#include <ruby.h>

#include <stdexcept>

#include "AppDefs.h"
#include "Convert.hpp"

struct RbUtils {
  static void printInstanceVarNames(VALUE rbObj)
  {
    VALUE varsArr = rb_obj_instance_variables(rbObj);
    rb_p(varsArr);
  }

  static void printInstanceVar(VALUE rbInst, const char* name)
  {
    rb_p(rb_iv_get(rbInst, name));
  }

  static VALUE createRbString(app::String& str)
  {
    VALUE rbStr = rb_str_new(str.c_str(), str.size());
    if (NIL_P(rbStr)) {
      throw std::runtime_error("Failed to create ruby string.");
    }
    return rbStr;
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

  static long getArraySize(VALUE arr)
  {
    return RARRAY_LEN(arr);
  }

  static app::String parseString(VALUE rbStr)
  {
    const char* strPtr = RSTRING_PTR(rbStr);
    long length = RSTRING_LEN(rbStr);
    return app::String(strPtr, length);
  }

  static int parseFix(VALUE rbFixNum)
  {
    Check_Type(rbFixNum, T_FIXNUM);
    return FIX2INT(rbFixNum);
  }

  static long parseFix2Long(VALUE rbFixNum)
  {
    Check_Type(rbFixNum, T_FIXNUM);
    return FIX2LONG(rbFixNum);
  }

  static long parseNum(VALUE rbFixNum)
  {
    Check_Type(rbFixNum, T_BIGNUM);
    return NUM2LONG(rbFixNum);
  }

  static void raiseRuntimeException(app::String msg)
  {
    VALUE exceptionClass = rb_eRuntimeError;
    VALUE ex = rb_exc_new2(exceptionClass, msg.c_str());
    rb_exc_raise(ex);
  }

  static void raiseCantConvertError(VALUE from, VALUE to)
  {
    app::String fromName = Convert::toCStr(rb_class_name(from));
    app::String toName = Convert::toCStr(rb_class_name(to));
    app::String msg = "Can't convert " + fromName + " into " + toName;
    raiseRuntimeException(msg);
  }
};