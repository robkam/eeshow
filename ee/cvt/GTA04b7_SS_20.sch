EESchema Schematic File Version 2
LIBS:GTA04b7
EELAYER 25 0
EELAYER END
$Descr A3 16535 11700
encoding utf-8
Sheet 21 39
Title "GTA04b7.sch"
Date "17 JUL 2016"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes -700 -650 0    200   ~ 0
B2B to upper board
Text Notes -7200 350 0    70   ~ 0
(c) 2014 Golden Delicious Computers GmbH&Co. KG. Licensed under CC-BY-SA.
Text Notes 2700 150 0    70   ~ 0
V2b
Text Notes -4000 -5950 0    70   ~ 0
Pin assignment musr be optimized for final component placement
Text Notes -4000 -5850 0    70   ~ 0
we might have to switch to 80 or 100 pin connectors
Text Notes -11600 -6550 0    70   ~ 0
ca. 130 signals (to be counted exactly after definition of upper/lower split)
$Comp
L DINA4_L #FRAME2
U 1 1 578D0019
P -7300 -450
	1    -7300 450 
	1    0    0    -1
$EndComp
$Comp
L DINA4_L #FRAME2
U 2 1 578D007D
P -900 -450
F 0 "#FRAME2" H -850 500 100  0000 L BNN
F 1 "Date:" H -850 500 100  0000 L BNN
F 2 "" H -900 450 60 0001 C CNN
F 3 "" H -900 450 60 0001 C CNN
F 0 "#FRAME2" H -400 499 100  0000 L BNN
F 1 "16 Jul 2016 23:57:11" H -400 499 100  0000 L BNN
F 2 "" H -900 450 60 0001 C CNN
F 3 "" H -900 450 60 0001 C CNN
F 0 "#FRAME2" H 1950 499 100  0000 L BNN
F 1 "Sheet:" H 1950 499 100  0000 L BNN
F 2 "" H -900 450 60 0001 C CNN
F 3 "" H -900 450 60 0001 C CNN
F 0 "#FRAME2" H 2500 499 100  0000 L BNN
F 1 "20/38" H 2500 499 100  0000 L BNN
F 2 "" H -900 450 60 0001 C CNN
F 3 "" H -900 450 60 0001 C CNN
F 0 "#FRAME2" H 2600 899 100  0000 L BNN
F 1 "REV:" H 2600 899 100  0000 L BNN
F 2 "" H -900 450 60 0001 C CNN
F 3 "" H -900 450 60 0001 C CNN
F 0 "#FRAME2" H -850 1200 100  0000 L BNN
F 1 "TITLE:" H -850 1200 100  0000 L BNN
F 2 "" H -900 450 60 0001 C CNN
F 3 "" H -900 450 60 0001 C CNN
F 0 "#FRAME2" H -850 900 100  0000 L BNN
F 1 "Document Number:" H -850 900 100  0000 L BNN
F 2 "" H -900 450 60 0001 C CNN
F 3 "" H -900 450 60 0001 C CNN
F 0 "#FRAME2" H -200 1200 100  0000 L BNN
F 1 "GTA04b7" H -200 1200 100  0000 L BNN
F 2 "" H -900 450 60 0001 C CNN
F 3 "" H -900 450 60 0001 C CNN
	2    -900 450 
	1    0    0    -1
$EndComp
$Comp
L DF40C-60DS-0.4V P2001
U 1 1 578D0145
P -4600 -3850
F 0 "P2001" H -4799 -2410 70  0000 L BNN
F 1 "DF40C-60DS-0.4V" H -4780 -5660 70  0000 L BNN
F 2 "DF40C-60DS-0.4V" H -4780 -5660 70  0000 L BNN
	1    -4600 -3850
	1    0    0    -1
$EndComp
$Comp
L DF40C-60DS-0.4V P2002
U 1 1 578D020D
P 200 -3850
F 0 "P2002" H 1 -2410 70  0000 L BNN
F 1 "DF40C-60DS-0.4V" H 20 -5660 70  0000 L BNN
F 2 "DF40C-60DS-0.4V" H 20 -5660 70  0000 L BNN
	1    200  -3850
	1    0    0    -1
