#define _WIN32_IE 0x501

#define _WIN32_IE 0x701
#define _WIN32_WINNT 0x0501
#ifdef DISTRIBUTION_EXPORTS
#define DISTRIBUTION_API __declspec(dllexport)
#else
#define DISTRIBUTION_API __declspec(dllimport)
#endif

#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <wingdi.h>
#include <shlwapi.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "resource.h"

#include "GenerateurEAN.h"
#pragma comment (lib,"comctl32")
#pragma comment (lib,"ddraw.lib")
#pragma comment (lib,"Gdi32.lib")
#pragma comment (lib,"Comdlg32.lib")
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"Lz32.lib")
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment (lib,"distribution.lib")


extern"C"{
	DISTRIBUTION_API void SetBits(char** lppData, int* lpPos, WORD wCode, int len);
	DISTRIBUTION_API int Decode(const char* pszCode);
	DISTRIBUTION_API int GetApproximateStep(int width);
	DISTRIBUTION_API void errhandler(LPCTSTR Texte, HWND hwnd);
	DISTRIBUTION_API void SetResultCode(HWND hwnd);
	DISTRIBUTION_API BOOL PreparePrint(HDC hdc, POINTFLOAT ptf, LPRECT lprc);
	DISTRIBUTION_API const char* ErrorString(int error);
	DISTRIBUTION_API PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
	DISTRIBUTION_API void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
	DISTRIBUTION_API int GetMinimalHeight(HDC hdc, int step);
	DISTRIBUTION_API int GetAppLastError();
	DISTRIBUTION_API const char* GetCodeString();
	DISTRIBUTION_API void Draw(HDC hdc, POINT pt, int height, int step);
	DISTRIBUTION_API int SetIndex(char car);
	DISTRIBUTION_API int GetApproximateWidth(int step);
	DISTRIBUTION_API int CheckDigit(const char* pszCode, BOOL bComplet);
	DISTRIBUTION_API char* GetDLLVersionC();
	DISTRIBUTION_API int MsgBox(HINSTANCE hInst, HWND hDlg, char* lpszText, char* lpszCaption, DWORD dwStyle, int lpszIcon);
	DISTRIBUTION_API char* GetDLLVersion();
};
char       szClassName[] = "CodeBarreWindowsApp";
char       g_szWndTitle[] = "Code Barre 2.0 ©Patrice Waechter-Ebling 2019-2020 | Code: EAN 128";
static int m_iError, m_nbBits, m_bitSize, m_step, m_height;

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	HDC hStatic = (HDC)wParam;
    switch (message)    {
	case WM_CREATE:{
        LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
        SetWindowLong(hwnd, -21,(LONG)lpcs->lpCreateParams);
			   SendMessage(hwnd, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
            }break;
        
        case WM_DESTROY:{
            PostQuitMessage (0);
            }break;
       case WM_COMMAND:{
		char szBuf[256];
		int digit;
			switch(LOWORD(wParam))			{
				case IDC_DECODE:{
						GetDlgItemText(hwnd, IDC_EDIT, szBuf, 255);
					m_bitSize = Decode(szBuf);
					InvalidateRect(GetDlgItem(hwnd, IDC_IMAGE), NULL, TRUE);
					EnableWindow(GetDlgItem(hwnd, IDC_BINARY), (BOOL)m_bitSize);
					for(int i=IDC_PRINT; i<=IDC_VZOOMDOWN; i++)
						EnableWindow(GetDlgItem(hwnd, i),(BOOL)m_bitSize );
					SetResultCode(hwnd);
					if(!m_bitSize)
						MessageBox(hwnd,ErrorString(GetAppLastError()),g_szWndTitle, 64);            
						}break;
				case IDC_BINARY:
					GetDlgItemText(hwnd, IDC_BINARY, szBuf, 64);
					if(!lstrcmp(szBuf, TEXT_BTN1))
						SetDlgItemText(hwnd, IDC_BINARY, TEXT_BTN2);
					else
						SetDlgItemText(hwnd, IDC_BINARY, TEXT_BTN1);
					SetResultCode(hwnd);
					break;
				case IDC_CHECKDIGIT:
					GetDlgItemText(hwnd, IDC_EDIT, szBuf, 255);
					digit = CheckDigit(szBuf, FALSE);
					wsprintf(szBuf, "Le checkdigit est %i", digit);
					MessageBox(hwnd, szBuf, g_szWndTitle, 64);
					break;
				case IDC_HZOOMUP:
					m_step = min(4, m_step+1);
					InvalidateRect(GetDlgItem(hwnd, IDC_IMAGE), NULL, TRUE);
					if(!IsWindowEnabled(GetDlgItem(hwnd, IDC_COPY)))
						EnableWindow(GetDlgItem(hwnd, IDC_COPY), TRUE);
						EnableWindow(GetDlgItem(hwnd, IDC_FILEHELP), TRUE);
					break;
				case IDC_HZOOMDOWN:
					m_step = max(1, m_step-1);
					InvalidateRect(GetDlgItem(hwnd, IDC_IMAGE), NULL, TRUE);
					if(!IsWindowEnabled(GetDlgItem(hwnd, IDC_COPY)))
						EnableWindow(GetDlgItem(hwnd, IDC_COPY), TRUE);
						EnableWindow(GetDlgItem(hwnd, IDC_FILEHELP), TRUE);
					break;
				case IDC_VZOOMUP:
					m_height = min(140, m_height+5);
					InvalidateRect(GetDlgItem(hwnd, IDC_IMAGE), NULL, TRUE);
					if(!IsWindowEnabled(GetDlgItem(hwnd, IDC_COPY)))
						EnableWindow(GetDlgItem(hwnd, IDC_COPY), TRUE);
						EnableWindow(GetDlgItem(hwnd, IDC_FILEHELP), TRUE);
					break;
				case IDC_VZOOMDOWN:
					HDC hdc;
					hdc = GetWindowDC(hwnd);
					m_height = max(GetMinimalHeight(hdc, m_step), m_height-5);
					ReleaseDC(hwnd, hdc);
					InvalidateRect(GetDlgItem(hwnd, IDC_IMAGE), NULL, TRUE);
					if(!IsWindowEnabled(GetDlgItem(hwnd, IDC_COPY)))
						EnableWindow(GetDlgItem(hwnd, IDC_FILEHELP), TRUE);
					break;        
				case IDC_PRINT:{
					CDlgPrintSetup * pDlg;    
					pDlg = new CDlgPrintSetup(&m_rcPrint);
					if(!pDlg)        return 2;
					if(!pDlg->Create(hwnd))    {delete pDlg; return 2;}
					delete pDlg;
					PRINTDLG prd;
					DOCINFO di;
					HDC hdcPrn;
					POINT ptLog;
					POINTFLOAT ptFloat; // nbre de pixel pour 1mm
					BOOL bOk, bEnd;
					ZeroMemory(&di, sizeof(DOCINFO));
					di.cbSize = sizeof(DOCINFO);
					di.lpszDocName = "Impression Code barre";
					ZeroMemory(&prd, sizeof(PRINTDLG));
					prd.lStructSize = sizeof(PRINTDLG);
					prd.hwndOwner = hwnd;
					prd.Flags = PD_RETURNDC|PD_NOPAGENUMS|PD_NOSELECTION|PD_HIDEPRINTTOFILE;
					if(!PrintDlg(&prd))        return 1;
					if((hdcPrn=prd.hDC)==NULL)        return 1;
					ptLog.x = GetDeviceCaps(hdcPrn, LOGPIXELSX);
					ptLog.y = GetDeviceCaps(hdcPrn, LOGPIXELSY);
					ptFloat.x = (float)ptLog.x/25.4f;
					ptFloat.y = (float)ptLog.y/25.4f;
					bOk = StartDoc(hdcPrn, &di);
					SetMapMode(hdcPrn, MM_TEXT);
					SetBkMode(hdcPrn, TRANSPARENT);
					bEnd = FALSE;
					while((bOk)&&(!bEnd))    {
						StartPage(hdcPrn);        
						bEnd = PreparePrint(hdcPrn, ptFloat, &m_rcPrint);
						bEnd = TRUE;
						bOk = EndPage(hdcPrn);
					}
					if(bOk) EndDoc(hdcPrn);
					else
						AbortDoc(hdcPrn);
					return DeleteDC(hdcPrn);
							   }break;
				case IDC_COPY:{
					HWND hMain;
					HBITMAP hBmp;
					HDC     hMem, hdc;
					POINT   pt;
					RECT    rc;    
					hMain = GetParent(GetParent(hwnd));
					if(!OpenClipboard(hMain))          return 3;
					EmptyClipboard();
					hdc = GetDC(hMain);
					hBmp = CreateCompatibleBitmap(hdc, GetApproximateWidth(m_step), m_height);
					if(!hBmp)  {        CloseClipboard();        return 4;    }
					hMem = CreateCompatibleDC(hdc);
					if(!hMem)    {        CloseClipboard();        DeleteObject(hBmp);        return 5;    }
					pt.x = pt.y = 0;
					rc.left = rc.top = 0;
					rc.right = GetApproximateWidth(m_step);
					rc.bottom = m_height;
					SelectObject(hMem, hBmp);
					FillRect(hMem, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
					Draw(hMem, pt, m_height, m_step);
					SetClipboardData(CF_BITMAP, hBmp);
					DeleteDC(hMem);
					ReleaseDC(hMain, hdc);    
					DeleteObject(hBmp);
					CloseClipboard();
					EnableWindow(GetDlgItem(hwnd, IDC_COPY), FALSE);
							  }break;
				case IDC_FILEHELP:{
					HWND hMain;
					HBITMAP hBmp;
					HDC     hMem, hdc;
					POINT   pt;
					RECT    rc;    
					GetDlgItemText(hwnd,IDC_EDIT,buffer,128);
					hMain = GetParent(GetParent(hwnd));
					if(!OpenClipboard(hMain))          return 3;
					EmptyClipboard();
					hdc = GetDC(hMain);
					hBmp = CreateCompatibleBitmap(hdc, GetApproximateWidth(m_step), m_height);
					if(!hBmp)  {        CloseClipboard();        return 4;    }
					hMem = CreateCompatibleDC(hdc);
					if(!hMem)    {        CloseClipboard();        DeleteObject(hBmp);        return 5;    }
					pt.x = pt.y = 0;
					rc.left = rc.top = 0;
					rc.right = GetApproximateWidth(m_step);
					rc.bottom = m_height;
					SelectObject(hMem, hBmp);
					FillRect(hMem, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
					Draw(hMem, pt, m_height, m_step);
					SetClipboardData(CF_BITMAP, hBmp);
					char szTmp[260];
					char buffer2[260];
					wsprintf(szTmp,"%s\\%s.BMP",szApp,buffer);
					wsprintf(buffer2,"Voulez vous sauvegarder le codebarre graphique pour %s sous: \n%s ?",buffer,szTmp);
					if(MsgBox(wincl.hInstance,hwnd,buffer2,"Sauvegarde Graphique",MB_YESNO|MB_DEFBUTTON2,103)==0x06){CreateBMPFile(hMain,szTmp , CreateBitmapInfoStruct(hMain,hBmp),hBmp,hMem);}
					DeleteDC(hMem);
					ReleaseDC(hMain, hdc);    
					DeleteObject(hBmp);
					EmptyClipboard();
					CloseClipboard();
								  }break;
				case IDC_ABOUTHELP:{
					MSGBOXPARAMS  mbp;    
					mbp.cbSize      = sizeof(MSGBOXPARAMS);
					mbp.dwStyle     = MB_USERICON;
					mbp.hInstance   = wincl.hInstance;
					mbp.hwndOwner   = hwnd;
					mbp.lpszIcon    = MAKEINTRESOURCE(131);
					mbp.lpszCaption = " A propos de...";
					char buf[1024];
					wsprintf(buf,"\tCode barre\tVersion 2.00 \n\n\tNoyau OLEGDI+: v:%s \n\n    ©Patrice Waechter-Ebling 2019-2020\n\nArchives=%s\nRapports=%s\nReferentiel=%s",GetDLLVersion(),tdatas2.ArchivePath,tdatas2.RapportPath,tdatas2.ReferentielFileName);

					mbp.lpszText    = buf;
				   return  MessageBoxIndirect(&mbp);
								   }break;
				case IDC_FILECLOSE:
					PostMessage((hwnd), WM_CLOSE, 0, 0);
					break;
			}
					   }return 0;
		case WM_DRAWITEM:{    
			LPDRAWITEMSTRUCT lpdis;
			POINT pt;	
			lpdis = (LPDRAWITEMSTRUCT)lParam;
			if(lpdis->CtlID!=IDC_IMAGE)	    return 0;
			FillRect(lpdis->hDC, &lpdis->rcItem, CreateSolidBrush(RGB(0,128,0)));
			SetBkMode(lpdis->hDC, TRANSPARENT);
			pt.x = (lpdis->rcItem.right - GetApproximateWidth(m_step))/2;
			pt.y = (lpdis->rcItem.bottom - m_height)/2;
			Draw(lpdis->hDC, pt, m_height, m_step);/**/
						 }return 0;
		case WM_TIMER:{GetLocalTime(&st);wsprintf(buffer,"Nous sommes %s, le %2d %s %4d il est %.2d:%.2d:%.2d",jours[st.wDayOfWeek],st.wDay,mois[st.wMonth-1],st.wYear,st.wHour,st.wMinute,st.wSecond);SetDlgItemText(hwnd,6000,buffer);}break;
		case WM_CTLCOLORSTATIC:{SetTextColor( hStatic, RGB(128,128, 0xFF) );SetBkMode( hStatic, TRANSPARENT ); }return (LONG)wincl.hbrBackground;
		case WM_CTLCOLOREDIT:{SetTextColor( hStatic, RGB(120,0,0xAA ) );SetBkMode( hStatic, TRANSPARENT ); }return (LONG)wincl.hbrBackground;
		case WM_CTLCOLORLISTBOX:{SetTextColor( hStatic, RGB(0,255, 255) );SetBkMode( hStatic, TRANSPARENT ); }return (LONG)wincl.hbrBackground;
		case WM_CTLCOLORBTN:{SetTextColor( hStatic, RGB(0,255, 255) );	SetBkMode( hStatic, TRANSPARENT ); }return (LONG)wincl.hbrBackground;
		case WM_CTLCOLORMSGBOX:	return (long)wincl.hbrBackground;
			break;
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nFunsterStil){
    HWND hwnd;
    MSG messages;
    InitCommonControls();
    wincl.hInstance = hInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);
    wincl.hIcon = LoadIcon(wincl.hInstance, MAKEINTRESOURCE(IDI_APPMAIN));
    wincl.hIconSm = LoadIcon(wincl.hInstance, MAKEINTRESOURCE(IDI_APPMAIN));
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH) CreateSolidBrush(RGB(192,196,255));
   	GetCurrentDirectory(256,tdatas2.appPath);
	wsprintf(tdatas2.RapportPath,"%s\\Datas\\Transfo\\Rapports\\",tdatas2.appPath);
	wsprintf(tdatas2.ArchivePath,"%s\\Datas\\Transfo\\Archives\\",tdatas2.appPath);
	wsprintf(tdatas2.ReferentielFileName,"%s\\Datas\\Transformateurs.ref",tdatas2.appPath);
 
    if(!RegisterClassEx (&wincl))  return 0;
	hwnd = CreateWindowEx (WS_EX_DLGMODALFRAME|WS_EX_CLIENTEDGE, szClassName, g_szWndTitle,WS_CAPTION|WS_VISIBLE|WS_SYSMENU,CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,HWND_DESKTOP, NULL, hInstance, NULL);
	GetClientRect(hwnd, &rc);
	rc.bottom=rc.bottom-21;

	HWND hCtrl = CreateWindowEx(0, "button", " Code barre ",WS_CHILD|WS_VISIBLE|BS_GROUPBOX,5, 5, rc.right-10, 55, hwnd, (HMENU)0, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	if(__argc>1){
		hCtrl = CreateWindowEx(512, "edit", __argv[1],WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, 15, 25, rc.right-248, 22, hwnd, (HMENU)IDC_EDIT, wincl.hInstance, NULL);    
	// ModeTelecommande(hwnd);
	}else{
		hCtrl = CreateWindowEx(512, "edit", "",WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL, 15, 25, rc.right-248, 22, hwnd, (HMENU)IDC_EDIT, wincl.hInstance, NULL);    
	}
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "Button", "Générer code",WS_CHILD|WS_VISIBLE, rc.right-120, 25, 110, 22, hwnd, (HMENU)IDC_DECODE, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "Button", "Checkdigit",WS_CHILD|WS_VISIBLE, rc.right-221, 25, 90, 22, hwnd, (HMENU)IDC_CHECKDIGIT, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "button", " Code résultant ",WS_CHILD|WS_VISIBLE|BS_GROUPBOX, 5, 65, rc.right-10, 55, hwnd, NULL, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(512, "edit", "© Patrice Waechter-Ebling 2020",WS_CHILD|WS_VISIBLE|ES_READONLY|ES_AUTOHSCROLL, 15, 85, rc.right-137, 22, hwnd, (HMENU)IDC_EDITRES, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "Button", TEXT_BTN1,WS_CHILD|WS_VISIBLE|WS_DISABLED, rc.right-120, 85, 110, 22, hwnd, (HMENU)IDC_BINARY, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "button", " Affichage du code barre généré ",WS_CHILD|WS_VISIBLE|BS_GROUPBOX, 5, 125, rc.right-10, rc.bottom - 160, hwnd, (HMENU)0, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(1), 1L);
	hCtrl = CreateWindowEx(WS_EX_DLGMODALFRAME|WS_EX_CLIENTEDGE, "static", "",WS_CHILD|WS_VISIBLE|SS_OWNERDRAW,/*|WS_BORDER,*/15, 145, rc.right-30, rc.bottom - 225,hwnd, (HMENU)IDC_IMAGE, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "static", "",WS_CHILD|WS_VISIBLE|SS_ETCHEDHORZ, 15, rc.bottom-75, rc.right-30, 3,hwnd, NULL, wincl.hInstance, NULL); 
	hCtrl = CreateWindowEx(0, "Button", "Imprimer",WS_CHILD|WS_VISIBLE|WS_DISABLED, rc.right-125, rc.bottom-65, 110, 22, hwnd, (HMENU)IDC_PRINT, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "Button", "Copier",WS_CHILD|WS_VISIBLE|WS_DISABLED, rc.right-236, rc.bottom-65, 110, 22, hwnd, (HMENU)IDC_COPY, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "Button", "I -",WS_CHILD|WS_VISIBLE|WS_DISABLED, 15, rc.bottom-65, 40, 22, hwnd, (HMENU)IDC_VZOOMDOWN, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "Button", "I +",WS_CHILD|WS_VISIBLE|WS_DISABLED, 56, rc.bottom-65, 40, 22, hwnd, (HMENU)IDC_VZOOMUP, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "Button", "> - <",WS_CHILD|WS_VISIBLE|WS_DISABLED, 97, rc.bottom-65, 40, 22, hwnd, (HMENU)IDC_HZOOMDOWN, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "Button", "< + >",WS_CHILD|WS_VISIBLE|WS_DISABLED, 138, rc.bottom-65, 40, 22, hwnd, (HMENU)IDC_HZOOMUP, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "Button", "Fermer",WS_CHILD|WS_VISIBLE, rc.right-125, rc.bottom-25, 110, 22, hwnd, (HMENU)IDC_FILECLOSE, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "Button", "Exporter",WS_CHILD|WS_VISIBLE, rc.right-236, rc.bottom-25, 110, 22, hwnd, (HMENU)IDC_FILEHELP, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	hCtrl = CreateWindowEx(0, "Button", "A propos ...",WS_CHILD|WS_VISIBLE, rc.right-347, rc.bottom-25, 110, 22, hwnd, (HMENU)IDC_ABOUTHELP, wincl.hInstance, NULL);    
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(18), 1L);
	GetLocalTime(&st);
	wsprintf(buffer,"Nous sommes %s, le %2d %s %4d",jours[st.wDayOfWeek],st.wDay,mois[st.wMonth-1],st.wYear);
	hCtrl =CreateStatusWindow(WS_CHILD|WS_VISIBLE,buffer,hwnd,6000);
	SendMessage(hCtrl, WM_SETFONT, (WPARAM)GetStockObject(10), 1L);
	SendMessage(hCtrl, SB_SETBKCOLOR, (WPARAM)0,(LPARAM)RGB(192,192,192));
	HWND imghWnd = CreateWindowEx(0, "STATIC", NULL, WS_VISIBLE|WS_CHILD|SS_ICON,1, rc.bottom-33, 10, 10,hwnd , (HMENU)45000, wincl.hInstance, NULL);
	SendMessage(imghWnd, STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(wincl.hInstance,(LPCTSTR)102)); //Logo Autheur
	SetTimer(hwnd,IDM_TIMER1,1000,(TIMERPROC) NULL);
	GetCurrentDirectory(256,szApp);
	EnableWindow(GetDlgItem(hwnd, IDC_FILEHELP), FALSE);
	ShowWindow (hwnd, nFunsterStil);
    while (GetMessage (&messages, NULL, 0, 0))    {        TranslateMessage(&messages);        DispatchMessage(&messages);    }    return messages.wParam;
}
