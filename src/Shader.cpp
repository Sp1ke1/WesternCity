#include "Shader.h"
#include "Shader.h"

Shader::Shader(const std::string& ShaderName)
    : m_ShaderName ( ShaderName ) 
{
}

bool Shader::LoadShaderFromFile(const std::string& VSPath, const std::string& FSPath)
{
    GLuint VS_ID = pgr::createShaderFromFile(GL_VERTEX_SHADER, VSPath); 
    if (!VS_ID)
    {
        std::cerr << "Shader::LoadShaderFromFile() Error: Can't create vertex shader with given path: " << VSPath << std::endl;
        return false;
    }

    GLuint FS_ID = pgr::createShaderFromFile(GL_FRAGMENT_SHADER, FSPath); 
    if (!FS_ID)
    {
        std::cerr << "Shader::LoadShaderFromFile() Error: Can't create fragment shader with given path: " << FSPath << std::endl;
        return false;
    }

    GLuint shaders[3];
    shaders[0] = VS_ID; 
    shaders[1] = FS_ID;
    shaders[2] = 0; 
    GLuint ProgramID = pgr::createProgram(shaders); 
    if (!ProgramID)
    {
        std::cerr << "Shader::LoadShaderFromFile() Error: Can't create shader program from given shaders: " << VSPath << " , " << FSPath << std::endl; 
        return false;
    }
    
    CHECK_GL_ERROR();
    m_ProgramID = ProgramID; 
    m_IsLoaded = true; 

    return true;
}

void Shader::UseShader() const
{
    if (!GetIsValid()) {
        std::cerr << "Shader::UseShader() Error : Can't use shader. Shader is not loaded " << std::endl; 
    }
    glUseProgram(m_ProgramID);
}

bool Shader::GetIsValid() const
{
    return m_IsLoaded; 
}

std::string Shader::GetShaderName() const
{
    return m_ShaderName; 
}

void Shader::BindMaterial(const Material& mat) const
{
    SetVec3Parameter("material.ambientColor", mat.m_AmbientColor); 
    SetVec3Parameter("material.diffuseColor", mat.m_DiffuseColor); 
    SetVec3Parameter("material.specularColor", mat.m_SpecularColor);
    SetFloatParameter("material.shininess", mat.m_Shininess); 
}


void Shader::SetBoolParameter(const std::string& ParameterName, bool value) const
{
    GLint UniformLocation = glGetUniformLocation(m_ProgramID, ParameterName.c_str());
    if (UniformLocation == -1)
    {
        std::cerr << "Shader::SetBoolParameter() Error: Can't find parameter with given name: " << ParameterName << std::endl; 
        return; 
    }
    glUniform1i(UniformLocation, static_cast<GLint> ( value ) );
}

void Shader::SetIntParameter(const std::string& ParameterName, int value) const
{
    GLint UniformLocation = glGetUniformLocation(m_ProgramID, ParameterName.c_str());
    if (UniformLocation == -1)
    {
        std::cerr << "Shader::SetIntParameter() Error: Can't find parameter with given name: " << ParameterName << std::endl;
        return;
    }
    glUniform1i(UniformLocation, value);
}

void Shader::SetFloatParameter(const std::string& ParameterName, float value) const
{
    GLint UniformLocation = glGetUniformLocation(m_ProgramID, ParameterName.c_str());
    if (UniformLocation == -1)
    {
        std::cerr << "Shader::SetFloatParameter() Error: Can't find parameter with given name: " << ParameterName << std::endl;
        return;
    }
    glUniform1f(UniformLocation, value);
    CHECK_GL_ERROR();
}

void Shader::SetVec3Parameter(const std::string& ParameterName, const glm::vec3& value) const
{
    GLint UniformLocation = glGetUniformLocation(m_ProgramID, ParameterName.c_str());
    if (UniformLocation == -1)
    {
        std::cerr << "Shader::SetVec3Parameter() Error: Can't find parameter with given name: " << ParameterName << std::endl;
        return;
    }
    glUniform3fv(UniformLocation, 1, glm::value_ptr(value)); 
}

void Shader::SetVec4Parameter(const std::string& ParameterName, const glm::vec4 & Value) const
{
    GLint UniformLocation = glGetUniformLocation(m_ProgramID, ParameterName.c_str());
    if (UniformLocation == -1)
    {
        std::cerr << "Shader::SetVec3Parameter() Error: Can't find parameter with given name: " << ParameterName << std::endl;
        return;
    }
    glUniform4fv(UniformLocation, 1, glm::value_ptr(Value));
}

void Shader::SetMat4Parameter(const std::string& ParameterName, const glm::mat4& value) const
{
    GLint UniformLocation = glGetUniformLocation(m_ProgramID, ParameterName.c_str());
    if (UniformLocation == -1)
    {
        std::cerr << "Shader::SetMat4Parameter() Error: Can't find parameter with given name: " << ParameterName << std::endl;
        return;
    }
    glUniformMatrix4fv(UniformLocation, 1, false, glm::value_ptr ( value ));
}

void Shader::SetMat3Parameter(const std::string& ParameterName, const glm::mat3& value) const
{
    GLint UniformLocation = glGetUniformLocation(m_ProgramID,ParameterName.c_str());
    if (UniformLocation == -1)
    {
        std::cerr << "Shader::SetMat3Parameter() Error: Can't find parameter with given name: " << ParameterName << std::endl;
        return;
    }
    glUniformMatrix3fv(UniformLocation, 1, false, glm::value_ptr ( value ));
}


