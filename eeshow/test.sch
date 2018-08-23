EESchema Schematic File Version 2
LIBS:test
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title "title"
Date "date"
Rev "revision"
Comp "company"
Comment1 "hello"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 1450 1300 0    100  ~ 0
Text, Right, Normal, 100 mil
Wire Notes Line
	1400 1300 1250 1300
Wire Notes Line
	1450 1350 1450 1500
Text Notes 3750 1500 2    100  ~ 20
Text, Left, Bold, 100 mil
Wire Notes Line
	3800 1500 3950 1500
Wire Notes Line
	3750 1550 3750 1700
Text Notes 1050 1550 1    100  Italic 0
Up, Italic
Wire Notes Line
	1100 1550 1250 1550
Wire Notes Line
	1050 1600 1050 1750
Text Notes 4300 700  3    100  Italic 20
Down, Bold italic
Wire Notes Line
	4350 700  4500 700 
Wire Notes Line
	4300 650  4300 500 
Wire Wire Line
	4800 1100 5300 1100
Wire Wire Line
	5050 850  5050 1350
Wire Wire Line
	5550 850  5550 1350
Wire Wire Line
	6050 1100 5550 1100
Connection ~ 5550 1100
Wire Bus Line
	6750 600  6750 1400
Entry Bus Bus
	6650 1250 6750 1350
Wire Bus Line
	6200 1250 6650 1250
Wire Wire Line
	6650 950  6200 950 
Entry Wire Line
	6650 950  6750 1050
NoConn ~ 5050 850 
NoConn ~ 5050 1350
NoConn ~ 5300 1100
NoConn ~ 4800 1100
Wire Wire Line
	1300 2400 2800 2400
Text Label 1300 2400 0    60   ~ 0
LOCAL_RIGHT_NORMAL
Wire Wire Line
	1300 2600 2800 2600
Text Label 2800 2600 2    60   ~ 12
LOCAL_LEFT_BOLD
Wire Wire Line
	3400 1800 3400 2900
Text Label 3400 2900 1    60   Italic 0
LOCAL_UP_ITALIC
Wire Wire Line
	3700 1800 3700 2900
Text Label 3700 1800 3    60   Italic 12
LOCAL_DOWN_BI
Wire Wire Line
	4650 2400 5350 2400
Wire Wire Line
	4650 2700 5350 2700
Text GLabel 4650 2400 0    60   Input ~ 0
G_R_IN
Text GLabel 5350 2700 2    60   Input ~ 0
G_L_IN
Text GLabel 4650 2700 0    60   Output ~ 0
G_R_OUT
Text GLabel 5350 2400 2    60   Output ~ 0
G_L_OUT
Wire Wire Line
	6100 2200 6100 2900
Wire Wire Line
	6400 2200 6400 2900
Text GLabel 6100 2200 1    60   BiDi ~ 0
G_UP_BI
Wire Wire Line
	4650 3000 5350 3000
Text GLabel 4650 3000 0    60   BiDi ~ 0
G_R_BIDIR
Text GLabel 5350 3000 2    60   UnSpc ~ 0
G_LEFT_PASS
Text GLabel 4650 3150 0    60   UnSpc ~ 0
G_R_TRI
Wire Wire Line
	4650 3150 4850 3150
Wire Wire Line
	4850 3150 4850 3000
Connection ~ 4850 3000
Text GLabel 6100 2900 3    60   Input ~ 0
G_DOWN_IN
Text GLabel 6400 2200 1    60   Output ~ 0
G_UP_OUT
Text GLabel 6400 2900 3    60   UnSpc ~ 0
G_DOWN_PASS
Wire Wire Line
	7250 2700 7950 2700
Wire Wire Line
	8750 2200 8750 2900
Wire Wire Line
	9050 2200 9050 2900
Wire Wire Line
	7250 3150 7450 3150
Wire Wire Line
	7450 3150 7450 3000
Connection ~ 7450 3000
Text HLabel 7250 2400 0    60   Input ~ 0
H_R_IN
Text HLabel 7250 2700 0    60   Output ~ 0
H_R_OUT
Text HLabel 7250 3000 0    60   BiDi ~ 0
H_R_BIDIR
Text HLabel 7250 3150 0    60   UnSpc ~ 0
H_R_TRI
Text HLabel 7950 2400 2    60   Output ~ 0
H_L_OUT
Text HLabel 7950 2700 2    60   Input ~ 0
H_L_IN
Wire Wire Line
	7250 2400 7950 2400
