/*
** Lua binding: Config
** Generated automatically by tolua++-1.0.92 on Sun Oct 23 14:05:36 2022.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_Config_open (lua_State* tolua_S);

#include "../stdafx.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_CDbCfg (lua_State* tolua_S)
{
 CDbCfg* self = (CDbCfg*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_ConfigMgr (lua_State* tolua_S)
{
 ConfigMgr* self = (ConfigMgr*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CServerCfg (lua_State* tolua_S)
{
 CServerCfg* self = (CServerCfg*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"CBaseCfgMgr");
 tolua_usertype(tolua_S,"ConfigMgr");
 tolua_usertype(tolua_S,"CServerCfg");
 tolua_usertype(tolua_S,"CDbCfg");
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

/* method: CoutLog of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_CoutLog00
static int tolua_Config_ConfigMgr_CoutLog00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  int level = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* log = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'CoutLog'", NULL);
#endif
  {
   self->CoutLog(level,log);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'CoutLog'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetCBaseCfgMgr of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_GetCBaseCfgMgr00
static int tolua_Config_ConfigMgr_GetCBaseCfgMgr00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetCBaseCfgMgr'", NULL);
#endif
  {
   CBaseCfgMgr& tolua_ret = (CBaseCfgMgr&)  self->GetCBaseCfgMgr();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"CBaseCfgMgr");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetCBaseCfgMgr'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadServerCfg of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadServerCfg00
static int tolua_Config_ConfigMgr_ReadServerCfg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CServerCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  CServerCfg* config = ((CServerCfg*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadServerCfg'", NULL);
#endif
  {
   self->ReadServerCfg(config);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadServerCfg'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,2,"CDbCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  CDbCfg* config = ((CDbCfg*)  tolua_tousertype(tolua_S,2,0));
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

/* method: ReadThreadCntCfg of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadThreadCntCfg00
static int tolua_Config_ConfigMgr_ReadThreadCntCfg00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadThreadCntCfg'", NULL);
#endif
  {
   self->ReadThreadCntCfg(cnt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadThreadCntCfg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadLogPrintTmCfg of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadLogPrintTmCfg00
static int tolua_Config_ConfigMgr_ReadLogPrintTmCfg00(lua_State* tolua_S)
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
  int tm = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadLogPrintTmCfg'", NULL);
#endif
  {
   self->ReadLogPrintTmCfg(tm);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadLogPrintTmCfg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadMaxSocketCntCfg of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadMaxSocketCntCfg00
static int tolua_Config_ConfigMgr_ReadMaxSocketCntCfg00(lua_State* tolua_S)
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
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadMaxSocketCntCfg'", NULL);
#endif
  {
   self->ReadMaxSocketCntCfg(cnt);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadMaxSocketCntCfg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadExitCfg of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadExitCfg00
static int tolua_Config_ConfigMgr_ReadExitCfg00(lua_State* tolua_S)
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
  std::string exit = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadExitCfg'", NULL);
#endif
  {
   self->ReadExitCfg(exit);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadExitCfg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: ip of class  CServerCfg */
