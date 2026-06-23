
// MyFractalView.cpp : implémentation de la classe CMyFractalView
//

#include "pch.h"
#include "framework.h"

// AJOUTS :
#include <complex>  //  <- doit être inclus pour utiliser std::complex
#include <cmath>    //  <- doit être inclus pour utiliser sin() et cos()
#include <vector>   //  <- doit être inclus pour utiliser std::vector


// SHARED_HANDLERS peuvent être définis dans les gestionnaires d'aperçu, de miniature
// et de recherche d'implémentation de projet ATL, et permettent le partage de code de document avec ce projet.
#ifndef SHARED_HANDLERS
#include "MyFractal.h"
#endif

#include "MyFractalDoc.h"
#include "MyFractalView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyFractalView

IMPLEMENT_DYNCREATE(CMyFractalView, CView)

BEGIN_MESSAGE_MAP(CMyFractalView, CView)
	// Commandes d'impression standard
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMyFractalView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// construction/destruction de CMyFractalView

CMyFractalView::CMyFractalView() noexcept
{
	// TODO: ajoutez ici du code de construction

}

CMyFractalView::~CMyFractalView()
{
}

BOOL CMyFractalView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: changez ici la classe ou les styles Window en modifiant
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


// Ajouts :

// Fonction pour générer une couleur fluide basée sur le nombre d'itérations
COLORREF GetFractalColor(int iter, int max_iter) {
	if (iter == max_iter) return RGB(0, 0, 0); // L'intérieur de l'ensemble est noir

	// Un algorithme simple de dégradé (sinusoïdal) pour de jolies couleurs
	int r = static_cast<int>(sin(0.1 * iter + 0.0) * 127 + 128);
	int g = static_cast<int>(sin(0.1 * iter + 2.0) * 127 + 128);
	int b = static_cast<int>(sin(0.1 * iter + 4.0) * 127 + 128);

	return RGB(r, g, b);
}

// dessin de CMyFractalView

void CMyFractalView::OnDraw(CDC* pDC)
{
	CMyFractalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ajoutez ici le code de dessin pour les données natives
    // 1. Récupérer la taille de la fenêtre
    CRect rect;
    GetClientRect(&rect);
    int width = rect.Width();
    int height = rect.Height();

    if (width == 0 || height == 0) return;

    // 1. Création du tableau de pixels en mémoire (32 bits par pixel)
        // Taille = Largeur * Hauteur
    std::vector<DWORD> pixelBuffer(width * height);

    // Paramètres de la fractale
    const int max_iter = 150;
    
    // Décommentez UNE des deux lignes suivantes pour choisir le mode :
    bool drawMandelbrot = false;  // true = Mandelbrot, false = Julia

    // Constante 'c' pour l'ensemble de Julia (ex: -0.7 + 0.27015i)
    std::complex<double> juliaC(-0.7, 0.27015);

    // Espace complexe à cartographier (Zoom et déplacement)
    double minX = -2.0, maxX = 1.0;
    double minY = -1.5, maxY = 1.5;

    // Ajuster le ratio pour éviter la distorsion
    double aspect = (double)width / height;
    if (aspect > 1.0) {
        minX *= aspect;
        maxX *= aspect;
    }
    else {
        minY /= aspect;
        maxY /= aspect;
    }

    // Boucle de calcul
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            double cr = minX + (double)x / width * (maxX - minX);
            double ci = minY + (double)y / height * (maxY - minY);

            std::complex<double> z(cr, ci);
			std::complex<double> c = drawMandelbrot ? z : juliaC; // imprimer Mandelbrot ou Julia selon le mode choisi
            int iter = 0;

            while (std::norm(z) <= 4.0 && iter < max_iter) {
                z = z * z + c; // Exemple rapide (Mandelbrot)
                iter++;
            }

            // Calcul de la couleur (Format Windows : 0x00BBGGRR)
            int r = static_cast<int>(sin(0.1 * iter + 0.0) * 127 + 128);
            int g = static_cast<int>(sin(0.1 * iter + 2.0) * 127 + 128);
            int b = static_cast<int>(sin(0.1 * iter + 4.0) * 127 + 128);

            if (iter == max_iter) { r = g = b = 0; }

            // 2. Stockage du pixel dans le tableau à une dimension
            // Formule de l'index : (y * largeur) + x
            pixelBuffer[y * width + x] = RGB(r, g, b);
        }
    }

    // 3. Configuration de la structure BITMAPINFO pour Windows
    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // Hauteur négative pour inverser l'axe Y (de haut en bas)
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;     // 32 bits (4 octets par pixel : RGBA)
    bmi.bmiHeader.biCompression = BI_RGB;

    // 4. Envoi du tableau de pixels directement à l'écran (Super rapide !)
    SetDIBitsToDevice(
        pDC->GetSafeHdc(),
        0, 0, width, height,     // Destination (x, y, largeur, hauteur)
        0, 0, 0, height,         // Source
        pixelBuffer.data(),      // Pointeur vers notre std::vector
        &bmi,                    // Structure de configuration
        DIB_RGB_COLORS
    );

}


// impression de CMyFractalView


void CMyFractalView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMyFractalView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// préparation par défaut
	return DoPreparePrinting(pInfo);
}

void CMyFractalView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ajoutez une initialisation supplémentaire avant l'impression
}

void CMyFractalView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ajoutez un nettoyage après l'impression
}

void CMyFractalView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMyFractalView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// diagnostics de CMyFractalView

#ifdef _DEBUG
void CMyFractalView::AssertValid() const
{
	CView::AssertValid();
}

void CMyFractalView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyFractalDoc* CMyFractalView::GetDocument() const // la version non Debug est inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyFractalDoc)));
	return (CMyFractalDoc*)m_pDocument;
}
#endif //_DEBUG


// gestionnaires de messages de CMyFractalView
