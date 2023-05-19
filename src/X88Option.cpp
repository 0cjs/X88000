////////////////////////////////////////////////////////////
// X88000 CommandLine/Profile Option
//
// Written by Manuke

////////////////////////////////////////////////////////////
// include

#include "StdHeader.h"

#include "X88Option.h"

#include "X88Resource.h"
#include "X88Utility.h"

using namespace NX88Utility;

////////////////////////////////////////////////////////////
// macro

// macro for add option

#define ADD_OPTION(nam, prm, cmd, env, opt, grp, process, status) { \
	class nam : public CX88Option { \
	public: \
		nam() : CX88Option(prm, cmd, env, opt, grp) {} \
		virtual void Process( \
			CX88000& x88k, \
			const std::string& strParam, bool bDefault = false) \
		{ x88k; strParam; bDefault; process; } \
		virtual bool GetCurrentStatus( \
			CX88000& x88k, \
			std::string& strParam) \
		{ x88k; strParam; status; } \
	} *pOption = new nam; \
	m_listOptions.push_back(pOption); \
}

////////////////////////////////////////////////////////////
// implementation of CX88Option

////////////////////////////////////////////////////////////
// attribute

// option name

const char CX88Option::OPT_DEFAULT[] = "default";
const char CX88Option::OPT_SAVEENV[] = "saveenv";
const char CX88Option::OPT_UILANG[] = "uilang";
const char CX88Option::OPT_BASIC[] = "basic";
const char CX88Option::OPT_CLOCK[] = "clock";
const char CX88Option::OPT_BOOST[] = "boost";
const char CX88Option::OPT_BOOSTLIM[] = "boostlim";
const char CX88Option::OPT_DRIVE1[] = "drive1";
const char CX88Option::OPT_DRIVE2[] = "drive2";
const char CX88Option::OPT_DRIVE3[] = "drive3";
const char CX88Option::OPT_DRIVE4[] = "drive4";
const char CX88Option::OPT_DRIVES[] = "drives";
const char CX88Option::OPT_READONLY[] = "readonly";
const char CX88Option::OPT_FRAMERATE[] = "framerate";
const char CX88Option::OPT_INTERLACE[] = "interlace";
const char CX88Option::OPT_DIRECTINPUT[] = "directinput";
const char CX88Option::OPT_IMEDISABLE[] = "imedisable";
const char CX88Option::OPT_DIRECTSOUND[] = "directsound";
const char CX88Option::OPT_DSOUNDFREQ[] = "dsoundfreq";
const char CX88Option::OPT_DSOUNDBIT[] = "dsoundbit";
const char CX88Option::OPT_DSOUNDCH[] = "dsoundch";
const char CX88Option::OPT_BEEPVOLUME[] = "beepvolume";
const char CX88Option::OPT_BEEPMUTE[] = "beepmute";
const char CX88Option::OPT_PCGVOLUME[] = "pcgvolume";
const char CX88Option::OPT_PCGMUTE[] = "pcgmute";
const char CX88Option::OPT_FULLSCREEN[] = "fullscreen";
const char CX88Option::OPT_PARALLEL[] = "parallel";
const char CX88Option::OPT_CRTVSYNC[] = "crtvsync";
const char CX88Option::OPT_WAITEMU[] = "waitemu";
const char CX88Option::OPT_OLDCOMPAT[] = "oldcompat";
const char CX88Option::OPT_PCGENABLE[] = "pcg";
const char CX88Option::OPT_OPTIONFONT[] = "optfont";

// group name

const char CX88Option::GROUP_OPTION[] = "option";
const char CX88Option::GROUP_COMMAND[] = "command";

////////////////////////////////////////////////////////////
// implementation of CX88OptionManager

////////////////////////////////////////////////////////////
// create & destroy

// default constructor

