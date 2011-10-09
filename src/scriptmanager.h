
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

// This is the only header we need to include for LuaBind to work
#include "luabind/luabind.hpp"

class ScriptManager {
    lua_State *Lg;
  public:
    ScriptManager ();
    //onKeyDown (EKEY_CODE keyCode) // TODO: this
    void update (int);
  private:
    std::string get_error();
};

