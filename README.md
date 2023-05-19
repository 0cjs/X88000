X80000 NEC PC-8001 Emulator
===========================

> The X88000 is a multimedia workstation equipped with the RISC-MPU88000
> made by Moto-La, which was launched by 〇HARP as a way to recover from
> the disaster.
                 --Home Page


This is the X80000 NEC PC-8001 emulator (Linux version) source code. More
information can be found on the [home page][x88home]. The sources were
downloaded from the [source code page][x88src] and extracted on the
`vendor` branch under the `src/` directory (the tar file's `x88_1_5_3_src/`
prefix was removed with `--strip-components=1`).

Information about the source code (in Japanese) can be found in
[`src/X88000Src.txt`](src/X88000Src.txt).


Building and Running
---------------------

The `setup` script will install the packages needed to build this. (This
works only on Debian-based systems at the moment.)

    ./setup
    (cd src/ && make -j8)
    src/X88000


<!-------------------------------------------------------------------->
[x88home]: https://quagma.sakura.ne.jp/manuke/x88000.html
[x88src]: https://quagma.sakura.ne.jp/manuke/x88src.html
