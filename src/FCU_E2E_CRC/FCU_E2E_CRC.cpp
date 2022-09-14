/**
 * @file FCU_E2E_CRC.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief Canbus E2E Control File
 * @version 0.1
 * @date 2022-09-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "FCU_E2E_CRC.h"

t_E2E_ListMember u32MessageListRear[E2E_REAR_RADAR_MESSAGE_LIST_SIZE] = {{0x18FFB69C, en_E2E_Message_Tx, {0, 8}, {8, 4}},
                                                                         {0x18FFA59C, en_E2E_Message_Tx, {0, 8}, {8, 4}},
                                                                         {0x18FFAB9C, en_E2E_Message_Tx, {0, 8}, {8, 4}},
                                                                         {0x18FFAC9F, en_E2E_Message_Tx, {0, 8}, {8, 4}},
                                                                         {0x0CFBF30B, en_E2E_Message_Rx, {48, 8}, {56, 4}},
                                                                         {0x18FEBF0B, en_E2E_Message_Rx, {56, 8}, {52, 4}},
                                                                         {0x18FFA30B, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                         {0x18FF980B, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                         {0x18FFAA0B, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                         {0x0CFF9D00, en_E2E_Message_Rx, {56, 8}, {52, 4}},
                                                                         {0x0CFF1200, en_E2E_Message_Rx, {56, 8}, {4, 4}},
                                                                         {0x18FFAF03, en_E2E_Message_Rx, {56, 8}, {52, 4}},
                                                                         {0x0CFFC613, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                         {0x18FF1113, en_E2E_Message_Rx, {255, 255}, {255, 255}},
                                                                         {0x18FFC521, en_E2E_Message_Rx, {255, 255}, {255, 255}},
                                                                         {0x0CFE4121, en_E2E_Message_Rx, {255, 255}, {255, 255}},
                                                                         {0x19FF7321, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                         {0x0CFFD221, en_E2E_Message_Rx, {48, 8}, {56, 4}},
                                                                         {0x18FF9353, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                         {0x063140C9, en_E2E_Message_Rx, {56, 8}, {52, 4}},
                                                                         {0x18FFB617, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                         {0x18FFAB17, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                         {0x18FFAC17, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                         {0x18F0010B, en_E2E_Message_Rx, {32, 8}, {28, 4}}};

t_E2E_ListMember u32MessageListFront[E2E_FRONT_RADAR_MESSAGE_LIST_SIZE] = {{0x18FFAC9E, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                           {0xCFBF30B, en_E2E_Message_Rx, {48, 8}, {56, 4}},
                                                                           {0x18FEBF0B, en_E2E_Message_Rx, {56, 8}, {52, 4}},
                                                                           {0x18FFA30B, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                           {0x18FF980B, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                           {0xCFF9D00, en_E2E_Message_Rx, {56, 8}, {52, 4}},
                                                                           {0xCFF1200, en_E2E_Message_Rx, {56, 8}, {4, 4}},
                                                                           {0x18FFAF03, en_E2E_Message_Rx, {56, 8}, {52, 4}},
                                                                           {0xCFFC613, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                           {0x19FF7321, en_E2E_Message_Rx, {0, 8}, {8, 4}},
                                                                           {0xCFFD221, en_E2E_Message_Rx, {48, 8}, {56, 4}},
                                                                           {0x63140C9, en_E2E_Message_Rx, {56, 8}, {52, 4}},
                                                                           {0x18FFAC17, en_E2E_Message_Rx, {0, 8}, {8, 4}}};

ten_E2E_ReturnStatus E2E_ReceiveCheck(t_E2E_CanMessage *pinout_canMessage, t_E2E_ListMember pin_u32MessageListRear[], t_E2E_ListMember pin_u32MessageListFront[], ten_RadarPosition pin_enRadarPosition)
{
    ten_E2E_ReturnStatus l_enRetVal = en_E2E_Unknown;
    if ((pinout_canMessage == NULL) || (pinout_canMessage->ptrMessage == NULL) || (pinout_canMessage->ptrSize == NULL) || (pinout_canMessage->ptrCurrentCounter == NULL) || (pinout_canMessage->u32DataID == NULL))
    {
        Serial.println("  ## E2E_ReceiveCheck: Null Parameter Error.");
    }
    else
    {
        uint8_t l_u8DeltaCounter = 0;
        uint8_t l_counter = 0;
        t_E2E_IntegrityInfo l_crcInfo = E2E_GetCrc8Info(*pinout_canMessage, u32MessageListRear, u32MessageListFront, pin_enRadarPosition);
        t_E2E_IntegrityInfo l_counterInfo = E2E_GetCounterInfo(*pinout_canMessage, u32MessageListRear, u32MessageListFront, pin_enRadarPosition);

        if (l_crcInfo.size > 8 || l_counterInfo.size > 8)
        {
            Serial.println("  ## E2E_ReceiveCheck: Counter/Crc Size Error.");
        }
        else
        {
            uint8_t l_u8Bitmask = GetBitmask(l_counterInfo.size);
            uint8_t l_u8CounterUnmasked = pinout_canMessage->ptrMessage[l_counterInfo.pos / 8];
            uint8_t l_u8Counter = l_u8CounterUnmasked & (l_u8Bitmask << l_counterInfo.pos % 8);

            if (l_u8Counter >= *pinout_canMessage->ptrCurrentCounter)
            {
                l_u8DeltaCounter = (uint8_t)(l_u8Counter - *pinout_canMessage->ptrCurrentCounter);
            }
            else
            {
                l_u8DeltaCounter = (uint8_t)(E2E_P02_MAXCOUNTER - *pinout_canMessage->ptrCurrentCounter + l_u8Counter + 1);
            }

            Serial.println("l_counterInfo.size : " + String(l_counterInfo.size));
            Serial.println("l_counterInfo.pos : " + String(l_counterInfo.pos));
            Serial.println("l_u8CounterUnmasked : " + String(l_u8CounterUnmasked));
            Serial.println("l_u8Bitmask : " + String(l_u8Bitmask));
            Serial.println("l_u8Counter : " + String(l_u8Counter));
            Serial.println("*pinout_canMessage->ptrCurrentCounter : " + String(*pinout_canMessage->ptrCurrentCounter));
            Serial.println("l_u8DeltaCounter : " + String(l_u8DeltaCounter));

            if (l_u8DeltaCounter == 0)
            {
                l_enRetVal = en_E2E_Repetition;
            }
            else if (l_u8DeltaCounter == 1)
            {
                uint8_t l_u8TempCRC = Crc_CalculateCrc8(*pinout_canMessage, l_crcInfo);
                if ((pinout_canMessage->ptrMessage[l_crcInfo.pos / 8]) == l_u8TempCRC)
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
        }
        *pinout_canMessage->ptrCurrentCounter = pinout_canMessage->ptrMessage[l_counterInfo.pos];
    }
    return l_enRetVal;
}

uint8_t GetBitmask(uint8_t size)
{
    uint8_t l_u8Bitmask = 0;
    if (size > 8)
    {
        Serial.println("  ## GetBitmask : Size error.");
    }
    else
    {
        for (uint8_t i = 0; i < size; i++)
        {
            l_u8Bitmask += pow(2, i);
        }
    }
    return l_u8Bitmask;
}

t_E2E_IntegrityInfo E2E_GetCrc8Info(t_E2E_CanMessage pin_canMessage, t_E2E_ListMember pin_u32MessageListRear[], t_E2E_ListMember pin_u32MessageListFront[], ten_RadarPosition pin_enRadarPosition)
{
    t_E2E_IntegrityInfo crc;
    crc.pos = 255;
    crc.size = 255;
    if (pin_enRadarPosition == en_RadarRear)
    {
        for (int i = 0; i < E2E_REAR_RADAR_MESSAGE_LIST_SIZE; i++)
        {
            if (*pin_canMessage.u32DataID == pin_u32MessageListRear[i].u32MessageId)
            {
                crc.pos = pin_u32MessageListRear[i].crc.pos;
                crc.size = pin_u32MessageListRear[i].crc.size;
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < E2E_FRONT_RADAR_MESSAGE_LIST_SIZE; i++)
        {
            if (*pin_canMessage.u32DataID == pin_u32MessageListFront[i].u32MessageId)
            {
                crc.pos = pin_u32MessageListFront[i].crc.pos;
                crc.size = pin_u32MessageListFront[i].crc.size;
                break;
            }
        }
    }
    return crc;
}

t_E2E_IntegrityInfo E2E_GetCounterInfo(t_E2E_CanMessage pin_canMessage, t_E2E_ListMember pin_u32MessageListRear[], t_E2E_ListMember pin_u32MessageListFront[], ten_RadarPosition pin_enRadarPosition)
{
    t_E2E_IntegrityInfo counter;
    counter.pos = 255;
    counter.size = 255;
    if (pin_enRadarPosition == en_RadarRear)
    {
        for (int i = 0; i < E2E_REAR_RADAR_MESSAGE_LIST_SIZE; i++)
        {
            if (*pin_canMessage.u32DataID == pin_u32MessageListRear[i].u32MessageId)
            {
                counter.pos = pin_u32MessageListRear[i].counter.pos;
                counter.size = pin_u32MessageListRear[i].counter.size;
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < E2E_FRONT_RADAR_MESSAGE_LIST_SIZE; i++)
        {
            if (*pin_canMessage.u32DataID == pin_u32MessageListFront[i].u32MessageId)
            {
                counter.pos = pin_u32MessageListFront[i].counter.pos;
                counter.size = pin_u32MessageListFront[i].counter.size;
                break;
            }
        }
    }
    return counter;
}

uint8_t Crc_CalculateCrc8(t_E2E_CanMessage pin_canMessage, t_E2E_IntegrityInfo pin_crc8Info)
{
    return TEST_CRC_VAL;
}