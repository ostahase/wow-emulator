// (c) Abyss Group
#include "ConfigEnv.h"

createFileSingleton(Config);

Config::Config() : mConf(0)
{
}


Config::~Config()
{
    if (mConf)
        delete mConf;
}


bool Config::SetSource(const char *file, bool ignorecase)
{
    mConf = new DOTCONFDocument(ignorecase ?
        DOTCONFDocument::CASEINSENSETIVE :
        DOTCONFDocument::CASESENSETIVE);

    if (mConf->setContent(file) == -1)
    {
        delete mConf;
        mConf = 0;
        return false;
    }

    return true;
}


bool Config::GetString(const char* name, std::string *value)
{
    if(!mConf)
        return false;

    DOTCONFDocumentNode *node = (DOTCONFDocumentNode *)mConf->findNode(name);
    if(!node || !node->getValue())
        return false;

    *value = node->getValue();

    return true;
}


std::string Config::GetStringDefault(const char* name, const char* def)
{
    if(!mConf)
        return std::string(def);

    DOTCONFDocumentNode *node = (DOTCONFDocumentNode *)mConf->findNode(name);
    if(!node || !node->getValue())
        return std::string(def);

    return std::string(node->getValue());
}


bool Config::GetBool(const char* name, bool *value)
{
    if(!mConf)
        return false;

    DOTCONFDocumentNode *node = (DOTCONFDocumentNode *)mConf->findNode(name);
    if(!node || !node->getValue())
        return false;

    const char* str = node->getValue();
    if(strcmp(str, "true") == 0 || strcmp(str, "TRUE") == 0 ||
        strcmp(str, "yes") == 0 || strcmp(str, "YES") == 0 ||
        strcmp(str, "1") == 0)
    {
        *value = true;
    }
    else
        *value = false;

    return true;
}


bool Config::GetBoolDefault(const char* name, const bool def)
{
    bool val;
    return GetBool(name, &val) ? val : def;
}


bool Config::GetInt(const char* name, int *value)
{
    if(!mConf)
        return false;

    DOTCONFDocumentNode *node = (DOTCONFDocumentNode *)mConf->findNode(name);
    if(!node || !node->getValue())
        return false;

    *value = atoi(node->getValue());

    return true;
}

bool Config::GetFloat(const char* name, float *value)
{
    if(!mConf)
        return false;

    DOTCONFDocumentNode *node = (DOTCONFDocumentNode *)mConf->findNode(name);
    if(!node || !node->getValue())
        return false;

    *value = atof(node->getValue());

    return true;
}




int Config::GetIntDefault(const char* name, const int def)
{
    int val;
    return GetInt(name, &val) ? val : def;
}

float Config::GetFloatDefault(const char* name, const float def)
{
    float val;
    return (GetFloat(name, &val) ? val : def);
}

