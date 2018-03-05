#ifndef _MHAL_PQL_H
#define _MHAL_PQL_H

#include "mhal_pql_datatype.h"

void Mhal_PQL_SetQuality(void);
void mapi_pql_customer_Init(void);
void Mhal_PQL_Handler(MS_BOOL bRealTimeMonitorOnly);
void Mhal_PQL_UpdatePicture(mhal_pql_ColorTemperature_t *pstPQLColorTemperature);
MS_U16 Mhal_PQL_GetPQDelayTime(void);
void Mhal_PQL_SetColorCorrectTbl(MS_S16 *pColorCorrectTbl);

#endif
