////////////////////////////////////////////////////////////
// X88000 Printer Image Drawer
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88PrinterDrawer.h"

#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88PrinterDrawer

////////////////////////////////////////////////////////////
// create & destroy

// default constructor

CX88PrinterDrawer::CX88PrinterDrawer() {
}

// destructor

CX88PrinterDrawer::~CX88PrinterDrawer() {
}

////////////////////////////////////////////////////////////
// operation

// getr CG-charater data

uint16_t CX88PrinterDrawer::GetCGCharacterData(
	uint16_t wText,
	int anPoints[64][2],
	int anPointCounts[16],
	int& nPolygonCount, int& nTotal)
{
	nPolygonCount = 0, nTotal = 0;
	int nTotalPrev = 0;
	uint16_t wChar = 0;
	switch (wText) {
	case 0x20:
		SetPoint(anPoints[nTotal++], 0, 22);
		SetPoint(anPoints[nTotal++], 16, 22);
		SetPoint(anPoints[nTotal++], 16, 24);
		SetPoint(anPoints[nTotal++], 0, 24);
		anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		break;
	case 0x21:
	case 0x22:
	case 0x23:
	case 0x24:
	case 0x25:
	case 0x26:
	case 0x27:
		SetPoint(anPoints[nTotal++], 0, 18-(wText-0x21)*3);
		SetPoint(anPoints[nTotal++], 16, 18-(wText-0x21)*3);
		SetPoint(anPoints[nTotal++], 16, 24);
		SetPoint(anPoints[nTotal++], 0, 24);
		anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		break;
	case 0x28:
	case 0x29:
	case 0x2A:
	case 0x2B:
	case 0x2C:
	case 0x2D:
	case 0x2E:
		SetPoint(anPoints[nTotal++], 0, 0);
		SetPoint(anPoints[nTotal++], 2+(wText-0x28)*2, 0);
		SetPoint(anPoints[nTotal++], 2+(wText-0x28)*2, 24);
		SetPoint(anPoints[nTotal++], 0, 24);
		anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		break;
	case 0x2F:
	case 0x30:
	case 0x31:
	case 0x32:
	case 0x33:
	case 0x38:
	case 0x39:
	case 0x3A:
	case 0x3B:
		{ // dummy block
			uint8_t btFlags = 0;
			switch (wText) {
			case 0x2F:
				btFlags = 0x0F;
				break;
			case 0x30:
				btFlags = 0x0B;
				break;
			case 0x31:
				btFlags = 0x0E;
				break;
			case 0x32:
				btFlags = 0x0D;
				break;
			case 0x33:
				btFlags = 0x07;
				break;
			case 0x38:
				btFlags = 0x06;
				break;
			case 0x39:
				btFlags = 0x0C;
				break;
			case 0x3A:
				btFlags = 0x03;
				break;
			case 0x3B:
				btFlags = 0x09;
				break;
			}
			SetPoint(anPoints[nTotal++], 7, 11);
			if ((btFlags & 0x01) != 0) {
				SetPoint(anPoints[nTotal++], 7, 0);
				SetPoint(anPoints[nTotal++], 9, 0);
			}
			SetPoint(anPoints[nTotal++], 9, 11);
			if ((btFlags & 0x02) != 0) {
				SetPoint(anPoints[nTotal++], 16, 11);
				SetPoint(anPoints[nTotal++], 16, 13);
			}
			SetPoint(anPoints[nTotal++], 9, 13);
			if ((btFlags & 0x04) != 0) {
				SetPoint(anPoints[nTotal++], 9, 24);
				SetPoint(anPoints[nTotal++], 7, 24);
			}
			SetPoint(anPoints[nTotal++], 7, 13);
			if ((btFlags & 0x08) != 0) {
				SetPoint(anPoints[nTotal++], 0, 13);
				SetPoint(anPoints[nTotal++], 0, 11);
			}
			anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		}
		break;
	case 0x34:
	case 0x35:
		SetPoint(anPoints[nTotal++], 0, (wText-0x34)*11);
		SetPoint(anPoints[nTotal++], 16, (wText-0x34)*11);
		SetPoint(anPoints[nTotal++], 16, 2+(wText-0x34)*11);
		SetPoint(anPoints[nTotal++], 0, 2+(wText-0x34)*11);
		anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		break;
	case 0x36:
	case 0x37:
		SetPoint(anPoints[nTotal++], 7+(wText-0x36)*7, 0);
		SetPoint(anPoints[nTotal++], 9+(wText-0x36)*7, 0);
		SetPoint(anPoints[nTotal++], 9+(wText-0x36)*7, 24);
		SetPoint(anPoints[nTotal++], 7+(wText-0x36)*7, 24);
		anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		break;
	case 0x3C:
	case 0x3D:
	case 0x3E:
	case 0x3F:
		{ // dummy block
			SetPoint(anPoints[nTotal++], 9, 0);
			SetPoint(anPoints[nTotal++], 8, 5);
			SetPoint(anPoints[nTotal++], 6, 9);
			SetPoint(anPoints[nTotal++], 3, 12);
			SetPoint(anPoints[nTotal++], 0, 13);
			SetPoint(anPoints[nTotal++], 0, 11);
			SetPoint(anPoints[nTotal++], 3, 10);
			SetPoint(anPoints[nTotal++], 5, 7);
			SetPoint(anPoints[nTotal++], 6, 5);
			SetPoint(anPoints[nTotal++], 7, 0);
			anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
			for (int i = 0; i < nTotal; i++) {
				if ((wText == 0x3C) || (wText == 0x3E)) {
					anPoints[i][0] = 16-anPoints[i][0];
				}
				if ((wText == 0x3C) || (wText == 0x3D)) {
					anPoints[i][1] = 24-anPoints[i][1];
				}
			}
		}
		break;
	case 0x40:
		SetPoint(anPoints[nTotal++], 0, 7);
		SetPoint(anPoints[nTotal++], 16, 7);
		SetPoint(anPoints[nTotal++], 16, 9);
		SetPoint(anPoints[nTotal++], 0, 9);
		anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		nTotalPrev = nTotal;
		SetPoint(anPoints[nTotal++], 0, 15);
		SetPoint(anPoints[nTotal++], 16, 15);
		SetPoint(anPoints[nTotal++], 16, 17);
		SetPoint(anPoints[nTotal++], 0, 17);
		anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		break;
	case 0x41:
	case 0x42:
	case 0x43:
		{ // dummy block
			uint8_t btFlags = 0;
			switch (wText) {
			case 0x41:
				btFlags = 0x01;
				break;
			case 0x42:
				btFlags = 0x03;
				break;
			case 0x43:
				btFlags = 0x02;
				break;
			}
			SetPoint(anPoints[nTotal++], 7, 7);
			SetPoint(anPoints[nTotal++], 7, 0);
			SetPoint(anPoints[nTotal++], 9, 0);
			SetPoint(anPoints[nTotal++], 9, 7);
			if ((btFlags & 0x01) != 0) {
				SetPoint(anPoints[nTotal++], 16, 7);
				SetPoint(anPoints[nTotal++], 16, 9);
				SetPoint(anPoints[nTotal++], 9, 9);
				SetPoint(anPoints[nTotal++], 9, 15);
				SetPoint(anPoints[nTotal++], 16, 15);
				SetPoint(anPoints[nTotal++], 16, 17);
			}
			SetPoint(anPoints[nTotal++], 9, 17);
			SetPoint(anPoints[nTotal++], 9, 24);
			SetPoint(anPoints[nTotal++], 7, 24);
			SetPoint(anPoints[nTotal++], 7, 17);
			if ((btFlags & 0x02) != 0) {
				SetPoint(anPoints[nTotal++], 0, 17);
				SetPoint(anPoints[nTotal++], 0, 15);
				SetPoint(anPoints[nTotal++], 7, 15);
				SetPoint(anPoints[nTotal++], 7, 9);
				SetPoint(anPoints[nTotal++], 0, 9);
				SetPoint(anPoints[nTotal++], 0, 7);
			}
			anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		}
		break;
	case 0x44:
		SetPoint(anPoints[nTotal++], 16, 0);
		SetPoint(anPoints[nTotal++], 16, 24);
		SetPoint(anPoints[nTotal++], 0, 24);
		anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		break;
	case 0x45:
		SetPoint(anPoints[nTotal++], 0, 0);
		SetPoint(anPoints[nTotal++], 16, 24);
		SetPoint(anPoints[nTotal++], 0, 24);
		anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		break;
	case 0x46:
		SetPoint(anPoints[nTotal++], 0, 0);
		SetPoint(anPoints[nTotal++], 16, 0);
		SetPoint(anPoints[nTotal++], 16, 24);
		anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		break;
	case 0x47:
		SetPoint(anPoints[nTotal++], 0, 0);
		SetPoint(anPoints[nTotal++], 16, 0);
		SetPoint(anPoints[nTotal++], 0, 24);
		anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		break;
	case 0x4E:
	case 0x4F:
	case 0x50:
		if (wText != 0x4F) {
			SetPoint(anPoints[nTotal++], 16, 0);
			SetPoint(anPoints[nTotal++], 16, 1);
			SetPoint(anPoints[nTotal++], 1, 24);
			SetPoint(anPoints[nTotal++], 0, 24);
			SetPoint(anPoints[nTotal++], 0, 23);
			SetPoint(anPoints[nTotal++], 15, 0);
			anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
			nTotalPrev = nTotal;
		}
		if (wText != 0x4E) {
			SetPoint(anPoints[nTotal++], 0, 0);
			SetPoint(anPoints[nTotal++], 1, 0);
			SetPoint(anPoints[nTotal++], 16, 23);
			SetPoint(anPoints[nTotal++], 16, 24);
			SetPoint(anPoints[nTotal++], 15, 24);
			SetPoint(anPoints[nTotal++], 0, 1);
			anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		}
		break;
	case 0x48:
	case 0x49:
	case 0x4A:
	case 0x4B:
		{ // dummy block
			switch (wText) {
			case 0x48:
				SetPoint(anPoints[nTotal++], 8, 3);
				SetPoint(anPoints[nTotal++], 15, 10);
				SetPoint(anPoints[nTotal++], 16, 12);
				SetPoint(anPoints[nTotal++], 16, 15);
				SetPoint(anPoints[nTotal++], 15, 17);
				SetPoint(anPoints[nTotal++], 14, 18);
				SetPoint(anPoints[nTotal++], 12, 18);
				SetPoint(anPoints[nTotal++], 9, 17);
				SetPoint(anPoints[nTotal++], 11, 21);
				break;
			case 0x49:
				SetPoint(anPoints[nTotal++], 8, 5);
				SetPoint(anPoints[nTotal++], 9, 4);
				SetPoint(anPoints[nTotal++], 11, 3);
				SetPoint(anPoints[nTotal++], 14, 4);
				SetPoint(anPoints[nTotal++], 15, 5);
				SetPoint(anPoints[nTotal++], 16, 7);
				SetPoint(anPoints[nTotal++], 16, 11);
				SetPoint(anPoints[nTotal++], 15, 14);
				SetPoint(anPoints[nTotal++], 8, 21);
				break;
			case 0x4A:
				SetPoint(anPoints[nTotal++], 8, 3);
				SetPoint(anPoints[nTotal++], 16, 12);
				SetPoint(anPoints[nTotal++], 8, 21);
				break;
			case 0x4B:
				SetPoint(anPoints[nTotal++], 8, 3);
				SetPoint(anPoints[nTotal++], 10, 4);
				SetPoint(anPoints[nTotal++], 12, 6);
				SetPoint(anPoints[nTotal++], 12, 8);
				SetPoint(anPoints[nTotal++], 11, 11);
				SetPoint(anPoints[nTotal++], 13, 10);
				SetPoint(anPoints[nTotal++], 15, 11);
				SetPoint(anPoints[nTotal++], 16, 14);
				SetPoint(anPoints[nTotal++], 15, 17);
				SetPoint(anPoints[nTotal++], 13, 18);
				SetPoint(anPoints[nTotal++], 11, 18);
				SetPoint(anPoints[nTotal++], 9, 17);
				SetPoint(anPoints[nTotal++], 11, 21);
				break;
			}
			int i = nTotal-1;
			if (( i >= 0) && (anPoints[i][0] > 8)) {
				SetPoint(
					anPoints[nTotal++],
					16-anPoints[i][0], anPoints[i][1]);
			}
			for (; i >= 0; i--) {
				SetPoint(
					anPoints[nTotal++],
					16-anPoints[i][0], anPoints[i][1]);
			}
			anPointCounts[nPolygonCount++] = nTotal-nTotalPrev;
		}
		break;
	case 0x4C: // filled circle-mark
		wChar = 0x217C;
		break;
	case 0x4D: // unfilled circle-mark
		wChar = 0x217B;
		break;
	case 0x51: // yen-mark
		wChar = 0x315F;
		break;
	case 0x52: // year-kanji
		wChar = 0x472F;
		break;
	case 0x53: // month-kanji
		wChar = 0x376E;
		break;
	case 0x54: // day-kanji
		wChar = 0x467C;
		break;
	case 0x55: // hour-kanji
		wChar = 0x3B7E;
		break;
	case 0x56: // minute-kanji
		wChar = 0x4A2C;
		break;
	case 0x57: // second-kanji
		wChar = 0x4943;
		break;
	}
	return wChar;
}

