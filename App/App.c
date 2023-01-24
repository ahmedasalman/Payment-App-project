/******************* INLCUDES ******************/
#include "App.h"
#include <stdio.h>

/******************* MACROS ******************/
#define MaxTerminalTransaction 15000.0

/******************* GLOBAL FUNCTIONS DEFINITIONS ******************/
void appStart(void)
{
    printf("Transaction Start");
    ST_transaction_t transData;
    printf("\nplease enter a card holder name: ");
    while (getCardHolderName(&transData.cardHolderData) != CARD_OK)
    {
        printf("\nThe Input was not valid, please enter a valid card holder name: ");
    }
    printf("\nplease enter the card expiry date: ");
    while (getCardExpiryDate(&transData.cardHolderData) != CARD_OK)
    {
        printf("\nThe Input was not valid, please enter a valid card expiry date: ");
    }
    printf("\nplease enter the card PAN: ");
    while (getCardPAN(&transData.cardHolderData) != CARD_OK)
    {
        printf("\nThe Input was not valid, please enter a valid card PAN: ");
    }
    //finished card
    printf("\ngetting the date");
    if (getTransactionDate(&transData.terminalData) != TERMINAL_OK)
    {
        printf("\nWas not able to get the date");
    }
    else
    {
        printf("\nGot the date");
    }
    if (isCardExpired(&transData.cardHolderData, &transData.terminalData) != TERMINAL_OK)
    {
        printf("\nThe card is expired");
        printf("\nEnd of transaction\n");
        return;
    }
    printf("\nplease enter the transaction amount: ");
    while (getTransactionAmount(&transData.terminalData) != TERMINAL_OK)
    {
        printf("\nThe Input was not valid, please enter a valid transaction amount: ");
    }
    setMaxAmount(&transData.terminalData, MaxTerminalTransaction);
    if (isBelowMaxAmount(&transData.terminalData) != TERMINAL_OK)
    {
        printf("\nTransaction Amount is greater than Max transaction amount for terminal");
        printf("\nEnd of transaction\n");
        return;
    }
    transData.transState = recieveTransactionData(&transData);
    saveTransaction(&transData);
    printf("\nEnd of transaction\n");
    return;
}
int main()
{
    while (1)
    {
        appStart();
    }
	return 0;
}