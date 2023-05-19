# macro

APP	=	X88000

CC	=	g++
CFLAGS	=	`pkg-config gtk+-2.0 --cflags` -O2

LNK	=	g++
LFLAGS	=

EXE	=	$(APP)
OBJS	=	DiskImage.o \
		DiskImageCollection.o \
		DiskImageFile.o \
		InitGuid.o \
		ParallelDevice.o \
		ParallelNull.o \
		ParallelPR201.o \
		ParallelPrinter.o \
		PC88.o \
		PC88Calendar.o \
		PC88Crtc.o \
		PC88Fdc.o \
		PC88Opna.o \
		PC88Pcg.o \
		PC88Ppi.o \
		PC88Timer.o \
		PC88Usart.o \
		PC88Z80Main.o \
		PC88Z80Sub.o \
		StdHeader.o \
		TapeImage.o \
		X88000.o \
		X88BeepPlayer.o \
		X88DebugBreakPointDlg.o \
		X88DebugDisAssembleDlg.o \
		X88DebugDumpDlg.o \
		X88DebugWnd.o \
		X88DebugWriteRamDlg.o \
		X88Dialog.o \
		X88DirectX.o \
		X88DiskImageDlg.o \
		X88DiskImageMemory.o \
		X88EnvSetDlg.o \
		X88FileDlg.o \
		X88Frame.o \
		X88Option.o \
		X88ParallelManager.o \
		X88PrinterCopyDlg.o \
		X88PrinterDlg.o \
		X88PrinterDrawer.o \
		X88PrinterPaperDelDlg.o \
		X88PrinterPaperFeedDlg.o \
		X88PrinterPreviewWnd.o \
		X88Resource.o \
		X88ScreenDrawer.o \
		X88TapeImageDlg.o \
		X88Utility.o \
		X88VersionDlg.o \
		X88View.o \
		Z80.o \
		Z80Adapter.o \
		Z80Data.o \
		Z80_2.o
LIBS	=	`pkg-config gtk+-2.0 --libs` -lX11

# dir

# rule

all:	$(EXE)

clean:
	rm -f *.o
	rm -f $(EXE)

$(EXE): $(OBJS)
	$(LNK) $(LFLAGS) $(OBJS) $(LIBS) -o $(EXE)

.cpp.o:
	$(CC) $(CFLAGS) -c $<
