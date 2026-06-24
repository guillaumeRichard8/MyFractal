
// MyFractal.cpp : définit les comportements de classe de l'application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MyFractal.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "MyFractalDoc.h"
#include "MyFractalView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyFractalApp

BEGIN_MESSAGE_MAP(CMyFractalApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CMyFractalApp::OnAppAbout)
	// Commandes de document de fichier standard
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Commande standard de configuration de l'impression
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// Construction de CMyFractalApp

CMyFractalApp::CMyFractalApp() noexcept
{
	m_bHiColorIcons = TRUE;


	m_nAppLook = 0;
	// TODO: remplacer la chaîne d'ID de l'application ci-dessous par une chaîne ID unique ; le format recommandé
	// pour la chaîne est CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MyFractal.AppID.NoVersion"));

	// TODO: ajoutez ici du code de construction,
	// Placez toutes les initialisations significatives dans InitInstance
}

// Le seul et unique objet CMyFractalApp

CMyFractalApp theApp;


// Initialisation de CMyFractalApp

BOOL CMyFractalApp::InitInstance()
{
	// InitCommonControlsEx() est requis sur Windows XP si le manifeste de l'application
	// spécifie l'utilisation de ComCtl32.dll version 6 ou ultérieure pour activer les
	// styles visuels.  Dans le cas contraire, la création de fenêtres échouera.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// À définir pour inclure toutes les classes de contrôles communs à utiliser
	// dans votre application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialiser les bibliothèques OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() est obligatoire pour utiliser un contrôle RichEdit
	// AfxInitRichEdit2();

	// Initialisation standard
	// Si vous n'utilisez pas ces fonctionnalités et que vous souhaitez réduire la taille
	// de votre exécutable final, vous devez supprimer ci-dessous
	// les routines d'initialisation spécifiques dont vous n'avez pas besoin.
	// Changez la clé de Registre sous laquelle nos paramètres sont enregistrés
	// TODO: modifiez cette chaîne avec des informations appropriées,
	// telles que le nom de votre société ou organisation
	SetRegistryKey(_T("Applications locales générées par AppWizard"));
	LoadStdProfileSettings(4);  // Charge les options de fichier INI standard (y compris les derniers fichiers utilisés)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Inscrire les modèles de document de l'application.  Ces modèles
	//  lient les documents, fenêtres frame et vues entre eux
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MyFractalTYPE,
		RUNTIME_CLASS(CMyFractalDoc),
		RUNTIME_CLASS(CChildFrame), // frame enfant MDI personnalisé
		RUNTIME_CLASS(CMyFractalView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// crée la fenêtre frame MDI principale
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// Analyser la ligne de commande pour les commandes shell standard, DDE, ouverture de fichiers
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Commandes de dispatch spécifiées sur la ligne de commande.  Retournent FALSE si
	// l'application a été lancée avec /RegServer, /Register, /Unregserver ou /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// La fenêtre principale a été initialisée et peut donc être affichée et mise à jour
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CMyFractalApp::ExitInstance()
{
	//TODO: gérez les ressources supplémentaires que vous avez ajoutées
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// gestionnaires de messages de CMyFractalApp


// boîte de dialogue CAboutDlg utilisée pour la boîte de dialogue 'À propos de' pour votre application

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Données de boîte de dialogue
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

// Implémentation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Commande App pour exécuter la boîte de dialogue
void CMyFractalApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// méthodes de chargement/d'enregistrement de la personnalisation de CMyFractalApp

void CMyFractalApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CMyFractalApp::LoadCustomState()
{
}

void CMyFractalApp::SaveCustomState()
{
}

// gestionnaires de messages de CMyFractalApp