$EndComp
$Comp
L GND #U$89
U 1 1 578D0271
P -2200 1950
F 0 "#U$89" H -2290 -2050 70  0000 L BNN
F 1 "GND" H -2290 -2050 70  0000 L BNN
F 2 "" H -2200 -1950 60 0001 C CNN
F 3 "" H -2200 -1950 60 0001 C CNN
	1    -2200 -1950
	1    0    0    -1
$EndComp
$Comp
L GND #U$90
U 1 1 578D02D5
P -2200 4950
F 0 "#U$90" H -2290 -5050 70  0000 L BNN
F 1 "GND" H -2290 -5050 70  0000 L BNN
F 2 "" H -2200 -4950 60 0001 C CNN
F 3 "" H -2200 -4950 60 0001 C CNN
	1    -2200 -4950
	1    0    0    -1
$EndComp
Wire Wire Line
	-8000 -750 -7500 -750
Text Label -8060 -701 2    65   ~ 0
1V8/19.1D
Wire Wire Line
	-8000 -850 -7500 -850
Text Label -8060 -801 2    65   ~ 0
2V5/13.3B
Wire Wire Line
	-8000 50 -7500 50
Text Label -8060 99 2    65   ~ 0
2V7/8.4C
Wire Wire Line
	-10200 -1350 -9700 -1350
Text Label -10260 -1301 2    65   ~ 0
3G-WOE/6.3B
Wire Wire Line
	-8000 -950 -7500 -950
Text Label -8060 -901 2    65   ~ 0
3V3/19.1D
Wire Wire Line
	-8000 -5050 -7500 -5050
Text Label -8060 -5001 2    65   ~ 0
32KHZ/9.4A
Wire Wire Line
	-8000 -3850 -7500 -3850
Text Label -8060 -3801 2    65   ~ 0
ACCEL-INT/10.4C
Wire Wire Line
	-10200 -3250 -9700 -3250
Text Label -10260 -3201 2    65   ~ 0
ADC$114/8.1C
Wire Wire Line
	-10200 -3150 -9700 -3150
Text Label -10260 -3101 2    65   ~ 0
ADC1/8.4C
Wire Wire Line
	-10200 -3050 -9700 -3050
Text Label -10260 -3001 2    65   ~ 0
ADC2/8.4C
Wire Wire Line
	-10200 -4250 -9700 -4250
Text Label -10260 -4201 2    65   ~ 0
BATTEMP/5.4A
Wire Wire Line
	-10200 -5050 -9700 -5050
Text Label -10260 -5001 2    65   ~ 0
CAM1-GPIO/1.3B
Wire Wire Line
	-10200 -4950 -9700 -4950
Text Label -10260 -4901 2    65   ~ 0
CAM2-GPIO/1.3C
Wire Wire Line
	-10200 -4650 -9700 -4650
Text Label -10260 -4601 2    65   ~ 0
CHG_IND/3.1B
Wire Wire Line
	-8000 -2950 -7500 -2950
Text Label -8060 -2901 2    65   ~ 0
ECI-ADC/13.3C
Wire Wire Line
	-10200 -1750 -9700 -1750
Text Label -10260 -1701 2    65   ~ 0
EMERG_OFF/6.3B
Wire Wire Line
	-4100 -2350 -4000 -2350
Wire Wire Line
	-5100 -2350 -4100 -2350
Wire Wire Line
	-300 -2350 700 -2350
Wire Wire Line
	-4100 -2350 -2200 -2350
Wire Wire Line
	-2200 -2350 -300 -2350
Wire Wire Line
	-2200 -2050 -2200 -2350
Connection ~ -4100 -2350
Connection ~ -300 -2350
Connection ~ -2200 -2350
Wire Wire Line
	700 -5050 -300 -5050
Wire Wire Line
	-300 -5050 -2200 -5050
Wire Wire Line
	-2200 -5050 -4100 -5050
Wire Wire Line
	-4100 -5050 -5100 -5050
Connection ~ -300 -5050
Connection ~ -4100 -5050
Connection ~ -2200 -5050
Wire Wire Line
	-8000 -4350 -7500 -4350
Text Label -8060 -4301 2    65   ~ 0
GPIN115/10.3B
Wire Wire Line
	-8000 -4150 -7500 -4150
Text Label -8060 -4101 2    65   ~ 0
GPIO$51/10.3B
Wire Wire Line
	-10200 -1250 -9700 -1250
