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
Sheet 24 25
Title "BB-xM Adapter (DISP)"
Date "Intentionally Left Blank"
Rev ""
Comp ""
Comment1 "UPPER"
Comment2 ""
Comment3 ""
Comment4 "Xsheetnumber=35"
$EndDescr
$Comp
L BEAGLEBOARD P?
U 2 1 578E3A87
P 9000 3200
AR Path="/578E3A87" Ref="P?"  Part="2" 
AR Path="/578AFF34/578E3A87" Ref="P2301"  Part="2" 
F 0 "P2301" H 8801 4300 60  0000 L BNN
F 1 "852-10-100-10-001000" H 10800 2100 60  0000 R TNN
F 2 "neo900:BBCONN" H 8990 2990 65  0001 L TNN
F 3 "" H 9000 3200 60  0001 C CNN
	2    9000 3200
	1    0    0    -1  
$EndComp
$Comp
L BEAGLEBOARD P?
U 3 1 578E3AEB
P 9000 6700
AR Path="/578E3AEB" Ref="P?"  Part="3" 
AR Path="/578AFF34/578E3AEB" Ref="P2301"  Part="3" 
F 0 "P2301" H 8801 7800 60  0000 L BNN
F 1 "852-10-100-10-001000" H 10800 5600 60  0000 R TNN
F 2 "neo900:BBCONN" H 8990 6490 65  0001 L TNN
F 3 "" H 9000 6700 60  0001 C CNN
	3    9000 6700
	1    0    0    -1  
$EndComp
$Comp
L GND-4 #U$0420
U 1 1 578E4AEF
P 6700 5800
F 0 "#U$0420" H 6581 5700 70  0001 L BNN
F 1 "GND4" H 6581 5700 70  0001 L BNN
F 2 "" H 6700 5800 60  0001 C CNN
F 3 "" H 6700 5800 60  0001 C CNN
	1    6700 5800
	0    1    1    0   
$EndComp
$Comp
L GND-4 #U$0421
U 1 1 578E4B53
P 11300 5800
F 0 "#U$0421" H 11180 5700 70  0001 L BNN
F 1 "GND4" H 11180 5700 70  0001 L BNN
F 2 "" H 11300 5800 60  0001 C CNN
F 3 "" H 11300 5800 60  0001 C CNN
	1    11300 5800
	0    -1   -1   0   
$EndComp
$Comp
L GND-4 #U$0422
U 1 1 578E4BB7
P 11300 2300
F 0 "#U$0422" H 11180 2200 70  0001 L BNN
F 1 "GND4" H 11180 2200 70  0001 L BNN
F 2 "" H 11300 2300 60  0001 C CNN
F 3 "" H 11300 2300 60  0001 C CNN
	1    11300 2300
	0    -1   -1   0   
$EndComp
$Comp
L GND-4 #U$0423
U 1 1 578E4C1B
P 6650 2300
F 0 "#U$0423" H 6531 2200 70  0001 L BNN
F 1 "GND4" H 6531 2200 70  0001 L BNN
F 2 "" H 6650 2300 60  0001 C CNN
F 3 "" H 6650 2300 60  0001 C CNN
	1    6650 2300
	0    1    1    0   
$EndComp
Text Notes 7200 1900 0    120  ~ 0
P11 (7.25)
Text Notes 7200 5400 0    120  ~ 0
P13 (7.25)
Text GLabel 6800 7600 0    60   Output ~ 0
BB_1V8
Text GLabel 14300 2700 2    60   BiDi ~ 0
I2C3_SDA_U
Text GLabel 5400 6400 0    60   BiDi ~ 0
I2C3_SCL_U
Wire Wire Line
	11000 5800 11200 5800
Wire Wire Line
	6800 5800 7000 5800
Wire Wire Line
	7000 6400 5400 6400
Wire Wire Line
	11000 2700 14300 2700
NoConn ~ 7000 2500
NoConn ~ 7000 6600
Wire Wire Line
	6750 2300 7000 2300
Wire Wire Line
	11000 2300 11200 2300
Wire Wire Line
	7000 7600 6800 7600
Text Notes 11350 7250 0    60   ~ 0
WLAN_EN (on IO expander)
Text Notes 6800 2950 2    60   ~ 0
CHG_INT (on IO expander)
Text GLabel 11200 6000 2    60   Input ~ 0
CAM_CAP_2_U
Wire Wire Line
	11000 6000 11200 6000
