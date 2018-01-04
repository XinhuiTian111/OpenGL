#pragma once
class GLShader
{
public:
    typedef enum {
        vertext_shader,
        fragment_shader,
    }ShaderType;
public:
    GLShader(void);
    ~GLShader(void);

    int LoadFromMem(const char* shader, int len, ShaderType st);
    int LoadFromFile(const char* filename, ShaderType st);

    int CompileShader();
    int LinkShader();

    inline int GetShaderID() const {return m_ShaderID;} 

protected:
    int ShaderLog();

protected:
    char* m_Shader;
    unsigned int m_ShaderID;
};


class GLProgram 
{
public:
    GLProgram() {}
    virtual ~GLProgram() {}

    int Init();
    int AttachShader(const GLShader& shader);
    int LinkProgram();

protected:
    unsigned int m_ProgramID;
};

