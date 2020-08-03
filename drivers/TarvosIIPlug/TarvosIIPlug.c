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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#include <errno.h>
#include "string.h"

#include "TarvosIIPlug.h"
#include "../../drivers/TarvosII/TarvosII.h"
#include "../../drivers/WE-common.h"

#define CMD_WAIT_TIME 500
#define MAX_PAYLOAD_LENGTH 255

/*
 *Initialize the TarvosIIPlug interface
 *
 *input:
 * -baudrate:  baudrate of the interface
 * -rp:        reset pin number
 * -RXcb:      RX callback function
 * -addrmode:  address mode of the TarvosIIPlug
 *
 *Caution: the parameters baudrate and addrmode must match the configured UserSettings of the TarvosIIPlug
 *         -the baudrate parameter must match to perform a successful UART communication
 *          *updating this parameter during runtime may lead to communication errors
 *         -the addrmode must match when RF packet transmission or reception is performed
 *          *this parameter can be updated to the correct value (used in TarvosIIPlug_Init function) as soon as no RF packet transmission or reception was performed
 *
 *return true if initialization succeeded
 *       false otherwise
 */
bool TarvosIIPlug_Init(int baudrate, int rp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), TarvosIIPlug_AddressMode_t addrmode)
{
    return TarvosII_Init(baudrate, rp, RXcb, addrmode);
}

/*
 *Deinitialize the TarvosIIPlug interface
 *
 *return true if deinitialization succeeded
 *       false otherwise
 */
bool TarvosIIPlug_Deinit()
{
    return TarvosII_Deinit();
}

/*
 *Reset the TarvosIIPlug by pin
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool TarvosIIPlug_PinReset()
{
    return TarvosII_PinReset();
}

/*
 *Reset the TarvosIIPlug by command
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool TarvosIIPlug_Reset()
{
    return TarvosII_Reset();
}

/*
 *Factory reset the TarvosIIPlug
 *
 *note: use them only in rare cases, since flash can be updated only a limited number times
 *
 *return true if factory reset succeeded
 *       false otherwise
 */
bool TarvosIIPlug_FactoryReset()
{
#pragma message "Using FactoryReset() is not recommended. This command sets module to transparent mode and makes driver useless!"
    return TarvosII_FactoryReset();
}

