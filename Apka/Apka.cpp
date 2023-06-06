#include <iostream>
#include <string>
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <atlstr.h>
#include <commctrl.h>
#include <thread>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <utility>
#include <strsafe.h>

#include "framework.h"
#include "Apka.h"

#define OTL_ODBC // Compile OTL 4/ODBC, MS SQL 2008
#include "otlv4.h"

//#define OTL_ODBC // Compile OTL 4/ODBC. Uncomment this when used with MS SQL 7.0/ 2000

#define MAX_LOADSTRING 100
#define BUFSIZE 4096 
// Global Variables:

HWND hWnd;

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int window_width;
int window_height;
std::wstring shopID;
std::wstring shopAdress;
std::wstring shopIP;
bool active = false;
bool found = false;
bool sqlRunning = false;
std::wstring sqlStatus(L"executing");
int index;

int posx = 0;
int posy = 30;

std::string doc_in;
std::string doc_out;

bool extended = 0;

std::wstring device1, deivce2;

std::string appdata;

struct Competency
{
    bool open = false;

    std::vector<std::wstring> dane1;
    std::vector<std::wstring> dane2;
    std::vector<std::wstring> dane3;
    std::vector<std::wstring> dane4;
    std::vector<std::wstring> dane5;
    std::vector<std::wstring> dane6;
    std::vector<std::wstring> dane7;
    std::vector<std::wstring> dane8;
};

struct Company
{
    bool open = false;

    std::vector<std::wstring> dane1;
    std::vector<std::wstring> dane2;
    std::vector<std::wstring> dane3;
    std::vector<std::wstring> dane4;
    std::vector<std::wstring> dane5;
    std::vector<std::wstring> dane6;
};

Competency competency;
Company com;

HWND text_storeNumber;
HWND combo_country;
HWND button_run;
HWND button_stop;

HWND button_resize;

HWND text_manager;
HWND text_deputy;

HWND text_phone1, text_phone2;
HWND text_vendor, text_link;
HWND desc_phone1, desc_phone2;
HWND desc_vendor, desc_link;
HWND text_email, desc_email;


HWND text_bos;      HWND text_laptop;       HWND text_gw;
HWND text_pos1;     HWND text_sco1;         HWND text_router;
HWND text_pos2;     HWND text_sco2;         HWND text_printer;
HWND text_pos3;     HWND text_sco3;         HWND text_music;
HWND text_pos4;     HWND text_sco4;         HWND text_ap1;
                                            HWND text_ap2;

// resized parameters ------------------------------------------------------------------------
HWND button_sql_ok;
HWND text_sql,       desc_mm;
HWND text_doc_in,   desc_doc_in;
HWND text_doc_out,  desc_doc_out;
HWND button_sql_abort;
HWND button_sql_clear;
HWND desc_sql_status;

HWND frame_resize;

// checkboxes and frames --------------------------------------------------------------------

HWND checkbox_lap;
HWND checkbox_sco;
HWND checkbox_ap;
HWND checkbox_fastConnect;

HWND frame_lap;
HWND frame_sco;
HWND frame_ap;

//HWND text_console;                                                                  /////////////////////

HWND button_bos;      HWND button_laptop;       HWND button_gw;
HWND button_pos1;     HWND button_sco1;         HWND button_router;
HWND button_pos2;     HWND button_sco2;         HWND button_printer;
HWND button_pos3;     HWND button_sco3;         HWND button_music;
HWND button_pos4;     HWND button_sco4;         HWND button_ap1;
                                                HWND button_ap2;

HWND button_exp_bos;      HWND button_exp_laptop;
HWND button_exp_pos1;     HWND button_exp_sco1;
HWND button_exp_pos2;     HWND button_exp_sco2;
HWND button_exp_pos3;     HWND button_exp_sco3;
HWND button_exp_pos4;     HWND button_exp_sco4;         


HWND progressBar_bos;      HWND progressBar_laptop;       HWND progressBar_gw;
HWND progressBar_pos1;     HWND progressBar_sco1;         HWND progressBar_router;
HWND progressBar_pos2;     HWND progressBar_sco2;         HWND progressBar_printer;
HWND progressBar_pos3;     HWND progressBar_sco3;         HWND progressBar_music;
HWND progressBar_pos4;     HWND progressBar_sco4;         HWND progressBar_ap1;
                                                          HWND progressBar_ap2;

std::thread* bos;      std::thread* laptop;       std::thread* gw;
std::thread* pos1;     std::thread* sco1;         std::thread* router;
std::thread* pos2;     std::thread* sco2;         std::thread* printer;
std::thread* pos3;     std::thread* sco3;         std::thread* music;
std::thread* pos4;     std::thread* sco4;         std::thread* ap1;
                                                    std::thread* ap2;
                                                    std::thread* findap;
HANDLE sql = 0;
std::thread* sqlDescStatus;


std::wstring string_bos;           std::wstring string_lap;           std::wstring string_gw;
std::wstring string_pos1;          std::wstring string_sco1;          std::wstring string_router;
std::wstring string_pos2;          std::wstring string_sco2;          std::wstring string_print;
std::wstring string_pos3;          std::wstring string_sco3;          std::wstring string_MXQ;
std::wstring string_pos4;          std::wstring string_sco4;          std::wstring string_ap1;
                                                                      std::wstring string_ap2;
std::wstring string_aruba;


HACCEL hAccelTable;

MSG msg;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Tools(HWND, UINT, WPARAM, LPARAM);

std::wstring ExecCmd(wchar_t*);

void updateWindowSize(HWND hWnd);
void runConnection(std::wstring machine);
void runWebsite(std::wstring adress);
void runExplorer(std::wstring machine, std::wstring ip);
void updatePingIP(HWND id, HWND textId, std::wstring ID);
void readCompetency();
void read();
void CreateConsole();
void findAccesPoints();
DWORD WINAPI runSQL(LPVOID lpParam);
void updateSQLStatus();
std::wstring StringToWString(const std::string& str);
std::string WStringToString(const std::wstring& wstr);

otl_connect db[6]; // connect object

