#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <resource.h>
#include <fileutils.h>

// Declare the function that responds to events
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"NotePad Class";

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // Big icon (title bar & taskbar)
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);   // Default mouse arrow
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Default background color
    wc.lpszMenuName = NULL;


    // Register the window
    RegisterClassEx(&wc);

    // create a menu with file sub menu that contains the Open,Save and Exit options
    HMENU hMenu = CreateMenu();
    HMENU hFileMenu = CreatePopupMenu(); // file submenu

    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFileMenu,L"File"); // add "file" sub menu to the main menu
    
    AppendMenu(hFileMenu,MF_STRING,ID_FILE_OPEN,L"Open"); // add "open" menu item to the file sub menu
    AppendMenu(hFileMenu,MF_STRING,ID_FILE_SAVE,L"Save"); // add "save" menu item to the file sub menu
    AppendMenu(hFileMenu,MF_STRING,ID_FILE_EXIT,L"Exit"); // add "exit" menu item to the file sub menu

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                // Optional window styles
        CLASS_NAME,       // Window class
        L"NotePad App",   // Window text
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, 
        hMenu,     // pass the main menu with the sub menu created to the window 
        hInstance, 
        NULL
    );

    if (hwnd == NULL)
    {
        return 0; // Window creation failed
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Message handler
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndEdit;

    switch (uMsg) // handling messages in order in crucial !
    {
        
        case WM_CREATE:  // needs to be handled before you try to size the control in WM_SIZE.
        {
            hwndEdit = CreateWindowEx(
                0,          // no window styles 
                L"EDIT",    // control name for a text edit box
                NULL,       // no initial text
                WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
                                // Window/control styles:
                                // WS_BORDER --> thin border around the control.
                                // WS_CHILD --> child of the main window.
                                // WS_VISIBLE --> Make it visible
                                // WS_VSCROLL -> vertical scroll bar.
                                // ES_LEFT --> Left-align the text inside the edit box.
                                // ES_MULTILINE --> Allow multiple lines of text.
                                // ES_AUTOVSCROLL --> Scroll automatically when text exceeds height.
                0, 0, 0, 0,   // x-position, y-position, width, height later set in WM_SIZE
                hwnd,        // Parent window handle
                (HMENU)1,   // Control ID
                ((LPCREATESTRUCT)lParam)->hInstance,
                                // Application instance handle (from WM_CREATE lParam).
                NULL
            );
            return 0;
        }

        case WM_SIZE:
        {
            MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
            return 0;
        }

        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case ID_FILE_OPEN:
                {
                    OPENFILENAME ofn; // holds the configuration for the dialog that will be opened.
                    wchar_t szFile[MAX_PATH]; // array which holds the path of the file.
                
                    ZeroMemory(&ofn, sizeof(ofn)); // this struct should be initialized with zeros to avoid garbage values
                
                    ofn.lStructSize = sizeof(OPENFILENAME);     // Size of the struct
                    ofn.hwndOwner = hwnd;                      // Handle to parent window
                    ofn.lpstrFile = szFile;                   // array which holds the path of the file.
                    ofn.lpstrFile[0] = '\0';                 // Initialize array with empty string
                    ofn.nMaxFile = MAX_PATH;                // Size of the array
                    
                    if(GetOpenFileName(&ofn))
                    {
                        // read the content from file to open
                        std::wstring fileContent = ReadFileFromPath(ofn.lpstrFile);

                        //change the text displayed by a control/window when file is loaded.
                        SetWindowText(hwndEdit,fileContent.c_str());
                    }
                    return 0;
                }

                case ID_FILE_SAVE:
                {
                    OPENFILENAME sv; // holds the configuration for the dialog that will be opened.
                    wchar_t szFile[10*MAX_PATH]; // array which holds the path of the file.

                    ZeroMemory(&sv, sizeof(sv));  // this struct should be initialized with zeros to avoid garbage values
                    
                    sv.lStructSize = sizeof(OPENFILENAME);     // Size of the struct
                    sv.hwndOwner = hwnd;                      // Handle to parent window
                    sv.lpstrFile = szFile;                   // array which holds the path of the file.
                    sv.lpstrFile[0] = '\0';                 // Initialize array with empty string
                    sv.nMaxFile = sizeof(szFile) / sizeof(wchar_t);                // Size of the array
                    
                    if(GetSaveFileName(&sv))
                    {
                        
                        //buffer to hold the text 
                        wchar_t buffer[12*MAX_PATH];

                        //Get the text displayed by a control/window.
                        int stringLength = GetWindowText(hwndEdit,buffer,12*MAX_PATH);

                        // save the content to a file
                        SaveFileToPath(sv.lpstrFile,buffer);
                    }
                    return 0;
                }

                case ID_FILE_EXIT:
                {
                    PostQuitMessage(0);
                    return 0;
                }
            }
        }
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps;                         // holds painting information
            HDC hdc = BeginPaint(hwnd, &ps);       // Prepare window for painting
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));  // Fill the repaint area with the default window background color
                                                                    // ie just clear the background by filling it with its color
            EndPaint(hwnd, &ps);                   // validate painted region
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);  // exit message loop and shut down
            return 0;

        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
