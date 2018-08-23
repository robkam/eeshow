EESchema Schematic File Version 2
LIBS:GTA04b7
EELAYER 25 0
EELAYER END
$Descr A3 16535 11700
encoding utf-8
Sheet 24 39
Title "GTA04b7.sch"
Date "17 JUL 2016"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes -2750 -650 0    200   ~ 0
Keypad
Text Notes -9250 350 0    70   ~ 0
(c) 2014 Golden Delicious Computers GmbH&Co. KG. Licensed under CC-BY-SA.
Text Notes -2550 -5750 0    70   ~ 0
Address: 0x34
Text Notes -9150 -6450 0    70   ~ 0
needs final tuning (which button on which position)
Text Notes -9150 -6250 0    70   ~ 0
VOL+ and VOL- could drive FETs sitting in the matrix
Text Notes -8650 150 0    70   ~ 0
replace by 2x RB521ZS8A30 for space constraints
Text Notes 650 150 0    70   ~ 0
V2b
$Comp
L BUTTON-N900 B1/CTL3709
U 1 1 578D2D0C
P -9050 -250
F 0 "B1/CTL3709" V -9220 -110 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -8840 -370 50  0000 L BNN
F 2 "N900-BUTTON" H -9060 -460 65  0001 L TNN
	1    -9050 -250
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B2/CTL2301
U 1 1 578D2DD4
P -8150 -550
F 0 "B2/CTL2301" V -8320 -410 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -7940 -670 50  0000 L BNN
F 2 "N900-BUTTON" H -8160 -760 65  0001 L TNN
	1    -8150 -550
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B3/\\3709
U 1 1 578D2E9C
P -7550 -250
F 0 "B3/\\3709" V -7720 -110 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -7340 -370 50  0000 L BNN
F 2 "N900-BUTTON" H -7560 -460 65  0001 L TNN
	1    -7550 -250
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B4/A3709
U 1 1 578D2F64
P -6950 -250
F 0 "B4/A3709" V -7120 -110 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -6740 -370 50  0000 L BNN
F 2 "N900-BUTTON" H -6960 -460 65  0001 L TNN
	1    -6950 -250
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B5/Q3709
U 1 1 578D302C
P -6350 -250
F 0 "B5/Q3709" V -6520 -110 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -6140 -370 50  0000 L BNN
F 2 "N900-BUTTON" H -6360 -460 65  0001 L TNN
	1    -6350 -250
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B6/2301
U 1 1 578D30F4
P -5750 -250
F 0 "B6/2301" V -5920 -110 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -5540 -370 50  0000 L BNN
F 2 "N900-BUTTON" H -5760 -460 65  0001 L TNN
	1    -5750 -250
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B7/3709
U 1 1 578D31BC
P -5150 -250
F 0 "B7/3709" V -5320 -110 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -4940 -370 50  0000 L BNN
F 2 "N900-BUTTON" H -5160 -460 65  0001 L TNN
	1    -5150 -250
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B8/3709
U 1 1 578D3284
P -4550 -250
F 0 "B8/3709" V -4720 -110 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -4340 -370 50  0000 L BNN
F 2 "N900-BUTTON" H -4560 -460 65  0001 L TNN
	1    -4550 -250
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B9/SPC2301
U 1 1 578D334C
P -3950 -250
F 0 "B9/SPC2301" V -4120 -110 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -3740 -370 50  0000 L BNN
F 2 "N900-BUTTON" H -3960 -460 65  0001 L TNN
	1    -3950 -250
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B10/OK3709
U 1 1 578D3414
P -3350 -250
F 0 "B10/OK3709" V -3520 -110 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -3140 -370 50  0000 L BNN
F 2 "N900-BUTTON" H -3360 -460 65  0001 L TNN
	1    -3350 -250
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B11/ALT3709
U 1 1 578D34DC
P -9050 -850
F 0 "B11/ALT3709" V -9220 -710 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -8840 -970 50  0000 L BNN
F 2 "N900-BUTTON" H -9060 -1060 65  0001 L TNN
	1    -9050 -850
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B12/FN3709
U 1 1 578D35A4
P -8150 -1150
F 0 "B12/FN3709" V -8320 -1010 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -7940 -1270 50  0000 L BNN
F 2 "N900-BUTTON" H -8160 -1360 65  0001 L TNN
	1    -8150 -1150
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B13/SPC2301
U 1 1 578D366C
P -7550 -850
F 0 "B13/SPC2301" V -7720 -710 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -7340 -970 50  0000 L BNN
F 2 "N900-BUTTON" H -7560 -1060 65  0001 L TNN
	1    -7550 -850
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B14/SPC2301
U 1 1 578D3734
P -6950 -850
F 0 "B14/SPC2301" V -7120 -710 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -6740 -970 50  0000 L BNN
F 2 "N900-BUTTON" H -6960 -1060 65  0001 L TNN
	1    -6950 -850
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B15/M3709
U 1 1 578D37FC
P -6350 -850
F 0 "B15/M3709" V -6520 -710 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -6140 -970 50  0000 L BNN
F 2 "N900-BUTTON" H -6360 -1060 65  0001 L TNN
	1    -6350 -850
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B16/,3709
U 1 1 578D38C4
P -5750 -850
F 0 "B16/,3709" V -5920 -710 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -5540 -970 50  0000 L BNN
F 2 "N900-BUTTON" H -5760 -1060 65  0001 L TNN
	1    -5750 -850
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B17/PGDN3709
U 1 1 578D398C
P -5150 -850
F 0 "B17/PGDN3709" V -5320 -710 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -4940 -970 50  0000 L BNN
F 2 "N900-BUTTON" H -5160 -1060 65  0001 L TNN
	1    -5150 -850
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B18/END3709
U 1 1 578D3A54
P -4550 -850
F 0 "B18/END3709" V -4720 -710 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -4340 -970 50  0000 L BNN
F 2 "N900-BUTTON" H -4560 -1060 65  0001 L TNN
	1    -4550 -850
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B19/LEFT3709
U 1 1 578D3B1C
P -3950 -850
F 0 "B19/LEFT3709" V -4120 -710 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -3740 -970 50  0000 L BNN
F 2 "N900-BUTTON" H -3960 -1060 65  0001 L TNN
	1    -3950 -850
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B20/RGHT3709
U 1 1 578D3BE4
P -3350 -850
F 0 "B20/RGHT3709" V -3520 -710 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -3140 -970 50  0000 L BNN
F 2 "N900-BUTTON" H -3360 -1060 65  0001 L TNN
	1    -3350 -850
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B21/DEL3709
U 1 1 578D3CAC
P -9050 -1450
F 0 "B21/DEL3709" V -9220 -1310 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -8840 -1570 50  0000 L BNN
F 2 "N900-BUTTON" H -9060 -1660 65  0001 L TNN
	1    -9050 -1450
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B22/3709
U 1 1 578D3D74
P -8150 -1750
F 0 "B22/3709" V -8320 -1610 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -7940 -1870 50  0000 L BNN
F 2 "N900-BUTTON" H -8160 -1960 65  0001 L TNN
	1    -8150 -1750
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B23/TAB3709
U 1 1 578D3E3C
P -7550 -1450
F 0 "B23/TAB3709" V -7720 -1310 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -7340 -1570 50  0000 L BNN
F 2 "N900-BUTTON" H -7560 -1660 65  0001 L TNN
	1    -7550 -1450
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B24/BSP3709
U 1 1 578D3F04
P -6950 -1450
F 0 "B24/BSP3709" V -7120 -1310 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -6740 -1570 50  0000 L BNN
F 2 "N900-BUTTON" H -6960 -1660 65  0001 L TNN
	1    -6950 -1450
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B25/RET3709
U 1 1 578D3FCC
P -6350 -1450
F 0 "B25/RET3709" V -6520 -1310 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -6140 -1570 50  0000 L BNN
F 2 "N900-BUTTON" H -6360 -1660 65  0001 L TNN
	1    -6350 -1450
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B26/PIPE3709
U 1 1 578D4094
P -5750 -1450
F 0 "B26/PIPE3709" V -5920 -1310 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -5540 -1570 50  0000 L BNN
F 2 "N900-BUTTON" H -5760 -1660 65  0001 L TNN
	1    -5750 -1450
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B27/PGUP3709
U 1 1 578D415C
P -5150 -1450
F 0 "B27/PGUP3709" V -5320 -1310 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -4940 -1570 50  0000 L BNN
F 2 "N900-BUTTON" H -5160 -1660 65  0001 L TNN
	1    -5150 -1450
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B28/HOME3709
U 1 1 578D4224
P -4550 -1450
F 0 "B28/HOME3709" V -4720 -1310 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -4340 -1570 50  0000 L BNN
F 2 "N900-BUTTON" H -4560 -1660 65  0001 L TNN
	1    -4550 -1450
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B29/UP3709
U 1 1 578D42EC
P -3950 -1450
F 0 "B29/UP3709" V -4120 -1310 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -3740 -1570 50  0000 L BNN
F 2 "N900-BUTTON" H -3960 -1660 65  0001 L TNN
	1    -3950 -1450
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B30/DN3709
U 1 1 578D43B4
P -3350 -1450
F 0 "B30/DN3709" V -3520 -1310 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -3140 -1570 50  0000 L BNN
F 2 "N900-BUTTON" H -3360 -1660 65  0001 L TNN
	1    -3350 -1450
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B31/ALT2301
U 1 1 578D447C
P -9050 -2050
F 0 "B31/ALT2301" V -9220 -1910 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -8840 -2170 50  0000 L BNN
F 2 "N900-BUTTON" H -9060 -2260 65  0001 L TNN
	1    -9050 -2050
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B32/SHLCK3709
U 1 1 578D4544
P -8150 -2350
F 0 "B32/SHLCK3709" V -8320 -2210 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -7940 -2470 50  0000 L BNN
F 2 "N900-BUTTON" H -8160 -2560 65  0001 L TNN
	1    -8150 -2350
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B33/ESC3709
U 1 1 578D460C
P -7550 -2050
F 0 "B33/ESC3709" V -7720 -1910 50  0000 R TNN
F 1 "BUTTONMAEANDER" V -7340 -2170 50  0000 L BNN
F 2 "N900-BUTTON" H -7560 -2260 65  0001 L TNN
	1    -7550 -2050
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B60/^2301
U 1 1 578D46D4
P -3950 -3050
F 0 "B60/^2301" V -4120 -2910 50  0000 R TNN
F 1 "BUTTON-N900" V -3740 -3170 50  0000 L BNN
F 2 "N900-BUTTON" H -3960 -3260 65  0001 L TNN
	1    -3950 -3050
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B60/^3709
U 1 1 578D479C
P -3350 -3050
F 0 "B60/^3709" V -3520 -2910 50  0000 R TNN
F 1 "BUTTON-N900" V -3140 -3170 50  0000 L BNN
F 2 "N900-BUTTON" H -3360 -3260 65  0001 L TNN
	1    -3350 -3050
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B69/O3709
U 1 1 578D4864
P -3950 -3450
F 0 "B69/O3709" V -4120 -3310 50  0000 R TNN
F 1 "BUTTON-N900" V -3740 -3570 50  0000 L BNN
F 2 "N900-BUTTON" H -3960 -3660 65  0001 L TNN
	1    -3950 -3450
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B70/P3709
U 1 1 578D492C
P -3350 -3450
F 0 "B70/P3709" V -3520 -3310 50  0000 R TNN
F 1 "BUTTON-N900" V -3140 -3570 50  0000 L BNN
F 2 "N900-BUTTON" H -3360 -3660 65  0001 L TNN
	1    -3350 -3450
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B79/2301
U 1 1 578D49F4
P -3950 -3850
F 0 "B79/2301" V -4120 -3710 50  0000 R TNN
F 1 "BUTTON-N900" V -3740 -3970 50  0000 L BNN
F 2 "N900-BUTTON" H -3960 -4060 65  0001 L TNN
	1    -3950 -3850
	0    -1    -1    0