void select();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_APKA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APKA));

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(hWnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APKA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_APKA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, CW_USEDEFAULT, 540 * 1.2, 430 * 1.2, nullptr, nullptr, hInstance, nullptr);

   // default 540 415

   INITCOMMONCONTROLSEX icc;
   icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
   icc.dwICC = ICC_BAR_CLASSES;
   InitCommonControlsEx(&icc);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   appdata.append("\\PingApp\\");

   //CreateConsole();
   //updateWindowSize(hWnd);

   // TEXT
   text_storeNumber = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       50 * 1.2, 5 * 1.2, 55 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);

   //text_console = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
    //   5 * 1.2, 420 * 1.2, 555 * 1.2, 60 * 1.2, hWnd, NULL, hInstance, NULL);

   text_phone1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       415 * 1.2, 2 * 1.2, 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_phone2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       415 * 1.2, 23 * 1.2, 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);

   desc_phone1 = CreateWindowEx(0, L"STATIC", L"Manager:", WS_CHILD | WS_VISIBLE,
       350 * 1.2, 2 * 1.2, 63 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   desc_phone2 = CreateWindowEx(0, L"STATIC", L"Deputy:", WS_CHILD | WS_VISIBLE,
       361 * 1.2, 23 * 1.2, 52 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);

   text_vendor = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       250 * 1.2, 2 * 1.2, 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_link = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       250 * 1.2, 23 * 1.2, 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);

   desc_vendor = CreateWindowEx(0, L"STATIC", L"Vendor:", WS_CHILD | WS_VISIBLE,
       206 * 1.2, 2 * 1.2, 44 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   desc_link = CreateWindowEx(0, L"STATIC", L"Link:", WS_CHILD | WS_VISIBLE,
       222 * 1.2, 23 * 1.2, 27 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);

   text_email = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       270 * 1.2, 43 * 1.2, 245 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   desc_email = CreateWindowEx(0, L"STATIC", L"e-mail:", WS_CHILD | WS_VISIBLE,
       222 * 1.2, 43 * 1.2, 47 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);

   //ramki

   frame_lap = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
       posx + (175 * 1.2), posy + (37 * 1.2), 169 * 1.2, 68 * 1.2, hWnd, NULL, hInstance, NULL);
   frame_sco = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
       posx + (175 * 1.2), posy + (97 * 1.2), 169 * 1.2, 248 * 1.2, hWnd, NULL, hInstance, NULL);
   frame_ap = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
       posx + (344 * 1.2), posy + (277 * 1.2), 169 * 1.2, 68 * 1.2, hWnd, NULL, hInstance, NULL);

   frame_resize = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
       posx + (530 * 1.2), -20, 2 * 1.2, 500 * 1.2, hWnd, NULL, hInstance, NULL);

   //checkbox'y

   checkbox_lap = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
       posx + (336 * 1.2), posy + (50 * 1.2), 13 * 1.2, 13 * 1.2, hWnd, (HMENU)CHECKBOX_LAP, hInstance, NULL);

   checkbox_sco = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
       posx + (336 * 1.2), posy + (110 * 1.2), 13 * 1.2, 13 * 1.2, hWnd, (HMENU)CHECKBOX_SCO, hInstance, NULL);

   checkbox_ap = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
       posx + (506 * 1.2), posy + (290 * 1.2), 13 * 1.2, 13 * 1.2, hWnd, (HMENU)CHECKBOX_AP, hInstance, NULL);

   checkbox_fastConnect = CreateWindowEx(0, L"BUTTON", L"FastConnect", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
       5 * 1.2, 25 * 1.2, 99 * 1.2, 20 * 1.2, hWnd, (HMENU)CHECKBOX_FASTCONNECT, hInstance, NULL);

   //Combo

   combo_country = CreateWindowEx(WS_EX_CLIENTEDGE, L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
       CBS_DROPDOWNLIST, 5 * 1.2, 5 * 1.2, 45 * 1.2, 200 * 1.2, hWnd, NULL, hInstance, NULL);

   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"PL");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"SK");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"CZ");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"RO");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"HU");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"HR");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"SI");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"LT");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"LV");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"EE");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"BG");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"IT");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"RS");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"ES");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"AT");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"DE");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"GR");
   SendMessage(combo_country, CB_ADDSTRING, 0, (LPARAM)L"PG");

   button_run = CreateWindowEx(0, L"BUTTON", L"RUN", WS_CHILD | WS_VISIBLE,
       110 * 1.2, 5 * 1.2, 45 * 1.2, 35 * 1.2, hWnd, (HMENU)BUTTON_RUN, hInstance, NULL);
   button_stop = CreateWindowEx(0, L"BUTTON", L"STOP", WS_CHILD | WS_VISIBLE,
       155 * 1.2, 5 * 1.2, 45 * 1.2, 35 * 1.2, hWnd, (HMENU)BUTTON_STOP, hInstance, NULL);


   // MM -------------------------------------------------------------------------------------------

   button_sql_ok = CreateWindowEx(0, L"BUTTON", L"Search", WS_CHILD | WS_VISIBLE,
       702 * 1.2, 5 * 1.2, 45 * 1.2, 20 * 1.2, hWnd, (HMENU)BUTTON_MM_OK, hInstance, NULL);

   button_sql_clear = CreateWindowEx(0, L"BUTTON", L"Clear", WS_CHILD | WS_VISIBLE,
       878 * 1.2, 27 * 1.2, 45 * 1.2, 20 * 1.2, hWnd, (HMENU)BUTTON_MM_CLEAR, hInstance, NULL);

   button_sql_abort = CreateWindowEx(0, L"BUTTON", L"Abort", WS_CHILD | WS_VISIBLE,
       878 * 1.2, 2 * 1.2, 45 * 1.2, 20 * 1.2, hWnd, (HMENU)BUTTON_MM_ABORT, hInstance, NULL);

   text_sql = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       540 * 1.2, 5 * 1.2, 160 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);

   desc_sql_status = CreateWindowEx(0, L"STATIC", L"ready!", WS_CHILD | WS_VISIBLE,
       748 * 1.2, 5 * 1.2, 80 * 1.2, 16 * 1.2, hWnd, NULL, hInstance, NULL);

   desc_doc_in = CreateWindowEx(0, L"STATIC", L"desc_doc_in: ", WS_CHILD | WS_VISIBLE,
       540 * 1.2, 30 * 1.2, 80 * 1.2, 16 * 1.2, hWnd, NULL, hInstance, NULL);

   text_doc_in = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 
       540 * 1.2, 48 * 1.2, 385 * 1.2, 140 * 1.2, hWnd, NULL, hInstance, NULL);

   desc_doc_out = CreateWindowEx(0, L"STATIC", L"desc_doc_out:", WS_CHILD | WS_VISIBLE,
       540 * 1.2, 190 * 1.2, 80 * 1.2, 16 * 1.2, hWnd, NULL, hInstance, NULL);

   text_doc_out = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
       540 * 1.2, 208 * 1.2, 385 * 1.2, 160 * 1.2, hWnd, NULL, hInstance, NULL);
   

   button_bos = CreateWindowEx(0, L"BUTTON", L"BOS", WS_CHILD | WS_VISIBLE,
       posx + (10 * 1.2), posy + (50 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_BOS1, hInstance, NULL);
   button_pos1 = CreateWindowEx(0, L"BUTTON", L"POS01", WS_CHILD | WS_VISIBLE,
       posx + (10 * 1.2), posy + (110 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_POS1, hInstance, NULL);
   button_pos2 = CreateWindowEx(0, L"BUTTON", L"POS02", WS_CHILD | WS_VISIBLE,
       posx + (10 * 1.2), posy + (170 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_POS2, hInstance, NULL);
   button_pos3 = CreateWindowEx(0, L"BUTTON", L"POS03", WS_CHILD | WS_VISIBLE,
       posx + (10 * 1.2), posy + (230 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_POS3, hInstance, NULL);
   button_pos4 = CreateWindowEx(0, L"BUTTON", L"POS04", WS_CHILD | WS_VISIBLE,
       posx + (10 * 1.2), posy + (290 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_POS4, hInstance, NULL);

   button_laptop = CreateWindowEx(0, L"BUTTON", L"LAP", WS_CHILD | WS_VISIBLE,
       posx + (180 * 1.2), posy + (50 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_LAP, hInstance, NULL);
   button_sco1 = CreateWindowEx(0, L"BUTTON", L"SCO01", WS_CHILD | WS_VISIBLE,
       posx + (180 * 1.2), posy + (110 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_SCO1, hInstance, NULL);
   button_sco2 = CreateWindowEx(0, L"BUTTON", L"SCO02", WS_CHILD | WS_VISIBLE,
       posx + (180 * 1.2), posy + (170 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_SCO2, hInstance, NULL);
   button_sco3 = CreateWindowEx(0, L"BUTTON", L"SCO03", WS_CHILD | WS_VISIBLE,
       posx + (180 * 1.2), posy + (230 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_SCO3, hInstance, NULL);
   button_sco4 = CreateWindowEx(0, L"BUTTON", L"SCO04", WS_CHILD | WS_VISIBLE,
       posx + (180 * 1.2), posy + (290 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_SCO4, hInstance, NULL);

   button_gw = CreateWindowEx(0, L"BUTTON", L"GW", WS_CHILD | WS_VISIBLE,
       posx + (350 * 1.2), posy + (50 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_GW, hInstance, NULL);
   button_router = CreateWindowEx(0, L"BUTTON", L"RUTER", WS_CHILD | WS_VISIBLE,
       posx + (350 * 1.2), posy + (110 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_ROUTER, hInstance, NULL);
   button_printer = CreateWindowEx(0, L"BUTTON", L"PRINT", WS_CHILD | WS_VISIBLE,
       posx + (350 * 1.2), posy + (170 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_PRINT, hInstance, NULL);
   button_music = CreateWindowEx(0, L"BUTTON", L"MXQ", WS_CHILD | WS_VISIBLE,
       posx + (350 * 1.2), posy + (230 * 1.2), 40 * 1.2, 40 * 1.2, hWnd, (HMENU)BUTTON_MUSIC, hInstance, NULL);

   button_ap1 = CreateWindowEx(0, L"BUTTON", L"AP1", WS_CHILD | WS_VISIBLE,
       posx + (350 * 1.2), posy + (290 * 1.2), 40 * 1.2, 20 * 1.2, hWnd, (HMENU)BUTTON_AP1, hInstance, NULL);
   button_ap2 = CreateWindowEx(0, L"BUTTON", L"AP2", WS_CHILD | WS_VISIBLE,
       posx + (350 * 1.2), posy + (310 * 1.2), 40 * 1.2, 20 * 1.2, hWnd, (HMENU)BUTTON_AP2, hInstance, NULL);

   //---------------------------------------------------------

   button_exp_bos = CreateWindowEx(0, L"BUTTON", L"BOS", WS_CHILD | WS_VISIBLE,
       posx + (10 * 1.2), posy + (90 * 1.2), 40 * 1.2, 10 * 1.2, hWnd, (HMENU)BUTTON_BOS1E, hInstance, NULL);
   button_exp_pos1 = CreateWindowEx(0, L"BUTTON", L"POS01", WS_CHILD | WS_VISIBLE,
       posx + (10 * 1.2), posy + (150 * 1.2), 40 * 1.2, 10 * 1.2, hWnd, (HMENU)BUTTON_POS1E, hInstance, NULL);
   button_exp_pos2 = CreateWindowEx(0, L"BUTTON", L"POS02", WS_CHILD | WS_VISIBLE,
       posx + (10 * 1.2), posy + (210 * 1.2), 40 * 1.2, 10 * 1.2, hWnd, (HMENU)BUTTON_POS2E, hInstance, NULL);
   button_exp_pos3 = CreateWindowEx(0, L"BUTTON", L"POS03", WS_CHILD | WS_VISIBLE,
       posx + (10 * 1.2), posy + (270 * 1.2), 40 * 1.2, 10 * 1.2, hWnd, (HMENU)BUTTON_POS3E, hInstance, NULL);
   button_exp_pos4 = CreateWindowEx(0, L"BUTTON", L"POS04", WS_CHILD | WS_VISIBLE,
       posx + (10 * 1.2), posy + (330 * 1.2), 40 * 1.2, 10 * 1.2, hWnd, (HMENU)BUTTON_POS4E, hInstance, NULL);

   button_exp_laptop = CreateWindowEx(0, L"BUTTON", L"LAP", WS_CHILD | WS_VISIBLE,
       posx + (180 * 1.2), posy + (90 * 1.2), 40 * 1.2, 10 * 1.2, hWnd, (HMENU)BUTTON_LAP, hInstance, NULL);
   button_exp_sco1 = CreateWindowEx(0, L"BUTTON", L"SCO01", WS_CHILD | WS_VISIBLE,
       posx + (180 * 1.2), posy + (150 * 1.2), 40 * 1.2, 10 * 1.2, hWnd, (HMENU)BUTTON_SCO1E, hInstance, NULL);
   button_exp_sco2 = CreateWindowEx(0, L"BUTTON", L"SCO02", WS_CHILD | WS_VISIBLE,
       posx + (180 * 1.2), posy + (210 * 1.2), 40 * 1.2, 10 * 1.2, hWnd, (HMENU)BUTTON_SCO2E, hInstance, NULL);
   button_exp_sco3 = CreateWindowEx(0, L"BUTTON", L"SCO03", WS_CHILD | WS_VISIBLE,
       posx + (180 * 1.2), posy + (270 * 1.2), 40 * 1.2, 10 * 1.2, hWnd, (HMENU)BUTTON_SCO3E, hInstance, NULL);
   button_exp_sco4 = CreateWindowEx(0, L"BUTTON", L"SCO04", WS_CHILD | WS_VISIBLE,
       posx + (180 * 1.2), posy + (330 * 1.2), 40 * 1.2, 10 * 1.2, hWnd, (HMENU)BUTTON_SCO4E, hInstance, NULL);

   // ---------------------------------------------------------

   text_bos = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((64 - 10) * 1.2), posy + ((70 + 6) * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_pos1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((64 - 10) * 1.2), posy + ((130 + 6) * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_pos2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((64 - 10) * 1.2), posy + ((190 + 6) * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_pos3 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((64 - 10) * 1.2), posy + ((250 + 6) * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_pos4 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((64 - 10) * 1.2), posy + ((310 + 6) * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);


   text_laptop = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((234 - 10) * 1.2), posy + ((70 + 6) * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_sco1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((234 - 10) * 1.2), posy + ((130 + 6) * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_sco2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((234 - 10) * 1.2), posy + ((190 + 6) * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_sco3 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((234 - 10) * 1.2), posy + ((250 + 6) * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_sco4 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((234 - 10) * 1.2), posy + ((310 + 6) * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);

   text_gw = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((404 - 10) * 1.2), posy + (70 * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_router = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((404 - 10) * 1.2), posy + (130 * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_printer = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((404 - 10) * 1.2), posy + (190 * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_music = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + ((404 - 10) * 1.2), posy + (250 * 1.2), 100 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);

   text_ap1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + (404 * 1.2), posy + (310 * 1.2), 50 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   text_ap2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
       posx + (454 * 1.2), posy + (310 * 1.2), 50 * 1.2, 20 * 1.2, hWnd, NULL, hInstance, NULL);
   
   // PROGRES BAR'S

   progressBar_bos = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((65 - 10) * 1.2), posy + (50 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);
   progressBar_pos1 = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((65 - 10) * 1.2), posy + (110 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);
   progressBar_pos2 = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((65 - 10) * 1.2), posy + (170 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);
   progressBar_pos3 = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((65 - 10) * 1.2), posy + (230 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);
   progressBar_pos4 = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((65 - 10) * 1.2), posy + (290 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);

   progressBar_laptop = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((235 - 10) * 1.2), posy + (50 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);
   progressBar_sco1 = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((235 - 10) * 1.2), posy + (110 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);
   progressBar_sco2 = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((235 - 10) * 1.2), posy + (170 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);
   progressBar_sco3 = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((235 - 10) * 1.2), posy + (230 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);
   progressBar_sco4 = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((235 - 10) * 1.2), posy + (290 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);

   progressBar_gw = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((405 - 10) * 1.2), posy + (50 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);
   progressBar_router = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((405 - 10) * 1.2), posy + (110 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);
   progressBar_printer = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((405 - 10) * 1.2), posy + (170 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);
   progressBar_music = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + ((405 - 10) * 1.2), posy + (230 * 1.2), 100 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);

   progressBar_ap1 = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + (405 * 1.2), posy + (290 * 1.2), 50 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);
   progressBar_ap2 = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
       posx + (455 * 1.2), posy + (290 * 1.2), 50 * 1.2, 15 * 1.2, hWnd, (HMENU)200, hInstance, NULL);

   button_resize = CreateWindowEx(0, L"BUTTON", L">>", WS_CHILD | WS_VISIBLE,
       500 * 1.2, 65 * 1.2, 15 * 1.2, 15 * 1.2, hWnd, (HMENU)BUTTON_RESIZE, hInstance, NULL);

   SendMessage(progressBar_bos, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_bos, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_pos1, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_pos1, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_pos2, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_pos2, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_pos3, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_pos3, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_pos4, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_pos4, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_laptop, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_laptop, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_sco1, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_sco1, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_sco2, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_sco2, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_sco3, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_sco3, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_sco4, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_sco4, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_gw, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_gw, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_router, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_router, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_printer, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_printer, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_music, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_music, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_ap1, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_ap1, PBM_SETSTEP,( WPARAM ) 1, 0 );
   SendMessage(progressBar_ap2, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0, 44));
   SendMessage( progressBar_ap2, PBM_SETSTEP,( WPARAM ) 1, 0 );

   //-----------------------------------------------------------------------------------------------

   SendMessage(progressBar_bos, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_pos1, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_pos2, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_pos3, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_pos4, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_laptop, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_sco1, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_sco2, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_sco3, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_sco4, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_gw, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_router, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_printer, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_music, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_ap1, PBM_SETSTEP, (WPARAM)4, 0);
   SendMessage(progressBar_ap2, PBM_SETSTEP, (WPARAM)4, 0);


    EnableWindow(button_stop, false);
    EnableWindow(button_bos, false);
    EnableWindow(button_pos1, false); 
    EnableWindow(button_pos2, false); 
    EnableWindow(button_pos3, false); 
    EnableWindow(button_pos4, false); 
    EnableWindow(button_laptop, false); 
    EnableWindow(button_sco1, false); 
    EnableWindow(button_sco2, false); 
    EnableWindow(button_sco3, false); 
    EnableWindow(button_sco4, false);
    EnableWindow(button_gw, false);
    EnableWindow(button_router, false);
    EnableWindow(button_printer , false); 
    EnableWindow(button_music , false); 
    EnableWindow(button_ap2, false);
    EnableWindow(button_ap1, false);

    EnableWindow(button_exp_bos, false);
    EnableWindow(button_exp_pos1, false); 
    EnableWindow(button_exp_pos2, false); 
    EnableWindow(button_exp_pos3, false); 
    EnableWindow(button_exp_pos4, false); 
    EnableWindow(button_exp_laptop, false); 
    EnableWindow(button_exp_sco1, false); 
    EnableWindow(button_exp_sco2, false); 
    EnableWindow(button_exp_sco3, false); 
    EnableWindow(button_exp_sco4, false);

    EnableWindow(button_sql_abort, false);
    EnableWindow(checkbox_lap, false);
    EnableWindow(checkbox_ap, false);

    CheckDlgButton(hWnd, CHECKBOX_SCO, BST_CHECKED);
    CheckDlgButton(hWnd, CHECKBOX_FASTCONNECT, BST_CHECKED);

   // -------------------------------------------------- 
    SetFocus(text_storeNumber);

    readCompetency();
    read();
    
   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
                case (BUTTON_RUN):
                {
                    shopID.clear();
                    std::wstring numberDNS;

                    int len = GetWindowTextLength(text_storeNumber) + 1;
                    wchar_t* text = new wchar_t[len];
                    GetWindowText(text_storeNumber, &text[0], len);

                    numberDNS.append(text);
                    delete[] text;
                    text = nullptr;

                    if (numberDNS[0] == 'w')
                    {

                        
                    }
                    else
                    {
                        shopID.append(numberDNS);


                        std::wstring cmd = L"ping ";
                        cmd.append(shopID);
                        cmd.append(L" /n 1");
                        std::wstring pingStatus = ExecCmd((wchar_t*)cmd.c_str());

                        std::wstring ip = pingStatus;

                        if (pingStatus[0] == 'P')
                        {
                            MessageBox(hWnd, L"Sklep nie istnieje", L"Nie znaleziono sklepu", MB_ICONINFORMATION);
                            shopID.clear();
                            SetFocus(text_storeNumber);
                            SendMessage(text_storeNumber, EM_SETSEL, 0, 6);
                        }
                        else
                        {
                            if (competency.open == true)
                            {
                            }

                            if (numberDNS[0] == L'0')
                                numberDNS.erase(0, 1);

                            shopAdress = string_print;

                            string_gw = L"127.0.0.1";
                            string_bos = L"127.0.0.1";
                            string_lap = L"127.0.0.1";
                            string_pos1 = L"127.0.0.1";
                            string_pos2 = L"127.0.0.1";
                            string_pos3 = L"127.0.0.1";
                            string_pos4 = L"127.0.0.1";
                            string_sco1 = L"127.0.0.1";
                            string_sco2 = L"127.0.0.1";
                            string_sco3 = L"127.0.0.1";
                            string_sco4 = L"127.0.0.1";
                            string_MXQ = L"127.0.0.1";
                            string_print = L"127.0.0.1";
                            string_router = L"127.0.0.1";


                            active = true;
                            EnableWindow(button_run, false);
                            EnableWindow(button_stop, true);
                            EnableWindow(text_storeNumber, false);

                            EnableWindow(button_bos, true);
                            EnableWindow(button_laptop, true);
                            EnableWindow(button_pos1, true);
                            EnableWindow(button_sco1, true);
                            EnableWindow(button_pos2, true);
                            EnableWindow(button_sco2, true);
                            EnableWindow(button_pos3, true);
                            EnableWindow(button_sco3, true);
                            EnableWindow(button_pos4, true);
                            EnableWindow(button_sco4, true);
                            EnableWindow(button_gw, true);
                            EnableWindow(button_router, true);
                            EnableWindow(button_music, true);
                            EnableWindow(button_printer, true);
                            EnableWindow(button_exp_bos, true);
                            EnableWindow(button_exp_laptop, true);
                            EnableWindow(button_exp_pos1, true);
                            EnableWindow(button_exp_sco1, true);
                            EnableWindow(button_exp_pos2, true);
                            EnableWindow(button_exp_sco2, true);
                            EnableWindow(button_exp_pos3, true);
                            EnableWindow(button_exp_sco3, true);
                            EnableWindow(button_exp_pos4, true);
                            EnableWindow(button_exp_sco4, true);

                            SetWindowText(text_gw, string_gw.c_str());
                            SetWindowText(text_bos, string_bos.c_str());
                            SetWindowText(text_laptop, string_lap.c_str());
                            SetWindowText(text_pos1, string_pos1.c_str());
                            SetWindowText(text_pos2, string_pos2.c_str());
                            SetWindowText(text_pos3, string_pos3.c_str());
                            SetWindowText(text_pos4, string_pos4.c_str());
                            SetWindowText(text_sco1, string_sco1.c_str());
                            SetWindowText(text_sco2, string_sco2.c_str());
                            SetWindowText(text_sco3, string_sco3.c_str());
                            SetWindowText(text_sco4, string_sco4.c_str());
                            SetWindowText(text_music, string_MXQ.c_str());
                            SetWindowText(text_printer, string_print.c_str());
                            if (competency.open == true)
                                SetWindowText(text_router, string_router.c_str());
                            else
                                SetWindowText(text_router, L"Brak pliku");

                            Button_SetCheck(checkbox_ap, BST_UNCHECKED);
                            Button_SetCheck(checkbox_lap, BST_UNCHECKED);
                            EnableWindow(checkbox_lap, true);
                            EnableWindow(checkbox_ap, true);

                            bos = new std::thread(updatePingIP, progressBar_bos, text_bos, string_bos);

                            pos1 = new std::thread(updatePingIP, progressBar_pos1, text_pos1, string_pos1);
                            pos2 = new std::thread(updatePingIP, progressBar_pos2, text_pos2, string_pos2);
                            pos3 = new std::thread(updatePingIP, progressBar_pos3, text_pos3, string_pos3);
                            pos4 = new std::thread(updatePingIP, progressBar_pos4, text_pos4, string_pos4);

                            if (IsDlgButtonChecked(hWnd, CHECKBOX_SCO) == BST_CHECKED)
                            {
                                sco1 = new std::thread(updatePingIP, progressBar_sco1, text_sco1, string_sco1);
                                sco2 = new std::thread(updatePingIP, progressBar_sco2, text_sco2, string_sco2);
                                sco3 = new std::thread(updatePingIP, progressBar_sco3, text_sco3, string_sco3);
                                sco4 = new std::thread(updatePingIP, progressBar_sco4, text_sco4, string_sco4);
                            }
                            music = new std::thread(updatePingIP, progressBar_music, text_music, string_MXQ);
                            printer = new std::thread(updatePingIP, progressBar_printer, text_printer, string_print);
                            gw = new std::thread(updatePingIP, progressBar_gw, text_gw, string_gw);

                            if (competency.open == true)
                                router = new std::thread(updatePingIP, progressBar_router, text_router, string_router);

                        }
                    }

                    break;
                }
            case BUTTON_STOP:

                    active = false;
                    EnableWindow(button_run, true);
                    EnableWindow(button_stop, false);
                    EnableWindow(text_storeNumber, true);

                    EnableWindow(button_bos, false); 
                    EnableWindow(button_laptop, false);
                    EnableWindow(button_pos1, false);
                    EnableWindow(button_sco1, false); 
                    EnableWindow(button_pos2, false); 
                    EnableWindow(button_sco2, false); 
                    EnableWindow(button_pos3, false); 
                    EnableWindow(button_sco3, false); 
                    EnableWindow(button_pos4, false); 
                    EnableWindow(button_sco4, false);
                    EnableWindow(button_gw, false);
                    EnableWindow(button_music, false);
                    EnableWindow(button_printer, false);
                    EnableWindow(button_ap2, false);
                    EnableWindow(button_ap1, false);
                    EnableWindow(button_exp_bos, false);
                    EnableWindow(button_exp_laptop, false);
                    EnableWindow(button_exp_pos1, false);
                    EnableWindow(button_exp_sco1, false); 
                    EnableWindow(button_exp_pos2, false); 
                    EnableWindow(button_exp_sco2, false); 
                    EnableWindow(button_exp_pos3, false); 
                    EnableWindow(button_exp_sco3, false); 
                    EnableWindow(button_exp_pos4, false); 
                    EnableWindow(button_exp_sco4, false); 

                    SendMessage(progressBar_bos, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_laptop, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_pos1, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_pos2, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_pos3, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_pos4, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_sco1, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_sco2, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_sco3, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_sco4, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_music, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_printer, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_gw, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_ap1, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_ap2, PBM_SETPOS, 0, 0);
                    SendMessage(progressBar_router, PBM_SETPOS, 0, 0);
                    SetWindowText(text_bos, 0);
                    SetWindowText(text_laptop, 0);
                    SetWindowText(text_pos1, 0);
                    SetWindowText(text_pos2, 0);
                    SetWindowText(text_pos3, 0);
                    SetWindowText(text_pos4, 0);
                    SetWindowText(text_sco1, 0);
                    SetWindowText(text_sco2, 0);
                    SetWindowText(text_sco3, 0);
                    SetWindowText(text_sco4, 0);
                    SetWindowText(text_music, 0);
                    SetWindowText(text_printer, 0);
                    SetWindowText(text_gw, 0);
                    SetWindowText(text_ap1, 0);
                    SetWindowText(text_ap2, 0);
                    SetWindowText(text_router, 0);
                    SetWindowText(text_email, 0);

                    SetWindowText(text_phone1, 0);
                    SetWindowText(text_phone2, 0);
                    SetWindowText(text_vendor, 0);
                    SetWindowText(text_link, 0);

                    EnableWindow(checkbox_lap, false);
                    EnableWindow(checkbox_ap, false);


                    Button_SetCheck(checkbox_ap, BST_UNCHECKED);
                    Button_SetCheck(checkbox_lap, BST_UNCHECKED);

                    found = false;

                    SetFocus(text_storeNumber);
                    SendMessage(text_storeNumber, EM_SETSEL, 0, 8);
                    shopID.clear();

                break;

            case CHECKBOX_LAP:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_LAP) == BST_CHECKED))
                {
                    if (active == true)
                    {
                        int len = GetWindowTextLength(text_laptop) + 1;
                        wchar_t* text = new wchar_t[len];
                        GetWindowText(text_laptop, &string_lap[0], len);
                        laptop = new std::thread(updatePingIP, progressBar_laptop, text_laptop, string_lap);

                        EnableWindow(checkbox_lap, false);
                    }
                }
                break;

            case CHECKBOX_SCO:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_SCO) == BST_CHECKED))
                {
                    if (active == true)
                    {
                        sco1 = new std::thread(updatePingIP, progressBar_sco1, text_sco1, string_sco1);
                        sco2 = new std::thread(updatePingIP, progressBar_sco2, text_sco2, string_sco2);
                        sco3 = new std::thread(updatePingIP, progressBar_sco3, text_sco3, string_sco3);
                        sco4 = new std::thread(updatePingIP, progressBar_sco4, text_sco4, string_sco4);
                    }
                }
                break;

            case CHECKBOX_AP:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_AP) == BST_CHECKED))
                {
                    if (found == false and active == true)
                    {
                        findap = new std::thread(findAccesPoints);
                        EnableWindow(checkbox_ap, false);
                    }
                }
                break;

            case BUTTON_BOS1:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_FASTCONNECT) == BST_CHECKED))
                    runConnection(string_bos.c_str());
                else
                    runConnection(string_bos.c_str());
                break;

            case BUTTON_POS1:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_FASTCONNECT) == BST_CHECKED))
                    runConnection(string_pos1.c_str());
                else
                    runConnection(string_pos1.c_str());
                break;

            case BUTTON_POS2:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_FASTCONNECT) == BST_CHECKED))
                    runConnection(string_pos2.c_str());
                else
                    runConnection(string_pos2.c_str());
                break;

            case BUTTON_POS3:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_FASTCONNECT) == BST_CHECKED))
                    runConnection(string_pos3.c_str());
                else
                    runConnection(string_pos3.c_str());
                break;

            case BUTTON_POS4:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_FASTCONNECT) == BST_CHECKED))
                    runConnection(string_pos4.c_str());
                else
                    runConnection(string_pos4.c_str());
                break;

            case BUTTON_PRINT:
                runWebsite(shopAdress);
                break;

            case BUTTON_GW:
            {
                //SetWindowText(text_console, parameter.c_str());
                runWebsite(string_aruba);
                break;
            }

            case BUTTON_ROUTER:
            {
                std::wstring foo = shopID;
                std::wstring parameter;
                foo.erase(0, 1);
                foo.erase(0, 1);
                if (foo[0] == L'0')
                    foo.erase(0, 2);

                parameter.append(foo);

                //SetWindowText(text_console, parameter.c_str());
                ShellExecute(NULL, NULL, parameter.c_str(), NULL, NULL, SW_SHOWNORMAL);
                break;
            }

            case BUTTON_MUSIC:
            {
                std::wstring foo = shopID;
                std::wstring parameter;
                foo.erase(0, 1);
                foo.erase(0, 1);
                if (foo[0] == L'0')
                    foo.erase(0, 2);

                parameter.append(foo);

                ShellExecute(NULL, NULL, parameter.c_str(), NULL, NULL, SW_SHOWNORMAL);
                break;
            }

            case BUTTON_RESIZE:
            {
                if (extended == 0)
                {
                    std::cout << SetWindowPos(hWnd, 0, 0, 0, 940 * 1.2, 430 * 1.2, SWP_NOMOVE | SWP_NOOWNERZORDER) << std::endl;
                    SetWindowText(button_resize, L"<<");

                    extended = 1;
                }
                else
                {
                    std::cout << SetWindowPos(hWnd, 0, 0, 0, 540 * 1.2, 430 * 1.2, SWP_NOMOVE | SWP_NOOWNERZORDER) << std::endl;
                    SetWindowText(button_resize, L">>");

                    extended = 0;
                }

                break;
            }

            case BUTTON_MM_OK:

                sqlRunning = true;
                EnableWindow(button_sql_ok, false);
                EnableWindow(text_sql, false);
                EnableWindow(button_sql_abort, true);
                EnableWindow(button_sql_clear, false);

                sql = CreateThread(
                    NULL,                   // default security attributes
                    0,                      // use default stack size  
                    runSQL,       // thread function name
                    NULL,          // argument to thread function 
                    0,                      // use default creation flags 
                    NULL);   // returns the thread identifier 
                sqlDescStatus = new std::thread(updateSQLStatus);

                break;

            case BUTTON_MM_ABORT:

                sqlRunning = false;
                sqlDescStatus->~thread();

                EnableWindow(button_sql_ok, true);
                EnableWindow(text_sql, true);
                EnableWindow(button_sql_abort, false);
                EnableWindow(button_sql_clear, true);


                break;
            case BUTTON_MM_CLEAR:

                doc_in.clear();
                doc_out.clear();

                SetWindowText(text_doc_in, StringToWString(doc_in).c_str());
                SetWindowText(text_doc_out, StringToWString(doc_out).c_str());
                break;

            case BUTTON_SCO1:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_FASTCONNECT) == BST_CHECKED))
                    runConnection(string_sco1.c_str());
                else
                    runConnection(string_sco1.c_str());
                break;

            case BUTTON_SCO2:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_FASTCONNECT) == BST_CHECKED))
                    runConnection(string_sco2.c_str());
                else
                    runConnection(string_sco2.c_str());
                break;

            case BUTTON_SCO3:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_FASTCONNECT) == BST_CHECKED))
                    runConnection(string_sco3.c_str());
                else
                    runConnection(string_sco3.c_str());
                break;

            case BUTTON_SCO4:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_FASTCONNECT) == BST_CHECKED))
                    runConnection(string_sco4.c_str());
                else
                    runConnection(string_sco4.c_str());
                break;

            case BUTTON_LAP:
                if ((IsDlgButtonChecked(hWnd, CHECKBOX_FASTCONNECT) == BST_CHECKED))
                {
                    int len = GetWindowTextLength(text_laptop) + 1;
                    wchar_t* text = new wchar_t[len];
                    GetWindowText(text_laptop, &string_lap[0], len);
                    runConnection(string_lap.c_str());
                }
                else
                {
                    int len = GetWindowTextLength(text_laptop) + 1;
                    wchar_t* text = new wchar_t[len];
                    GetWindowText(text_laptop, &string_lap[0], len);
                    runConnection(string_lap.c_str());
                }
                break;

            case BUTTON_BOS1E:
                runExplorer(L"BOS01", string_bos.c_str());
                break;

            case BUTTON_POS1E:
                runExplorer(L"POS01", string_pos1.c_str());
                break;

            case BUTTON_POS2E:
                runExplorer(L"POS02", string_pos2.c_str());
                break;

            case BUTTON_POS3E:
                runExplorer(L"POS03", string_pos3.c_str());
                break;

            case BUTTON_POS4E:
                runExplorer(L"POS04", string_pos4.c_str());
                break;

            case BUTTON_SCO1E:
                runExplorer(L"SCO01", string_sco1.c_str());
                break;

            case BUTTON_SCO2E:
                runExplorer(L"SCO02", string_sco2.c_str());
                break;

            case BUTTON_SCO3E:
                runExplorer(L"SCO03", string_sco3.c_str());
                break;

            case BUTTON_SCO4E:
                runExplorer(L"SCO04", string_sco4.c_str());
                break;

            case IDM_ABOUT:
            {
                std::wstring foo;

                if (competency.dane8[index] == L"")
                    foo = L"Brak w bazie";
                else
                {
                    foo = competency.dane8[index];
                    std::replace(foo.begin(), foo.end(), L'^', L'\n');
                }

                MessageBox(hWnd, foo.c_str(), L"Descryption", MB_ICONINFORMATION);

                break;
            }
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

            updateWindowSize(hWnd);

            ReleaseDC(hWnd, hdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK Tools(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        
        break;
    }
    return (INT_PTR)FALSE;
}

