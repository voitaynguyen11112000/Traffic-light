#include "../../BT PIC/libraries/libraries/button/main.h"
#include"../../BT PIC/libraries/libraries/lcd/lcd.h"
#include"../libraries/libraries/button/button.h"
#include"../libraries/libraries/interrupt/interrupt.h"
#include"../libraries/libraries/timer/timer.h"

#define     LED     PORTD
#define     LED1     PORTA
#define     LED2     PORTE
#define     ON      1
#define     OFF     0
#define PORTE_OUT   PORTE
#define TRISE_OUT   TRISE
#define PORTD_OUT   PORTD
#define TRISD_OUT   TRISD
#define PORTA_OUT   PORTA
#define TRISA_OUT   TRISA
#define     INIT_SYSTEM         25
#define     PHASE1_GREEN        26
#define     PHASE1_YELLOW       27
#define     PHASE2_GREEN        28
#define     PHASE2_YELLOW       29
#define     WAIT                30

#define     SETTIME_PHASE1GREEN            31
#define     SETTIME_PHASE1YELLOW            32
#define     SETTIME_PHASE2GREEN            33
#define     SETTIME_PHASE2YELLOW            34

#define     BLINK_YELLOW        35

#define     SET_PHASE1GREEN     36
#define     SET_PHASE1YELLOW    37
#define     SET_PHASE2GREEN     38
#define     SET_PHASE2YELLOW     39

#define     FORCE_SWITCH_GREEN1     40
#define     FORCE_SWITCH_GREEN2     41

#define     TIME_PHASE1_GREEN   15
#define     TIME_PHASE1_YELLOW   5
#define     TIME_PHASE2_GREEN   10
#define     TIME_PHASE2_YELLOW   4

void init_output(void);
// Noi khai bao bien toan cuc
unsigned char arrayMapOfOutput [8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
unsigned char statusOutput[8] = {0,0,0,0,0,0,0,0};
unsigned char arrayNumber[16] = {1,2,3,10,4,5,6,11,7,8,9,12,13,0,14,15};
// Khai bao cac ham co ban IO
void init_system(void);
void delay_ms(int value);
void OpenOutput(int index);
void CloseOutput(int index);
void TestOutput(void);
void ReverseOutput(int index);

// Den giao thong
void Phase1_GreenOn(void);
void Phase1_GreenOff(void);
void Phase1_YellowOn(void);
void Phase1_YellowOff(void);
void Phase1_RedOn(void);
void Phase1_RedOff(void);

void Phase2_GreenOn(void);
void Phase2_GreenOff(void);
void Phase2_YellowOn(void);
void Phase2_YellowOff(void);
void Phase2_RedOn(void);
void Phase2_RedOff(void);

void Phase1_BlinkYellow(void);
void Phase2_BlinkYellow(void);

unsigned char isButtonNumber();

unsigned char statusOfLight = INIT_SYSTEM;
unsigned char timeOfGreenPhase1 = TIME_PHASE1_GREEN;
unsigned char timeOfYellowPhase1 = TIME_PHASE1_YELLOW;
unsigned char timeOfGreenPhase2 = TIME_PHASE2_GREEN;
unsigned char timeOfYellowPhase2 = TIME_PHASE2_YELLOW;
unsigned char timeOfLight = 0;
unsigned char cntOfLight = 0;
unsigned char timeDisplay=0;
unsigned char indexOfNumber=0;
unsigned char numberValue=0;
unsigned char time[2];
unsigned char chuc1=0;
unsigned char donvi1=0;
unsigned char chuc2=0;
unsigned char donvi2=0;
void AppTrafficLight(void);
unsigned char SetTime(void);
void OpenOutputA(int index);
void CloseOutputA(int index);
unsigned char Decode(unsigned char data);
void PrintSevenSegment1(unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4);
void PrintSevenSegment2(unsigned char data);
////////////////////////////////////////////////////////////////////
//Hien thuc cac chuong trinh con, ham, module, function duoi cho nay
////////////////////////////////////////////////////////////////////
void main(void)
{
	unsigned int k = 0; 
	init_system();
	while (1)
	{            
        
        
            while (!Timer3_flag);
            Timer3_flag = 0;
            LCD_clearSync();
            KeyMatrix_scan(); // 8 button
            AppTrafficLight();
            PrintSevenSegment1(Decode(donvi1), Decode(chuc1), Decode(donvi2), Decode(chuc2));
            //PrintSevenSegment(Decode(chuc));
            LCD_display(); //Output process 14ms vs 10ms with no timer
            
	}
}
// Hien thuc cac module co ban cua chuong trinh
void delay_ms(int value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<238;j++);
}