#ifdef X88_GUI_WINDOWS

// draw CG-charater

uint16_t CX88PrinterDrawer::DrawCGCharacter(
	HDC hdc,
	int xDraw, int yDraw, int cxDraw, int cyDraw,
	uint16_t wText)
{
	int anPoints[64][2];
	int anPointCounts[16];
	int nPolygonCount, nTotal;
	uint16_t wChar = GetCGCharacterData(
		wText,
		anPoints, anPointCounts,
		nPolygonCount, nTotal);
	if (nPolygonCount > 0) {
		POINT aptPoints[64];
		for (int i = 0; i < nTotal; i++) {
			aptPoints[i].x = xDraw+(anPoints[i][0]*cxDraw+8)/16;
			aptPoints[i].y = yDraw+(anPoints[i][1]*cyDraw+12)/24;
		}
		PolyPolygon(hdc, aptPoints, anPointCounts, nPolygonCount);
	}
	if (wChar != 0) {
		wChar = (uint16_t)JIS2SJIS(wChar);
	}
	return wChar;
}

// draw monochrome bitmap

void CX88PrinterDrawer::DrawMonoBitmap(
	HDC hdc,
	int xDraw, int yDraw, int cxDraw, int cyDraw,
	const FPrinterBitmap* pPrtBmp,
	bool bTurn)
{
	int nDibWidth, nDibHeight;
	if (!bTurn) {
		nDibWidth = pPrtBmp->GetDotCX();
		nDibHeight = pPrtBmp->GetDotCY();
	} else {
		nDibWidth = pPrtBmp->GetDotCY();
		nDibHeight = pPrtBmp->GetDotCX();
	}
	uint8_t abtTmp[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*2];
	memset(abtTmp, 0, sizeof(abtTmp));
	BITMAPINFO* pbmi = (BITMAPINFO*)abtTmp;
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = nDibWidth;
	pbmi->bmiHeader.biHeight = nDibHeight;
	pbmi->bmiHeader.biBitCount = 1;
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiColors[1].rgbRed = 255;
	pbmi->bmiColors[1].rgbGreen = 255;
	pbmi->bmiColors[1].rgbBlue = 255;
	void* pvBits = NULL;
	HBITMAP hbmpImage = (HBITMAP)CreateDIBSection(
		hdc,
		pbmi, DIB_RGB_COLORS, &pvBits,
		NULL, 0);
	if (hbmpImage != NULL) {
		int nScanLine = ((nDibWidth+31)/32)*4;
		memset(
			pvBits, 0xFF,
			nDibHeight*nScanLine);
		uint8_t* pbtBits = ((uint8_t*)pvBits)+(nDibHeight-1)*nScanLine;
		if (!bTurn) {
			for (int y = 0; y < nDibHeight; y++) {
				for (int x = 0; x < nDibWidth; x++) {
					if (pPrtBmp->GetPixel(x, y) == 0) {
						pbtBits[x/8] &= (uint8_t)(~(0x80 >> (x%8)));
					}
				}
				pbtBits -= nScanLine;
			}
		} else {
			for (int y = 0; y < nDibHeight; y++) {
				for (int x = 0; x < nDibWidth; x++) {
					if (pPrtBmp->GetPixel(nDibHeight-y-1, x) == 0) {
						pbtBits[x/8] &= (uint8_t)(~(0x80 >> (x%8)));
					}
				}
				pbtBits -= nScanLine;
			}
		}
		HDC hdcMem = CreateCompatibleDC(hdc);
		if (hdcMem != NULL) {
			HBITMAP hbmpOrg = (HBITMAP)SelectObject(
				hdcMem, hbmpImage);
			SetStretchBltMode(hdc, STRETCH_ANDSCANS);
			StretchBlt(
				hdc,
				xDraw, yDraw, cxDraw, cyDraw,
				hdcMem,
				0, 0,
				nDibWidth, nDibHeight,
				SRCAND);
			SelectObject(hdcMem, hbmpOrg);
			DeleteDC(hdcMem);
		}
		DeleteObject(hbmpImage);
	}
}

#elif defined(X88_GUI_GTK)

#ifdef X88_PRINTER_DRAW_POSTSCRIPT

// draw CG-charater to PostScript

uint16_t CX88PrinterDrawer::DrawCGCharacterToPostScript(
	FILE* pFile,
	int xDraw, int yDraw, int cxDraw, int cyDraw,
	uint16_t wText)
{
	int anPoints[64][2];
	int anPointCounts[16];
	int nPolygonCount, nTotal;
	uint16_t wChar = GetCGCharacterData(
		wText,
		anPoints, anPointCounts,
		nPolygonCount, nTotal);
	if (nPolygonCount > 0) {
		int i = 0;
		for (int nPoly = 0; nPoly < nPolygonCount; nPoly++) {
			fprintf(pFile, "[");
			for (int nPoint = 0; nPoint < anPointCounts[nPoly]; nPoint++) {
				fprintf(
					pFile, "%lg %lg",
					xDraw+(anPoints[i][0]*cxDraw)/16.0,
					yDraw+(anPoints[i][1]*cyDraw)/24.0);
				i++;
				if (nPoint < anPointCounts[nPoly]-1) {
					if (nPoint%8 != 7) {
						fprintf(pFile, " ");
					} else {
						fprintf(pFile, "\n  ");
					}
				}
			}
			fprintf(pFile, "]\n");
			fprintf(pFile, "POLYGON\n");
		}
		fprintf(pFile, "fill\n");
	}
	return wChar;
}

// draw monochrome bitmap to PostScript

void CX88PrinterDrawer::DrawMonoBitmapToPostScript(
	FILE* pFile,
	int xDraw, int yDraw, int cxDraw, int cyDraw,
	const FPrinterBitmap* pPrtBmp,
	bool bTurn)
{
	int nImageWidth = pPrtBmp->GetDotCX(),
		nImageHeight = pPrtBmp->GetDotCY();
	if ((cxDraw <= 0) || (cyDraw <= 0) ||
		(nImageWidth <= 0) || (nImageHeight <= 0))
	{
		return;
	}
	fprintf(pFile, "%d %d false\n", nImageWidth, nImageHeight);
	if (!bTurn) {
		fprintf(
			pFile,
			"[%d %d div 0 0 %d %d div "
				"%d %d %d div mul neg %d %d %d div mul neg]\n",
			nImageWidth, cxDraw,
			nImageHeight, cyDraw,
			nImageWidth, xDraw, cxDraw,
			nImageHeight, yDraw, cyDraw);
	} else {
		fprintf(
			pFile,
			"[0 %d %d div %d %d div neg 0 "
				"%d dup %d %d div mul add %d %d %d div mul neg]\n",
			nImageHeight, cxDraw,
			nImageWidth, cyDraw,
			nImageWidth, yDraw, cyDraw,
			nImageHeight, xDraw, cxDraw);
	}
	fprintf(pFile, "{<\n");
	for (int y = 0; y < nImageHeight; y++) {
		int nCol = 0;
		uint8_t btBits = 0xFF;
		for (int x = 0; x < nImageWidth; x++) {
			if (pPrtBmp->GetPixel(x, y) == 0) {
				btBits &= (uint8_t)(~(0x80 >> (x%8)));
			}
			if ((x%8 == 7) || (x >= nImageWidth-1)) {
				fprintf(pFile, "%02X", btBits);
				btBits = 0xFF;
				if ((++nCol >= 32) || (x >= nImageWidth-1)) {
					fprintf(pFile, "\n");
				}
			}
		}
	}
	fprintf(pFile, ">}\n");
	fprintf(pFile, "imagemask\n");
}

