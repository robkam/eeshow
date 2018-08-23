EESchema Schematic File Version 2
LIBS:neo900
LIBS:powered
LIBS:bme280
LIBS:bmx055
LIBS:slg46531
LIBS:tmd2671
LIBS:gencon
LIBS:msl0104rgb
LIBS:nmosfet-gsd
LIBS:74x1g123
LIBS:74x1g74_6d
LIBS:74x2g07_6
LIBS:74x1g32_6
LIBS:fsa2866
LIBS:74x1g04_6
LIBS:101-00271-82
LIBS:lsf204rut
LIBS:txs4555rut
LIBS:ada4505-1acbz
LIBS:opt-mini-rf-tap
LIBS:tlv3691idpf
LIBS:tps61160drv
LIBS:dualbjt
LIBS:wl1837mod
LIBS:phs8-clean
LIBS:tsa5a22366
LIBS:xra1201
LIBS:pmosfet-gsd
LIBS:bb-xm-aux
LIBS:bb-xm-audio
LIBS:dualnmosfet
LIBS:xo-quad-en
LIBS:74x4051
LIBS:fuse
LIBS:two-way-jumper
LIBS:nx3v1g66_6
LIBS:dualpmosfet
LIBS:diode-3
LIBS:sp7538p
LIBS:r-array-8-2com
LIBS:slg46533v
EELAYER 26 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 25 25
Title "BB-xM Adapter (CAM)"
Date "Intentionally Left Blank"
Rev ""
Comp ""
Comment1 "UPPER / Adapter"
Comment2 ""
Comment3 ""
Comment4 "Xsheetnumber=36"
$EndDescr
Text Notes 4550 1300 0    120  ~ 0
Processor Camera Port Interface (P10, 7.20.3)
Text Notes 11000 5350 0    60   ~ 0
CAM_COVER_INT_U (cam_d11, on IO expander)
Text GLabel 11000 4000 2    60   Output ~ 0
CAM_B_EN
Text GLabel 11000 4200 2    60   Input ~ 0
ALS_INT
Text Notes 12200 4050 0    60   ~ 0
(N900 compatible GPIO)
Text Notes 12200 4250 0    60   ~ 0
(N900 compatible GPIO)
Text GLabel 11000 4300 2    60   Input ~ 0
PENIRQ
Text GLabel 11000 4500 2    60   Output ~ 0
CAM_MAIN_SHDN
Text Notes 12200 4550 0    60   ~ 0
(N900 compatible GPIO, cam_d3)
Text GLabel 11000 4700 2    60   Output ~ 0
TSC_RST
Text Notes 12200 4750 0    60   ~ 0
(N900 compatible GPIO)
Text Notes 11000 5850 0    60   ~ 0
HEADPH_EN_U (on IO expander)
Text GLabel 11000 4800 2    60   Input ~ 0
CAM_CLK_P
Text GLabel 11000 4900 2    60   Input ~ 0
CAM_CLK_N
Text GLabel 11000 5000 2    60   BiDi ~ 0
CAM_DATA_P
Text GLabel 11000 5100 2    60   BiDi ~ 0
CAM_DATA_N
Text GLabel 11000 5900 2    60   Input ~ 0
FLASH_INT_U
Text GLabel 11000 6100 2    60   Output ~ 0
VAUDIO_U
Text GLabel 11000 3400 2    60   Output ~ 0
VHB_U
Text GLabel 11000 5400 2    60   Output ~ 0
CAM_XCLK
Text Notes 12200 4650 0    60   Italic 0
N900: unused
Text Notes 12200 5250 0    60   Italic 0
N900: unused
Text Notes 12200 3850 0    60   Italic 0
N900: unused
Text Notes 12200 3950 0    60   Italic 0
N900: unused
Text Notes 11000 5650 0    60   ~ 0
I2C2 also on EXPANSION
Text Notes 11000 5750 0    60   ~ 0
I2C2 also on EXPANSION
$Comp
L BB-XM-AUDIO P2502
U 1 1 57F7381D
P 7400 9300
F 0 "P2502" H 6900 9550 60  0000 L CNN
F 1 "852-10-020-10-001000" H 7400 9000 60  0000 C CNN
F 2 "header:HDR-2x2-50mil" H 7400 9300 60  0001 C CNN
F 3 "" H 7400 9300 60  0001 C CNN
	1    7400 9300
	1    0    0    -1  
