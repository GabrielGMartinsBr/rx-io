#pragma once

#include <ruby.h>

#include "Convert.hpp"
#include "RbUtils.hpp"
#include "Table.hpp"

namespace It {

class Table {
 public:

  static void integrate()
  {
    VALUE tableClass = rb_define_class("Table", rb_cObject);
    rb_define_alloc_func(tableClass, instance_allocator);

    // Serialize

    rb_define_module_function(tableClass, "_load", RUBY_METHOD_FUNC(method_load), 1);
    rb_define_module_function(tableClass, "_dump", RUBY_METHOD_FUNC(method_dump), 1);

    // Class methods and properties

    rb_define_method(tableClass, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);
    rb_define_method(tableClass, "[]", RUBY_METHOD_FUNC(getValue), -1);
    rb_define_method(tableClass, "[]=", RUBY_METHOD_FUNC(setValue), -1);
    rb_define_method(tableClass, "resize", RUBY_METHOD_FUNC(method_resize), -1);
    rb_define_method(tableClass, "xsize", RUBY_METHOD_FUNC(method_xsize), 0);
    rb_define_method(tableClass, "ysize", RUBY_METHOD_FUNC(method_ysize), 0);
    rb_define_method(tableClass, "zsize", RUBY_METHOD_FUNC(method_zsize), 0);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Table"));
  }

  static VALUE createRubyObject(Eng::Table *inst)
  {
    return Data_Wrap_Struct(getRbClass(), instance_mark, instance_free, inst);
  }

  static VALUE getRubyObject(Eng::Table *inst)
  {
    if (inst == nullptr) {
      return Qnil;
    }
    if (inst->rbId == Qnil) {
      inst->rbId = createRubyObject(inst);
    }
    return inst->rbId;
  }

  static Eng::Table *getObjectValue(VALUE rbObj)
  {
    return (Eng::Table *)DATA_PTR(rbObj);
  }

  // Private

 private:

  /*
    Allocator
  */

  static VALUE instance_allocator(VALUE instanceClass)
  {
    return Data_Wrap_Struct(instanceClass, instance_mark, instance_free, nullptr);
  }

  /*
    Deallocator
  */

  static void instance_free(void *ptr)
  {
    if (ptr == nullptr) return;
    delete static_cast<Eng::Table *>(ptr);
  }

  static void instance_mark(void *ptr)
  {
  }

  /*
    Deserialize / Marshal load
  */

  static VALUE method_load(VALUE self, VALUE marshaled_data)
  {
    Check_Type(marshaled_data, T_STRING);
    const char *data = RSTRING_PTR(marshaled_data);
    int len = RSTRING_LEN(marshaled_data);

    Eng::Table *table = Eng::Table::deserialize(data, len);
    return getRubyObject(table);
  }

  /*
    Serialize / Marshal dump
  */

  static VALUE method_dump(VALUE self, VALUE arg)
  {
    Eng::Table *inst = getObjectValue(self);
    int size = inst->serialSize();
    char *data = new char[size];

    inst->serialize(data);

    VALUE str = rb_str_new(data, size);
    delete[] data;

    return str;
  }

  /*
    Method initialize
  */

  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    Eng::Table *instance = nullptr;
    VALUE rb_x, rb_y, rb_z;

    if (argc == 1) {
      rb_scan_args(argc, argv, "1", &rb_x);
      int x = Convert::toCInt(rb_x);
      instance = new Eng::Table(x);
    } else if (argc == 2) {
      rb_scan_args(argc, argv, "2", &rb_x, &rb_y);
      int x = Convert::toCInt(rb_x);
      int y = Convert::toCInt(rb_y);
      instance = new Eng::Table(x, y);
    } else if (argc == 3) {
      rb_scan_args(argc, argv, "3", &rb_x, &rb_y, &rb_z);
      int x = Convert::toCInt(rb_x);
      int y = Convert::toCInt(rb_y);
      int z = Convert::toCInt(rb_z);
      instance = new Eng::Table(x, y, z);
    } else {
      throw std::runtime_error(
        "Table initialize takes exactly 1, 2, or 3 arguments, but received "
        + std::to_string(argc) + " arguments."
      );
    }

    DATA_PTR(self) = instance;
    instance->rbId = self;
    return self;
  }

  static VALUE getValue(int argc, VALUE *argv, VALUE self)
  {
    Eng::Table *inst = getObjectValue(self);
    VALUE rb_x, rb_y, rb_z;

    int x, y, z;
    x = y = z = 0;

    x = Convert::toCInt(argv[0]);
    if (argc > 1) {
      y = Convert::toCInt(argv[1]);
    }
    if (argc > 2) {
      z = Convert::toCInt(argv[2]);
    }
    if (argc > 3) {
      RbUtils::raiseRuntimeException(
        "Method get value takes exactly 1, 2, or 3 arguments, but received "
        + std::to_string(argc) + " arguments."
      );
      return Qnil;
    }

    if (
      x < 0 || x >= inst->getXSize()
      || y < 0 || y >= inst->getYSize()
      || z < 0 || z >= inst->getZSize()
    ) {
      return Qnil;
    }

    short value = inst->getValue(x, y, z);

    return Convert::toRubyNumber(value);
  }

  static VALUE setValue(int argc, VALUE *argv, VALUE self)
  {
    Eng::Table *inst = getObjectValue(self);
    VALUE rb_v, rb_x, rb_y, rb_z;

    if (argc == 2) {
      rb_scan_args(argc, argv, "2", &rb_x, &rb_v);
      int x = Convert::toCInt(rb_x);
      int v = Convert::toCInt(rb_v);
      inst->setValue(v, x);
      return Qnil;
    }

    if (argc == 3) {
      rb_scan_args(argc, argv, "3", &rb_x, &rb_y, &rb_v);
      int x = Convert::toCInt(rb_x);
      int y = Convert::toCInt(rb_y);
      int v = Convert::toCInt(rb_v);
      inst->setValue(v, x, y);
      return Qnil;
    }

    if (argc == 4) {
      rb_scan_args(argc, argv, "4", &rb_x, &rb_y, &rb_z, &rb_v);
      int x = Convert::toCInt(rb_x);
      int y = Convert::toCInt(rb_y);
      int z = Convert::toCInt(rb_z);
      int v = Convert::toCInt(rb_v);
      inst->setValue(v, x, y, z);
      return Qnil;
    }

    throw std::runtime_error(
      "Method set value takes exactly 2, 3, or 4 arguments, but received "
      + std::to_string(argc) + " arguments."
    );
  }

  static VALUE method_resize(int argc, VALUE *argv, VALUE self)
  {
    if (argc == 0 || argc > 3) {
      throw std::runtime_error(
        "Method resize takes exactly 1, 2, or 3 arguments, but received "
        + std::to_string(argc) + " arguments."
      );
    }

    return Qnil;
  }

  static VALUE method_xsize(VALUE self)
  {
    Eng::Table *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getXSize());
  }

  static VALUE method_ysize(VALUE self)
  {
    Eng::Table *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getYSize());
  }

  static VALUE method_zsize(VALUE self)
  {
    Eng::Table *inst = getObjectValue(self);
    return Convert::toRubyNumber(inst->getZSize());
  }
};
}  // namespace It