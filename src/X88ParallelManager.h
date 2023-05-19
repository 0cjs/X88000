////////////////////////////////////////////////////////////
// X88000 Parallel Device Manager
//
// Written by Manuke

#ifndef X88ParallelManager_DEFINED
#define X88ParallelManager_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88ParallelUnit;
class CX88ParallelManager;

////////////////////////////////////////////////////////////
// include

#include "ParallelDevice.h"

#include "X88Dialog.h"

////////////////////////////////////////////////////////////
// declaration of CX88ParallelUnit

class CX88ParallelUnit {
// attribute
protected:
	// device
	CParallelDevice* m_pDevice;
	// dialog
	CX88Dialog* m_pDialog;

public:
	// get device
	CParallelDevice* Device() {
		return m_pDevice;
	}
	// get dialog
	CX88Dialog* Dialog() {
		return m_pDialog;
	}

// create & destroy
public:
	// default constructor
	CX88ParallelUnit() :
		m_pDevice(NULL),
		m_pDialog(NULL)
	{
	}
	// standard constructor
	CX88ParallelUnit(
			CParallelDevice* pDevice,
			CX88Dialog* pDialog) :
		m_pDevice(pDevice),
		m_pDialog(pDialog)
	{
	}
};

////////////////////////////////////////////////////////////
// declaration of CX88ParallelManager

class CX88ParallelManager :
	public std::vector<CX88ParallelUnit>
{
//attribute
protected:
	// singleton
	static CX88ParallelManager m_paramanInstance;

public:
	// get singleton instance
	static CX88ParallelManager& GetInstance() {
		return m_paramanInstance;
	}

// create & destroy
public:
	// default constructor
	CX88ParallelManager();
	// destructor
	~CX88ParallelManager();

// initialize
public:
	// initialize at first
	void Initialize();

// operation
public:
	// find device from id
	int FindDevice(const std::string& strID);
};

#endif // X88ParallelManager_DEFINED