Text GLabel 11200 2500 2    60   Input ~ 0
CAM_CAP_1_U
Wire Wire Line
	11200 2500 11000 2500
Text Notes 12250 2550 0    60   ~ 0
(N900 compatible GPIO)
Text Notes 12450 6050 0    60   ~ 0
(N900 compatible GPIO)
Text GLabel 6800 2700 0    60   BiDi ~ 0
CDP_D0_P
Text GLabel 6800 3300 0    60   BiDi ~ 0
CDP_D0_M
Text GLabel 6750 6800 0    60   BiDi ~ 0
CDP_D1_P
Text GLabel 11200 3300 2    60   BiDi ~ 0
CDP_D1_M
Text GLabel 6800 3500 0    60   Output ~ 0
CDP_CLK_P
Text GLabel 11200 3500 2    60   Output ~ 0
CDP_CLK_M
Wire Wire Line
	11200 3300 11000 3300
Wire Wire Line
	6800 2700 7000 2700
Wire Wire Line
	7000 3300 6800 3300
Wire Wire Line
	7000 6800 6750 6800
Wire Wire Line
	11200 3500 11000 3500
Wire Wire Line
	6800 3500 7000 3500
Text GLabel 11200 7400 2    60   Output ~ 0
LCD_RST
Text Notes 12450 7450 0    60   ~ 0
(GPIO reassigned in Neo900)
Wire Wire Line
	7000 3700 6800 3700
Text GLabel 11200 3700 2    60   Output ~ 0
LCD_CS
Text GLabel 11200 3900 2    60   Output ~ 0
LCD_MOSI
Text GLabel 6800 3700 0    60   Input ~ 0
LCD_MISO
Text GLabel 6800 3900 0    60   Output ~ 0
LCD_SCLK
Wire Wire Line
	7000 3900 6800 3900
Wire Wire Line
	11200 3900 11000 3900
Wire Wire Line
	11200 3700 11000 3700
Wire Wire Line
	11000 7400 11200 7400
Text GLabel 11200 6800 2    60   Input ~ 0
UART1_RX_U
Text GLabel 11200 6200 2    60   Output ~ 0
UART1_TX_U
Wire Wire Line
	11000 6200 11200 6200
Wire Wire Line
	11200 6800 11000 6800
Text Notes 12450 6250 0    60   ~ 0
Modem
Text Notes 12450 6850 0    60   ~ 0
Modem
Text GLabel 11200 6400 2    60   BiDi ~ 0
UART3_TX_IRTX_U
Text GLabel 11200 6600 2    60   BiDi ~ 0
UART3_RX_IRRX_U
Wire Wire Line
	11200 6400 11000 6400
Wire Wire Line
	11000 6600 11200 6600
Text Notes 12450 6450 0    60   ~ 0
IR/Hackerbus
Text Notes 12450 6650 0    60   ~ 0
IR/Hackerbus
Text GLabel 6750 6200 0    60   Output ~ 0
RGB_CTRL_EN
Text GLabel 6750 7000 0    60   Output ~ 0
MODEM_EN_U
Text GLabel 6800 3100 0    60   Output ~ 0
ECI_OUT_U
Text GLabel 6750 7200 0    60   Output ~ 0
MODEM_EMERG_U
Text GLabel 11200 3100 2    60   Output ~ 0
CPU_SIM_CLK_U
Text GLabel 6750 6000 0    60   BiDi ~ 0
CPU_SIM_IO_U
Wire Wire Line
	7000 3100 6800 3100
Wire Wire Line
	11200 3100 11000 3100
Wire Wire Line
	7000 6000 6750 6000
Wire Wire Line
	7000 6200 6750 6200
Wire Wire Line
	6750 7000 7000 7000
Wire Wire Line
	7000 7200 6750 7200
Text GLabel 6750 7400 0    60   BiDi ~ 0
LCD_CABC
Wire Wire Line
	7000 7400 6750 7400
Text GLabel 11200 7000 2    60   Output ~ 0
LCD_LEDS_PWM
Wire Wire Line
	11200 7000 11000 7000