/*
 *Request the current TarvosIIPlug settings
 *
 *input:
 * -us: user setting to be requested
 *
 *output:
 * -response: pointer of the memory to put the request content
 * -response_length: length of the request content
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_Get(TarvosIIPlug_UserSettings_t us, uint8_t* response, uint8_t* response_length)
{
    return TarvosII_Get(us, response, response_length);
}

/*
 *Request multiple of the current TarvosIIPlug settings
 *
 *input:
 * -startAddress: first usersetting to be read
 * -lengthToRead: Length of memory to be read
 *
 *output:
 * -response: pointer of the memory to put the request content
 * -response_length: length of the request content
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_GetMultiple(uint8_t startAddress, uint8_t lengthToRead, uint8_t *response, uint8_t *response_length)
{
    return TarvosII_GetMultiple(startAddress, lengthToRead, response, response_length);
}

/*
 *Set a special TarvosIIPlug setting
 *
 *input:
 * -us:     user setting to be updated
 * -value:  pointer to the new settings value
 * -length: length of the value
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_Set(TarvosIIPlug_UserSettings_t us, uint8_t* value, uint8_t length)
{
    return TarvosII_Set(us, value, length);
}


/*
 *Request the 3 byte firmware version
 *
 *output:
 * -fw: pointer to the 3 byte firmware version
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_GetFirmwareVersion(uint8_t* fw)
{
    return TarvosII_GetFirmwareVersion(fw);
}

/*
 *Request the 4 byte serial number
 *
 *output:
 * -sn: pointer to the 4 byte serial number
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_GetSerialNumber(uint8_t* sn)
{
    return TarvosII_GetSerialNumber(sn);
}

/*
 *Request the default TX power
 *
 *output:
 * -txpower: pointer to the TXpower
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_GetDefaultTXPower(int8_t* txpower)
{
    return TarvosII_GetDefaultTXPower(txpower);
}

/*
 *Get the default destination address
 *
 *output:
 * -destaddr_lsb: LSB of the destination address
 * -destaddr_msb: MSB of the destination address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_GetDefaultDestAddr(uint8_t* destaddr_lsb)
{
    return TarvosII_GetDefaultDestAddr(destaddr_lsb);
}

/*
 *Get the default destination address
 *
 *output:
 * -destnetid: destination net id
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_GetDefaultDestNetID(uint8_t* destnetid)
{
    return TarvosII_GetDefaultDestNetID(destnetid);
}

/*
 *Get the default source address
 *
 *output:
 * -srcaddr_lsb: LSB of the source address
 * -srcaddr_msb: MSB of the source address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_GetSourceAddr(uint8_t* srcaddr_lsb)
{
    return TarvosII_GetSourceAddr(srcaddr_lsb);
}

/*
 *Get the default source address
 *
 *output:
 * -srcnetid: source net id
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_GetSourceNetID(uint8_t* srcnetid)
{
    return TarvosII_GetSourceNetID(srcnetid);
}

/*
 *Get the default RF channel
 *
 *output:
 * -channel: channel
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_GetDefaultRFChannel(uint8_t* channel)
{
    return TarvosII_GetDefaultRFChannel(channel);
}

/*
 *Set the default TX power
 *
 *input:
 * -txpower: TXpower
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use TarvosIIPlug_SetVolatile_TXPower for frequent adaption of the TX power
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_SetDefaultTXPower(int8_t txpower)
{
    return TarvosII_SetDefaultTXPower(txpower);
}

/*
 *Set the default destination address
 *
 *input:
 * -destaddr_lsb: LSB of the destination address
 * -destaddr_msb: MSB of the destination address
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use TarvosIIPlug_SetVolatile_DestAddr for frequent adaption of the destination address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_SetDefaultDestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb)
{
   return TarvosII_SetDefaultDestAddr(destaddr_lsb, destaddr_msb);
}

/*
 *Set the default destination address
 *
 *input:
 * -destnetid: destination net id
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use TarvosIIPlug_SetVolatile_DestNetID for frequent adaption of the destination net id
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_SetDefaultDestNetID(uint8_t destnetid)
{
    return TarvosII_SetDefaultDestNetID( destnetid);
}

/*
 *Set the default source address
 *
 *input:
 * -srcaddr_lsb: LSB of the source address
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_SetSourceAddr(uint8_t srcaddr_lsb, uint8_t srcaddr_msb)
{
    return TarvosII_SetSourceAddr(srcaddr_lsb, srcaddr_msb);
}

/*
 *Set the default source net id
 *
 *input:
 * -srcnetid: source net id
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_SetSourceNetID(uint8_t srcnetid)
{
    return TarvosII_SetSourceNetID(srcnetid);
}

/*
 *Set the default RF channel
 *
 *input:
 * -channel: channel
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use TarvosIIPlug_SetVolatile_Channel for frequent adaption of the channel
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_SetDefaultRFChannel(uint8_t channel)
{
    return TarvosII_SetDefaultRFChannel(channel);
}

/*
 *Set the volatile TX power
 *
 *input:
 * -power: new TX power value
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_SetVolatile_TXPower(int8_t power)
{
    return TarvosII_SetVolatile_TXPower(power);
}

/*
 *Set the volatile channel
 *
 *input:
 * -channel: new channel value
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_SetVolatile_Channel(uint8_t channel)
{
    return TarvosII_SetVolatile_Channel(channel);
}

/*
 *Set the volatile destination net ID
 *
 *input:
 * -destnetid: new destination net ID
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_SetVolatile_DestNetID(uint8_t destnetid)
{
    return TarvosII_SetVolatile_DestNetID(destnetid);
}

/*
 *Set the volatile destination address
 *
 *input:
 * -destaddr_lsb: lsb of the new destination address value
 * -destaddr_msb: msb of the new destination address value
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_SetVolatile_DestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb)
{
    return TarvosII_SetVolatile_DestAddr(destaddr_lsb,destaddr_msb);
}

/*
 *Transmit data using the configured settings
 *
 *input:
 * -payload: pointer to the data
 * -length: length of the data
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_Transmit(uint8_t* payload, uint8_t length)
{
    return TarvosII_Transmit(payload,length);
}

/*
 *Transmit data
 *
 *input:
 * -payload: pointer to the data
 * -length: length of the data
 * -channel: channel to be used
 * -dest_network_id: destination network ID
 * -dest_address_lsb: destination address lsb
 * -dest_address_msb: destination address msb
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIIPlug_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb)
{
    return TarvosII_Transmit_Extended(payload, length, channel, dest_network_id, dest_address_lsb);
}

/*
 *Configure the TarvosIIPlug
 *
 *input:
 * -config: pointer to the configuration struct
 * -config_length: length of the configuration struct
 * -factory_reset: apply a factory reset before or not
 *
 *return true if request succeeded
 *       false otherwise
*/
bool TarvosIIPlug_Configure(TarvosIIPlug_Configuration_t* config, uint8_t config_length, bool factory_reset)
{
    return TarvosII_Configure(config, config_length, factory_reset);
}