#elif defined(X88_PRINTER_DRAW_CAIRO)

// draw CG-charater to cairo

uint16_t CX88PrinterDrawer::DrawCGCharacterToCairo(
	cairo_t* pCairo,
	int xDraw, int yDraw, int cxDraw, int cyDraw,
	uint16_t wText)
{
	int anPoints[64][2];
	int anPointCounts[16];
	int nPolygonCount, nTotal;
	uint16_t wChar = GetCGCharacterData(
		wText,
		anPoints, anPointCounts,
		nPolygonCount, nTotal);
	if (nPolygonCount > 0) {
		int i = 0;
		cairo_new_path(pCairo);
		for (int nPoly = 0; nPoly < nPolygonCount; nPoly++) {
			for (int nPoint = 0; nPoint < anPointCounts[nPoly]; nPoint++) {
				double x = xDraw+(anPoints[i][0]*cxDraw)/16.0,
					y = yDraw+(anPoints[i][1]*cyDraw)/24.0;
				if (nPoint == 0) {
					cairo_move_to(pCairo, x, y);
				} else {
					cairo_line_to(pCairo, x, y);
				}
				i++;
			}
			cairo_close_path(pCairo);
		}
		cairo_fill(pCairo);
	}
	return wChar;
}

// draw monochrome bitmap to cairo

void CX88PrinterDrawer::DrawMonoBitmapToCairo(
	cairo_t* pCairo,
	int xDraw, int yDraw, int cxDraw, int cyDraw,
	const FPrinterBitmap* pPrtBmp,
	bool bTurn)
{
	int nImageWidth, nImageHeight;
	if (!bTurn) {
		nImageWidth = pPrtBmp->GetDotCX();
		nImageHeight = pPrtBmp->GetDotCY();
	} else {
		nImageWidth = pPrtBmp->GetDotCY();
		nImageHeight = pPrtBmp->GetDotCX();
	}
	cairo_surface_t* pSurface = cairo_image_surface_create(
		CAIRO_FORMAT_A1,
		nImageWidth, nImageHeight);
	int nStride = cairo_image_surface_get_stride(pSurface);
	uint8_t* pbtBits = (uint8_t*)cairo_image_surface_get_data(pSurface);
	memset(pbtBits, 0x00, nImageHeight*nStride);
	if (!bTurn) {
		for (int y = 0; y < nImageHeight; y++) {
			for (int x = 0; x < nImageWidth; x++) {
				if (pPrtBmp->GetPixel(x, y) == 0) {

#ifdef X88_BYTEORDER_LITTLE_ENDIAN

					pbtBits[x/8] |= (uint8_t)(0x01 << (x%8));

#else // X88_BYTEORDER_BIG_ENDIAN

					pbtBits[x/8] |= (uint8_t)(0x80 >> (x%8));

#endif // X88_BYTEORDER

				}
			}
			pbtBits += nStride;
		}
	} else {
		for (int y = 0; y < nImageHeight; y++) {
			for (int x = 0; x < nImageWidth; x++) {
				if (pPrtBmp->GetPixel(nImageHeight-y-1, x) == 0) {

#ifdef X88_BYTEORDER_LITTLE_ENDIAN

					pbtBits[x/8] |= (uint8_t)(0x01 << (x%8));

#else // X88_BYTEORDER_BIG_ENDIAN

					pbtBits[x/8] |= (uint8_t)(0x80 >> (x%8));

#endif // X88_BYTEORDER

				}
			}
			pbtBits += nStride;
		}
	}
	cairo_surface_mark_dirty(pSurface);
	cairo_save(pCairo);
	cairo_translate(
		pCairo,
		xDraw, yDraw);
	cairo_scale(
		pCairo,
		(double)cxDraw/nImageWidth, (double)cyDraw/nImageHeight);
	cairo_set_source_surface(pCairo, pSurface, 0, 0);
	cairo_rectangle(pCairo, 0, 0, cxDraw, cyDraw);
	cairo_fill(pCairo);
	cairo_restore(pCairo);
	cairo_surface_destroy(pSurface);
}

#endif // X88_PRINTER_DRAW

#endif // X88_GUI

#ifdef X88_GUI_WINDOWS

// draw sprocket-hole
//
//     hdc
//         device-context
//     pPrinter
//         target printer
//     nPage
//         target page number
//     prectClip
//         clipping rectangle
//         (NULL: not clipping)

void CX88PrinterDrawer::DrawSprocketHole(
	HDC hdc,
	const CParallelPrinter* pPrinter,
	int /*nPage*/,
	const RECT* prectClip)
{
	if (pPrinter->IsExistSheetFeeder()) {
		return;
	}
	int nPaperWidth = pPrinter->GetPaperWidth(),
		nPaperHeight = pPrinter->GetPaperHeight();
	int nGap = pPrinter->GetDPI()/2,
		nDiameter = (pPrinter->GetDPI()*40)/254;
	RECT rectIntersect;
	HPEN hpenOrg = (HPEN)SelectObject(
		hdc, GetStockObject(BLACK_PEN));
	HBRUSH hbrushOrg = (HBRUSH)SelectObject(
		hdc, GetStockObject(NULL_BRUSH));
	for (int i = 0; i < 2; i++) {
		RECT rectSprocketHole;
		if (i == 0) {
			rectSprocketHole.left = (nGap-nDiameter)/2;
			rectSprocketHole.right = rectSprocketHole.left+nDiameter;
		} else {
			rectSprocketHole.right = nPaperWidth-(nGap-nDiameter)/2;
			rectSprocketHole.left = rectSprocketHole.right-nDiameter;
		}
		rectSprocketHole.top = (nGap-nDiameter)/2;
		rectSprocketHole.bottom = rectSprocketHole.top+nDiameter;
		while (rectSprocketHole.top < nPaperHeight) {
			if ((prectClip == NULL) ||
				IntersectRect(
					&rectIntersect, &rectSprocketHole, prectClip))
			{
				Ellipse(
					hdc,
					rectSprocketHole.left, rectSprocketHole.top,
					rectSprocketHole.right, rectSprocketHole.bottom);
			}
			rectSprocketHole.top += nGap;
			rectSprocketHole.bottom += nGap;
		}
	}
	DeleteObject(SelectObject(hdc, hpenOrg));
	DeleteObject(SelectObject(hdc, hbrushOrg));
}

// draw printer-head position
//
//     hdc
//         device-context
//     pPrinter
//         target printer
//     nPage
//         target page number
//     prectClip
//         clipping rectangle
//         (NULL: not clipping)

void CX88PrinterDrawer::DrawPrinterHead(
	HDC hdc,
	const CParallelPrinter* pPrinter,
	int nPage,
	const RECT* prectClip)
{
	if (nPage != pPrinter->GetCurrentPage()) {
		return;
	}
	RECT rectHead;
	rectHead.left = 0;
	rectHead.top = pPrinter->GetHeadY()-pPrinter->GetPaperTop();
	rectHead.right = pPrinter->GetPaperWidth();
	rectHead.bottom = rectHead.top+pPrinter->GetHeadHeight();
	RECT rectIntersect;
	if ((prectClip != NULL) &&
		!IntersectRect(
			&rectIntersect, &rectHead, prectClip))
	{
		return;
	}
	HPEN hpenHead = CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
	HPEN hpenOrg = (HPEN)SelectObject(hdc, hpenHead);
	HBRUSH hbrushHead = CreateHatchBrush(HS_FDIAGONAL, RGB(255, 0, 0));
	HBRUSH hbrushOrg = (HBRUSH)SelectObject(hdc, hbrushHead);
	SetBkMode(hdc, TRANSPARENT);
	Rectangle(
		hdc,
		rectHead.left, rectHead.top,
		rectHead.right, rectHead.bottom);
	DeleteObject(SelectObject(hdc, hpenOrg));
	DeleteObject(SelectObject(hdc, hbrushOrg));
}

// draw paper-border
//
//     hdc
//         device-context
//     pPrinter
//         target printer
//     nPage
//         target page number
//     prectClip
//         clipping rectangle
//         (NULL: not clipping)

void CX88PrinterDrawer::DrawPaperBorder(
	HDC hdc,
	const CParallelPrinter* pPrinter,
	int /*nPage*/,
	const RECT* /*prectClip*/)
{
	HPEN hpenOrg = (HPEN)SelectObject(
		hdc, GetStockObject(BLACK_PEN));
	HBRUSH hbrushOrg = (HBRUSH)SelectObject(
		hdc, GetStockObject(NULL_BRUSH));
	Rectangle(
		hdc,
		0, 0,
		pPrinter->GetPaperWidth(), pPrinter->GetPaperHeight());
	SelectObject(hdc, hpenOrg);
	SelectObject(hdc, hbrushOrg);
}

// draw printer object
//
//     hdc
//         device-context
//     pPrinter
//         target printer
//     nPage
//         target page number
//     prectClip
//         clipping rectangle
//         (NULL: not clipping)

