/******************* INLCUDES ******************/
#include "Terminal.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>

/******************* GLOBAL FUNCTIONS DEFINITIONS ******************/
EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
    time_t timeinstance = time(NULL);
    struct tm timeformatted = *localtime(&timeinstance);
    uint8_t transactionDate[11];
    sprintf(transactionDate, "%02d/%02d/%04d", timeformatted.tm_mday, timeformatted.tm_mon + 1, timeformatted.tm_year + 1900);
    //no need to run the checks since there is no user input here
    strcpy(termData->transactionDate, (const uint8_t*)transactionDate);
    return TERMINAL_OK;
}
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
    uint8_t MonthString[3] = "00";
    uint8_t YearString[3] = "00";
    strncpy(MonthString, &termData->transactionDate[3],2);
    strncpy(YearString, &termData->transactionDate[8],2);
    uint8_t transactionDateMonth = atoi((const uint8_t *)MonthString);
    uint8_t transactionDateYear = atoi((const uint8_t*)YearString);
    strncpy(MonthString, &cardData->cardExpirationDate[0],2);
    strncpy(YearString, &cardData->cardExpirationDate[3],2);
    uint8_t cardExpirationDateMonth = atoi((const uint8_t*)MonthString);
    uint8_t cardExpirationDateYear = atoi((const uint8_t*)YearString);
    if(transactionDateYear > cardExpirationDateYear)
    {
        return EXPIRED_CARD;
    }
    else if (transactionDateYear == cardExpirationDateYear)
    {
        if(transactionDateMonth > cardExpirationDateMonth)
        {
            return EXPIRED_CARD;
        }
        else
        {
            return TERMINAL_OK;
        }
    }
    else
    {
        return TERMINAL_OK;
    }
}
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
    float transAmount;
    scanf("%f", &transAmount);
    if(transAmount <= 0.0) 
    {
        return INVALID_AMOUNT;
    }
    termData->transAmount = transAmount;
    return TERMINAL_OK;
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
    if(termData->maxTransAmount < termData->transAmount)
    {
        return EXCEED_MAX_AMOUNT;
    }
    else
    {
        return TERMINAL_OK;
    }
}
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{
    if(maxAmount <= 0.0) 
    {
        return INVALID_MAX_AMOUNT;
    }
    termData->maxTransAmount = maxAmount;
    return TERMINAL_OK;
}