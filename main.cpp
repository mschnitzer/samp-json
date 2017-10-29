#include <iostream>
#include <string>
#include <vector>
#include "json_spirit/json_spirit.h"
#include "amx/amx.h"
#include "plugincommon.h"
#include "settings.h"
#include "AmxUtils.h"

typedef void(*logprintf_t)(char* format, ...);
logprintf_t logprintf;
extern void *pAMXFunctions;

struct t_JsonData
{
    int id;
    json_spirit::Object object;
};

std::vector<t_JsonData> json_data;
int json_data_id = 0;

cell AMX_NATIVE_CALL json_decode(AMX* amx, cell* params)
{
    std::string json_string = AmxUtils::amx_GetStdString(amx, &params[1]);

    json_spirit::Value value;
    auto success = json_spirit::read(json_string, value);

    if (success == false)
        return 0;

    json_data_id++;
    json_data.push_back({ json_data_id, value.get_obj() });

    return json_data_id;
}

cell AMX_NATIVE_CALL json_get_int(AMX* amx, cell* params)
{
    int data_id = params[1];
    std::string key = AmxUtils::amx_GetStdString(amx, &params[2]);

    for (auto &x : json_data)
    {
        if (x.id == data_id)
        {
            for (auto &i : x.object)
            {
                if (i.name_ == key && i.value_.type() == json_spirit::int_type)
                {
                    return i.value_.get_int();
                }
            }
        }
    }

    return -1;
}

cell AMX_NATIVE_CALL json_get_string(AMX* amx, cell* params)
{
    int data_id = params[1];
    std::string key = AmxUtils::amx_GetStdString(amx, &params[2]);

    for (auto &x : json_data)
    {
        if (x.id == data_id)
        {
            for (auto &i : x.object)
            {
                if (i.name_ == key && i.value_.type() == json_spirit::str_type)
                {
                    cell *addr = NULL;

                    amx_GetAddr(amx, params[3], &addr);
                    amx_SetString(addr, i.value_.get_str().c_str(), 0, 0, params[4]);

                    return 1;
                }
            }
        }
    }

    return -1;
}

cell AMX_NATIVE_CALL json_get_float(AMX* amx, cell* params)
{
    int data_id = params[1];
    std::string key = AmxUtils::amx_GetStdString(amx, &params[2]);

    for (auto &x : json_data)
    {
        if (x.id == data_id)
        {
            for (auto &i : x.object)
            {
                if (i.name_ == key && i.value_.type() == json_spirit::real_type)
                {
                    const float value = (float)i.value_.get_real();
                    return amx_ftoc(value);
                }
            }
        }
    }

    return -1.0;
}

cell AMX_NATIVE_CALL json_delete(AMX* amx, cell* params)
{
    int data_id = params[1], index = 0;

    for (auto &i : json_data)
    {
        if (i.id == data_id)
        {
            json_data.erase(json_data.begin()+index);
            return 1;
        }

        index++;
    }

    return 0;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
    logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];

    logprintf(" -----------------------------------------------------------------");
    logprintf(" SA:MP JSON plugin version %s successfully loaded!", PLUGIN_VERSION);
    logprintf(" Developer: Manuel Schnitzer");
    logprintf(" -----------------------------------------------------------------");

    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
    logprintf(" * SA:MP JSON plugin was unloaded.");
}

extern "C" const AMX_NATIVE_INFO PluginNatives[] =
{
    { "json_decode", json_decode },
    { "json_get_int", json_get_int },
    { "json_get_string", json_get_string },
    { "json_get_float", json_get_float },
    { "json_delete", json_delete },
    { 0, 0 }
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
    return amx_Register(amx, PluginNatives, -1);
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
    return AMX_ERR_NONE;
}