$EndComp
Text GLabel 6600 9200 0    60   Output ~ 0
CODEC_DIN_U
Text GLabel 8200 9400 2    60   Output ~ 0
CODEC_BCLK_U
Text GLabel 8200 9200 2    60   Output ~ 0
CODEC_WCLK_U
Text GLabel 6600 9400 0    60   Input ~ 0
CODEC_DOUT_U
Wire Wire Line
	6600 9200 6700 9200
Wire Wire Line
	6700 9400 6600 9400
Wire Wire Line
	8100 9200 8200 9200
Wire Wire Line
	8200 9400 8100 9400
Text Notes 12200 4350 0    60   ~ 0
(N900 compatible GPIO)
Text GLabel 11000 3800 2    60   Output ~ 0
FLASH_STROBE_U
Text Notes 13150 3850 0    60   ~ 0
No FLASH_STROBE on BB-xM, using GPIO
Text Notes 12200 3450 0    60   ~ 0
VAUX3, up to 200 mA, 1.5-3.0 V
Text Notes 12200 6150 0    60   ~ 0
VAUX4, up to 100 mA, 0.7-3.15 V
Text GLabel 11000 5200 2    60   Input ~ 0
UART1_CTS_U
Text GLabel 11000 4600 2    60   Output ~ 0
UART1_RTS_U
Text Notes 13150 4650 0    60   ~ 0
No UART1_RTS on BB-xM, using GPIO
Text Notes 13150 5250 0    60   ~ 0
No UART1_CTS on BB-xM, using GPIO
Text Notes 6300 10100 0    60   ~ 0
This part is a "breakaway" strip (20 positions) and needs to be\ncustomized (cut) before assembly.\nAlternatively, 852-10-100-10-001000 (100 positions) could be used.
$Comp
L CONN_34 CON2501
U 1 1 580FA2ED
P 9300 4550
F 0 "CON2501" H 9300 2750 60  0000 C CNN
F 1 "CONN_34" H 9300 6350 60  0000 C CNN
F 2 "header:HDR-34x1-50mil" H 9300 4550 60  0001 C CNN
F 3 "" H 9300 4550 60  0001 C CNN
	1    9300 4550
	1    0    0    1   
$EndComp
$Comp
L CONN_34 CON2502
U 1 1 580FA355
P 10100 4550
F 0 "CON2502" H 10100 2750 60  0000 C CNN
F 1 "CONN_34" H 10100 6350 60  0000 C CNN
F 2 "header:HDR-34x1-50mil" H 10100 4550 60  0001 C CNN
F 3 "" H 10100 4550 60  0001 C CNN
	1    10100 4550
	-1   0    0    1   
$EndComp
$Comp
L GND #M0425
U 1 1 580FA6DE
P 10600 6400
F 0 "#M0425" H 10600 6400 45  0001 C CNN
F 1 "GND" H 10600 6400 45  0001 C CNN
F 2 "" H 10600 6400 60  0001 C CNN
F 3 "" H 10600 6400 60  0001 C CNN
	1    10600 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10600 2900 10600 6300
Wire Wire Line
	10600 2900 10500 2900
Wire Wire Line
	10500 3000 10600 3000
Connection ~ 10600 3000
NoConn ~ 10500 3100
NoConn ~ 10500 3200
Wire Wire Line
	10500 3300 10600 3300
Connection ~ 10600 3300
Wire Wire Line
	10500 3400 11000 3400
Wire Wire Line
	10500 3500 10800 3500
Wire Wire Line
	10800 3500 10800 3400
Connection ~ 10800 3400
Wire Wire Line
	10500 3600 10600 3600
Connection ~ 10600 3600
Wire Wire Line
	10500 3700 10600 3700
