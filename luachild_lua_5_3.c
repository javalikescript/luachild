#include "luachild.h"
#ifdef USE_LUAPUC
#if LUA_VERSION_NUM > 501

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
  return lua_rawlen(L, index);
}

static int file_close(lua_State *L) {
  int result = 1;
  luaL_Stream *lf = luaL_checkudata(L, 1, LUA_FILEHANDLE);
  if (!fclose(lf->f)) {
    lua_pushboolean(L, 1);
  } else {
    int en = errno;  /* calls to Lua API may change this value */
    lua_pushnil(L);
    lua_pushfstring(L, "%s", strerror(en));
    lua_pushinteger(L, en);
    result = 3;
  }
  lf->f = NULL;
  return result;
}

int file_handler_creator(lua_State *L, const char * file_path, int get_path_from_env){
  return 1;
}

void lua_pushcfile(lua_State *L, FILE * f){
  luaL_Stream *lf = lua_newuserdata(L, sizeof(luaL_Stream));
  luaL_getmetatable(L, LUA_FILEHANDLE);
  lua_setmetatable(L, -2);
  lf->f = f;
  lf->closef = &file_close;
}

FILE *check_file(lua_State *L, int idx, const char *argname)
{
  luaL_Stream *lf;
  if (idx > 0) lf = luaL_checkudata(L, idx, LUA_FILEHANDLE);
  else {
    idx = absindex(L, idx);
    lf = lua_touserdata(L, idx);
    luaL_getmetatable(L, LUA_FILEHANDLE);
    if (!lf || !lua_getmetatable(L, idx) || !lua_rawequal(L, -1, -2))
      luaL_error(L, "bad %s option (%s expected, got %s)",
                 argname, LUA_FILEHANDLE, luaL_typename(L, idx));
    lua_pop(L, 2);
  }
  if (!lf->f) return luaL_error(L, "attempt to use a closed file"), NULL;
  return lf->f;
}

#endif
#endif // USE_LUAPUC