CX88OptionManager::CX88OptionManager() {
	// set all ontion to default(no parameter, command line only)
	ADD_OPTION(
		CX88DefaultOption,
		false, true, false,
		CX88Option::OPT_DEFAULT, CX88Option::GROUP_COMMAND,
		{ // Process
			if (!bDefault) {
				x88k.DefaultOptions();
			}
		}, { // GetCurrentStatus
			return false;
		});
	// saving environment settings(command line only)
	ADD_OPTION(
		CX88SaveEnvOption,
		true, true, false,
		CX88Option::OPT_SAVEENV, CX88Option::GROUP_COMMAND,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetSaveEnv(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetSaveEnv(false);
				}
			} else {
				x88k.SetSaveEnv(false);
			}
		}, { // GetCurrentStatus
			if (x88k.IsBoostMode()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
	// UI language
	ADD_OPTION(
		CX88UILangOption,
		true, true, true,
		CX88Option::OPT_UILANG, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if ((StrCaseCmp(strParam, "ja") == 0) ||
					(StrCaseCmp(strParam, "japanese") == 0))
				{
					NX88Resource::SetUILanguage(
						NX88Resource::X88LANG_JAPANESE);
				} else if (
					(StrCaseCmp(strParam, "en") == 0) ||
					(StrCaseCmp(strParam, "english") == 0))
				{
					NX88Resource::SetUILanguage(
						NX88Resource::X88LANG_ENGLISH);
				} else {
					NX88Resource::SetUILanguage(
						NX88Resource::X88LANG_AUTO);
				}
			} else {
				NX88Resource::SetUILanguage(
					NX88Resource::X88LANG_AUTO);
			}
		}, { // GetCurrentStatus
			switch (NX88Resource::GetUILanguage()) {
			case NX88Resource::X88LANG_JAPANESE:
				strParam = "ja";
				break;
			case NX88Resource::X88LANG_ENGLISH:
				strParam = "en";
				break;
			default:
				strParam = "auto";
				break;
			}
			return true;
		});
	// BASIC mode
	ADD_OPTION(
		CX88BasicOption,
		true, true, true,
		CX88Option::OPT_BASIC, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "n") == 0) {
					x88k.SetBasicMode(CPC88Z80Main::BASICMODE_N, false);
				} else if (StrCaseCmp(strParam, "v1s") == 0) {
					x88k.SetBasicMode(CPC88Z80Main::BASICMODE_N88V1, false);
				} else if (StrCaseCmp(strParam, "v1h") == 0) {
					x88k.SetBasicMode(CPC88Z80Main::BASICMODE_N88V1, true);
				} else if (StrCaseCmp(strParam, "v2") == 0) {
					x88k.SetBasicMode(CPC88Z80Main::BASICMODE_N88V2, true);
				} else if (StrCaseCmp(strParam, "n80v1") == 0) {
					x88k.SetBasicMode(CPC88Z80Main::BASICMODE_N80V1, false);
				} else if (StrCaseCmp(strParam, "n80v2") == 0) {
					x88k.SetBasicMode(CPC88Z80Main::BASICMODE_N80V2, true);
				}
			} else {
				x88k.SetBasicMode(CPC88Z80Main::BASICMODE_N88V2, true);
			}
		}, { // GetCurrentStatus
			switch (x88k.PC88().GetBasicMode()) {
			case CPC88Z80Main::BASICMODE_N:
				strParam = "n";
				break;
			case CPC88Z80Main::BASICMODE_N80V1:
				strParam = "n80v1";
				break;
			case CPC88Z80Main::BASICMODE_N80V2:
				strParam = "n80v2";
				break;
			case CPC88Z80Main::BASICMODE_N88V1:
				if (x88k.PC88().IsHighSpeedMode()) {
					strParam = "v1h";
				} else {
					strParam = "v1s";
				}
				break;
			default:
				strParam = "v2";
				break;
			}
			return true;
		});
	// clock
	ADD_OPTION(
		CX88ClockOption,
		true, true, true,
		CX88Option::OPT_CLOCK, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "4") == 0) {
					x88k.SetBaseClock(4);
				} else if (StrCaseCmp(strParam, "8") == 0) {
					x88k.SetBaseClock(8);
				}
			} else {
				x88k.SetBaseClock(4);
			}
		}, { // GetCurrentStatus
			switch (x88k.PC88().GetBaseClock()) {
			case 8:
				strParam = "8";
				break;
			default:
				strParam = "4";
				break;
			}
			return true;
		});
	// boost mode
	ADD_OPTION(
		CX88BoostOption,
		true, true, true,
		CX88Option::OPT_BOOST, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetBoostMode(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetBoostMode(false);
				}
			} else {
				x88k.SetBoostMode(false);
			}
		}, { // GetCurrentStatus
			if (x88k.IsBoostMode()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
	// boost limiter
	ADD_OPTION(
		CX88BoostLimOption,
		true, true, true,
		CX88Option::OPT_BOOSTLIM, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				x88k.SetBoostLimiter(AtoI(strParam));
			} else {
				x88k.SetBoostLimiter(0);
			}
		}, { // GetCurrentStatus
			if (x88k.GetBoostLimiter() <= 0) {
				strParam = "v-max";
			} else {
				strParam = FormatStr(
					"%d", x88k.GetBoostLimiter());
			}
			return true;
		});
	// selected image of floppy drive1(command line only)
	ADD_OPTION(
		CX88Drive1Option,
		true, true, false,
		CX88Option::OPT_DRIVE1, CX88Option::GROUP_COMMAND,
		{ // Process
			if (!bDefault) {
				x88k.SetDriveSelect(1-1, AtoI(strParam));
			} else {
				x88k.SetDriveSelect(1-1, 1);
			}
		}, { // GetCurrentStatus
			return false;
		});
	// selected image of floppy drive2(command line only)
	ADD_OPTION(
		CX88Drive2Option,
		true, true, false,
		CX88Option::OPT_DRIVE2, CX88Option::GROUP_COMMAND,
		{ // Process
			if (!bDefault) {
				x88k.SetDriveSelect(2-1, AtoI(strParam));
			} else {
				x88k.SetDriveSelect(1-1, 2);
			}
		}, { // GetCurrentStatus
			return false;
		});
	// selected image of floppy drive3(command line only)
	ADD_OPTION(
		CX88Drive3Option,
		true, true, false,
		CX88Option::OPT_DRIVE3, CX88Option::GROUP_COMMAND,
		{ // Process
			if (!bDefault) {
				x88k.SetDriveSelect(3-1, AtoI(strParam));
			} else {
				x88k.SetDriveSelect(1-1, 3);
			}
		}, { // GetCurrentStatus
			return false;
		});
	// selected image of floppy drive4(command line only)
	ADD_OPTION(
		CX88Drive4Option,
		true, true, false,
		CX88Option::OPT_DRIVE4, CX88Option::GROUP_COMMAND,
		{ // Process
			if (!bDefault) {
				x88k.SetDriveSelect(4-1, AtoI(strParam));
			} else {
				x88k.SetDriveSelect(1-1, 4);
			}
		}, { // GetCurrentStatus
			return false;
		});
	// floppy drive count
	ADD_OPTION(
		CX88DrivesOption,
		true, true, true,
		CX88Option::OPT_DRIVES, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				x88k.SetDriveCount(AtoI(strParam));
			} else {
				x88k.SetDriveCount(2);
			}
		}, { // GetCurrentStatus
			strParam = FormatStr(
				"%d", x88k.PC88().Fdc().GetDriveCount());
			return true;
		});
	// floppy read-only attribute(command line only)
	ADD_OPTION(
		CX88ReadOnlyOption,
		true, true, false,
		CX88Option::OPT_READONLY, CX88Option::GROUP_COMMAND,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetReadOnlyMode(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetReadOnlyMode(false);
				}
			} else {
				x88k.SetReadOnlyMode(false);
			}
		}, { // GetCurrentStatus
			return false;
		});
	// screen frame-rate
	ADD_OPTION(
		CX88FrameRateOption,
		true, true, true,
		CX88Option::OPT_FRAMERATE, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				x88k.SetScreenFrameRate(AtoI(strParam));
			} else {
				x88k.SetScreenFrameRate(20);
			}
		}, { // GetCurrentStatus
			strParam = FormatStr(
				"%d", x88k.GetScreenFrameRate());
			return true;
		});
	// interlace mode
	ADD_OPTION(
		CX88InterlaceOption,
		true, true, true,
		CX88Option::OPT_INTERLACE, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetInterlace(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetInterlace(false);
				}
			} else {
				x88k.SetInterlace(false);
			}
		}, { // GetCurrentStatus
			if (x88k.IsInterlace()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
	// using DirectInput
	ADD_OPTION(
		CX88DirectInputOption,
		true, true, true,
		CX88Option::OPT_DIRECTINPUT, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetDirectInputUse(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetDirectInputUse(false);
				}
			} else {
				x88k.SetDirectInputUse(true);
			}
		}, { // GetCurrentStatus
			if (x88k.IsDirectInputUse()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
	// IME disable
	ADD_OPTION(
		CX88IMEDisableOption,
		true, true, true,
		CX88Option::OPT_IMEDISABLE, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetIMEDisable(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetIMEDisable(false);
				}
			} else {
				x88k.SetIMEDisable(false);
			}
		}, { // GetCurrentStatus
			if (x88k.IsIMEDisable()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
	// using DirectSound
	ADD_OPTION(
		CX88DirectSoundOption,
		true, true, true,
		CX88Option::OPT_DIRECTSOUND, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetDirectSoundUse(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetDirectSoundUse(false);
				}
			} else {
				x88k.SetDirectSoundUse(true);
			}
		}, { // GetCurrentStatus
			if (x88k.IsDirectSoundUse()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
	// DirectSound frequency
	ADD_OPTION(
		CX88DSoundFreqOption,
		true, true, true,
		CX88Option::OPT_DSOUNDFREQ, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				x88k.SetDirectSoundFrequency(AtoI(strParam));
			} else {
				x88k.SetDirectSoundFrequency(44100);
			}
		}, { // GetCurrentStatus
			strParam = FormatStr(
				"%d", x88k.GetDirectSoundFrequency());
			return true;
		});
	// DirectSound bit count
	ADD_OPTION(
		CX88DSoundBitOption,
		true, true, true,
		CX88Option::OPT_DSOUNDBIT, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				x88k.SetDirectSoundBits(AtoI(strParam));
			} else {
				x88k.SetDirectSoundBits(8);
			}
		}, { // GetCurrentStatus
			strParam = FormatStr(
				"%d", x88k.GetDirectSoundBits());
			return true;
		});
	// DirectSound channel count
	ADD_OPTION(
		CX88DSoundChOption,
		true, true, true,
		CX88Option::OPT_DSOUNDCH, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				x88k.SetDirectSoundChannels(AtoI(strParam));
			} else {
				x88k.SetDirectSoundChannels(1);
			}
		}, { // GetCurrentStatus
			strParam = FormatStr(
				"%d", x88k.GetDirectSoundChannels());
			return true;
		});
	// beep player volume
	ADD_OPTION(
		CX88BeepVolumeOption,
		true, true, true,
		CX88Option::OPT_BEEPVOLUME, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (!strParam.empty()) {
					x88k.SetBeepPlayerVolume(AtoI(strParam));
				}
			} else {
				x88k.SetBeepPlayerVolume(50);
			}
		}, { // GetCurrentStatus
			strParam = FormatStr(
				"%d", x88k.GetBeepPlayerVolume());
			return true;
		});
	// beep player mute
	ADD_OPTION(
		CX88BeepMuteOption,
		true, true, true,
		CX88Option::OPT_BEEPMUTE, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetBeepPlayerMute(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetBeepPlayerMute(false);
				}
			} else {
				x88k.SetBeepPlayerMute(false);
			}
		}, { // GetCurrentStatus
			if (x88k.IsBeepPlayerMute()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
	// PCG sound player volume
	ADD_OPTION(
		CX88PcgVolumeOption,
		true, true, true,
		CX88Option::OPT_PCGVOLUME, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (!strParam.empty()) {
					x88k.SetPcgPlayerVolume(AtoI(strParam));
				}
			} else {
				x88k.SetPcgPlayerVolume(50);
			}
		}, { // GetCurrentStatus
			strParam = FormatStr(
				"%d", x88k.GetPcgPlayerVolume());
			return true;
		});
	// PCG sound player mute
	ADD_OPTION(
		CX88PcgMuteOption,
		true, true, true,
		CX88Option::OPT_PCGMUTE, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetPcgPlayerMute(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetPcgPlayerMute(false);
				}
			} else {
				x88k.SetPcgPlayerMute(false);
			}
		}, { // GetCurrentStatus
			if (x88k.IsPcgPlayerMute()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
	// full-screen boot
	ADD_OPTION(
		CX88FullScreenOption,
		true, true, true,
		CX88Option::OPT_FULLSCREEN, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetFullScreenBoot(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetFullScreenBoot(false);
				}
			} else {
				x88k.SetFullScreenBoot(false);
			}
		}, { // GetCurrentStatus
			if (x88k.IsFullScreenBoot()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
	// parallel device
	ADD_OPTION(
		CX88ParallelOption,
		true, true, true,
		CX88Option::OPT_PARALLEL, CX88Option::GROUP_OPTION,
		{ // Process
			std::string strDeviceID = !bDefault?
				strParam:
				std::string("null");
			int nDevNo = x88k.ParallelManager().FindDevice(strDeviceID);
			if (nDevNo >= 0) {
				x88k.SelectParallelDevice(nDevNo);
			}
		}, { // GetCurrentStatus
			strParam = x88k.ParallelManager()[
				x88k.GetConnectedParallelDeviceNo()].Device()->
					GetDeviceID();
			return true;
		});
	// CRT v-sync
	ADD_OPTION(
		CX88CRTVSyncOption,
		true, true, true,
		CX88Option::OPT_CRTVSYNC, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "24") == 0) {
					x88k.SetHiresolution(true);
				} else if (StrCaseCmp(strParam, "15") == 0) {
					x88k.SetHiresolution(false);
				}
			} else {
				x88k.SetHiresolution(true);
			}
		}, { // GetCurrentStatus
			if (x88k.IsHiresolution()) {
				strParam = "24";
			} else {
				strParam = "15";
			}
			return true;
		});
	// wait emulation mode
	ADD_OPTION(
		CX88WaitEmulationOption,
		true, true, true,
		CX88Option::OPT_WAITEMU, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetWaitEmulation(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetWaitEmulation(false);
				}
			} else {
				x88k.SetWaitEmulation(false);
			}
		}, { // GetCurrentStatus
			if (x88k.IsWaitEmulation()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
	// old-machine compatible mode
	ADD_OPTION(
		CX88OldCompatibleOption,
		true, true, true,
		CX88Option::OPT_OLDCOMPAT, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetOldCompatible(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetOldCompatible(false);
				}
			} else {
				x88k.SetOldCompatible(false);
			}
		}, { // GetCurrentStatus
			if (x88k.IsOldCompatible()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
	// PCG enable
	ADD_OPTION(
		CX88PcgEnableOption,
		true, true, true,
		CX88Option::OPT_PCGENABLE, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetPcgEnable(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetPcgEnable(false);
				}
			} else {
				x88k.SetPcgEnable(false);
			}
		}, { // GetCurrentStatus
			if (x88k.IsPcgEnable()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
	// international display mode
	ADD_OPTION(
		CX88OptionFontOption,
		true, true, true,
		CX88Option::OPT_OPTIONFONT, CX88Option::GROUP_OPTION,
		{ // Process
			if (!bDefault) {
				if (StrCaseCmp(strParam, "on") == 0) {
					x88k.SetOptionFont(true);
				} else if (StrCaseCmp(strParam, "off") == 0) {
					x88k.SetOptionFont(false);
				}
			} else {
				x88k.SetOptionFont(false);
			}
		}, { // GetCurrentStatus
			if (x88k.IsOptionFont()) {
				strParam = "on";
			} else {
				strParam = "off";
			}
			return true;
		});
}

// destructor

CX88OptionManager::~CX88OptionManager() {
	while (m_listOptions.size() > 0) {
		delete m_listOptions.back();
		m_listOptions.pop_back();
	}
}

////////////////////////////////////////////////////////////
// operation

// find option
//
//     strOption
//         option for find
//     return value
//         option object(NULL : not matched)

CX88Option* CX88OptionManager::FindOption(
	const std::string& strOption)
{
	COptionList::iterator itOption;
	for (
		itOption = m_listOptions.begin();
		itOption != m_listOptions.end();
		itOption++)
	{
		if (StrCaseCmp((*itOption)->GetOptionName(), strOption) == 0) {
			break;
		}
	}
	return (itOption != m_listOptions.end())? *itOption: NULL;
}

// parse command line
// (# option must formatted "-opt:param")
//
//     x88k
//         X88000 object
//     argc, argv
//         command line parameter

void CX88OptionManager::ParseCommandLine(
	CX88000& x88k,
	int argc, char** argv)
{
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			const char* pfszArg = &argv[i][1];
			while (isspace(*pfszArg)) {
				pfszArg++;
			}
			const char* pfszOption = pfszArg;
			while (
				(*pfszArg != '\0') &&
				!IsSeparator(*pfszArg) &&
				!isspace(*pfszArg))
			{
				pfszArg++;
			}
			std::string strOption = ConvFStoUTF8(
				std::string(pfszOption, pfszArg));
			CX88Option* pOption = FindOption(strOption);
			if ((pOption == NULL) || !pOption->IsSupportCommandLine()) {
				continue;
			}
			const char* pfszParam = NULL;
			int nParamLength = 0;
			if (pOption->IsExistParam()) {
				while (isspace(*pfszArg)) {
					pfszArg++;
				}
				if ((*pfszArg == '\0') &&
					(i+1 < argc) && IsSeparator(argv[i+1][0]))
				{
					i++;
					pfszArg = &argv[i][0];
				}
				if (IsSeparator(*pfszArg)) {
					pfszArg++;
					while (isspace(*pfszArg)) {
						pfszArg++;
					}
					if ((*pfszArg == '\0') && (i+1 < argc)) {
						i++;
						pfszArg = &argv[i][0];
						while (isspace(*pfszArg)) {
							pfszArg++;
						}
					}
					if (*pfszArg != '\0') {
						pfszParam = pfszArg;
						nParamLength = (int)strlen(pfszParam);
						while (
							(nParamLength > 0) &&
							isspace(pfszParam[nParamLength-1]))
						{
							nParamLength--;
						}
					}
				}
			}
			std::string strParam;
			if (pfszParam != NULL) {
				strParam = ConvFStoUTF8(
					std::string(pfszParam, pfszParam+nParamLength));
			}
			pOption->Process(x88k, strParam);
		} else {
			char* pfszBegin = &argv[i][0];
			char* pfszEnd = pfszBegin+strlen(pfszBegin);
			while (isspace(*pfszBegin)) {
				pfszBegin++;
			}
			while ((pfszEnd > pfszBegin) && isspace(*pfszEnd)) {
				pfszEnd--;
			}
			if (pfszEnd-pfszBegin >= 2) {
				if ((*pfszBegin == '"') && (*(pfszEnd-1) == '"')) {
					pfszBegin++;
					pfszEnd--;
				} else if ((*pfszBegin == '\'') && (*(pfszEnd-1) == '\'')) {
					pfszBegin++;
					pfszEnd--;
				}
			}
			if (pfszEnd > pfszBegin) {
				std::string fstrFileName = std::string(pfszBegin, pfszEnd);
				x88k.AddImageFile(fstrFileName);
			}
		}
	}
}

// load from environment settings file
//
//     pX88k
//         X88000 object
//     fstrEnvFileName
//         environment settings file name(filesystem encoding)

void CX88OptionManager::LoadEnvFile(
	CX88000& x88k,
	const std::string& fstrEnvFileName)
{
	CX88EnvFile envfile(fstrEnvFileName);
	for (
		COptionList::iterator itOption = m_listOptions.begin();
		itOption != m_listOptions.end();
		itOption++)
	{
		if ((*itOption)->IsSupportEnvFile()) {
			std::string strParam;
			envfile.GetEntry(
				(*itOption)->GetGroupName(),
				(*itOption)->GetOptionName(),
				strParam);
			(*itOption)->Process(x88k, strParam);
		}
	}
}

// save to environment settings file
//
//     x88k
//         X88000 object
//     fstrEnvFileName
//         environment settings file name(filesystem encoding)
//     pvectOptionNames = NULL
//         option vector for saving(NULL : all options)

void CX88OptionManager::SaveEnvFile(
	CX88000& x88k,
	const std::string& fstrEnvFileName,
	std::vector<std::string>* pvectOptionNames)
{
	CX88EnvFile envfile(fstrEnvFileName);
	for (
		COptionList::iterator itOption = m_listOptions.begin();
		itOption != m_listOptions.end();
		itOption++)
	{
		bool bSave = false;
		if (pvectOptionNames == NULL) {
			bSave = true;
		} else {
			for (
				std::vector<std::string>::iterator itOptionName =
					pvectOptionNames->begin();
				itOptionName != pvectOptionNames->end();
				itOptionName++)
			{
				if (StrCaseCmp(
						(*itOption)->GetOptionName(), *itOptionName) == 0)
				{
					bSave = true;
					break;
				}
			}
		}
		if (bSave && (*itOption)->IsSupportEnvFile()) {
			std::string strParam;
			if ((*itOption)->GetCurrentStatus(x88k, strParam)) {
				envfile.SetEntry(
					(*itOption)->GetGroupName(),
					(*itOption)->GetOptionName(),
					strParam);
			}
		}
	}
}

// set default
//
//     x88k
//         X88000 object
//     pvectOptionNames = NULL
//         option vector for default setting(NULL : all options)

void CX88OptionManager::SetDefault(
	CX88000& x88k,
	std::vector<std::string>* pvectOptionNames)
{
	for (
		COptionList::iterator itOption = m_listOptions.begin();
		itOption != m_listOptions.end();
		itOption++)
	{
		bool bSet = false;
		if (pvectOptionNames == NULL) {
			bSet = true;
		} else {
			for (
				std::vector<std::string>::iterator itOptionName =
					pvectOptionNames->begin();
				itOptionName != pvectOptionNames->end();
				itOptionName++)
			{
				if (StrCaseCmp(
						(*itOption)->GetOptionName(), *itOptionName) == 0)
				{
					bSet = true;
					break;
				}
			}
		}
		if (bSet) {
			(*itOption)->Process(x88k, NULL);
		}
	}
}

////////////////////////////////////////////////////////////
// implementation of CX88EnvFile

////////////////////////////////////////////////////////////
// create & destroy

// default constructor

CX88EnvFile::CX88EnvFile() {

#ifdef X88_PLATFORM_UNIX

	m_bDirty = false;

#endif // X88_PLATFORM_UNIX

}

// standard constructor

CX88EnvFile::CX88EnvFile(const std::string& fstrFileName) {

#ifdef X88_PLATFORM_UNIX

	m_bDirty = false;

#endif // X88_PLATFORM_UNIX

	Open(fstrFileName);
}

// destructor

CX88EnvFile::~CX88EnvFile() {
	if (!m_fstrFileName.empty()) {
		Close();
	}
}

////////////////////////////////////////////////////////////
// operation

// open environment file

bool CX88EnvFile::Open(const std::string& fstrFileName) {
	bool bResult = true;
	if (!m_fstrFileName.empty()) {
		Close();
	}
	m_fstrFileName = fstrFileName;

#ifdef X88_PLATFORM_UNIX

	m_bDirty = false;
	FILE* pFile = fopen(m_fstrFileName.c_str(), "r");
	if (pFile != NULL) {
		char lszBuffer[4096];
		while (fgets(lszBuffer, sizeof(lszBuffer), pFile) != NULL) {
			m_listLines.push_back(SEnvLine());
			SEnvLine& line = m_listLines.back();
			line.lstrLine = lszBuffer;
			char* plszTmp = lszBuffer+strspn(lszBuffer, " \t");
			switch (*plszTmp) {
			case '\0':
			case '\n':
				line.nType = SEnvLine::TYPE_BLANK;
				break;
			case ';':
				line.nType = SEnvLine::TYPE_REMARK;
				break;
			case '[':
				{ // dummy block
					plszTmp++;
					plszTmp += strspn(plszTmp, " \t");
					char* plszBegin = plszTmp;
					plszTmp += strcspn(plszTmp, " \t\n]");
					char* plszEnd = plszTmp;
					plszTmp += strspn(plszTmp, " \t");
					if ((*plszTmp == ']') && (plszBegin != plszEnd)) {
						*plszEnd = '\0';
						line.nType = SEnvLine::TYPE_SECTION;
						line.lstrID = plszBegin;
					}
				}
				break;
			default:
				{ // dummy block
					char* plszBegin = plszTmp;
					plszTmp += strcspn(plszTmp, " \t\n=");
					char* plszEnd = plszTmp;
					plszTmp += strspn(plszTmp, " \t");
					if ((*plszTmp == '=') && (plszBegin != plszEnd)) {
						*plszEnd = '\0';
						line.nType = SEnvLine::TYPE_ENTRY;
						line.lstrID = plszBegin;
					}
				}
				break;
			}
		}
	}

#endif // X88_PLATFORM

	return bResult;
}

// close environment file

bool CX88EnvFile::Close() {
	bool bResult = true;
	if (m_fstrFileName.empty()) {
		bResult = false;
	} else {

#ifdef X88_PLATFORM_UNIX

		if (m_bDirty) {
			FILE* pFile = fopen(m_fstrFileName.c_str(), "w");
			if (pFile == NULL) {
				bResult = false;
			} else {
				for (
					std::list<SEnvLine>::iterator itLine = m_listLines.begin();
					itLine != m_listLines.end();
					itLine++)
				{
					if (fputs((*itLine).lstrLine.c_str(), pFile) == EOF) {
						bResult = false;
						break;
					}
				}
				if (fclose(pFile) != 0) {
					bResult = false;
				}
			}
		}
		m_listLines.clear();
		m_bDirty = false;

#endif // X88_PLATFORM

		m_fstrFileName = "";
	}
	return bResult;
}

// get entry

bool CX88EnvFile::GetEntry(
	const std::string& strSection,
	const std::string& strEntry,
	std::string& strParam)
{
	bool bResult = true;
	std::string lstrSection = ConvUTF8toLOC(strSection),
		lstrEntry = ConvUTF8toLOC(strEntry);

#ifdef X88_PLATFORM_WINDOWS

	if (m_fstrFileName.empty()) {
		bResult = false;
	} else {
		char lszParam[256];
		GetPrivateProfileString(
			lstrSection.c_str(),
			lstrEntry.c_str(),
			"",
			lszParam, sizeof(lszParam),
			m_fstrFileName.c_str());
		lszParam[255] = '\0';
		strParam = ConvLOCtoUTF8(lszParam);
	}

#elif defined(X88_PLATFORM_UNIX)

	if (m_fstrFileName.empty()) {
		bResult = false;
	} else {
		bool bFoundSection = false, bFoundEntry = false;
		std::list<SEnvLine>::iterator itLine;
		for (
			itLine = m_listLines.begin();
			itLine != m_listLines.end();
			itLine++)
		{
			if (!bFoundSection) {
				if ((*itLine).nType == SEnvLine::TYPE_SECTION) {
					if (StrCaseCmp((*itLine).lstrID, lstrSection) == 0) {
						bFoundSection = true;
					}
				}
			} else {
				if ((*itLine).nType == SEnvLine::TYPE_SECTION) {
					break;
				} else if ((*itLine).nType == SEnvLine::TYPE_ENTRY) {
					if (StrCaseCmp((*itLine).lstrID, lstrEntry) == 0) {
						bFoundEntry = true;
						break;
					}
				}
			}
		}
		if (bFoundEntry) {
			const char* plszData = (*itLine).lstrLine.c_str();
			plszData += strcspn(plszData, "=");
			if (*plszData == '=') {
				plszData++;
				plszData += strspn(plszData, " \t");
				int nDataLength = strcspn(plszData, "\n");
				while (
					(nDataLength > 0) &&
					((plszData[nDataLength-1] == ' ') ||
						(plszData[nDataLength-1] == '\t')))
				{
					nDataLength--;
				}
				strParam = ConvLOCtoUTF8(
					std::string(plszData, plszData+nDataLength));
			}
		}
	}

#endif // X88_PLATFORM

	return bResult;
}

// set entry

bool CX88EnvFile::SetEntry(
	const std::string& strSection,
	const std::string& strEntry,
	const std::string& strParam)
{
	bool bResult = true;
	std::string lstrSection = ConvUTF8toLOC(strSection),
		lstrEntry = ConvUTF8toLOC(strEntry),
		lstrParam = ConvUTF8toLOC(strParam);

#ifdef X88_PLATFORM_WINDOWS

	if (m_fstrFileName.empty()) {
		bResult = false;
	} else {
		WritePrivateProfileString(
			lstrSection.c_str(),
			lstrEntry.c_str(),
			lstrParam.c_str(),
			m_fstrFileName.c_str());
	}

#elif defined(X88_PLATFORM_UNIX)

	if (m_fstrFileName.empty()) {
		bResult = false;
	} else {
		bool bFoundSection = false, bFoundEntry = false;
		std::list<SEnvLine>::iterator itLine,
			itInsert = m_listLines.end();
		for (
			itLine = m_listLines.begin();
			itLine != m_listLines.end();
			itLine++)
		{
			if (!bFoundSection) {
				if ((*itLine).nType == SEnvLine::TYPE_SECTION) {
					if (StrCaseCmp((*itLine).lstrID, lstrSection) == 0) {
						bFoundSection = true;
						itInsert = itLine;
						itInsert++;
					}
				}
			} else {
				if ((*itLine).nType == SEnvLine::TYPE_SECTION) {
					break;
				} else if ((*itLine).nType == SEnvLine::TYPE_ENTRY) {
					if (StrCaseCmp((*itLine).lstrID, lstrEntry) == 0) {
						bFoundEntry = true;
						break;
					}
					itInsert = itLine;
					itInsert++;
				}
			}
		}
		if (!bFoundSection) {
			itLine = m_listLines.insert(itInsert, SEnvLine());
			(*itLine).nType = SEnvLine::TYPE_SECTION;
			(*itLine).lstrID = lstrSection;
			(*itLine).lstrLine = FormatStr(
				"[%s]\n", lstrSection.c_str());
			itInsert = itLine;
			itInsert++;
		}
		if (!bFoundEntry) {
			itLine = m_listLines.insert(itInsert, SEnvLine());
			(*itLine).nType = SEnvLine::TYPE_ENTRY;
			(*itLine).lstrID = lstrEntry;
		}
		(*itLine).lstrLine = FormatStr(
			"%s=%s\n", lstrEntry.c_str(), lstrParam.c_str());
		m_bDirty = true;
	}

#endif // X88_PLATFORM

	return bResult;
}
