// LG_LIST.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "windows.h"
#include "Resource.h"


#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

#define SCREEN_WIDTH    (256U)
#define SCREEN_HIGHT    (240U)
//#define SCREEN_WIDTH CRAPHICS_DEVICE_SCREENSIZE_X
//#define SCREEN_HIGHT CRAPHICS_DEVICE_SCREENSIZE_Y

#define SCREEN_REFRESH_RATE (3)//fps

#define COLOR_RED       (0x00ff0000)
#define COLOR_GREEN     (0x0000ff00)
#define COLOR_BLUE      (0x000000ff)
#define COLOR_WHITE     (0x00ffffff)
#define COLOR_BLACK     (0x00000000)
#define COLOR_1         (0x00ffff00)
#define COLOR_2         (0x0000ffff)
#define COLOR_3         (0x00ff00ff)

//static unsigned int GDRAM[SCREEN_WIDTH * SCREEN_HIGHT];
unsigned int* GDRAM;
static HWND hWnd;
static bool flag_btn_pressed_up = false;
static bool flag_btn_pressed_down = false;
static bool flag_btn_pressed_left = false;
static bool flag_btn_pressed_right = false;
static bool flag_btn_pressed_enter = false;

bool read_io_up() {
    return flag_btn_pressed_up;
}
bool read_io_down() {
    return flag_btn_pressed_down;
}
bool read_io_left() {
    return flag_btn_pressed_left;
}
bool read_io_right() {
    return flag_btn_pressed_right;
}
bool read_io_enter() {
    return flag_btn_pressed_enter;
}
void simulator_screen_init() {

}

void simulator_screen_draw_point(unsigned int x, unsigned int y, int color) {
    GDRAM[y * SCREEN_WIDTH + x] = color;
    //LGTools.GDDRAM[y * SCREEN_WIDTH + x] = color;
}

void simulator_screen_repaint() {
    InvalidateRect(hWnd, NULL, FALSE);
    UpdateWindow(hWnd);
}

void fill_screen(int color) {
    int x = 0, y = 0;
    for (y = 0; y < SCREEN_HIGHT; y++) {
        for (x = 0; x < SCREEN_WIDTH; x++) {
            GDRAM[y * SCREEN_WIDTH + x] = color;
            //LGTools.GDDRAM[y * SCREEN_WIDTH + x] = color;
        }
    }
}

void onPaint(HDC hdc) {
    HDC bitmapDC = CreateCompatibleDC(hdc);
    HBITMAP bitmap = CreateBitmap(SCREEN_WIDTH, SCREEN_HIGHT, 1, 32, GDRAM);
   // HBITMAP bitmap = CreateBitmap(SCREEN_WIDTH, SCREEN_HIGHT, 1, 32, LGTools.GDDRAM);
    BITMAP bitmap_;
    SelectObject(bitmapDC, bitmap);
    GetObject(bitmap, sizeof(BITMAP), &bitmap_);

    BitBlt(hdc, 0, 0, bitmap_.bmWidth, bitmap_.bmHeight, bitmapDC, 0, 0, SRCCOPY);
    //OutputDebugString(L"onPaint\n");
}
unsigned int color_list[] = { COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_1, COLOR_2, COLOR_3 };

/*屏幕显示程序在此调用*/
DWORD WINAPI Task1(LPVOID p) {
    int color_index = 0;
    while (1) {

        //lg_task_handler();
        OutputDebugString(L"lg_task_handler() invoke\n");
        //OutputDebugString(L"lala");
        //fill_screen(color_list[color_index++]);
        //color_index %= 6;
        InvalidateRect(hWnd, NULL, FALSE);
        UpdateWindow(hWnd);
        Sleep(1000 / SCREEN_REFRESH_RATE);
    }
}
/*lg_task_handler();在此处调用*/
DWORD WINAPI Task2(LPVOID p) {

    //x_AppMain();
    OutputDebugString(L"x_AppMain() invoke\n");
    while (1) {
        //flex_button_scan();
        Sleep(5);
    }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NOFRENDO3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


    /*
    CreateThread(NULL, 0, Task2, NULL, 0, NULL);
    GDRAM = LGTools.GDDRAM;
    Sleep(1000);

    fill_screen(COLOR_WHITE);
    */
    //fill_screen(COLOR_WHITE);
    //lg_init();
    //x_AppMain();
    CreateThread(NULL, 0, Task2, NULL, 0, NULL);
    Sleep(1000);
    /*
    LGTools.pf_FastDrawPoint(50, 50, COLOR_SET);
    LGTools.pf_FastDrawPoint(50, 51, COLOR_SET);
    LGTools.pf_dispTextArea("1a23456");
    */
    CreateThread(NULL, 0, Task1, NULL, 0, NULL);

    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOFRENDO3));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOFRENDO3));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_NOFRENDO3);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, SCREEN_WIDTH +100, SCREEN_HIGHT + 100, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 分析菜单选择:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
        onPaint(hdc);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_UP:
            OutputDebugString(L"up\n");
            flag_btn_pressed_up = true;
            //lg_SetBtnVal(LG_ITEM_A_CLICK, 1);
            break;

        case VK_DOWN:
            OutputDebugString(L"down\n");
            flag_btn_pressed_down = true;
            //lg_SetBtnVal(LG_ITEM_B_CLICK, 1);
            break;

        case VK_LEFT:
            OutputDebugString(L"left\n");
            flag_btn_pressed_left = true;
            break;

        case VK_RIGHT:
            OutputDebugString(L"right\n");
            flag_btn_pressed_right = true;
            break;
        case VK_RETURN:
            OutputDebugString(L"enter\n");
            flag_btn_pressed_enter = true;
            //lg_SetBtnVal(LG_ITEM_OK_CLICK, 1);
            break;
        }break;
    case WM_KEYUP:
        switch (wParam)
        {
        case VK_UP:
            OutputDebugString(L"up......\n");
            flag_btn_pressed_up = false;
            //lg_SetBtnVal(LG_ITEM_A_CLICK, 1);
            break;

        case VK_DOWN:
            OutputDebugString(L"down......\n");
            flag_btn_pressed_down = false;
            //lg_SetBtnVal(LG_ITEM_B_CLICK, 1);
            break;

        case VK_LEFT:
            OutputDebugString(L"left......\n");
            flag_btn_pressed_left = false;
            break;

        case VK_RIGHT:
            OutputDebugString(L"right......\n");
            flag_btn_pressed_right = false;
            break;
        case VK_RETURN:
            OutputDebugString(L"enter......\n");
            flag_btn_pressed_enter = false;
            //lg_SetBtnVal(LG_ITEM_OK_CLICK, 1);
            break;
        }break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
