////////////////////////////////////////////////////////////
// X88000 Parallel Device Manager
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88ParallelManager.h"

#include "ParallelNull.h"
#include "ParallelPR201.h"

#include "X88PrinterDlg.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// implementation of CX88ParallelManager

////////////////////////////////////////////////////////////
//attribute

// singleton

CX88ParallelManager CX88ParallelManager::m_paramanInstance;

////////////////////////////////////////////////////////////
// create & destroy

// default constructor

CX88ParallelManager::CX88ParallelManager() {
}

// destructor

CX88ParallelManager::~CX88ParallelManager() {
	for (iterator itUnit = begin(); itUnit != end(); itUnit++) {
		delete (*itUnit).Device();
		if ((*itUnit).Dialog() != NULL) {
			delete (*itUnit).Dialog();
		}
	}
}

////////////////////////////////////////////////////////////
// initialize

// initialize at first

void CX88ParallelManager::Initialize() {
	// null device
	push_back(CX88ParallelUnit(new CParallelNull, NULL));

	// PC-PR201
	CParallelPrinter* pPrinter = new CParallelPR201;
	push_back(CX88ParallelUnit(pPrinter, new CX88PrinterDlg(pPrinter)));

	for (iterator itUnit = begin(); itUnit != end(); itUnit++) {
		(*itUnit).Device()->Initialize();
		(*itUnit).Device()->Reset();
	}
}

////////////////////////////////////////////////////////////
// operation

// find device from id

int CX88ParallelManager::FindDevice(const std::string& strID) {
	for (int i = 0; i < (int)size(); i++) {
		if (StrCaseCmp((*this)[i].Device()->GetDeviceID(), strID) == 0) {
			return i;
		}
	}
	return -1;
}