$EndComp
$Comp
L BUTTON-N900 B80/2301
U 1 1 578D4ABC
P -3350 -3850
F 0 "B80/2301" V -3520 -3710 50  0000 R TNN
F 1 "BUTTON-N900" V -3140 -3970 50  0000 L BNN
F 2 "N900-BUTTON" H -3360 -4060 65  0001 L TNN
	1    -3350 -3850
	0    -1    -1    0
$EndComp
$Comp
L C0603 C2301
U 1 1 578D4B84
P -3550 -6250
F 0 "C2301" H -3490 -6236 50  0000 L BNN
F 1 "100nF" H -3491 -6435 50  0000 L BNN
F 2 "SMD-0603" H -3560 -6460 65  0001 L TNN
	1    -3550 -6250
	1    0    0    -1
$EndComp
$Comp
L D0603 D2301
U 1 1 578D4C4C
P -8050 -2050
F 0 "D2301" H -8050 -1880 50  0000 L BNN
F 1 "D0603" H -8050 -2250 50  0000 L BNN
F 2 "SMD-0603D" H -8060 -2260 65  0001 L TNN
	1    -8050 -2050
	1    0    0    -1
$EndComp
$Comp
L D0603 D2302
U 1 1 578D4D14
P -8650 -1950
F 0 "D2302" H -8650 -1780 50  0000 L BNN
F 1 "D0603" H -8650 -2150 50  0000 L BNN
F 2 "SMD-0603D" H -8660 -2160 65  0001 L TNN
	1    -8650 -1950
	-1    0    0    1
$EndComp
$Comp
L D0603 D2303
U 1 1 578D4DDC
P -8050 -1450
F 0 "D2303" H -8050 -1280 50  0000 L BNN
F 1 "D0603" H -8050 -1650 50  0000 L BNN
F 2 "SMD-0603D" H -8060 -1660 65  0001 L TNN
	1    -8050 -1450
	1    0    0    -1
$EndComp
$Comp
L D0603 D2304
U 1 1 578D4EA4
P -8650 -1350
F 0 "D2304" H -8650 -1180 50  0000 L BNN
F 1 "D0603" H -8650 -1550 50  0000 L BNN
F 2 "SMD-0603D" H -8660 -1560 65  0001 L TNN
	1    -8650 -1350
	-1    0    0    1
$EndComp
$Comp
L D0603 D2305
U 1 1 578D4F6C
P -8050 -850
F 0 "D2305" H -8050 -680 50  0000 L BNN
F 1 "D0603" H -8050 -1050 50  0000 L BNN
F 2 "SMD-0603D" H -8060 -1060 65  0001 L TNN
	1    -8050 -850
	1    0    0    -1
$EndComp
$Comp
L D0603 D2306
U 1 1 578D5034
P -8650 -750
F 0 "D2306" H -8650 -580 50  0000 L BNN
F 1 "D0603" H -8650 -950 50  0000 L BNN
F 2 "SMD-0603D" H -8660 -960 65  0001 L TNN
	1    -8650 -750
	-1    0    0    1
$EndComp
$Comp
L D0603 D2307
U 1 1 578D50FC
P -8050 -250
F 0 "D2307" H -8050 -80 50  0000 L BNN
F 1 "D0603" H -8050 -450 50  0000 L BNN
F 2 "SMD-0603D" H -8060 -460 65  0001 L TNN
	1    -8050 -250
	1    0    0    -1
