#include "derivative.h" /* include peripheral declarations */
#include "tpm.h"
#include <string.h>


volatile unsigned long		ch1Time, ch2Time;
volatile unsigned long		lastCh1Time, lastCh2Time;
volatile unsigned long		ch1Ovf, ch2Ovf, ovf;
volatile unsigned long		lastCh1Ovf, lastCh2Ovf;
volatile unsigned long		testOvf = 0;

volatile unsigned long		buf1[FILTER_SIZE], buf2[FILTER_SIZE];
volatile short				ptr1, ptr2;
volatile short				count1, count2;

void FTM2_IRQHandler(void)
{
	unsigned short	status = TPM2_STATUS;
	unsigned long 	t;
	
	if (status & 0x101 || status & 0x102)
		testOvf++;
	
	if (status & 0x01)
	{
		lastCh1Time = ch1Time;
		ch1Time = TPM2_C0V;
		lastCh1Ovf = ch1Ovf;
		ch1Ovf = ovf;

		t = ch1Ovf - lastCh1Ovf;
		t <<= 16;
		t += ch1Time;
		t -= lastCh1Time;
		
		buf1[ptr1++] = t;
		if (ptr1 >= FILTER_SIZE)
			ptr1 = 0;
		
		if (count1 < FILTER_SIZE)
			count1++;
	}

	if (status & 0x02)
	{
		lastCh2Time = ch2Time;
		ch2Time = TPM2_C1V;
		lastCh2Ovf = ch2Ovf;
		ch2Ovf = ovf;

		t = ch2Ovf - lastCh2Ovf;
		t <<= 16;
		t += ch2Time;
		t -= lastCh2Time;
		buf2[ptr2++] = t;
		
		if (ptr2 >= FILTER_SIZE)
			ptr2 = 0;
		
		if (count2 < FILTER_SIZE)
			count2++;
	}

	if (status & 0x100)
		ovf++;
	
	TPM2_STATUS = 0xffff;
}

void GetTime(unsigned long* t1, unsigned long* t2)
{
	short			i;
	unsigned long	f1 = 0, f2 = 0;
	
	if (count1 >= FILTER_SIZE && count2 >= FILTER_SIZE)
	{
		for (i = 0; i < FILTER_SIZE; i++)
		{
			f1 += buf1[i] / FILTER_SIZE;
			f2 += buf2[i] / FILTER_SIZE;
		}
	}
	else
	{
		for (i = 0; i < count1; i++)
			f1 += buf1[i] / count1;
			
		for (i = 0; i < count2; i++)
			f2 += buf2[i] / count2;
	}
	
	*t1 = f1;
	*t2 = f2;
}

void TPM2_Init(void)
{

	ch1Time = 0;
	ch2Time = 0;
	lastCh1Time = 0;
	lastCh2Time = 0;

	ovf = 0;
	ch1Ovf = 0;
	ch2Ovf = 0;
	lastCh1Ovf = 0;
	lastCh2Ovf = 0;

	ptr1 = 0;
	ptr2 = 0;
	count1 = 0;
	count2 = 0;
	
	memset(buf1, 0, sizeof(buf1));
	memset(buf2, 0, sizeof(buf2));
	
	SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;
	TPM2_SC = (TPM_SC_CMOD(0x00) | TPM_SC_PS(0x00)); /* Clear status and control register */
	
	TPM2_CNT = TPM_CNT_COUNT(0x00);      /* Reset counter register */
	TPM2_C0SC = 0x00U;                   /* Clear channel status and control register */
	TPM2_C1SC = 0x00U;                   /* Clear channel status and control register */
	
	/* TPM2_MOD: MOD=0xFFFF */
	TPM2_MOD = TPM_MOD_MOD(0xFFFF);      /* Set up modulo register */
	
	/* TPM2_C1SC: CHF=0,CHIE=1,MSB=0,MSA=0,ELSB=1,ELSA=0,??=0,DMA=0 */
	TPM2_C1SC = (TPM_CnSC_CHIE_MASK | TPM_CnSC_ELSB_MASK); /* Set up channel status and control register */
	/* TPM2_C0SC: CHF=0,CHIE=1,MSB=0,MSA=0,ELSB=1,ELSA=0,??=0,DMA=0 */
	TPM2_C0SC = (TPM_CnSC_CHIE_MASK | TPM_CnSC_ELSB_MASK); /* Set up channel status and control register */

	
	// /* PORTA_PCR2: ISF=0,MUX=3 */
	// PORTA_PCR2 = (uint32_t)((PORTA_PCR2 & (uint32_t)~(uint32_t)(PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x04))) | (uint32_t)(PORT_PCR_MUX(0x03)));
	// /* PORTA_PCR1: ISF=0,MUX=3 */
	// PORTA_PCR1 = (uint32_t)((PORTA_PCR1 & (uint32_t)~(uint32_t)(PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x04))) | (uint32_t)(PORT_PCR_MUX(0x03)));
	// //DeviceDataPrv->EnEvents = 0x03U;     /* Enable selected events */
	
	/* NVIC_IPR4: PRI_19=0x80 */
	NVIC_IPR4 = (uint32_t)((NVIC_IPR4 & (uint32_t)~(uint32_t)(
		   NVIC_IP_PRI_19(0x7F)
		  )) | (uint32_t)(
		   NVIC_IP_PRI_19(0x80)
		  ));
	
	NVIC_ISER |= NVIC_ISER_SETENA(0x00080000);
	/* TPM2_SC: DMA=0,TOF=0,TOIE=1,CPWMS=0,CMOD=1,PS=3 */
	// TPM2_SC = (TPM_SC_TOIE_MASK) | (TPM_SC_CMOD(0x01) | TPM_SC_PS(0x07));

}
