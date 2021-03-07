
#ifndef PWM_INTERPRETER_H
#define PWM_INTERPRETER_H


/****************** Include Files ********************/
#include "xil_types.h"
#include "xstatus.h"
#include "xil_io.h"

/****************** Macro Definitions ****************/
#define PWM_INTERPRETER_S00_AXI_SLV_REG0_OFFSET 0
#define PWM_INTERPRETER_S00_AXI_SLV_REG1_OFFSET 4
#define PWM_INTERPRETER_S00_AXI_SLV_REG2_OFFSET 8
#define PWM_INTERPRETER_S00_AXI_SLV_REG3_OFFSET 12

#define PWM_INTERPRETER_ENABLE_REG_OFFSET   PWM_INTERPRETER_S00_AXI_SLV_REG0_OFFSET
#define PWM_INTERPRETER_DUTY_REG_OFFSET     PWM_INTERPRETER_S00_AXI_SLV_REG1_OFFSET
#define PWM_INTERPRETER_PERIOD_REG_OFFSET   PWM_INTERPRETER_S00_AXI_SLV_REG2_OFFSET
#define PWM_INTERPRETER_ID_REG_OFFSET       PWM_INTERPRETER_S00_AXI_SLV_REG3_OFFSET

/**************************** Type Definitions *******************************/

typedef struct
{
    UINTPTR             baseaddress;
    int                 is_enabled;
    int                 is_intrenabled;
}PWMIntepreterDevice;


/**************************** Function Definitions ***************************/


/*****************************************************************************/
/**
*
* @brief    Initialized a PWMInterpreterDevice structure to be used as an
			interface by all other functions in this header file.
*
* @param	Pointer to a PWMInterpreterDevice struct to be initialized.
* @param	Base address of the device retrieved from xparameters.h.
*
* @return	XST_SUCCESS or XST_FAILURE.
*
******************************************************************************/
int PWMInterpreter_Init(PWMIntepreterDevice *InstancePtr, UINTPTR baseaddress);


int PWMInterpreter_Enable(PWMIntepreterDevice *);
int PWMInterpreter_Disable(PWMIntepreterDevice *);
int PWMInterpreter_EnableInterrupt(PWMIntepreterDevice *);
int PWMInterpreter_DisableInterrupt(PWMIntepreterDevice *);
int PWMInterpreter_ReadDutyPeriod(PWMIntepreterDevice *, uint32_t *);
int PWMInterpreter_ReadPeriod(PWMIntepreterDevice *, uint32_t *);
int PWMInterpreter_ReadID(PWMIntepreterDevice *, char* );

#endif // PWM_INTERPRETER_H
