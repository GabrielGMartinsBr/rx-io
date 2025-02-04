#pragma once

#include "Log.hpp"
#include "StrUtils.hpp"
#include "rbx/It_Color.hpp"
#include "rbx/It_Table.hpp"
#include "readers/MapReader.hpp"
#include "readers/TilesetReader.hpp"

using StrUtils::hash;

class RxIo {

 public:

  int run(int argc, char* argv[])
  {
    initRuby();

    if (argc == 1) {
      devTest();
    } else {
      handleArgs(argc, argv);
    }

    shutdownRuby();

    return 0;
  }

 private:

  void handleArgs(int argc, char* argv[])
  {
    const char* actionArg = argv[1];
    if (!actionArg) {
      Log::err() << "Ops, invalid action argument received.";
      return;
    }

    switch (hash(actionArg)) {
      case hash("read"):
        handleRead(argc, argv);
        break;
      case hash("write"):
        Log::out() << "You said write!";
        break;
      default:
        Log::out() << "Unknown input.";
        break;
    }
  }

  void handleRead(int argc, char* argv[])
  {
    const char* typeArg = argv[2];
    if (!typeArg) {
      Log::err() << "Ops, invalid type argument received.";
      return;
    }

    const char* pathArg = argv[3];
    if (!pathArg) {
      Log::err() << "Ops, invalid path argument received.";
      return;
    }

    // Log::out() << "{\n  type: '" << typeArg << "',\n  path: '" << pathArg << "'\n}";

    switch (hash(typeArg)) {
      case hash("map"): {
        MapReader::read(pathArg);
        break;
      }
      case hash("tileset"): {
        TilesetReader::read(pathArg);
        break;
      }
      default: {
        Log::err() << "Unexpected resource type received: " << typeArg;
      }
    }
  }

  void devTest()
  {
    const char* mapArgs[4] = {
      "./AeonRxIo",
      "read",
      "map",
      "/home/gabriel/gabrielmartins.dev/dev/cpp/orm-xp/projects/Project1/Data/Map001.rxdata"
    };

    const char* tilesetArgs[4] = {
      "./AeonRxIo",
      "read",
      "tileset",
      "/home/gabriel/gabrielmartins.dev/dev/cpp/orm-xp/projects/Project1/Data/Tilesets.rxdata"
    };

    // handleArgs(4, const_cast<char**>(tilesetArgs));
    handleArgs(4, const_cast<char**>(mapArgs));
  }

  void initRuby()
  {
    ruby_init();
    integrateRubyAssets();
    loadScriptsPath();
    loadRubyBuiltIns();
  }

  void shutdownRuby()
  {
    ruby_cleanup(0);
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

  void loadScriptsPath()
  {
    const char* scriptsDirPath = "/home/gabriel/gabrielmartins.dev/dev/aeon/rx-io/assets";
    VALUE loadPathArray = rb_gv_get("$LOAD_PATH");
    rb_funcall(loadPathArray, rb_intern("<<"), 1, rb_str_new2(scriptsDirPath));
  }

  void loadRubyBuiltIns()
  {
    require("RGSS-BuiltIn.rb");
  }

  void integrateRubyAssets()
  {
    It::Color::integrate();
    It::Table::integrate();
  }
};