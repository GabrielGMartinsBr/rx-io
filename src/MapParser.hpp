#include <ruby.h>

#include <nlohmann/json.hpp>

#include "Convert.hpp"
#include "It_Table.hpp"
#include "Log.hpp"
#include "RbUtils.hpp"
#include "Table.hpp"

using json = nlohmann::json;

class MapParser {
 public:

  static void readMap(VALUE rbMapObj)
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

  static void parserTileset(VALUE tilesets)
  {
    int size = RbUtils::getArraySize(tilesets);
    Log::out() << size;

    for (int i = 0; i < size; i++) {
      VALUE entry = rb_ary_entry(tilesets, i);
      if (entry == Qnil) continue;
      // rb_p(entry);
      // printInstanceVarNames(entry);
      VALUE rbValue = rb_iv_get(entry, "@tileset_name");
      const char* value = Convert::toCStr(rbValue);
      Log::out() << "[" << i << "]: " << value;
    }
  }

  static void printInstanceVarNames(VALUE rbObj)
  {
    VALUE varsArr = rb_obj_instance_variables(rbObj);
    rb_p(varsArr);
  }

  static void printInstanceVar(VALUE rbInst, const char* name)
  {
    rb_p(rb_iv_get(rbInst, name));
  }
};