#pragma once
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
#define  TEXT_BTN1   "Binaire"
#define  TEXT_BTN2   "Caractère"
#define IDM_TIMER1 0x01000

#include "resource.h"
HBRUSH hBrushStatic;
WNDCLASSEX wincl;    
RECT rc, rect,m_rcPrint;
SYSTEMTIME st;
TCHAR buffer[MAX_PATH];
TCHAR szApp[MAX_PATH];
HDC hdcScreen,hdcCompatible;
HBITMAP hbmScreen,bmp,hbmScaled;
BITMAP btmp;

static char *jours[7] = {"dimanche", "lundi","mardi","mercredi","jeudi","vendredi","samedi"};
static char *mois[12] = {"janvier", "février","mars", "avril", "mai", "juin","juillet","aout","septembre", "octobre", "novembre", "décembre"};
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
class __declspec(dllimport) CDlgPrintSetup{
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



class __declspec(dllimport) TTransfoDatasV2{
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
TTransfoDatasV2 tdatas2;