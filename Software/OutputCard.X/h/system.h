/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/


/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void configureOscillator(void); /* Handles clock switching/osc initialization */

void clearIntrflags(void); /* Clears Interrupt Flag Registers */