Connection ~ 10600 3700
NoConn ~ 10500 4100
Text Notes 12200 4450 0    60   Italic 0
N900: BTHOSTWAKE
Wire Wire Line
	10500 3800 11000 3800
Wire Wire Line
	10500 4000 11000 4000
Wire Wire Line
	10500 4200 11000 4200
Wire Wire Line
	11000 4300 10500 4300
Wire Wire Line
	10500 4500 11000 4500
NoConn ~ 10500 5300
Wire Wire Line
	10500 5500 10600 5500
Connection ~ 10600 5500
Wire Wire Line
	10500 4600 11000 4600
Wire Wire Line
	11000 4700 10500 4700
Wire Wire Line
	10500 4800 11000 4800
Wire Wire Line
	11000 4900 10500 4900
Wire Wire Line
	10500 5000 11000 5000
Wire Wire Line
	11000 5100 10500 5100
Wire Wire Line
	10500 5200 11000 5200
Wire Wire Line
	11000 5400 10500 5400
NoConn ~ 10500 5800
NoConn ~ 10500 5700
NoConn ~ 10500 5600
Wire Wire Line
	11000 5900 10500 5900
Wire Wire Line
	10500 6000 10600 6000
Connection ~ 10600 6000
Wire Wire Line
	10500 6100 11000 6100
Wire Wire Line
	10500 6200 10800 6200
Wire Wire Line
	10800 6200 10800 6100
Connection ~ 10800 6100
$Comp
L BEAGLEBOARD-XM-CAMERA P2501
U 1 1 578E5E9D
P 6150 4500
F 0 "P2501" H 5550 6250 60  0000 L CNN
F 1 "852-80-034-10-001101" H 5550 2650 65  0000 L TNN
F 2 "header:HDR-17x2-50mil" H 6140 4290 65  0001 L TNN
F 3 "" H 6150 4500 60  0001 C CNN
	1    6150 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 3700 7700 3700
Wire Wire Line
	7700 3700 7700 2900
Wire Wire Line
	7700 2900 6850 2900
Wire Wire Line
	8900 3800 7600 3800
Wire Wire Line
	7600 3800 7600 3100
Wire Wire Line
	7600 3100 6850 3100
Wire Wire Line
	8900 3900 7500 3900
Wire Wire Line
	7500 3900 7500 3300
Wire Wire Line
	7500 3300 6850 3300
Wire Wire Line
	8900 4000 7400 4000
Wire Wire Line
	7400 4000 7400 3500
Wire Wire Line
	7400 3500 6850 3500
Wire Wire Line
	8900 4100 7300 4100
Wire Wire Line
	7300 4100 7300 3700
Wire Wire Line
	7300 3700 6850 3700
Wire Wire Line
	5350 2900 5200 2900
Wire Wire Line
	5200 2900 5200 2500
Wire Wire Line
	5200 2500 7900 2500
Wire Wire Line
	7900 2500 7900 3600
Wire Wire Line
	7900 3600 8900 3600
Wire Wire Line
	5350 3100 5100 3100
Wire Wire Line
	5100 3100 5100 2400
Wire Wire Line
	5100 2400 8000 2400
Wire Wire Line
	8000 2400 8000 3500
Wire Wire Line
	8000 3500 8900 3500
Wire Wire Line
	5350 3300 5000 3300
Wire Wire Line
	5000 3300 5000 2300
Wire Wire Line
	5000 2300 8100 2300
Wire Wire Line
	8100 2300 8100 3400
Wire Wire Line
	8100 3400 8900 3400
Wire Wire Line
	5350 3500 4900 3500
Wire Wire Line
	4900 3500 4900 2200
Wire Wire Line
	4900 2200 8200 2200
Wire Wire Line
	8200 2200 8200 3300
Wire Wire Line
	8200 3300 8900 3300
Wire Wire Line
	5350 3700 4800 3700
Wire Wire Line
	4800 3700 4800 2100
Wire Wire Line
	4800 2100 8300 2100
Wire Wire Line
	8300 2100 8300 3200
Wire Wire Line
	8300 3200 8900 3200
