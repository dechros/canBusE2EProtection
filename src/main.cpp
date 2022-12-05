/**
 * @file main.cpp
 * @author Halit Cetin (halitcetin@live.com)
 * @brief CanBus E2E Concept Project
 * @version 0.1
 * @date 2022-09-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <Arduino.h>
#include <FCU_E2E_CRC/FCU_E2E_CRC.h>

void setup()
{
    Serial.begin(9600);
    t_E2E_CanMessage testMessage;
    testMessage.ptrMessage = new uint8_t[TEST_CAN_MESSAGE_SIZE];
    testMessage.ptrSize = new uint32_t[1];
    testMessage.ptrCurrentCounter = new uint8_t[1];
    testMessage.u32DataID = new uint32_t[1];
    testMessage.ptrMessage[0] = TEST_CRC_VAL;
    t_gunion gunion;
    gunion.bits.bit1 = 0;
    gunion.bits.bit2 = 1;
    gunion.bits.bit3 = 0;
    gunion.bits.bit4 = 1;
    gunion.bits.bit5 = 0;
    gunion.bits.bit6 = 0;
    gunion.bits.bit7 = 0;
    gunion.bits.bit8 = 0;
    testMessage.ptrMessage[1] = gunion.byte; /* Counter */

    for (int i = 2; i < TEST_CAN_MESSAGE_SIZE; i++)
    {
        testMessage.ptrMessage[i] = 0;
    }
    *testMessage.ptrSize = TEST_CAN_MESSAGE_SIZE;
    *testMessage.ptrCurrentCounter = TEST_COUNTER_INIT_VAL;
    *testMessage.u32DataID = TEST_MESSAGE_ID;
    ten_E2E_ReturnStatus testResult = E2E_ReceiveCheck(&testMessage, u32MessageListRear, u32MessageListFront, TEST_RADAR_POSITION);
    Serial.println("E2E Result : " + String(testResult));
    delete testMessage.ptrMessage;
    delete testMessage.ptrSize;
    delete testMessage.ptrCurrentCounter;
    delete testMessage.u32DataID;
}

void loop()
{
    return;
}