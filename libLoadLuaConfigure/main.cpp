#include <iostream>
#include <iomanip>
#include "LoadLuaConfigure.h"

CLoadLuaConfigure g_LoadLuaConfigure;
using namespace std;

string luaScriptStr = "";

void initScript()
{
    luaScriptStr = "";
    luaScriptStr += "g_Bool = true\n";
    luaScriptStr += "g_String = \"This is a global string\"\n";
    luaScriptStr += "g_Int = 123\n";
    luaScriptStr += "g_Float = 123.456\n";
    luaScriptStr += "g_ByteArray = {0x00,0x11,0x22,0x33,0x44,0x55}\n";
    luaScriptStr += "section1 = {\n";
    luaScriptStr += "	key_int = 1,\n";
    luaScriptStr += "	key_float = 1.23,\n";
    luaScriptStr += "	key_string = \"key string\",\n";
    luaScriptStr += "	key_intArray = {1,2,3},\n";
    luaScriptStr += "	key_map = {\n";
    luaScriptStr += "		{0,{0}},\n";
    luaScriptStr += "		{1,{1,2}},\n";
    luaScriptStr += "		{2,{1}}\n";
    luaScriptStr += "	},\n";
    luaScriptStr += "	keyTable = { subkey_string = \"value string \", subkey_int = 1} \n";
    luaScriptStr += "}\n";
    luaScriptStr += "function retIntValue()\n";
    luaScriptStr += "	return 1234\n";
    luaScriptStr += "end\n";
    luaScriptStr += "function retFloatValue()\n";
    luaScriptStr += "	return 12.345\n";
    luaScriptStr += "end\n";
    luaScriptStr += "function retIntValue1(arg)\n";
    luaScriptStr += "	return 1+arg\n";
    luaScriptStr += "end\n";
    luaScriptStr += "function retIntValue2(arg1,arg2)\n";
    luaScriptStr += "	return arg1+arg2\n";
    luaScriptStr += "end\n";
}

void getGlobalBooleanTest()
{
    cout << "getGlobalBooleanTest begin "<<endl;
    bool ret = false;
    cout<<"Before get : "<<ret<<endl;
    if(g_LoadLuaConfigure.getGlobalBoolean("g_Bool",&ret))
        cout<<"After get  : "<<ret<<endl;
    else
        cout<<"Get fail!"<<endl;
    cout << "getGlobalBooleanTest end \n"<<endl;
}

void getGlobalStringTest()
{
    cout << "getGlobalStringTest begin"<<endl;
    char retBuf[1024] = {0};
    cout<<"Before get : "<<retBuf<<endl;
    if(g_LoadLuaConfigure.getGlobalString("g_String",retBuf,sizeof(retBuf)))
        cout<<"After get  : "<<retBuf<<endl;
    else
        cout<<"Get fail!"<<endl;
    cout << "getGlobalStringTest end\n"<<endl;
}

void getGlobalIntTest()
{
    cout << "getGlobalIntTest begin"<<endl;
    int retInt = 0;
    cout<<"Before get : "<<retInt<<endl;
    if(g_LoadLuaConfigure.getGlobalInt("g_Int",&retInt))
        cout<<"After get  : "<<retInt<<endl;
    else
        cout<<"Get fail!"<<endl;
    cout << "getGlobalIntTest end\n"<<endl;
}

void getGlobalFloatTest()
{
    cout << "getGlobalFloatTest begin"<<endl;
    float retFloat = 0;
    cout<<"Before get : "<<retFloat<<endl;
    if(g_LoadLuaConfigure.getGlobalFloat("g_Float",&retFloat))
        cout<<"After get  : "<<retFloat<<endl;
    else
        cout<<"Get fail!"<<endl;
    cout << "getGlobalFloatTest end\n"<<endl;
}

void getGlobalByteArrayTest()
{
    cout << "getGlobalByteArrayTest begin"<<endl;
    unsigned char retByteArr[6] = {0};
    int i=0,sz=sizeof(retByteArr);
    cout<<"Before get : ";
    for(i = 0 ; i < sz ; ++i)
    {
        cout << hex << setfill('0')<<setw(2) << (int)retByteArr[i] ;
    }
    cout << dec;
    cout << endl;

    if(g_LoadLuaConfigure.getGlobalByteArray("g_ByteArray",retByteArr,sz))
    {
        cout<<"After get  : ";
        for(i = 0 ; i < sz ; ++i)
        {
            cout << hex << setfill('0') << setw(2) << (int)retByteArr[i] ;
        }
        cout << dec;
        cout << endl;
    }

    else
        cout<<"Get fail!"<<endl;
    cout << "getGlobalByteArrayTest end\n"<<endl;
}

void getIntFieldTest()
{
    cout << "getIntFieldTest begin"<<endl;
    int ret = 0;
    cout<<"Before get : "<<ret<<endl;
    if(g_LoadLuaConfigure.getIntField("section1","key_int",&ret))
        cout<<"After get  : "<<ret<<endl;
    else
        cout<<"Get fail!"<<endl;
    cout << "getIntFieldTest end\n"<<endl;
}

void getFloatFieldTest()
{
    cout << "getFloatFieldTest begin"<<endl;
    float ret = 0;
    cout<<"Before get : "<<ret<<endl;
    if(g_LoadLuaConfigure.getFloatField("section1","key_float",&ret))
        cout<<"After get  : "<<ret<<endl;
    else
        cout<<"Get fail!"<<endl;
    cout << "getFloatFieldTest end\n"<<endl;
}

