#include "StdAfx.h"
#include "GLShader.h"

#include <assert.h>

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/wglew.h>
#include <gl/gl.h>
#include <gl/GLU.h>

#include <stdio.h>


#pragma comment(lib,"glew32s.lib")


GLShader::GLShader(void)
{
}


GLShader::~GLShader(void)
{
}

int GLShader::LoadFromMem(const char* shader, int len, ShaderType st)
{
    assert(shader && len > 0);

    if (st == vertext_shader) {
        m_ShaderID = glCreateShader(GL_VERTEX_SHADER);
    }else if (st == fragment_shader){
        m_ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    }else return -1;

    if (!m_ShaderID) return -1;

    const char* codes[1] = {shader};
    glShaderSource(m_ShaderID, 1, codes, NULL);

    return 0;
}

int GLShader::CompileShader()
{
    glCompileShader(m_ShaderID);
    return ShaderLog();
}

int GLShader::LinkShader()
{
    return 0;
}

int GLShader::ShaderLog()
{
    int ret = 0;
    int logLen;
    char* log;
    int w; 
    glGetShaderiv(m_ShaderID, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
        log = new char[logLen+1];

        if (log) {
            glGetShaderInfoLog(m_ShaderID, logLen, &w, log);
            printf("%s\n", log);
            delete[] log;
            log = NULL;
        }
        ret = -1;
    }
    
    return ret;
}

int GLShader::LoadFromFile(const char* filename, ShaderType st)
{
    FILE* fp;
    size_t size;
    char* buff;

    assert(filename);

    fp = fopen(filename, "r");
    if (NULL == fp) return -1;
    
    fseek(fp, 0, SEEK_SET);
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    
    fseek(fp, 0, SEEK_SET);

    buff = new char[st+1];
    if (NULL == buff) return -1;
    fread(buff, size, 1, fp);

    fclose(fp);

    return LoadFromMem(buff, size, st);
}

int GLProgram::Init()
{
    m_ProgramID = glCreateProgram();
    if (0 == m_ProgramID) return -1;
    else return 0;
}

int GLProgram::AttachShader(const GLShader& shader)
{
    glAttachShader(m_ProgramID, shader.GetShaderID());
    return 0;
}

int GLProgram::LinkProgram()
{
    glLinkProgram(m_ProgramID);
    return 0;
}
