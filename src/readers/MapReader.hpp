#pragma once

#include <ruby.h>

#include <nlohmann/json.hpp>

#include "Convert.hpp"
#include "FileIO.hpp"
#include "Log.hpp"
#include "MarshalUtils.hpp"
#include "StrUtils.hpp"
#include "rbx/It_Table.hpp"

using json = nlohmann::json;

struct MapReader {
  static void read(const char* filePath)
  {
    Str content = FileIO::readFile(filePath);
    VALUE rbContent = rb_str_new(content.c_str(), content.size());

    VALUE rbResult = MarshalUtils::load(rbContent);

    parse(rbResult);
  }

  static void parse(VALUE rbMapObj)
  {
    VALUE rbClassName = rb_class_name(rb_class_of(rbMapObj));

    std::string className = Convert::toCStr(rbClassName);
    bool isMapInstance = className.compare("RPG::Map") == 0;
    if (!isMapInstance) {
      throw std::runtime_error("Input file is not a valid RGSS1 Map file.");
    }

    // printInstanceVarNames(rbMapObj);
    // printInstanceVar(rbMapObj, "@tileset_id");

    json mapObj;
    mapObj["width"] = Convert::toCInt(rb_iv_get(rbMapObj, "@width"));
    mapObj["height"] = Convert::toCInt(rb_iv_get(rbMapObj, "@height"));
    mapObj["tileset_id"] = Convert::toCInt(rb_iv_get(rbMapObj, "@tileset_id"));

    VALUE rbData = rb_iv_get(rbMapObj, "@data");
    Eng::Table* dataTable = It::Table::getObjectValue(rbData);

    int xSize = dataTable->getXSize();
    int ySize = dataTable->getYSize();
    int zSize = dataTable->getZSize();

    json arr;
    for (int z = 0; z < zSize; z++) {
      for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
          arr[z][y][x] = dataTable->getValue(x, y, z);
        }
      }
    }
    mapObj["data"] = arr;

    std::string strData = mapObj.dump();

    Log::out() << strData;
  }
};
