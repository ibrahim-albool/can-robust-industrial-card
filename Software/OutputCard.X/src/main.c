/**********************************************************************
* © 2005 Microchip Technology Inc.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33Fxxxx
* Compiler:        MPLAB® C30 v3.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author          	Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Vinaya Skanda 	10/18/06  First release of source file
* Vinaya Skanda		07/25/07  Updates from Joe Supinsky and Jatinder Gharoo incorporated
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This code is tested on CANNodeCard Version2 board with B&R 2003 PLC.
* The device used is dsPIC33EP512MC506 controller 
*
* The Processor starts with the External Crystal without PLL enabled and then the Clock is switched to PLL Mode.
*************************************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif


#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "../h/ECAN1Config.h"
#include "../h/user.h"
#include "../h/system.h"
#include "../h/common.h"

//  Macros for Configuration Fuse Registers 
#pragma config GWRP = OFF           // General Segment Write-Protect bit (General Segment may be written)
//#pragma config GSS = OFF            // General Segment Code-Protect bit (General Segment Code protect is disabled)
//#pragma config GSSK = OFF           // General Segment Key bits (General Segment Write Protection and Code Protection is Disabled)

// FOSCSEL
#pragma config FNOSC = FRC          // Initial Oscillator Source Selection Bits (Internal Fast RC (FRC))
#pragma config IESO = OFF           // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FOSC
#pragma config POSCMD = XT          // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF       // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = OFF        // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECMD       // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

// FWDT
#pragma config WDTPOST = PS32768    // Watchdog Timer Postscaler Bits (1:32,768)
#pragma config WDTPRE = PR128       // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON          // PLL Lock Wait Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF         // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF         // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FPOR
//#pragma config FPWRT = PWR128       // Power-on Reset Timer Value Select bits (128ms)
//#pragma config BOREN = ON           // Brown-out Reset (BOR) Detection Enable bit (BOR is enabled)
#pragma config ALTI2C1 = OFF        // Alternate I2C pins for I2C1 (SDA1/SCK1 pins are selected as the I/O pins for I2C1)
#pragma config ALTI2C2 = OFF        // Alternate I2C pins for I2C2 (SDA2/SCK2 pins are selected as the I/O pins for I2C2)

// FICD
#pragma config ICS = PGD1           // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
//#pragma config RSTPRI = PF          // Reset Target Vector Select bit (Device will obtain reset instruction from Primary flash)
#pragma config JTAGEN = OFF         // JTAG Enable bit (JTAG is disabled)


// Define ECAN Message Buffers
__eds__ ECAN1MSGBUF ecan1msgBuf __attribute__((eds,aligned(ECAN1_MSG_BUF_LENGTH*16)));

// Prototype Declaration
unsigned int array[3];

int main(void)
{

/* Configure Oscillator Clock Source 	*/
	configureOscillator();
/* Clear Interrupt Flags 				*/
	clearIntrflags();


/* ECAN1 Initialisation 		
   Configure DMA Channel 0 for ECAN1 Transmit
   Configure DMA Channel 2 for ECAN1 Receive */
	ecan1Init();
	dma0init();	
	dma2init();

/* Enable DMA related ECAN1 Interrupt 	*/			
    IEC0bits.DMA0IE=1;
    IEC1bits.DMA2IE=1;
    
//    IEC2bits.C1IE = 1;
//	C1INTEbits.TBIE = 1;	
//	C1INTEbits.RBIE = 1;


 
/* Write messages to the buffers			*/

    ecan1WriteConnectionAcknowldgment();
    ecan1WriteDataAcknowldgment();
    ecan1WriteErrorMessage();
	C1TR01CONbits.TXREQ0=0;
	C1TR01CONbits.TXREQ1=0;
	C1TR23CONbits.TXREQ2=0;
	C1TR23CONbits.TXREQ3=0;
	C1TR45CONbits.TXREQ4=0;
	C1TR45CONbits.TXREQ5=0;

    
/* IO TRIS and LAT registers initialization*/
    initIOs();
   
    
/* Loop infinitely */
    
    while (1){
//        long i=3e5;
//        while (i-->0)
//            ;
//        unsigned int ddata[3]={0x2233,0x1144,0x6677};
//        setOutPoints(ddata);

        if(messageReceived){
            unsigned char bufferPointer=(unsigned char)(C1FIFObits.FNRB&0x3F);
            unsigned int messageID=(ecan1msgBuf[bufferPointer][0]>>2)&0x7FF;
//            __eds__ mID *message=ecan1msgBuf[bufferPointer];
            
            switch(messageID){
                case CAN_CONNECTION_ID:// Do nothing as the acknowledgment message is already sent
                    break;
                case CAN_DATA_ID:
                    setOutPoints(&ecan1msgBuf[bufferPointer][3]);
                    break;
            }
            /* Turns off RXFUL bit of the current FIFO message to pop the
             *  message from FIFO automatically */
            if(bufferPointer>15)
                C1RXFUL2^=1<<(bufferPointer&0xF);
            else
                C1RXFUL1^=1<<bufferPointer;
            messageReceived=0;
        }
    }
	
}















