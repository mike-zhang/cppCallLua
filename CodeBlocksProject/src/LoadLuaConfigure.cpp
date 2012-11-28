#include "LoadLuaConfigure.h"
#include <string.h>

CLoadLuaConfigure::CLoadLuaConfigure()
{
    //ctor
    L = NULL;
}

CLoadLuaConfigure::~CLoadLuaConfigure()
{
    //dtor
    if(L)
    {
        lua_close(L);
    }
}


CLoadLuaConfigure::CLoadLuaConfigure(const char *fileName)
{
    L = luaL_newstate();
    luaL_openlibs (L);
    luaL_loadfile(L, fileName);
    lua_pcall(L, 0, 0, 0);
}

/// 加载脚本
bool CLoadLuaConfigure::loadScript(const char *luaScript)
{
    if(L)
    {
        lua_close(L);
    }
    L = luaL_newstate();
    luaL_openlibs (L);
    if (luaL_dostring(L,luaScript))  // Run lua script
    {
        lua_close(L);
        return false;
    }
    lua_pcall(L, 0, 0, 0);
    return true;
}
/// 通过文件加载脚本
bool CLoadLuaConfigure::loadScriptByFile(const char *fileName)
{
    //L = lua_open();
    L = luaL_newstate();
    luaL_openlibs (L);
    luaL_loadfile(L, fileName);
    lua_pcall(L, 0, 0, 0);
}

/// 读取全局布尔值
/// 返回值
///		true : 成功
///		false: 配置中不存在或配置出错
bool CLoadLuaConfigure::getGlobalBoolean(const char *key,bool *ret)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    lua_getglobal(L,key);
    if (lua_isboolean(L, -1))
    {
        *ret = (bool)lua_toboolean(L, -1);
        return true;
    }
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return false;
}

/// 读取全局字符串
/// 返回值
///		true : 成功
///		false: 配置中不存在、配置出错、bufLen太小
bool CLoadLuaConfigure::getGlobalString(
    const char *key,char *retBuf,int bufLen)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    lua_getglobal(L,key);
    if (lua_isstring(L, -1))
    {
        const char* p = (const char*)lua_tolstring(L, -1,NULL);
        int pLen = strlen(p);
        if(bufLen < pLen) //buf太小
        {
            lua_settop(L,nowTop);// 恢复之前的栈顶位置
            return false;
        }
        memcpy(retBuf,p,strlen(p));
        return true;
    }
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return false; //配置文件不存在或出错是
}

/// 读取全局整型数据
/// 返回值
///		true : 成功
///		false: 配置中不存在、配置出错
bool CLoadLuaConfigure::getGlobalInt(
    const char *key,int *retInt)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    lua_getglobal(L,key);
    if (lua_isnumber(L, -1))
    {
        *retInt = (int)lua_tonumber(L, -1);
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return true;
    }
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return false;
}


/// 读取全局浮点型数据
/// 返回值
///		true : 成功
///		false: 配置中不存在、配置出错
bool CLoadLuaConfigure::getGlobalFloat(
    const char *key,float *retFloat)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    lua_getglobal(L,key);
    if (lua_isnumber(L, -1))
    {
        *retFloat = (float)lua_tonumber(L, -1);
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return true;
    }
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return false;
}


/// 读取全局Byte数组
/// 返回值
///		true : 成功
///		false: 配置中不存在、配置出错
bool CLoadLuaConfigure::getGlobalByteArray(
    const char *key,unsigned char* dst,int len)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置

    lua_getglobal(L,key);
    if(!lua_istable(L,-1))
    {
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return false;
    }

    for(int i=1; i<=len; ++i)
    {
        int result = 0;
        lua_pushnumber(L, i);
        lua_gettable(L, -2);
        if (lua_isnumber(L, -1))
            result = (int)lua_tonumber(L, -1);
        else
        {
            lua_settop(L,nowTop);// 恢复之前的栈顶位置
            return false;
        }
        lua_pop(L, 1);
        *(dst+i-1) = result;
    }
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return true;
}

/// 读取Table中的整型数据
/// 返回值
///		true : 成功
///		false: 配置中不存在、配置出错
/*
例如：
section1 = {
    key1 = 2
}
*/
bool CLoadLuaConfigure::getIntField(
    const char *section,const char *key,int *retInt)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    lua_getglobal(L,section);
    if(!lua_istable(L,-1))
    {
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return false;
    }
    lua_pushstring(L, key);
    lua_gettable(L, -2);

    if (lua_isnumber(L, -1))
        *retInt = (int)lua_tonumber(L, -1);
    else
    {
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return false;
    }
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return true;
}

