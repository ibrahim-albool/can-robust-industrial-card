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

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */



/* The Ids of the filters acceptance messages */
#define CAN_CONNECTION_ID 0x11 /* ID of the received connection message*/
#define CAN_CONNECTION_ACKNOWLEDGMENT_ID 0x12 /* ID of the connection acknowledgment message*/

#define CAN_DATA_ID 0x3  //0x3 /* ID of the received data message*/
#define CAN_DATA_ACKNOWLEDGMENT_ID 0x2 //0x4 /* ID of the data acknowledgment message*/

#define CAN_DATA_REQUEST_ID 0x5 /* ID of the data request from main controller*/

#define CAN_ERROR_MESSAGE_ID 0x100 /* ID of the error frame to be sent to the PLC*/

#define clock LATCbits.LATC6
#define parallelLoad LATCbits.LATC7 // strobe pin
#define dataIn PORTDbits.RD6

/******************************************************************************/
/* User Global Variables                                                      */
/******************************************************************************/

extern unsigned char messageReceived; /* Data message is received and ready to be processed*/
extern unsigned char dataAcknowledgmentWaiting; /* Data is sent and acknowledgment signal is being waiteds.*/
extern unsigned char manualMessageTrigger; /* Send the data manually even it is not changed, when connection message is sent*/
extern unsigned char doSingleShoot; /* Starts the counting for sending a single message */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void initIOs(void);         /* I/O and Peripheral Initialization */

void ecan1WriteConnectionAcknowldgment(void);

void ecan1WriteDataTemplate(void);

void ecan1WriteErrorMessage(void);

void getInPoints();

void startCountingForMessageSend(unsigned char);