#ifndef TOLUA_DISABLE_tolua_get_CServerCfg_ip
static int tolua_get_CServerCfg_ip(lua_State* tolua_S)
{
  CServerCfg* self = (CServerCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ip'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->ip);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ip of class  CServerCfg */
#ifndef TOLUA_DISABLE_tolua_set_CServerCfg_ip
static int tolua_set_CServerCfg_ip(lua_State* tolua_S)
{
  CServerCfg* self = (CServerCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: port of class  CServerCfg */
#ifndef TOLUA_DISABLE_tolua_get_CServerCfg_port
static int tolua_get_CServerCfg_port(lua_State* tolua_S)
{
  CServerCfg* self = (CServerCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'port'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->port);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: port of class  CServerCfg */
#ifndef TOLUA_DISABLE_tolua_set_CServerCfg_port
static int tolua_set_CServerCfg_port(lua_State* tolua_S)
{
  CServerCfg* self = (CServerCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  CServerCfg */
#ifndef TOLUA_DISABLE_tolua_Config_CServerCfg_new00
static int tolua_Config_CServerCfg_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CServerCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CServerCfg* tolua_ret = (CServerCfg*)  Mtolua_new((CServerCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CServerCfg");
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

/* method: new_local of class  CServerCfg */
#ifndef TOLUA_DISABLE_tolua_Config_CServerCfg_new00_local
static int tolua_Config_CServerCfg_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CServerCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CServerCfg* tolua_ret = (CServerCfg*)  Mtolua_new((CServerCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CServerCfg");
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

/* method: delete of class  CServerCfg */
#ifndef TOLUA_DISABLE_tolua_Config_CServerCfg_delete00
static int tolua_Config_CServerCfg_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CServerCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CServerCfg* self = (CServerCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: ip of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_get_CDbCfg_ip
static int tolua_get_CDbCfg_ip(lua_State* tolua_S)
{
  CDbCfg* self = (CDbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'ip'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->ip);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: ip of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_set_CDbCfg_ip
static int tolua_set_CDbCfg_ip(lua_State* tolua_S)
{
  CDbCfg* self = (CDbCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: user of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_get_CDbCfg_user
static int tolua_get_CDbCfg_user(lua_State* tolua_S)
{
  CDbCfg* self = (CDbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'user'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->user);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: user of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_set_CDbCfg_user
static int tolua_set_CDbCfg_user(lua_State* tolua_S)
{
  CDbCfg* self = (CDbCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: passwd of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_get_CDbCfg_passwd
static int tolua_get_CDbCfg_passwd(lua_State* tolua_S)
{
  CDbCfg* self = (CDbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'passwd'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->passwd);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: passwd of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_set_CDbCfg_passwd
static int tolua_set_CDbCfg_passwd(lua_State* tolua_S)
{
  CDbCfg* self = (CDbCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: database of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_get_CDbCfg_database
static int tolua_get_CDbCfg_database(lua_State* tolua_S)
{
  CDbCfg* self = (CDbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'database'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->database);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: database of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_set_CDbCfg_database
static int tolua_set_CDbCfg_database(lua_State* tolua_S)
{
  CDbCfg* self = (CDbCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: port of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_get_CDbCfg_port
static int tolua_get_CDbCfg_port(lua_State* tolua_S)
{
  CDbCfg* self = (CDbCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'port'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->port);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: port of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_set_CDbCfg_port
static int tolua_set_CDbCfg_port(lua_State* tolua_S)
{
  CDbCfg* self = (CDbCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_Config_CDbCfg_new00
static int tolua_Config_CDbCfg_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CDbCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CDbCfg* tolua_ret = (CDbCfg*)  Mtolua_new((CDbCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CDbCfg");
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

/* method: new_local of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_Config_CDbCfg_new00_local
static int tolua_Config_CDbCfg_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CDbCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CDbCfg* tolua_ret = (CDbCfg*)  Mtolua_new((CDbCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CDbCfg");
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

/* method: delete of class  CDbCfg */
#ifndef TOLUA_DISABLE_tolua_Config_CDbCfg_delete00
static int tolua_Config_CDbCfg_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CDbCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CDbCfg* self = (CDbCfg*)  tolua_tousertype(tolua_S,1,0);
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
   tolua_function(tolua_S,"CoutLog",tolua_Config_ConfigMgr_CoutLog00);
   tolua_function(tolua_S,"GetCBaseCfgMgr",tolua_Config_ConfigMgr_GetCBaseCfgMgr00);
   tolua_function(tolua_S,"ReadServerCfg",tolua_Config_ConfigMgr_ReadServerCfg00);
   tolua_function(tolua_S,"ReadDbCfg",tolua_Config_ConfigMgr_ReadDbCfg00);
   tolua_function(tolua_S,"ReadThreadCntCfg",tolua_Config_ConfigMgr_ReadThreadCntCfg00);
   tolua_function(tolua_S,"ReadLogPrintTmCfg",tolua_Config_ConfigMgr_ReadLogPrintTmCfg00);
   tolua_function(tolua_S,"ReadMaxSocketCntCfg",tolua_Config_ConfigMgr_ReadMaxSocketCntCfg00);
   tolua_function(tolua_S,"ReadExitCfg",tolua_Config_ConfigMgr_ReadExitCfg00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"LOG_INFO",LOG_INFO);
  tolua_constant(tolua_S,"LOG_WARN",LOG_WARN);
  tolua_constant(tolua_S,"LOG_ERROR",LOG_ERROR);
  tolua_constant(tolua_S,"LOG_CINFO",LOG_CINFO);
  tolua_constant(tolua_S,"LOG_CERROR",LOG_CERROR);
  tolua_constant(tolua_S,"LOG_ERROR_SYS",LOG_ERROR_SYS);
  tolua_constant(tolua_S,"LOG_END",LOG_END);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CServerCfg","CServerCfg","",tolua_collect_CServerCfg);
  #else
  tolua_cclass(tolua_S,"CServerCfg","CServerCfg","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CServerCfg");
   tolua_variable(tolua_S,"ip",tolua_get_CServerCfg_ip,tolua_set_CServerCfg_ip);
   tolua_variable(tolua_S,"port",tolua_get_CServerCfg_port,tolua_set_CServerCfg_port);
   tolua_function(tolua_S,"new",tolua_Config_CServerCfg_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_CServerCfg_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_CServerCfg_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_CServerCfg_delete00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CDbCfg","CDbCfg","",tolua_collect_CDbCfg);
  #else
  tolua_cclass(tolua_S,"CDbCfg","CDbCfg","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CDbCfg");
   tolua_variable(tolua_S,"ip",tolua_get_CDbCfg_ip,tolua_set_CDbCfg_ip);
   tolua_variable(tolua_S,"user",tolua_get_CDbCfg_user,tolua_set_CDbCfg_user);
   tolua_variable(tolua_S,"passwd",tolua_get_CDbCfg_passwd,tolua_set_CDbCfg_passwd);
   tolua_variable(tolua_S,"database",tolua_get_CDbCfg_database,tolua_set_CDbCfg_database);
   tolua_variable(tolua_S,"port",tolua_get_CDbCfg_port,tolua_set_CDbCfg_port);
   tolua_function(tolua_S,"new",tolua_Config_CDbCfg_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_CDbCfg_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_CDbCfg_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_CDbCfg_delete00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Config (lua_State* tolua_S) {
 return tolua_Config_open(tolua_S);
};
#endif