Wire Wire Line
	5350 3900 4700 3900
Wire Wire Line
	4700 3900 4700 2000
Wire Wire Line
	4700 2000 8400 2000
Wire Wire Line
	8400 2000 8400 3100
Wire Wire Line
	8400 3100 8900 3100
Wire Wire Line
	5350 4100 4600 4100
Wire Wire Line
	4600 4100 4600 1900
Wire Wire Line
	4600 1900 8500 1900
Wire Wire Line
	8500 1900 8500 3000
Wire Wire Line
	8500 3000 8900 3000
Wire Wire Line
	5350 4300 4500 4300
Wire Wire Line
	4500 4300 4500 1800
Wire Wire Line
	4500 1800 8600 1800
Wire Wire Line
	8600 1800 8600 2900
Wire Wire Line
	8600 2900 8900 2900
Wire Wire Line
	6850 3900 7200 3900
Wire Wire Line
	7200 3900 7200 4200
Wire Wire Line
	7200 4200 8900 4200
Wire Wire Line
	6850 4100 7100 4100
Wire Wire Line
	7100 4100 7100 4300
Wire Wire Line
	7100 4300 8900 4300
Wire Wire Line
	6850 4300 7000 4300
Wire Wire Line
	7000 4300 7000 4400
Wire Wire Line
	7000 4400 8900 4400
Wire Wire Line
	6850 4500 8900 4500
Wire Wire Line
	7000 4700 6850 4700
Wire Wire Line
	7000 4600 7000 4700
Wire Wire Line
	7000 4600 8900 4600
Wire Wire Line
	6850 4900 7100 4900
Wire Wire Line
	7100 4900 7100 4700
Wire Wire Line
	7100 4700 8900 4700
Wire Wire Line
	6850 5100 7200 5100
Wire Wire Line
	7200 5100 7200 4800
Wire Wire Line
	7200 4800 8900 4800
Wire Wire Line
	6850 5300 7300 5300
Wire Wire Line
	7300 5300 7300 4900
Wire Wire Line
	7300 4900 8900 4900
Wire Wire Line
	6850 5500 7400 5500
Wire Wire Line
	7400 5500 7400 5000
Wire Wire Line
	7400 5000 8900 5000
Wire Wire Line
	6850 5700 7500 5700
Wire Wire Line
	7500 5700 7500 5100
Wire Wire Line
	7500 5100 8900 5100
Wire Wire Line
	6850 5900 7600 5900
Wire Wire Line
	7600 5900 7600 5200
Wire Wire Line
	7600 5200 8900 5200
Wire Wire Line
	6850 6100 7700 6100
Wire Wire Line
	7700 6100 7700 5300
Wire Wire Line
	7700 5300 8900 5300
Wire Wire Line
	5350 6100 5200 6100
Wire Wire Line
	5200 6100 5200 6800
Wire Wire Line
	5200 6800 7900 6800
Wire Wire Line
	7900 6800 7900 5400
Wire Wire Line
	7900 5400 8900 5400
Wire Wire Line
	5350 5900 5100 5900
Wire Wire Line
	5100 5900 5100 6900
Wire Wire Line
	5100 6900 8000 6900
Wire Wire Line
	8000 6900 8000 5500
Wire Wire Line
	8000 5500 8900 5500
Wire Wire Line
	5350 5700 5000 5700
Wire Wire Line
	5000 5700 5000 7000
Wire Wire Line
	5000 7000 8100 7000
Wire Wire Line
	8100 7000 8100 5600
Wire Wire Line
	8100 5600 8900 5600
Wire Wire Line
	5350 5500 4900 5500
Wire Wire Line
	4900 5500 4900 7100
Wire Wire Line
	4900 7100 8200 7100
Wire Wire Line
	8200 7100 8200 5700
Wire Wire Line
	8200 5700 8900 5700
Wire Wire Line
	5350 5300 4800 5300
Wire Wire Line
	4800 5300 4800 7200
Wire Wire Line
	4800 7200 8300 7200
Wire Wire Line
	8300 7200 8300 5800