void init_output(void)
{
   TRISD_OUT = 0x00;
   PORTD_OUT = 0x00;
   TRISB = 0x00;
   
}

void init_system(void)
{
    init_output();
    
    LCD_init();
    LCD_clearSync();
    LCD_clear();
    KeyMatrix_init();
    Interrupt_init();
   // Timer0_init(4695);//dinh thoi 1ms sai so 1%
    Timer3_init(46950);//dinh thoi 10ms
    //Timer0_set(2);
    Timer3_set(50); //Chu ky thuc hien viec xu ly input,proccess,output
    
   
   
}
void OpenOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT | arrayMapOfOutput[index];
	}


}

void CloseOutput(int index)
{
	if (index >= 0 && index <= 7)
	{
		PORTD_OUT = PORTD_OUT & ~arrayMapOfOutput[index];
	}
}


void ReverseOutput(int index)
{
    if (statusOutput[index]  == ON)
    {
        CloseOutput(index);
        statusOutput[index] = OFF;
    }
    else
    {
        OpenOutput(index);
        statusOutput[index] = ON;
    }
}

void TestOutput(void)
{
	int k;
	for (k=0;k<14 ;k++ )
	{
		OpenOutput(k);
		delay_ms(500);
		CloseOutput(k);
		delay_ms(500);
	}
}

void Phase1_GreenOn(void)
{
    OpenOutput(0);
}
void Phase1_GreenOff(void)
{
    CloseOutput(0);
}

void Phase1_YellowOn(void)
{
    OpenOutput(4);
}
void Phase1_YellowOff(void)
{
    CloseOutput(4);
}

void Phase1_RedOn(void)
{
    OpenOutput(6);
}
void Phase1_RedOff(void)
{
    CloseOutput(6);
}

void Phase2_GreenOn(void)
{
    OpenOutput(1);
}
void Phase2_GreenOff(void)
{
    CloseOutput(1);
}

void Phase2_YellowOn(void)
{
    OpenOutput(5);
}
void Phase2_YellowOff(void)
{
    CloseOutput(5);
}

void Phase2_RedOn(void)
{
    OpenOutput(7);
}
void Phase2_RedOff(void)
{
    CloseOutput(7);
}
void Phase1_BlinkYellow(void){
    OpenOutput(4);
    CloseOutput(4);
}
void Phase2_BlinkYellow(void){
    OpenOutput(5);
    CloseOutput(5);
}
unsigned char SetTime(void){
    
}
unsigned char isButtonNumber(){
    unsigned char i;
    for(i=0; i<16;i++){
        if(keyCode[i]==1){
            numberValue = arrayNumber[i];
            return 1;
        }      
    }
    return 0;
}
unsigned char Decode(unsigned char data){
    switch (data){
        case 0:
            return 0x03;
           break; 
        case 1:
            return 0x9f;
           break; 
        case 2:
            return 0x25;
           break; 
        case 3:
            return 0x0d;
           break;
        case 4:
            return 0x99;
           break; 
        case 5:
            return 0x49;
           break; 
        case 6:
            return 0x41;
           break; 
        case 7:
            return 0x1f;
           break;
        case 8:
            return 0x01;
           break;
        case 9:
            return 0x09;
           break;
    }
}
void PrintSevenSegment1(unsigned char data1, unsigned char data2, unsigned char data3, unsigned char data4 ){
    unsigned char i;
    unsigned char j;
    for(i=0;i<8;i++){
        CloseOutput(0);
        //delay_ms(1);
        if((data1&0x01)==1)
        {
            OpenOutput(2);
        }
        else
        {
            CloseOutput(2);
        }
        if(data3&0x01==1){
            OpenOutput(3);
        }
        else
        {
            CloseOutput(3);
        }
        data1=data1>>1;
        data3=data3>>1;
       // delay_ms(1);
        OpenOutput(0);
    }
    CloseOutput(0);
    
    for(i=0;i<8;i++){
        CloseOutput(0);
        //delay_ms(1);
        if((data2&0x01)==1)
        {
            OpenOutput(2);
        }
        else
        {
            CloseOutput(2);
        }
        if((data4&0x01)==1)
        {
            OpenOutput(3);
        }
        else
        {
            CloseOutput(3);
        }
        data2=data2>>1;
        data4=data4>>1;
       // delay_ms(1);
        OpenOutput(0);
    }
    CloseOutput(0);
    OpenOutput(1);
    CloseOutput(1);
    
}
void DisplayLed(unsigned char data){
    unsigned char i;
    unsigned char j;
    for(i=0;i<8;i++){
        CloseOutput(4);
        //delay_ms(1);
        if((data&0x01)==1)
        {
            OpenOutput(6);
        }
        else
        {
            CloseOutput(6);
        }
        data=data>>1;
       // delay_ms(1);
        OpenOutput(4);
    }
    CloseOutput(4);
    OpenOutput(5);
    
   // delay_ms(1);
    CloseOutput(5);
}