$EndComp
$Comp
L D0603 D2308
U 1 1 578D51C4
P -8650 -150
F 0 "D2308" H -8650 20 50  0000 L BNN
F 1 "D0603" H -8650 -350 50  0000 L BNN
F 2 "SMD-0603D" H -8660 -360 65  0001 L TNN
	1    -8650 -150
	-1    0    0    1
$EndComp
$Comp
L DINA4_L #FRAME16
U 1 1 578D5228
P -9350 -450
	1    -9350 450 
	1    0    0    -1
$EndComp
$Comp
L DINA4_L #FRAME16
U 2 1 578D528C
P -2950 -450
F 0 "#FRAME16" H -2900 500 100  0000 L BNN
F 1 "Date:" H -2900 500 100  0000 L BNN
F 2 "" H -2950 450 60 0001 C CNN
F 3 "" H -2950 450 60 0001 C CNN
F 0 "#FRAME16" H -2450 499 100  0000 L BNN
F 1 "16 Jul 2016 23:57:11" H -2450 499 100  0000 L BNN
F 2 "" H -2950 450 60 0001 C CNN
F 3 "" H -2950 450 60 0001 C CNN
F 0 "#FRAME16" H -100 499 100  0000 L BNN
F 1 "Sheet:" H -100 499 100  0000 L BNN
F 2 "" H -2950 450 60 0001 C CNN
F 3 "" H -2950 450 60 0001 C CNN
F 0 "#FRAME16" H 450 499 100  0000 L BNN
F 1 "23/38" H 450 499 100  0000 L BNN
F 2 "" H -2950 450 60 0001 C CNN
F 3 "" H -2950 450 60 0001 C CNN
F 0 "#FRAME16" H 550 899 100  0000 L BNN
F 1 "REV:" H 550 899 100  0000 L BNN
F 2 "" H -2950 450 60 0001 C CNN
F 3 "" H -2950 450 60 0001 C CNN
F 0 "#FRAME16" H -2900 1200 100  0000 L BNN
F 1 "TITLE:" H -2900 1200 100  0000 L BNN
F 2 "" H -2950 450 60 0001 C CNN
F 3 "" H -2950 450 60 0001 C CNN
F 0 "#FRAME16" H -2900 900 100  0000 L BNN
F 1 "Document Number:" H -2900 900 100  0000 L BNN
F 2 "" H -2950 450 60 0001 C CNN
F 3 "" H -2950 450 60 0001 C CNN
F 0 "#FRAME16" H -2250 1200 100  0000 L BNN
F 1 "GTA04b7" H -2250 1200 100  0000 L BNN
F 2 "" H -2950 450 60 0001 C CNN
F 3 "" H -2950 450 60 0001 C CNN
	2    -2950 450 
	1    0    0    -1
$EndComp
$Comp
L GND-3 #U$99
U 1 1 578D52F0
P -3550 5950
F 0 "#U$99" H -3670 -6049 70  0000 L BNN
F 1 "GND3" H -3670 -6049 70  0000 L BNN
F 2 "" H -3550 -5950 60 0001 C CNN
F 3 "" H -3550 -5950 60 0001 C CNN
	1    -3550 -5950
	1    0    0    -1
$EndComp
$Comp
L TCA8418RTW U2301
U 1 1 578D53B8
P -2550 -4750
F 0 "U2301" H -2659 -2910 50  0000 L BNN
F 1 "TCA8418RTW" H -2650 -5650 50  0000 L BNN
F 2 "LPCC-24" H -2560 -4960 65  0001 L TNN
	1    -2550 -4750
	1    0    0    -1
$EndComp
Wire Wire Line
	-3250 -6450 -3550 -6450
Wire Wire Line
	-3550 -6450 -4050 -6450
Wire Wire Line
	-3550 -6350 -3550 -6450
Connection ~ -3550 -6450
Text Label -4110 -6401 2    65   ~ 0
1V8-CPU/24.1A
Wire Wire Line
	-9250 -850 -9250 -250
Wire Wire Line
	-9250 -1450 -9250 -850
Wire Wire Line
	-9250 -2050 -9250 -1450
Wire Wire Line
	-9250 -2650 -9250 -2050
Wire Wire Line
	-9250 -3050 -9250 -2650
Wire Wire Line
	-9250 -3450 -9250 -3050
Wire Wire Line
	-9250 -3850 -9250 -3450
Wire Wire Line
	-9250 -4050 -9250 -3850
Wire Wire Line
	-9250 -4150 -9250 -4050
Wire Wire Line
	-9250 -5850 -9250 -4150
Wire Wire Line
	-3250 -5850 -9250 -5850
Connection ~ -9250 -850
Connection ~ -9250 -1450
Connection ~ -9250 -2050
Connection ~ -9250 -250
Wire Wire Line
	-8350 -1150 -8350 -550
Wire Wire Line
	-8350 -1750 -8350 -1150
Wire Wire Line
	-8350 -2350 -8350 -1750
Wire Wire Line
	-8350 -2650 -8350 -2350
Wire Wire Line
	-8350 -3050 -8350 -2650
Wire Wire Line
	-8350 -3450 -8350 -3050
Wire Wire Line
	-8350 -3850 -8350 -3450
Wire Wire Line
	-3250 -5650 -8350 -5650
Wire Wire Line
	-8350 -5650 -8350 -3950
Wire Wire Line
	-8350 -3950 -8350 -3850
Wire Wire Line
	-8350 -3850 -8350 -3450
Wire Wire Line
	-8350 -3450 -8350 -3050
Wire Wire Line
	-8350 -3050 -8350 -2650
Wire Wire Line
	-8350 -2650 -8350 -2350
Wire Wire Line
	-8350 -2350 -8350 -1750
Wire Wire Line
	-8350 -1750 -8350 -1150
Wire Wire Line
	-8350 -1150 -8350 -550
Connection ~ -8350 -3850
Connection ~ -8350 -3450
Connection ~ -8350 -3050
Connection ~ -8350 -2650
Connection ~ -8350 -2350
Connection ~ -8350 -1750
Connection ~ -8350 -1150
Connection ~ -8350 -550
Wire Wire Line
	-7750 -850 -7750 -250
Wire Wire Line
	-7750 -1450 -7750 -850
Wire Wire Line
	-7750 -2050 -7750 -1450
Wire Wire Line
	-7750 -2650 -7750 -2050
Wire Wire Line
	-7750 -3050 -7750 -2650
Wire Wire Line
	-7750 -3450 -7750 -3050
Wire Wire Line
	-7750 -3850 -7750 -3450
Wire Wire Line
	-3250 -5450 -7750 -5450
Wire Wire Line
	-7750 -5450 -7750 -3950
Wire Wire Line
	-7750 -3950 -7750 -3850
Wire Wire Line
	-7750 -3850 -7750 -3450
Wire Wire Line
	-7750 -3450 -7750 -3050
Wire Wire Line
	-7750 -3050 -7750 -2650
Wire Wire Line
	-7750 -2650 -7750 -2050
Wire Wire Line
	-7750 -2050 -7750 -1450
Wire Wire Line
	-7750 -1450 -7750 -850
Wire Wire Line
	-7750 -850 -7750 -250
Connection ~ -7750 -3850
Connection ~ -7750 -3450
Connection ~ -7750 -3050
Connection ~ -7750 -2650
Connection ~ -7750 -2050
Connection ~ -7750 -1450
Connection ~ -7750 -850
Connection ~ -7750 -250
Wire Wire Line
	-7150 -3150 -7150 -3050
Wire Wire Line
	-7150 -3050 -7150 -2650
Wire Wire Line
	-7150 -2650 -7150 -2050
Wire Wire Line
	-7150 -2050 -7150 -1450
