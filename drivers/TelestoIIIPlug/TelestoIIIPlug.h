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

#include "../../drivers/TelestoIII/TelestoIII.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Pins */
#define TelestoIIIPlug_CBUS_RESET PIN_FTDI_CBUS0

#define TelestoIIIPlug_BROADCASTADDRESS 0xFF

#define TelestoIIIPlug_AddressMode_t                       TelestoIII_AddressMode_t
#define TelestoIIIPlug_UserSettings_t                      TelestoIII_UserSettings_t
#define TelestoIIIPlug_Configuration_t                     TelestoIII_Configuration_t

#define TelestoIIIPlug_CMD_SETGET_OPTION_FULLACCESS            TelestoIII_CMD_SETGET_OPTION_FULLACCESS
#define TelestoIIIPlug_CMD_SETGET_OPTION_UARTBAUDRATE          TelestoIII_CMD_SETGET_OPTION_UARTBAUDRATE
#define TelestoIIIPlug_CMD_SETGET_OPTION_DEFAULTRFPROFILE      TelestoIII_CMD_SETGET_OPTION_DEFAULTRFPROFILE
#define TelestoIIIPlug_CMD_SETGET_OPTION_DEFAULTRFTXPOWER      TelestoIII_CMD_SETGET_OPTION_DEFAULTRFTXPOWER
#define TelestoIIIPlug_CMD_SETGET_OPTION_DEFAULTRFCHANNEL      TelestoIII_CMD_SETGET_OPTION_DEFAULTRFCHANNEL
#define TelestoIIIPlug_CMD_SETGET_OPTION_DEFAULTADDRESSMODE    TelestoIII_CMD_SETGET_OPTION_DEFAULTADDRESSMODE
#define TelestoIIIPlug_CMD_SETGET_OPTION_NUMRETRYS             TelestoIII_CMD_SETGET_OPTION_NUMRETRYS
#define TelestoIIIPlug_CMD_SETGET_OPTION_DEFAULTDESTNETID      TelestoIII_CMD_SETGET_OPTION_DEFAULTDESTNETID
#define TelestoIIIPlug_CMD_SETGET_OPTION_DEFAULTDESTADDR       TelestoIII_CMD_SETGET_OPTION_DEFAULTDESTADDR
#define TelestoIIIPlug_CMD_SETGET_OPTION_SOURCENETID           TelestoIII_CMD_SETGET_OPTION_SOURCENETID
#define TelestoIIIPlug_CMD_SETGET_OPTION_SOURCEADDR            TelestoIII_CMD_SETGET_OPTION_SOURCEADDR
#define TelestoIIIPlug_CMD_SETGET_OPTION_OP_MODE               TelestoIII_CMD_SETGET_OPTION_OP_MODE
#define TelestoIIIPlug_CMD_SETGET_OPTION_CFG_FLAGS             TelestoIII_CMD_SETGET_OPTION_CFG_FLAGS
#define TelestoIIIPlug_CMD_SETGET_OPTION_RP_FLAGS              TelestoIII_CMD_SETGET_OPTION_RP_FLAGS
#define TelestoIIIPlug_CMD_SETGET_OPTION_RP_NUMSLOTS           TelestoIII_CMD_SETGET_OPTION_RP_NUMSLOTS
#define TelestoIIIPlug_CMD_SETGET_OPTION_FACTORYSETTINGS       TelestoIII_CMD_SETGET_OPTION_FACTORYSETTINGS
#define TelestoIIIPlug_CMD_SETGET_OPTION_FWVERSION             TelestoIII_CMD_SETGET_OPTION_FWVERSION
#define TelestoIIIPlug_CMD_SETGET_OPTION_RUNTIMESETTINGS       TelestoIII_CMD_SETGET_OPTION_RUNTIMESETTINGS


extern bool TelestoIIIPlug_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), TelestoIIIPlug_AddressMode_t addrmode);
extern bool TelestoIIIPlug_Deinit(void);

extern bool TelestoIIIPlug_PinReset(void);
extern bool TelestoIIIPlug_Reset(void);

extern bool TelestoIIIPlug_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb, uint8_t dest_address_msb);
extern bool TelestoIIIPlug_Transmit(uint8_t* payload, uint8_t length);

/* read the non-volatile settings */
extern bool TelestoIIIPlug_Get(TelestoIIIPlug_UserSettings_t us, uint8_t* response, uint8_t* response_length);
extern bool TelestoIIIPlug_GetFirmwareVersion(uint8_t* fw);
extern bool TelestoIIIPlug_GetSerialNumber(uint8_t* sn);
extern bool TelestoIIIPlug_GetDefaultTXPower(uint8_t* txpower);
extern bool TelestoIIIPlug_GetSourceAddr(uint8_t* srcaddr_lsb, uint8_t* srcaddr_msb);
extern bool TelestoIIIPlug_GetSourceNetID(uint8_t* srcnetid);
extern bool TelestoIIIPlug_GetDefaultDestAddr(uint8_t* destaddr_lsb, uint8_t* srcaddr_msb);
extern bool TelestoIIIPlug_GetDefaultDestNetID(uint8_t* destnetid);
extern bool TelestoIIIPlug_GetDefaultRFChannel(uint8_t* channel);
extern bool TelestoIIIPlug_GetDefaultRFProfile(uint8_t* profile);

/* functions that write the non-volatile settings in the flash,
 * after modification of any non-volatile setting, the module must be reset such that the update takes effect
 * IMPORTANT: use them only in rare cases, since flash can be updated only a limited number times
 */
extern bool TelestoIIIPlug_FactoryReset(void);
extern bool TelestoIIIPlug_Set(TelestoIIIPlug_UserSettings_t us, uint8_t* value, uint8_t length);
extern bool TelestoIIIPlug_Configure(TelestoIIIPlug_Configuration_t* config, uint8_t config_length, bool factory_reset);
extern bool TelestoIIIPlug_SetDefaultTXPower(uint8_t txpower);
extern bool TelestoIIIPlug_SetSourceAddr(uint8_t srcaddr_lsb, uint8_t srcaddr_msb);
extern bool TelestoIIIPlug_SetSourceNetID(uint8_t srcnetid);
extern bool TelestoIIIPlug_SetDefaultDestAddr(uint8_t destaddr_lsb, uint8_t srcaddr_msb);
extern bool TelestoIIIPlug_SetDefaultDestNetID(uint8_t destnetid);
extern bool TelestoIIIPlug_SetDefaultRFChannel(uint8_t channel);
extern bool TelestoIIIPlug_SetDefaultRFProfile(uint8_t profile);
extern bool TelestoIIIPlug_EnableSnifferMode();

/* write volatile settings into RAM, these settings are lost after a reset */
extern bool TelestoIIIPlug_SetVolatile_DestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb);
extern bool TelestoIIIPlug_SetVolatile_DestNetID(uint8_t destnetid);
extern bool TelestoIIIPlug_SetVolatile_TXPower(uint8_t power);
extern bool TelestoIIIPlug_SetVolatile_Channel(uint8_t channel);

extern bool TelestoIIIPlug_SendUART(uint8_t* payload, uint8_t length, uint16_t waittime);

#ifdef __cplusplus
}
#endif