Text HLabel 7950 3000 2    60   UnSpc ~ 0
H_LEFT_PASS
Text HLabel 8750 2200 1    60   BiDi ~ 0
H_UP_BI
Text HLabel 8750 2900 3    60   Input ~ 0
H_DOWN_IN
Text HLabel 9050 2200 1    60   Output ~ 0
H_UP_OUT
Text HLabel 9050 2900 3    60   UnSpc ~ 0
H_DOWN_PASS
Wire Wire Line
	7250 3000 7950 3000
Text Notes 1300 3400 0    60   ~ 0
60 mil Text
Wire Notes Line
	1300 3150 1300 3550
Wire Notes Line
	1850 3550 1850 3150
Wire Notes Line
	1150 3300 2000 3300
Wire Notes Line
	1150 3400 2000 3400
Text GLabel 1400 3750 2    60   Output ~ 0
GLOBAL
Text GLabel 1800 3950 0    60   Output ~ 0
GLOBAL
Wire Notes Line
	1400 3550 1400 4150
Wire Notes Line
	1800 3550 1800 4150
Wire Notes Line
	1250 3700 1950 3700
Wire Notes Line
	1250 3800 1950 3800
Wire Notes Line
	1250 3900 1950 3900
Wire Notes Line
	1250 4000 1950 4000
Text HLabel 2950 3600 0    60   Input ~ 0
HIERARCHICAL
Text HLabel 2250 3750 2    60   Input ~ 0
HIERARCHICAL
Wire Notes Line
	2250 3400 2250 3950
Wire Notes Line
	2100 3700 3100 3700
Wire Notes Line
	2100 3650 3100 3650
Wire Notes Line
	2100 3550 3100 3550
Wire Notes Line
	2100 3800 3100 3800
Wire Notes Line
	2950 3400 2950 3950
$Comp
L TEST U?
U 1 1 57933A17
P 1650 5200
F 0 "U?" H 1175 5875 60  0000 C CNN
F 1 "TEST" H 1225 4525 60  0000 C CNN
F 2 "" H 1800 5400 60  0001 C CNN
F 3 "" H 1800 5400 60  0001 C CNN
	1    1650 5200
	1    0    0    -1  
$EndComp
$Comp
L TEST U?
U 1 1 57933A5F
P 4650 4750
F 0 "U?" H 4175 5425 60  0000 C CNN
F 1 "TEST-BOLD-IT" H 4225 4075 60  0000 C CIB
F 2 "" H 4800 4950 60  0001 C CNN
F 3 "" H 4800 4950 60  0001 C CNN
	1    4650 4750
	0    1    1    0   
$EndComp
$Comp
L TEST U?
U 1 1 57933A97
P 7450 4900
F 0 "U?" H 6975 5575 60  0000 C CNN
F 1 "TEST-BOLD" H 7025 4225 60  0000 C CNB
F 2 "" H 7600 5100 60  0001 C CNN
F 3 "" H 7600 5100 60  0001 C CNN
	1    7450 4900
	-1   0    0    1   
$EndComp
$Comp
L TEST U?
U 1 1 57933ACD
P 9750 5250
F 0 "U?" H 9275 5925 60  0000 C CNN
F 1 "TEST" H 9325 4575 60  0000 C CNN
F 2 "" H 9900 5450 60  0001 C CNN
F 3 "" H 9900 5450 60  0001 C CNN
	1    9750 5250
	0    -1   -1   0   
$EndComp
$Comp
L TEST U?
U 1 1 57933B03
P 2450 6950
F 0 "U?" H 1975 7625 60  0000 C CNN
F 1 "TEST" H 2025 6275 60  0000 C CNN
F 2 "" H 2600 7150 60  0001 C CNN
F 3 "" H 2600 7150 60  0001 C CNN
	1    2450 6950
	1    0    0    1   
$EndComp
$Comp
L TEST U?
U 1 1 57933B47
P 5950 6900
F 0 "U?" H 5475 7575 60  0000 C CNN
F 1 "TEST-IT" H 5525 6225 60  0000 C CIN
F 2 "" H 6100 7100 60  0001 C CNN
F 3 "" H 6100 7100 60  0001 C CNN
	1    5950 6900
	-1   0    0    -1  
