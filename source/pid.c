#include "pid.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

PID* InitPID()
{
	PID*		pPID = malloc(sizeof(PID));
	
	if (pPID)
		memset(pPID, 0, sizeof(PID));
	
	pPID->startup = 1;
	
	return pPID;
}

void SetPIDSetpoint(PID* pPID, float setPt)
{
	if (! pPID)
		return;
	
	pPID->setpoint = setPt;
}

void SetPIDLimits(PID* pPID, float lwrLmt, float uprLmt)
{
	if (! pPID)
		return;
	
	pPID->lowerLimit = lwrLmt;
	pPID->upperLimit = uprLmt;
}

void SetPIDGain(PID* pPID, float p, float i, float d)
{
	if (! pPID)
		return;

	pPID->Kp = p;
	pPID->Ki = i;
	pPID->Kd = d;
}

float GetPIDOutput(PID* pPID, float input, float dTime)
{
	float		output = 0;
	float		error;
	
	if (! pPID)
		return 0;

	if (pPID->startup)
	{
		pPID->startup = 0;
		return pPID->setpoint;
	}
	
	error = pPID->setpoint - input;
	pPID->errSum += pPID->Ki*(error * dTime);
	
	if (pPID->errSum > pPID->upperLimit)
		pPID->errSum = pPID->upperLimit;
	else if (pPID->errSum < pPID->lowerLimit)
		pPID->errSum = pPID->lowerLimit;
	
	pPID->dError = fabs(dTime) < 0.00001 ? pPID->lastError : error/dTime - pPID->lastError;
	pPID->lastError = error;
	
	// Compute PID Output
	output = pPID->Kp*error + pPID->errSum + pPID->Kd*pPID->dError;
	
	// check limits
	if (output > pPID->upperLimit)
		output = pPID->upperLimit;
	else if (output < pPID->lowerLimit)
		output = pPID->lowerLimit;
	
	return output;
}




PIDI* InitPIDI()
{
	PIDI*		pPIDI = malloc(sizeof(PIDI));
	
	if (pPIDI)
		memset(pPIDI, 0, sizeof(PIDI));

	pPIDI->feedbackGain = 100;
	
	pPIDI->startup = 1;
	pPIDI->Fp = 1;
	pPIDI->Fi = 1;
	pPIDI->Fd = 1;
	
	return pPIDI;
}

void SetPIDISetpoint(PIDI* pPIDI, int setPt)
{
	if (! pPIDI)
		return;
	
	pPIDI->setpoint = setPt;
}


void SetPIDIGainFactors(PIDI* pPIDI, int Fp, int Fi, int Fd)
{
	pPIDI->Fp = Fp;
	pPIDI->Fi = Fi;
	pPIDI->Fd = Fd;
}

void SetPIDIFeedbackGain(PIDI* pPIDI, int Gfb)
{
	pPIDI->feedbackGain = Gfb;
}

void SetPIDILimits(PIDI* pPIDI, int lwrLmt, int uprLmt)
{
	if (! pPIDI)
		return;
	
	pPIDI->lowerLimit = lwrLmt;
	pPIDI->upperLimit = uprLmt;
}

void SetPIDIGain(PIDI* pPIDI, int p, int i, int d)
{
	if (! pPIDI)
		return;

	pPIDI->Kp = p;
	pPIDI->Ki = i;
	pPIDI->Kd = d;
}

int GetPIDIOutput(PIDI* pPIDI, int input, int dTime)
{
	int		output = 0;
	int		error;
	
	if (! pPIDI)
		return 0;

	if (pPIDI->startup)
	{
		pPIDI->startup = 0;
		return pPIDI->setpoint;
	}
	
	error = pPIDI->setpoint - (input * pPIDI->feedbackGain)/100;
	pPIDI->errSum += (error * dTime);
	
	// Integrator anti-windup
	if ((pPIDI->errSum / pPIDI->Fi) > pPIDI->upperLimit)
		pPIDI->errSum = pPIDI->upperLimit * pPIDI->Fi;
	else if ((pPIDI->errSum / pPIDI->Fi) < pPIDI->lowerLimit)
		pPIDI->errSum = pPIDI->lowerLimit * pPIDI->Fi;
	
	pPIDI->dError = (dTime == 0 ? 0 : error/dTime - pPIDI->lastError);
	pPIDI->lastError = error;
	
	// Compute PIDI Output
	output = ((pPIDI->Kp * error) / pPIDI->Fp) + 
			((pPIDI->Ki * pPIDI->errSum) / pPIDI->Fi) + 
			((pPIDI->Kd * pPIDI->dError) / pPIDI->Fd);
	
	// check limits
	if (output > pPIDI->upperLimit)
		output = pPIDI->upperLimit;
	else if (output < pPIDI->lowerLimit)
		output = pPIDI->lowerLimit;
	
	return output;
}