/**
 * @file FCU_E2E_CRC.h
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief Canbus E2E Control Header File
 * @version 0.1
 * @date 2022-09-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <Arduino.h>

#ifndef FCU_E2E_CRC_H
#define FCU_E2E_CRC_H

#define E2E_P02_MAXCOUNTER 0x0F /* TODO: Look for the document from Calin */
#define E2E_INITIAL_CRC 0xFFU   /* TODO: Ask to Conti */
#define E2E_REAR_RADAR_MESSAGE_LIST_SIZE 24

/* Test Definitions */
#define TEST_CAN_MESSAGE_SIZE 8U
#define TEST_COUNTER_INIT_VAL 9U
#define TEST_CRC_VAL 31U
#define TEST_MESSAGE_ID 0x18FFB69C

/**
 * @brief General purpose, simple bitfield
 * 
 */
typedef struct
{
    uint8_t bit1 : 1;
    uint8_t bit2 : 1;
    uint8_t bit3 : 1;
    uint8_t bit4 : 1;
    uint8_t bit5 : 1;
    uint8_t bit6 : 1;
    uint8_t bit7 : 1;
    uint8_t bit8 : 1;
} t_bitField;

/**
 * @brief General purpose, simple union
 * 
 */
typedef union
{
    t_bitField bits;
    uint8_t byte;
} t_gunion;

/**
 * @brief E2E message integrity checking results
 * 
 */
typedef enum
{
    en_E2E_OK = 0,
    en_E2E_Repetition,
    en_E2E_Counter_SmallGap,
    en_E2E_Counter_BigGap,
    en_E2E_CRC_Missmatch,
    en_E2E_Unknown
} ten_E2E_ReturnStatus;

/**
 * @brief CanBus message receive/transfer type
 * 
 */
typedef enum
{
    en_E2E_Message_Rx = 0,
    en_E2E_Message_Tx
} ten_E2E_MessageType;

/**
 * @brief CanBus message struct
 * 
 */
typedef struct
{
    uint8_t *ptrMessage = NULL;
    uint32_t *ptrSize = NULL;
    uint8_t *ptrCurrentCounter = NULL;
    uint32_t *u32DataID = NULL;
} t_E2E_CanMessage;

/**
 * @brief CanBus mesage integrity information structs
 * 
 */
typedef struct
{
    uint8_t pos;
    uint8_t size;
} t_E2E_IntegrityInfo;

/**
 * @brief Rear radar E2E message list struct
 * 
 */
typedef struct
{
    uint32_t u32MessageId;
    ten_E2E_MessageType enMessageType;
    t_E2E_IntegrityInfo crc;
    t_E2E_IntegrityInfo counter;
} t_E2E_ListMember;

/**
 * @brief Rear radar E2E message list
 * 
 */
extern t_E2E_ListMember u32MessageListRear[E2E_REAR_RADAR_MESSAGE_LIST_SIZE];

/**
 * @brief E2E message integrity checking funciton
 * 
 * @param pinout_canMessage CanBus message to be checked
 * @param pin_u32MessageListRear Rear radar E2E message list
 * @return ten_E2E_ReturnStatus E2E integrity status
 */
ten_E2E_ReturnStatus E2E_ReceiveCheck(t_E2E_CanMessage *pinout_canMessage, t_E2E_ListMember pin_u32MessageListRear[]);

/**
 * @brief Creates bitmask for selected size
 * 
 * @param size Bitmask size
 * @return uint8_t Bitmask
 */
uint8_t GetBitmask(uint8_t size);

/**
 * @brief Gets the CRC8 info from the list for selected CanBus message
 * 
 * @param pin_canMessage CanBus message to be searched
 * @param pin_u32MessageListRear Rear radar E2E message list
 * @return t_E2E_IntegrityInfo CRC8 information
 */
t_E2E_IntegrityInfo E2E_GetCrc8Info(t_E2E_CanMessage pin_canMessage, t_E2E_ListMember pin_u32MessageListRear[]);

/**
 * @brief Gets the message counter info from the list for selected CanBus message
 * 
 * @param pin_canMessage CanBus message to be searched
 * @param pin_u32MessageListRear Rear radar E2E message list
 * @return t_E2E_IntegrityInfo Message counter information
 */
t_E2E_IntegrityInfo E2E_GetCounterInfo(t_E2E_CanMessage pin_canMessage, t_E2E_ListMember pin_u32MessageListRear[]);

/**
 * @brief Dummy function that simulates calculation of CRC8
 * 
 * @param pin_canMessage No usage
 * @param pin_crc8Info No usage
 * @return uint8_t Dummy CRC8 value
 */
uint8_t Crc_CalculateCrc8(t_E2E_CanMessage pin_canMessage, t_E2E_IntegrityInfo pin_crc8Info);

#endif