Wire Wire Line
	-7150 -1450 -7150 -850
Wire Wire Line
	-7150 -850 -7150 -250
Wire Wire Line
	-7150 -3550 -7150 -3450
Wire Wire Line
	-7150 -3450 -7150 -3050
Wire Wire Line
	-7150 -3050 -7150 -2650
Wire Wire Line
	-7150 -2650 -7150 -2050
Wire Wire Line
	-7150 -2050 -7150 -1450
Wire Wire Line
	-7150 -1450 -7150 -850
Wire Wire Line
	-7150 -3950 -7150 -3450
Wire Wire Line
	-7150 -3450 -7150 -3050
Wire Wire Line
	-7150 -3050 -7150 -2650
Wire Wire Line
	-7150 -2650 -7150 -2050
Wire Wire Line
	-7150 -2050 -7150 -1450
Wire Wire Line
	-7150 -4250 -7150 -3450
Wire Wire Line
	-7150 -3450 -7150 -3050
Wire Wire Line
	-7150 -3050 -7150 -2650
Wire Wire Line
	-7150 -2650 -7150 -2050
Wire Wire Line
	-7150 -4350 -7150 -3450
Wire Wire Line
	-7150 -3450 -7150 -3050
Wire Wire Line
	-7150 -3050 -7150 -2650
Wire Wire Line
	-7150 -4650 -7150 -3450
Wire Wire Line
	-7150 -3450 -7150 -3050
Wire Wire Line
	-7150 -4950 -7150 -3450
Wire Wire Line
	-3250 -5250 -7150 -5250
Wire Wire Line
	-7150 -5250 -7150 -3850
Wire Wire Line
	-7150 -3850 -7150 -3450
Wire Wire Line
	-7150 -3450 -7150 -3050
Wire Wire Line
	-7150 -3050 -7150 -2650
Wire Wire Line
	-7150 -2650 -7150 -2050
Wire Wire Line
	-7150 -2050 -7150 -1450
Wire Wire Line
	-7150 -1450 -7150 -850
Wire Wire Line
	-7150 -850 -7150 -250
Connection ~ -7150 -3450
Connection ~ -7150 -3050
Connection ~ -7150 -2650
Connection ~ -7150 -2050
Connection ~ -7150 -1450
Connection ~ -7150 -850
Connection ~ -7150 -250
Wire Wire Line
	-6550 -2950 -6550 -2650
Wire Wire Line
	-6550 -2650 -6550 -2050
Wire Wire Line
	-6550 -2050 -6550 -1450
Wire Wire Line
	-6550 -1450 -6550 -850
Wire Wire Line
	-6550 -850 -6550 -450
Wire Wire Line
	-6550 -450 -6550 -250
Wire Wire Line
	-6550 -3350 -6550 -3050
Wire Wire Line
	-6550 -3050 -6550 -2650
Wire Wire Line
	-6550 -2650 -6550 -2050
Wire Wire Line
	-6550 -2050 -6550 -1450
Wire Wire Line
	-6550 -1450 -6550 -1250
Wire Wire Line
	-6550 -1250 -6550 -850
Wire Wire Line
	-6550 -3450 -6550 -3050
Wire Wire Line
	-6550 -3050 -6550 -2650
Wire Wire Line
	-6550 -2650 -6550 -2050
Wire Wire Line
	-6550 -2050 -6550 -1650
Wire Wire Line
	-6550 -1650 -6550 -1450
Wire Wire Line
	-6550 -3450 -6550 -3050
Wire Wire Line
	-6550 -3050 -6550 -2650
Wire Wire Line
	-6550 -2650 -6550 -2150
Wire Wire Line
	-6550 -2150 -6550 -2050
Wire Wire Line
	-6550 -3450 -6550 -3050
Wire Wire Line
	-6550 -3050 -6550 -2850
Wire Wire Line
	-6550 -2850 -6550 -2650
Wire Wire Line
	-6550 -3450 -6550 -3150
Wire Wire Line
	-6550 -3150 -6550 -3050
Wire Wire Line
	-3250 -5050 -6550 -5050
Wire Wire Line
	-6550 -5050 -6550 -3950
Wire Wire Line
	-6550 -3950 -6550 -3850
Wire Wire Line
	-6550 -3850 -6550 -3450
Wire Wire Line
	-6550 -3450 -6550 -3050
Wire Wire Line
	-6550 -3050 -6550 -2650
Wire Wire Line
	-6550 -2650 -6550 -2050
Wire Wire Line
	-6550 -2050 -6550 -1450
Wire Wire Line
	-6550 -1450 -6550 -850
Wire Wire Line
	-6550 -850 -6550 -250
Connection ~ -6550 -3450
Connection ~ -6550 -3050
Connection ~ -6550 -2650
Connection ~ -6550 -2050
Connection ~ -6550 -1450
Connection ~ -6550 -850
Connection ~ -6550 -250
Wire Wire Line
	-5950 -250 -5950 -750
Wire Wire Line
	-5950 -750 -5950 -850
Wire Wire Line
	-5950 -850 -5950 -1450
Wire Wire Line
	-5950 -1450 -5950 -2050
Wire Wire Line
	-5950 -2050 -5950 -2650
Wire Wire Line
	-5950 -850 -5950 -1350
Wire Wire Line
	-5950 -1350 -5950 -1450
Wire Wire Line
	-5950 -1450 -5950 -2050
Wire Wire Line
	-5950 -2050 -5950 -2650
Wire Wire Line
	-5950 -2650 -5950 -3050
Wire Wire Line
	-5950 -1450 -5950 -1950
Wire Wire Line
	-5950 -1950 -5950 -2050
Wire Wire Line
	-5950 -2050 -5950 -2650
Wire Wire Line
	-5950 -2650 -5950 -3050
Wire Wire Line
	-5950 -3050 -5950 -3450
Wire Wire Line
	-5950 -2050 -5950 -2550
Wire Wire Line
	-5950 -2550 -5950 -2650
Wire Wire Line
	-5950 -2650 -5950 -3050
Wire Wire Line
	-5950 -3050 -5950 -3450
Wire Wire Line
	-5950 -3450 -5950 -3850
Wire Wire Line
	-5950 -2650 -5950 -2950
Wire Wire Line
	-5950 -2950 -5950 -3050
Wire Wire Line
	-5950 -3050 -5950 -3450
Wire Wire Line
	-5950 -3450 -5950 -3850
Wire Wire Line
	-5950 -3050 -5950 -3350
Wire Wire Line
	-5950 -3350 -5950 -3450
Wire Wire Line
	-5950 -3450 -5950 -3850
Wire Wire Line
	-5950 -3450 -5950 -3750
Wire Wire Line
	-5950 -3750 -5950 -3850
Wire Wire Line
	-5950 -250 -5950 -850
Wire Wire Line
	-5950 -850 -5950 -1450
Wire Wire Line
	-5950 -1450 -5950 -2050
Wire Wire Line
	-5950 -2050 -5950 -2650
Wire Wire Line
	-5950 -2650 -5950 -3050
Wire Wire Line
	-5950 -3050 -5950 -3450
Wire Wire Line
	-5950 -3450 -5950 -3850
Wire Wire Line
	-5950 -3850 -5950 -4850
Wire Wire Line
	-5950 -4850 -3250 -4850
Connection ~ -5950 -3850
Connection ~ -5950 -3450
Connection ~ -5950 -3050
Connection ~ -5950 -2650
Connection ~ -5950 -2050
Connection ~ -5950 -1450
Connection ~ -5950 -850
Connection ~ -5950 -250
Wire Wire Line
	-5350 -850 -5350 -250
Wire Wire Line
	-5350 -1450 -5350 -850
