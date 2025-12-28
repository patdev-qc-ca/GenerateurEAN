#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define WINVER 0x0501
#define _WIN32_IE 0x501
#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <wingdi.h>
#include <shlwapi.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "resource.h"

#pragma comment (lib,"olegdi2.lib")
#pragma comment (lib,"ddraw.lib")
#pragma comment (lib,"Gdi32.lib")
#pragma comment (lib,"Comdlg32.lib")
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"Lz32.lib")
#pragma comment(lib,"shlwapi.lib")
#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "advapi32.lib")

#define IDC_IMAGE        200
#define IDC_DECODE       201
#define IDC_EDIT         202
#define IDC_EDITRES      203
#define IDC_BINARY       204
#define IDC_CHECKDIGIT   205
#define IDC_PRINT        206
#define IDC_COPY         207
#define IDC_HZOOMUP      208
#define IDC_HZOOMDOWN    209
#define IDC_VZOOMUP      210
#define IDC_VZOOMDOWN    211
#define IDC_FILECLOSE    212
#define IDC_FILEHELP     213
#define IDC_ABOUTHELP    214
#define IDI_APPMAIN    100
#define IDD_PRINTSETUP 101
#define IDC_EDIT_LEFT   1000
#define IDC_EDIT_TOP    1001
#define IDC_EDIT_WIDTH  1002
#define IDC_EDIT_HEIGHT 1003
#define IDC_UD_LEFT     1004
#define IDC_UD_TOP      1005
#define IDC_UD_WIDTH    1006
#define IDC_UD_HEIGHT   1007
#define TEXT_BTN1   "Binaire"
#define TEXT_BTN2   "Caractère"
#define Version "2.0.1.87"


static char m_szData[256];
static int m_iError,m_nbBits,m_bitSize,m_step,m_height;
static const short cstMargin[4][2] = {0, 190, 0, 280, 10, 200, 2, 100};
const WORD cstTable128[] = {0x0a6d,0x0d2b,0x0b2b,0x0d95,0x0a6b,0x0d35,0x0b35,0x0a5b,0x0d2d,0x0b2d,0x0d4b,0x0b4b,0x0da5,0x0acb,0x0d65,0x0b65,0x0a9b,0x0d4d,0x0b4d,0x0acd,0x0d53,0x0b53,0x0da9,0x0ad3,0x0d69,0x0b69,0x0ab3,0x0d59,0x0b59,0x0ad9,0x0cab,0x09ab,0x0cd5,0x096b,0x0cb5,0x09b5,0x095b,0x0cad,0x09ad,0x096d,0x0927,0x0929,0x0949,0x0a49};
enum { ERR_NONE = 0, ERR_VAR_CODE_NULL, ERR_CODE_INCOMPATIBLE, ERR_BAD_FORMAT_SIZE, ERR_SIZE_LESS, ERR_CODE_TRUNCATE };
/**/
class __declspec(dllexport) CDlgPrintSetup{
public:
    CDlgPrintSetup(LPRECT);
    ~CDlgPrintSetup();    
    int Create(HWND);
    
private:
    static BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
    BOOL MsgInitDialog(HWND, WPARAM, LPARAM);
    BOOL MsgCommand(HWND, WPARAM, LPARAM);
    BOOL MsgVScroll(HWND, WPARAM, LPARAM);
    BOOL MsgSysCommand(WPARAM);    
    void CmdOk(HWND);
    HWND       m_hDlg;
    LPRECT     m_pRect;
};



