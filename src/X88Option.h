////////////////////////////////////////////////////////////
// X88000 CommandLine/Profile Option
//
// Written by Manuke

#ifndef X88Option_DEFINED
#define X88Option_DEFINED

////////////////////////////////////////////////////////////
// declare

class CX88Option;
class CX88OptionManager;
class CX88EnvFile;

////////////////////////////////////////////////////////////
// include

#include "X88000.h"

////////////////////////////////////////////////////////////
// declaration of CX88Option

class CX88Option {
// attribute
protected:
	// exist parameter
	bool m_bExistParam;
	// supporting command line
	bool m_bSupportCommandLine;
	// supporting environment settings file
	bool m_bSupportEnvFile;

	// option name
	std::string m_strOptionName;
	// group name
	std::string m_strGroupName;

public:
	// is exist parameter
	bool IsExistParam() {
		return m_bExistParam;
	}
	// is supporting command line
	bool IsSupportCommandLine() const {
		return m_bSupportCommandLine;
	}
	// is supporting environment settings file
	bool IsSupportEnvFile() const {
		return m_bSupportEnvFile;
	}
	// get option name
	std::string GetOptionName() const {
		return m_strOptionName;
	}
	// get group name
	std::string GetGroupName() const {
		return m_strGroupName;
	}

	// option name
	static const char OPT_DEFAULT[];
	static const char OPT_SAVEENV[];
	static const char OPT_UILANG[];
	static const char OPT_BASIC[];
	static const char OPT_CLOCK[];
	static const char OPT_BOOST[];
	static const char OPT_BOOSTLIM[];
	static const char OPT_DRIVE1[];
	static const char OPT_DRIVE2[];
	static const char OPT_DRIVE3[];
	static const char OPT_DRIVE4[];
	static const char OPT_DRIVES[];
	static const char OPT_READONLY[];
	static const char OPT_FRAMERATE[];
	static const char OPT_INTERLACE[];
	static const char OPT_DIRECTINPUT[];
	static const char OPT_IMEDISABLE[];
	static const char OPT_DIRECTSOUND[];
	static const char OPT_DSOUNDFREQ[];
	static const char OPT_DSOUNDBIT[];
	static const char OPT_DSOUNDCH[];
	static const char OPT_BEEPVOLUME[];
	static const char OPT_BEEPMUTE[];
	static const char OPT_PCGVOLUME[];
	static const char OPT_PCGMUTE[];
	static const char OPT_FULLSCREEN[];
	static const char OPT_PARALLEL[];
	static const char OPT_CRTVSYNC[];
	static const char OPT_WAITEMU[];
	static const char OPT_OLDCOMPAT[];
	static const char OPT_PCGENABLE[];
	static const char OPT_OPTIONFONT[];

	// group name
	static const char GROUP_OPTION[];
	static const char GROUP_COMMAND[];

// create & destroy
public:
	// standard constructor
	CX88Option(
			bool bExistParam,
			bool bSupportCommandLine,
			bool bSupportEnvFile,
			const std::string& strOptionName,
			const std::string& strGroupName) :
		m_bExistParam(bExistParam),
		m_bSupportCommandLine(bSupportCommandLine),
		m_bSupportEnvFile(bSupportEnvFile),
		m_strOptionName(strOptionName),
		m_strGroupName(strGroupName)
	{
	}
	// destructor
	virtual ~CX88Option() {
	}

// implementation
public:
	// process
	virtual void Process(
		CX88000& x88k,
		const std::string& strParam, bool bDefault = false) = 0;
	// get current status
	virtual bool GetCurrentStatus(
		CX88000& x88k,
		std::string& strParam) = 0;
};

////////////////////////////////////////////////////////////
// declaration of CX88OptionManager

class CX88OptionManager {
// typedef
protected:
	typedef std::list<CX88Option*> COptionList;

// attribute
protected:
	// options list
	COptionList m_listOptions;

// create & destroy
public:
	// default constructor
	CX88OptionManager();
	// destructor
	~CX88OptionManager();

// operation
protected:
	// find option
	CX88Option* FindOption(const std::string& strOption);
	// is command line separator
	static bool IsSeparator(char ch) {
		return (ch == ':') || (ch == '=');
	}

public:
	// parse command line
	void ParseCommandLine(
		CX88000& x88k,
		int argc, char** argv);
	// load from environment settings file
	void LoadEnvFile(
		CX88000& x88k,
		const std::string& fstrEnvFileName);
	// save to environment settings file
	void SaveEnvFile(
		CX88000& x88k,
		const std::string& fstrEnvFileName,
		std::vector<std::string>* pvectOptionNames = NULL);
	// set default
	void SetDefault(
		CX88000& x88k,
		std::vector<std::string>* pvectOptionNames = NULL);
};

////////////////////////////////////////////////////////////
// declaration of CX88EnvFile

class CX88EnvFile {
// struct
protected:

#ifdef X88_PLATFORM_UNIX

	// declaration and implementation of SEnvLine
	struct SEnvLine {
	// enum
	public:
		// kind
		enum {
			TYPE_UNKNOWN,
			TYPE_BLANK,
			TYPE_REMARK,
			TYPE_SECTION,
			TYPE_ENTRY
		};
	// attribute
	public:
		// kind
		int nType;
		// line buffer(locale encoding)
		std::string lstrLine;
		// id(locale encoding)
		std::string lstrID;
	// create & destroy
	public:
		// default constructor
		SEnvLine() :
			nType(TYPE_UNKNOWN)
		{
		}
		// destructor
		~SEnvLine() {
		}
	// operation
	public:
	};

#endif // X88_PLATFORM_UNIX

// attribute
protected:
	// file name(filesystem encoding)
	std::string m_fstrFileName;

#ifdef X88_PLATFORM_UNIX

	// line
	std::list<SEnvLine> m_listLines;
	// dirty
	bool m_bDirty;

#endif // X88_PLATFORM_UNIX

public:
	// get file name(filesystem encoding)
	std::string GetFileName() const {
		return m_fstrFileName;
	}

// create & destroy
public:
	// default constructor
	CX88EnvFile();
	// standard constructor
	CX88EnvFile(const std::string& fstrFileName);
	// destructor
	virtual ~CX88EnvFile();

// operation
public:
	// open environment file
	virtual bool Open(const std::string& fstrFileName);
	// close environment file
	virtual bool Close();
	// get entry
	virtual bool GetEntry(
		const std::string& strSection,
		const std::string& strEntry,
		std::string& strParam);
	// set entry
	virtual bool SetEntry(
		const std::string& strSection,
		const std::string& strEntry,
		const std::string& strParam);
};

#endif // X88Option_DEFINED