Wire Wire Line
	-5350 -2050 -5350 -1450
Wire Wire Line
	-5350 -2650 -5350 -2050
Wire Wire Line
	-5350 -3050 -5350 -2650
Wire Wire Line
	-5350 -3450 -5350 -3050
Wire Wire Line
	-5350 -3850 -5350 -3450
Wire Wire Line
	-3250 -4650 -5350 -4650
Wire Wire Line
	-5350 -4650 -5350 -3950
Wire Wire Line
	-5350 -3950 -5350 -3850
Wire Wire Line
	-5350 -3850 -5350 -3450
Wire Wire Line
	-5350 -3450 -5350 -3050
Wire Wire Line
	-5350 -3050 -5350 -2650
Wire Wire Line
	-5350 -2650 -5350 -2050
Wire Wire Line
	-5350 -2050 -5350 -1450
Wire Wire Line
	-5350 -1450 -5350 -850
Wire Wire Line
	-5350 -850 -5350 -250
Connection ~ -5350 -3850
Connection ~ -5350 -3450
Connection ~ -5350 -3050
Connection ~ -5350 -2650
Connection ~ -5350 -2050
Connection ~ -5350 -1450
Connection ~ -5350 -850
Connection ~ -5350 -250
Wire Wire Line
	-4750 -850 -4750 -250
Wire Wire Line
	-4750 -1450 -4750 -850
Wire Wire Line
	-4750 -2050 -4750 -1450
Wire Wire Line
	-4750 -2650 -4750 -2050
Wire Wire Line
	-4750 -3050 -4750 -2650
Wire Wire Line
	-4750 -3450 -4750 -3050
Wire Wire Line
	-4750 -3850 -4750 -3450
Wire Wire Line
	-3250 -4450 -4750 -4450
Wire Wire Line
	-4750 -4450 -4750 -3950
Wire Wire Line
	-4750 -3950 -4750 -3850
Wire Wire Line
	-4750 -3850 -4750 -3450
Wire Wire Line
	-4750 -3450 -4750 -3050
Wire Wire Line
	-4750 -3050 -4750 -2650
Wire Wire Line
	-4750 -2650 -4750 -2050
Wire Wire Line
	-4750 -2050 -4750 -1450
Wire Wire Line
	-4750 -1450 -4750 -850
Wire Wire Line
	-4750 -850 -4750 -250
Connection ~ -4750 -3850
Connection ~ -4750 -3450
Connection ~ -4750 -3050
Connection ~ -4750 -2650
Connection ~ -4750 -2050
Connection ~ -4750 -1450
Connection ~ -4750 -850
Connection ~ -4750 -250
Wire Wire Line
	-4150 -1450 -4150 -850
Wire Wire Line
	-4150 -850 -4150 -250
Wire Wire Line
	-4150 -2050 -4150 -1450
Wire Wire Line
	-4150 -1450 -4150 -850
Wire Wire Line
	-4150 -2650 -4150 -2050
Wire Wire Line
	-4150 -2050 -4150 -1450
Wire Wire Line
	-4150 -3050 -4150 -2650
Wire Wire Line
	-4150 -2650 -4150 -2050
Wire Wire Line
	-4150 -3450 -4150 -3050
Wire Wire Line
	-4150 -3050 -4150 -2650
Wire Wire Line
	-4150 -3850 -4150 -3450
Wire Wire Line
	-4150 -3450 -4150 -3050
Wire Wire Line
	-4150 -3950 -4150 -3450
Wire Wire Line
	-3250 -4250 -4150 -4250
Wire Wire Line
	-4150 -4250 -4150 -3850
Wire Wire Line
	-4150 -3850 -4150 -3450
Wire Wire Line
	-4150 -3450 -4150 -3050
Wire Wire Line
	-4150 -3050 -4150 -2650
Wire Wire Line
	-4150 -2650 -4150 -2050
Wire Wire Line
	-4150 -2050 -4150 -1450
Wire Wire Line
	-4150 -1450 -4150 -850
Wire Wire Line
	-4150 -850 -4150 -250
Connection ~ -4150 -3850
Connection ~ -4150 -3450
Connection ~ -4150 -3050
Connection ~ -4150 -2650
Connection ~ -4150 -2050
Connection ~ -4150 -1450
Connection ~ -4150 -850
Connection ~ -4150 -250
Wire Wire Line
	-3550 -1250 -3550 -850
Wire Wire Line
	-3550 -850 -3550 -250
Wire Wire Line
	-3550 -1650 -3550 -1450
Wire Wire Line
	-3550 -1450 -3550 -850
Wire Wire Line
	-3550 -2150 -3550 -2050
Wire Wire Line
	-3550 -2050 -3550 -1450
Wire Wire Line
	-3550 -2850 -3550 -2650
Wire Wire Line
	-3550 -2650 -3550 -2050
Wire Wire Line
	-3550 -3150 -3550 -3050
Wire Wire Line
	-3550 -3050 -3550 -2650
Wire Wire Line
	-3550 -3550 -3550 -3450
Wire Wire Line
	-3550 -3450 -3550 -3050
Wire Wire Line
	-3550 -3950 -3550 -3850
Wire Wire Line
	-3550 -3850 -3550 -3450
Wire Wire Line
	-3250 -4050 -3550 -4050
Wire Wire Line
	-3550 -4050 -3550 -3850
Wire Wire Line
	-3550 -3850 -3550 -3450
Wire Wire Line
	-3550 -3450 -3550 -3050
Wire Wire Line
	-3550 -3050 -3550 -2650
Wire Wire Line
	-3550 -2650 -3550 -2050
Wire Wire Line
	-3550 -2050 -3550 -1450
Wire Wire Line
	-3550 -1450 -3550 -850
Wire Wire Line
	-3550 -850 -3550 -250
Connection ~ -3550 -3850
Connection ~ -3550 -3450
Connection ~ -3550 -3050
Connection ~ -3550 -2650
Connection ~ -3550 -2050
Connection ~ -3550 -1450
Connection ~ -3550 -850
Connection ~ -3550 -250
Wire Wire Line
	-3250 -6050 -3550 -6050
Connection ~ -3550 -6050
Wire Wire Line
	-1850 -6250 -1550 -6250
Wire Wire Line
	-1550 -6250 -650 -6250
Text Label -590 -6215 0    65   ~ 0
I2C2-SCL-CPU/24.3B
Wire Wire Line
	-1850 -6450 -1450 -6450
Wire Wire Line
	-1450 -6450 -650 -6450
Text Label -590 -6415 0    65   ~ 0
I2C2-SDA-CPU/24.3B
Wire Wire Line
	-1850 -6050 -1650 -6050
Wire Wire Line
	-1650 -6050 -650 -6050
Text Label -590 -6015 0    65   ~ 0
KEYIRQ_UPPER
Wire Wire Line
	-8850 -150 -8850 -250
Wire Wire Line
	-8850 -850 -8850 -750
Wire Wire Line
	-8850 -1450 -8850 -1350
Wire Wire Line
	-8850 -2050 -8850 -1950
Wire Wire Line
	-7850 -250 -7850 -550
Wire Wire Line
	-7850 -550 -7950 -550
Wire Wire Line
	-7850 -850 -7850 -1150
Wire Wire Line
	-7850 -1150 -7950 -1150
Wire Wire Line
	-7850 -1450 -7850 -1750
Wire Wire Line
	-7850 -1750 -7950 -1750
Wire Wire Line
	-7850 -2050 -7850 -2350
Wire Wire Line
	-7850 -2350 -7950 -2350
Wire Wire Line
	-4050 -6250 -3250 -6250
Text Label -4110 -6201 2    65   ~ 0
RESET_UPPER
Wire Wire Line
	-7950 -150 -8250 -150
