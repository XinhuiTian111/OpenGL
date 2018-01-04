#pragma once

#include "KeyPress.h"

class OpenGLWin
{
public:
    OpenGLWin(void);
    ~OpenGLWin(void);

    bool Initialize();
    void Shutdown();
    void Run();
    int CreateGLWin(const TCHAR* title, UINT width, UINT height, bool fullscreen);

protected:

    virtual inline void DrawGLScence();
    
    virtual inline ATOM RegisterClass(const TCHAR* clsname);
    virtual inline BOOL InitInstance();

    int InitGLWin32();
    int InitGL();
    int ResizeGLScence(UINT width, UINT height);

protected:
    HINSTANCE m_hinstance;
    TCHAR m_applicationName[256];
    HWND m_hwnd_main;
    HGLRC m_hRC;
    bool m_FullScreen;

    KeyPress m_kp;
    bool m_Active;
};