Text Label -10260 -1201 2    65   ~ 0
GPIO$52/8.4A
Wire Wire Line
	-8000 -4050 -7500 -4050
Text Label -8060 -4001 2    65   ~ 0
GPIO$55/10.4A
Wire Wire Line
	-8000 -3950 -7500 -3950
Text Label -8060 -3901 2    65   ~ 0
GPIO$56/10.4A
Wire Wire Line
	-8000 -2650 -7500 -2650
Text Label -8060 -2601 2    65   ~ 0
GPIO$60/12.2D
Wire Wire Line
	-10200 -3650 -9700 -3650
Text Label -10260 -3601 2    65   ~ 0
GPIO$70/8.3B
Wire Wire Line
	-10200 -3550 -9700 -3550
Text Label -10260 -3501 2    65   ~ 0
GPIO$110/8.1D
Wire Wire Line
	-8000 -2050 -7500 -2050
Text Label -8060 -2001 2    65   ~ 0
GPIO-BATT-LID/15.1B
Wire Wire Line
	-8000 -6050 -7500 -6050
Text Label -8060 -6001 2    65   ~ 0
GPIO-BT-EN/9.1C
Wire Wire Line
	-8000 150 -7500 150
Text Label -8060 199 2    65   ~ 0
GPIO-CAM-COVER/15.1B
Wire Wire Line
	-10200 -2950 -9700 -2950
Text Label -10260 -2901 2    65   ~ 0
GPIO-COMPARATOR/8.4D
Wire Wire Line
	-8000 -3050 -7500 -3050
Text Label -8060 -3001 2    65   ~ 0
GPIO-ECI0/13.3C
Wire Wire Line
	-8000 -3150 -7500 -3150
Text Label -8060 -3101 2    65   ~ 0
GPIO-ECI1/13.3B
Wire Wire Line
	-10200 -4150 -9700 -4150
Text Label -10260 -4101 2    65   ~ 0
GPIO-EN-MODEM/4.1A
Wire Wire Line
	-8000 -2250 -7500 -2250
Text Label -8060 -2201 2    65   ~ 0
GPIO-FLASH-EN/15.1A
Wire Wire Line
	-8000 -2150 -7500 -2150
Text Label -8060 -2101 2    65   ~ 0
GPIO-FLASH-INT/15.1A
Wire Wire Line
	-8000 -2350 -7500 -2350
Text Label -8060 -2301 2    65   ~ 0
GPIO-FLASH-STR/15.1A
Wire Wire Line
	-8000 -4950 -7500 -4950
Text Label -8060 -4901 2    65   ~ 0
GPIO-FM-EN/9.3A
Wire Wire Line
	-8000 -4850 -7500 -4850
Text Label -8060 -4801 2    65   ~ 0
GPIO-FMIRQ/9.3A
Wire Wire Line
	-8000 -2450 -7500 -2450
Text Label -8060 -2401 2    65   ~ 0
GPIO-HF-EN/14.1B
Wire Wire Line
	-8000 -2750 -7500 -2750
Text Label -8060 -2701 2    65   ~ 0
GPIO-HP-EN/12.1B
Wire Wire Line
	-10200 -1850 -9700 -1850
Text Label -10260 -1801 2    65   ~ 0
GPIO-MODEM_EMERG/6.3A
Wire Wire Line
	-10200 -1950 -9700 -1950
Text Label -10260 -1901 2    65   ~ 0
GPIO-MODEM_IGT/6.3A
Wire Wire Line
	-10200 -5250 -9700 -5250
Text Label -10260 -5201 2    65   ~ 0
GPIO-VOL+/1.2B
Wire Wire Line
	-10200 -5150 -9700 -5150
Text Label -10260 -5101 2    65   ~ 0
GPIO-VOL-/1.2B
Wire Wire Line
	-10200 -1150 -9700 -1150
Text Label -10260 -1101 2    65   ~ 0
GPIO-WLAN-EN/9.1A
Wire Wire Line
	-8000 -6150 -7500 -6150
Text Label -8060 -6101 2    65   ~ 0
GPIO-WLAN-IRQ/9.1A
Wire Wire Line
	-8000 -4250 -7500 -4250
