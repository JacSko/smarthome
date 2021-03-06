EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "SmartHouse MainBoard"
Date "2021-01-10"
Rev "1.0"
Comp ""
Comment1 "This schematic defines the main board of whole system."
Comment2 "System is run on STM32F4xx MCU as BlackPill board."
Comment3 "Board is communcating with RPi using WiFi Module"
Comment4 ""
$EndDescr
$Comp
L Connector:Screw_Terminal_01x03 J2
U 1 1 5FFB4AF7
P 1700 750
F 0 "J2" V 1664 562 50  0000 R CNN
F 1 "Power_Conn" V 1200 1000 50  0000 R CNN
F 2 "TerminalBlock:TerminalBlock_bornier-3_P5.08mm" H 1700 750 50  0001 C CNN
F 3 "~" H 1700 750 50  0001 C CNN
	1    1700 750 
	0    -1   -1   0   
$EndComp
Text GLabel 1800 950  3    50   Input ~ 0
12V
Text GLabel 1700 950  3    50   Input ~ 0
GND
Text GLabel 1600 950  3    50   Input ~ 0
5V
$Comp
L Device:C C1
U 1 1 5FFB5A7E
P 2200 950
F 0 "C1" H 2315 996 50  0000 L CNN
F 1 "22u" H 2315 905 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-7343-31_Kemet-D_Pad2.25x2.55mm_HandSolder" H 2238 800 50  0001 C CNN
F 3 "~" H 2200 950 50  0001 C CNN
	1    2200 950 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 5FFB60D9
P 3850 950
F 0 "C4" H 3965 996 50  0000 L CNN
F 1 "22u" H 3965 905 50  0000 L CNN
F 2 "Capacitor_Tantalum_SMD:CP_EIA-7343-31_Kemet-D_Pad2.25x2.55mm_HandSolder" H 3888 800 50  0001 C CNN
F 3 "~" H 3850 950 50  0001 C CNN
	1    3850 950 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5FFB631C
P 2550 950
F 0 "C2" H 2665 996 50  0000 L CNN
F 1 "470u" H 2665 905 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P2.50mm" H 2588 800 50  0001 C CNN
F 3 "~" H 2550 950 50  0001 C CNN
	1    2550 950 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5FFB6F52
P 3450 950
F 0 "C3" H 3565 996 50  0000 L CNN
F 1 "470u" H 3565 905 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P2.50mm" H 3488 800 50  0001 C CNN
F 3 "~" H 3450 950 50  0001 C CNN
	1    3450 950 
	1    0    0    -1  
$EndComp
$Comp
L Regulator_Linear:AMS1117-3.3 U4
U 1 1 5FFB77B5
P 3050 800
F 0 "U4" H 3050 1042 50  0000 C CNN
F 1 "AMS1117-3.3" H 3050 951 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223-3_TabPin2" H 3050 1000 50  0001 C CNN
F 3 "http://www.advanced-monolithic.com/pdf/ds1117.pdf" H 3150 550 50  0001 C CNN
	1    3050 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 800  2550 800 
Connection ~ 2550 800 
Wire Wire Line
	2550 800  2750 800 
Wire Wire Line
	3350 800  3450 800 
Connection ~ 3450 800 
Wire Wire Line
	3450 800  3850 800 
Wire Wire Line
	3850 1100 3450 1100
Connection ~ 2550 1100
Wire Wire Line
	2550 1100 2200 1100
Connection ~ 3050 1100
Wire Wire Line
	3050 1100 2550 1100
Connection ~ 3450 1100
Wire Wire Line
	3450 1100 3050 1100
Text GLabel 2200 800  0    50   Input ~ 0
5V
Text GLabel 3050 1100 3    50   Input ~ 0
GND
Text GLabel 3850 800  2    50   Input ~ 0
3.3V
$Comp
L Connector:Conn_01x10_Female J1
U 1 1 5FFC36B1
P 1550 2400
F 0 "J1" H 1442 1675 50  0000 C CNN
F 1 "Sensors_Conn" H 1442 1766 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 1550 2400 50  0001 C CNN
F 3 "~" H 1550 2400 50  0001 C CNN
	1    1550 2400
	-1   0    0    1   
