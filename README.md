====
ARMv7-Aアーキテクチャー向けマルチタスクモニタです。

フルスクラッチでOSを作ろう、というところが出発点です。
(※でもprintf.cだけは拝借しました。)

ターゲット環境：
CUBIETRUCK (cortex-A7)

ビルド環境：
cygwin
linaro ARM-GCC (arm-linux-gnueabihf-gcc v4.9)
IAR workbench + JTAG-ICE


実行方法：
０．buildディレクトリで make
１．cubietruckでu-bootが起動できるようにする。
２．u-boot起動
３．IAR workbenchでICE接続(プログラムのダウンロード)
４．リセット実行
５．PCを0x40000000に設定
６．runを実行