/// 读取Table中的浮点型数据
/// 返回值
///		true : 成功
///		false: 配置中不存在、配置出错
/*
例如：
section1 = {
    key1 = 2.34
}
*/
bool CLoadLuaConfigure::getFloatField(
    const char *section,const char *key,float *retFloat)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    lua_getglobal(L,section);
    if(!lua_istable(L,-1))
    {
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return false;
    }
    lua_pushstring(L, key);
    lua_gettable(L, -2);

    if (lua_isnumber(L, -1))
        *retFloat = (float)lua_tonumber(L, -1);
    else
    {
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return false;
    }

    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return true;
}

/// 读取Table中的字符型数据
/// 返回值
///		true : 成功
///		false: 配置中不存在、配置出错
/*
例如：
section1 = {
    key1 = "retStr"
}
*/
bool CLoadLuaConfigure::getStrField(
    const char *section,const char *key,string &retStr)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置

    lua_getglobal(L,section);
    if(!lua_istable(L,-1))
    {
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return false;
    }

    lua_pushstring(L, key);
    lua_gettable(L, -2);

    if(lua_isstring(L,-1))
        retStr = lua_tostring(L,-1);
    else
    {
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return false;
    }

    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return true;
}

/// 读取Table中的int table型数据
/// 返回值
///		true : 成功
///		false: 配置中不存在、配置出错
/*
例如：
section1 = {
    key1 = {1,2}
}
*/
bool CLoadLuaConfigure::getIntTableField(
    const char *section,const char *key,vector<int> &vInt)

{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    lua_getglobal(L,section);
    if(!lua_istable(L,-1))
    {
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return false;
    }

    lua_pushstring(L, key);
    lua_gettable(L, -2);

    if(!lua_istable(L,-1))
    {
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return false;
    }

    int it = lua_gettop(L);
    lua_pushnil(L);

    while(lua_next(L, it))
    {
        int result = 0;
        if (lua_isnumber(L, -1))
            result = (int)lua_tonumber(L, -1);
        else
        {
            lua_pop(L, 1);
            break;
        }
        lua_pop(L, 1);
        vInt.push_back(result);
    }
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return true;
}

/// 读取Table中的Byte数组
/// 返回值
///		true : 成功
///		false: 配置中不存在、配置出错
/*
例如：
key = {
{0x00,0x1E,0xC0,0x00,0x00,0xF0},
{0x00,0x1E,0xC0,0x00,0x00,0xF1}
｝
*/
bool CLoadLuaConfigure::getByteArrayField(const char *key,
        vector<vector<unsigned char> > &vvDst)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置

    unsigned char dst[1024];
    int len = sizeof(dst);


    lua_getglobal(L,key);

    if(!lua_istable(L,-1))
    {
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return false;
    }

    int sz = 0;
//	sz = lua_gettop(L);
    int it = lua_gettop(L);
    int i = 1;
    lua_pushnil(L);
    while(lua_next(L, it))
    {
        int tmp = 0;
        if (lua_istable(L, -1))
        {
            vector<unsigned char> vTmp;
            sz = lua_gettop(L);
            sz += 1;
            for(int j=1; j <= sz; ++j)
            {
                int result = 0;
                lua_pushnumber(L, j);
                lua_gettable(L, -2);
                if (lua_isnumber(L, -1))
                    result = (int)lua_tonumber(L, -1);
                else
                {
                    lua_settop(L,nowTop);// 恢复之前的栈顶位置
                    return false;
                }
                lua_pop(L, 1);
                //*(dst+i-1) = result;
                vTmp.push_back((unsigned char)result);
            }
            vvDst.push_back(vTmp);
        }
        else
        {
            lua_pop(L, 1);
            break;
        }

        lua_pop(L, 1);
    }
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return true;
}


