/*
** Lua binding: Config
** Generated automatically by tolua++-1.0.92 on Wed Aug 17 15:31:13 2022.
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

static int tolua_collect_CSkillIdList (lua_State* tolua_S)
{
 CSkillIdList* self = (CSkillIdList*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CActivityList (lua_State* tolua_S)
{
 CActivityList* self = (CActivityList*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CActivityBreakdown (lua_State* tolua_S)
{
 CActivityBreakdown* self = (CActivityBreakdown*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CBrushMonsterCfg (lua_State* tolua_S)
{
 CBrushMonsterCfg* self = (CBrushMonsterCfg*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CDbCfg (lua_State* tolua_S)
{
 CDbCfg* self = (CDbCfg*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CLogicCfg (lua_State* tolua_S)
{
 CLogicCfg* self = (CLogicCfg*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_CHeroList (lua_State* tolua_S)
{
 CHeroList* self = (CHeroList*) tolua_tousertype(tolua_S,1,0);
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
 tolua_usertype(tolua_S,"CActivityBreakdown");
 tolua_usertype(tolua_S,"CBrushMonsterCfg");
 tolua_usertype(tolua_S,"CSkillIdList");
 tolua_usertype(tolua_S,"CLogicCfg");
 tolua_usertype(tolua_S,"AttrsMap");
 tolua_usertype(tolua_S,"CHeroList");
 tolua_usertype(tolua_S,"CDbCfg");
 tolua_usertype(tolua_S,"ConfigMgr");
 tolua_usertype(tolua_S,"CActivityList");
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

/* method: ReadOpenServerTime of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadOpenServerTime00
static int tolua_Config_ConfigMgr_ReadOpenServerTime00(lua_State* tolua_S)
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
  std::string servertm = ((std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadOpenServerTime'", NULL);
#endif
  {
   self->ReadOpenServerTime(servertm);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadOpenServerTime'.",&tolua_err);
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
     !tolua_isusertype(tolua_S,2,"CActivityList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  CActivityList* config = ((CActivityList*)  tolua_tousertype(tolua_S,2,0));
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
     !tolua_isusertype(tolua_S,2,"CBrushMonsterCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  CBrushMonsterCfg* config = ((CBrushMonsterCfg*)  tolua_tousertype(tolua_S,2,0));
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
     !tolua_isusertype(tolua_S,2,"CActivityBreakdown",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  CActivityBreakdown* config = ((CActivityBreakdown*)  tolua_tousertype(tolua_S,2,0));
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

/* method: ReadHeroList of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadHeroList00
static int tolua_Config_ConfigMgr_ReadHeroList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CHeroList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  CHeroList* config = ((CHeroList*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadHeroList'", NULL);
#endif
  {
   self->ReadHeroList(config);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadHeroList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: ReadSkillIdList of class  ConfigMgr */