Text Label -8060 -4201 2    65   ~ 0
GPIO56/10.3A
Wire Wire Line
	-8000 -2550 -7500 -2550
Text Label -8060 -2501 2    65   ~ 0
GPIO177/12.4C
Wire Wire Line
	-10200 -3750 -9700 -3750
Text Label -10260 -3701 2    65   ~ 0
HDQ/5.2A
Wire Wire Line
	-10200 -3950 -9700 -3950
Text Label -10260 -3901 2    65   ~ 0
I2C2-SCL/15.1A
Wire Wire Line
	-10200 -4050 -9700 -4050
Text Label -10260 -4001 2    65   ~ 0
I2C2-SDA/15.1A
Wire Wire Line
	-10200 -4750 -9700 -4750
Text Label -10260 -4701 2    65   ~ 0
I2C3-SCL/10.4C
Wire Wire Line
	-10200 -4850 -9700 -4850
Text Label -10260 -4801 2    65   ~ 0
I2C3-SDA/10.4C
Wire Wire Line
	-10200 -3850 -9700 -3850
Text Label -10260 -3801 2    65   ~ 0
INA231-INT/4.4C
Wire Wire Line
	-10200 -1550 -9700 -1550
Text Label -10260 -1501 2    65   ~ 0
LC_IND/6.3B
Wire Wire Line
	-10200 -5450 -9700 -5450
Text Label -10260 -5401 2    65   ~ 0
LOCK-GPIO/1.2A
Wire Wire Line
	-8000 -4650 -7500 -4650
Text Label -8060 -4601 2    65   ~ 0
MCBSP2-CLK/9.3A
Wire Wire Line
	-8000 -4550 -7500 -4550
Text Label -8060 -4501 2    65   ~ 0
MCBSP2-DR/9.3A
Wire Wire Line
	-8000 -4450 -7500 -4450
Text Label -8060 -4401 2    65   ~ 0
MCBSP2-DX/9.3A
Wire Wire Line
	-8000 -4750 -7500 -4750
Text Label -8060 -4701 2    65   ~ 0
MCBSP2-FCK/9.3A
Wire Wire Line
	-8000 -5450 -7500 -5450
Text Label -8060 -5401 2    65   ~ 0
MCBSP3-CLK/9.1D
Wire Wire Line
	-8000 -5350 -7500 -5350
Text Label -8060 -5301 2    65   ~ 0
MCBSP3-DR/9.1D
Wire Wire Line
	-8000 -5250 -7500 -5250
Text Label -8060 -5201 2    65   ~ 0
MCBSP3-DX/9.1D
Wire Wire Line
	-8000 -5550 -7500 -5550
Text Label -8060 -5501 2    65   ~ 0
MCBSP3-FCK/9.1D
Wire Wire Line
	-10200 -2650 -9700 -2650
Text Label -10260 -2601 2    65   ~ 0
MCBSP4-CLKX/6.2A
Wire Wire Line
	-10200 -2850 -9700 -2850
Text Label -10260 -2801 2    65   ~ 0
MCBSP4-DR/6.2A
Wire Wire Line
	-10200 -2750 -9700 -2750
Text Label -10260 -2701 2    65   ~ 0
MCBSP4-DX/6.2A
Wire Wire Line
	-10200 -2550 -9700 -2550
Text Label -10260 -2501 2    65   ~ 0
MCBSP4-FSX/6.2A
Wire Wire Line
	-10200 -1050 -9700 -1050
Text Label -10260 -1001 2    65   ~ 0
MMC3-CLK/9.1A
Wire Wire Line
	-10200 -950 -9700 -950
Text Label -10260 -901 2    65   ~ 0
MMC3-CMD/9.1A
Wire Wire Line
	-10200 -850 -9700 -850
Text Label -10260 -801 2    65   ~ 0
MMC3-DATA0/9.1A
Wire Wire Line
	-8000 -6450 -7500 -6450
Text Label -8060 -6401 2    65   ~ 0
MMC3-DATA1/9.1A
Wire Wire Line
	-8000 -6350 -7500 -6350
Text Label -8060 -6301 2    65   ~ 0
MMC3-DATA2/9.1A
Wire Wire Line
	-8000 -6250 -7500 -6250
