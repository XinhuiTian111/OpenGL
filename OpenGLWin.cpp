#include "StdAfx.h"
#include "OpenGLWin.h"

#include "KeyPress.h"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <gl/gl.h>
#include <gl/GLU.h>

static KeyPress kp;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

OpenGLWin::OpenGLWin(void) : m_Active(true)
{
    _tcscpy(m_applicationName, TEXT("MyRenderMainFrame"));
}


OpenGLWin::~OpenGLWin(void)
{
}

bool OpenGLWin::Initialize()
{
    RegisterClass(TEXT("MyRenderMainFrame"));
    InitInstance();
    return true;
}

ATOM OpenGLWin::RegisterClass(const TCHAR* clsname)
{
    WNDCLASSEX wc;
    DEVMODE dmScreenSettings;

    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = WndProc;         //指定回调函数
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(NULL);       // 应用程序实例句柄
    wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
    wc.hIconSm       = wc.hIcon;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //默认黑色窗口黑色背景
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = clsname;
    wc.cbSize        = sizeof(WNDCLASSEX);

    return RegisterClassEx(&wc);
}

BOOL OpenGLWin::InitInstance()
{
    //m_hwnd_main = 
    //    CreateWindowEx(
    //        WS_EX_APPWINDOW, 
    //        m_applicationName,
    //        L"my_window", 
    //        WS_POPUP|WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS | WS_CLIPCHILDREN,//WS_OVERLAPPEDWINDOW
    //        100, 100, 800, 600, 
    //        NULL, NULL, 
    //        GetModuleHandle(NULL), 
    //        NULL);

    //ShowWindow(m_hwnd_main, SW_SHOW);
    //SetForegroundWindow(m_hwnd_main);
    //SetFocus(m_hwnd_main);

    return TRUE;
}

void OpenGLWin::Run()
{
    MSG msg;
    bool exit = false;

    ZeroMemory(&msg, sizeof(msg));
    while (!exit) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            switch (msg.message) {
            case WM_CLOSE:
                exit = true;
                break;
            case WM_SIZE:
                ResizeGLScence(LOWORD(msg.lParam), HIWORD(msg.lParam));
                break;

//             case WM_KEYUP:
//                 kp.KeyUp(msg.wParam);
//                 break;

            case WM_KEYDOWN:
                m_kp.KeyDown(msg.wParam);
                break;

            case WM_ACTIVATE:
                !HIWORD(msg.wParam) ? m_Active = true : m_Active = false;
                break;
            default:
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                break;
            }
            
        }else{
            if (m_kp.IsKeyDown(VK_ESCAPE)) {
                exit = true;
                continue;
            }

            DrawGLScence();
            SwapBuffers(GetDC(m_hwnd_main));
        }
    }
}

void OpenGLWin::Shutdown()
{
}

int OpenGLWin::InitGLWin32()
{
    HDC hDC;
    GLuint pixFmt;

    static  PIXELFORMATDESCRIPTOR pfd = {           // pfd Tells Windows How We Want Things To Be  
        sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor  
        1,                                          // Version Number  
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|
        PFD_DOUBLEBUFFER|PFD_STEREO_DONTCARE,
        PFD_TYPE_RGBA,                              // Request An RGBA Format  
        24,                                         // Select Our Color Depth  
        0, 0, 0, 0, 0, 0,                           // Color Bits Ignored  
        0,                                          // No Alpha Buffer  
        0,                                          // Shift Bit Ignored  
        0,                                          // No Accumulation Buffer  
        0, 0, 0, 0,                                 // Accumulation Bits Ignored  
        16,                                         // 16Bit Z-Buffer (Depth Buffer)    
        0,                                          // No Stencil Buffer  
        0,                                          // No Auxiliary Buffer  
        PFD_MAIN_PLANE,                             // Main Drawing Layer  
        0,                                          // Reserved  
        0, 0, 0                                     // Layer Masks Ignored  
    };

    hDC = GetDC(m_hwnd_main);
    pixFmt = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, pixFmt, &pfd);
    m_hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, m_hRC);

    return 0;
}

int OpenGLWin::InitGL()
{
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    return 0;
}

int OpenGLWin::CreateGLWin(const TCHAR* title, UINT width, UINT height, bool fullscreen)
{
    RECT winRC, deskRC;
    HWND hDesk;
    DWORD style, styleEx;
    winRC.left = 0;
    winRC.top = 0;
    winRC.right = width;
    winRC.bottom = height;

    m_FullScreen = fullscreen;

    RegisterClass(title);

    m_hwnd_main = 
        CreateWindowEx(
        WS_EX_APPWINDOW, 
        title,
        title, 
        WS_POPUP|WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS | WS_CLIPCHILDREN,//WS_OVERLAPPEDWINDOW
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
        NULL, NULL, 
        GetModuleHandle(NULL), 
        NULL);

    hDesk = GetDesktopWindow();
    GetWindowRect(hDesk, &deskRC);
    SetWindowLong(m_hwnd_main, GWL_STYLE, WS_POPUP);
    SetWindowPos(m_hwnd_main, HWND_TOP, 0, 0, deskRC.right, deskRC.bottom, SWP_SHOWWINDOW);
    
    InitGLWin32();

    ShowWindow(m_hwnd_main, SW_SHOW);
    UpdateWindow(m_hwnd_main);
    SetForegroundWindow(m_hwnd_main);
    SetFocus(m_hwnd_main);

    ResizeGLScence(deskRC.right, deskRC.bottom);

    InitGL();

    return 0;
}

int OpenGLWin::ResizeGLScence(UINT width, UINT height)
{
    if (0 == height) height = 1;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

//    glDrawBuffer(GL_BACK);

    return 0;
}

void OpenGLWin::DrawGLScence()
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    glColor4f(1.0f,0.0f,0.0f,1.0f);
    glVertex2f(100.0f,50.0f);
    glColor4f(0.0f,1.0f,0.0f,1.0f);
    glVertex2f(450.0f,400.0f);
    glColor4f(0.0f,0.0f,1.0f,1.0f);
    glVertex2f(450.0f,50.0f);
    glEnd();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message) {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, message, wparam, lparam);
    }
}