#ifndef TOLUA_DISABLE_tolua_Config_ConfigMgr_ReadSkillIdList00
static int tolua_Config_ConfigMgr_ReadSkillIdList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ConfigMgr",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CSkillIdList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ConfigMgr* self = (ConfigMgr*)  tolua_tousertype(tolua_S,1,0);
  CSkillIdList* config = ((CSkillIdList*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'ReadSkillIdList'", NULL);
#endif
  {
   self->ReadSkillIdList(config);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ReadSkillIdList'.",&tolua_err);
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

/* get function: id of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_get_CActivityList_id
static int tolua_get_CActivityList_id(lua_State* tolua_S)
{
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: id of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_set_CActivityList_id
static int tolua_set_CActivityList_id(lua_State* tolua_S)
{
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: type of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_get_CActivityList_type
static int tolua_get_CActivityList_type(lua_State* tolua_S)
{
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'type'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->type);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: type of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_set_CActivityList_type
static int tolua_set_CActivityList_type(lua_State* tolua_S)
{
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: sid of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_get_CActivityList_sid
static int tolua_get_CActivityList_sid(lua_State* tolua_S)
{
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sid'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->sid);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: sid of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_set_CActivityList_sid
static int tolua_set_CActivityList_sid(lua_State* tolua_S)
{
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: activityBreakdown of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_get_CActivityList_activityBreakdown
static int tolua_get_CActivityList_activityBreakdown(lua_State* tolua_S)
{
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'activityBreakdown'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->activityBreakdown);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: activityBreakdown of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_set_CActivityList_activityBreakdown
static int tolua_set_CActivityList_activityBreakdown(lua_State* tolua_S)
{
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_CActivityList_new00
static int tolua_Config_CActivityList_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CActivityList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CActivityList* tolua_ret = (CActivityList*)  Mtolua_new((CActivityList)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CActivityList");
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

/* method: new_local of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_CActivityList_new00_local
static int tolua_Config_CActivityList_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CActivityList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CActivityList* tolua_ret = (CActivityList*)  Mtolua_new((CActivityList)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CActivityList");
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

/* method: delete of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_CActivityList_delete00
static int tolua_Config_CActivityList_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CActivityList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
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

/* method: AddBeginTime of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_CActivityList_AddBeginTime00
static int tolua_Config_CActivityList_AddBeginTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CActivityList",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
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

/* method: AddEndTime of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_CActivityList_AddEndTime00
static int tolua_Config_CActivityList_AddEndTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CActivityList",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
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

/* method: AddSolidTime of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_CActivityList_AddSolidTime00
static int tolua_Config_CActivityList_AddSolidTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CActivityList",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
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

/* method: AddOpenServerTime of class  CActivityList */
#ifndef TOLUA_DISABLE_tolua_Config_CActivityList_AddOpenServerTime00
static int tolua_Config_CActivityList_AddOpenServerTime00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CActivityList",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CActivityList* self = (CActivityList*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: id of class  CActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_get_CActivityBreakdown_id
static int tolua_get_CActivityBreakdown_id(lua_State* tolua_S)
{
  CActivityBreakdown* self = (CActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: id of class  CActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_set_CActivityBreakdown_id
static int tolua_set_CActivityBreakdown_id(lua_State* tolua_S)
{
  CActivityBreakdown* self = (CActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: dayBreakdown of class  CActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_get_CActivityBreakdown_dayBreakdown
static int tolua_get_CActivityBreakdown_dayBreakdown(lua_State* tolua_S)
{
  CActivityBreakdown* self = (CActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dayBreakdown'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->dayBreakdown);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: dayBreakdown of class  CActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_set_CActivityBreakdown_dayBreakdown
static int tolua_set_CActivityBreakdown_dayBreakdown(lua_State* tolua_S)
{
  CActivityBreakdown* self = (CActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'dayBreakdown'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->dayBreakdown = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: hourBreakdown of class  CActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_get_CActivityBreakdown_hourBreakdown
static int tolua_get_CActivityBreakdown_hourBreakdown(lua_State* tolua_S)
{
  CActivityBreakdown* self = (CActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hourBreakdown'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->hourBreakdown);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: hourBreakdown of class  CActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_set_CActivityBreakdown_hourBreakdown
static int tolua_set_CActivityBreakdown_hourBreakdown(lua_State* tolua_S)
{
  CActivityBreakdown* self = (CActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'hourBreakdown'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->hourBreakdown = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_Config_CActivityBreakdown_new00
static int tolua_Config_CActivityBreakdown_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CActivityBreakdown",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CActivityBreakdown* tolua_ret = (CActivityBreakdown*)  Mtolua_new((CActivityBreakdown)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CActivityBreakdown");
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

/* method: new_local of class  CActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_Config_CActivityBreakdown_new00_local
static int tolua_Config_CActivityBreakdown_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CActivityBreakdown",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CActivityBreakdown* tolua_ret = (CActivityBreakdown*)  Mtolua_new((CActivityBreakdown)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CActivityBreakdown");
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

/* method: delete of class  CActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_Config_CActivityBreakdown_delete00
static int tolua_Config_CActivityBreakdown_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CActivityBreakdown",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CActivityBreakdown* self = (CActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
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

/* method: AddDayBreakdownList of class  CActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_Config_CActivityBreakdown_AddDayBreakdownList00
static int tolua_Config_CActivityBreakdown_AddDayBreakdownList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CActivityBreakdown",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CActivityBreakdown* self = (CActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
  int idex = ((int)  tolua_tonumber(tolua_S,2,0));
  int tm = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddDayBreakdownList'", NULL);
#endif
  {
   self->AddDayBreakdownList(idex,tm);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddDayBreakdownList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddHourBreakdownList of class  CActivityBreakdown */
#ifndef TOLUA_DISABLE_tolua_Config_CActivityBreakdown_AddHourBreakdownList00
static int tolua_Config_CActivityBreakdown_AddHourBreakdownList00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CActivityBreakdown",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CActivityBreakdown* self = (CActivityBreakdown*)  tolua_tousertype(tolua_S,1,0);
  int idex = ((int)  tolua_tonumber(tolua_S,2,0));
  int tm = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddHourBreakdownList'", NULL);
#endif
  {
   self->AddHourBreakdownList(idex,tm);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddHourBreakdownList'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: id of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_CBrushMonsterCfg_id
static int tolua_get_CBrushMonsterCfg_id(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'id'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->id);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: id of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_CBrushMonsterCfg_id
static int tolua_set_CBrushMonsterCfg_id(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: sid of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_CBrushMonsterCfg_sid
static int tolua_get_CBrushMonsterCfg_sid(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'sid'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->sid);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: sid of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_CBrushMonsterCfg_sid
static int tolua_set_CBrushMonsterCfg_sid(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: mid of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_CBrushMonsterCfg_mid
static int tolua_get_CBrushMonsterCfg_mid(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'mid'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->mid);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: mid of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_CBrushMonsterCfg_mid
static int tolua_set_CBrushMonsterCfg_mid(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: count of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_CBrushMonsterCfg_count
static int tolua_get_CBrushMonsterCfg_count(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'count'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->count);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: count of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_CBrushMonsterCfg_count
static int tolua_set_CBrushMonsterCfg_count(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: delayTime of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_CBrushMonsterCfg_delayTime
static int tolua_get_CBrushMonsterCfg_delayTime(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'delayTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->delayTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: delayTime of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_CBrushMonsterCfg_delayTime
static int tolua_set_CBrushMonsterCfg_delayTime(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: refreshTime of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_CBrushMonsterCfg_refreshTime
static int tolua_get_CBrushMonsterCfg_refreshTime(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'refreshTime'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->refreshTime);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: refreshTime of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_CBrushMonsterCfg_refreshTime
static int tolua_set_CBrushMonsterCfg_refreshTime(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: x of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_CBrushMonsterCfg_x
static int tolua_get_CBrushMonsterCfg_x(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_CBrushMonsterCfg_x
static int tolua_set_CBrushMonsterCfg_x(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: y of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_get_CBrushMonsterCfg_y
static int tolua_get_CBrushMonsterCfg_y(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_set_CBrushMonsterCfg_y
static int tolua_set_CBrushMonsterCfg_y(lua_State* tolua_S)
{
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_Config_CBrushMonsterCfg_new00
static int tolua_Config_CBrushMonsterCfg_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CBrushMonsterCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CBrushMonsterCfg* tolua_ret = (CBrushMonsterCfg*)  Mtolua_new((CBrushMonsterCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CBrushMonsterCfg");
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

/* method: new_local of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_Config_CBrushMonsterCfg_new00_local
static int tolua_Config_CBrushMonsterCfg_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CBrushMonsterCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CBrushMonsterCfg* tolua_ret = (CBrushMonsterCfg*)  Mtolua_new((CBrushMonsterCfg)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CBrushMonsterCfg");
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

/* method: delete of class  CBrushMonsterCfg */
#ifndef TOLUA_DISABLE_tolua_Config_CBrushMonsterCfg_delete00
static int tolua_Config_CBrushMonsterCfg_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CBrushMonsterCfg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CBrushMonsterCfg* self = (CBrushMonsterCfg*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: heroId of class  CHeroList */
#ifndef TOLUA_DISABLE_tolua_get_CHeroList_heroId
static int tolua_get_CHeroList_heroId(lua_State* tolua_S)
{
  CHeroList* self = (CHeroList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'heroId'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->heroId);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: heroId of class  CHeroList */
#ifndef TOLUA_DISABLE_tolua_set_CHeroList_heroId
static int tolua_set_CHeroList_heroId(lua_State* tolua_S)
{
  CHeroList* self = (CHeroList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'heroId'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->heroId = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: heroType of class  CHeroList */
#ifndef TOLUA_DISABLE_tolua_get_CHeroList_heroType
static int tolua_get_CHeroList_heroType(lua_State* tolua_S)
{
  CHeroList* self = (CHeroList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'heroType'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->heroType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: heroType of class  CHeroList */
#ifndef TOLUA_DISABLE_tolua_set_CHeroList_heroType
static int tolua_set_CHeroList_heroType(lua_State* tolua_S)
{
  CHeroList* self = (CHeroList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'heroType'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->heroType = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: heroName of class  CHeroList */
#ifndef TOLUA_DISABLE_tolua_get_CHeroList_heroName
static int tolua_get_CHeroList_heroName(lua_State* tolua_S)
{
  CHeroList* self = (CHeroList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'heroName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->heroName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: heroName of class  CHeroList */
#ifndef TOLUA_DISABLE_tolua_set_CHeroList_heroName
static int tolua_set_CHeroList_heroName(lua_State* tolua_S)
{
  CHeroList* self = (CHeroList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'heroName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->heroName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddskillId of class  CHeroList */
#ifndef TOLUA_DISABLE_tolua_Config_CHeroList_AddskillId00
static int tolua_Config_CHeroList_AddskillId00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CHeroList",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CHeroList* self = (CHeroList*)  tolua_tousertype(tolua_S,1,0);
  int skillid = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddskillId'", NULL);
#endif
  {
   self->AddskillId(skillid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddskillId'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CHeroList */
#ifndef TOLUA_DISABLE_tolua_Config_CHeroList_new00
static int tolua_Config_CHeroList_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CHeroList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CHeroList* tolua_ret = (CHeroList*)  Mtolua_new((CHeroList)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CHeroList");
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

/* method: new_local of class  CHeroList */
#ifndef TOLUA_DISABLE_tolua_Config_CHeroList_new00_local
static int tolua_Config_CHeroList_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CHeroList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CHeroList* tolua_ret = (CHeroList*)  Mtolua_new((CHeroList)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CHeroList");
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

/* method: delete of class  CHeroList */
#ifndef TOLUA_DISABLE_tolua_Config_CHeroList_delete00
static int tolua_Config_CHeroList_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CHeroList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CHeroList* self = (CHeroList*)  tolua_tousertype(tolua_S,1,0);
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

/* get function: skillId of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_get_CSkillIdList_skillId
static int tolua_get_CSkillIdList_skillId(lua_State* tolua_S)
{
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skillId'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->skillId);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: skillId of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_set_CSkillIdList_skillId
static int tolua_set_CSkillIdList_skillId(lua_State* tolua_S)
{
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skillId'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->skillId = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: skillType of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_get_CSkillIdList_skillType
static int tolua_get_CSkillIdList_skillType(lua_State* tolua_S)
{
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skillType'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->skillType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: skillType of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_set_CSkillIdList_skillType
static int tolua_set_CSkillIdList_skillType(lua_State* tolua_S)
{
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skillType'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->skillType = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: skillCd of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_get_CSkillIdList_skillCd
static int tolua_get_CSkillIdList_skillCd(lua_State* tolua_S)
{
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skillCd'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->skillCd);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: skillCd of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_set_CSkillIdList_skillCd
static int tolua_set_CSkillIdList_skillCd(lua_State* tolua_S)
{
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skillCd'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->skillCd = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: skillEffect of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_get_CSkillIdList_skillEffect
static int tolua_get_CSkillIdList_skillEffect(lua_State* tolua_S)
{
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skillEffect'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->skillEffect);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: skillEffect of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_set_CSkillIdList_skillEffect
static int tolua_set_CSkillIdList_skillEffect(lua_State* tolua_S)
{
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skillEffect'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->skillEffect = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: skillEffectCd of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_get_CSkillIdList_skillEffectCd
static int tolua_get_CSkillIdList_skillEffectCd(lua_State* tolua_S)
{
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skillEffectCd'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->skillEffectCd);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: skillEffectCd of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_set_CSkillIdList_skillEffectCd
static int tolua_set_CSkillIdList_skillEffectCd(lua_State* tolua_S)
{
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'skillEffectCd'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->skillEffectCd = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: attrs of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_get_CSkillIdList_attrs
static int tolua_get_CSkillIdList_attrs(lua_State* tolua_S)
{
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attrs'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->attrs,"AttrsMap");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: attrs of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_set_CSkillIdList_attrs
static int tolua_set_CSkillIdList_attrs(lua_State* tolua_S)
{
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'attrs'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"AttrsMap",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->attrs = *((AttrsMap*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddAttrs of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_Config_CSkillIdList_AddAttrs00
static int tolua_Config_CSkillIdList_AddAttrs00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CSkillIdList",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
  int id = ((int)  tolua_tonumber(tolua_S,2,0));
  int value = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddAttrs'", NULL);
#endif
  {
   self->AddAttrs(id,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddAttrs'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_Config_CSkillIdList_new00
static int tolua_Config_CSkillIdList_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CSkillIdList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CSkillIdList* tolua_ret = (CSkillIdList*)  Mtolua_new((CSkillIdList)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CSkillIdList");
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

/* method: new_local of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_Config_CSkillIdList_new00_local
static int tolua_Config_CSkillIdList_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"CSkillIdList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   CSkillIdList* tolua_ret = (CSkillIdList*)  Mtolua_new((CSkillIdList)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"CSkillIdList");
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

/* method: delete of class  CSkillIdList */
#ifndef TOLUA_DISABLE_tolua_Config_CSkillIdList_delete00
static int tolua_Config_CSkillIdList_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"CSkillIdList",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  CSkillIdList* self = (CSkillIdList*)  tolua_tousertype(tolua_S,1,0);
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
   tolua_function(tolua_S,"ReadOpenServerTime",tolua_Config_ConfigMgr_ReadOpenServerTime00);
   tolua_function(tolua_S,"ReadActivityList",tolua_Config_ConfigMgr_ReadActivityList00);
   tolua_function(tolua_S,"ReadBrushMonsterCfg",tolua_Config_ConfigMgr_ReadBrushMonsterCfg00);
   tolua_function(tolua_S,"ReadActivityBreakdownCfg",tolua_Config_ConfigMgr_ReadActivityBreakdownCfg00);
   tolua_function(tolua_S,"ReadHeroList",tolua_Config_ConfigMgr_ReadHeroList00);
   tolua_function(tolua_S,"ReadSkillIdList",tolua_Config_ConfigMgr_ReadSkillIdList00);
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
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CActivityList","CActivityList","",tolua_collect_CActivityList);
  #else
  tolua_cclass(tolua_S,"CActivityList","CActivityList","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CActivityList");
   tolua_variable(tolua_S,"id",tolua_get_CActivityList_id,tolua_set_CActivityList_id);
   tolua_variable(tolua_S,"type",tolua_get_CActivityList_type,tolua_set_CActivityList_type);
   tolua_variable(tolua_S,"sid",tolua_get_CActivityList_sid,tolua_set_CActivityList_sid);
   tolua_variable(tolua_S,"activityBreakdown",tolua_get_CActivityList_activityBreakdown,tolua_set_CActivityList_activityBreakdown);
   tolua_function(tolua_S,"new",tolua_Config_CActivityList_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_CActivityList_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_CActivityList_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_CActivityList_delete00);
   tolua_function(tolua_S,"AddBeginTime",tolua_Config_CActivityList_AddBeginTime00);
   tolua_function(tolua_S,"AddEndTime",tolua_Config_CActivityList_AddEndTime00);
   tolua_function(tolua_S,"AddSolidTime",tolua_Config_CActivityList_AddSolidTime00);
   tolua_function(tolua_S,"AddOpenServerTime",tolua_Config_CActivityList_AddOpenServerTime00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CActivityBreakdown","CActivityBreakdown","",tolua_collect_CActivityBreakdown);
  #else
  tolua_cclass(tolua_S,"CActivityBreakdown","CActivityBreakdown","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CActivityBreakdown");
   tolua_variable(tolua_S,"id",tolua_get_CActivityBreakdown_id,tolua_set_CActivityBreakdown_id);
   tolua_variable(tolua_S,"dayBreakdown",tolua_get_CActivityBreakdown_dayBreakdown,tolua_set_CActivityBreakdown_dayBreakdown);
   tolua_variable(tolua_S,"hourBreakdown",tolua_get_CActivityBreakdown_hourBreakdown,tolua_set_CActivityBreakdown_hourBreakdown);
   tolua_function(tolua_S,"new",tolua_Config_CActivityBreakdown_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_CActivityBreakdown_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_CActivityBreakdown_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_CActivityBreakdown_delete00);
   tolua_function(tolua_S,"AddDayBreakdownList",tolua_Config_CActivityBreakdown_AddDayBreakdownList00);
   tolua_function(tolua_S,"AddHourBreakdownList",tolua_Config_CActivityBreakdown_AddHourBreakdownList00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CBrushMonsterCfg","CBrushMonsterCfg","",tolua_collect_CBrushMonsterCfg);
  #else
  tolua_cclass(tolua_S,"CBrushMonsterCfg","CBrushMonsterCfg","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CBrushMonsterCfg");
   tolua_variable(tolua_S,"id",tolua_get_CBrushMonsterCfg_id,tolua_set_CBrushMonsterCfg_id);
   tolua_variable(tolua_S,"sid",tolua_get_CBrushMonsterCfg_sid,tolua_set_CBrushMonsterCfg_sid);
   tolua_variable(tolua_S,"mid",tolua_get_CBrushMonsterCfg_mid,tolua_set_CBrushMonsterCfg_mid);
   tolua_variable(tolua_S,"count",tolua_get_CBrushMonsterCfg_count,tolua_set_CBrushMonsterCfg_count);
   tolua_variable(tolua_S,"delayTime",tolua_get_CBrushMonsterCfg_delayTime,tolua_set_CBrushMonsterCfg_delayTime);
   tolua_variable(tolua_S,"refreshTime",tolua_get_CBrushMonsterCfg_refreshTime,tolua_set_CBrushMonsterCfg_refreshTime);
   tolua_variable(tolua_S,"x",tolua_get_CBrushMonsterCfg_x,tolua_set_CBrushMonsterCfg_x);
   tolua_variable(tolua_S,"y",tolua_get_CBrushMonsterCfg_y,tolua_set_CBrushMonsterCfg_y);
   tolua_function(tolua_S,"new",tolua_Config_CBrushMonsterCfg_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_CBrushMonsterCfg_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_CBrushMonsterCfg_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_CBrushMonsterCfg_delete00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CHeroList","CHeroList","",tolua_collect_CHeroList);
  #else
  tolua_cclass(tolua_S,"CHeroList","CHeroList","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CHeroList");
   tolua_variable(tolua_S,"heroId",tolua_get_CHeroList_heroId,tolua_set_CHeroList_heroId);
   tolua_variable(tolua_S,"heroType",tolua_get_CHeroList_heroType,tolua_set_CHeroList_heroType);
   tolua_variable(tolua_S,"heroName",tolua_get_CHeroList_heroName,tolua_set_CHeroList_heroName);
   tolua_function(tolua_S,"AddskillId",tolua_Config_CHeroList_AddskillId00);
   tolua_function(tolua_S,"new",tolua_Config_CHeroList_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_CHeroList_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_CHeroList_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_CHeroList_delete00);
  tolua_endmodule(tolua_S);
  #ifdef __cplusplus
  tolua_cclass(tolua_S,"CSkillIdList","CSkillIdList","",tolua_collect_CSkillIdList);
  #else
  tolua_cclass(tolua_S,"CSkillIdList","CSkillIdList","",NULL);
  #endif
  tolua_beginmodule(tolua_S,"CSkillIdList");
   tolua_variable(tolua_S,"skillId",tolua_get_CSkillIdList_skillId,tolua_set_CSkillIdList_skillId);
   tolua_variable(tolua_S,"skillType",tolua_get_CSkillIdList_skillType,tolua_set_CSkillIdList_skillType);
   tolua_variable(tolua_S,"skillCd",tolua_get_CSkillIdList_skillCd,tolua_set_CSkillIdList_skillCd);
   tolua_variable(tolua_S,"skillEffect",tolua_get_CSkillIdList_skillEffect,tolua_set_CSkillIdList_skillEffect);
   tolua_variable(tolua_S,"skillEffectCd",tolua_get_CSkillIdList_skillEffectCd,tolua_set_CSkillIdList_skillEffectCd);
   tolua_variable(tolua_S,"attrs",tolua_get_CSkillIdList_attrs,tolua_set_CSkillIdList_attrs);
   tolua_function(tolua_S,"AddAttrs",tolua_Config_CSkillIdList_AddAttrs00);
   tolua_function(tolua_S,"new",tolua_Config_CSkillIdList_new00);
   tolua_function(tolua_S,"new_local",tolua_Config_CSkillIdList_new00_local);
   tolua_function(tolua_S,".call",tolua_Config_CSkillIdList_new00_local);
   tolua_function(tolua_S,"delete",tolua_Config_CSkillIdList_delete00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_Config (lua_State* tolua_S) {
 return tolua_Config_open(tolua_S);
};
#endif

