

/***************************** Include Files *******************************/
#include "pwm_interpreter.h"
#include "xparameters.h"
#include 

/************************** Function Definitions ***************************/

int PWMInterpreter_Init(PWMInterpretDevice *InstancePtr, UINTPTR baseaddress)
{
	if( InstancePtr == NULL)
		return XST_FAILURE;
	
	InstancePtr->baseaddress 	= baseaddress;
	InstancePtr->isenabled		= 0;
	InstancePtr->is_intrenabled = 0;
	
	return XST_SUCCESS;
}

int PWMInterpreter_Enable(PWMInterpretDevice *InstancePtr)
{
	if( InstancePtr == NULL)
		return XST_FAILURE;
	
	u32 mask;
	mask = XilIn32(InstancePtr->baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET);
	mask |= 0x1;
	XilOut32(InstancePtr->baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET, mask);	
	
	return XST_SUCCESS;
}

int PWMInterpreter_Disable(PWMInterpretDevice *InstancePtr)
{
	if( InstancePtr == NULL)
		return XST_FAILURE;
	
	u32 mask;
	mask = XilIn32(InstancePtr->baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET);
	mask &= (0xFFFFFFFF-0x1);
	XilOut32(InstancePtr->baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET, mask);
	
	return XST_SUCCESS;
}

int PWMInterpreter_EnableInterrupt(PWMInterpretDevice *InstancePtr)
{
	if( InstancePtr == NULL)
		return XST_FAILURE;
	
	u32 mask;
	mask = XilIn32(InstancePtr->baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET);
	mask |= 0x2;
	XilOut32(InstancePtr->baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET, mask);
	
	return XST_SUCCESS;
}

int PWMInterpreter_DisableInterrupt(PWMInterpretDevice *InstancePtr)
{
	if( InstancePtr == NULL)
		return XST_FAILURE;
	
	u32 mask;
	mask = XilIn32(InstancePtr->baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET);
	mask &= (0xFFFFFFFF-0x2);
	XilOut32(InstancePtr->baseaddress + PWM_INTERPRETER_ENABLE_REG_OFFSET, mask);
	
	return XST_SUCCESS;
}

int PWMInterpreter_ReadDutyPeriod(PWMInterpretDevice *InstancePtr, uint32_t *regval)
{
	if( InstancePtr == NULL || regval == NULL)
		return XST_FAILURE;
	
	*regval = XilIn32(InstancePtr->baseaddress + PWM_INTERPRETER_DUTY_REG_OFFSET);
	
	return XST_SUCCESS;
}

int PWMInterpreter_ReadPeriod(PWMInterpretDevice *InstancePtr, uint32_t *regval)
{
	if( InstancePtr == NULL || regval == NULL)
		return XST_FAILURE;
	
	*regval = XilIn32(InstancePtr->baseaddress + PWM_INTERPRETER_PERIOD_REG_OFFSET);
	
	return XST_SUCCESS;
}

int PWMInterpreter_ReadID(PWMInterpretDevice *InstancePtr, char* str)
{
	if( InstancePtr == NULL || str == NULL)
		return XST_FAILURE;
	
	u32 raw;
	raw = XilIn32(InstancePtr->baseaddress + PWM_INTERPRETER_ID_REG_OFFSET);
	
	str[0] = (raw & 0x000000FF) >> 0;
    str[1] = (raw & 0x0000FF00)	>> 8;
    str[2] = (raw & 0x00FF0000) >> 16;
    str[3] = (raw & 0xFF000000) >> 24;	
	
	return XST_SUCCESS;
}
