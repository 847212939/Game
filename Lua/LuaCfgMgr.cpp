/*
** Lua binding: Config
** Generated automatically by tolua++-1.0.92 on Wed Aug 10 15:36:19 2022.
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

static int tolua_collect_ActivityList (lua_State* tolua_S)
{
 ActivityList* self = (ActivityList*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_DbCfg (lua_State* tolua_S)
{
 DbCfg* self = (DbCfg*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

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

static int tolua_collect_BrushMonsterCfg (lua_State* tolua_S)
{
 BrushMonsterCfg* self = (BrushMonsterCfg*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_ActivityBreakdown (lua_State* tolua_S)
{
 ActivityBreakdown* self = (ActivityBreakdown*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"ActivityList");
 tolua_usertype(tolua_S,"DbCfg");
 tolua_usertype(tolua_S,"ConfigMgr");
 tolua_usertype(tolua_S,"LogicCfg");
 tolua_usertype(tolua_S,"BrushMonsterCfg");
 tolua_usertype(tolua_S,"ActivityBreakdown");
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

/* method: ReadServerIDCfg of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadServerIDCfg00
static int tolua_Config_ConfigMgr_ReadServerIDCfg00(lua_State* tolua_S)
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
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadServerIDCfg'", NULL);
#endif
  {
   self->ReadServerIDCfg(id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadServerIDCfg'.",&tolua_err);
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

/* method: ReadActivityList of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadActivityList00
static int tolua_Config_ConfigMgr_ReadActivityList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ActivityList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  ActivityList* config = ((ActivityList*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadActivityList'", NULL);
#endif
  {
   self->ReadActivityList(config);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadActivityList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadBrushMonsterCfg of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadBrushMonsterCfg00
static int tolua_Config_ConfigMgr_ReadBrushMonsterCfg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"BrushMonsterCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  BrushMonsterCfg* config = ((BrushMonsterCfg*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadBrushMonsterCfg'", NULL);
#endif
  {
   self->ReadBrushMonsterCfg(config);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadBrushMonsterCfg'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadActivityBreakdownCfg of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadActivityBreakdownCfg00
static int tolua_Config_ConfigMgr_ReadActivityBreakdownCfg00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ActivityBreakdown",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  ActivityBreakdown* config = ((ActivityBreakdown*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadActivityBreakdownCfg'", NULL);
#endif
  {
   self->ReadActivityBreakdownCfg(config);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadActivityBreakdownCfg'.",&tolua_err);
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

/* get function: id of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_get_ActivityList_id
static int tolua_get_ActivityList_id(lua_State* tolua_S)
{
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: id of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_set_ActivityList_id
static int tolua_set_ActivityList_id(lua_State* tolua_S)
{
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->id = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: type of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_get_ActivityList_type
static int tolua_get_ActivityList_type(lua_State* tolua_S)
{
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: type of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_set_ActivityList_type
static int tolua_set_ActivityList_type(lua_State* tolua_S)
{
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->type = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: sid of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_get_ActivityList_sid
static int tolua_get_ActivityList_sid(lua_State* tolua_S)
{
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sid'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->sid);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: sid of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_set_ActivityList_sid
static int tolua_set_ActivityList_sid(lua_State* tolua_S)
{
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sid'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sid = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: activityBreakdown of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_get_ActivityList_activityBreakdown
static int tolua_get_ActivityList_activityBreakdown(lua_State* tolua_S)
{
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'activityBreakdown'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->activityBreakdown);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: activityBreakdown of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_set_ActivityList_activityBreakdown
static int tolua_set_ActivityList_activityBreakdown(lua_State* tolua_S)
{
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'activityBreakdown'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->activityBreakdown = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: breakdown of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_get_ActivityList_breakdown
static int tolua_get_ActivityList_breakdown(lua_State* tolua_S)
{
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'breakdown'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->breakdown);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: breakdown of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_set_ActivityList_breakdown
static int tolua_set_ActivityList_breakdown(lua_State* tolua_S)
{
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'breakdown'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->breakdown = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_ActivityList_new00
static int tolua_Config_ActivityList_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ActivityList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ActivityList* tolua_ret = (ActivityList*)  Mtolua_new((ActivityList)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ActivityList");
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

/* method: new_local of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_ActivityList_new00_local
static int tolua_Config_ActivityList_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ActivityList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ActivityList* tolua_ret = (ActivityList*)  Mtolua_new((ActivityList)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ActivityList");
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

/* method: delete of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_ActivityList_delete00
static int tolua_Config_ActivityList_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ActivityList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
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

/* method: AddBeginTime of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_ActivityList_AddBeginTime00
static int tolua_Config_ActivityList_AddBeginTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ActivityList",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
  int tm = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddBeginTime'", NULL);
#endif
  {
   self->AddBeginTime(tm);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddBeginTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddEndTime of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_ActivityList_AddEndTime00
static int tolua_Config_ActivityList_AddEndTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ActivityList",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
  int tm = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddEndTime'", NULL);
#endif
  {
   self->AddEndTime(tm);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddEndTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddSolidTime of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_ActivityList_AddSolidTime00
static int tolua_Config_ActivityList_AddSolidTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ActivityList",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
  int tm = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddSolidTime'", NULL);
#endif
  {
   self->AddSolidTime(tm);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddSolidTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddOpenServerTime of class  ActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_ActivityList_AddOpenServerTime00
static int tolua_Config_ActivityList_AddOpenServerTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ActivityList",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ActivityList* self = (ActivityList*)  tolua_tousertype(tolua_S,1,0);
  int tm = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddOpenServerTime'", NULL);
#endif
  {
   self->AddOpenServerTime(tm);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddOpenServerTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: id of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_BrushMonsterCfg_id
static int tolua_get_BrushMonsterCfg_id(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: id of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_BrushMonsterCfg_id
static int tolua_set_BrushMonsterCfg_id(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->id = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: sid of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_BrushMonsterCfg_sid
static int tolua_get_BrushMonsterCfg_sid(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sid'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->sid);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: sid of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_BrushMonsterCfg_sid
static int tolua_set_BrushMonsterCfg_sid(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sid'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->sid = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: mid of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_BrushMonsterCfg_mid
static int tolua_get_BrushMonsterCfg_mid(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mid'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->mid);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mid of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_BrushMonsterCfg_mid
static int tolua_set_BrushMonsterCfg_mid(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mid'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->mid = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: count of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_BrushMonsterCfg_count
static int tolua_get_BrushMonsterCfg_count(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'count'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->count);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: count of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_BrushMonsterCfg_count
static int tolua_set_BrushMonsterCfg_count(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'count'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->count = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: delayTime of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_BrushMonsterCfg_delayTime
static int tolua_get_BrushMonsterCfg_delayTime(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'delayTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->delayTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: delayTime of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_BrushMonsterCfg_delayTime
static int tolua_set_BrushMonsterCfg_delayTime(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'delayTime'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->delayTime = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: refreshTime of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_BrushMonsterCfg_refreshTime
static int tolua_get_BrushMonsterCfg_refreshTime(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'refreshTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->refreshTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: refreshTime of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_BrushMonsterCfg_refreshTime
static int tolua_set_BrushMonsterCfg_refreshTime(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'refreshTime'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->refreshTime = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_BrushMonsterCfg_x
static int tolua_get_BrushMonsterCfg_x(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_BrushMonsterCfg_x
static int tolua_set_BrushMonsterCfg_x(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_BrushMonsterCfg_y
static int tolua_get_BrushMonsterCfg_y(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_BrushMonsterCfg_y
static int tolua_set_BrushMonsterCfg_y(lua_State* tolua_S)
{
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_Config_BrushMonsterCfg_new00
static int tolua_Config_BrushMonsterCfg_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"BrushMonsterCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   BrushMonsterCfg* tolua_ret = (BrushMonsterCfg*)  Mtolua_new((BrushMonsterCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"BrushMonsterCfg");
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

/* method: new_local of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_Config_BrushMonsterCfg_new00_local
static int tolua_Config_BrushMonsterCfg_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"BrushMonsterCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   BrushMonsterCfg* tolua_ret = (BrushMonsterCfg*)  Mtolua_new((BrushMonsterCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"BrushMonsterCfg");
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

/* method: delete of class  BrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_Config_BrushMonsterCfg_delete00
static int tolua_Config_BrushMonsterCfg_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"BrushMonsterCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  BrushMonsterCfg* self = (BrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: id of class  ActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_get_ActivityBreakdown_id
static int tolua_get_ActivityBreakdown_id(lua_State* tolua_S)
{
  ActivityBreakdown* self = (ActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: id of class  ActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_set_ActivityBreakdown_id
static int tolua_set_ActivityBreakdown_id(lua_State* tolua_S)
{
  ActivityBreakdown* self = (ActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->id = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: brushMonsterCfg of class  ActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_get_ActivityBreakdown_brushMonsterCfg
static int tolua_get_ActivityBreakdown_brushMonsterCfg(lua_State* tolua_S)
{
  ActivityBreakdown* self = (ActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'brushMonsterCfg'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->brushMonsterCfg);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: brushMonsterCfg of class  ActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_set_ActivityBreakdown_brushMonsterCfg
static int tolua_set_ActivityBreakdown_brushMonsterCfg(lua_State* tolua_S)
{
  ActivityBreakdown* self = (ActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'brushMonsterCfg'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->brushMonsterCfg = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  ActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_Config_ActivityBreakdown_new00
static int tolua_Config_ActivityBreakdown_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ActivityBreakdown",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ActivityBreakdown* tolua_ret = (ActivityBreakdown*)  Mtolua_new((ActivityBreakdown)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ActivityBreakdown");
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

/* method: new_local of class  ActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_Config_ActivityBreakdown_new00_local
static int tolua_Config_ActivityBreakdown_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ActivityBreakdown",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ActivityBreakdown* tolua_ret = (ActivityBreakdown*)  Mtolua_new((ActivityBreakdown)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ActivityBreakdown");
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

/* method: delete of class  ActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_Config_ActivityBreakdown_delete00
static int tolua_Config_ActivityBreakdown_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ActivityBreakdown",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ActivityBreakdown* self = (ActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
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

/* method: AddBeginTime of class  ActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_Config_ActivityBreakdown_AddBeginTime00
static int tolua_Config_ActivityBreakdown_AddBeginTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ActivityBreakdown",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ActivityBreakdown* self = (ActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
  int tm = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddBeginTime'", NULL);
#endif
  {
   self->AddBeginTime(tm);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddBeginTime'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddEndTime of class  ActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_Config_ActivityBreakdown_AddEndTime00
static int tolua_Config_ActivityBreakdown_AddEndTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ActivityBreakdown",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ActivityBreakdown* self = (ActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
  int tm = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddEndTime'", NULL);
#endif
  {
   self->AddEndTime(tm);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddEndTime'.",&tolua_err);
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
  tolua_constant(tolua_S,"LOG_INFO",LOG_INFO);
  tolua_constant(tolua_S,"LOG_WARN",LOG_WARN);
  tolua_constant(tolua_S,"LOG_ERROR",LOG_ERROR);
  tolua_constant(tolua_S,"LOG_CINFO",LOG_CINFO);
  tolua_constant(tolua_S,"LOG_CERROR",LOG_CERROR);
  tolua_constant(tolua_S,"LOG_ERROR_SYS",LOG_ERROR_SYS);
  tolua_constant(tolua_S,"LOG_END",LOG_END);
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
   tolua_function(tolua_S,"ReadLogicCfg",tolua_Config_ConfigMgr_ReadLogicCfg00);
   tolua_function(tolua_S,"ReadDbCfg",tolua_Config_ConfigMgr_ReadDbCfg00);
   tolua_function(tolua_S,"ReadThreadCntCfg",tolua_Config_ConfigMgr_ReadThreadCntCfg00);
   tolua_function(tolua_S,"ReadServerIDCfg",tolua_Config_ConfigMgr_ReadServerIDCfg00);
   tolua_function(tolua_S,"ReadLogPrintTmCfg",tolua_Config_ConfigMgr_ReadLogPrintTmCfg00);
   tolua_function(tolua_S,"ReadMaxSocketCntCfg",tolua_Config_ConfigMgr_ReadMaxSocketCntCfg00);
   tolua_function(tolua_S,"ReadExitCfg",tolua_Config_ConfigMgr_ReadExitCfg00);
   tolua_function(tolua_S,"ReadTimerCntCfg",tolua_Config_ConfigMgr_ReadTimerCntCfg00);
   tolua_function(tolua_S,"ReadKeyCfg",tolua_Config_ConfigMgr_ReadKeyCfg00);
   tolua_function(tolua_S,"ReadActivityList",tolua_Config_ConfigMgr_ReadActivityList00);
   tolua_function(tolua_S,"ReadBrushMonsterCfg",tolua_Config_ConfigMgr_ReadBrushMonsterCfg00);
   tolua_function(tolua_S,"ReadActivityBreakdownCfg",tolua_Config_ConfigMgr_ReadActivityBreakdownCfg00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"LogicCfg","LogicCfg","",tolua_collect_LogicCfg);
  #else
  tolua_cclass(tolua_S,"LogicCfg","LogicCfg","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"LogicCfg");
   tolua_variable(tolua_S,"ip",tolua_get_LogicCfg_ip,tolua_set_LogicCfg_ip);
   tolua_variable(tolua_S,"port",tolua_get_LogicCfg_port,tolua_set_LogicCfg_port);
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
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"ActivityList","ActivityList","",tolua_collect_ActivityList);
  #else
  tolua_cclass(tolua_S,"ActivityList","ActivityList","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"ActivityList");
   tolua_variable(tolua_S,"id",tolua_get_ActivityList_id,tolua_set_ActivityList_id);
   tolua_variable(tolua_S,"type",tolua_get_ActivityList_type,tolua_set_ActivityList_type);
   tolua_variable(tolua_S,"sid",tolua_get_ActivityList_sid,tolua_set_ActivityList_sid);
   tolua_variable(tolua_S,"activityBreakdown",tolua_get_ActivityList_activityBreakdown,tolua_set_ActivityList_activityBreakdown);
   tolua_variable(tolua_S,"breakdown",tolua_get_ActivityList_breakdown,tolua_set_ActivityList_breakdown);
   tolua_function(tolua_S,"new",tolua_Config_ActivityList_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_ActivityList_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_ActivityList_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_ActivityList_delete00);
   tolua_function(tolua_S,"AddBeginTime",tolua_Config_ActivityList_AddBeginTime00);
   tolua_function(tolua_S,"AddEndTime",tolua_Config_ActivityList_AddEndTime00);
   tolua_function(tolua_S,"AddSolidTime",tolua_Config_ActivityList_AddSolidTime00);
   tolua_function(tolua_S,"AddOpenServerTime",tolua_Config_ActivityList_AddOpenServerTime00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"BrushMonsterCfg","BrushMonsterCfg","",tolua_collect_BrushMonsterCfg);
  #else
  tolua_cclass(tolua_S,"BrushMonsterCfg","BrushMonsterCfg","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"BrushMonsterCfg");
   tolua_variable(tolua_S,"id",tolua_get_BrushMonsterCfg_id,tolua_set_BrushMonsterCfg_id);
   tolua_variable(tolua_S,"sid",tolua_get_BrushMonsterCfg_sid,tolua_set_BrushMonsterCfg_sid);
   tolua_variable(tolua_S,"mid",tolua_get_BrushMonsterCfg_mid,tolua_set_BrushMonsterCfg_mid);
   tolua_variable(tolua_S,"count",tolua_get_BrushMonsterCfg_count,tolua_set_BrushMonsterCfg_count);
   tolua_variable(tolua_S,"delayTime",tolua_get_BrushMonsterCfg_delayTime,tolua_set_BrushMonsterCfg_delayTime);
   tolua_variable(tolua_S,"refreshTime",tolua_get_BrushMonsterCfg_refreshTime,tolua_set_BrushMonsterCfg_refreshTime);
   tolua_variable(tolua_S,"x",tolua_get_BrushMonsterCfg_x,tolua_set_BrushMonsterCfg_x);
   tolua_variable(tolua_S,"y",tolua_get_BrushMonsterCfg_y,tolua_set_BrushMonsterCfg_y);
   tolua_function(tolua_S,"new",tolua_Config_BrushMonsterCfg_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_BrushMonsterCfg_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_BrushMonsterCfg_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_BrushMonsterCfg_delete00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"ActivityBreakdown","ActivityBreakdown","",tolua_collect_ActivityBreakdown);
  #else
  tolua_cclass(tolua_S,"ActivityBreakdown","ActivityBreakdown","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"ActivityBreakdown");
   tolua_variable(tolua_S,"id",tolua_get_ActivityBreakdown_id,tolua_set_ActivityBreakdown_id);
   tolua_variable(tolua_S,"brushMonsterCfg",tolua_get_ActivityBreakdown_brushMonsterCfg,tolua_set_ActivityBreakdown_brushMonsterCfg);
   tolua_function(tolua_S,"new",tolua_Config_ActivityBreakdown_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_ActivityBreakdown_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_ActivityBreakdown_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_ActivityBreakdown_delete00);
   tolua_function(tolua_S,"AddBeginTime",tolua_Config_ActivityBreakdown_AddBeginTime00);
   tolua_function(tolua_S,"AddEndTime",tolua_Config_ActivityBreakdown_AddEndTime00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Config (lua_State* tolua_S) {
 return tolua_Config_open(tolua_S);
};
#endif