void CX88PrinterDrawer::DrawPrinterObject(
	HDC hdc,
	const CParallelPrinter* pPrinter,
	int nPage,
	const RECT* prectClip)
{
	if (nPage >= (int)pPrinter->size()) {
		return;
	}
	int nPaperLeft = pPrinter->GetPaperLeft(),
		nPaperTop = pPrinter->GetPaperTop();
	CParallelPrinter::const_iterator itPage = pPrinter->begin();
	std::advance(itPage, nPage);
	RECT rectIntersect;
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkColor(hdc, RGB(255, 255, 255));
	CPrinterPage::const_iterator itObject;
	for (
		itObject = (*itPage)->begin();
		itObject != (*itPage)->end();
		itObject++)
	{
		RECT rectObj;
		rectObj.left = (*itObject)->GetX()-nPaperLeft;
		rectObj.top = (*itObject)->GetY()-nPaperTop;
		rectObj.right = rectObj.left+(*itObject)->GetWidth();
		rectObj.bottom = rectObj.top+(*itObject)->GetHeight();
		if ((prectClip != NULL) &&
			!IntersectRect(
				&rectIntersect, &rectObj, prectClip))
		{
			continue;
		}
		if ((*itObject)->GetObjectType() == CPrinterObject::POBJ_IMAGE) {
			const CPrinterImageObject* pobjImage =
				(const CPrinterImageObject*)(*itObject);
			DrawMonoBitmap(
				hdc,
				pobjImage->GetX()-nPaperLeft,
					pobjImage->GetY()-nPaperTop,
				pobjImage->GetWidth(), pobjImage->GetHeight(),
				pobjImage,
				false);
		}
	}
	HPEN hpenOrg = (HPEN)SelectObject(
		hdc, GetStockObject(BLACK_PEN));
	HBRUSH hbrushOrg = (HBRUSH)SelectObject(
		hdc, GetStockObject(BLACK_BRUSH));
	HFONT hfontOrg = NULL;
	LOGFONT lfSelect;
	memset(&lfSelect, 0, sizeof(lfSelect));
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	SetPolyFillMode(hdc, WINDING);
	for (
		itObject = (*itPage)->begin();
		itObject != (*itPage)->end();
		itObject++)
	{
		RECT rectObj;
		rectObj.left = (*itObject)->GetX()-nPaperLeft;
		rectObj.top = (*itObject)->GetY()-nPaperTop;
		rectObj.right = rectObj.left+(*itObject)->GetWidth();
		rectObj.bottom = rectObj.top+(*itObject)->GetHeight();
		if ((prectClip != NULL) &&
			!IntersectRect(
				&rectIntersect, &rectObj, prectClip))
		{
			continue;
		}
		if ((*itObject)->GetObjectType() == CPrinterObject::POBJ_TEXT) {
			const CPrinterTextObject* pobjText =
				(const CPrinterTextObject*)(*itObject);
			int nLength = pobjText->GetTextLength();
			if (nLength <= 0) {
				continue;
			}
			LOGFONT lfNew;
			memset(&lfNew, 0, sizeof(lfNew));
			lfNew.lfHeight = pobjText->GetCharHeight();
			lfNew.lfPitchAndFamily = FIXED_PITCH;
			int nFontType =pobjText->GetFontType();
			if ((nFontType & CPrinterTextObject::FONT_STRONG) != 0) {
				lfNew.lfWeight = FW_BOLD;
			}
			if ((nFontType & CPrinterTextObject::FONT_ITALIC) != 0) {
				lfNew.lfItalic = TRUE;
			}
			bool bVert = ((nFontType & CPrinterTextObject::FONT_VERT) != 0);
			switch (nFontType & CPrinterTextObject::FONT_KIND) {
			case CPrinterTextObject::FONT_MINCHO:
				if (!bVert) {
					strcat(lfNew.lfFaceName, RSTR("MS Mincho"));
				} else {
					strcat(lfNew.lfFaceName, RSTR("@MS Mincho"));
				}
				lfNew.lfCharSet = SHIFTJIS_CHARSET;
				break;
			case CPrinterTextObject::FONT_GOTHIC:
				if (!bVert) {
					strcat(lfNew.lfFaceName, RSTR("MS Gothic"));
				} else {
					strcat(lfNew.lfFaceName, RSTR("@MS Gothic"));
				}
				lfNew.lfCharSet = SHIFTJIS_CHARSET;
				break;
			case CPrinterTextObject::FONT_COURIER:
				strcpy(lfNew.lfFaceName, "Courier New");
				break;
			}
			std::vector<char> vectChar;
			std::vector<int> vectDX;
			vectDX.push_back(0);
			bool bFontChecked = false;
			int xDraw = pobjText->GetX()-nPaperLeft,
				yDraw = pobjText->GetY()-nPaperTop,
				cxDraw = 0,
				nGaijiIndex = 0;
			for (int i = 0; i <= nLength; i++) {
				uint16_t wChar = 0;
				int nCharWidth = 0,
					nLeftGap = 0,
					nRightGap = 0;
				if (i < nLength) {
					uint16_t wText = pobjText->GetText()[i];
					nCharWidth = pobjText->GetCharWidth(i);
					nLeftGap = pobjText->GetLeftGap(i);
					nRightGap = pobjText->GetRightGap(i);
					switch (pobjText->GetCharType(i)) {
					case CPrinterTextObject::CHAR_ANK:
						if (((wText >= 0x20) && (wText <= 0x7E)) ||
							((wText >= 0xA1) && (wText <= 0xDF)))
						{
							wChar = wText;
						}
						break;
					case CPrinterTextObject::CHAR_ASCII:
						if ((wText >= 0x20) && (wText <= 0x7E)) {
							wChar = (uint16_t)JIS2SJIS(
								JISHan2Zen(wText));
						}
						break;
					case CPrinterTextObject::CHAR_KATAKANA:
						if ((wText >= 0x21) && (wText <= 0x5F)) {
							wChar = (uint16_t)JIS2SJIS(
								JISHan2Zen(wText+0x80));
						}
						break;
					case CPrinterTextObject::CHAR_HIRAGANA:
						if ((wText >= 0x21) && (wText <= 0x5F)) {
							unsigned nCode =
								JISHan2Zen(wText+0x80);
							if ((nCode >= 0x2521) && (nCode <= 0x2573)) {
								nCode -= 0x0100;
							}
							wChar = (uint16_t)JIS2SJIS(nCode);
						}
						break;
					case CPrinterTextObject::CHAR_CG:
						wChar = DrawCGCharacter(
							hdc,
							xDraw+cxDraw+nLeftGap,
							yDraw,
							nCharWidth,
							pobjText->GetHeight(),
							wText);
						break;
					case CPrinterTextObject::CHAR_KANJI:
						wChar = (uint16_t)JIS2SJIS(wText);
						break;
					case CPrinterTextObject::CHAR_GAIJI:
						if (nGaijiIndex < pobjText->GetGaijiCount()) {
							DrawMonoBitmap(
								hdc,
								xDraw+cxDraw+nLeftGap,
								yDraw,
								nCharWidth,
								pobjText->GetHeight(),
								pobjText->GetGaiji(nGaijiIndex),
								bVert);
						}
						nGaijiIndex++;
						break;
					}
				} else {
					wChar = 1;
				}
				if (wChar == 0) {
					vectDX.back() += nLeftGap+nCharWidth+nRightGap;
				} else {
					vectDX.back() += nLeftGap;
					int nNewFontWidth = nCharWidth;
					if (wChar > 0xFF) {
						nNewFontWidth /= 2;
					}
					if ((lfNew.lfWidth != 0) &&
						(nNewFontWidth != lfNew.lfWidth))
					{
						if (bFontChecked ||
							(memcmp(&lfNew, &lfSelect, sizeof(lfNew)) != 0))
						{
							HFONT hfontNew = CreateFontIndirect(&lfNew);
							if (hfontNew != NULL) {
								HFONT hfontOld =
									(HFONT)SelectObject(hdc, hfontNew);
								if (hfontOrg == NULL) {
									hfontOrg = hfontOld;
								} else {
									DeleteObject(hfontOld);
								}
							}
							bFontChecked = true;
							lfSelect = lfNew;
						}
						if (vectChar.size() > 0) {
							ExtTextOut(
								hdc,
								xDraw+vectDX[0],
								yDraw,
								0,
								&rectObj,
								&vectChar[0],
								(int)vectChar.size(),
								&vectDX[1]);
						}
						xDraw += cxDraw;
						cxDraw = 0;
						vectChar.clear();
						vectDX.clear();
						vectDX.push_back(nLeftGap);
					}
					lfNew.lfWidth = nNewFontWidth;
					int nDX = nCharWidth;
					if (wChar > 0xFF) {
						vectChar.push_back((char)(wChar >> 8));
						vectDX.push_back(nDX/2);
						nDX -= nDX/2;
					}
					vectChar.push_back((char)(wChar & 0xFF));
					vectDX.push_back(nDX+nRightGap);
				}
				cxDraw += nLeftGap+nCharWidth+nRightGap;
			}
			int nLineType = pobjText->GetLineType(),
				nLineWidth = nLineType & CPrinterTextObject::LINE_WIDTH;
			if (nLineWidth != 0) {
				xDraw = pobjText->GetX()-nPaperLeft;
				yDraw = pobjText->GetY()-nPaperTop;
				switch (nLineType & CPrinterTextObject::LINE_KIND) {
				case CPrinterTextObject::LINE_UPPER:
					Rectangle(
						hdc,
						xDraw,
						yDraw,
						xDraw+pobjText->GetWidth(),
						yDraw+nLineWidth);
					break;
				case CPrinterTextObject::LINE_UNDER:
					Rectangle(
						hdc,
						xDraw,
						yDraw+pobjText->GetHeight()-nLineWidth,
						xDraw+pobjText->GetWidth(),
						yDraw+pobjText->GetHeight());
					break;
				}
			}
		}
	}
	if (hfontOrg != NULL) {
		DeleteObject(SelectObject(hdc, hfontOrg));
	}
	SelectObject(hdc, hpenOrg);
	SelectObject(hdc, hbrushOrg);
}

#elif defined(X88_GUI_GTK)

