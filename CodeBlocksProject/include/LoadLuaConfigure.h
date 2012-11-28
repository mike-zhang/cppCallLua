#pragma once
#ifndef CLOADLUACONFIGURE_H
#define CLOADLUACONFIGURE_H

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::map;
using std::pair;

class CLoadLuaConfigure
{
public:
    CLoadLuaConfigure();
    bool loadScript(const char *luaScript);
    bool loadScriptByFile(const char *fileName);
    CLoadLuaConfigure(const char *fileName);
    virtual ~CLoadLuaConfigure();
protected:
private:
    lua_State  *L;
public:
    bool getGlobalBoolean(const char *key,bool *ret);
    bool getGlobalString(const char *key,char *retBuf,int len);
    bool getGlobalInt(const char *key,int *retInt);
    bool getGlobalFloat(const char *key,float *retFloat);
    bool getGlobalByteArray(
        const char *key,unsigned char* dst,int len);
    bool getIntField(
        const char *section,const char *key,int *retInt);
    bool getFloatField(
        const char *section,const char *key,float *retFloat);
    bool getStrField(
        const char *section,const char *key,string &retStr);
    bool getIntTableField(
        const char *section,const char *key,vector<int> &vInt);
    bool getByteArrayField(const char *key,
                           vector<vector<unsigned char> > &vvDst);
    bool getMapTableField(
        const char *section,
        const char *key,
        map<int,vector<int> > &mapRet);
    short luaFunWithShortRet(const char* funName);
    int luaFunWithIntRet(const char* funName);
    float luaFunWithFloatRet(const char* funName);
    char* luaFunWithStrRet(const char* funName);
    int luaFun1IntParamWithIntRet(
        const char* funName,int x);
    int luaFun2IntParamsWithIntRet(
        const char* funName,int x,int y);
    float luaFun2IntParamsWithFloatRet(
        const char* funName,int x,int y);
    short luaFun3IntParamsWithShortRet(
        const char* funName,int x,float y,int z);
};

#endif // CLOADLUACONFIGURE_H
