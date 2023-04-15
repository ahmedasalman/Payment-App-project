/******************* INLCUDES ******************/
#include "Server.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
/******************* LOCAL VARIABLES DEFINITIONS ******************/
static lastaccountsIndex = 6;
static lastTransactionIndex = 0;

/******************* GLOBAL VARIABLES DEFINITIONS ******************/
ST_accountsDB_t accountsDB[255] =
{
    {2000.0, RUNNING, "8989374615436851"},
    {100000.0, BLOCKED, "5807007076043875"},
    {134.5, RUNNING, "5918451286341479"},
    {7500410.0, RUNNING, "6308142854424722"},
    {210000000.0, RUNNING, "4512565474675612"},
    {30000.0, BLOCKED, "5128653654923661"},
};
ST_transaction_t transactionDB[255] ={0};

/******************* STATIC FUNCTIONS DEFINITIONS ******************/
static printTransactionState(EN_transState_t valueToPrint)
{
    switch (valueToPrint)
    {
    case APPROVED:
        printf("APPROVED");
        break;
    case DECLINED_INSUFFECIENT_FUND:
        printf("DECLINED_INSUFFECIENT_FUND");
        break;
    case DECLINED_STOLEN_CARD:
        printf("DECLINED_STOLEN_CARD");
        break;
    case FRAUD_CARD:
        printf("FRAUD_CARD");
        break;
    case INTERNAL_SERVER_ERROR:
        printf("INTERNAL_SERVER_ERROR");
        break;
    default:
        break;
    }
}

/******************* GLOBAL FUNCTIONS DEFINITIONS ******************/
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
    ST_accountsDB_t* accountreference;
    if (isValidAccount(&transData->cardHolderData, &accountreference) == ACCOUNT_NOT_FOUND)
    {
        printf("\nThere is no account with this PAN in the DataBase");
        return FRAUD_CARD;
    }
    else
    {
        if (isBlockedAccount(accountreference) == BLOCKED_ACCOUNT)
        {
            printf("\nThis account is blocked");
            return DECLINED_STOLEN_CARD;
        }
        else
        {
            if (isAmountAvailable(&transData->terminalData, accountreference) == LOW_BALANCE)
            {
                printf("\nInsuffecient funds in account");
                return DECLINED_INSUFFECIENT_FUND;
            }
            else
            {
                accountreference->balance -= transData->terminalData.transAmount;
                printf("\nTransaction success, the remaining balance is: %f", accountreference->balance);
                return APPROVED;
            }
        }
    }
}
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t** accountRefrence)
{
    uint8_t accountsindex = 0;
    for (accountsindex = 0; accountsindex < lastaccountsIndex; accountsindex++)
    {
        if (!strcmp(cardData->primaryAccountNumber, accountsDB[accountsindex].primaryAccountNumber))
        {
            *accountRefrence = &accountsDB[accountsindex];
            return SERVER_OK;
        }
    }
    *accountRefrence = NULL;
    return ACCOUNT_NOT_FOUND;
}
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
    if (accountRefrence->state == BLOCKED)
    {
        return BLOCKED_ACCOUNT;
    }
    else
    {
        return SERVER_OK;
    }
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
    if (accountRefrence->balance < termData->transAmount)
    {
        return LOW_BALANCE;
    }
    else
    {
        return SERVER_OK;
    }
}
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
    transData->transactionSequenceNumber = transactionDB[lastTransactionIndex - 1].transactionSequenceNumber + 1;
    transactionDB[lastTransactionIndex] = *transData;
    lastTransactionIndex++;
    listSavedTransactions();
    return SERVER_OK;
}
void listSavedTransactions(void)
{
    uint8_t Transactionindex = 0;
    for (Transactionindex = 0; Transactionindex < lastTransactionIndex; Transactionindex++)
    {
        printf("\n#########################\nTransaction Sequence Number: %d", transactionDB[Transactionindex].transactionSequenceNumber);
        printf("\nTransaction Date: ");
        puts(transactionDB[Transactionindex].terminalData.transactionDate);
        printf("Transaction Amount: %f", transactionDB[Transactionindex].terminalData.transAmount);
        printf("\nTransaction State: ");
        printTransactionState(transactionDB[Transactionindex].transState);
        printf("\nTransaction Max Amount: %f", transactionDB[Transactionindex].terminalData.maxTransAmount);
        printf("\nCardholder Name: ");
        puts(transactionDB[Transactionindex].cardHolderData.cardHolderName);
        printf("PAN: ");
        puts(transactionDB[Transactionindex].cardHolderData.primaryAccountNumber);
        printf("Card Expiration Date: ");
        puts(transactionDB[Transactionindex].cardHolderData.cardExpirationDate);
    }
}