NoConn ~ 11000 7200
NoConn ~ 7000 2900
NoConn ~ 11000 7600
NoConn ~ 11000 4100
NoConn ~ 7000 4100
Text Notes 6300 7650 2    60   ~ 0
(weak)
Text Notes 9250 4650 0    60   ~ 0
Same part, as "breakaway" strip (100 positions):\n852-10-100-10-001000
Text Notes 9200 8150 0    60   ~ 0
Same part, as "breakaway" strip (100 positions):\n852-10-100-10-001000
Text Notes 7700 9200 0    60   ~ 0
Connect to BB-xM P4.3 (S-Video connector)
$Comp
L CONN_2 CON2401
U 1 1 58210049
P 7450 9250
F 0 "CON2401" H 7450 9400 60  0000 C CNN
F 1 "DNP" H 7450 9100 60  0000 C CNN
F 2 "header:HDR-2x1-100mil" H 7450 9250 60  0001 C CNN
F 3 "" H 7450 9250 60  0001 C CNN
	1    7450 9250
	1    0    0    -1  
$EndComp
$Comp
L GND-4 #U$0424
U 1 1 582100EC
P 6950 9500
F 0 "#U$0424" H 6831 9400 70  0001 L BNN
F 1 "GND4" H 6831 9400 70  0001 L BNN
F 2 "" H 6950 9500 60  0001 C CNN
F 3 "" H 6950 9500 60  0001 C CNN
	1    6950 9500
	1    0    0    -1  
$EndComp
Text GLabel 6750 9200 0    60   Input ~ 0
TVOUT_U
Wire Wire Line
	6950 9400 6950 9300
Wire Wire Line
	6950 9300 7050 9300
Wire Wire Line
	6750 9200 7050 9200
Text Notes 7200 9550 0    60   ~ 0
2x1 100 mil TH
Text Notes 7000 8850 0    120  ~ 0
P4 (7.19)
NoConn ~ 11000 2900
Text Notes 10800 4350 2    60   ~ 0
852-10-020-10-001000
Text Notes 10800 7850 2    60   ~ 0
852-10-020-10-001000
$Comp
L R R?
U 1 1 583DFDD0
P 13900 2400
AR Path="/578AFF0C/583DFDD0" Ref="R?"  Part="1" 
AR Path="/578AFF12/583DFDD0" Ref="R?"  Part="1" 
AR Path="/578AFF32/583DFDD0" Ref="R?"  Part="1" 
AR Path="/578AFF34/583DFDD0" Ref="R2402"  Part="1" 
F 0 "R2402" V 13850 2500 50  0000 L CNN
F 1 "10k" H 13900 2400 50  0000 C CNN
F 2 "NeoFeet:R_0402" H 13930 2550 20  0001 C CNN
F 3 "" H 13900 2400 60  0001 C CNN
F 4 "MF-RES-0402-10k" H 10  10  60  0001 C CNN "MPN"
	1    13900 2400
	0    1    1    0   
$EndComp
Text GLabel 13800 2100 0    60   Input ~ 0
1V8_U
Wire Wire Line
	13800 2100 13900 2100
Wire Wire Line
	13900 2100 13900 2200
Wire Wire Line
	13900 2600 13900 2700
Connection ~ 13900 2700
Text Notes 14000 2500 0    60   ~ 0
DNP
$Comp
L R R?
U 1 1 583E0062
P 5600 6100
AR Path="/578AFF0C/583E0062" Ref="R?"  Part="1" 
AR Path="/578AFF12/583E0062" Ref="R?"  Part="1" 
AR Path="/578AFF32/583E0062" Ref="R?"  Part="1" 
AR Path="/578AFF34/583E0062" Ref="R2401"  Part="1" 
F 0 "R2401" V 5550 6200 50  0000 L CNN
F 1 "10k" H 5600 6100 50  0000 C CNN
F 2 "NeoFeet:R_0402" H 5630 6250 20  0001 C CNN
F 3 "" H 5600 6100 60  0001 C CNN
F 4 "MF-RES-0402-10k" H 10  10  60  0001 C CNN "MPN"
	1    5600 6100
	0    1    1    0   
$EndComp
Text GLabel 5500 5800 0    60   Input ~ 0
1V8_U
Wire Wire Line
	5500 5800 5600 5800
Wire Wire Line
	5600 5800 5600 5900
Wire Wire Line
	5600 6300 5600 6400
Text Notes 5700 6200 0    60   ~ 0
DNP
Connection ~ 5600 6400
$EndSCHEMATC
