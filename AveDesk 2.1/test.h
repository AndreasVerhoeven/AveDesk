#ifndef TEST_H_201010
#define TEST_H_201010

LONG CALLBACK WinProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
LONG CALLBACK LabelProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);

// function prototypes
int CALLBACK DockletPropertiesDlgProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam);
//old int CALLBACK CurrentDockletsDlgProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK AddDockletProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK LabelConfigDlgProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam);
docklets::OdDockletInstance* LoadDocklet(char* filename, char* iniFile, char* iniGroup, bool makeVisible=true);
//int CALLBACK GlobalSettingsDlgProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam);
int CALLBACK AboutDlgProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam);

INT CALLBACK DockletAppearanceSheetDlgProc(HWND hWnd,UINT msg,WPARAM wParam, LPARAM lParam);
INT CALLBACK DockletLabelSheetDlgProc(HWND hWnd,UINT msg,WPARAM wParam, LPARAM lParam);
INT CALLBACK DockletAdvancedSheetDlgProc(HWND hWnd,UINT msg,WPARAM wParam, LPARAM lParam);
INT CALLBACK WrapperProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
void MakeIcon();
void KillIcon();

HINSTANCE GetThisInstance();
#endif//TEST_H_20101