$EndComp
Text GLabel 1750 1900 2    50   Input ~ 0
GND
Text GLabel 1750 2800 2    50   Input ~ 0
5V
Text GLabel 1750 2300 2    50   Input ~ 0
DHT_SENSOR1
Text GLabel 1750 2200 2    50   Input ~ 0
DHT_SENSOR2
Text GLabel 1750 2700 2    50   Input ~ 0
DHT_SENSOR3
Text GLabel 1750 2600 2    50   Input ~ 0
DHT_SENSOR4
Text GLabel 1750 2500 2    50   Input ~ 0
DHT_SENSOR5
Text GLabel 1750 2400 2    50   Input ~ 0
DHT_SENSOR6
Text GLabel 1750 2100 2    50   Input ~ 0
LIGHT_SENSOR
Text GLabel 2450 5200 1    50   Input ~ 0
5V
Text GLabel 2450 6000 3    50   Input ~ 0
GND
Text GLabel 2850 5550 2    50   Input ~ 0
BT_TX_STM_RX
Text GLabel 2850 5650 2    50   Input ~ 0
BT_RX_STM_TX
NoConn ~ 3000 1900
NoConn ~ 3000 2000
NoConn ~ 3000 2100
$Comp
L Connector:Conn_01x04_Female J4
U 1 1 5FFD6F8B
P 2800 2950
F 0 "J4" H 2692 2525 50  0000 C CNN
F 1 "Garage_Conn" H 2692 2616 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 2800 2950 50  0001 C CNN
F 3 "~" H 2800 2950 50  0001 C CNN
	1    2800 2950
	-1   0    0    1   
$EndComp
Text GLabel 3000 2750 2    50   Input ~ 0
GND
Text GLabel 3000 3050 2    50   Input ~ 0
5V
Text GLabel 3000 2850 2    50   Input ~ 0
GARAGE_TX_STM_RX
Text GLabel 3000 2950 2    50   Input ~ 0
GARAGE_RX_STM_TX
Text GLabel 2450 4450 3    50   Input ~ 0
GND
Text GLabel 2450 3600 2    50   Input ~ 0
3.3V
Text GLabel 3050 3950 2    50   Input ~ 0
WIFI_TX_STM_RX
Text GLabel 3050 4150 2    50   Input ~ 0
WIFI_RX_STM_TX
Text GLabel 1850 4100 0    50   Input ~ 0
3.3V
Wire Notes Line
	1350 500  4200 500 
Wire Notes Line
	4200 500  4200 1400
Wire Notes Line
	4200 1400 1350 1400
Wire Notes Line
	1350 1400 1350 500 
Wire Notes Line
	1350 1450 3850 1450
Wire Notes Line
	1350 3300 3850 3300
Wire Notes Line
	3850 3300 3850 4750
Wire Notes Line
	3850 4750 1350 4750
Wire Notes Line
	1350 4750 1350 3300
Wire Notes Line
	3850 3150 1350 3150
Wire Notes Line
	3850 3150 3850 1450
Wire Notes Line
	1350 1450 1350 3150
NoConn ~ 2850 5750
Wire Notes Line
	1350 4950 3850 4950
Wire Notes Line
	3850 4950 3850 6350
Wire Notes Line
	3850 6350 1350 6350
Wire Notes Line
	1350 6350 1350 4950
$Comp
L Memory_EEPROM:AT24CS64-SSHM U3
U 1 1 6001BAFB
P 2500 7050
F 0 "U3" H 2500 7531 50  0000 C CNN
F 1 "AT24CS64-SSHM" H 2500 7440 50  0000 C CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 2500 7050 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-8870-SEEPROM-AT24CS64-Datasheet.pdf" H 2500 7050 50  0001 C CNN
	1    2500 7050
	1    0    0    -1  
$EndComp
Text GLabel 2900 7150 2    50   Input ~ 0
GND
Text GLabel 2500 7350 3    50   Input ~ 0
GND
Text GLabel 2100 7150 0    50   Input ~ 0
GND
Text GLabel 2100 7050 0    50   Input ~ 0
GND
Text GLabel 2100 6950 0    50   Input ~ 0
GND
Text GLabel 2500 6750 2    50   Input ~ 0
3.3V
Wire Notes Line
	1350 6500 3850 6500
Wire Notes Line
	3850 6500 3850 7600
Wire Notes Line
	3850 7600 1350 7600
Wire Notes Line
	1350 7600 1350 6500
