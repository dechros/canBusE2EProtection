/**
 * @file main.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief E2E CanBus Concept Project
 * @version 0.1
 * @date 2022-09-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <Arduino.h>

#define E2E_P05_MAXCOUNTER 0xFFU /* TODO: Ask to Conti */
#define E2E_INITIAL_CRC 0xFFU    /* TODO: Ask to Conti */

#define TEST_CAN_MESSAGE_SIZE 10U

typedef enum
{
    en_E2E_OK = 0,
    en_E2E_Repetition,
    en_E2E_Counter_SmallGap,
    en_E2E_Counter_BigGap,
    en_E2E_CRC_Missmatch,
    en_E2E_Unknown
} ten_E2E_ReturnStatus;

typedef struct
{
    uint8_t *ptrMessage = NULL;
    uint32_t *ptrSize = NULL;
    uint8_t *ptrCurrentCounter = NULL;
    uint8_t *u8DataID = NULL;
} t_E2E_CanMessage;

typedef struct
{
    uint8_t pos;
    uint8_t size;
} t_E2E_IntegrityInfo;

uint8_t u8TestCounter;

ten_E2E_ReturnStatus E2E_ReceiveCheck(t_E2E_CanMessage *pinout_canMessage);
t_E2E_IntegrityInfo E2E_GetCrcInfo(t_E2E_CanMessage pin_canMessage);
t_E2E_IntegrityInfo E2E_GetCounterInfo(t_E2E_CanMessage pin_canMessage);
uint8_t SCrc_CalculateCRC8(t_E2E_CanMessage pin_canMessage);

void setup()
{
    u8TestCounter = 1;
    Serial.begin(9600);
    delay(500);
}

void loop()
{
    t_E2E_CanMessage *nullMessage;
    t_E2E_CanMessage testMessage;
    testMessage.ptrMessage = new uint8_t[TEST_CAN_MESSAGE_SIZE];
    testMessage.ptrSize = new uint32_t[1];
    testMessage.ptrCurrentCounter = new uint8_t[1];
    testMessage.u8DataID = new uint8_t[1];
    for (int i = 0; i < TEST_CAN_MESSAGE_SIZE; i++)
    {
        testMessage.ptrMessage[i] = i;
    }
    *testMessage.ptrSize = TEST_CAN_MESSAGE_SIZE;
    *testMessage.ptrCurrentCounter = u8TestCounter;
    if (u8TestCounter >= 255)
    {
        u8TestCounter = 0;
    }
    u8TestCounter++;
    *testMessage.u8DataID = 52;

    Serial.println(E2E_ReceiveCheck(&testMessage));

    delete testMessage.ptrMessage;
    delete testMessage.ptrSize;
    delete testMessage.ptrCurrentCounter;
    delete testMessage.u8DataID;
}

ten_E2E_ReturnStatus E2E_ReceiveCheck(t_E2E_CanMessage *pinout_canMessage)
{
    ten_E2E_ReturnStatus l_enRetVal = en_E2E_Unknown;
    uint8_t l_u8TempCRC = E2E_INITIAL_CRC;
    uint8_t l_u8DeltaCounter = 0;

    if ((pinout_canMessage == NULL) || (pinout_canMessage->ptrMessage == NULL) || (pinout_canMessage->ptrSize == NULL) || (pinout_canMessage->ptrCurrentCounter == NULL) || (pinout_canMessage->u8DataID == NULL))
    {
        Serial.println("  ## E2E_ReceiveCheck: Null Parameter Error.");
    }
    else
    {
        if (pinout_canMessage->ptrMessage[E2E_GetCounterInfo(*pinout_canMessage).pos] >= *pinout_canMessage->ptrCurrentCounter)
        {
            l_u8DeltaCounter = (uint8_t)(pinout_canMessage->ptrMessage[E2E_GetCounterInfo(*pinout_canMessage).pos] - *pinout_canMessage->ptrCurrentCounter);
        }
        else
        {
            l_u8DeltaCounter = (uint8_t)(E2E_P05_MAXCOUNTER - *pinout_canMessage->ptrCurrentCounter + pinout_canMessage->ptrMessage[E2E_GetCounterInfo(*pinout_canMessage).pos] + 1);
        }

        if (l_u8DeltaCounter == 0)
        {
            l_enRetVal = en_E2E_Repetition;
        }
        else if (l_u8DeltaCounter == 1)
        {
            l_u8TempCRC = SCrc_CalculateCRC8(*pinout_canMessage);
            if ((uint16_t)((uint16_t)(pinout_canMessage->ptrMessage[E2E_GetCrcInfo(*pinout_canMessage).pos]) | (uint16_t)(pinout_canMessage->ptrMessage[E2E_GetCrcInfo(*pinout_canMessage).pos + 1] << E2E_GetCounterInfo(*pinout_canMessage).size)) == l_u8TempCRC)
            {
                l_enRetVal = en_E2E_OK;
            }
            else
            {
                l_enRetVal = en_E2E_CRC_Missmatch;
            }
        }
        else if (l_u8DeltaCounter == 2)
        {
            l_enRetVal = en_E2E_Counter_SmallGap;
        }
        else
        {
            l_enRetVal = en_E2E_Counter_BigGap;
        }
        *pinout_canMessage->ptrCurrentCounter = pinout_canMessage->ptrMessage[E2E_GetCounterInfo(*pinout_canMessage).pos];
    }
    return l_enRetVal;
}

t_E2E_IntegrityInfo E2E_GetCrcInfo(t_E2E_CanMessage pin_canMessage)
{
    t_E2E_IntegrityInfo crc;
    crc.pos = 3;
    crc.size = 4;
    return crc;
}

t_E2E_IntegrityInfo E2E_GetCounterInfo(t_E2E_CanMessage pin_canMessage)
{
    t_E2E_IntegrityInfo counter;
    counter.pos = 7;
    counter.size = 8;
    return counter;
}

uint8_t SCrc_CalculateCRC8(t_E2E_CanMessage pin_canMessage)
{
    return 31;
}