std::wstring ExecCmd(wchar_t* cmd)
{
    std::wstring strResult;
    HANDLE hPipeRead, hPipeWrite;

    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hPipeRead, &hPipeWrite, &saAttr, 0))
        return strResult;

    STARTUPINFOW si = { sizeof(STARTUPINFOW) };
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.hStdOutput = hPipeWrite;
    si.hStdError = hPipeWrite;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi = { 0 };

    BOOL fSuccess = CreateProcessW(NULL, cmd, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    if (!fSuccess)
    {
        CloseHandle(hPipeWrite);
        CloseHandle(hPipeRead);
        return strResult;
    }

    bool bProcessEnded = false;
    for (; !bProcessEnded;)
    {
        bProcessEnded = WaitForSingleObject(pi.hProcess, 50) == WAIT_OBJECT_0;
        
        for (;;)
        {
            char buf[1024];
            DWORD dwRead = 0;
            DWORD dwAvail = 0;

            if (!::PeekNamedPipe(hPipeRead, NULL, 0, NULL, &dwAvail, NULL))
                break;

            if (!dwAvail)
                break;

            if (!::ReadFile(hPipeRead, buf, min(sizeof(buf) - 1, dwAvail), &dwRead, NULL) || !dwRead)
                break;

            buf[dwRead] = 0;

            size_t newsize = strlen(buf) + 1;

            wchar_t* tmp = new wchar_t[newsize];

            size_t convertedChars = 0;
            mbstowcs_s(&convertedChars, tmp, newsize, buf, _TRUNCATE);

            strResult.append(tmp);
        }
    }

    CloseHandle(hPipeWrite);
    CloseHandle(hPipeRead);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return strResult;
}