Text Label -8060 -6201 2    65   ~ 0
MMC3-DATA3/9.1A
Wire Wire Line
	-10200 -3450 -9700 -3450
Text Label -10260 -3401 2    65   ~ 0
N$19/8.2D
Wire Wire Line
	-8000 250 -7500 250
Text Label -8060 299 2    65   ~ 0
N$38
Wire Wire Line
	-8000 -3750 -7500 -3750
Text Label -8060 -3701 2    65   ~ 0
N$129/11.2A
Wire Wire Line
	-10200 -4550 -9700 -4550
Text Label -10260 -4501 2    65   ~ 0
N$131/3.1C
Wire Wire Line
	-10200 -4450 -9700 -4450
Text Label -10260 -4401 2    65   ~ 0
N$141/3.1C
Wire Wire Line
	-10200 -4350 -9700 -4350
Text Label -10260 -4301 2    65   ~ 0
N$143/3.1C
Wire Wire Line
	-8000 -3650 -7500 -3650
Text Label -8060 -3601 2    65   ~ 0
N$148/11.1A
Wire Wire Line
	-8000 -3550 -7500 -3550
Text Label -8060 -3501 2    65   ~ 0
N$149/11.1A
Wire Wire Line
	-8000 -3450 -7500 -3450
Text Label -8060 -3401 2    65   ~ 0
N$150/11.1A
Wire Wire Line
	-8000 -3350 -7500 -3350
Text Label -8060 -3301 2    65   ~ 0
N$153/11.1A
Wire Wire Line
	-8000 -3250 -7500 -3250
Text Label -8060 -3201 2    65   ~ 0
N$154/11.1A
Wire Wire Line
	-10200 -3350 -9700 -3350
Text Label -10260 -3301 2    65   ~ 0
N$229/8.3C
Wire Wire Line
	-8000 -450 -7500 -450
Text Label -8060 -401 2    65   ~ 0
OTG-D+/3.1B
Wire Wire Line
	-8000 -550 -7500 -550
Text Label -8060 -501 2    65   ~ 0
OTG-D-/3.1B
Wire Wire Line
	-8000 -350 -7500 -350
Text Label -8060 -301 2    65   ~ 0
OTG-ID/2.2B
Wire Wire Line
	-10200 -5350 -9700 -5350
Text Label -10260 -5301 2    65   ~ 0
POWERON/1.3A
Wire Wire Line
	-10200 -1650 -9700 -1650
Text Label -10260 -1601 2    65   ~ 0
PWR_IND/6.3B
Wire Wire Line
	-10200 -2050 -9700 -2050
Text Label -10260 -2001 2    65   ~ 0
RING/6.2C
Wire Wire Line
	-8000 -1750 -7500 -1750
Text Label -8060 -1701 2    65   ~ 0
SD-CD/15.2B
Wire Wire Line
	-8000 -1850 -7500 -1850
Text Label -8060 -1801 2    65   ~ 0
SD-CLK/15.2A
Wire Wire Line
	-8000 -1950 -7500 -1950
Text Label -8060 -1901 2    65   ~ 0
SD-CMD/15.2A
Wire Wire Line
	-8000 -1550 -7500 -1550
Text Label -8060 -1501 2    65   ~ 0
SD-DAT0/15.2B
Wire Wire Line
	-8000 -1450 -7500 -1450
Text Label -8060 -1401 2    65   ~ 0
SD-DAT1/15.2B
Wire Wire Line
	-8000 -1350 -7500 -1350
Text Label -8060 -1301 2    65   ~ 0
SD-DAT2/15.2A
Wire Wire Line
	-8000 -1250 -7500 -1250
Text Label -8060 -1201 2    65   ~ 0
SD-DAT3/15.2A
Wire Wire Line
	-8000 -1650 -7500 -1650
Text Label -8060 -1601 2    65   ~ 0
SD-VDD/15.2A
Wire Wire Line
	-10200 -1450 -9700 -1450
Text Label -10260 -1401 2    65   ~ 0
STATUS/6.3B
Wire Wire Line
	-8000 -5150 -7500 -5150
Text Label -8060 -5101 2    65   ~ 0
SYSCLK/9.3C
Wire Wire Line
	-8000 -5750 -7500 -5750