#ifdef X88_PRINTER_DRAW_POSTSCRIPT

// initialize PostScript
//
//     pFile
//         file object
//     pPrinter
//         target printer
//     nZoomOut
//         zoom-out rate
//     return value
//         OK / NG

bool CX88PrinterDrawer::InitializePostScript(
	FILE* pFile,
	const CParallelPrinter* pPrinter,
	int nZoomOut)
{
	fprintf(pFile, "%%!PS-Adobe-2.0\n");
	fprintf(pFile, "%%%% Creator: X88000\n");
	fprintf(pFile, "%%%% Title: PC-PR201 Output\n");
	fprintf(pFile, "%%%% Pages: 1\n");
	fprintf(pFile, "%%%% LanguageLevel: 2\n");
	fprintf(pFile, "%%%% DocumentFonts: Courier\n");
	fprintf(pFile, "%%%%+ Ryumin-Light-H\n");
	fprintf(pFile, "%%%%+ GothicBBB-Medium-H\n");
	fprintf(pFile, "%%%%+ Ryumin-Light-V\n");
	fprintf(pFile, "%%%%+ GothicBBB-Medium-V\n");
	fprintf(pFile,
		"%%%% BoundingBox: 0 0 %d %d\n",
		(pPrinter->GetPaperWidth()*72)/(pPrinter->GetDPI() << nZoomOut),
		(pPrinter->GetPaperHeight()*72)/(pPrinter->GetDPI() << nZoomOut));
	fprintf(pFile, "%%%% EndComments\n");
	fprintf(pFile, "\n");
	// Paper Size
	fprintf(pFile, "%% Paper Size\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "[{\n");
	fprintf(pFile, "  2 dict dup /PageSize\n");
	fprintf(
		pFile, "  [%d %d]\n",
		(pPrinter->GetPaperWidth()*72)/(pPrinter->GetDPI() << nZoomOut),
		(pPrinter->GetPaperHeight()*72)/(pPrinter->GetDPI() << nZoomOut));
	fprintf(pFile, "  put dup /ImagingBBox null put setpagedevice\n");
	fprintf(pFile, "} stopped cleartomark\n");
	fprintf(pFile, "\n");
	// Font
	fprintf(pFile, "%% Font\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "/SETFONT_Sub {\n");
	fprintf(pFile, "  /FONTNAME exch def\n");
	fprintf(pFile, "  /FONTRATIO exch def\n");
	fprintf(pFile, "  /FONTHORZ exch def\n");
	fprintf(pFile, "  /FONTITALIC exch def\n");
	fprintf(pFile, "  /FONTHEIGHT exch def\n");
	fprintf(pFile, "  /FONTWIDTH exch def\n");
	fprintf(pFile, "  /FONTTRUEWIDTH FONTWIDTH FONTRATIO div def\n");
	fprintf(pFile, "  /FONTASCENT 88 100 div FONTHEIGHT mul def\n");
	fprintf(pFile, "  /FONTSLANT\n");
	fprintf(pFile, "    FONTITALIC {\n");
	fprintf(pFile, "      FONTHEIGHT 0.25 mul\n");
	fprintf(pFile, "    } {\n");
	fprintf(pFile, "      0\n");
	fprintf(pFile, "    } ifelse\n");
	fprintf(pFile, "    def\n");
	fprintf(pFile, "  FONTNAME findfont\n");
	fprintf(pFile, "  FONTHORZ {\n");
	fprintf(
		pFile,
		"    [FONTTRUEWIDTH 0 FONTSLANT FONTHEIGHT neg "
		"0 FONTASCENT]\n");
	fprintf(pFile, "  } {\n");
	fprintf(
		pFile,
		"    [FONTSLANT FONTHEIGHT neg FONTTRUEWIDTH neg 0 "
		"0 FONTHEIGHT 2 div]\n");
	fprintf(pFile, "  } ifelse\n");
	fprintf(pFile, "  makefont setfont\n");
	fprintf(pFile, "} def\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "/SETFONT_Mincho {\n");
	fprintf(pFile, "  true 1 /Ryumin-Light-H SETFONT_Sub\n");
	fprintf(pFile, "} def\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "/SETFONT_Gothic {\n");
	fprintf(pFile, "  true 1 /GothicBBB-Medium-H SETFONT_Sub\n");
	fprintf(pFile, "} def\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "/SETFONT_MinchoV {\n");
	fprintf(pFile, "  false 1 /Ryumin-Light-V SETFONT_Sub\n");
	fprintf(pFile, "} def\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "/SETFONT_GothicV {\n");
	fprintf(pFile, "  false 1 /GothicBBB-Medium-V SETFONT_Sub\n");
	fprintf(pFile, "} def\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "/COURIERRATIO 0 def\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "/SETFONT_Courier {\n");
	fprintf(pFile, "  COURIERRATIO 0 eq {\n");
	fprintf(pFile, "    24 24 false true 1 /Courier SETFONT_Sub\n");
	fprintf(pFile, "    /COURIERRATIO (0) stringwidth pop 24 div def\n");
	fprintf(pFile, "  } if\n");
	fprintf(pFile, "  true COURIERRATIO /Courier SETFONT_Sub\n");
	fprintf(pFile, "} def\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "%% Polygon\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "/POLYGON {\n");
	fprintf(pFile, "  /POINTINDEX 0 def\n");
	fprintf(pFile, "  {\n");
	fprintf(pFile, "    POINTINDEX 2 mod 0 ne {\n");
	fprintf(pFile, "      POINTINDEX 2 lt {\n");
	fprintf(pFile, "        moveto\n");
	fprintf(pFile, "      } {\n");
	fprintf(pFile, "        lineto\n");
	fprintf(pFile, "      } ifelse\n");
	fprintf(pFile, "    } if\n");
	fprintf(pFile, "    /POINTINDEX POINTINDEX 1 add def\n");
	fprintf(pFile, "  } forall\n");
	fprintf(pFile, "  closepath\n");
	fprintf(pFile, "} def\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "%%%% EndProlog\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "%%%% Page: 1 1\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "gsave\n");
	fprintf(pFile, "\n");
	// Scale & Translate
	fprintf(pFile, "%% Scale & Translate\n");
	fprintf(pFile, "\n");
	fprintf(
		pFile, "72 %d div -72 %d div scale\n",
		pPrinter->GetDPI() << nZoomOut,
		pPrinter->GetDPI() << nZoomOut);
	fprintf(
		pFile, "0 %d -1 mul translate\n",
		pPrinter->GetPaperHeight());
	fprintf(pFile, "\n");
	// Clip
	fprintf(pFile, "%% Clip\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "newpath\n");
	fprintf(
		pFile, "[%d %d %d %d %d %d %d %d]\n",
		0, 0,
		pPrinter->GetPaperWidth(), 0,
		pPrinter->GetPaperWidth(), pPrinter->GetPaperHeight(),
		0, pPrinter->GetPaperHeight());
	fprintf(pFile, "POLYGON\n");
	fprintf(pFile, "clip\n");
	fprintf(pFile, "newpath\n");
	fprintf(pFile, "\n");
	// Draw
	fprintf(pFile, "%% Draw\n");
	fprintf(pFile, "\n");
	return true;
}

// finalize PostScript
//
//     pFile
//         file object
//     pPrinter
//         target printer
//     return value
//         OK / NG

bool CX88PrinterDrawer::FinalizePostScript(
	FILE* pFile,
	const CParallelPrinter* /*pPrinter*/)
{
	fprintf(pFile, "\n");
	fprintf(pFile, "%% Show\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "grestore\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "showpage\n");
	fprintf(pFile, "\n");
	fprintf(pFile, "%%%% Trailer\n");
	fprintf(pFile, "%%%% EOF\n");
	return true;
}

// draw sprocket-hole to PostScript
//
//     pFile
//         file object
//     pPrinter
//         target printer
//     nPage
//         target page number

bool CX88PrinterDrawer::DrawSprocketHoleToPostScript(
	FILE* pFile,
	const CParallelPrinter* pPrinter,
	int /*nPage*/)
{
	if (pPrinter->IsExistSheetFeeder()) {
		return true;
	}
	int nPaperWidth = pPrinter->GetPaperWidth(),
		nPaperHeight = pPrinter->GetPaperHeight();
	int nGap = pPrinter->GetDPI()/2,
		nDiameter = (pPrinter->GetDPI()*40)/254;
	fprintf(pFile, "1 setlinewidth\n");
	for (int i = 0; i < 2; i++) {
		int anSprocketHole[4];
		if (i == 0) {
			anSprocketHole[0] = (nGap-nDiameter)/2;
			anSprocketHole[2] = anSprocketHole[0]+nDiameter;
		} else {
			anSprocketHole[2] = nPaperWidth-(nGap-nDiameter)/2;
			anSprocketHole[0] = anSprocketHole[2]-nDiameter;
		}
		anSprocketHole[1] = (nGap-nDiameter)/2;
		anSprocketHole[3] = anSprocketHole[1]+nDiameter;
		while (anSprocketHole[1] < nPaperHeight) {
			fprintf(
				pFile, "newpath %lg %lg %lg 0 360 arc stroke\n",
				(anSprocketHole[0]+anSprocketHole[2])/2.0,
				(anSprocketHole[1]+anSprocketHole[3])/2.0,
				(anSprocketHole[2]-anSprocketHole[0])/2.0);
			anSprocketHole[1] += nGap;
			anSprocketHole[3] += nGap;
		}
	}
	return true;
}

// draw printer-head position to PostScript
//
//     pFile
//         file object
//     pPrinter
//         target printer
//     nPage
//         target page number
//     return value
//         OK / NG

bool CX88PrinterDrawer::DrawPrinterHeadToPostScript(
	FILE* pFile,
	const CParallelPrinter* pPrinter,
	int nPage)
{
	if (nPage != pPrinter->GetCurrentPage()) {
		return true;
	}
	int anHead[4];
	anHead[0] = 0;
	anHead[1] = pPrinter->GetHeadY()-pPrinter->GetPaperTop();
	anHead[2] = pPrinter->GetPaperWidth();
	anHead[3] = anHead[1]+pPrinter->GetHeadHeight();
	fprintf(pFile, "0.875 setgray\n");
	fprintf(
		pFile, "[%d %d %d %d %d %d %d %d]\n",
		anHead[0], anHead[1],
		anHead[2], anHead[1],
		anHead[2], anHead[3],
		anHead[0], anHead[3]);
	fprintf(pFile, "POLYGON\n");
	fprintf(pFile, "fill\n");
	fprintf(pFile, "0 setgray\n");
	fprintf(pFile, "1 setlinewidth\n");
	fprintf(
		pFile, "[%d %d %d %d %d %d %d %d]\n",
		anHead[0], anHead[1],
		anHead[2], anHead[1],
		anHead[2], anHead[3],
		anHead[0], anHead[3]);
	fprintf(pFile, "POLYGON\n");
	fprintf(pFile, "stroke\n");
	return true;
}

// draw paper-border to PostScript
//
//     pFile
//         file object
//     pPrinter
//         target printer
//     nPage
//         target page number
//     return value
//         OK / NG

bool CX88PrinterDrawer::DrawPaperBorderToPostScript(
	FILE* pFile,
	const CParallelPrinter* pPrinter,
	int /*nPage*/)
{
	fprintf(pFile, "2 setlinewidth\n");
	fprintf(
		pFile, "[%d %d %d %d %d %d %d %d]\n",
		1, 1,
		pPrinter->GetPaperWidth()-1, 1,
		pPrinter->GetPaperWidth()-1, pPrinter->GetPaperHeight()-1,
		1, pPrinter->GetPaperHeight()-1);
	fprintf(pFile, "POLYGON\n");
	fprintf(pFile, "stroke\n");
	fprintf(pFile, "\n");
	return true;
}

// draw printer object to PostScript
//
//     pFile
//         file object
//     pPrinter
//         target printer
//     nPage
//         target page number
//     return value
//         OK / NG

bool CX88PrinterDrawer::DrawPrinterObjectToPostScript(
	FILE* pFile,
	const CParallelPrinter* pPrinter,
	int nPage)
{
	if (nPage >= (int)pPrinter->size()) {
		return true;
	}
	int nPaperLeft = pPrinter->GetPaperLeft(),
		nPaperTop = pPrinter->GetPaperTop();
	CParallelPrinter::const_iterator itPage = pPrinter->begin();
	std::advance(itPage, nPage);
	CPrinterPage::const_iterator itObject;
	for (
		itObject = (*itPage)->begin();
		itObject != (*itPage)->end();
		itObject++)
	{
		if ((*itObject)->GetObjectType() == CPrinterObject::POBJ_IMAGE) {
			const CPrinterImageObject* pobjImage =
				(const CPrinterImageObject*)(*itObject);
			DrawMonoBitmapToPostScript(
				pFile,
				pobjImage->GetX()-nPaperLeft,
					pobjImage->GetY()-nPaperTop,
				pobjImage->GetWidth(), pobjImage->GetHeight(),
				pobjImage,
				false);
		}
	}
	struct SLogFont {
		int lfWidth;
		int lfHeight;
		bool lfItalic;
		const char* lfFaceName;
	} lfSelect;
	memset(&lfSelect, 0, sizeof(lfSelect));
	for (
		itObject = (*itPage)->begin();
		itObject != (*itPage)->end();
		itObject++)
	{
		if ((*itObject)->GetObjectType() == CPrinterObject::POBJ_TEXT) {
			const CPrinterTextObject* pobjText =
				(const CPrinterTextObject*)(*itObject);
			int nLength = pobjText->GetTextLength();
			if (nLength <= 0) {
				continue;
			}
			SLogFont lfNew;
			memset(&lfNew, 0, sizeof(lfNew));
			bool bBold = false;
			lfNew.lfHeight = pobjText->GetCharHeight();
			int nFontType =pobjText->GetFontType();
			if ((nFontType & CPrinterTextObject::FONT_STRONG) != 0) {
				bBold = true;
			}
			lfNew.lfItalic =
				((nFontType & CPrinterTextObject::FONT_ITALIC) != 0);
			bool bVert = ((nFontType & CPrinterTextObject::FONT_VERT) != 0);
			bool bZenkakuFont = false;
			switch (nFontType & CPrinterTextObject::FONT_KIND) {
			case CPrinterTextObject::FONT_MINCHO:
				if (!bVert) {
					lfNew.lfFaceName = "SETFONT_Mincho";
				} else {
					lfNew.lfFaceName = "SETFONT_MinchoV";
				}
				bZenkakuFont = true;
				break;
			case CPrinterTextObject::FONT_GOTHIC:
				if (!bVert) {
					lfNew.lfFaceName = "SETFONT_Gothic";
				} else {
					lfNew.lfFaceName = "SETFONT_GothicV";
				}
				bZenkakuFont = true;
				break;
			default:
				lfNew.lfFaceName = "SETFONT_Courier";
				break;
			}
			std::vector<uint16_t> vectChar;
			std::vector<int> vectDX;
			vectDX.push_back(0);
			bool bFontChecked = false;
			int xDraw = pobjText->GetX()-nPaperLeft,
				yDraw = pobjText->GetY()-nPaperTop,
				cxDraw = 0,
				nGaijiIndex = 0;
			for (int i = 0; i <= nLength; i++) {
				uint16_t wChar = 0;
				int nCharWidth = 0,
					nLeftGap = 0,
					nRightGap = 0;
				if (i < nLength) {
					uint16_t wText = pobjText->GetText()[i];
					nCharWidth = pobjText->GetCharWidth(i);
					nLeftGap = pobjText->GetLeftGap(i);
					nRightGap = pobjText->GetRightGap(i);
					switch (pobjText->GetCharType(i)) {
					case CPrinterTextObject::CHAR_ANK:
						if (bZenkakuFont) {
							wChar = (uint16_t)JISHan2Zen(wText);
						} else if ((wText >= 0x20) && (wText <= 0x7E)) {
							wChar = wText;
						}
						break;
					case CPrinterTextObject::CHAR_ASCII:
						if ((wText >= 0x20) && (wText <= 0x7E)) {
							wChar = (uint16_t)JISHan2Zen(wText);
						}
						break;
					case CPrinterTextObject::CHAR_KATAKANA:
						if (bZenkakuFont &&
							(wText >= 0x21) && (wText <= 0x5F))
						{
							wChar = (uint16_t)JISHan2Zen(
								wText+0x80);
						}
						break;
					case CPrinterTextObject::CHAR_HIRAGANA:
						if (bZenkakuFont &&
							(wText >= 0x21) && (wText <= 0x5F))
						{
							unsigned nCode =
								JISHan2Zen(wText+0x80);
							if ((nCode >= 0x2521) && (nCode <= 0x2573)) {
								nCode -= 0x0100;
							}
							wChar = (uint16_t)nCode;
						}
						break;
					case CPrinterTextObject::CHAR_CG:
						wChar = DrawCGCharacterToPostScript(
							pFile,
							xDraw+cxDraw+nLeftGap,
							yDraw,
							nCharWidth,
							pobjText->GetHeight(),
							wText);
						if (!bZenkakuFont) {
							wChar = 0;
						}
						break;
					case CPrinterTextObject::CHAR_KANJI:
						wChar = wText;
						break;
					case CPrinterTextObject::CHAR_GAIJI:
						if (nGaijiIndex < pobjText->GetGaijiCount()) {
							DrawMonoBitmapToPostScript(
								pFile,
								xDraw+cxDraw+nLeftGap,
								yDraw,
								nCharWidth,
								pobjText->GetHeight(),
								pobjText->GetGaiji(nGaijiIndex),
								bVert);
						}
						nGaijiIndex++;
						break;
					}
				} else {
					wChar = 1;
				}
				if (wChar == 0) {
					vectDX.back() += nLeftGap+nCharWidth+nRightGap;
				} else {
					vectDX.back() += nLeftGap;
					int nNewFontWidth = nCharWidth;
					if ((lfNew.lfWidth != 0) &&
						(nNewFontWidth != lfNew.lfWidth))
					{
						if (bFontChecked ||
							(memcmp(&lfNew, &lfSelect, sizeof(lfNew)) != 0))
						{
							fprintf(
								pFile, "%d %d %s %s\n",
								lfNew.lfWidth,
								lfNew.lfHeight,
								lfNew.lfItalic? "true": "false",
								lfNew.lfFaceName);
							bFontChecked = true;
							lfSelect = lfNew;
						}
						if (bZenkakuFont) {
							fprintf(pFile, "<");
							for (int i = 0; i < (int)vectChar.size(); i++) {
								fprintf(pFile, "%04X", vectChar[i]);
							}
							fprintf(pFile, ">\n");
						} else {
							fprintf(pFile, "(");
							for (int i = 0; i < (int)vectChar.size(); i++) {
								char ch = (char)vectChar[i];
								switch (ch) {
								case '(':
								case ')':
								case '\\':
									fputc('\\', pFile);
									// no break
								default:
									fputc(ch, pFile);
									break;
								}
							}
							fprintf(pFile, ")\n");
						}
						fprintf(pFile, "[");
						for (int i = 1; i < (int)vectDX.size(); i++) {
							fprintf(pFile, "%d", vectDX[i]);
							if (i < (int)vectDX.size()-1) {
								fputc(' ', pFile);
							}
						}
						fprintf(pFile, "]\n");
						fprintf(pFile, "%d %d ", xDraw+vectDX[0], yDraw);
						if (bBold) {
							fprintf(pFile, "1 index 1 index moveto\n");
							fprintf(pFile, "3 index 3 index xshow\n");
							fprintf(pFile, "exch 1 add exch ");
						}
						fprintf(pFile, "moveto\n");
						fprintf(pFile, "xshow\n");
						xDraw += cxDraw;
						cxDraw = 0;
						vectChar.clear();
						vectDX.clear();
						vectDX.push_back(nLeftGap);
					}
					lfNew.lfWidth = nNewFontWidth;
					int nDX = nCharWidth;
					vectChar.push_back(wChar);
					vectDX.push_back(nDX+nRightGap);
				}
				cxDraw += nLeftGap+nCharWidth+nRightGap;
			}
			int nLineType = pobjText->GetLineType(),
				nLineWidth = nLineType & CPrinterTextObject::LINE_WIDTH;
			if (nLineWidth != 0) {
				xDraw = pobjText->GetX()-nPaperLeft;
				yDraw = pobjText->GetY()-nPaperTop;
				switch (nLineType & CPrinterTextObject::LINE_KIND) {
				case CPrinterTextObject::LINE_UPPER:
					fprintf(
						pFile, "%d %d %d dup setlinewidth 2 div add moveto\n",
						xDraw, yDraw, nLineWidth);
					break;
				case CPrinterTextObject::LINE_UNDER:
					fprintf(
						pFile, "%d %d %d dup setlinewidth 2 div sub moveto\n",
						xDraw, yDraw+pobjText->GetHeight(), nLineWidth);
					break;
				}
				fprintf(
					pFile, "%d 0 rlineto\n",
					pobjText->GetWidth());
				fprintf(pFile, "stroke\n");
			}
		}
	}
	fprintf(pFile, "\n");
	return true;
}

#elif defined(X88_PRINTER_DRAW_CAIRO)

// draw sprocket-hole to cairo
//
//     pCairo
//         cairo context
//     pPrinter
//         target printer
//     nPage
//         target page number

void CX88PrinterDrawer::DrawSprocketHoleToCairo(
	cairo_t* pCairo,
	const CParallelPrinter* pPrinter,
	int /*nPage*/)
{
	if (pPrinter->IsExistSheetFeeder()) {
		return;
	}
	int nPaperWidth = pPrinter->GetPaperWidth(),
		nPaperHeight = pPrinter->GetPaperHeight();
	int nGap = pPrinter->GetDPI()/2,
		nDiameter = (pPrinter->GetDPI()*40)/254;
	cairo_set_line_width(pCairo, 1);
	for (int i = 0; i < 2; i++) {
		int anSprocketHole[4];
		if (i == 0) {
			anSprocketHole[0] = (nGap-nDiameter)/2;
			anSprocketHole[2] = anSprocketHole[0]+nDiameter;
		} else {
			anSprocketHole[2] = nPaperWidth-(nGap-nDiameter)/2;
			anSprocketHole[0] = anSprocketHole[2]-nDiameter;
		}
		anSprocketHole[1] = (nGap-nDiameter)/2;
		anSprocketHole[3] = anSprocketHole[1]+nDiameter;
		while (anSprocketHole[1] < nPaperHeight) {
			cairo_arc(
				pCairo,
				(anSprocketHole[0]+anSprocketHole[2])/2.0,
				(anSprocketHole[1]+anSprocketHole[3])/2.0,
				(anSprocketHole[2]-anSprocketHole[0])/2.0,
				0, M_PI*2);
			cairo_stroke(pCairo);
			anSprocketHole[1] += nGap;
			anSprocketHole[3] += nGap;
		}
	}
}

// draw printer-head position to cairo
//
//     pCairo
//         cairo context
//     pPrinter
//         target printer
//     nPage
//         target page number

void CX88PrinterDrawer::DrawPrinterHeadToCairo(
	cairo_t* pCairo,
	const CParallelPrinter* pPrinter,
	int nPage)
{
	if (nPage != pPrinter->GetCurrentPage()) {
		return;
	}
	cairo_rectangle(
		pCairo,
		0,
		pPrinter->GetHeadY()-pPrinter->GetPaperTop(),
		pPrinter->GetPaperWidth(),
		pPrinter->GetHeadHeight());
	cairo_fill(pCairo);
}

// draw paper-border to cairo
//
//     pCairo
//         cairo context
//     pPrinter
//         target printer
//     nPage
//         target page number

void CX88PrinterDrawer::DrawPaperBorderToCairo(
	cairo_t* pCairo,
	const CParallelPrinter* pPrinter,
	int /*nPage*/)
{
	cairo_set_line_width(pCairo, 2);
	cairo_rectangle(
		pCairo,
		1, 1,
		pPrinter->GetPaperWidth()-2, pPrinter->GetPaperHeight()-2);
	cairo_stroke(pCairo);
}

// draw printer object to cairo
//
//     pCairo
//         cairo context
//     pPrinter
//         target printer
//     nPage
//         target page number

void CX88PrinterDrawer::DrawPrinterObjectToCairo(
	cairo_t* pCairo,
	const CParallelPrinter* pPrinter,
	int nPage)
{
	static const uint16_t awNotTurn[] = {
		0x2131, 0x2132, 0x213C, 0x213D, 0x213E, 0x2141, 0x2142, 0x2143,
		0x2144, 0x2145, 0x214A, 0x214B, 0x214C, 0x214D, 0x214E, 0x214F,
		0x2150, 0x2151, 0x2152, 0x2153, 0x2154, 0x2155, 0x2156, 0x2157,
		0x2158, 0x2159, 0x215A, 0x215B, 0x2161, 0x222A, 0x222B, 0x222C,
		0x222D, 0x222E, 0x2821, 0x2822, 0x2823, 0x2824, 0x2825, 0x2826,
		0x2827, 0x2828, 0x2829, 0x282A, 0x282B, 0x282C, 0x282D, 0x282E,
		0x282F, 0x2830, 0x2831, 0x2832, 0x2833, 0x2834, 0x2835, 0x2836,
		0x2837, 0x2838, 0x2839, 0x283A, 0x283B, 0x283C, 0x283D, 0x283E,
		0x283F, 0x2840,
		0
	};
	static const uint16_t awOfs1[] = {
		0x2421, 0x2423, 0x2425, 0x2427, 0x2429, 0x2443, 0x2463, 0x2465,
		0x2467, 0x246E, 0x2521, 0x2523, 0x2525, 0x2527, 0x2529, 0x2543,
		0x2563, 0x2565, 0x2567, 0x256E, 0x2575, 0x2576,
		0
	};
	static const uint16_t awOfs2[] = {
		0x2122, 0x2123, 0x2124, 0x2125,
		0
	};
	static std::set<uint16_t> setNotTurn;
	static std::set<uint16_t> setOfs1;
	static std::set<uint16_t> setOfs2;
	if (setNotTurn.empty()) {
		int i;
		for (i = 0; awNotTurn[i] != 0; i++) {
			setNotTurn.insert(awNotTurn[i]);
		}
		for (i = 0; awOfs1[i] != 0; i++) {
			setOfs1.insert(awOfs1[i]);
		}
		for (i = 0; awOfs2[i] != 0; i++) {
			setOfs2.insert(awOfs2[i]);
		}
	}

	if (nPage >= (int)pPrinter->size()) {
		return;
	}
	int nPaperLeft = pPrinter->GetPaperLeft(),
		nPaperTop = pPrinter->GetPaperTop();
	CParallelPrinter::const_iterator itPage = pPrinter->begin();
	std::advance(itPage, nPage);
	CPrinterPage::const_iterator itObject;
	for (
		itObject = (*itPage)->begin();
		itObject != (*itPage)->end();
		itObject++)
	{
		if ((*itObject)->GetObjectType() == CPrinterObject::POBJ_IMAGE) {
			const CPrinterImageObject* pobjImage =
				(const CPrinterImageObject*)(*itObject);
			DrawMonoBitmapToCairo(
				pCairo,
				pobjImage->GetX()-nPaperLeft,
					pobjImage->GetY()-nPaperTop,
				pobjImage->GetWidth(), pobjImage->GetHeight(),
				pobjImage,
				false);
		}
	}
	enum {
		LFATTR_BOLD   = 0x01,
		LFATTR_ITALIC = 0x02,
		LFATTR_VERT   = 0x04,
		LFATTR_OFS1   = 0x08,
		LFATTR_OFS2   = 0x10
	};
	struct SLogFont {
		int lfWidth;
		int lfHeight;
		uint8_t lfAttr;
		const char* lfFaceName;
	} lfSel;
	memset(&lfSel, 0, sizeof(lfSel));
	for (
		itObject = (*itPage)->begin();
		itObject != (*itPage)->end();
		itObject++)
	{
		if ((*itObject)->GetObjectType() == CPrinterObject::POBJ_TEXT) {
			const CPrinterTextObject* pobjText =
				(const CPrinterTextObject*)(*itObject);
			int nLength = pobjText->GetTextLength();
			if (nLength <= 0) {
				continue;
			}
			SLogFont lfPre, lfNow;
			memset(&lfPre, 0, sizeof(lfPre));
			memset(&lfNow, 0, sizeof(lfNow));
			lfNow.lfHeight = pobjText->GetCharHeight();
			int nFontType =pobjText->GetFontType();
			lfNow.lfAttr |=
				(((nFontType & CPrinterTextObject::FONT_STRONG) != 0)?
					LFATTR_BOLD: 0) |
				(((nFontType & CPrinterTextObject::FONT_ITALIC) != 0)?
					LFATTR_ITALIC: 0);
			bool bVert = ((nFontType & CPrinterTextObject::FONT_VERT) != 0);
			switch (nFontType & CPrinterTextObject::FONT_KIND) {
			case CPrinterTextObject::FONT_GOTHIC:
				lfNow.lfFaceName = RSTR("MS Gothic");
				break;
			default:
				lfNow.lfFaceName = RSTR("MS Mincho");
				break;
			}
			std::vector<uint16_t> vectChar;
			std::vector<int> vectDX;
			vectDX.push_back(0);
			int xDraw = pobjText->GetX()-nPaperLeft,
				yDraw = pobjText->GetY()-nPaperTop,
				cxDraw = 0,
				nGaijiIndex = 0;
			for (int i = 0; i <= nLength; i++) {
				uint16_t wChar = 0;
				int nCharWidth = 0,
					nLeftGap = 0,
					nRightGap = 0;
				if (i < nLength) {
					uint16_t wText = pobjText->GetText()[i];
					nCharWidth = pobjText->GetCharWidth(i);
					nLeftGap = pobjText->GetLeftGap(i);
					nRightGap = pobjText->GetRightGap(i);
					switch (pobjText->GetCharType(i)) {
					case CPrinterTextObject::CHAR_ANK:
						wChar = (uint16_t)JISHan2Zen(wText);
						break;
					case CPrinterTextObject::CHAR_ASCII:
						if ((wText >= 0x20) && (wText <= 0x7E)) {
							wChar = (uint16_t)JISHan2Zen(wText);
						}
						break;
					case CPrinterTextObject::CHAR_KATAKANA:
						if ((wText >= 0x21) && (wText <= 0x5F)) {
							wChar = (uint16_t)JISHan2Zen(
								wText+0x80);
						}
						break;
					case CPrinterTextObject::CHAR_HIRAGANA:
						if ((wText >= 0x21) && (wText <= 0x5F)) {
							unsigned nCode =
								JISHan2Zen(wText+0x80);
							if ((nCode >= 0x2521) && (nCode <= 0x2573)) {
								nCode -= 0x0100;
							}
							wChar = (uint16_t)nCode;
						}
						break;
					case CPrinterTextObject::CHAR_CG:
						wChar = DrawCGCharacterToCairo(
							pCairo,
							xDraw+cxDraw+nLeftGap,
							yDraw,
							nCharWidth,
							pobjText->GetHeight(),
							wText);
						break;
					case CPrinterTextObject::CHAR_KANJI:
						wChar = wText;
						break;
					case CPrinterTextObject::CHAR_GAIJI:
						if (nGaijiIndex < pobjText->GetGaijiCount()) {
							DrawMonoBitmapToCairo(
								pCairo,
								xDraw+cxDraw+nLeftGap,
								yDraw,
								nCharWidth,
								pobjText->GetHeight(),
								pobjText->GetGaiji(nGaijiIndex),
								bVert);
						}
						nGaijiIndex++;
						break;
					}
				} else {
					wChar = 1;
				}
				lfNow.lfWidth = nCharWidth;
				lfNow.lfAttr &= ~(LFATTR_VERT | LFATTR_OFS1 | LFATTR_OFS2);
				if (bVert) {
					if (setNotTurn.find(wChar) == setNotTurn.end()) {
						lfNow.lfAttr |= LFATTR_VERT;
					}
					if (setOfs1.find(wChar) != setOfs1.end()) {
						lfNow.lfAttr |= LFATTR_OFS1;
					}
					if (setOfs2.find(wChar) != setOfs2.end()) {
						lfNow.lfAttr |= LFATTR_OFS2;
					}
				}
				if (wChar == 0) {
					vectDX.back() += nLeftGap+nCharWidth+nRightGap;
				} else {
					if (memcmp(&lfNow, &lfPre, sizeof(lfNow)) != 0) {
						if (lfPre.lfWidth != 0) {
							if (memcmp(
									&lfPre, &lfSel, sizeof(lfPre)) != 0)
							{
								cairo_matrix_t matFont;
								cairo_matrix_init_scale(
									&matFont,
									lfPre.lfWidth, lfPre.lfHeight);
								cairo_select_font_face(
									pCairo,
									lfPre.lfFaceName,
									((lfPre.lfAttr & LFATTR_ITALIC) == 0)?
										CAIRO_FONT_SLANT_NORMAL:
										CAIRO_FONT_SLANT_ITALIC,
									((lfPre.lfAttr & LFATTR_BOLD) == 0)?
									CAIRO_FONT_WEIGHT_NORMAL:
									CAIRO_FONT_WEIGHT_BOLD);
								cairo_set_font_matrix(
									pCairo,
									&matFont);
								cairo_font_extents_t ext;
								cairo_font_extents(
									pCairo,
									&ext);
								if ((lfPre.lfAttr & LFATTR_VERT) != 0) {
									cairo_matrix_rotate(
										&matFont, -M_PI/2);
									cairo_matrix_translate(
										&matFont, -1, 0);
								}
								if ((lfPre.lfAttr & LFATTR_OFS1) != 0) {
									cairo_matrix_translate(
										&matFont, 0.1, -0.1);
								}
								if ((lfPre.lfAttr & LFATTR_OFS2) != 0) {
									cairo_matrix_translate(
										&matFont, 0.6, -0.6);
								}
								cairo_matrix_translate(
									&matFont,
									0, ext.ascent/ext.height);
								cairo_set_font_matrix(
									pCairo,
									&matFont);
								lfSel = lfPre;
							}
							int xDraw2 = xDraw;
							for (int i = 0; i < (int)vectChar.size(); i++) {
								xDraw2 += vectDX[i];
								unsigned ch = JIS2SJIS(vectChar[i]);
								std::string jstrChar;
								if (ch > 0xFF) {
									jstrChar = (char)(ch >> 8);
								}
								jstrChar += (char)(ch & 0xFF);
								std::string strChar = ConvSJIStoUTF8(
									jstrChar);
								cairo_move_to(
									pCairo,
									xDraw2, yDraw);
								cairo_show_text(
									pCairo,
									strChar.c_str());
							}
						}
						lfPre = lfNow;
						xDraw += cxDraw;
						cxDraw = 0;
						vectChar.clear();
						vectDX.clear();
						vectDX.push_back(0);
					}
					vectDX.back() += nLeftGap;
					vectChar.push_back(wChar);
					vectDX.push_back(nCharWidth+nRightGap);
				}
				cxDraw += nLeftGap+nCharWidth+nRightGap;
			}
			int nLineType = pobjText->GetLineType(),
				nLineWidth = nLineType & CPrinterTextObject::LINE_WIDTH;
			if (nLineWidth != 0) {
				xDraw = pobjText->GetX()-nPaperLeft;
				yDraw = pobjText->GetY()-nPaperTop;
				cairo_set_line_width(pCairo, nLineWidth);
				switch (nLineType & CPrinterTextObject::LINE_KIND) {
				case CPrinterTextObject::LINE_UPPER:
					cairo_move_to(
						pCairo,
						xDraw, yDraw+nLineWidth/2.0);
					break;
				case CPrinterTextObject::LINE_UNDER:
					cairo_move_to(
						pCairo,
						xDraw, yDraw+pobjText->GetHeight()-nLineWidth/2.0);
					break;
				}
				cairo_rel_line_to(
					pCairo,
					pobjText->GetWidth(), 0);
				cairo_stroke(pCairo);
			}
		}
	}
}

#endif // X88_PRINTER_DRAW

#endif // X88_GUI

// extract text
//
//     pPrinter
//         target printer
//     nPage
//         target page number
//     jstrText
//         extracted text from paper(SJIS encoding)
//     bCRLF
//         true  : line-break = CR+LF
//         false : line-break = LF

void CX88PrinterDrawer::ExtractText(
	const CParallelPrinter* pPrinter,
	int nPage,
	std::string& jstrText,
	bool bCRLF)
{
	jstrText = "";
	if (nPage >= (int)pPrinter->size()) {
		return;
	}
	CParallelPrinter::const_iterator itPage = pPrinter->begin();
	std::advance(itPage, nPage);
	bool bPageTop = true, bLineTop = true;
	int yPrev = 0;
	for (
		CPrinterPage::const_iterator itObject = (*itPage)->begin();
		true;
		itObject++)
	{
		bool bReturn = false;
		if (itObject == (*itPage)->end()) {
			if (!bLineTop) {
				bReturn = true;
			}
		} else if (
			(*itObject)->GetObjectType() == CPrinterObject::POBJ_TEXT)
		{
			if (!bPageTop && ((*itObject)->GetY() != yPrev)) {
				bReturn = true;
			}
		}
		if (bReturn) {
			if (bCRLF) {
				jstrText += "\r\n";
			} else {
				jstrText += '\n';
			}
			bPageTop = false;
			bLineTop = true;
		}
		if (itObject == (*itPage)->end()) {
			break;
		} else if (
			(*itObject)->GetObjectType() == CPrinterObject::POBJ_TEXT)
		{
			const CPrinterTextObject* pobjText =
				(const CPrinterTextObject*)(*itObject);
			yPrev = pobjText->GetY();
			int nLength = pobjText->GetTextLength();
			for (int i = 0; i < nLength; i++) {
				uint16_t wText = pobjText->GetText()[i],
					wChar = 0x20;
				switch (pobjText->GetCharType(i)) {
				case CPrinterTextObject::CHAR_NULL:
					wChar = 0;
					break;
				case CPrinterTextObject::CHAR_ANK:
					if (((wText >= 0x20) && (wText <= 0x7E)) ||
						((wText >= 0xA1) && (wText <= 0xDF)))
					{
						wChar = wText;
					}
					break;
				case CPrinterTextObject::CHAR_ASCII:
					if ((wText >= 0x20) && (wText <= 0x7E)) {
						wChar = wText;
					}
					break;
				case CPrinterTextObject::CHAR_KATAKANA:
				case CPrinterTextObject::CHAR_HIRAGANA:
					if ((wText >= 0x21) && (wText <= 0x5F)) {
						wChar = (uint16_t)(wText+0x80);
					}
					break;
				case CPrinterTextObject::CHAR_CG:
					break;
				case CPrinterTextObject::CHAR_KANJI:
					wChar = (uint16_t)JIS2SJIS(wText);
					break;
				case CPrinterTextObject::CHAR_GAIJI:
					if (((wText >= 0x20) && (wText <= 0x7E)) ||
						((wText >= 0xA1) && (wText <= 0xDF)))
					{
						wChar = wText;
					} else if (wText > 0xFF) {
						wChar = (uint16_t)JIS2SJIS(0x2121);
					}
					break;
				}
				if (wChar != 0) {
					if (wChar > 0xFF) {
						jstrText += (char)(wChar >> 8);
						jstrText += (char)(wChar & 0xFF);
					} else {
						jstrText += (char)wChar;
					}
				}
			}
			bPageTop = false;
			bLineTop = false;
		}
	}
}