void AppTrafficLight(void)
{
    cntOfLight = (cntOfLight + 1)%20;
    if (cntOfLight == 0){
        timeOfLight ++;
        timeDisplay--;
    }
    switch (statusOfLight)
    {
        case INIT_SYSTEM:
            LCD_printStrSync(0,0,"INIT SYSTEM");
            if(1){
                timeOfLight=0;
                indexOfNumber=0;
                timeDisplay=timeOfGreenPhase1;
                statusOfLight=PHASE1_GREEN;
            }
            break;
        case PHASE1_GREEN:
            LCD_printStrSync(0,0,"PHASE1 GREEN:");
            LCD_printNumSync(0,14,timeDisplay);
            LCD_printStrSync(1,0,"PHASE2 RED:");
            LCD_printNumSync(1,14,timeDisplay+timeOfYellowPhase1+1);
            donvi1=timeDisplay%10;
            chuc1=timeDisplay/10;
            donvi2=(timeDisplay+timeOfYellowPhase1+1)%10;
            chuc2=(timeDisplay+timeOfYellowPhase1+1)/10;
            DisplayLed(0x21);
            if(timeOfLight>timeOfGreenPhase1){
                timeOfLight=0;
                timeDisplay=timeOfYellowPhase1;
                statusOfLight=PHASE1_YELLOW;
            }
            if(keyCode[3]==1){ 
                statusOfLight=SETTIME_PHASE1GREEN;
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1GREEN;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
        case PHASE1_YELLOW:
            LCD_printStrSync(0,0,"PHASE1 YELLOW:");
            LCD_printNumSync(0,14,timeDisplay);
            LCD_printStrSync(1,0,"PHASE2 RED:");
            LCD_printNumSync(1,14,timeDisplay);
            donvi1=timeDisplay%10;
            chuc1=timeDisplay/10;
            donvi2=(timeDisplay)%10;
            chuc2=(timeDisplay)/10;
            DisplayLed(0x11);
            if(timeOfLight>timeOfYellowPhase1){
                timeOfLight=0;
                timeDisplay=timeOfGreenPhase2;
                statusOfLight=PHASE2_GREEN;
            }
            if(keyCode[3]==1){ 
                statusOfLight=SETTIME_PHASE1GREEN;
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1GREEN;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
        case PHASE2_GREEN:
            LCD_printStrSync(0,0,"PHASE1 RED:");
            LCD_printNumSync(0,14,timeDisplay+timeOfYellowPhase2+1);
            LCD_printStrSync(1,0,"PHASE2 GREEN:");
            LCD_printNumSync(1,14,timeDisplay);
            donvi2=timeDisplay%10;
            chuc2=timeDisplay/10;
            donvi1=(timeDisplay+timeOfYellowPhase2+1)%10;
            chuc1=(timeDisplay+timeOfYellowPhase2+1)/10;
            DisplayLed(0x0c);
            if(timeOfLight>timeOfGreenPhase2){
                timeOfLight=0;
                timeDisplay=timeOfYellowPhase2;
                statusOfLight=PHASE2_YELLOW;
            }
            if(keyCode[3]==1){ 
                statusOfLight=SETTIME_PHASE1GREEN;
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1GREEN;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
        case PHASE2_YELLOW:
            LCD_printStrSync(0,0,"PHASE1 RED:");
            LCD_printNumSync(0,14,timeDisplay);
            LCD_printStrSync(1,0,"PHASE2_YELLOW:");
            LCD_printNumSync(1,14,timeDisplay);
            donvi1=timeDisplay%10;
            chuc1=timeDisplay/10;
            donvi2=(timeDisplay)%10;
            chuc2=(timeDisplay)/10;
            DisplayLed(0x0a);
            if(timeOfLight>timeOfYellowPhase2){
                timeOfLight=0;
                timeDisplay=timeOfGreenPhase1;
                statusOfLight=PHASE1_GREEN;
            }
            if(keyCode[3]==1){ 
                statusOfLight=SETTIME_PHASE1GREEN;
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1GREEN;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
            
        case SETTIME_PHASE1GREEN:
            LCD_printStrSync(0,0,"SETTIME GREEN 1 ");
            donvi1=0xff;
            chuc1=0xff;
            donvi2=0xff;
            chuc2=0xff;
            if(cntOfLight<10){
                DisplayLed(0x12);
            }
            else{
                DisplayLed(0x00);
            }
            if(indexOfNumber>2){
                timeOfGreenPhase1 = TIME_PHASE1_GREEN;
                timeOfYellowPhase1 = TIME_PHASE1_YELLOW;
                timeOfGreenPhase2 = TIME_PHASE2_GREEN;
                timeOfYellowPhase2 = TIME_PHASE2_YELLOW;
                statusOfLight=INIT_SYSTEM;
            }
            if(isButtonNumber()){
                time[indexOfNumber]=numberValue;
                indexOfNumber++;  
                
            }           
            else if(isButtonNumber()==0 && indexOfNumber==1){
                timeOfGreenPhase1=time[0];
                LCD_printNumSync(1,0,timeOfGreenPhase1);
                if(cntOfLight<10){
                    LCD_printStrSync(1,1,"_");
                }
            }
            else if(isButtonNumber()==0 && indexOfNumber==2){
                timeOfGreenPhase1=time[0]*10+time[1];
                LCD_printNumSync(1,0,timeOfGreenPhase1);
            }
            else if(indexOfNumber==0&&cntOfLight<10){
                LCD_printStrSync(1,0,"__");
            }

            
            if(keyCode[3]==1){ 
                indexOfNumber=0;
                statusOfLight=SETTIME_PHASE1YELLOW;               
            }
            if(keyCode[7]==1){
                timeOfLight=0;
                statusOfLight=INIT_SYSTEM;
                
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1GREEN;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
        case SETTIME_PHASE1YELLOW:
            LCD_printStrSync(0,0,"SETTIME YELLOW 1");
            donvi1=0xff;
            chuc1=0xff;
            donvi2=0xff;
            chuc2=0xff;
            if(cntOfLight<10){
                DisplayLed(0x12);
            }
            else{
                DisplayLed(0x00);
            }
            if(indexOfNumber>2){
                timeOfGreenPhase1 = TIME_PHASE1_GREEN;
                timeOfYellowPhase1 = TIME_PHASE1_YELLOW;
                timeOfGreenPhase2 = TIME_PHASE2_GREEN;
                timeOfYellowPhase2 = TIME_PHASE2_YELLOW;
                statusOfLight=INIT_SYSTEM;
            }
            if(isButtonNumber()){
                time[indexOfNumber]=numberValue;
                indexOfNumber++;  
            }           
            else if(isButtonNumber()==0 && indexOfNumber==1){
                timeOfYellowPhase1=time[0];
                LCD_printNumSync(1,0,timeOfYellowPhase1);
                if(cntOfLight<10){
                    LCD_printStrSync(1,1,"_");
                }
            }
            else if(isButtonNumber()==0 && indexOfNumber==2){
                timeOfYellowPhase1=time[0]*10+time[1];
                LCD_printNumSync(1,0,timeOfYellowPhase1);
            }
            else if(indexOfNumber==0&&cntOfLight<10){
                LCD_printStrSync(1,0,"__");
            }
            if(keyCode[3]==1){
                indexOfNumber=0;
                statusOfLight=SETTIME_PHASE2GREEN;
            }
            if(keyCode[7]==1){ 
                timeOfLight=0;
                statusOfLight=INIT_SYSTEM;
                
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1GREEN;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
        case SETTIME_PHASE2GREEN:
            LCD_printStrSync(0,0,"SETTIME GREEN 2");
            donvi1=0xff;
            chuc1=0xff;
            donvi2=0xff;
            chuc2=0xff;
            if(cntOfLight<10){
                DisplayLed(0x12);
            }
            else{
                DisplayLed(0x00);
            }
            if(indexOfNumber>2){
                timeOfGreenPhase1 = TIME_PHASE1_GREEN;
                timeOfYellowPhase1 = TIME_PHASE1_YELLOW;
                timeOfGreenPhase2 = TIME_PHASE2_GREEN;
                timeOfYellowPhase2 = TIME_PHASE2_YELLOW;
                statusOfLight=INIT_SYSTEM;
            }
            if(isButtonNumber()){
                time[indexOfNumber]=numberValue;
                indexOfNumber++;  
            }           
            else if(isButtonNumber()==0 && indexOfNumber==1){
                timeOfGreenPhase2=time[0];
                LCD_printNumSync(1,0,timeOfGreenPhase2);
                if(cntOfLight<10){
                    LCD_printStrSync(1,1,"_");
                }
            }
            else if(isButtonNumber()==0 && indexOfNumber==2){
                timeOfGreenPhase2=time[0]*10+time[1];
                LCD_printNumSync(1,0,timeOfGreenPhase2);
            } 
            else if(indexOfNumber==0&&cntOfLight<10){
                LCD_printStrSync(1,0,"__");
            }
            if(keyCode[3]==1){
                indexOfNumber=0;
                statusOfLight=SETTIME_PHASE2YELLOW;
                
            }
            if(keyCode[7]==1){ 
                timeOfLight=0;
                statusOfLight=INIT_SYSTEM;
                
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1GREEN;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
        case SETTIME_PHASE2YELLOW:
            LCD_printStrSync(0,0,"SETTIME YELLOW 2");
            donvi1=0xff;
            chuc1=0xff;
            donvi2=0xff;
            chuc2=0xff;
            if(cntOfLight<10){
                DisplayLed(0x12);
            }
            else{
                DisplayLed(0x00);
            }
            if(indexOfNumber>2){
                timeOfGreenPhase1 = TIME_PHASE1_GREEN;
                timeOfYellowPhase1 = TIME_PHASE1_YELLOW;
                timeOfGreenPhase2 = TIME_PHASE2_GREEN;
                timeOfYellowPhase2 = TIME_PHASE2_YELLOW;
                statusOfLight=INIT_SYSTEM;
            }
            if(isButtonNumber()){
                time[indexOfNumber]=numberValue;
                indexOfNumber++;  
            }           
            else if(isButtonNumber()==0 && indexOfNumber==1){
                timeOfYellowPhase2=time[0];
                LCD_printNumSync(1,0,timeOfYellowPhase2);
                if(cntOfLight<10){
                    LCD_printStrSync(1,1,"_");
                }
            }
            else if(isButtonNumber()==0 && indexOfNumber==2){
                timeOfYellowPhase2=time[0]*10+time[1];
                LCD_printNumSync(1,0,timeOfYellowPhase2);
            }
            else if(indexOfNumber==0&&cntOfLight<10){
                LCD_printStrSync(1,0,"__");
            }
            if(keyCode[3]==1){
                indexOfNumber=0;
                statusOfLight=SETTIME_PHASE1GREEN;
            }
            if(keyCode[7]==1){
                timeOfLight=0;
                statusOfLight=INIT_SYSTEM;              
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1GREEN;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
            
        case BLINK_YELLOW:
            LCD_printStrSync(0,0,"BLINK YELLOW");
            donvi1=0xff;
            chuc1=0xff;
            donvi2=0xff;
            chuc2=0xff;
            if(cntOfLight<10){
                DisplayLed(0x12);
            }
            else{
                DisplayLed(0x00);
            }
            if(keyCode[3]==1){
                indexOfNumber=0;
                timeOfLight=0;
                statusOfLight=SETTIME_PHASE1GREEN;
            }
            if(keyCode[7]==1){
                timeOfLight=0;
                statusOfLight=INIT_SYSTEM;              
            }
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1GREEN;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
            
        case SET_PHASE1GREEN:
            LCD_printStrSync(0,0,"PHASE1 GREEN");
            LCD_printStrSync(1,0,"PHASE2 RED");
            donvi1=0xff;
            chuc1=0xff;
            donvi2=0xff;
            chuc2=0xff;
            DisplayLed(0x21);
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1YELLOW;
            }
            if(keyCode[3]==1){
                indexOfNumber=0;
                timeOfLight=0;
                statusOfLight=SETTIME_PHASE1GREEN;
            }
            if(keyCode[7]==1){
                timeOfLight=0;
                statusOfLight=INIT_SYSTEM;              
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
        case SET_PHASE1YELLOW:
            LCD_printStrSync(0,0,"PHASE1 YELLOW");
            LCD_printStrSync(1,0,"PHASE2 RED");
            donvi1=0xff;
            chuc1=0xff;
            donvi2=0xff;
            chuc2=0xff;
            DisplayLed(0x11);
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE2GREEN;
            }
            if(keyCode[3]==1){
                indexOfNumber=0;
                timeOfLight=0;
                statusOfLight=SETTIME_PHASE1GREEN;
            }
            if(keyCode[7]==1){
                timeOfLight=0;
                statusOfLight=INIT_SYSTEM;              
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
        case SET_PHASE2GREEN:
            LCD_printStrSync(0,0,"PHASE1 RED");
            LCD_printStrSync(1,0,"PHASE2 GREEN");
            donvi1=0xff;
            chuc1=0xff;
            donvi2=0xff;
            chuc2=0xff;
            DisplayLed(0x0c);
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE2YELLOW;
            }
            if(keyCode[3]==1){
                indexOfNumber=0;
                timeOfLight=0;
                statusOfLight=SETTIME_PHASE1GREEN;
            }
            if(keyCode[7]==1){
                timeOfLight=0;
                statusOfLight=INIT_SYSTEM;              
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
        case SET_PHASE2YELLOW:
            LCD_printStrSync(0,0,"PHASE1 RED");
            LCD_printStrSync(1,0,"PHASE2 YELLOW");
            donvi1=0xff;
            chuc1=0xff;
            donvi2=0xff;
            chuc2=0xff;
            DisplayLed(0x0a);
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1GREEN;
            }
            if(keyCode[3]==1){
                indexOfNumber=0;
                timeOfLight=0;
                statusOfLight=SETTIME_PHASE1GREEN;
            }
            if(keyCode[7]==1){
                timeOfLight=0;
                statusOfLight=INIT_SYSTEM;              
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            break;
        case FORCE_SWITCH_GREEN1:
            LCD_printStrSync(0,0,"PHASE1 GREEN");
            LCD_printStrSync(1,0,"PHASE2 RED");
            DisplayLed(0x21);
            donvi1=0xff;
            chuc1=0xff;
            donvi2=0xff;
            chuc2=0xff;
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN2;
            }
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1GREEN;
            }
            if(keyCode[3]==1){
                indexOfNumber=0;
                timeOfLight=0;
                statusOfLight=SETTIME_PHASE1GREEN;
            }
            if(keyCode[7]==1){
                timeOfLight=0;
                statusOfLight=INIT_SYSTEM;              
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            break;
        case FORCE_SWITCH_GREEN2:
            LCD_printStrSync(0,0,"PHASE1 RED");
            LCD_printStrSync(1,0,"PHASE2 GREEN");
            DisplayLed(0x0c);
            donvi1=0xff;
            chuc1=0xff;
            donvi2=0xff;
            chuc2=0xff;
            if(keyCode[12]==1){
                statusOfLight=FORCE_SWITCH_GREEN1;
            }
            if(keyCode[15]==1){
                statusOfLight=SET_PHASE1GREEN;
            }
            if(keyCode[3]==1){
                indexOfNumber=0;
                timeOfLight=0;
                statusOfLight=SETTIME_PHASE1GREEN;
            }
            if(keyCode[7]==1){
                timeOfLight=0;
                statusOfLight=INIT_SYSTEM;              
            }
            if(keyCode[11]==1){
                statusOfLight=BLINK_YELLOW;
            }
            break;
    }
}