Wire Wire Line
	-8250 -150 -8450 -150
Wire Wire Line
	-7350 -150 -7950 -150
Wire Wire Line
	-7350 -250 -7350 -150
Wire Wire Line
	-6750 -150 -7350 -150
Wire Wire Line
	-6750 -250 -6750 -150
Wire Wire Line
	-6150 -150 -6750 -150
Wire Wire Line
	-6150 -250 -6150 -150
Wire Wire Line
	-5550 -150 -6150 -150
Wire Wire Line
	-5550 -250 -5550 -150
Wire Wire Line
	-4950 -150 -5550 -150
Wire Wire Line
	-4950 -250 -4950 -150
Wire Wire Line
	-4350 -150 -4950 -150
Wire Wire Line
	-4350 -250 -4350 -150
Wire Wire Line
	-3750 -150 -4350 -150
Wire Wire Line
	-3750 -250 -3750 -150
Wire Wire Line
	-3150 -150 -3750 -150
Wire Wire Line
	-3150 -250 -3150 -150
Wire Wire Line
	-3050 -150 -3150 -150
Wire Wire Line
	-3050 -1150 -3050 -150
Wire Wire Line
	-1850 -5450 -1150 -5450
Wire Wire Line
	-1150 -5450 -1150 -1150
Wire Wire Line
	-1150 -1150 -3050 -1150
Wire Wire Line
	-8250 -250 -8250 -150
Connection ~ -7350 -150
Connection ~ -6750 -150
Connection ~ -6150 -150
Connection ~ -5550 -150
Connection ~ -4950 -150
Connection ~ -4350 -150
Connection ~ -3750 -150
Connection ~ -3150 -150
Connection ~ -8250 -150
Wire Wire Line
	-7950 -750 -8250 -750
Wire Wire Line
	-8250 -750 -8450 -750
Wire Wire Line
	-7350 -750 -7950 -750
Wire Wire Line
	-7350 -850 -7350 -750
Wire Wire Line
	-6750 -750 -7350 -750
Wire Wire Line
	-6750 -850 -6750 -750
Wire Wire Line
	-6150 -750 -6750 -750
Wire Wire Line
	-6150 -850 -6150 -750
Wire Wire Line
	-5550 -750 -6150 -750
Wire Wire Line
	-5550 -850 -5550 -750
Wire Wire Line
	-4950 -750 -5550 -750
Wire Wire Line
	-4950 -850 -4950 -750
Wire Wire Line
	-4350 -750 -4950 -750
Wire Wire Line
	-4350 -850 -4350 -750
Wire Wire Line
	-3750 -750 -4350 -750
Wire Wire Line
	-3750 -850 -3750 -750
Wire Wire Line
	-3150 -750 -3750 -750
Wire Wire Line
	-3150 -850 -3150 -750
Wire Wire Line
	-3150 -1250 -3150 -850
Wire Wire Line
	-3150 -850 -3150 -750
Wire Wire Line
	-1850 -5250 -1250 -5250
Wire Wire Line
	-1250 -5250 -1250 -1250
Wire Wire Line
	-1250 -1250 -3150 -1250
Wire Wire Line
	-8250 -850 -8250 -750
Connection ~ -7350 -750
Connection ~ -6750 -750
Connection ~ -6150 -750
Connection ~ -5550 -750
Connection ~ -4950 -750
Connection ~ -4350 -750
Connection ~ -3750 -750
Connection ~ -3150 -750
Connection ~ -8250 -750
Connection ~ -3150 -850
Wire Wire Line
	-7950 -1350 -8250 -1350
Wire Wire Line
	-8250 -1350 -8450 -1350
Wire Wire Line
	-7350 -1350 -7950 -1350
Wire Wire Line
	-7350 -1450 -7350 -1350
Wire Wire Line
	-6750 -1350 -7350 -1350
Wire Wire Line
	-6750 -1450 -6750 -1350
Wire Wire Line
	-6150 -1350 -6750 -1350
Wire Wire Line
	-6150 -1450 -6150 -1350
Wire Wire Line
	-5550 -1350 -6150 -1350
Wire Wire Line
	-5550 -1450 -5550 -1350
Wire Wire Line
	-4950 -1350 -5550 -1350
Wire Wire Line
	-4950 -1450 -4950 -1350
Wire Wire Line
	-4350 -1350 -4950 -1350
Wire Wire Line
	-4350 -1450 -4350 -1350
Wire Wire Line
	-3750 -1350 -4350 -1350
Wire Wire Line
	-3750 -1450 -3750 -1350
Wire Wire Line
	-3150 -1350 -3750 -1350
Wire Wire Line
	-3150 -1350 -3150 -1450
Wire Wire Line
	-1850 -5050 -1350 -5050
Wire Wire Line
	-1350 -5050 -1350 -1350
Wire Wire Line
	-1350 -1350 -3150 -1350
Wire Wire Line
	-8250 -1450 -8250 -1350
Connection ~ -7350 -1350
Connection ~ -6750 -1350
Connection ~ -6150 -1350
Connection ~ -5550 -1350
Connection ~ -4950 -1350
Connection ~ -4350 -1350
Connection ~ -3750 -1350
Connection ~ -3150 -1350
Connection ~ -8250 -1350
Wire Wire Line
	-7950 -1950 -8250 -1950
Wire Wire Line
	-8250 -1950 -8450 -1950
Wire Wire Line
	-7350 -1950 -7950 -1950
Wire Wire Line
	-7350 -2050 -7350 -1950
Wire Wire Line
	-6750 -1950 -7350 -1950
Wire Wire Line
	-6750 -2050 -6750 -1950
Wire Wire Line
	-6150 -1950 -6750 -1950
Wire Wire Line
	-6150 -2050 -6150 -1950
Wire Wire Line
	-5550 -1950 -6150 -1950
Wire Wire Line
	-5550 -2050 -5550 -1950
Wire Wire Line
	-4950 -1950 -5550 -1950
Wire Wire Line
	-4950 -2050 -4950 -1950
Wire Wire Line
	-4350 -1950 -4950 -1950
Wire Wire Line
	-4350 -2050 -4350 -1950
Wire Wire Line
	-3750 -1950 -4350 -1950
Wire Wire Line
	-3750 -2050 -3750 -1950
Wire Wire Line
	-3150 -1950 -3750 -1950
Wire Wire Line
	-3150 -2050 -3150 -1950
Wire Wire Line
	-1850 -4850 -1450 -4850
Wire Wire Line
	-1450 -4850 -1450 -1950
Wire Wire Line
	-1450 -1950 -3150 -1950
Wire Wire Line
	-8250 -2050 -8250 -1950
Connection ~ -7350 -1950
Connection ~ -6750 -1950
Connection ~ -6150 -1950
Connection ~ -5550 -1950
Connection ~ -4950 -1950
Connection ~ -4350 -1950
Connection ~ -3750 -1950
Connection ~ -3150 -1950
Connection ~ -8250 -1950
Wire Wire Line
	-7950 -2550 -8450 -2550
Wire Wire Line
	-7950 -2650 -7950 -2550
Wire Wire Line
	-7350 -2550 -7950 -2550
Wire Wire Line
	-7350 -2650 -7350 -2550
Wire Wire Line
	-6750 -2550 -7350 -2550
Wire Wire Line
	-6750 -2650 -6750 -2550
Wire Wire Line
	-6150 -2550 -6750 -2550
Wire Wire Line
	-6150 -2650 -6150 -2550
Wire Wire Line
	-5550 -2550 -6150 -2550
Wire Wire Line
	-5550 -2650 -5550 -2550
Wire Wire Line
	-4950 -2550 -5550 -2550
Wire Wire Line
	-4950 -2650 -4950 -2550
