====
ARMv7-Aアーキテクチャー向けマルチタスクOSです。

フルスクラッチでOSを作ろう、というところが出発点です。
(※でもprintf.cだけは拝借しました。)

ターゲット環境：
Firefly RK3288 (Cortex-A17 x 4)
CUBIETRUCK (cortex-A7 x 2)

ビルド環境：
cygwin
linaro ARM-GCC (arm-linux-gnueabihf-gcc v4.9)
IAR workbench + JTAG-ICE(デバッグ用途:CUBIETRUCKのみ)


ビルド方法：
 buildディレクトリで make
 test/xxx ディレクトリでmake

実行方法：
○cubietruckでの実行方法 (ICEを使用します。)
１．cubietruckでu-bootが起動できるようにする。
２．u-boot起動
３．IAR workbenchでICE接続(プログラムのダウンロード)
４．リセット実行
５．PCを0x40000000に設定 SCTLR.Mビットをクリア
６．runを実行

○FireFly RK3288での実行方法 (u-bootから起動 / ICEは使用できません。)
１．u-bootを起動 (コマンドプロンプトにできること / timer7の初期化が行ってあること)
２．loadb 1000 実行
３．kermitにてbwos.binをダウンロード
４．go 1000 実行
