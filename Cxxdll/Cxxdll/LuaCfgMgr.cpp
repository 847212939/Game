/*
** Lua binding: Config
** Generated automatically by tolua++-1.0.92 on Sat Aug 20 13:57:33 2022.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Config_open (lua_State* tolua_S);

#include "pch.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CLogicCfg (lua_State* tolua_S)
{
 CLogicCfg* self = (CLogicCfg*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

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
 tolua_usertype(tolua_S,"CLogicCfg");
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

/* method: ReadLogicCfg of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadLogicCfg00
static int tolua_Config_ConfigMgr_ReadLogicCfg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CLogicCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  CLogicCfg* config = ((CLogicCfg*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadLogicCfg'", NULL);
#endif
  {
   self->ReadLogicCfg(config);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadLogicCfg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadTimerCntCfg of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadTimerCntCfg00
static int tolua_Config_ConfigMgr_ReadTimerCntCfg00(lua_State* tolua_S)
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
  int cnt = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadTimerCntCfg'", NULL);
#endif
  {
   self->ReadTimerCntCfg(cnt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadTimerCntCfg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadKeyCfg of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadKeyCfg00
static int tolua_Config_ConfigMgr_ReadKeyCfg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  std::string key = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadKeyCfg'", NULL);
#endif
  {
   self->ReadKeyCfg(key);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadKeyCfg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ip of class  CLogicCfg */
#ifndef TOLUA_DISABLE_tolua_get_CLogicCfg_ip
static int tolua_get_CLogicCfg_ip(lua_State* tolua_S)
{
  CLogicCfg* self = (CLogicCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ip'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->ip);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ip of class  CLogicCfg */
#ifndef TOLUA_DISABLE_tolua_set_CLogicCfg_ip
static int tolua_set_CLogicCfg_ip(lua_State* tolua_S)
{
  CLogicCfg* self = (CLogicCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ip'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->ip = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: port of class  CLogicCfg */
#ifndef TOLUA_DISABLE_tolua_get_CLogicCfg_port
static int tolua_get_CLogicCfg_port(lua_State* tolua_S)
{
  CLogicCfg* self = (CLogicCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'port'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->port);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: port of class  CLogicCfg */
#ifndef TOLUA_DISABLE_tolua_set_CLogicCfg_port
static int tolua_set_CLogicCfg_port(lua_State* tolua_S)
{
  CLogicCfg* self = (CLogicCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'port'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->port = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CLogicCfg */
#ifndef TOLUA_DISABLE_tolua_Config_CLogicCfg_new00
static int tolua_Config_CLogicCfg_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CLogicCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CLogicCfg* tolua_ret = (CLogicCfg*)  Mtolua_new((CLogicCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CLogicCfg");
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

/* method: new_local of class  CLogicCfg */
#ifndef TOLUA_DISABLE_tolua_Config_CLogicCfg_new00_local
static int tolua_Config_CLogicCfg_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CLogicCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CLogicCfg* tolua_ret = (CLogicCfg*)  Mtolua_new((CLogicCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CLogicCfg");
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

/* method: delete of class  CLogicCfg */
#ifndef TOLUA_DISABLE_tolua_Config_CLogicCfg_delete00
static int tolua_Config_CLogicCfg_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CLogicCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CLogicCfg* self = (CLogicCfg*)  tolua_tousertype(tolua_S,1,0);
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
   tolua_function(tolua_S,"ReadLogicCfg",tolua_Config_ConfigMgr_ReadLogicCfg00);
   tolua_function(tolua_S,"ReadTimerCntCfg",tolua_Config_ConfigMgr_ReadTimerCntCfg00);
   tolua_function(tolua_S,"ReadKeyCfg",tolua_Config_ConfigMgr_ReadKeyCfg00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CLogicCfg","CLogicCfg","",tolua_collect_CLogicCfg);
  #else
  tolua_cclass(tolua_S,"CLogicCfg","CLogicCfg","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CLogicCfg");
   tolua_variable(tolua_S,"ip",tolua_get_CLogicCfg_ip,tolua_set_CLogicCfg_ip);
   tolua_variable(tolua_S,"port",tolua_get_CLogicCfg_port,tolua_set_CLogicCfg_port);
   tolua_function(tolua_S,"new",tolua_Config_CLogicCfg_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_CLogicCfg_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_CLogicCfg_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_CLogicCfg_delete00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Config (lua_State* tolua_S) {
 return tolua_Config_open(tolua_S);
};
#endif