$Comp
L project_symbols:Black_Pill U5
U 1 1 6002FE9C
P 7000 4300
F 0 "U5" H 7000 5465 50  0000 C CNN
F 1 "Black_Pill" H 7000 5374 50  0000 C CNN
F 2 "project_footprints:Black_pill" H 7000 4350 50  0001 C CNN
F 3 "" H 6700 4450 50  0001 C CNN
	1    7000 4300
	1    0    0    -1  
$EndComp
NoConn ~ 7400 3400
NoConn ~ 6600 5100
NoConn ~ 7400 3600
NoConn ~ 7400 3500
Text GLabel 6600 5300 0    50   Input ~ 0
3.3V
Text GLabel 6600 5200 0    50   Input ~ 0
GND
NoConn ~ 7400 5300
NoConn ~ 7400 4900
Text GLabel 6600 4000 0    50   Input ~ 0
BT_TX_STM_RX
Text GLabel 6600 5000 0    50   Input ~ 0
DHT_SENSOR1
Text GLabel 7400 3700 2    50   Input ~ 0
DHT_SENSOR2
Text GLabel 6600 3400 0    50   Input ~ 0
DHT_SENSOR3
Text GLabel 6600 3500 0    50   Input ~ 0
DHT_SENSOR4
Text GLabel 6600 3600 0    50   Input ~ 0
DHT_SENSOR5
Text GLabel 6600 3700 0    50   Input ~ 0
DHT_SENSOR6
$Comp
L Device:R R1
U 1 1 6004496A
P 5100 4400
F 0 "R1" H 5170 4446 50  0000 L CNN
F 1 "10k" H 5170 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 5030 4400 50  0001 C CNN
F 3 "~" H 5100 4400 50  0001 C CNN
	1    5100 4400
	1    0    0    -1  
$EndComp
Text GLabel 5100 4250 1    50   Input ~ 0
5V
$Comp
L Connector:Conn_01x06_Female J3
U 1 1 5FFD3153
P 2800 2200
F 0 "J3" H 2692 1675 50  0000 C CNN
F 1 "I2C_Conn" H 2692 1766 50  0000 C CNN
F 2 "Connector_IDC:IDC-Header_2x03_P2.54mm_Vertical" H 2800 2200 50  0001 C CNN
F 3 "~" H 2800 2200 50  0001 C CNN
	1    2800 2200
	-1   0    0    1   
$EndComp
$Comp
L Device:R R3
U 1 1 6004C3D0
P 5650 4400
F 0 "R3" H 5720 4446 50  0000 L CNN
F 1 "10k" H 5720 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 5580 4400 50  0001 C CNN
F 3 "~" H 5650 4400 50  0001 C CNN
	1    5650 4400
	1    0    0    -1  
$EndComp
Text GLabel 5350 4250 1    50   Input ~ 0
5V
Text GLabel 5650 4250 1    50   Input ~ 0
5V
$Comp
L Device:R R5
U 1 1 600573C9
P 8000 4950
F 0 "R5" H 7930 4904 50  0000 R CNN
F 1 "10k" H 7930 4995 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 7930 4950 50  0001 C CNN
F 3 "~" H 8000 4950 50  0001 C CNN
	1    8000 4950
	-1   0    0    1   
$EndComp
$Comp
L Device:R R7
U 1 1 60059002
P 8250 4950
F 0 "R7" H 8320 4996 50  0000 L CNN
F 1 "10k" H 8320 4905 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 8180 4950 50  0001 C CNN
F 3 "~" H 8250 4950 50  0001 C CNN
	1    8250 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 4800 8000 4800
Text GLabel 8000 5100 3    50   Input ~ 0
GND
Connection ~ 8000 4800
Text GLabel 8250 5100 3    50   Input ~ 0
5V
$Comp
L Device:R R11
U 1 1 6005F8F3
P 8750 4550
F 0 "R11" H 8680 4504 50  0000 R CNN
F 1 "5.1k" H 8680 4595 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 8680 4550 50  0001 C CNN
F 3 "~" H 8750 4550 50  0001 C CNN
	1    8750 4550
	-1   0    0    1   
$EndComp
$Comp
L Device:R R9
U 1 1 6005FC28
P 8500 4550
F 0 "R9" H 8430 4504 50  0000 R CNN
F 1 "1k" H 8430 4595 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 8430 4550 50  0001 C CNN
F 3 "~" H 8500 4550 50  0001 C CNN
	1    8500 4550
	-1   0    0    1   