void updateWindowSize(HWND hWnd)
{
    RECT tmp;
    LPRECT lpRect = &tmp;
    GetWindowRect(hWnd, lpRect);

    window_width = lpRect->right - lpRect->left;
    window_height = lpRect->bottom - lpRect->top;
}

void runConnection(std::wstring ip)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::wstring parameter = L"Prgogram ";
    parameter.append(ip);

    CreateProcess(L"Prgogram",   // the path
        (LPWSTR)parameter.c_str(),
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void runWebsite(std::wstring adress)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::wstring parameter = L"explorer http://";
    parameter.append(adress);

    CreateProcess(L"C:/Windows/explorer.exe",   // the path
        (LPWSTR)parameter.c_str(),
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void runExplorer(std::wstring machine, std::wstring ip)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    std::wstring path;
    std::wstring parameter;

    path = L"Ścieżka do programu";

    CreateProcess(path.c_str(),   // the path
        (LPWSTR)parameter.c_str(),
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void updatePingIP(HWND id, HWND textId, std::wstring IP)
{
    while (active)
    {
        std::wstring cmd = L"ping ";
        cmd.append(IP);
        cmd.append(L" /n 1");

        std::wstring compare = L"Ping request could not find host ";
        compare.append(IP);
        compare.append(L". Please check the name and try again.\r\n");

        std::wstring pingStatus = ExecCmd((wchar_t*)cmd.c_str());

        if (pingStatus == compare)
        {
            SendMessage(id, PBM_STEPIT, 0, 0);
            SendMessage(id, (UINT)PBM_SETBARCOLOR, 0, (LPARAM)RGB(255, 60, 60));

            SetWindowText(textId, L"Brak w bazie");
        }
        else
        {
            pingStatus.erase(0, pingStatus.find(L"\n") + 1);
            pingStatus.erase(0, pingStatus.find(L"\n") + 1);

            pingStatus.erase(pingStatus.begin() + pingStatus.find(L"\n") + 1, pingStatus.end());


            if (pingStatus[2] == L'q')
            {
                SendMessage(id, PBM_STEPIT, 0, 0);
                SendMessage(id, (UINT)PBM_SETBARCOLOR, 0, (LPARAM)RGB(255, 60, 60));
            }
            else if (pingStatus[0] == L'D')
            {
                SendMessage(id, PBM_STEPIT, 0, 0);
                SendMessage(id, (UINT)PBM_SETBARCOLOR, 0, (LPARAM)RGB(255, 60, 60));
            }

            else
            {
                if (pingStatus[0] == L'H')
                {
                    SendMessage(id, PBM_STEPIT, 0, 0);
                    SendMessage(id, (UINT)PBM_SETBARCOLOR, 0, (LPARAM)RGB(255, 60, 60));
                }
                else
                {
                    pingStatus.erase(0, pingStatus.find(L":") + 2);
                    if (pingStatus[0] == L'D')
                    {
                        SendMessage(id, PBM_STEPIT, 0, 0);
                        SendMessage(id, (UINT)PBM_SETBARCOLOR, 0, (LPARAM)RGB(255, 60, 60));
                    }
                    else if (pingStatus[0] == L'I')
                    {
                        SendMessage(id, PBM_STEPIT, 0, 0);
                        SendMessage(id, (UINT)PBM_SETBARCOLOR, 0, (LPARAM)RGB(255, 60, 60));
                    }
                    else if (pingStatus[0] == L'U' or pingStatus[0] == '\n')
                    {
                        SendMessage(id, PBM_STEPIT, 0, 0);
                        SendMessage(id, (UINT)PBM_SETBARCOLOR, 0, (LPARAM)RGB(255, 60, 60));
                    }
                    else
                    {
                        SendMessage(id, PBM_STEPIT, 0, 0);
                        SendMessage(id, (UINT)PBM_SETBARCOLOR, 0, (LPARAM)RGB(60, 60, 255));
                        
                        //SetWindowText(text_console, (LPWSTR)pingStatus.c_str());
                    }
                }
                Sleep(1500);
            }
        }
    }

    SendMessage(id, PBM_SETPOS, 0, 0);
    SetWindowText(textId, 0);
    return;
}

void readCompetency()
{
    std::wifstream file;

    std::wstring text;

    if (!file.is_open()) 
        MessageBox(hWnd, L"Brak pliku, kontyunowanie bez", L"Brak pliku competency.csv", MB_ICONINFORMATION);
    else
    {
        competency.open = true;
        std::getline(file, text);

        while (!file.eof())
        {
            std::getline(file, text);
        }
    }

    file.close();
}

void read()
{

    std::wifstream file;

    std::wstring text;

    if (!file.is_open())
        MessageBox(hWnd, L"Brak pliku, kontyunowanie bez", L"Brak pliku dane.csv", MB_ICONINFORMATION);
    else
    {
        com.open = true;
        std::getline(file, text);

        while (!file.eof())
        {
            std::getline(file, text);

        }
    }
    file.close();
}


void CreateConsole()
{
    if (!AllocConsole()) {
        // Add some error handling here.
        // You can call GetLastError() to get more info about the error.
        return;
    }

    // std::cout, std::clog, std::cerr, std::cin
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    // std::wcout, std::wclog, std::wcerr, std::wcin
    HANDLE hConOut = CreateFile(_T("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hConIn = CreateFile(_T("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
    SetStdHandle(STD_ERROR_HANDLE, hConOut);
    SetStdHandle(STD_INPUT_HANDLE, hConIn);
    std::wcout.clear();
    std::wclog.clear();
    std::wcerr.clear();
    std::wcin.clear();
}

void findAccesPoints()
{
    short counter = 0;

    for (int i = 5; i <= 20; i++)
    {
        std::wstring ip = shopIP;
        ip.append(std::to_wstring(i));

        std::wstring cmd = L"ping ";
        cmd.append(ip);
        cmd.append(L" /n 1 /w 50");
        std::wstring foo = ExecCmd((wchar_t*)cmd.c_str());

        foo.erase(0, foo.find(L"\n") + 1);

        if (foo.find(L"Reply") != std::wstring::npos)
        {
            if (counter == 0)
            {
                counter++;
                device1 = ip;

                if (active == true)
                {
                    found = true;

                    ap1 = new std::thread(updatePingIP, progressBar_ap1, text_ap1, device1);
                    device1.erase(0, device1.find_last_of(L'.') + 1);
                    SetWindowText(text_ap1, device1.c_str());
                }
            }
            else if(counter == 1)
            {
                counter++;
                deivce2 = ip;

                if (active == true)
                {
                    ap2 = new std::thread(updatePingIP, progressBar_ap2, text_ap2, deivce2);
                    deivce2.erase(0, deivce2.find_last_of(L'.') + 1);
                    SetWindowText(text_ap2, deivce2.c_str());
                }

                break;
            }
        }
    }

    if (counter == 0)
    {
        SetWindowText(text_ap1, L"Brak");
        SetWindowText(text_ap2, L"Brak");
        device1 = L"Brak";
        deivce2 = L"Brak";

        ap1 = new std::thread(updatePingIP, progressBar_ap1, text_ap1, device1);
        ap2 = new std::thread(updatePingIP, progressBar_ap2, text_ap2, deivce2);
    }
    else if (counter == 1)
    {
        SetWindowText(text_ap2, L"Brak");
        deivce2 = L"Brak";

        ap2 = new std::thread(updatePingIP, progressBar_ap2, text_ap2, deivce2);
    }

    return;
}

std::wstring StringToWString(const std::string& str)
{
    std::wstring wstr;
    size_t size;
    wstr.resize(str.length());
    mbstowcs_s(&size, &wstr[0], wstr.size() + 1, str.c_str(), str.size());
    return wstr;
}

std::string WStringToString(const std::wstring& wstr)
{
    std::string str;
    size_t size;
    str.resize(wstr.length());
    wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
    return str;
}

void select()
{
    int len = GetWindowTextLength(text_sql) + 1;
    wchar_t* text = new wchar_t[len];
    GetWindowText(text_sql, &text[0], len);

    std::wstring wsql(text);
    std::string sql = WStringToString(wsql);

    for (int j = 0; j < 6; j++)
    {
        otl_stream i(512,
            "SELECT * FROM base",
            db[j]
        );

        otl_stream i2(512,
            "SELECT * FROM base",
            db[j]
        );


        i << sql.c_str();
        i2 << sql.c_str();

        int data1;
        char data2[30];
        otl_datetime data3;
        char data4[255];
        short int prc_status;

        

        while (!i.eof()) 
        {
            i >> data2;
            doc_in.append(data2); doc_in.append(" | ");

            i >> data3;
            doc_in.append(std::to_string(data3.year)); doc_in.append(".");  doc_in.append(std::to_string(data3.month)); doc_in.append(".");  doc_in.append(std::to_string(data3.day)); doc_in.append(" "); doc_in.append(std::to_string(data3.hour)); doc_in.append(":"); doc_in.append(std::to_string(data3.minute)); doc_in.append(" | ");     // created data time

            i >> data1;
            doc_in.append(std::to_string(data1)); doc_in.append(" | ");

            i >> data4;
            doc_in.append(data4); doc_in.append(" | ");

            i >> prc_status;
            doc_in.append(std::to_string(prc_status)); doc_in.append("\n");

        }

        while (!i2.eof()) 
        {
            i2 >> data2;
            doc_out.append(data2); doc_out.append(" | ");

            i2 >> data3;
            doc_out.append(std::to_string(data3.year)); doc_out.append(".");  doc_out.append(std::to_string(data3.month)); doc_out.append(".");  doc_out.append(std::to_string(data3.day)); doc_out.append(" "); doc_out.append(std::to_string(data3.hour)); doc_out.append(":"); doc_out.append(std::to_string(data3.minute)); doc_out.append(" | ");     // created data time

            i2 >> data1;
            doc_out.append(std::to_string(data1)); doc_out.append(" | ");

            i2 >> data4;
            doc_out.append(data4); doc_out.append(" | ");

            i2 >> prc_status;
            doc_out.append(std::to_string(prc_status)); doc_out.append("\n");

        }
    }
}

DWORD WINAPI runSQL(LPVOID lpParam)
{
    otl_connect::otl_initialize();
    try
    {
        db[0].rlogon("Driver={ODBC Driver 17 for SQL Server}; Server=nazwa serwera;"); // connect to ODBC
        db[1].rlogon("Driver={ODBC Driver 17 for SQL Server}; Server=nazwa serwera;"); // connect to ODBC
        db[2].rlogon("Driver={ODBC Driver 17 for SQL Server}; Server=nazwa serwera;"); // connect to ODBC
        db[3].rlogon("Driver={ODBC Driver 17 for SQL Server}; Server=nazwa serwera;"); // connect to ODBC
        db[4].rlogon("Driver={ODBC Driver 17 for SQL Server}; Server=nazwa serwera;"); // connect to ODBC
        db[5].rlogon("Driver={ODBC Driver 17 for SQL Server}; Server=nazwa serwera;"); // connect to ODBC

        select();

    }
    catch (otl_exception& p)
    { // intercept OTL exceptions
        sqlRunning = false;
        SetWindowText(desc_sql_status, L"Failed");

        std::cout << p.msg << std::endl; // print out error message
        std::cout << p.code << std::endl; // print out error code
        std::cout << p.var_info << std::endl; // print out the variable that caused the error
        std::cout << p.sqlstate << std::endl; // print out SQLSTATE message
        std::cout << p.stm_text << std::endl; // print out SQL that caused the error
    }

    SetWindowText(text_doc_in, StringToWString(doc_in).c_str());
    SetWindowText(text_doc_out, StringToWString(doc_out).c_str());

    db[0].logoff(); // disconnect from the data source
    db[1].logoff(); // disconnect from the data source
    db[2].logoff(); // disconnect from the data source
    db[3].logoff(); // disconnect from the data source
    db[4].logoff(); // disconnect from the data source
    db[5].logoff(); // disconnect from the data source

    sqlRunning = false;
    SetWindowText(desc_sql_status, L"Finished");

    EnableWindow(button_sql_ok, true);
    EnableWindow(text_sql, true);
    EnableWindow(button_sql_abort, false);
    EnableWindow(button_sql_clear, true);

    return 0;
}

void updateSQLStatus()
{
    while (sqlRunning == true)
    {
        SetWindowText(desc_sql_status, sqlStatus.c_str());

        if (sqlStatus == L"executing")
        {
            sqlStatus = L"executing.";
            SetWindowText(desc_sql_status, sqlStatus.c_str());
        }
        else if (sqlStatus == L"executing.")
        {
            sqlStatus = L"executing..";
            SetWindowText(desc_sql_status, sqlStatus.c_str());
        }
        else if (sqlStatus == L"executing..")
        {
            sqlStatus = L"executing...";
            SetWindowText(desc_sql_status, sqlStatus.c_str());
        }
        else if (sqlStatus == L"executing...")
        {
            sqlStatus = L"executing";
            SetWindowText(desc_sql_status, sqlStatus.c_str());
        }
        Sleep(1000);
    }


}