/// 读取Table中的map型数据
/// 返回值
///		true : 成功
///		false: 配置中不存在、配置出错
/*
例如：
section1 = {
    key1 = {
        {0,{0}},
        {1,{1,2}},
        {2,{1}}
    }
}
*/
bool CLoadLuaConfigure::getMapTableField(
    const char *section,const char *key,
    map<int,vector<int> > &mapRet)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    lua_getglobal(L,section);
    if(!lua_istable(L,-1))
    {
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return false;
    }

    lua_pushstring(L, key);
    lua_gettable(L, -2);

    if(!lua_istable(L,-1))
    {
        lua_settop(L,nowTop);// 恢复之前的栈顶位置
        return false;
    }
    int len = 0;
    len = lua_gettop(L);
    len = len+1;
    for(int i = 1; i <= len; ++i)
    {
        long nowTop2 = lua_gettop(L); //在最开始，保存栈顶位置

        int result = 0;
        lua_pushnumber(L, i);
        lua_gettable(L, -2);

        if(!lua_istable(L,-1))
        {
            lua_settop(L,nowTop2);// 恢复之前的栈顶位置
            //break;
            continue;
        }

        lua_pushnumber(L, 1);
        lua_gettable(L, -2);

        if (lua_isnumber(L, -1))
            result = (int)lua_tonumber(L, -1);
        else
        {
            lua_settop(L,nowTop2);// 恢复之前的栈顶位置
            //break;
            continue;
        }

        lua_pop(L, 1);

        lua_pushnumber(L, 2);
        lua_gettable(L, -2);

        if(!lua_istable(L,-1))
        {
            lua_settop(L,nowTop2);// 恢复之前的栈顶位置
            //break;
            continue;
        }

        vector<int> vInt;
        int it = lua_gettop(L);
        lua_pushnil(L);

        while(lua_next(L, it))
        {
            int tmp = 0;
            if (lua_isnumber(L, -1))
                tmp = (int)lua_tonumber(L, -1);
            else
            {
                lua_pop(L, 1);
                break;
            }

            lua_pop(L, 1);
            vInt.push_back(tmp);
        }
        mapRet.insert(pair<int,vector<int> >(result,vInt));
        lua_settop(L,nowTop2);// 恢复之前的栈顶位置
    }
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return true;
}

short CLoadLuaConfigure::luaFunWithShortRet(const char* funName)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    short ret;
    //函数名
    lua_getglobal(L,funName);
    //调用函数
    lua_call(L, 0, 1);
    //得到返回值
    ret = (short)lua_tonumber(L, -1);
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return ret;
}

int CLoadLuaConfigure::luaFunWithIntRet(const char* funName)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    int ret;
    //函数名
    lua_getglobal(L,funName);
    //调用函数
    lua_call(L, 0, 1);
    //得到返回值
    ret = (int)lua_tonumber(L, -1);
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return ret;
}

float CLoadLuaConfigure::luaFunWithFloatRet(const char* funName)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    float ret;
    //函数名
    lua_getglobal(L,funName);
    //调用函数
    lua_call(L, 0, 1);
    //得到返回值
    ret = (float)lua_tonumber(L, -1);
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return ret;
}


char* CLoadLuaConfigure::luaFunWithStrRet(const char* funName)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    char* ret;
    //函数名
    lua_getglobal(L,funName);
    //调用函数
    lua_call(L, 0, 1);
    //得到返回值
    ret = (char*)lua_tostring(L,-1);
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return ret;
}

int CLoadLuaConfigure::luaFun1IntParamWithIntRet(
    const char* funName,int x)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    int ret = 0;
    //函数名
    lua_getglobal(L,funName);
    //第一个参数压栈
    lua_pushnumber(L, x);
    //调用函数
    lua_call(L, 1, 1);
    //得到返回值
    ret = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return ret;
}

int CLoadLuaConfigure::luaFun2IntParamsWithIntRet(
    const char* funName,int x,int y)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    int ret;
    //函数名
    lua_getglobal(L,funName);
    //第一个参数压栈
    lua_pushnumber(L, x);
    //第二个参数压栈
    lua_pushnumber(L, y);
    //调用函数
    lua_call(L, 2, 1);
    //得到返回值
    ret = (int)lua_tonumber(L, -1);
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return ret;
}


float CLoadLuaConfigure::luaFun2IntParamsWithFloatRet(const char* funName,int x,int y)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    float ret;
    //函数名
    lua_getglobal(L,funName);
    //第一个参数压栈
    lua_pushnumber(L, x);
    //第二个参数压栈
    lua_pushnumber(L, y);
    //调用函数
    lua_call(L, 2, 1);
    //得到返回值
    ret = (float)lua_tonumber(L, -1);
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return ret;
}

short CLoadLuaConfigure::luaFun3IntParamsWithShortRet(
    const char* funName,int x,float y,int z)
{
    long nowTop = lua_gettop(L); //在最开始，保存栈顶位置
    short ret;
    //函数名
    lua_getglobal(L,funName);
    //第一个参数压栈
    lua_pushnumber(L, x);
    //第二个参数压栈
    lua_pushnumber(L, y);
    //第三个参数压栈
    lua_pushnumber(L, z);
    //调用函数
    lua_call(L, 3, 1);
    //得到返回值
    ret = (int)lua_tonumber(L, -1);
    lua_settop(L,nowTop);// 恢复之前的栈顶位置
    return ret;
}