Wire Wire Line
	-4350 -2550 -4950 -2550
Wire Wire Line
	-4350 -2650 -4350 -2550
Wire Wire Line
	-3750 -2550 -4350 -2550
Wire Wire Line
	-3750 -2650 -3750 -2550
Wire Wire Line
	-3150 -2550 -3750 -2550
Wire Wire Line
	-3150 -2650 -3150 -2550
Wire Wire Line
	-1850 -4650 -1550 -4650
Wire Wire Line
	-1550 -4650 -1550 -2550
Wire Wire Line
	-1550 -2550 -3150 -2550
Connection ~ -7950 -2550
Connection ~ -7350 -2550
Connection ~ -6750 -2550
Connection ~ -6150 -2550
Connection ~ -5550 -2550
Connection ~ -4950 -2550
Connection ~ -4350 -2550
Connection ~ -3750 -2550
Connection ~ -3150 -2550
Wire Wire Line
	-8450 -2950 -7950 -2950
Wire Wire Line
	-7950 -3050 -7950 -2950
Wire Wire Line
	-7950 -2950 -7450 -2950
Wire Wire Line
	-7350 -2950 -7450 -2950
Wire Wire Line
	-7350 -3050 -7350 -2950
Wire Wire Line
	-6750 -2950 -7350 -2950
Wire Wire Line
	-6750 -3050 -6750 -2950
Wire Wire Line
	-6150 -2950 -6750 -2950
Wire Wire Line
	-6150 -3050 -6150 -2950
Wire Wire Line
	-5550 -2950 -6150 -2950
Wire Wire Line
	-5550 -3050 -5550 -2950
Wire Wire Line
	-4950 -2950 -5550 -2950
Wire Wire Line
	-4950 -3050 -4950 -2950
Wire Wire Line
	-4350 -2950 -4950 -2950
Wire Wire Line
	-4350 -3050 -4350 -2950
Wire Wire Line
	-3750 -2950 -4350 -2950
Wire Wire Line
	-3750 -3050 -3750 -2950
Wire Wire Line
	-3150 -2950 -3750 -2950
Wire Wire Line
	-3150 -3050 -3150 -2950
Wire Wire Line
	-1850 -4450 -1650 -4450
Wire Wire Line
	-1650 -4450 -1650 -2950
Wire Wire Line
	-1650 -2950 -3150 -2950
Connection ~ -7950 -2950
Connection ~ -7350 -2950
Connection ~ -6750 -2950
Connection ~ -6150 -2950
Connection ~ -5550 -2950
Connection ~ -4950 -2950
Connection ~ -4350 -2950
Connection ~ -3750 -2950
Connection ~ -3150 -2950
Wire Wire Line
	-5550 -3450 -5550 -3350
Wire Wire Line
	-4950 -3350 -5550 -3350
Wire Wire Line
	-4950 -3450 -4950 -3350
Wire Wire Line
	-4350 -3350 -4950 -3350
Wire Wire Line
	-4350 -3450 -4350 -3350
Wire Wire Line
	-3750 -3350 -4350 -3350
Wire Wire Line
	-3750 -3450 -3750 -3350
Wire Wire Line
	-3150 -3350 -3750 -3350
Wire Wire Line
	-3150 -3450 -3150 -3350
Wire Wire Line
	-1850 -4250 -1750 -4250
Wire Wire Line
	-1750 -4250 -1750 -3350
Wire Wire Line
	-1750 -3350 -3150 -3350
Wire Wire Line
	-7950 -3450 -7950 -3350
Wire Wire Line
	-7350 -3450 -7350 -3350
Wire Wire Line
	-6750 -3450 -6750 -3350
Wire Wire Line
	-6150 -3450 -6150 -3350
Wire Wire Line
	-6150 -3350 -6750 -3350
Wire Wire Line
	-6750 -3350 -7350 -3350
Wire Wire Line
	-7350 -3350 -7950 -3350
Wire Wire Line
	-7950 -3350 -8050 -3350
Wire Wire Line
	-8450 -3350 -8050 -3350
Wire Wire Line
	-6150 -3350 -5550 -3350
Connection ~ -4950 -3350
Connection ~ -4350 -3350
Connection ~ -3750 -3350
Connection ~ -3150 -3350
Connection ~ -6750 -3350
Connection ~ -7350 -3350
Connection ~ -7950 -3350
Connection ~ -6150 -3350
Connection ~ -5550 -3350
Wire Wire Line
	-8450 -3750 -8050 -3750
Wire Wire Line
	-7950 -3750 -8050 -3750
Wire Wire Line
	-7950 -3850 -7950 -3750
Wire Wire Line
	-7350 -3750 -7950 -3750
Wire Wire Line
	-7350 -3850 -7350 -3750
Wire Wire Line
	-6750 -3750 -7350 -3750
Wire Wire Line
	-6750 -3850 -6750 -3750
Wire Wire Line
	-6150 -3750 -6750 -3750
Wire Wire Line
	-6150 -3850 -6150 -3750
Wire Wire Line
	-5550 -3750 -6150 -3750
Wire Wire Line
	-5550 -3850 -5550 -3750
Wire Wire Line
	-4950 -3750 -5550 -3750
Wire Wire Line
	-4950 -3850 -4950 -3750
Wire Wire Line
	-4350 -3750 -4950 -3750
Wire Wire Line
	-4350 -3850 -4350 -3750
Wire Wire Line
	-3750 -3750 -4350 -3750
Wire Wire Line
	-3750 -3850 -3750 -3750
Wire Wire Line
	-1850 -4050 -1850 -3750
Wire Wire Line
	-1850 -3750 -3150 -3750
Wire Wire Line
	-3150 -3750 -3750 -3750
Wire Wire Line
	-3150 -3850 -3150 -3750
