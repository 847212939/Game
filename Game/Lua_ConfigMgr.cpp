/*
** Lua binding: Config
** Generated automatically by tolua++-1.0.92 on Mon Jul  4 22:57:08 2022.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Config_open (lua_State* tolua_S);

#include "ConfigMgr.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_ConfigMgr (lua_State* tolua_S)
{
 ConfigMgr* self = (ConfigMgr*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"ConfigMgr");
}

/* method: new of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_new00
static int tolua_Config_ConfigMgr_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ConfigMgr* tolua_ret = (ConfigMgr*)  Mtolua_new((ConfigMgr)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ConfigMgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_new00_local
static int tolua_Config_ConfigMgr_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ConfigMgr* tolua_ret = (ConfigMgr*)  Mtolua_new((ConfigMgr)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ConfigMgr");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_delete00
static int tolua_Config_ConfigMgr_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'", NULL);
#endif
  Mtolua_delete(self);
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Run of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_Run00
static int tolua_Config_ConfigMgr_Run00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Run'", NULL);
#endif
  {
   self->Run();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Run'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Run of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_Run200
static int tolua_Config_ConfigMgr_Run200(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  int a = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Run'", NULL);
#endif
  {
   self->Run(a);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Run2'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_Config_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"ConfigMgr","ConfigMgr","",tolua_collect_ConfigMgr);
  #else
  tolua_cclass(tolua_S,"ConfigMgr","ConfigMgr","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"ConfigMgr");
   tolua_function(tolua_S,"new",tolua_Config_ConfigMgr_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_ConfigMgr_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_ConfigMgr_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_ConfigMgr_delete00);
   tolua_function(tolua_S,"Run",tolua_Config_ConfigMgr_Run00);
   tolua_function(tolua_S,"Run2",tolua_Config_ConfigMgr_Run200);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Config (lua_State* tolua_S) {
 return tolua_Config_open(tolua_S);
};
#endif

