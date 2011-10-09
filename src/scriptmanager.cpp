// Include the lua headers (the extern "C" is a requirement because we're
// using C++ and lua has been compiled as C code)
#include "scriptmanager.h"
#include <iostream>
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

ScriptManager::ScriptManager () {
    /* Set up Lua */
        this->Lg = lua_open();
        luabind::open(this->Lg);
        luaL_openlibs(this->Lg);
        luaL_dofile(this->Lg,"src/scripts/test.lua");
        
}

void ScriptManager::update (int frame) {
    try {
        luabind::call_function<void>(this->Lg,"update",frame);
    } catch(const std::exception &TheError) {
        std::cout << this->get_error() << std::endl;        
    }
}

std::string ScriptManager::get_error () {
    if (!lua_isstring(this->Lg, lua_gettop(this->Lg)))
                return "";

    return lua_tostring(this->Lg, lua_gettop(this->Lg));
    lua_pop(this->Lg, 1);
}