Wire Wire Line
	8300 5800 8900 5800
Wire Wire Line
	5350 5100 4700 5100
Wire Wire Line
	4700 5100 4700 7300
Wire Wire Line
	4700 7300 8400 7300
Wire Wire Line
	8400 7300 8400 5900
Wire Wire Line
	8400 5900 8900 5900
Wire Wire Line
	5350 4900 4600 4900
Wire Wire Line
	4600 4900 4600 7400
Wire Wire Line
	4600 7400 8500 7400
Wire Wire Line
	8500 7400 8500 6000
Wire Wire Line
	8500 6000 8900 6000
Wire Wire Line
	5350 4700 4500 4700
Wire Wire Line
	4500 4700 4500 7500
Wire Wire Line
	4500 7500 8600 7500
Wire Wire Line
	8600 7500 8600 6100
Wire Wire Line
	8600 6100 8900 6100
Wire Wire Line
	5350 4500 4400 4500
Wire Wire Line
	4400 4500 4400 7600
Wire Wire Line
	4400 7600 8700 7600
Wire Wire Line
	8700 7600 8700 6200
Wire Wire Line
	8700 6200 8900 6200
Wire Notes Line
	4100 1500 9700 1550
Wire Notes Line
	9700 1550 9700 8000
Wire Notes Line
	9700 8000 4100 8000
Wire Notes Line
	4100 8000 4100 1500
Text Notes 4200 7900 0    139  ~ 28
Adapter board
Text Notes 9900 6700 0    60   ~ 0
34 x 1, 1.27 mm TH row (for ribbon cable)
NoConn ~ 10500 4400
Wire Wire Line
	11000 3900 10500 3900
Text GLabel 11000 3900 2    60   Input ~ 0
NFC_INT_U
Text Notes 8700 2900 0    60   ~ 0
20
Text Notes 8700 3600 0    50   ~ 0
34
Text Notes 8700 3700 0    50   ~ 0
33
Text Notes 8700 5300 0    50   ~ 0
1
Text Notes 8700 5400 0    50   ~ 0
2
Text Notes 8700 6200 0    50   ~ 0
18
Text Notes 8700 3000 0    50   ~ 0
22
Text Notes 8700 3100 0    50   ~ 0
24
Text Notes 8700 3200 0    50   ~ 0
26
Text Notes 8700 3300 0    50   ~ 0
28
Text Notes 8700 3400 0    50   ~ 0
30
Text Notes 8700 3500 0    50   ~ 0
32
Text Notes 8700 3800 0    50   ~ 0
31
Text Notes 8700 3900 0    50   ~ 0
29
Text Notes 8700 4000 0    50   ~ 0
27
Text Notes 8700 4100 0    50   ~ 0
25
Text Notes 8700 4200 0    50   ~ 0
23
Text Notes 8700 4300 0    50   ~ 0
21
Text Notes 8700 4400 0    50   ~ 0
19
Text Notes 8700 4500 0    50   ~ 0
17
Text Notes 8700 4600 0    50   ~ 0
15
Text Notes 8700 4700 0    50   ~ 0
13
Text Notes 8700 4800 0    50   ~ 0
11
Text Notes 8700 4900 0    50   ~ 0
9
Text Notes 8700 5000 0    50   ~ 0
7
Text Notes 8700 5100 0    50   ~ 0
5
Text Notes 8700 5200 0    50   ~ 0
3
Text Notes 8700 5500 0    50   ~ 0
4
Text Notes 8700 5600 0    50   ~ 0
6
Text Notes 8700 5700 0    50   ~ 0
8
Text Notes 8700 5800 0    50   ~ 0
10
Text Notes 8700 5900 0    50   ~ 0
12
Text Notes 8700 6000 0    50   ~ 0
14
Text Notes 8700 6100 0    50   ~ 0
16
Text Notes 5300 6650 0    60   ~ 0
Equivalent part as "breakaway" strip (100 positions):\n852-10-100-10-001000
Text Notes 10880 2750 0    139  ~ 0
UPPER
$EndSCHEMATC
