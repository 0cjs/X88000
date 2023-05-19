X80000 NEC PC-8001 Emulator
===========================

> The X88000 is a multimedia workstation equipped with the RISC-MPU88000
> made by Moto-La, which was launched by 〇HARP as a way to recover from
> the disaster.
                 --Home Page


This is the X88000 NEC PC-8001 emulator (Linux version) source code. More
information can be found on the [home page][x88home]. The sources were
downloaded from the [source code page][x88src] and extracted on the
`vendor` branch under the `src/` directory (the tar file's `x88_1_5_3_src/`
prefix was removed with `--strip-components=1`).

Information about the source code (in Japanese) can be found in
[`src/X88000Src.txt`](src/X88000Src.txt).


Building
--------

The `setup` script will install the packages needed to build this. (This
works only on Debian-based systems at the moment.)

    ./setup
    (cd src/ && make -j8)
    src/X88000              # emulates all-NOP memory w/o ROMs


Running
-------

Usage of X88000 appears not to be documented; what is here was gleaned from
reading the source code, especially:

    PC88.cpp:CPC88::Initialize: ROM file names and formats

X88000 loads ROM files at startup; if you add new ones you'll need to
restart the emulator. The files may be in the current working directory
or the directory in which the binary lives. (XXX or elsewhere? Look
for config options.) The files it's looking for are:

- `pc88.rom` (112K): A file containing the full set of PC-88 ROMs,
  concatenated. If this does not exist, it will look for the ROMs as
  separate files:
  - `n80.rom` or `8801-n80.rom` (32K, optional, needed for N-BASIC only)
  - `n88_0.rom` or `8801-4th.rom` (8K)
  - `n88_1.rom` (8K)
  - `n88_2.rom` (8K)
  - `n88_3.rom` (8K)
  - `disk.rom` (8K)
- `n80_2.rom` (32K): (does some sort of 24K memcopy if `!bExistNBasic`)
- `e8.rom` (8K)
- `n80_3.rom` (40K)
- `kanji1.rom`: (128K). Alternatively, one of:
  - `pc88.fnt` or `font.rom`: (XXX need to work these alterantives out)
  - `pc-8801.fon`
  - nothing, in which case some copying is done
- `kanji2.rom` (128K) (alternative is a copy)
- `font80sr.rom` or `hirafont.rom`
- `optfont.rom`

Tested configurations include:
- PC-8001, NeoKobe names to X88000 names:
  - `N80_101.ROM` → `n80.rom`
  - `FONT80.ROM` → `font.rom`


<!-------------------------------------------------------------------->
[x88home]: https://quagma.sakura.ne.jp/manuke/x88000.html
[x88src]: https://quagma.sakura.ne.jp/manuke/x88src.html