Text Label -8060 -5701 2    65   ~ 0
UART1-CTS/9.1D
Wire Wire Line
	-8000 -5850 -7500 -5850
Text Label -8060 -5801 2    65   ~ 0
UART1-RTS/9.1C
Wire Wire Line
	-8000 -5950 -7500 -5950
Text Label -8060 -5901 2    65   ~ 0
UART1-RX/9.1C
Wire Wire Line
	-8000 -5650 -7500 -5650
Text Label -8060 -5601 2    65   ~ 0
UART1-TX/9.1D
Wire Wire Line
	-10200 -2350 -9700 -2350
Text Label -10260 -2301 2    65   ~ 0
UART?-CTS/6.2C
Wire Wire Line
	-10200 -2450 -9700 -2450
Text Label -10260 -2401 2    65   ~ 0
UART?-RTS/6.2C
Wire Wire Line
	-10200 -2250 -9700 -2250
Text Label -10260 -2201 2    65   ~ 0
UART?-RX/6.2C
Wire Wire Line
	-10200 -2150 -9700 -2150
Text Label -10260 -2101 2    65   ~ 0
UART?-TX/6.2C
Wire Wire Line
	-8000 -150 -7500 -150
Text Label -8060 -101 2    65   ~ 0
USB-WWAN-D+/18.3A
Wire Wire Line
	-8000 -50 -7500 -50
Text Label -8060 -1 2    65   ~ 0
USB-WWAN-D-/18.3A
Wire Wire Line
	-8000 -650 -7500 -650
Text Label -8060 -601 2    65   ~ 0
VBUS/18.1C
Wire Wire Line
	-8000 -250 -7500 -250
Text Label -8060 -201 2    65   ~ 0
VBUS-MODEM/6.3B
Wire Wire Line
	-8000 -1150 -7500 -1150
Text Label -8060 -1101 2    65   ~ 0
VIB+/15.1D
Wire Wire Line
	-8000 -1050 -7500 -1050
Text Label -8060 -1001 2    65   ~ 0
VIB-/15.1D
Wire Wire Line
	-8000 -2850 -7500 -2850
Text Label -8060 -2801 2    65   ~ 0
VMMC2/12.1A
Wire Wire Line
	-5100 -5150 -4100 -5150
Wire Wire Line
	-4100 -5150 -300 -5150
Wire Wire Line
	-300 -5150 700 -5150
Wire Wire Line
	-5100 -5150 -5600 -5150
Connection ~ -4100 -5150
Connection ~ -300 -5150
Connection ~ -5100 -5150
Text Label -5660 -5101 2    65   ~ 0
VSYS/19.3B
Wire Wire Line
	-5100 -2250 -4100 -2250
Wire Wire Line
	-4100 -2250 -300 -2250
Wire Wire Line
	-300 -2250 700 -2250
Wire Wire Line
	-5100 -2250 -5500 -2250
Connection ~ -4100 -2250
Connection ~ -300 -2250
Connection ~ -5100 -2250
Text Label -5560 -2201 2    65   ~ 0
VSYS/19.3B
Text GLabel -2200 -2050 1 10 UnSpc
GND
Text GLabel -4100 -2350 2 10 UnSpc
GND
Text GLabel -5100 -2350 0 10 UnSpc
GND
Text GLabel -300 -2350 0 10 UnSpc
GND
Text GLabel 700 -2350 2 10 UnSpc
GND
Text GLabel 700 -5050 2 10 UnSpc
GND
Text GLabel -300 -5050 0 10 UnSpc
GND
Text GLabel -4100 -5050 2 10 UnSpc
GND
Text GLabel -5100 -5050 0 10 UnSpc
GND
Text GLabel -2200 -5050 1 10 UnSpc
GND
Text GLabel -5100 -5150 0 10 UnSpc
VSYS
Text GLabel -4100 -5150 2 10 UnSpc
VSYS
Text GLabel -300 -5150 0 10 UnSpc
VSYS
Text GLabel 700 -5150 2 10 UnSpc
VSYS
Text GLabel -5100 -2250 0 10 UnSpc
VSYS
Text GLabel -4100 -2250 2 10 UnSpc
VSYS
Text GLabel -300 -2250 0 10 UnSpc
VSYS
Text GLabel 700 -2250 2 10 UnSpc
VSYS
$EndSCHEMATC
