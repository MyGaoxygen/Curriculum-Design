/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                             (c) Copyright 1998-2004, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
*                                            WIN32 Sample Code
*
* File : APP.C
* By   : Eric Shufro
*********************************************************************************************************
*/

#include <includes.h>
#include "conio.h"
#include "ucos_ii.h"
#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>



/*
*********************************************************************************************************
*                                                CONSTANTS
*********************************************************************************************************
*/
#define  TASK_STK_SIZE    512
//#define TASK_STACK_SIZE 1024 // 例如，设置一个合适的栈大小
//char task1_stack[TASK_STACK_SIZE]; // 使用定义的 TASK_STACK_SIZE



/*
*********************************************************************************************************
*                                                VARIABLES
*********************************************************************************************************
*/

void inputValues(int *heartRate, int *bloodPressure, int *respirationRate);
void checkValues(int heartRate, int bloodPressure, int respirationRate);
void printErrorMessage(int heartRate, int bloodPressure, int respirationRate);

// Task priorities
#define TASK_INPUT_PRIO  5
#define TASK_CHECK_PRIO  6

// Task stack sizes
#define TASK_INPUT_STK_SIZE  512
#define TASK_CHECK_STK_SIZE  512
/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/
//OS_STK hr_task_stk[TASK_STACK_SIZE];
//OS_STK bp_task_stk[TASK_STACK_SIZE];
//OS_STK br_task_stk[TASK_STACK_SIZE];
// 定义全局变量
OS_STK TaskInputStk[TASK_INPUT_STK_SIZE];
OS_STK TaskCheckStk[TASK_CHECK_STK_SIZE];

// Task prototypes
void TaskInput(void *pdata);
void TaskCheck(void *pdata);

// Global variables for user input values
int g_HeartRate = 0;
int g_BloodPressure = 0;
int g_RespirationRate = 0;
unsigned char index;
//#if OS_VIEW_MODULE > 0
//static  void  AppTerminalRx(INT8U rx_data);
//#endif

/*
*********************************************************************************************************
*                                                _tmain()
*
* Description : This is the standard entry point for C++ WIN32 code.  
* Arguments   : none
*********************************************************************************************************
*/




/*$PAGE*/
/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
* Arguments   : p_arg   is the argument passed to 'AppStartTask()' by 'OSTaskCreate()'.
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*               2) Interrupts are enabled once the task start because the I-bit of the CCR register was
*                  set to 0 by 'OSTaskCreate()'.
*********************************************************************************************************
*/

OS_EVENT *g_Mutex;

int main(void) {
    // Initialize uC/OS-II
    OSInit();

    // Create a mutex
    g_Mutex = OSMutexCreate(0, NULL);

    // Create tasks
    OSTaskCreate(TaskInput, NULL, &TaskInputStk[TASK_INPUT_STK_SIZE - 1], TASK_INPUT_PRIO);
    OSTaskCreate(TaskCheck, NULL, &TaskCheckStk[TASK_CHECK_STK_SIZE - 1], TASK_CHECK_PRIO);

    // Start uC/OS-II
    OSStart();

    return 0;
}

void inputValues(int *heartRate, int *bloodPressure, int *respirationRate) {
    printf("请输入心率、血压与呼吸值（以逗号分隔）：");
    scanf("%d,%d,%d", heartRate, bloodPressure, respirationRate);
}

void checkValues(int heartRate, int bloodPressure, int respirationRate) {
    // Check values based on priority
    if (bloodPressure > 120) 
	{
		if(heartRate > 80)
		{
			
			if(respirationRate < 20)
			{
				index = 2;//血压心率呼吸
				printErrorMessage(heartRate,bloodPressure,respirationRate);
			}
			else
			{
				index = 1;//血压心率
				printErrorMessage(heartRate,bloodPressure,respirationRate);
			}
			index = 1;//血压心率
			printErrorMessage(heartRate,bloodPressure,respirationRate);
		}
		if(heartRate < 80)
		{
			if(respirationRate < 20)
			{
				index = 3;//血压呼吸
			    printErrorMessage(heartRate,bloodPressure,respirationRate);
			}
			if(respirationRate > 20)
			{
				index = 0;//血压
			    printErrorMessage(heartRate,bloodPressure,respirationRate);
			}
		}
		   // printErrorMessage(bloodPressure);
		index = 0;//血压
		printErrorMessage(heartRate,bloodPressure,respirationRate);
    } 
	if (bloodPressure < 120)
	{
		if(heartRate > 80)
		{
			printErrorMessage(heartRate,bloodPressure,respirationRate);
			if(respirationRate < 20)
			{
				index = 5;//心率呼吸
				printErrorMessage(heartRate,bloodPressure,respirationRate);
			}
			index = 4;//心率
			printErrorMessage(heartRate,bloodPressure,respirationRate);
		}
		if(heartRate < 80)
		{
			if(respirationRate < 20)
			{
				index = 6;//呼吸
				printErrorMessage(heartRate,bloodPressure,respirationRate);
			}
			if(respirationRate > 20)//no
			{
				index = 7;//wu
				printErrorMessage(heartRate,bloodPressure,respirationRate);
			}
		}
	}

}

void printErrorMessage(int heartRate, int bloodPressure, int respirationRate) {
    // Print error message
	if(index == 0)
    printf("血压异常：%d，心率正常：%d，呼吸正常：%d\n", heartRate,bloodPressure,respirationRate);
	if(index == 1)
    printf("血压异常：%d，心率异常：%d，呼吸正常：%d\n", heartRate,bloodPressure,respirationRate);
	if(index == 2)
    printf("血压异常：%d，心率异常：%d，呼吸异常：%d\n", heartRate,bloodPressure,respirationRate);
	if(index == 3)
    printf("血压异常：%d，心率正常：%d，呼吸异常：%d\n", heartRate,bloodPressure,respirationRate);
	if(index == 4)
    printf("血压正常：%d，心率异常：%d，呼吸正常：%d\n", heartRate,bloodPressure,respirationRate);
	if(index == 5)
    printf("血压正常：%d，心率异常：%d，呼吸异常：%d\n", heartRate,bloodPressure,respirationRate);
	if(index == 6)
    printf("血压正常：%d，心率正常：%d，呼吸异常：%d\n", heartRate,bloodPressure,respirationRate);
	if(index == 7)
    printf("血压正常：%d，心率正常：%d，呼吸正常：%d\n", heartRate,bloodPressure,respirationRate);
}

// Task for getting user input
void TaskInput(void *pdata) {
    INT8U err;
    while (1) {
        OSMutexPend(g_Mutex, 0, &err); // Acquire mutex
        inputValues(&g_HeartRate, &g_BloodPressure, &g_RespirationRate);
        OSMutexPost(g_Mutex); // Release mutex
        OSTimeDlyHMSM(0, 0, 5, 0); // Delay for 5 seconds
    }
}

// Task for checking values
void TaskCheck(void *pdata) {
    INT8U err;
    while (1) {
        OSMutexPend(g_Mutex, 0, &err); // Acquire mutex
        checkValues(g_HeartRate, g_BloodPressure, g_RespirationRate);
        OSMutexPost(g_Mutex); // Release mutex
        OSTimeDlyHMSM(0, 0, 1, 0); // Delay for 1 second
    }
}