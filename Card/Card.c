/******************* INLCUDES ******************/
#include "Card.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
/******************* STATIC FUNCTIONS DEFINITIONS ******************/
static uint8_t checkDateFormat(uint8_t *testedstring)
{
    uint8_t charindex = 0;
    uint8_t stringlegth = strlen(testedstring);
    for(charindex = 0; charindex < stringlegth; charindex++)
    {
        if(charindex == 2)
        {
            if(testedstring[charindex] != '/')
            {
                return 0;
            }
        }
        else
        {
            if(!(isdigit(testedstring[charindex])))
            {
                return 0;
            }
        }
    }
    return 1;
}

static uint8_t checkStringisalpha(uint8_t *testedstring)
{
    uint8_t charindex = 0;
    uint8_t stringlegth = strlen(testedstring);
    for(charindex = 0; charindex < stringlegth; charindex++)
    {
        if(!(isalpha(testedstring[charindex]) || isspace(testedstring[charindex])))
        {
            return 0;
        }
    }
    if(!(isalpha(testedstring[0])))
    {
        return 0;
    }
    if(!(isalpha(testedstring[stringlegth - 1])))
    {
        return 0;
    }
    return 1;
}

static uint8_t checkPanFormat(uint8_t *testedstring)
{
    uint8_t charindex = 0, numericCharCount = 0;
    uint8_t stringlegth = strlen(testedstring);
    for(charindex = 0; charindex < stringlegth; charindex++)
    {
        if(!(isdigit(testedstring[charindex]) || isspace(testedstring[charindex])))
        {
            return 0;
        }
        if(isdigit(testedstring[charindex]))
        {
            numericCharCount++;
        }
    }
    if(numericCharCount != 16)
    {
        return 0;
    }
    return 1;
}

/******************* GLOBAL FUNCTIONS DEFINITIONS ******************/
EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
    uint8_t cardHolderName[50];

    gets(cardHolderName);

    if((strlen(cardHolderName) > 25) || (strlen(cardHolderName) < 20))
    {
        return WRONG_NAME;
    }
    if (!checkStringisalpha(cardHolderName))
    {
        return WRONG_NAME;
    }
    strcpy(cardData->cardHolderName, (const uint8_t*)cardHolderName);
    return CARD_OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
    uint8_t cardExpirationDate[50];
    gets(cardExpirationDate);
    if(strlen(cardExpirationDate) != 5)
    {
        return WRONG_EXP_DATE;
    }
    if(!checkDateFormat(cardExpirationDate))
    {
        return WRONG_EXP_DATE;
    }
    strcpy(cardData->cardExpirationDate, (const uint8_t*)cardExpirationDate);
    return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
    uint8_t primaryAccountNumber[50];
    gets(primaryAccountNumber);
    if((strlen(primaryAccountNumber) > 19) || (strlen(primaryAccountNumber) < 16))
    {
        return WRONG_PAN;
    }
    if(!checkPanFormat(primaryAccountNumber))
    {
        return WRONG_PAN;
    }
    strcpy(cardData->primaryAccountNumber, (const uint8_t*)primaryAccountNumber);
    return CARD_OK;
}