class __declspec(dllexport) TTransfoDatasV2{
public:
	class TABS{
	public:
		class TAB1{
		public:
			double CoefV1;
			double CoefV2;
			double CoefV3;
			double CoefV4;
			double CoefV5;
			double RatioV1;
			double RatioV2;
			double RatioV3;
			double RatioV4;
			double RatioV5;
			double AmpV1;
			double AmpV2;
			double AmpV3;
			double AmpV4;
			double AmpV5;
		} t1;
		class TAB2{
		public:
			double CoefV1;
			double CoefV2;
			double CoefV3;
			double CoefV4;
			double CoefV5;
			double RatioV1;
			double RatioV2;
			double RatioV3;
			double RatioV4;
			double RatioV5;
			double AmpV1;
			double AmpV2;
			double AmpV3;
			double AmpV4;
			double AmpV5;
		} t2;
		class TAB3{
		public:
			double CoefV1;
			double CoefV2;
			double CoefV3;
			double CoefV4;
			double CoefV5;
			double RatioV1;
			double RatioV2;
			double RatioV3;
			double RatioV4;
			double RatioV5;
			double AmpV1;
			double AmpV2;
			double AmpV3;
			double AmpV4;
			double AmpV5;
		} t3;
		double VoltageV1;
		double VoltageV2;
		double VoltageV3;
		double VoltageV4;
		double VoltageV5;
		double PCent1;
		double PCent2;
		double PCent3;
		double PCent4;
		double PCent5;
	} tableau;
	class IDENTIFICATION{
	public:
		double dimH;
		double dimD;
		double dimV;
		double VolHuile;
		double Numero;
		double Primaire;
		double Secondaire1;
		double Secondaire2;
		double Secondaire3;
		double Capacite;
		double Phases;
		double Frequence;
		double Ratio1;
		double Ratio2;
		double Ratio3;
		double Serial;
		double Pos;
		double Poids;
		char* LUG;
		char* Lot;
		char* Reference;
		char* Fabriquant;
		char* Localisation;
		char* RecuLe;
		char* Modele;
	} id;
	class TESTE{
	public:
		double IsolementLN;
		double IsolementLL;
		double IsolemenTN;
		double IsolementLT;
		double IsolateurPrimaireExterne;
		double IsolateurPrimaire;
		double IsolateurSecondaire;
		double HumiditePresente;
		double RefroissementEtanche;
		double HuileRefroidissement;
		double ImpedancePrimaire;
		double ImpedanceSecondaire;
		double TensionPrimaire;
		double TensionSecondaire1;
		double TensionSecondaire2;
		double TensionSecondaire3;
		double Fixations;
		double Supports;
		double Etancheite;
		double Peinture;
		double QualiteHuile;
		double Selecteur;
		double Borne2Terre;
		double ConnecteursSecondaire;
		double ConexionBobinage;
		double MultiTension;
		double InductanceSecondaire;
		char* Commentaire;
	} tst;
	char* Designaton;
	char* Etat;
	char* Teste;
	char appPath[260];
	char RapportPath[260];
	char ArchivePath[260];
	char ReferentielFileName[260];
}TransfoDatas2 ,*LPTransfoDatas2;
char* GetDLLVersion();
void initTransfoDatas();
char* ReadDatas(char* Section, char* Clef, char* Source);
int GetPackVersion(char* Source);
int SetDefautVersion(char* Source);
int UpdateDatas(char* Section, char* Clef, char* Valeur, char* Cible);
int UpdateDatas(char* Section, char* Clef, double Valeur, char* Cible);
int SaveData(TTransfoDatasV2 datas, char* Cible);
int CreerRapportText(char* Cible, TTransfoDatasV2 datas);
int InsererControleCodeBarre(HINSTANCE hExterne, HWND hConteneur, COLORREF CouleurCB, HBRUSH Arrierplan);
LRESULT CALLBACK ModuleCBProcedure(HWND hDlg , UINT Message, WPARAM wParam, LPARAM lParam);
void SetBits(char **lppData, int * lpPos, WORD wCode, int len);
int Decode(const char * pszCode);
int GetApproximateStep(int width);
int GetMinimalHeight(HDC hdc, int step);
int CheckDigit(const char * pszCode, BOOL bComplet);
void Draw(HDC hdc, POINT pt, int height, int step);
int SetIndex(char car);
int GetApproximateWidth(int step);
const char* GetCodeString();
int GetAppLastError();
const char* ErrorString(int);
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,HBITMAP hBMP, HDC hDC);
void errhandler(LPCTSTR Texte,HWND hwnd);
void SetResultCode(HWND hwnd);
BOOL PreparePrint(HDC hdc, POINTFLOAT ptf, LPRECT lprc);
void SetResultCode(HWND hwnd);
BOOL PreparePrint(HDC hdc, POINTFLOAT ptf, LPRECT lprc);
int GetAppLastError();
const char* ErrorString(int error);
const char* GetCodeString();
int MsgBox(HINSTANCE hInst,HWND hDlg,char* lpszText,char* lpszCaption, DWORD dwStyle,int lpszIcon);
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