$EndComp
Text Notes 900  7000 0    60   ~ 0
X-flip
Text Notes 4250 6900 0    60   ~ 0
Y-flip
$Comp
L TEST U?
U 1 1 57933BAB
P 10200 2400
F 0 "U?" H 9725 3075 60  0000 C CNN
F 1 "TEST" H 9775 1725 60  0000 C CNN
F 2 "" H 10350 2600 60  0001 C CNN
F 3 "" H 10350 2600 60  0001 C CNN
	1    10200 2400
	0    1    -1   0   
$EndComp
Text Notes 10000 850  0    60   ~ 0
R+X
$Sheet
S 7250 790  800  560 
U 579BE133
F0 "Sheet579BE132" 60
F1 "file579BE132.sch" 60
$EndSheet
$Sheet
S 7190 1790 770  400 
U 579BE13D
F0 "Sheet579BE13C" 60
F1 "file579BE13C.sch" 60
$EndSheet
Wire Bus Line
	6200 1200 6650 1200
Entry Bus Bus
	6650 1200 6750 1100
Wire Wire Line
	6650 900  6200 900 
Entry Wire Line
	6650 900  6750 800 
Wire Wire Line
	6850 750  7100 750 
Wire Wire Line
	6850 800  7100 800 
Entry Wire Line
	6750 900  6850 800 
Entry Wire Line
	6750 650  6850 750 
Entry Bus Bus
	6750 1250 6850 1150
Entry Bus Bus
	6750 1000 6850 1100
Wire Bus Line
	7100 1100 6850 1100
Wire Bus Line
	7100 1150 6850 1150
Wire Bus Line
	4750 1950 5700 1950
Wire Wire Line
	4850 1850 4850 1500
Wire Bus Line
	4950 1850 4950 1500
Wire Wire Line
	5400 1850 5400 1500
Wire Bus Line
	5500 1500 5500 1850
Wire Wire Line
	4800 2250 4800 2050
Wire Wire Line
	5300 2250 5300 2050
Wire Bus Line
	5450 2250 5450 2050
Wire Bus Line
	4900 2250 4900 2050
Entry Wire Line
	4850 1850 4950 1950
Entry Wire Line
	5200 1950 5300 2050
Entry Wire Line
	4800 2050 4900 1950
Entry Wire Line
	5300 1950 5400 1850
Entry Bus Bus
	4950 1850 5050 1950
Entry Bus Bus
	4900 2050 5000 1950
Entry Bus Bus
	5400 1950 5500 1850
Entry Bus Bus
	5350 1950 5450 2050
Text Notes 7000 6300 0    60   ~ 0
~OVER~normal
Text Notes 7000 6450 0    60   ~ 0
normal~OVER
Text Notes 7750 6450 1    60   ~ 0
abc~over~def
Text Notes 8350 6450 2    60   ~ 0
foo~over~bar
Wire Wire Line
	800  2000 1800 2000
Wire Wire Line
	800  2100 1800 2100
Text Label 800  2000 0    60   ~ 0
Norm
Text Label 800  2100 0    60   ~ 12
Bold
Text Label 1800 2000 2    60   Italic 0
Italic
Text Label 1800 2100 2    60   Italic 12
BoldItalic
Text GLabel 2300 1900 0    60   Input Italic 12
Norm
Text GLabel 2300 2100 0    60   Input Italic 0
Italic
Text GLabel 2500 1900 2    60   Input Italic 0
Bold
Text GLabel 2500 2100 2    60   Input Italic 0
BoldItalic
Wire Wire Line
	2300 1900 2500 1900
Wire Wire Line
	2300 2100 2500 2100
Text HLabel 7300 3400 0    60   Input ~ 0
Normal
Text HLabel 7600 3400 2    60   Input Italic 0
Italic
Text HLabel 7300 3500 0    60   Input ~ 12
Bold
Text HLabel 7600 3500 2    60   Input Italic 12
BoldItalic
Wire Wire Line
	7300 3400 7600 3400
Wire Wire Line
	7300 3500 7600 3500
Text Notes 8550 6450 0    60   ~ 0
trailing tilde~
Text Notes 8550 6300 0    60   ~ 0
double~~tilde
Text Notes 9400 6450 0    60   ~ 0
ending ~tilde~
Text Notes 9400 6300 0    60   ~ 0
overlined ~double~~tilde~
$EndSCHEMATC
