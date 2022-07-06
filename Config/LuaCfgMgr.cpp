/*
** Lua binding: Config
** Generated automatically by tolua++-1.0.92 on Wed Jul  6 18:08:51 2022.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Config_open (lua_State* tolua_S);

#include "../Game/stdafx.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_ConfigMgr (lua_State* tolua_S)
{
 ConfigMgr* self = (ConfigMgr*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_LogicCfg (lua_State* tolua_S)
{
 LogicCfg* self = (LogicCfg*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_DbCfg (lua_State* tolua_S)
{
 DbCfg* self = (DbCfg*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"ConfigMgr");
 tolua_usertype(tolua_S,"LogicCfg");
 tolua_usertype(tolua_S,"DbCfg");
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
     !tolua_isusertype(tolua_S,2,"LogicCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  LogicCfg* config = ((LogicCfg*)  tolua_tousertype(tolua_S,2,0));
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

/* method: ReadDbCfg of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadDbCfg00
static int tolua_Config_ConfigMgr_ReadDbCfg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"DbCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  DbCfg* config = ((DbCfg*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadDbCfg'", NULL);
#endif
  {
   self->ReadDbCfg(config);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadDbCfg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ip of class  LogicCfg */
#ifndef TOLUA_DISABLE_tolua_get_LogicCfg_ip
static int tolua_get_LogicCfg_ip(lua_State* tolua_S)
{
  LogicCfg* self = (LogicCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ip'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->ip);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ip of class  LogicCfg */
#ifndef TOLUA_DISABLE_tolua_set_LogicCfg_ip
static int tolua_set_LogicCfg_ip(lua_State* tolua_S)
{
  LogicCfg* self = (LogicCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: port of class  LogicCfg */
#ifndef TOLUA_DISABLE_tolua_get_LogicCfg_port
static int tolua_get_LogicCfg_port(lua_State* tolua_S)
{
  LogicCfg* self = (LogicCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'port'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->port);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: port of class  LogicCfg */
#ifndef TOLUA_DISABLE_tolua_set_LogicCfg_port
static int tolua_set_LogicCfg_port(lua_State* tolua_S)
{
  LogicCfg* self = (LogicCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: maxSocketCnt of class  LogicCfg */
#ifndef TOLUA_DISABLE_tolua_get_LogicCfg_maxSocketCnt
static int tolua_get_LogicCfg_maxSocketCnt(lua_State* tolua_S)
{
  LogicCfg* self = (LogicCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'maxSocketCnt'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->maxSocketCnt);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: maxSocketCnt of class  LogicCfg */
#ifndef TOLUA_DISABLE_tolua_set_LogicCfg_maxSocketCnt
static int tolua_set_LogicCfg_maxSocketCnt(lua_State* tolua_S)
{
  LogicCfg* self = (LogicCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'maxSocketCnt'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->maxSocketCnt = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  LogicCfg */
#ifndef TOLUA_DISABLE_tolua_Config_LogicCfg_new00
static int tolua_Config_LogicCfg_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LogicCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   LogicCfg* tolua_ret = (LogicCfg*)  Mtolua_new((LogicCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"LogicCfg");
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

/* method: new_local of class  LogicCfg */
#ifndef TOLUA_DISABLE_tolua_Config_LogicCfg_new00_local
static int tolua_Config_LogicCfg_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"LogicCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   LogicCfg* tolua_ret = (LogicCfg*)  Mtolua_new((LogicCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"LogicCfg");
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

/* method: delete of class  LogicCfg */
#ifndef TOLUA_DISABLE_tolua_Config_LogicCfg_delete00
static int tolua_Config_LogicCfg_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"LogicCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  LogicCfg* self = (LogicCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: ip of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_get_DbCfg_ip
static int tolua_get_DbCfg_ip(lua_State* tolua_S)
{
  DbCfg* self = (DbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ip'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->ip);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ip of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_set_DbCfg_ip
static int tolua_set_DbCfg_ip(lua_State* tolua_S)
{
  DbCfg* self = (DbCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: user of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_get_DbCfg_user
static int tolua_get_DbCfg_user(lua_State* tolua_S)
{
  DbCfg* self = (DbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'user'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->user);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: user of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_set_DbCfg_user
static int tolua_set_DbCfg_user(lua_State* tolua_S)
{
  DbCfg* self = (DbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'user'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->user = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: passwd of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_get_DbCfg_passwd
static int tolua_get_DbCfg_passwd(lua_State* tolua_S)
{
  DbCfg* self = (DbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'passwd'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->passwd);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: passwd of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_set_DbCfg_passwd
static int tolua_set_DbCfg_passwd(lua_State* tolua_S)
{
  DbCfg* self = (DbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'passwd'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->passwd = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: database of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_get_DbCfg_database
static int tolua_get_DbCfg_database(lua_State* tolua_S)
{
  DbCfg* self = (DbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'database'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->database);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: database of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_set_DbCfg_database
static int tolua_set_DbCfg_database(lua_State* tolua_S)
{
  DbCfg* self = (DbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'database'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->database = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: port of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_get_DbCfg_port
static int tolua_get_DbCfg_port(lua_State* tolua_S)
{
  DbCfg* self = (DbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'port'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->port);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: port of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_set_DbCfg_port
static int tolua_set_DbCfg_port(lua_State* tolua_S)
{
  DbCfg* self = (DbCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_Config_DbCfg_new00
static int tolua_Config_DbCfg_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"DbCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   DbCfg* tolua_ret = (DbCfg*)  Mtolua_new((DbCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"DbCfg");
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

/* method: new_local of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_Config_DbCfg_new00_local
static int tolua_Config_DbCfg_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"DbCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   DbCfg* tolua_ret = (DbCfg*)  Mtolua_new((DbCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"DbCfg");
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

/* method: delete of class  DbCfg */
#ifndef TOLUA_DISABLE_tolua_Config_DbCfg_delete00
static int tolua_Config_DbCfg_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"DbCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  DbCfg* self = (DbCfg*)  tolua_tousertype(tolua_S,1,0);
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
   tolua_function(tolua_S,"ReadDbCfg",tolua_Config_ConfigMgr_ReadDbCfg00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"LogicCfg","LogicCfg","",tolua_collect_LogicCfg);
  #else
  tolua_cclass(tolua_S,"LogicCfg","LogicCfg","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"LogicCfg");
   tolua_variable(tolua_S,"ip",tolua_get_LogicCfg_ip,tolua_set_LogicCfg_ip);
   tolua_variable(tolua_S,"port",tolua_get_LogicCfg_port,tolua_set_LogicCfg_port);
   tolua_variable(tolua_S,"maxSocketCnt",tolua_get_LogicCfg_maxSocketCnt,tolua_set_LogicCfg_maxSocketCnt);
   tolua_function(tolua_S,"new",tolua_Config_LogicCfg_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_LogicCfg_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_LogicCfg_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_LogicCfg_delete00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"DbCfg","DbCfg","",tolua_collect_DbCfg);
  #else
  tolua_cclass(tolua_S,"DbCfg","DbCfg","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"DbCfg");
   tolua_variable(tolua_S,"ip",tolua_get_DbCfg_ip,tolua_set_DbCfg_ip);
   tolua_variable(tolua_S,"user",tolua_get_DbCfg_user,tolua_set_DbCfg_user);
   tolua_variable(tolua_S,"passwd",tolua_get_DbCfg_passwd,tolua_set_DbCfg_passwd);
   tolua_variable(tolua_S,"database",tolua_get_DbCfg_database,tolua_set_DbCfg_database);
   tolua_variable(tolua_S,"port",tolua_get_DbCfg_port,tolua_set_DbCfg_port);
   tolua_function(tolua_S,"new",tolua_Config_DbCfg_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_DbCfg_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_DbCfg_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_DbCfg_delete00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Config (lua_State* tolua_S) {
 return tolua_Config_open(tolua_S);
};
#endif