$EndComp
Text GLabel 8750 4400 1    50   Input ~ 0
12V
Text GLabel 8500 4400 1    50   Input ~ 0
GND
Wire Wire Line
	8000 4800 8250 4800
Wire Wire Line
	8500 4700 8750 4700
$Comp
L Device:R R14
U 1 1 6006FBED
P 9150 4550
F 0 "R14" H 9080 4504 50  0000 R CNN
F 1 "10k" H 9080 4595 50  0000 R CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 9080 4550 50  0001 C CNN
F 3 "~" H 9150 4550 50  0001 C CNN
	1    9150 4550
	-1   0    0    1   
$EndComp
$Comp
L Device:R R16
U 1 1 6006FBF3
P 9400 4550
F 0 "R16" H 9470 4596 50  0000 L CNN
F 1 "10k" H 9470 4505 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 9330 4550 50  0001 C CNN
F 3 "~" H 9400 4550 50  0001 C CNN
	1    9400 4550
	1    0    0    -1  
$EndComp
Text GLabel 9150 4700 3    50   Input ~ 0
GND
Text GLabel 6600 4200 0    50   Input ~ 0
GARAGE_TX_STM_RX
Text GLabel 6600 4100 0    50   Input ~ 0
GARAGE_RX_STM_TX
Wire Wire Line
	4800 4600 4900 4600
Wire Wire Line
	4800 4700 5350 4700
Connection ~ 5350 4700
Wire Wire Line
	4800 4800 5650 4800
Connection ~ 5650 4800
Wire Wire Line
	5350 4700 6600 4700
Wire Wire Line
	5650 4800 6600 4800
Text GLabel 7400 4500 2    50   Input ~ 0
WIFI_TX_STM_RX
Text GLabel 7400 4600 2    50   Input ~ 0
WIFI_RX_STM_TX
Wire Wire Line
	7400 4700 8500 4700
Connection ~ 8500 4700
Text GLabel 9400 4700 3    50   Input ~ 0
LIGHT_SENSOR
Text GLabel 7400 3800 2    50   Input ~ 0
GND
Text GLabel 7400 3900 2    50   Input ~ 0
GND
Text GLabel 7400 4000 2    50   Input ~ 0
GND
Text GLabel 7400 4100 2    50   Input ~ 0
GND
Text GLabel 7400 4200 2    50   Input ~ 0
GND
Text GLabel 7400 4300 2    50   Input ~ 0
GND
Text GLabel 6600 4300 0    50   Input ~ 0
GND
Text GLabel 6600 4400 0    50   Input ~ 0
GND
Text GLabel 6600 4900 0    50   Input ~ 0
GND
Text GLabel 6600 4600 0    50   Input ~ 0
GND
Wire Wire Line
	6250 4500 6250 4600
Wire Wire Line
	6250 4500 6600 4500
NoConn ~ 3050 3850
NoConn ~ 3050 4050
NoConn ~ 1850 3900
Text GLabel 2850 5450 2    50   Input ~ 0
BT_KEY
Text GLabel 6600 3900 0    50   Input ~ 0
BT_RX_STM_TX
Text GLabel 5600 3800 0    50   Input ~ 0
BT_KEY
$Comp
L Device:R R4
U 1 1 600A73C6
P 5700 3650
F 0 "R4" H 5770 3696 50  0000 L CNN
F 1 "10k" H 5770 3605 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 5630 3650 50  0001 C CNN
F 3 "~" H 5700 3650 50  0001 C CNN
	1    5700 3650
	1    0    0    -1  
$EndComp
Text GLabel 5700 3500 1    50   Input ~ 0
GND
Wire Wire Line
	5600 3800 5700 3800
Connection ~ 5700 3800
Wire Wire Line
	5700 3800 6600 3800
$Comp
L Device:C C5
U 1 1 600B5AF0
P 4900 4400
F 0 "C5" H 4650 4450 50  0000 L CNN
F 1 "470u" H 4650 4350 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P2.50mm" H 4938 4250 50  0001 C CNN
F 3 "~" H 4900 4400 50  0001 C CNN
	1    4900 4400
	1    0    0    -1  