void getStrFieldTest()
{
    cout << "getStrFieldTest begin"<<endl;
    string ret = "";
    cout<<"Before get : "<<ret<<endl;
    if(g_LoadLuaConfigure.getStrField("section1","key_string",ret))
        cout<<"After get  : "<<ret<<endl;
    else
        cout<<"Get fail!"<<endl;
    cout << "getStrFieldTest end\n"<<endl;
}

void getIntTableFieldTest()
{
    cout << "getIntTableFieldTest begin"<<endl;
    vector<int> vInt;
    cout<<"Before get : ";
    int i = 0,sz = 0;
    sz = vInt.size();
    for(i = 0 ; i < sz ; ++i)
    {
        cout << vInt.at(i) << " ";
    }
    cout << endl;

    if(g_LoadLuaConfigure.getIntTableField("section1","key_intArray",vInt))
    {
        cout<<"After get  : ";
        sz = vInt.size();
        for(i = 0 ; i < sz ; ++i)
        {
            cout << vInt.at(i) << " ";
        }
        cout << endl;
    }
    else
        cout<<"Get fail!"<<endl;
    cout << "getIntTableFieldTest end\n"<<endl;
}

void getMapTableFieldTest()
{
    cout << "getMapTableFieldTest begin"<<endl;
    map<int,vector<int> > mapRet;
    map<int,vector<int> >::iterator it;
    cout<<"Before get : "<<endl;
    for(it = mapRet.begin() ; it != mapRet.end() ; ++it)
    {
        cout << it->first << " : ";
        vector<int> tmp = it->second;
        for(int i = 0,sz = tmp.size(); i < sz ; ++i)
        {
            cout << tmp.at(i) << " ";
        }
        cout<<endl;
    }
    cout << endl;

    if(g_LoadLuaConfigure.getMapTableField("section1","key_map",mapRet))
    {
        cout<<"After get  : "<<endl;
        for(it = mapRet.begin() ; it != mapRet.end() ; ++it)
        {
            cout << it->first << " : ";
            vector<int> tmp = it->second;
            for(int i = 0,sz = tmp.size(); i < sz ; ++i)
            {
                cout << tmp.at(i) << " ";
            }
            cout<<endl;
        }
        cout << endl;
    }
    else
        cout<<"Get fail!"<<endl;
    cout << "getMapTableFieldTest end\n"<<endl;
}

void getSubTableStrFieldTest()
{
    cout << "getSubTableStrFieldTest begin"<<endl;
    string strTmp = "null";
    cout<<"Before get : "<<strTmp<<endl;

    if(g_LoadLuaConfigure.getSubTableStrField("section1","keyTable","subkey_string",strTmp))
    {
        cout<<"After get  : "<<strTmp<<endl;
    }
    else
        cout<<"Get fail!"<<endl;
    cout << "getSubTableStrFieldTest end\n"<<endl;
}

void getSubTableIntFieldTest()
{
    cout << "getSubTableIntFieldTest begin"<<endl;
    int itmp = 0;
    cout<<"Before get : "<< itmp<<endl;

    if(g_LoadLuaConfigure.getSubTableIntField("section1","keyTable","subkey_int", &itmp))
    {
        cout<<"After get  : "<< itmp<<endl;
    }
    else
        cout<<"Get fail!"<<endl;
    cout << "getSubTableIntFieldTest end\n"<<endl;
}


void retIntValueTest()
{
    cout << "retIntValueTest begin"<<endl;
    int ret = -1;
    cout<<"Before get : "<<ret<<endl;
    ret = g_LoadLuaConfigure.luaFunWithIntRet("retIntValue");
    cout<<"After get  : "<<ret<<endl;
    cout << "retIntValueTest end\n"<<endl;
}

void retFloatValueTest()
{
    cout << "retFloatValueTest begin"<<endl;
    float ret = -1;
    cout<<"Before get : "<<ret<<endl;
    ret = g_LoadLuaConfigure.luaFunWithFloatRet("retFloatValue");
    cout<<"After get  : "<<ret<<endl;
    cout << "retFloatValueTest end\n"<<endl;
}

void retIntValue1Test()
{
    cout << "retIntValue1Test begin"<<endl;
    int ret = -1;
    cout<<"Before get : "<<ret<<endl;
    ret = g_LoadLuaConfigure.luaFun1IntParamWithIntRet("retIntValue1",22);
    cout<<"After get  : "<<ret<<endl;
    cout << "retIntValue1Test end\n"<<endl;
}

void retIntValue2Test()
{
    cout << "retIntValue2Test begin"<<endl;
    int ret = -1;
    cout<<"Before get : "<<ret<<endl;
    ret = g_LoadLuaConfigure.luaFun2IntParamsWithIntRet("retIntValue2",22,23);
    cout<<"After get  : "<<ret<<endl;
    cout << "retIntValue2Test end\n"<<endl;
}

int main()
{
    initScript();
    g_LoadLuaConfigure.loadScript(luaScriptStr.c_str());

    getGlobalBooleanTest();
    getGlobalStringTest();
    getGlobalIntTest();
    getGlobalFloatTest();
    getGlobalByteArrayTest();
    getIntFieldTest();
    getFloatFieldTest();
    getStrFieldTest();
    getIntTableFieldTest();
    getMapTableFieldTest();
    getSubTableStrFieldTest();
    getSubTableIntFieldTest();
    retIntValueTest();
    retFloatValueTest();
    retIntValue1Test();
    retIntValue2Test();

    return 0;
}
