====
ARMアーキテクチャー向けマルチタスクOSです。
ARMv7-A / ARMv7-M / ARMv8-A がターゲットです。

フルスクラッチでOSを作ろう、というところが出発点です。
(※でも少し既存のものを流用しています・・・)

ターゲット環境：
Firefly RK3288 (Cortex-A17 x 4)
CUBIETRUCK (cortex-A7 x 2)
LPC4330-Xplorer Board (Cortex-M4F + Cortex-M0)
FVP v8 Foundation Platform (ARMv8-A)
Hikey 96board (Kirin6220 ARMv8-A/Cortex-A53 x 8)

ビルド環境：
cygwin
linaro ARM-GCC (arm-linux-gnueabihf-gcc v4.9 / aarch64-none-elf-gcc 4.9.3)

デバッグ環境： (リリース内容には含まれていません)
CUBIETRUCK
IAR workbench + JTAG-ICE(デバッグ用途:CUBIETRUCKのみ)

LPC4330-Xplorer Board
uVision + JTAG-ICE


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

○FVP v8 Foundation Platform
make run で実行

○Hikey 96board
１．U-BOOTが起動すること (http://people.linaro.org/~peter.griffin/hikey/hikey-u-boot-release_r1/)
２．loadb 10000 実行
３．kermitにてbwos.binをダウンロード
４．go 10000 実行
