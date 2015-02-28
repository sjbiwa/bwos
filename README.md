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


ビルド方法：
 buildディレクトリで make
 test/xxx ディレクトリでmake
 
実行方法：
○cubietruckでの実行方法 (ICEを使用します。)
１．cubietruckでu-bootが起動できるようにする。
２．u-boot起動
３．IAR workbenchでICE接続(プログラムのダウンロード)
４．リセット実行
５．PCを0x40000000に設定
６．runを実行

○FireFly RK3288での実行方法 (u-bootから起動 / ICEは使用できません。)
１．u-bootを起動 (コマンドプロンプトにできること / timer7の初期化が行ってあること)
２．loadb 0 実行
３．kermitにてbwos.binをダウンロード
４．go 0 実行