$EndComp
Connection ~ 4900 4600
Text GLabel 4900 4250 1    50   Input ~ 0
GND
$Comp
L Device:R R2
U 1 1 6004B406
P 5350 4400
F 0 "R2" H 5420 4446 50  0000 L CNN
F 1 "10k" H 5420 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 5280 4400 50  0001 C CNN
F 3 "~" H 5350 4400 50  0001 C CNN
	1    5350 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 4150 9150 4150
Wire Wire Line
	9150 4150 9400 4150
Connection ~ 9150 4150
Wire Wire Line
	7700 4150 7700 4400
Wire Wire Line
	9150 4150 9150 4400
Wire Wire Line
	9400 4150 9400 4400
Text GLabel 8650 1050 3    50   Input ~ 0
DHT_SENSOR3
Text GLabel 8900 1050 3    50   Input ~ 0
DHT_SENSOR4
Text GLabel 9150 1050 3    50   Input ~ 0
DHT_SENSOR5
Text GLabel 9400 1050 3    50   Input ~ 0
DHT_SENSOR6
Text GLabel 8150 1050 3    50   Input ~ 0
DHT_SENSOR1
Text GLabel 8400 1050 3    50   Input ~ 0
DHT_SENSOR2
Text GLabel 8800 750  1    50   Input ~ 0
5V
$Comp
L Device:R R6
U 1 1 5FFF28C7
P 8150 900
F 0 "R6" H 8220 946 50  0000 L CNN
F 1 "10k" H 8220 855 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 8080 900 50  0001 C CNN
F 3 "~" H 8150 900 50  0001 C CNN
	1    8150 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5FFF3C14
P 8400 900
F 0 "R8" H 8470 946 50  0000 L CNN
F 1 "10k" H 8470 855 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 8330 900 50  0001 C CNN
F 3 "~" H 8400 900 50  0001 C CNN
	1    8400 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 5FFF4040
P 8650 900
F 0 "R10" H 8720 946 50  0000 L CNN
F 1 "10k" H 8720 855 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 8580 900 50  0001 C CNN
F 3 "~" H 8650 900 50  0001 C CNN
	1    8650 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:R R12
U 1 1 5FFF434F
P 8900 900
F 0 "R12" H 8970 946 50  0000 L CNN
F 1 "10k" H 8970 855 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 8830 900 50  0001 C CNN
F 3 "~" H 8900 900 50  0001 C CNN
	1    8900 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 5FFF478A
P 9150 900
F 0 "R13" H 9220 946 50  0000 L CNN
F 1 "10k" H 9220 855 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 9080 900 50  0001 C CNN
F 3 "~" H 9150 900 50  0001 C CNN
	1    9150 900 
	1    0    0    -1  
$EndComp
$Comp
L Device:R R15
U 1 1 5FFF4ADD
P 9400 900
F 0 "R15" H 9470 946 50  0000 L CNN
F 1 "10k" H 9470 855 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.30x1.75mm_HandSolder" V 9330 900 50  0001 C CNN
F 3 "~" H 9400 900 50  0001 C CNN
	1    9400 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 750  8400 750 
Connection ~ 8400 750 
Wire Wire Line
	8400 750  8650 750 
Connection ~ 8650 750 
Wire Wire Line
	8650 750  8900 750 
Connection ~ 8900 750 
Wire Wire Line
	8900 750  9150 750 
Connection ~ 9150 750 
Wire Wire Line
	9150 750  9400 750 
$Comp
L Connector:Conn_01x10_Female J5
U 1 1 60026D17
P 10900 1050
F 0 "J5" H 10792 325 50  0000 C CNN
F 1 "Debug_Sensors_Conn" H 10792 416 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x10_P2.54mm_Vertical" H 10900 1050 50  0001 C CNN
F 3 "~" H 10900 1050 50  0001 C CNN
	1    10900 1050
	1    0    0    -1  
