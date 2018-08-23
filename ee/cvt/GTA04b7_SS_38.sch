EESchema Schematic File Version 2
LIBS:GTA04b7
EELAYER 25 0
EELAYER END
$Descr A3 16535 11700
encoding utf-8
Sheet 39 39
Title "GTA04b7.sch"
Date "17 JUL 2016"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes -2600 -600 0    200   ~ 0
Table of Contents
Text Notes -9100 400 0    70   ~ 0
(c) 2015 Golden Delicious Computers GmbH&Co. KG. Licensed under CC-BY-SA.
Text Notes 800 200 0    70   ~ 0
V2b
Text Notes -8700 -6200 0    110   ~ 0
1. Buttons\n2. OTG port\n3. Charger / OTG booster\n4. Modem Power\n5. Fuel Gauge\n6. 3G/4G Modem + SIM\n7. Dual SIM Switch\n8. Antenna Connections\n9. WLAN + Bluetooth + FM\n10. Sensors\n11. Audio: Codec\n12. Audio: Headset & Microphone\n13. ECI (remote control)\n14. Audio: Handsfree\n15. Misc on Lower Board\n16. RFID/NFC Reader\n17. NFC Controller\n18. Hackerbus\n19. Infrared\n20. B2B connectors lower to upper\n21. ~SD and Flash Breakout Board\n22. B2B connectors upper to lower\n23. Keypad\n24. Display: Peripherals\n25. Display: Panel & Power\n26. CPU + PoP RAM/NAND (placeholder)\n27. eMMC (placeholder)\n28. PMU+Codec (placeholder)\n29. BB-XM: twl4030 dummy\n30. Camera\n31. Control LEDs\n32. Fancy LEDs\n33. BB-XM: Connector to BB\n34. BB-XM: Adapter for CPU\n35. BB-XM: Adapter for Display\n36. BB-XM: Adapter for Camera\n37. Components not soldered\n38. Table of Contents
$Comp
L DINA4_L #FRAME38
U 1 1 578AF955
P -9200 -500
	1    -9200 500 
	1    0    0    -1
$EndComp
$Comp
L DINA4_L #FRAME38
U 2 1 578E6905
P -2800 -500
F 0 "#FRAME38" H -2750 550 100  0000 L BNN
F 1 "Date:" H -2750 550 100  0000 L BNN
F 2 "" H -2800 500 60 0001 C CNN
F 3 "" H -2800 500 60 0001 C CNN
F 0 "#FRAME38" H -2300 549 100  0000 L BNN
F 1 "16 Jul 2016 23:57:11" H -2300 549 100  0000 L BNN
F 2 "" H -2800 500 60 0001 C CNN
F 3 "" H -2800 500 60 0001 C CNN
F 0 "#FRAME38" H 50 549 100  0000 L BNN
F 1 "Sheet:" H 50 549 100  0000 L BNN
F 2 "" H -2800 500 60 0001 C CNN
F 3 "" H -2800 500 60 0001 C CNN
F 0 "#FRAME38" H 600 549 100  0000 L BNN
F 1 "38/38" H 600 549 100  0000 L BNN
F 2 "" H -2800 500 60 0001 C CNN
F 3 "" H -2800 500 60 0001 C CNN
F 0 "#FRAME38" H 700 949 100  0000 L BNN
F 1 "REV:" H 700 949 100  0000 L BNN
F 2 "" H -2800 500 60 0001 C CNN
F 3 "" H -2800 500 60 0001 C CNN
F 0 "#FRAME38" H -2750 1250 100  0000 L BNN
F 1 "TITLE:" H -2750 1250 100  0000 L BNN
F 2 "" H -2800 500 60 0001 C CNN
F 3 "" H -2800 500 60 0001 C CNN
F 0 "#FRAME38" H -2750 950 100  0000 L BNN
F 1 "Document Number:" H -2750 950 100  0000 L BNN
F 2 "" H -2800 500 60 0001 C CNN
F 3 "" H -2800 500 60 0001 C CNN
F 0 "#FRAME38" H -2100 1250 100  0000 L BNN
F 1 "GTA04b7" H -2100 1250 100  0000 L BNN
F 2 "" H -2800 500 60 0001 C CNN
F 3 "" H -2800 500 60 0001 C CNN
	2    -2800 500 
	1    0    0    -1
$EndComp
Wire Wire Line
	-9532 -6611 -9632 -6611
Text Label -9632 -6611 0    65   ~ 0
COL0_UPPER
Wire Wire Line
	-9532 -6511 -9632 -6511
Text Label -9632 -6511 0    65   ~ 0
COL2_UPPER
Wire Wire Line
	-9532 -6411 -9632 -6411
Text Label -9632 -6411 0    65   ~ 0
COL3_UPPER
Wire Wire Line
	-9532 -6311 -9632 -6311
Text Label -9632 -6311 0    65   ~ 0
COL4_UPPER
Wire Wire Line
	-9532 -6211 -9632 -6211
Text Label -9632 -6211 0    65   ~ 0
COL4_UPPER
Wire Wire Line
	-9532 -6111 -9632 -6111
Text Label -9632 -6111 0    65   ~ 0
GND12
$EndSCHEMATC
