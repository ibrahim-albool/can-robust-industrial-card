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

#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include "../h/user.h"            /* variables/params used by user.c               */
#include "../h/ECAN1Config.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

unsigned char messageReceived=0; /* Data message is received and ready to be processed*/


/* Initializes the ports of dspProcessor  */
void initIOs(void){
    
    LATCbits.LATC6=0;
    LATCbits.LATC7=0;
//    LATCbits.LATC8=0;
//    LATDbits.LATD5=0;
    TRISCbits.TRISC6=0;
    TRISCbits.TRISC7=0;
//    TRISCbits.TRISC8=0;
//    TRISDbits.TRISD5=0;
    TRISDbits.TRISD6=1;
}


/* ECAN1 write the successful connection acknowledgment message to the buffer  */
void ecan1WriteConnectionAcknowldgment(void){


/* Writing the message for Transmission

ecan1WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit);
ecan1WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);

buf -> Transmit Buffer number

txIdentifier -> SID<10:0> : EID<17:0>

ide = 0 -> Message will transmit standard identifier
ide = 1 -> Message will transmit extended identifier

remoteTransmit = 0 -> Normal message
remoteTransmit = 1 -> Message will request remote transmission

dataLength -> Data length can be from 0 to 8 bytes

data1, data2, data3, data4 -> Data words (2 bytes) each


*/

ecan1WriteTxMsgBufId(0,CAN_CONNECTION_ACKNOWLEDGMENT_ID,0,0);
ecan1WriteTxMsgBufData(0,0,0x0000,0x0000,0x0000,0x0000);



}

/* ECAN1 write the successful data message acknowledgment to the buffer  */
void ecan1WriteDataTemplate(void){


/* Writing the message for Transmission

ecan1WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit);
ecan1WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);

buf -> Transmit Buffer number

txIdentifier -> SID<10:0> : EID<17:0>

ide = 0 -> Message will transmit standard identifier
ide = 1 -> Message will transmit extended identifier

remoteTransmit = 0 -> Normal message
remoteTransmit = 1 -> Message will request remote transmission

dataLength -> Data length can be from 0 to 8 bytes

data1, data2, data3, data4 -> Data words (2 bytes) each


*/


//ecan1WriteTxMsgBufId(1,0x1,0,0);
ecan1WriteTxMsgBufId(1,CAN_DATA_ID,0,0);
ecan1WriteTxMsgBufData(1,6,0x0000,0x0000,0x0000,0x0000);



}


/* ECAN1 buffer loaded with error message */
void ecan1WriteErrorMessage(void){


/* Writing the message for Transmission

ecan1WriteTxMsgBufId(unsigned int buf, long txIdentifier, unsigned int ide, unsigned int remoteTransmit);
ecan1WriteTxMsgBufData(unsigned int buf, unsigned int dataLength, unsigned int data1, unsigned int data2, unsigned int data3, unsigned int data4);

buf -> Transmit Buffer number

txIdentifier -> SID<10:0> : EID<17:0>

ide = 0 -> Message will transmit standard identifier
ide = 1 -> Message will transmit extended identifier

remoteTransmit = 0 -> Normal message
remoteTransmit = 1 -> Message will request remote transmission

dataLength -> Data length can be from 0 to 8 bytes

data1, data2, data3, data4 -> Data words (2 bytes) each


*/

//ecan1WriteTxMsgBufId(0,0x1FFEFFFF,1,0);
ecan1WriteTxMsgBufId(2,CAN_ERROR_MESSAGE_ID,0,0);
ecan1WriteTxMsgBufData(2,0,0x0000,0x0000,0x0000,0x0000);

}

void delay(void);

void delay(void){
    char i=5;
    while (i-->0)
        ;
    
    
}

void getInPoints(void){
	char pntr;
    unsigned int points[3]={0x0000,0x0000,0x0000};
    static unsigned int prevPoints[3]={0x0000,0x0000,0x0000};
	parallelLoad=0;
	delay();
	parallelLoad=1;
	delay();
	parallelLoad=0;
	clock=1;
	delay();
	clock=0;
	delay();
    
    for(pntr=15;pntr>=0;pntr--){
        if(!dataIn)
            points[0]|=0x8000;
        if(pntr!=0)
            points[0]>>=1;
        clock=1;
        delay();
        clock=0;
        delay();
    }
    for(pntr=15;pntr>=0;pntr--){
        if(!dataIn)
            points[1]|=0x8000;
        if(pntr!=0)
            points[1]>>=1;
        clock=1;
        delay();
        clock=0;
        delay();
    }
    for(pntr=15;pntr>=1;pntr--){
        if(!dataIn)
            points[2]|=0x8000;
        if(pntr!=1)
            points[2]>>=1;
        clock=1;
        delay();
        clock=0;
        delay();
    }
    points[2]>>=1;
    
//    if(prevPoints[0]^points[0] || prevPoints[1]^points[1] || prevPoints[2]^points[2] || manualMessageTrigger){
        // If there is a change in the data, then transmit it.
        dataAcknowledgmentWaiting=1;
        ecan1msgBuf[1][3]=points[0];
        ecan1msgBuf[1][4]=points[1];
        ecan1msgBuf[1][5]=points[2];
        C1TR01CONbits.TXREQ1=1;
        manualMessageTrigger=0;
//    }
    
    prevPoints[0]=points[0];
    prevPoints[1]=points[1];
    prevPoints[2]=points[2];
}

void startCountingForMessageSend(unsigned char startCounting){
    static unsigned char cnt=0;
    
    cnt++;
    if(cnt>10){
        doSingleShoot=0;
        manualMessageTrigger=1;
    }

    if(startCounting){
        doSingleShoot=1;
        cnt=0;
    }
    
}