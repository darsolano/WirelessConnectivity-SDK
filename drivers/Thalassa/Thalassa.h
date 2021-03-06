/**
 ***************************************************************************************************
 * This file is part of WIRELESS CONNECTIVITY SDK:
 * https://www.we-online.com/wireless-connectivity
 *
 * THE SOFTWARE INCLUDING THE SOURCE CODE IS PROVIDED “AS IS”. YOU ACKNOWLEDGE THAT WÜRTH ELEKTRONIK
 * EISOS MAKES NO REPRESENTATIONS AND WARRANTIES OF ANY KIND RELATED TO, BUT NOT LIMITED
 * TO THE NON-INFRINGEMENT OF THIRD PARTIES’ INTELLECTUAL PROPERTY RIGHTS OR THE
 * MERCHANTABILITY OR FITNESS FOR YOUR INTENDED PURPOSE OR USAGE. WÜRTH ELEKTRONIK EISOS DOES NOT
 * WARRANT OR REPRESENT THAT ANY LICENSE, EITHER EXPRESS OR IMPLIED, IS GRANTED UNDER ANY PATENT
 * RIGHT, COPYRIGHT, MASK WORK RIGHT, OR OTHER INTELLECTUAL PROPERTY RIGHT RELATING TO ANY
 * COMBINATION, MACHINE, OR PROCESS IN WHICH THE PRODUCT IS USED. INFORMATION PUBLISHED BY
 * WÜRTH ELEKTRONIK EISOS REGARDING THIRD-PARTY PRODUCTS OR SERVICES DOES NOT CONSTITUTE A LICENSE
 * FROM WÜRTH ELEKTRONIK EISOS TO USE SUCH PRODUCTS OR SERVICES OR A WARRANTY OR ENDORSEMENT
 * THEREOF
 *
 * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
 * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
 * IN THE ROOT DIRECTORY OF THIS DRIVER PACKAGE.
 *
 * COPYRIGHT (c) 2019 Würth Elektronik eiSos GmbH & Co. KG
 *
 ***************************************************************************************************
 **/

#include <stdint.h>
#include <stdbool.h>
#include "../WE-common.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _Thalassa_defined
#define _Thalassa_defined

/* Pins */
#define Thalassa_PIN_RESET  7  // LED Pin - wiringPi (pin 7 - BCM_GPIO 4).

#define Thalassa_BROADCASTADDRESS 0xFF
#define MAX_USERSETTING_LENGTH 4

typedef enum Thalassa_AddressMode_t
{
    AddressMode_0 = 0x00,
    AddressMode_1,
    AddressMode_2,
}
Thalassa_AddressMode_t;

typedef enum Thalassa_UserSettings_t
{
    Thalassa_USERSETTING_MEMPOSITION_NUMRETRYS = 0x14,
    Thalassa_USERSETTING_MEMPOSITION_DESTNETID = 0x18,
    Thalassa_USERSETTING_MEMPOSITION_DESTADDRLSB = 0x19,
    Thalassa_USERSETTING_MEMPOSITION_SOURCENETID = 0x1C,
    Thalassa_USERSETTING_MEMPOSITION_SOURCEADDRLSB = 0x1D,
    Thalassa_USERSETTING_MEMPOSITION_PAPOWER = 0x29,
    Thalassa_USERSETTING_MEMPOSITION_DEFAULTCHANNEL = 0x2A,
    Thalassa_USERSETTING_MEMPOSITION_OP_MODE = 0x3C,
    Thalassa_USERSETTING_MEMPOSITION_CFG_FLAGS = 0x48,
} Thalassa_UserSettings_t;

/*
 * Struct repesenting  a usersetting
 * members:
 * -usersetting:  usersetting
 * -value:        value
 * -value-length: length of the value
 */
typedef struct Thalassa_Configuration_t
{
    Thalassa_UserSettings_t usersetting;
    uint8_t value[MAX_USERSETTING_LENGTH];
    uint8_t value_length;
} Thalassa_Configuration_t;

extern bool Thalassa_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), Thalassa_AddressMode_t addrmode);
extern bool Thalassa_Deinit(void);

extern bool Thalassa_PinReset(void);
extern bool Thalassa_Reset(void);

extern bool Thalassa_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb);
extern bool Thalassa_Transmit(uint8_t* payload, uint8_t length);

/* read the non-volatile settings */
extern bool Thalassa_Get(Thalassa_UserSettings_t us, uint8_t* response, uint8_t* response_length);
extern bool Thalassa_GetMultiple(uint8_t startAddress, uint8_t lengthToRead, uint8_t *response, uint8_t *response_length);
extern bool Thalassa_GetSerialNumber(uint8_t* sn);
extern bool Thalassa_GetDefaultTXPower(int8_t* txpower);
extern bool Thalassa_GetSourceAddr(uint8_t* srcaddr_lsb);
extern bool Thalassa_GetSourceNetID(uint8_t* srcnetid);
extern bool Thalassa_GetDefaultDestAddr(uint8_t* destaddr_lsb);
extern bool Thalassa_GetDefaultDestNetID(uint8_t* destnetid);
extern bool Thalassa_GetDefaultRFChannel(uint8_t* channel);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool Thalassa_Set(Thalassa_UserSettings_t us, uint8_t* value, uint8_t length);
extern bool Thalassa_Configure(Thalassa_Configuration_t* config, uint8_t config_length);
extern bool Thalassa_SetDefaultTXPower(int8_t txpower);
extern bool Thalassa_SetSourceAddr(uint8_t srcaddr_lsb);
extern bool Thalassa_SetSourceNetID(uint8_t srcnetid);
extern bool Thalassa_SetDefaultDestAddr(uint8_t destaddr_lsb);
extern bool Thalassa_SetDefaultDestNetID(uint8_t destnetid);
extern bool Thalassa_SetDefaultRFChannel(uint8_t channel);

/* write volatile settings into RAM, these settings are lost after a reset */
extern bool Thalassa_SetVolatile_DestAddr(uint8_t destaddr_lsb);
extern bool Thalassa_SetVolatile_DestNetID(uint8_t destnetid);
extern bool Thalassa_SetVolatile_TXPower(int8_t power);
extern bool Thalassa_SetVolatile_RFChannel(uint8_t channel);


#endif // _Thalassa_defined
#ifdef __cplusplus
}
#endif
