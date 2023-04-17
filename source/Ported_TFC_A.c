#include "CMSIS\MKL25Z4.h"
#include "TFC\MKL25Z4.h" /* include peripheral declarations */
#include "TFC\TFC.h"
#include "core_cm0plus.h"
#include "pid.h"
#include "tpm.h"
//#include "startup_mkl25z4.c"

/* Notes:
* Steering: left is 1.0 and right is -1.0
*/

#define STEERING_MAX (2)
#define STEERING_MIN (-STEERING_MAX)
#define STEERING_CENTER (0)
#define STEERING_SERVO (1)
#define MOTOR_TPM_MOD (5)
#define FULL_THROTTLE (2.0)
#define HALF_THROTTLE (1.0)
#define CRAWL_THROTTLE (0.6)

// Variables
PID*			pSteering;
PID*			pLeftMotor;
PID*			pRightMotor;
float			leftSpeed;
float			rightSpeed;
float			currentSpeed;
float			steeringError;
float			steeringControl;
float			C; // Constant that needs to be messed with.
float			left, right;
uint32_t		time, dTime, lastTime;
unsigned long rightTime, leftTime, rightActual, leftActual;

// Used as a small delay
void smallDevDelay()
{
	for (int i = 0; i<10000; i++) {
		for (int j =0; j<30; j++) {

		}
	}
}

// Used as a big delay
void devDelay()
{
	for (int i = 0; i<10000; i++) {
		for (int j =0; j<1000; j++) {

		}
	}
}

// Used to center the steering and holds it center for a slight moment.
void go_to_center()
{
	TFC_SetServo(STEERING_SERVO, STEERING_CENTER);
	for (int i = 0; i<10000; i++) {
		for (int j =0; j<10; j++) {

		}
	}
}

// Simple steering error controller
// Works like a simple state machine that checks thresholds and reacts depending how drastic the environment.
float getSteeringErrorBasic()
{
	uint16_t leftPoint=0, rightPoint=0;


	// Grabs the first linescan image buffer.
	for (int i=0; i<128; i++) {
		if (i < 63)
			leftPoint += LineScanImage0[i];
		if (i > 64)
			rightPoint += LineScanImage0[i];
	}

	 TERMINAL_PRINTF("leftPoint: %i\t", leftPoint);
	 TERMINAL_PRINTF("rightPoint: %i\n", rightPoint);

	// Check for edge cases where the camera flip flops
	if (rightPoint <= 2000 || rightPoint >= 64500) {
		return 0.45;
	}

	if (leftPoint <= 2000 || leftPoint >= 64500) {
		return -0.45;
	}

	// Check for middle line.
	if (rightPoint >= (leftPoint - 400) && rightPoint <= (leftPoint + 400) && leftPoint >= (rightPoint - 400) && leftPoint <= (rightPoint + 400)) {
		return 0.0f;
	}

	// Check for a harshest correction.
	if (rightPoint < (leftPoint - 33000)) { // Hard left
		return 0.43f;
	}

	if (leftPoint < (rightPoint - 33000)) { // Hard Right
		return -0.43f;
	}

	// Check for a harsh correction.
	if (rightPoint < (leftPoint - 28000)) { // Hard left
		return 0.40f;
	}

	if (leftPoint < (rightPoint - 28000)) { // Hard RIghr
		return -0.40f;
	}

	// Check for a somewhat harsh correction.
	if (rightPoint < (leftPoint - 17000)) { // Hard left
		return 0.35f;
	}

	if (leftPoint < (rightPoint - 17000)) { // Hard RIghr
		return -0.35f;
	}

	// Check for a small^2 correction.
	if (rightPoint < (leftPoint - 12000)) { // Hard left
		return 0.30f;
	}

	if (leftPoint < (rightPoint - 12000)) { // Hard RIghr
		return -0.30f;
	}

	// Check for a small correction.
	if (rightPoint < (leftPoint - 7000)) { // soft left
		return 0.24f;
	}

	if (leftPoint < (rightPoint - 7000)) { // Soft right
		return -0.24f;
	}

	// Check for a smaller correction.
	if (rightPoint < (leftPoint - 3100)) { // soft left
		return 0.18f;
	}

	if (leftPoint < (rightPoint - 3100)) { // Soft right
		return -0.18f;
	}

	// Check for a smaller correction.
	if (rightPoint < (leftPoint - 500)) { // soft left
		return 0.14f;
	}

	if (leftPoint < (rightPoint - 500)) { // Soft right
		return -0.14f;
	}

	// Return no steering error.
	return 0.0f;
}

unsigned int ran = 0;

int main(void)
{
	// Init Values
	uint32_t t,i=0;


	TFC_Init();
	// .38 was the working stable speed on a mostly fresh battery
	leftSpeed = .42;
	rightSpeed = .42;
	currentSpeed = leftSpeed + rightSpeed;
	steeringError = 0;
	steeringControl = 0;
	int steeringHold = 0;
	C = 1;

	pSteering = InitPID();
	pLeftMotor = InitPID();
	pRightMotor = InitPID();

	if (!pSteering || !pLeftMotor || !pRightMotor) {
		TERMINAL_PRINTF("Not all PID controllers were initiliazed properly.\n");
		return -1;
	}

	SetPIDGain(pSteering, 2.4, 0, 0.05);
	SetPIDLimits(pSteering, STEERING_MIN, STEERING_MAX);

	SetPIDGain(pLeftMotor, 1.8, 0, 0.05);
	SetPIDGain(pRightMotor, 1.8, 0, 0.05);

	// This might have to change so that reverse can happen.
	SetPIDLimits(pLeftMotor, 0, MOTOR_TPM_MOD-1);
	SetPIDLimits(pRightMotor, 0 , MOTOR_TPM_MOD-1);

	// Set the speed for each wheel at the start to 0 and what it should be keeping it at
	SetPIDSetpoint(pLeftMotor, leftSpeed);
	SetPIDSetpoint(pRightMotor, rightSpeed);

	go_to_center();

	// Delays the start for around 6s so that everything can stabilize.
	devDelay();
	devDelay();
	devDelay();

	// operation - do this every pass through the loop
	for(;;)
	{
		//TFC_Task must be called in your main loop.  This keeps certain processing happy (I.E. Serial port queue check)
		TFC_Task();

		TFC_HBRIDGE_ENABLE;


		time = TFC_Ticker[0];

		currentSpeed = leftSpeed + rightSpeed;

		if(TFC_Ticker[0] > 300 && LineScanImageReady==1)
		{

			dTime = time - lastTime;
			lastTime = time;


			LineScanImageReady=0;

			steeringError = getSteeringErrorBasic();


			// calculate the steering error
			steeringControl = GetPIDOutput(pSteering, steeringError, dTime);
			TFC_SetServo(STEERING_SERVO, steeringControl);

			// Get speed timing from TPM and update motor control
			GetTime(&rightTime, &leftTime);

			if (rightTime > 0)
				rightActual = 19500000 / rightTime;
			else
				rightActual = 0;
			if (leftTime > 0)
				leftActual = 19500000 / leftTime;
			else
				leftActual = 0;

			// you need to adjust this output by a constant
			left = C * GetPIDOutput(pLeftMotor, leftActual, dTime);
			right = C * GetPIDOutput(pRightMotor, rightActual, dTime);


			TFC_SetMotorPWM(right, left);

		}
	}

	return 0;
}
