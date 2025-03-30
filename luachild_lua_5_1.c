#include "luachild.h"
#ifdef USE_LUAPUC
#if LUA_VERSION_NUM = 501

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "lualib.h"
#include "lauxlib.h"

int lua_report_type_error(lua_State *L, int narg, const char * tname) {
  // TODO : proper implementation !-
  printf("bad argument %d to function %s\n", narg, tname);
  return 0;
}

size_t lua_value_length(lua_State *L, int index) {
  return lua_objlen(L, index);
}

int file_handler_creator(lua_State *L, const char * file_path, int get_path_from_env){
  return 1;
}

void lua_pushcfile(lua_State *L, FILE * f){
  FILE **pf = (FILE **)lua_newuserdata(L, sizeof(FILE *));
  luaL_getmetatable(L, LUA_FILEHANDLE);
  lua_setmetatable(L, -2);
  *pf = f;
}

FILE *check_file(lua_State *L, int idx, const char *argname)
{
  FILE **pf;
  if (idx > 0) pf = luaL_checkudata(L, idx, LUA_FILEHANDLE);
  else {
    idx = absindex(L, idx);
    pf = lua_touserdata(L, idx);
    luaL_getmetatable(L, LUA_FILEHANDLE);
    if (!pf || !lua_getmetatable(L, idx) || !lua_rawequal(L, -1, -2))
      luaL_error(L, "bad %s option (%s expected, got %s)",
                 argname, LUA_FILEHANDLE, luaL_typename(L, idx));
    lua_pop(L, 2);
  }
  if (!*pf) return luaL_error(L, "attempt to use a closed file"), NULL;
  return *pf;
}

#endif
#endif // USE_LUAPUC

