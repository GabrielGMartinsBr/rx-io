#pragma once

#include <ruby.h>

namespace It {

class Color {
 public:

  static void integrate()
  {
    VALUE colorClass = rb_define_class("Color", rb_cObject);
    rb_define_alloc_func(colorClass, instance_allocator);

    // Serialize

    rb_define_module_function(colorClass, "_load", RUBY_METHOD_FUNC(method_load), 1);
    // rb_define_module_function(colorClass, "_dump", RUBY_METHOD_FUNC(method_dump), 1);

    // Initialize

    rb_define_method(colorClass, "initialize", RUBY_METHOD_FUNC(method_initialize), -1);
  }

  // Utils

  static VALUE getRbClass()
  {
    return rb_const_get(rb_cObject, rb_intern("Color"));
  }

  // static VALUE createRubyObject(Eng::Color *inst)
  // {
  //   return Data_Wrap_Struct(getRbClass(), instance_mark, instance_free, inst);
  // }

  // static VALUE getRubyObject(Eng::Color *inst)
  // {
  //   if (inst == nullptr) {
  //     return Qnil;
  //   }
  //   if (inst->rbObj == Qnil) {
  //     inst->rbObj = createRubyObject(inst);
  //   }
  //   return inst->rbObj;
  // }

  // static Eng::Color *getObjectValue(VALUE rbObj)
  // {
  //   return (Eng::Color *)DATA_PTR(rbObj);
  // }

  // static inline bool isInstance(VALUE inst)
  // {
  //   return rb_class_of(inst) == getRbClass();
  // }

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
    // if (ptr == nullptr) return;
    // delete static_cast<Eng::Color *>(ptr);
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

    // Eng::Color *color = Eng::Color::deserialize(data, len);
    // VALUE value = getRubyObject(color);
    // return value;
    return Qnil;
  }

  /*
    Serialize / Marshal dump
  */

  // static VALUE method_dump(VALUE self, VALUE arg)
  // {
  //   char *data = new char[Eng::Color::SERIAL_SIZE];

  //   Eng::Color *inst = getObjectValue(self);
  //   inst->serialize(data);

  //   VALUE str = rb_str_new(data, Eng::Color::SERIAL_SIZE);
  //   delete[] data;

  //   return str;
  // }

  // Initialize

  static VALUE method_initialize(int argc, VALUE *argv, VALUE self)
  {
    // Eng::Color *instance;
    // VALUE rb_r, rb_g, rb_b, rb_a;

    // if (argc == 3) {
    //   rb_scan_args(argc, argv, "3", &rb_r, &rb_g, &rb_b);
    //   double r = Convert::toCDouble2(rb_r);
    //   double g = Convert::toCDouble2(rb_g);
    //   double b = Convert::toCDouble2(rb_b);
    //   instance = new Eng::Color(self, r, g, b, 255);
    // } else if (argc == 4) {
    //   rb_scan_args(argc, argv, "4", &rb_r, &rb_g, &rb_b, &rb_a);
    //   double r = Convert::toCDouble2(rb_r);
    //   double g = Convert::toCDouble2(rb_g);
    //   double b = Convert::toCDouble2(rb_b);
    //   double a = Convert::toCDouble2(rb_a);
    //   instance = new Eng::Color(self, r, g, b, a);
    // } else {
    //   RbUtils::raiseRuntimeException(
    //     "Color initialize takes 3 or 4 arguments, but " + std::to_string(argc) + " were received."
    //   );
    //   return Qnil;
    // }

    // DATA_PTR(self) = instance;
    return self;
  }

  // // Method to_s

  // static VALUE method_to_s(VALUE self)
  // {
  //   Eng::Color &i = *getObjectValue(self);
  //   char buffer[64];
  //   sprintf(buffer, "(%f, %f, %f, %f)", i.red, i.green, i.blue, i.alpha);
  //   return Convert::toRubyString(buffer);
  // }
};

}  // namespace It