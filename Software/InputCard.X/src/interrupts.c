/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

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

#include <stdint.h>        /* Includes uint16_t definition   */
#include <stdbool.h>       /* Includes true/false definition */
#include "../h/user.h"

//void __attribute__((interrupt, no_auto_psv))_C1Interrupt(void)  
//{
//	IFS2bits.C1IF = 0;        // clear interrupt flag
//	if(C1INTFbits.TBIF)
//    { 
//        LATCbits.LATC7=1;
//
//    	C1INTFbits.TBIF = 0;
//    } 
// 
//    if(C1INTFbits.RBIF)
//    {      
//        LATCbits.LATC6=1;
//
//		C1INTFbits.RBIF = 0;
//	}
//}


 

//------------------------------------------------------------------------------
//    DMA interrupt handlers
//------------------------------------------------------------------------------

/* Massage write interrupt */
void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
   IFS0bits.DMA0IF = 0;          // Clear the DMA0 Interrupt Flag;
}

/* Message read interrupt */
void __attribute__((interrupt, no_auto_psv)) _DMA2Interrupt(void)
{
//   //(ecan1msgBuf[C1FIFObits.FNRB&0x3F][0]>>2)&0x7FF
   switch(C1VECbits.FILHIT){
       case 0:// Message received through out filter 0 (CAN Connection)
           C1TR01CONbits.TXREQ0=1; /* Acknowledges CAN Connection message */
           startCountingForMessageSend(1);
           break;
       case 1:// Message received through out filter 1 (CAN Data Acknowledgment)
           dataAcknowledgmentWaiting=0; /* CAN Data message Acknowledgment */
           break;
       case 2:// Message received through out filter 2 (CAN Data Request)
           dataAcknowledgmentWaiting=1; /* Reply with the data */
           C1TR01CONbits.TXREQ1=1;
           break;
   }
   messageReceived=1;
   IFS1bits.DMA2IF = 0;          // Clear the DMA2 Interrupt Flag;
}
