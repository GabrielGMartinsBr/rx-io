#pragma once

#include <ruby.h>

#include <stdexcept>

#include "AppDefs.h"
#include "Log.hpp"
#include "MarshalUtils.hpp"

namespace Eng {

class Table {
 public:
  VALUE rbId = Qnil;

  static Table *deserialize(const char *data, int len)
  {
    if (len < 20) {
      throw std::runtime_error("Marshal error: Table has a bad file format");
    }

    MarshalUtils::readInt32(&data);
    int x = MarshalUtils::readInt32(&data);
    int y = MarshalUtils::readInt32(&data);
    int z = MarshalUtils::readInt32(&data);
    int size = MarshalUtils::readInt32(&data);

    if (size != x * y * z) {
      throw std::runtime_error("Marshal error: Table has a bad file format");
    } else if (len != 20 + x * y * z * 2) {
      throw std::runtime_error("Marshal error: Table has a bad file format");
    }

    Table *table = new Table(x, y, z);
    memcpy(&table->values[0], data, sizeof(int16_t) * size);

    return table;
  }

  int serialSize() const
  {
    return 20 + (xSize * ySize * zSize) * sizeof(int16_t);
  }

  void serialize(char *buffer) const
  {
    int size = xSize * ySize * zSize;
    int dim;
    if (zSize > 1) {
      dim = 3;
    } else if (ySize > 1) {
      dim = 2;
    } else {
      dim = 1;
    }

    MarshalUtils::writeInt32(&buffer, dim);
    MarshalUtils::writeInt32(&buffer, xSize);
    MarshalUtils::writeInt32(&buffer, ySize);
    MarshalUtils::writeInt32(&buffer, zSize);
    MarshalUtils::writeInt32(&buffer, size);

    writeSerializeValues(buffer, size);
  }

  void writeSerializeValues(char *buffer, int size) const
  {
    memcpy(buffer, &this->values[0], sizeof(int16_t) * size);
  }

  Table(int x, int y = 1, int z = 1) :
      xSize(x),
      ySize(y),
      zSize(z),
      values(x * y * z)
  {
  }

  // Clone another instance
  Table(const Table &source) :
      xSize(source.xSize),
      ySize(source.ySize),
      zSize(source.zSize),
      values(source.values) { }

  inline const int16_t getValue(int x, int y = 0, int z = 0) const
  {
    return values[calcIndex(x, y, z)];
  }

  inline const void setValue(int16_t value, int x, int y = 0, int z = 0)
  {
    if (x < 0 || x >= xSize) {
      Log::err() << "x index is out of range.";
      return;
    }
    if (y < 0 || y >= ySize) {
      Log::err() << "y index is out of range.";
      return;
    }
    if (z < 0 || z >= zSize) {
      Log::err() << "z index is out of range.";
      return;
    }
    values[calcIndex(x, y, z)] = value;
  }

  inline const int getXSize() const
  {
    return xSize;
  }

  inline const int getYSize() const
  {
    return ySize;
  }

  inline const int getZSize() const
  {
    return zSize;
  }

  void resize(int x, int y, int z)
  {
    if (x == xSize && y == ySize && z == zSize) {
      return;
    }

    app::Vector<int16_t> newValues(xSize * ySize * zSize);

    int xLimit = std::min(x, xSize);
    int yLimit = std::min(y, ySize);
    int zLimit = std::min(z, zSize);
    int index;

    for (int _z = 0; _z < zLimit; ++_z) {
      for (int _y = 0; _y < yLimit; ++_y) {
        for (int _x = 0; _x < xLimit; ++_x) {
          index = calcIndex(_x, _y, _z);
          newValues[index] = values[index];
        }
      }
    }

    values.swap(newValues);

    xSize = x;
    ySize = y;
    zSize = z;
  }

  void resize(int x, int y)
  {
    resize(x, y, zSize);
  }

  void resize(int x)
  {
    resize(x, ySize, zSize);
  }


 private:
  int xSize;
  int ySize;
  int zSize;
  app::Vector<int16_t> values;

  inline const int calcIndex(int x, int y = 0, int z = 0) const
  {
    return xSize * ySize * z + xSize * y + x;
  };

  void fill()
  {
    int index;
    for (int z = 0; z < zSize; ++z) {
      for (int y = 0; y < ySize; ++y) {
        for (int x = 0; x < xSize; ++x) {
          index = calcIndex(x, y, z);
          values[index] = 0;
        }
      }
    }
  }
};

}  // namespace Eng