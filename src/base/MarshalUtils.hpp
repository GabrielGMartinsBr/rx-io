#pragma once

#include <ruby.h>

#include <cstdint>
#include <cstring>

typedef VALUE (*Cb)(VALUE);

struct MarshalUtils {
  static VALUE load(VALUE str)
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

  static inline int readInt32(const char **data)
  {
    int32_t result;

    std::memcpy(&result, *data, 4);
    *data += 4;

    return result;
  }

  static inline int readFloat(const char **data)
  {
    float result;

    std::memcpy(&result, *data, 4);
    *data += 4;

    return result;
  }

  static inline int readDouble(const char **data)
  {
    double result;

    std::memcpy(&result, *data, 8);
    *data += 8;

    return result;
  }

  static inline void writeInt32(char **data, int32_t value)
  {
    memcpy(*data, &value, 4);
    *data += 4;
  }

  static inline void writeFloat(char **data, float value)
  {
    memcpy(*data, &value, 4);
    *data += 4;
  }

  static inline void writeDouble(char **data, double value)
  {
    memcpy(*data, &value, 8);
    *data += 8;
  }
};