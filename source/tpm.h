/*
 * tpm.h
 *
 *  Created on: Jun 15, 2015
 *      Author: bob
 */

#ifndef TPM_H_
#define TPM_H_

#define FILTER_SIZE		1

void GetTime(unsigned long* t1, unsigned long* t2);
void TPM2_Init(void);


#endif /* TPM_H_ */