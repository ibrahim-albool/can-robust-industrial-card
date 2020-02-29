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
    LATCbits.LATC8=0;
    LATDbits.LATD5=0;
    TRISCbits.TRISC6=0;
    TRISCbits.TRISC7=0;
    TRISCbits.TRISC8=0;
    TRISDbits.TRISD5=0;
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
void ecan1WriteDataAcknowldgment(void){


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

ecan1WriteTxMsgBufId(1,CAN_DATA_ACKNOWLEDGMENT_ID,0,0);
ecan1WriteTxMsgBufData(1,0,0x0000,0x0000,0x0000,0x0000);



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

void setOutPoints(__eds__ unsigned int * msgPtr){
    unsigned int d1,d2,d3;
    d1=*msgPtr++;
    d2=*msgPtr++;
    d3=*msgPtr;
	signed char pntr;
	strobe=0;
    //Redundancy of loops to decrease time of clock.
    for(pntr=15;pntr>=0;pntr--){
        clock=0;
        dataOut=0x01&d1;
        d1>>=1;
        clock=1;
    }
    for(pntr=15;pntr>=0;pntr--){
        clock=0;
        dataOut=0x01&d2;
        d2>>=1;
        clock=1;
    }
    for(pntr=15;pntr>=2;pntr--){
        clock=0;
        dataOut=0x01&d3;
        d3>>=1;
        clock=1;
    }
	clock=0;
	strobe=1;
	outEnable=1;
	strobe=0;
}