Connection ~ -7950 -3750
Connection ~ -7350 -3750
Connection ~ -6750 -3750
Connection ~ -6150 -3750
Connection ~ -5550 -3750
Connection ~ -4950 -3750
Connection ~ -4350 -3750
Connection ~ -3750 -3750
Connection ~ -3150 -3750
Text GLabel -3250 -6450 0 10 UnSpc
1V8-CPU
Text GLabel -3550 -6350 1 10 UnSpc
1V8-CPU
Text Label -9250 -250 3    10   ~ 0
COL0_UPPER
Text Label -9250 -850 3    10   ~ 0
COL0_UPPER
Text Label -9250 -1450 3    10   ~ 0
COL0_UPPER
Text Label -9250 -2050 3    10   ~ 0
COL0_UPPER
Text Label -3250 -5850 2    10   ~ 0
COL0_UPPER
Text Label -8350 -550 3    10   ~ 0
COL1_UPPER
Text Label -8350 -1150 3    10   ~ 0
COL1_UPPER
Text Label -8350 -1750 3    10   ~ 0
COL1_UPPER
Text Label -8350 -2350 3    10   ~ 0
COL1_UPPER
Text Label -3250 -5650 2    10   ~ 0
COL1_UPPER
Text Label -7750 -250 3    10   ~ 0
COL2_UPPER
Text Label -7750 -850 3    10   ~ 0
COL2_UPPER
Text Label -7750 -1450 3    10   ~ 0
COL2_UPPER
Text Label -7750 -2050 3    10   ~ 0
COL2_UPPER
Text Label -3250 -5450 2    10   ~ 0
COL2_UPPER
Text Label -7150 -250 3    10   ~ 0
COL3_UPPER
Text Label -7150 -850 3    10   ~ 0
COL3_UPPER
Text Label -7150 -1450 3    10   ~ 0
COL3_UPPER
Text Label -3250 -5250 2    10   ~ 0
COL3_UPPER
Text Label -6550 -250 3    10   ~ 0
COL4_UPPER
Text Label -6550 -850 3    10   ~ 0
COL4_UPPER
Text Label -6550 -1450 3    10   ~ 0
COL4_UPPER
Text Label -3250 -5050 2    10   ~ 0
COL4_UPPER
Text Label -5950 -250 3    10   ~ 0
COL5_UPPER
Text Label -5950 -850 3    10   ~ 0
COL5_UPPER
Text Label -5950 -1450 3    10   ~ 0
COL5_UPPER
Text Label -3250 -4850 2    10   ~ 0
COL5_UPPER
Text Label -5350 -250 3    10   ~ 0
COL6_UPPER
Text Label -5350 -850 3    10   ~ 0
COL6_UPPER
Text Label -5350 -1450 3    10   ~ 0
COL6_UPPER
Text Label -3250 -4650 2    10   ~ 0
COL6_UPPER
Text Label -4750 -250 3    10   ~ 0
COL7_UPPER
Text Label -4750 -850 3    10   ~ 0
COL7_UPPER
Text Label -4750 -1450 3    10   ~ 0
COL7_UPPER
Text Label -3250 -4450 2    10   ~ 0
COL7_UPPER
Text Label -4150 -250 3    10   ~ 0
COL8_UPPER
Text Label -4150 -850 3    10   ~ 0
COL8_UPPER
Text Label -4150 -1450 3    10   ~ 0
COL8_UPPER
Text Label -4150 -3450 3    10   ~ 0
COL8_UPPER
Text Label -3250 -4250 2    10   ~ 0
COL8_UPPER
Text Label -4150 -3850 3    10   ~ 0
COL8_UPPER
Text Label -4150 -3050 3    10   ~ 0
COL8_UPPER
Text Label -3550 -250 3    10   ~ 0
COL9_UPPER
Text Label -3550 -850 3    10   ~ 0
COL9_UPPER
Text Label -3550 -1450 3    10   ~ 0
COL9_UPPER
Text Label -3550 -3050 3    10   ~ 0
COL9_UPPER
Text Label -3550 -3450 3    10   ~ 0
COL9_UPPER
Text Label -3250 -4050 2    10   ~ 0
COL9_UPPER
Text Label -3550 -3850 3    10   ~ 0
COL9_UPPER
Text GLabel -3250 -6050 0 10 UnSpc
GND3
Text GLabel -3550 -6050 3 10 UnSpc
GND3
Text GLabel -3550 -6050 1 10 UnSpc
GND3
Text GLabel -1850 -6250 2 10 UnSpc
I2C2-SCL-CPU
Text GLabel -1850 -6450 2 10 UnSpc
I2C2-SDA-CPU
Text Label -1850 -6050 0    10   ~ 0
KEYIRQ_UPPER
Text Label -8850 -250 3    10   ~ 0
N$1_UPPER
Text Label -8850 -150 2    10   ~ 0
N$1_UPPER
Text Label -8850 -850 3    10   ~ 0
N$2_UPPER
Text Label -8850 -750 2    10   ~ 0
N$2_UPPER
Text Label -8850 -1450 3    10   ~ 0
N$3_UPPER
Text Label -8850 -1350 2    10   ~ 0
N$3_UPPER
Text Label -8850 -2050 3    10   ~ 0
N$4_UPPER
Text Label -8850 -1950 2    10   ~ 0
N$4_UPPER
Text Label -7850 -250 0    10   ~ 0
N$9_UPPER
Text Label -7950 -550 3    10   ~ 0
N$9_UPPER
Text Label -7850 -850 0    10   ~ 0
N$10_UPPER
Text Label -7950 -1150 3    10   ~ 0
N$10_UPPER
Text Label -7950 -1750 3    10   ~ 0
N$11_UPPER
Text Label -7850 -1450 0    10   ~ 0
N$11_UPPER
Text Label -7950 -2350 3    10   ~ 0
N$12_UPPER
Text Label -7850 -2050 0    10   ~ 0
N$12_UPPER
Text Label -3250 -6250 2    10   ~ 0
RESET_UPPER
Text Label -8450 -150 0    10   ~ 0
ROW0_UPPER
Text Label -7350 -250 3    10   ~ 0
ROW0_UPPER
Text Label -6750 -250 3    10   ~ 0
ROW0_UPPER
Text Label -6150 -250 3    10   ~ 0
ROW0_UPPER
Text Label -5550 -250 3    10   ~ 0
ROW0_UPPER
Text Label -4950 -250 3    10   ~ 0
ROW0_UPPER
Text Label -4350 -250 3    10   ~ 0
ROW0_UPPER
Text Label -3750 -250 3    10   ~ 0
ROW0_UPPER
Text Label -3150 -250 3    10   ~ 0
ROW0_UPPER
Text Label -1850 -5450 0    10   ~ 0
ROW0_UPPER
Text Label -8250 -250 2    10   ~ 0
ROW0_UPPER
Text Label -8450 -750 0    10   ~ 0
ROW1_UPPER
Text Label -7350 -850 3    10   ~ 0
ROW1_UPPER
Text Label -6750 -850 3    10   ~ 0
ROW1_UPPER
Text Label -6150 -850 3    10   ~ 0
ROW1_UPPER
Text Label -5550 -850 3    10   ~ 0
ROW1_UPPER
Text Label -4950 -850 3    10   ~ 0
ROW1_UPPER
Text Label -4350 -850 3    10   ~ 0
ROW1_UPPER
Text Label -3750 -850 3    10   ~ 0
ROW1_UPPER
Text Label -3150 -850 3    10   ~ 0
ROW1_UPPER
Text Label -1850 -5250 0    10   ~ 0
ROW1_UPPER
Text Label -8250 -850 2    10   ~ 0
ROW1_UPPER
Text Label -3150 -1450 3    10   ~ 0
ROW2_UPPER
Text Label -8450 -1350 0    10   ~ 0
ROW2_UPPER
Text Label -7350 -1450 3    10   ~ 0
ROW2_UPPER
Text Label -6750 -1450 3    10   ~ 0
ROW2_UPPER
Text Label -6150 -1450 3    10   ~ 0
ROW2_UPPER
Text Label -5550 -1450 3    10   ~ 0
ROW2_UPPER
Text Label -4950 -1450 3    10   ~ 0
ROW2_UPPER
Text Label -4350 -1450 3    10   ~ 0
ROW2_UPPER
Text Label -3750 -1450 3    10   ~ 0
ROW2_UPPER
Text Label -1850 -5050 0    10   ~ 0
ROW2_UPPER
Text Label -8250 -1450 2    10   ~ 0
ROW2_UPPER
Text Label -8450 -1950 0    10   ~ 0
ROW3_UPPER
Text Label -7350 -2050 3    10   ~ 0
ROW3_UPPER
Text Label -1850 -4850 0    10   ~ 0
ROW3_UPPER
Text Label -8250 -2050 2    10   ~ 0
ROW3_UPPER
Text Label -1850 -4650 0    10   ~ 0
ROW4_UPPER
Text Label -3150 -3050 3    10   ~ 0
ROW5_UPPER
Text Label -1850 -4450 0    10   ~ 0
ROW5_UPPER
Text Label -3750 -3050 3    10   ~ 0
ROW5_UPPER
Text Label -3750 -3450 3    10   ~ 0
ROW6_UPPER
Text Label -3150 -3450 3    10   ~ 0
ROW6_UPPER
Text Label -1850 -4250 0    10   ~ 0
ROW6_UPPER
Text Label -3750 -3850 3    10   ~ 0
ROW7_UPPER
Text Label -1850 -4050 0    10   ~ 0
ROW7_UPPER
Text Label -3150 -3850 3    10   ~ 0
ROW7_UPPER
$EndSCHEMATC
