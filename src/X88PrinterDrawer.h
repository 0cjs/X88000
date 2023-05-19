////////////////////////////////////////////////////////////
// X88000 Printer Image Drawer
//
// Written by Manuke

#ifndef X88PrinterDrawer_DEFINED
#define X88PrinterDrawer_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88PrinterDrawer;

////////////////////////////////////////////////////////////
// include

#include "ParallelPrinter.h"

////////////////////////////////////////////////////////////
// declaration of CX88PrinterDrawer

class CX88PrinterDrawer {
// create & destroy
public:
	// default constructor
	CX88PrinterDrawer();
	// destructor
	virtual ~CX88PrinterDrawer();

// operation
protected:

	// set point
	static void SetPoint(int anPoint[2], int x, int y) {
		anPoint[0] = x;
		anPoint[1] = y;
	}
	// getr CG-charater data
	static uint16_t GetCGCharacterData(
		uint16_t wText,
		int anPoints[64][2],
		int anPointCounts[16],
		int& nPolygonCount, int& nTotal);

#ifdef X88_GUI_WINDOWS

	// draw CG-charater
	static uint16_t DrawCGCharacter(
		HDC hdc,
		int xDraw, int yDraw, int cxDraw, int cyDraw,
		uint16_t wText);
	// draw monochrome bitmap
	static void DrawMonoBitmap(
		HDC hdc,
		int xDraw, int yDraw, int cxDraw, int cyDraw,
		const FPrinterBitmap* pPrtBmp,
		bool bTurn);

#elif defined(X88_GUI_GTK)

#ifdef X88_PRINTER_DRAW_POSTSCRIPT

	// draw CG-charater to PostScript
	static uint16_t DrawCGCharacterToPostScript(
		FILE* pFile,
		int xDraw, int yDraw, int cxDraw, int cyDraw,
		uint16_t wText);
	// draw monochrome bitmap to PostScript
	static void DrawMonoBitmapToPostScript(
		FILE* pFile,
		int xDraw, int yDraw, int cxDraw, int cyDraw,
		const FPrinterBitmap* pPrtBmp,
		bool bTurn);

#elif defined(X88_PRINTER_DRAW_CAIRO)

	// draw CG-charater to cairo
	static uint16_t DrawCGCharacterToCairo(
		cairo_t* pCairo,
		int xDraw, int yDraw, int cxDraw, int cyDraw,
		uint16_t wText);
	// draw monochrome bitmap to cairo
	static void DrawMonoBitmapToCairo(
		cairo_t* pCairo,
		int xDraw, int yDraw, int cxDraw, int cyDraw,
		const FPrinterBitmap* pPrtBmp,
		bool bTurn);

#endif // X88_PRINTER_DRAW

#endif // X88_GUI

public:

#ifdef X88_GUI_WINDOWS

	// draw sprocket-hole
	virtual void DrawSprocketHole(
		HDC hdc,
		const CParallelPrinter* pPrinter,
		int nPage,
		const RECT* prectClip);
	// draw printer-head position
	virtual void DrawPrinterHead(
		HDC hdc,
		const CParallelPrinter* pPrinter,
		int nPage,
		const RECT* prectClip);
	// draw paper-border
	virtual void DrawPaperBorder(
		HDC hdc,
		const CParallelPrinter* pPrinter,
		int nPage,
		const RECT* prectClip);
	// draw printer object
	virtual void DrawPrinterObject(
		HDC hdc,
		const CParallelPrinter* pPrinter,
		int nPage,
		const RECT* prectClip);

#elif defined(X88_GUI_GTK)

#ifdef X88_PRINTER_DRAW_POSTSCRIPT

	// initialize PostScript
	virtual bool InitializePostScript(
		FILE* pFile,
		const CParallelPrinter* pPrinter,
		int nZoomOut);
	// finalize PostScript
	virtual bool FinalizePostScript(
		FILE* pFile,
		const CParallelPrinter* pPrinter);
	// draw sprocket-hole to PostScript
	virtual bool DrawSprocketHoleToPostScript(
		FILE* pFile,
		const CParallelPrinter* pPrinter,
		int nPage);
	// draw printer-head position to PostScript
	virtual bool DrawPrinterHeadToPostScript(
		FILE* pFile,
		const CParallelPrinter* pPrinter,
		int nPage);
	// draw paper-border to PostScript
	virtual bool DrawPaperBorderToPostScript(
		FILE* pFile,
		const CParallelPrinter* pPrinter,
		int nPage);
	// draw printer object to PostScript
	virtual bool DrawPrinterObjectToPostScript(
		FILE* pFile,
		const CParallelPrinter* pPrinter,
		int nPage);

#elif defined(X88_PRINTER_DRAW_CAIRO)

	// draw sprocket-hole to cairo
	virtual void DrawSprocketHoleToCairo(
		cairo_t* pCairo,
		const CParallelPrinter* pPrinter,
		int nPage);
	// draw printer-head position to cairo
	virtual void DrawPrinterHeadToCairo(
		cairo_t* pCairo,
		const CParallelPrinter* pPrinter,
		int nPage);
	// draw paper-border to cairo
	virtual void DrawPaperBorderToCairo(
		cairo_t* pCairo,
		const CParallelPrinter* pPrinter,
		int nPage);
	// draw printer object to cairo
	virtual void DrawPrinterObjectToCairo(
		cairo_t* pCairo,
		const CParallelPrinter* pPrinter,
		int nPage);

#endif // X88_PRINTER_DRAW

#endif // X88_GUI

	// extract text
	virtual void ExtractText(
		const CParallelPrinter* pPrinter,
		int nPage,
		std::string& jstrText,
		bool bCRLF);
};

#endif // X88PrinterDrawer_DEFINED