$EndComp
Text GLabel 10700 1550 0    50   Input ~ 0
GND
Text GLabel 10700 650  0    50   Input ~ 0
5V
Text GLabel 10700 1150 0    50   Input ~ 0
DHT_SENSOR1
Text GLabel 10700 1250 0    50   Input ~ 0
DHT_SENSOR2
Text GLabel 10700 750  0    50   Input ~ 0
DHT_SENSOR3
Text GLabel 10700 850  0    50   Input ~ 0
DHT_SENSOR4
Text GLabel 10700 950  0    50   Input ~ 0
DHT_SENSOR5
Text GLabel 10700 1050 0    50   Input ~ 0
DHT_SENSOR6
Text GLabel 10700 1350 0    50   Input ~ 0
LIGHT_SENSOR
$Comp
L Connector:Conn_01x04_Female J6
U 1 1 6003235C
P 10900 2100
F 0 "J6" H 10792 1675 50  0000 C CNN
F 1 "Debug_I2C_Conn" H 10792 1766 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 10900 2100 50  0001 C CNN
F 3 "~" H 10900 2100 50  0001 C CNN
	1    10900 2100
	1    0    0    -1  
$EndComp
Text GLabel 10700 2300 0    50   Input ~ 0
GND
NoConn ~ 10700 1450
Text Notes 3050 3450 0    79   ~ 0
WiFi Module
Text Notes 3100 5150 0    79   ~ 0
BT Module
Text Notes 3050 6700 0    79   ~ 0
I2C EEPROM
Wire Notes Line
	9700 550  11200 550 
Wire Notes Line
	11200 550  11200 2850
Wire Notes Line
	11200 2850 9700 2850
Wire Notes Line
	9700 2850 9700 550 
Text Notes 9900 2800 0    79   ~ 0
Debug Connections
Wire Notes Line
	7900 550  9650 550 
Wire Notes Line
	9650 1850 7900 1850
Wire Notes Line
	7900 1850 7900 550 
Wire Notes Line
	9650 550  9650 1850
Text Notes 8300 1800 0    79   ~ 0
Sensors PullUp
Wire Notes Line
	4200 2750 9650 2750
Wire Notes Line
	9650 2750 9650 6050
Wire Notes Line
	9650 6050 4200 6050
Wire Notes Line
	4200 6050 4200 2750
Text Notes 8450 2950 0    79   ~ 0
MCU connections
Text Notes 2550 1600 0    79   ~ 0
External Connections
Text Notes 3350 650  0    79   ~ 0
Power Supply
NoConn ~ 1750 2000
NoConn ~ 7400 5000
NoConn ~ 7400 5100
NoConn ~ 7400 5200
NoConn ~ 12100 5900
Text GLabel 10700 2200 0    50   Input ~ 0
I2C_INT
Text GLabel 10700 2000 0    50   Input ~ 0
I2C_SCL
Text GLabel 10700 2100 0    50   Input ~ 0
I2C_SDA
Text GLabel 2900 6950 2    50   Input ~ 0
I2C_SDA
Text GLabel 2900 7050 2    50   Input ~ 0
I2C_SCL
Text GLabel 4800 4800 0    50   Input ~ 0
I2C_SDA
Text GLabel 4800 4700 0    50   Input ~ 0
I2C_SCL
Text GLabel 4800 4600 0    50   Input ~ 0
I2C_INT
Text GLabel 3000 2200 2    50   Input ~ 0
I2C_INT
Text GLabel 3000 2400 2    50   Input ~ 0
I2C_SDA
Text GLabel 3000 2300 2    50   Input ~ 0
I2C_SCL
Wire Wire Line
	4900 4600 5100 4600
Wire Wire Line
	4900 4550 4900 4600
Wire Wire Line
	5100 4550 5100 4600
Connection ~ 5100 4600
Wire Wire Line
	5650 4550 5650 4800
Wire Wire Line
	5350 4550 5350 4700
Wire Wire Line
	5100 4600 6250 4600
$Comp
L project_symbols:ESP-01 U1
U 1 1 5FFDA57F
P 2450 4000
F 0 "U1" H 2450 4581 50  0000 C CNN
F 1 "ESP-01" H 2450 4490 50  0000 C CNN
F 2 "project_footprints:ESP01" H 2450 4489 50  0001 C CNN
F 3 "" H 2100 4100 50  0001 C CNN
	1    2450 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 4400 7700 4400
$Comp
L project_symbols:HC_05 U2
U 1 1 600163EE
P 2450 5600
F 0 "U2" H 2122 5646 50  0000 R CNN
F 1 "HC_05" H 2122 5555 50  0000 R CNN
F 2 "project_footprints:HC_05" H 2350 5600 50  0001 C CNN
F 3 "" H 2100 5700 50  0001 C CNN
	1    2450 5600
	1    0    0    -1  
$EndComp
$EndSCHEMATC
