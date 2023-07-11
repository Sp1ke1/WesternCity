#include "Config.h"

bool Config::GetVariableByName(const std::string& Name, std::string & Variable) const
{
    if (m_Variables.count ( Name ) == 0 )
    {
        std::cerr << "Config :: GetVariableByName () => can't get variable with given name : " + Name << std::endl; // TODO:: consider deleting
        return false;
    }
    Variable = m_Variables . at ( Name );
    return true; 
}

bool Config::GetVariableAsInt(const std::string& Name, int& Variable) const
{
    std::string VarRaw; 
    if (!GetVariableByName(Name, VarRaw))
    {
        return false; 
    }
 
    try { Variable = std::stoi(VarRaw); }
    catch (...) { return false; }
    return true; 
}

bool Config::GetVariableAsString(const std::string& Name, std::string & Variable) const
{
    return GetVariableByName(Name, Variable); 

}

bool Config::LoadConfigFromFile(const std::string& filename)
{
    std::fstream f(filename); 
    if (!f)
    {
        std::cerr << "Config :: LoadConfigFromFile () => can't open file with given name: " << filename << std::endl; 
        return false; 
    }

    std::string VarName; 
    std::string VarValue; 
    while (f >> VarName >> VarValue)
    {
        if (m_Variables.count(VarName) != 0)
        {
            std::cerr << "Config :: LoadConfigFromFile () => duplicating name in the config variable  : " << VarName << std::endl; 
            return false; 
        }
        m_Variables[VarName] = VarValue; 
    }
    if ( !f.eof() )
    {
        std::cerr << "Config :: LoadConfigFromFile () => error during config file read. Didn't reached eof" << std::endl;
        return false;
    }
    return true; 

}