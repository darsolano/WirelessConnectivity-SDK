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

#include <errno.h>
#include "string.h"

#include "TarvosIII.h"
#include "../../drivers/WE-common.h"
#include "../global/global.h"

#define CMD_WAIT_TIME 500
#define CNFINVALID 255
#define MAX_PAYLOAD_LENGTH 224
#define MAX_DATA_BUFFER 255
#define TXPOWERINVALID -128
#define RSSIINVALID -128
#define CHANNELINVALID -1

#define CMD_STX 0x02
#define TarvosIII_CMD_TYPE_REQ (0 << 6)
#define TarvosIII_CMD_TYPE_CNF (1 << 6)
#define TarvosIII_CMD_TYPE_IND (2 << 6)
#define TarvosIII_CMD_TYPE_RSP (3 << 6)

#define TarvosIII_CMD_DATA 0x00
#define TarvosIII_CMD_DATA_REQ (TarvosIII_CMD_DATA | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_DATA_CNF (TarvosIII_CMD_DATA | TarvosIII_CMD_TYPE_CNF)
#define TarvosIII_CMD_REPEAT_IND (TarvosIII_CMD_DATA | TarvosIII_CMD_TYPE_IND)

#define TarvosIII_CMD_DATAEX 0x01
#define TarvosIII_CMD_DATAEX_REQ (TarvosIII_CMD_DATAEX | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_DATAEX_IND (TarvosIII_CMD_DATAEX | TarvosIII_CMD_TYPE_IND)

#define TarvosIII_CMD_RESET 0x05
#define TarvosIII_CMD_RESET_REQ (TarvosIII_CMD_RESET | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_RESET_CNF (TarvosIII_CMD_RESET | TarvosIII_CMD_TYPE_CNF)
#define TarvosIII_CMD_RESET_IND (TarvosIII_CMD_RESET | TarvosIII_CMD_TYPE_IND)

#define TarvosIII_CMD_SET_CHANNEL 0x06
#define TarvosIII_CMD_SET_CHANNEL_REQ (TarvosIII_CMD_SET_CHANNEL | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_SET_CHANNEL_CNF (TarvosIII_CMD_SET_CHANNEL | TarvosIII_CMD_TYPE_CNF)

#define TarvosIII_CMD_SET_DESTNETID 0x07
#define TarvosIII_CMD_SET_DESTNETID_REQ (TarvosIII_CMD_SET_DESTNETID | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_SET_DESTNETID_CNF (TarvosIII_CMD_SET_DESTNETID | TarvosIII_CMD_TYPE_CNF)

#define TarvosIII_CMD_SET_DESTADDR 0x08
#define TarvosIII_CMD_SET_DESTADDR_REQ (TarvosIII_CMD_SET_DESTADDR | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_SET_DESTADDR_CNF (TarvosIII_CMD_SET_DESTADDR | TarvosIII_CMD_TYPE_CNF)

#define TarvosIII_CMD_SET 0x09
#define TarvosIII_CMD_SET_REQ (TarvosIII_CMD_SET | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_SET_CNF (TarvosIII_CMD_SET | TarvosIII_CMD_TYPE_CNF)

#define TarvosIII_CMD_GET 0x0A
#define TarvosIII_CMD_GET_REQ (TarvosIII_CMD_GET | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_GET_CNF (TarvosIII_CMD_GET | TarvosIII_CMD_TYPE_CNF)

#define TarvosIII_CMD_RSSI 0x0D
#define TarvosIII_CMD_RSSI_REQ (TarvosIII_CMD_RSSI | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_RSSI_CNF (TarvosIII_CMD_RSSI | TarvosIII_CMD_TYPE_CNF)

#define TarvosIII_CMD_SHUTDOWN 0x0E
#define TarvosIII_CMD_SHUTDOWN_REQ (TarvosIII_CMD_SHUTDOWN | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_SHUTDOWN_CNF (TarvosIII_CMD_SHUTDOWN | TarvosIII_CMD_TYPE_CNF)

#define TarvosIII_CMD_STANDBY 0x0F
#define TarvosIII_CMD_STANDBY_REQ (TarvosIII_CMD_STANDBY | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_STANDBY_CNF (TarvosIII_CMD_STANDBY | TarvosIII_CMD_TYPE_CNF)
#define TarvosIII_CMD_STANDBY_IND (TarvosIII_CMD_STANDBY | TarvosIII_CMD_TYPE_IND)

#define TarvosIII_CMD_SET_PAPOWER 0x11
#define TarvosIII_CMD_SET_PAPOWER_REQ (TarvosIII_CMD_SET_PAPOWER | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_SET_PAPOWER_CNF (TarvosIII_CMD_SET_PAPOWER | TarvosIII_CMD_TYPE_CNF)

#define TarvosIII_CMD_FACTORY_RESET 0x12
#define TarvosIII_CMD_FACTORY_RESET_REQ (TarvosIII_CMD_FACTORY_RESET | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_FACTORY_RESET_CNF (TarvosIII_CMD_FACTORY_RESET | TarvosIII_CMD_TYPE_CNF)


/* AMBER test commands */
#define TarvosIII_CMD_PINGDUT 0x1F
#define TarvosIII_CMD_PINGDUT_REQ (TarvosIII_CMD_PINGDUT | TarvosIII_CMD_TYPE_REQ)
#define TarvosIII_CMD_PINGDUT_CNF (TarvosIII_CMD_PINGDUT | TarvosIII_CMD_TYPE_CNF)

/* Masks for FLAGS */
/* Sniffer mode is indicated by bit 1 of cfg-flags */
#define TarvosIII_CFGFLAGS_SNIFFERMODEENABLE 0x0001
#define TarvosIII_RPFLAGS_REPEATERENABLE 0X0001

/* type used to check the response, when a command was sent to the TarvosIII */
typedef enum CMD_Status_t
{
    CMD_Status_Success = 0x00,
    CMD_Status_Failed,
    CMD_Status_Invalid,
    CMD_Status_Reset,
} CMD_Status_t;

typedef struct
{
    uint8_t Stx;
    uint8_t Cmd;
    uint8_t Length;
    uint8_t Data[MAX_DATA_BUFFER + 1]; /* +1 for the CS */
} CMD_Frame_t;

typedef struct
{
    uint8_t cmd;                 /* variable to check if correct CMD has been confirmed */
    CMD_Status_t status;         /* variable used to check the response (*_CNF), when a request (*_REQ) was sent to the TarvosIII */
} CMD_Confirmation_t;

/**************************************
 *     Static function declarations   *
 **************************************/

static void HandleRxPacket(uint8_t*RxBuffer);                                                      /* RX packet interpreter */
static bool Wait4CNF(int max_time_ms, uint8_t expectedCmdConfirmation, CMD_Status_t expectedStatus, bool reset_confirmstate);
static bool FillChecksum(uint8_t* array, uint8_t length);                                           /* add the CS needed to finalize the command */
static bool InitDriver(void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), TarvosIII_AddressMode_t addrmode);

/**************************************
 *          Static variables          *
 **************************************/

int reset_pin;                             /* reset pin number */
int wakeup_pin;                            /* wakeup pin number */
int boot_pin;                              /* boot pin number */
CMD_Frame_t RxPacket;                      /* data buffer for RX */
bool AbortUartRxThread;                    /* boolean to abort the UART RX thread */
bool ResetUartRxThread;                    /* boolean to reset the UART RX thread */

pthread_t thread_read;

void(*RXcallback)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t); /* callback function */

#define CMDCONFIRMATIONARRAY_LENGTH 2

CMD_Confirmation_t cmdConfirmation_array[CMDCONFIRMATIONARRAY_LENGTH];
uint8_t channelVolatile = CHANNELINVALID;           /* variable used to check if setting the channel was successfull */
uint8_t powerVolatile = TXPOWERINVALID;             /* variable used to check if setting the TXPower was successfull */
TarvosIII_AddressMode_t addressmode = AddressMode_0;  /* initial address mode */


/**************************************
 *         Static functions           *
 **************************************/

/* thread function to receive bytes from interface */
void *rx_thread()
{
    uint8_t checksum = 0;
    uint8_t RxByteCounter = 0;
    uint8_t BytesToReceive = 0;
    uint8_t readBuffer;
    static uint8_t RxBuffer[sizeof(CMD_Frame_t)]; /* data buffer for RX */

    ResetUartRxThread = false;
    AbortUartRxThread = false;

    /* apply a higher priority to this thread to be prioritized w.r.t. the main function  */
    setThreadPrio(PRIO_UARTRXTHREAD);

    while(1)
    {
        /* wait for 1ms, then check if new RX data is available */
        delay (1) ;

        if(AbortUartRxThread == true)
        {
            /* jump out of the while loop and finish the thread */
            AbortUartRxThread = false;
            break;
        }

        if(ResetUartRxThread == true)
        {
            /* reset the RX thread */
            ResetUartRxThread = false;
            RxByteCounter = 0;
        }

        while (BytesAvailable())
        {
            /* interprete received byte */
            if(ReadByte(&readBuffer))
            {
                RxBuffer[RxByteCounter] = readBuffer;

                switch (RxByteCounter)
                {
                case 0:
                    /* wait for start byte of frame */
                    if (RxBuffer[RxByteCounter] == CMD_STX)
                    {
                        BytesToReceive = 0;
                        RxByteCounter = 1;
                    }
                    break;

                case 1:
                    /* CMD */
                    RxByteCounter++;
                    break;

                case 2:
                    /* length field */
                    RxByteCounter++;
                    BytesToReceive = (RxBuffer[RxByteCounter - 1] + 4); /* len + crc + sfd + cmd */
                    break;

                default:
                    /* data field */
                    RxByteCounter++;
                    if (RxByteCounter == BytesToReceive)
                    {
                        /* check CRC */
                        checksum = 0;
                        int i = 0;
                        for (i = 0; i < (BytesToReceive - 1); i++)
                        {
                            checksum ^= RxBuffer[i];
                        }

                        if (checksum == RxBuffer[BytesToReceive - 1])
                        {
                            /* received frame ok, interprete it now */
                            HandleRxPacket(RxBuffer);
                        }

                        RxByteCounter = 0;
                        BytesToReceive = 0;
                    }
                    break;
                }
            }
        }
    }
    return 0;
}

/* interprete the valid received UART data packet */
static void HandleRxPacket(uint8_t*RxBuffer)
{
    CMD_Confirmation_t cmdConfirmation;
    cmdConfirmation.cmd = CNFINVALID;
    cmdConfirmation.status = CMD_Status_Invalid;

    uint cmd_length = RxBuffer[2];
    memcpy((uint8_t*)&RxPacket,RxBuffer,cmd_length + 4);

    switch (RxPacket.Cmd)
    {
    case TarvosIII_CMD_FACTORY_RESET_CNF:
    {
        /* check whether the module returns success */
        if ((RxPacket.Data[0] == 0x00))
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case TarvosIII_CMD_RESET_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case TarvosIII_CMD_RESET_IND:
    {
        cmdConfirmation.status = CMD_Status_Success;
        cmdConfirmation.cmd = TarvosIII_CMD_RESET_IND;
    }
    break;

    case TarvosIII_CMD_STANDBY_IND:
    {
        cmdConfirmation.status = CMD_Status_Success;
        cmdConfirmation.cmd = TarvosIII_CMD_STANDBY_IND;
    }
    break;

    case TarvosIII_CMD_SHUTDOWN_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case TarvosIII_CMD_STANDBY_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case TarvosIII_CMD_DATA_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            /* transmission success, ACK received if enabled */
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            /* transmission failed, no ACK received if enabled */
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case TarvosIII_CMD_GET_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case TarvosIII_CMD_SET_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case TarvosIII_CMD_DATAEX_IND:
    {
        /* data received, give it to the RXcallback function */
        if (RXcallback != NULL)
        {
            switch (addressmode)
            {
            case AddressMode_0:
            {
                RXcallback(&RxPacket.Data[0], RxPacket.Length - 1, TarvosIII_BROADCASTADDRESS, TarvosIII_BROADCASTADDRESS, TarvosIII_BROADCASTADDRESS, (int8_t)RxPacket.Data[RxPacket.Length-1]);
            }
            break;

            case AddressMode_1:
            {
                RXcallback(&RxPacket.Data[1], RxPacket.Length - 2, TarvosIII_BROADCASTADDRESS, RxPacket.Data[0], TarvosIII_BROADCASTADDRESS, (int8_t)RxPacket.Data[RxPacket.Length-1]);
            }
            break;

            case AddressMode_2:
            {
                RXcallback(&RxPacket.Data[2], RxPacket.Length - 3, RxPacket.Data[0], RxPacket.Data[1], TarvosIII_BROADCASTADDRESS, (int8_t)RxPacket.Data[RxPacket.Length-1]);
            }
            break;

            case AddressMode_3:
            {
                RXcallback(&RxPacket.Data[3], RxPacket.Length - 4, RxPacket.Data[0], RxPacket.Data[1], RxPacket.Data[2], (int8_t)RxPacket.Data[RxPacket.Length-1]);
            }
            break;

            default:
                /* wrong address mode */
                break;
            }
        }
    }
    break;

    case TarvosIII_CMD_SET_CHANNEL_CNF:
    {
        /* check whether the module set value of channel as requested */
        if(RxPacket.Data[0] == channelVolatile)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case TarvosIII_CMD_SET_DESTADDR_CNF:
    {
        /* check whether the module returns success */
        if (RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case TarvosIII_CMD_SET_DESTNETID_CNF:
    {
        /* check whether the module returns success */
        if(RxPacket.Data[0] == 0x00)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    case TarvosIII_CMD_SET_PAPOWER_CNF:
    {
        /* check whether the module set value of power output as requested */
        if(RxPacket.Data[0] == powerVolatile)
        {
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    /* for internal use only */
    case TarvosIII_CMD_PINGDUT_CNF:
    {
        /* check the received packets */
        if(RxPacket.Data[4] == 0x0A)
        {
            /* 10 packets received */
            cmdConfirmation.status = CMD_Status_Success;
        }
        else
        {
            /* no 10 packets received */
            cmdConfirmation.status = CMD_Status_Failed;
        }
        cmdConfirmation.cmd = RxPacket.Cmd;
    }
    break;

    default:
        break;
    }

    int i = 0;
    for(i=0; i<CMDCONFIRMATIONARRAY_LENGTH; i++)
    {
        if(cmdConfirmation_array[i].cmd == CNFINVALID)
        {
            cmdConfirmation_array[i].cmd = cmdConfirmation.cmd;
            cmdConfirmation_array[i].status = cmdConfirmation.status;
            break;
        }
    }
}

/* function that waits for the return value of TarvosIII (*_CNF), when a command (*_REQ) was sent before */
static bool Wait4CNF(int max_time_ms, uint8_t expectedCmdConfirmation, CMD_Status_t expectedStatus, bool reset_confirmstate)
{
    int count = 0;
    int time_step_ms = 5; /* 5ms */
    int max_count = max_time_ms / time_step_ms;
    int i = 0;

    if(reset_confirmstate)
    {
        for(i=0; i<CMDCONFIRMATIONARRAY_LENGTH; i++)
        {
            cmdConfirmation_array[i].cmd = CNFINVALID;
        }
    }
    while (1)
    {
        for(i=0; i<CMDCONFIRMATIONARRAY_LENGTH; i++)
        {
            if(expectedCmdConfirmation == cmdConfirmation_array[i].cmd)
            {
                return (cmdConfirmation_array[i].status == expectedStatus);
            }
        }

        if (count >= max_count)
        {
            /* received no correct response within timeout */
            return false;
        }

        /* wait */
        count++;
        delay(time_step_ms);
    }
    return true;
}

/* function to add the checksum at the end of the data packet */
static bool FillChecksum(uint8_t* array, uint8_t length)
{
    bool ret = false;

    if ((length >= 4) && (array[0] == CMD_STX))
    {
        uint8_t checksum = 0;
        uint8_t payload_length = array[2];
        int i = 0;
        for (i = 0;
                i < payload_length + 3;
                i++)
        {
            checksum ^= array[i];
        }
        array[payload_length + 3] = checksum;
        ret = true;
    }
    return ret;
}

/*
 *Initialize the TarvosIII and driver
 *
 *input:
 * -RXcb:      RX callback function
 * -addrmode:  address mode of the TarvosIII
 *
 *Caution:  *the addrmode must match when RF packet transmission or reception is performed
 *          *this parameter can be updated to the correct value (used in Init function) as soon as no RF packet transmission or reception was performed
 *
 *return true if initialization succeeded
 *       false otherwise
 */
static bool InitDriver(void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), TarvosIII_AddressMode_t addrmode)
{
    /* set address mode */
    addressmode = addrmode;

    /* set RX callback function */
    RXcallback = RXcb;

    /* start RX thread */
    if(pthread_create(&thread_read, NULL, &rx_thread, NULL))
    {
        fprintf(stdout, "Failed to start rx_Thread\n");
        TarvosIII_Deinit();
        return false;
    }

    /* reset module*/
    if(TarvosIII_PinReset())
    {
        delay(300);
    }
    else
    {
        fprintf(stdout, "Pin Reset failed\n");
        TarvosIII_Deinit();
        return false;
    }

    /* Read out the firmware version of the connected AMBER module */
    uint8_t response[3];

    if(TarvosIII_GetFirmwareVersion(response))
    {
        fprintf (stdout, "Firmware version %d.%d.%d detected\n",response[0],response[1],response[2]);
    }
    else
    {
        fprintf (stdout, "Readout of the firmware version failed.\nProbably invalid UART configuration or module is shut down.\n");
        TarvosIII_Deinit();
        return false;
    }
    return true;
}


/**************************************
 *         Global functions           *
 **************************************/

/*
 *Initialize the TarvosIII for serial interface
 *
 *input:
 * -baudrate: baudrate of the serial interface
 * -rp:       reset pin number
 * -wp:       wakeup pin number
 * -bp:       boot pin number
 * -RXcb:     RX callback function
 * -addrmode: address mode of the TarvosIII
 *
 *Caution: the parameters baudrate and addrmode must match the configured UserSettings of the TarvosIII
 *         -the baudrate parameter must match to perform a successful UART communication
 *          *updating this parameter during runtime may lead to communication errors
 *         -the addrmode must match when RF packet transmission or reception is performed
 *          *this parameter can be updated to the correct value (used in TarvosIII_Init function) as soon as no RF packet transmission or reception was performed
 *
 *return true if initialization succeeded
 *       false otherwise
 */
bool TarvosIII_Init(int baudrate, int rp, int wp, int bp, void(*RXcb)(uint8_t*,uint8_t,uint8_t,uint8_t,uint8_t,int8_t), TarvosIII_AddressMode_t addrmode)
{

    if (false == InitPin(rp))
    {
        /* error */
        return false ;
    }

    if (false == InitSerial())
    {
        /* error */
        return false ;
    }

    if (false == OpenSerial(baudrate))
    {
        /* error */
        return false ;
    }

    /* initialize the boot pin */
    boot_pin = bp;
    SetPin(boot_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);
    delay(10);

    /* initialize the wakeup pin */
    wakeup_pin = wp;
    SetPin(wakeup_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);
    delay(10);

    /* initialize the reset pin,
     * we have to define it as input/pull-up such that the reset button
     * on the base PCB can pull the pin level down if the button is pressed */
    reset_pin = rp;
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_Up, SetPin_Out_High);
    delay(10);

    /* empty the UART buffers */
    FlushSerial();

    return InitDriver(RXcb, addrmode);
}

/*
 *Deinitialize the TarvosIII interface
 *
 *return true if deinitialization succeeded
 *       false otherwise
 */
bool TarvosIII_Deinit()
{
    /* close the communication interface to the module */
    CloseSerial();
    DeinitSerial();

    /* abort RX thread */
    AbortUartRxThread = true;

    /* deinit pins */
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_No, SetPin_Out_High);
    SetPin(wakeup_pin, SetPin_InputOutput_Input, SetPin_Pull_No, SetPin_Out_High);
    SetPin(boot_pin, SetPin_InputOutput_Input, SetPin_Pull_Down, SetPin_Out_High);
    DeinitPin(reset_pin);

    /* deinit RX callback and address mode */
    RXcallback = NULL;
    addressmode = AddressMode_0;

    return true;
}

/*
 *Wakeup the TarvosIII from standby or shutdown by pin
 *
 *return true if wakeup succeeded
 *       false otherwise
 */
bool TarvosIII_PinWakeup()
{
    int i = 0;
    SetPin(wakeup_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_High);
    delay (5);
    for(i=0; i<CMDCONFIRMATIONARRAY_LENGTH; i++)
    {
        cmdConfirmation_array[i].status = CMD_Status_Invalid;
        cmdConfirmation_array[i].cmd = CNFINVALID;
    }
    SetPin(wakeup_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);

    /* wait for cnf */
    return Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_RESET_IND, CMD_Status_Success, false);
}

/*
 *Reset the TarvosIII by pin
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool TarvosIII_PinReset()
{
    /* set to output mode */
    SetPin(reset_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_High);
    delay (5);
    SetPin(reset_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_Low);
    FlushSerial();
    ResetUartRxThread = true;
    delay (5);
    SetPin(reset_pin, SetPin_InputOutput_Output, SetPin_Pull_No, SetPin_Out_High);

    /* set to input mode again */
    SetPin(reset_pin, SetPin_InputOutput_Input, SetPin_Pull_Up, SetPin_Out_High);

    /* wait for cnf */
    return Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_RESET_IND, CMD_Status_Success, true);
}

/*
 *Reset the TarvosIII by command
 *
 *return true if reset succeeded
 *       false otherwise
 */
bool TarvosIII_Reset()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[4];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = TarvosIII_CMD_RESET_REQ;
    CMD_ARRAY[2] = 0x00;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_RESET_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Factory reset the TarvosIII
 *
 *note: use them only in rare cases, since flash can be updated only a limited number times
 *
 *return true if factory reset succeeded
 *       false otherwise
 */
bool TarvosIII_FactoryReset()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[4];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = TarvosIII_CMD_FACTORY_RESET_REQ;
    CMD_ARRAY[2] = 0x00;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(1500, TarvosIII_CMD_FACTORY_RESET_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Switch the module to standby mode
 *
 *return true if switching succeeded
 *       false otherwise
 */
bool TarvosIII_Standby()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[4];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = TarvosIII_CMD_STANDBY_REQ;
    CMD_ARRAY[2] = 0x00;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_STANDBY_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Switch the module to shutdown mode
 *
 *return true if switching succeeded
 *       false otherwise
 */
bool TarvosIII_Shutdown()
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[4];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = TarvosIII_CMD_SHUTDOWN_REQ;
    CMD_ARRAY[2] = 0x00;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_SHUTDOWN_CNF, CMD_Status_Success, true);
    }
    return ret;
}

/*
 *Request the current TarvosIII settings
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
bool TarvosIII_Get(TarvosIII_UserSettings_t us, uint8_t* response, uint8_t* response_length)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = TarvosIII_CMD_GET_REQ;
    CMD_ARRAY[2] = 0x01;
    CMD_ARRAY[3] = us;

    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        if (Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_GET_CNF, CMD_Status_Success, true))
        {
            int length = RxPacket.Length - 1;
            memcpy(response,&RxPacket.Data[1],length);
            *response_length = length;
            ret = true;
        }
    }
    return ret;
}

/*
 *Set a special TarvosIII setting
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
bool TarvosIII_Set(TarvosIII_UserSettings_t us, uint8_t* value, uint8_t length)
{
    bool ret = false;

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[length + 5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = TarvosIII_CMD_SET_REQ;
    CMD_ARRAY[2] = (1 + length);
    CMD_ARRAY[3] = us;
    memcpy(&CMD_ARRAY[4],value,length);
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_SET_CNF, CMD_Status_Success, true);
    }
    return ret;
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
bool TarvosIII_GetFirmwareVersion(uint8_t* fw)
{
    uint8_t help[3];
    uint8_t help_length;

    if(TarvosIII_Get(TarvosIII_CMD_SETGET_OPTION_FWVERSION, help, &help_length))
    {
        fw[0] = help[2];
        fw[1] = help[1];
        fw[2] = help[0];
        return true;
    }
    else
    {
        return false;
    }
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
bool TarvosIII_GetSerialNumber(uint8_t* sn)
{
    uint8_t help[8];
    uint8_t help_length;

    if(TarvosIII_Get(TarvosIII_CMD_SETGET_OPTION_FACTORYSETTINGS, help, &help_length))
    {
        sn[0] = help[3];
        sn[1] = help[2];
        sn[2] = help[1];
        sn[3] = help[0];
        return true;
    }
    else
    {
        return false;
    }
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
bool TarvosIII_GetDefaultTXPower(uint8_t* txpower)
{
    uint8_t length;

    if(TarvosIII_Get(TarvosIII_CMD_SETGET_OPTION_DEFAULTRFTXPOWER, txpower, &length))
    {
        return true;
    }
    else
    {
        *txpower = TXPOWERINVALID;
        return false;
    }
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
bool TarvosIII_GetDefaultDestAddr(uint8_t* destaddr_lsb, uint8_t* destaddr_msb)
{
    /* helper array */
    uint8_t help[2];
    uint8_t length;

    if(TarvosIII_Get(TarvosIII_CMD_SETGET_OPTION_DEFAULTDESTADDR, help, &length))
    {
        *destaddr_lsb = help[0];
        *destaddr_msb = help[1];
        return true;
    }
    else
    {
        return false;
    }
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
bool TarvosIII_GetDefaultDestNetID(uint8_t* destnetid)
{
    uint8_t length;

    if(TarvosIII_Get(TarvosIII_CMD_SETGET_OPTION_DEFAULTDESTNETID, destnetid, &length))
    {
        return true;
    }
    else
    {
        return false;
    }
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
bool TarvosIII_GetSourceAddr(uint8_t* srcaddr_lsb, uint8_t *srcaddr_msb)
{
    /* helper array */
    uint8_t help[2];
    uint8_t length;

    if(TarvosIII_Get(TarvosIII_CMD_SETGET_OPTION_SOURCEADDR, help, &length))
    {
        *srcaddr_lsb = help[0];
        *srcaddr_msb = help[1];
        return true;
    }
    else
    {
        return false;
    }
}

/*
 *Set the default source net id
 *
 *output:
 * -srcnetid: source net id
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIII_GetSourceNetID(uint8_t* srcnetid)
{
    uint8_t length;

    if(TarvosIII_Get(TarvosIII_CMD_SETGET_OPTION_SOURCENETID, srcnetid, &length))
    {
        return true;
    }
    else
    {
        return false;
    }
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
bool TarvosIII_GetDefaultRFChannel(uint8_t* channel)
{
    uint8_t length;

    if(TarvosIII_Get(TarvosIII_CMD_SETGET_OPTION_DEFAULTRFCHANNEL, channel, &length))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
 *Get the default RF profile
 *
 *output:
 * -profile: RF profile
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIII_GetDefaultRFProfile(uint8_t* profile)
{
    uint8_t length;

    if(TarvosIII_Get(TarvosIII_CMD_SETGET_OPTION_DEFAULTRFPROFILE, profile, &length))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
 *Set the default TX power
 *
 *input:
 * -txpower: TXpower
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use TarvosIII_SetVolatile_TXPower for frequent adaption of the TX power
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIII_SetDefaultTXPower(uint8_t txpower)
{
    /* check for invalid power */
    if((txpower < -11)||(txpower > 15))
    {
        /*invalid power*/
        return false;
    }
    return TarvosIII_Set(TarvosIII_CMD_SETGET_OPTION_DEFAULTRFTXPOWER, &txpower, 1);
}

/*
 *Set the default destination address
 *
 *input:
 * -destaddr_lsb: LSB of the destination address
 * -destaddr_msb: MSB of the destination address
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use TarvosIII_SetVolatile_DestAddr for frequent adaption of the destination address
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIII_SetDefaultDestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb)
{

    if((destaddr_lsb < 0)||(destaddr_lsb > 255))
    {
        return false;
    }

    /* fill array */
    uint8_t help[2];
    help[0] = destaddr_lsb;
    help[1] = destaddr_msb;
    return TarvosIII_Set(TarvosIII_CMD_SETGET_OPTION_DEFAULTDESTADDR, help, 2);
}

/*
 *Set the default destination address
 *
 *input:
 * -destnetid: destination net id
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use TarvosIII_SetVolatile_DestNetID for frequent adaption of the destination net id
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIII_SetDefaultDestNetID(uint8_t destnetid)
{
    /* check for valid destnetid */
    if((destnetid < 0)||(destnetid > 255))
    {
        /* invalid destnetid */
        return false;
    }
    return TarvosIII_Set(TarvosIII_CMD_SETGET_OPTION_DEFAULTDESTNETID, &destnetid, 1);
}

/*
 *Set the default source address
 *
 *input:
 * -srcaddr_lsb: LSB of the source address
 * -srcaddr_msb: MSB of the source address
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIII_SetSourceAddr(uint8_t srcaddr_lsb, uint8_t srcaddr_msb)
{
    if((srcaddr_lsb < 0)||(srcaddr_lsb > 255))
    {
        return false;
    }
    /* fill array */
    uint8_t help[2];
    help[0] = srcaddr_lsb;
    help[1] = srcaddr_msb;
    return TarvosIII_Set(TarvosIII_CMD_SETGET_OPTION_SOURCEADDR, help, 2);
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
bool TarvosIII_SetSourceNetID(uint8_t srcnetid)
{
    /* check for invalid srcnetid */
    if((srcnetid < 0)||(srcnetid > 254))
    {
        /* invalid destnetid */
        return false;
    }
    return TarvosIII_Set(TarvosIII_CMD_SETGET_OPTION_SOURCENETID, &srcnetid, 1);
}

/*
 *Set the default RF channel
 *
 *input:
 * -channel: channel
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *note: use TarvosIII_SetVolatile_Channel for frequent adaption of the channel
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIII_SetDefaultRFChannel(uint8_t channel)
{

    /* check for valid channel */
    if((channel<100)||(channel>140))
    {
        /* invalid channel */
        return false;
    }
    return TarvosIII_Set(TarvosIII_CMD_SETGET_OPTION_DEFAULTRFCHANNEL, &channel, 1);
}

/*
 *Set the default RF profile
 *
 *input:
 * -profile: RF profile
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 *       false otherwise
 */
bool TarvosIII_SetDefaultRFProfile(uint8_t profile)
{
    return TarvosIII_Set(TarvosIII_CMD_SETGET_OPTION_DEFAULTRFPROFILE, &profile, 1);
}

/*
 *Enables the Sniffer mode
 *
 *
 *note: reset the module after the adaption of the setting such that it can take effect
 *note: use this function only in rare case, since flash can be updated only a limited number times
 *
 *return true if request succeeded
 */
bool TarvosIII_EnableSnifferMode()
{
    bool ret = false;

    uint16_t rpFlags;
    uint16_t cfgFlags;
    uint8_t length;

	ret = TarvosIII_Get(TarvosIII_CMD_SETGET_OPTION_CFG_FLAGS, (uint8_t*)&cfgFlags, &length);
    if(ret == true)
	{
		/* set sniffer mode if not set already */
		if(TarvosIII_CFGFLAGS_SNIFFERMODEENABLE != (cfgFlags & TarvosIII_CFGFLAGS_SNIFFERMODEENABLE))
		{
			cfgFlags |= TarvosIII_CFGFLAGS_SNIFFERMODEENABLE;
			ret = TarvosIII_Set(TarvosIII_CMD_SETGET_OPTION_CFG_FLAGS, (uint8_t*)&cfgFlags, 2);
		}
		else
		{
			ret = true;
		}

		if(ret == true)
		{
			/* Make sure repeater mode is disabled once sniffer mode is active. Sniffer mode and repeater mode can not be used simultaneously */
			ret = TarvosIII_Get(TarvosIII_CMD_SETGET_OPTION_RP_FLAGS, (uint8_t*)&rpFlags, &length);
			if(ret == true)
			{
				if(TarvosIII_RPFLAGS_REPEATERENABLE == (rpFlags & TarvosIII_RPFLAGS_REPEATERENABLE))
				{
					rpFlags &= ~TarvosIII_RPFLAGS_REPEATERENABLE;
					ret &= TarvosIII_Set(TarvosIII_CMD_SETGET_OPTION_RP_FLAGS, (uint8_t*)&rpFlags, 2);
				}
				else
				{
					ret = true;
				}
			}
		}
	}
    return ret;
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
bool TarvosIII_SetVolatile_TXPower(uint8_t power)
{
    bool ret = false;

    /* check for invalid power */
    if((power < -11) || (power > 15))
    {
        /*invalid power*/
        return false;
    }

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = TarvosIII_CMD_SET_PAPOWER_REQ;
    CMD_ARRAY[2] = 0x01;
    CMD_ARRAY[3] = power;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        powerVolatile = power;
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_SET_PAPOWER_CNF, CMD_Status_Success, true);
        powerVolatile = TXPOWERINVALID;
    }
    return ret;
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
bool TarvosIII_SetVolatile_Channel(uint8_t channel)
{
    bool ret = false;

    /* check for valid channel */
    if((channel<100)||(channel>140))
    {
        /* invalid channel */
        return false;
    }

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = TarvosIII_CMD_SET_CHANNEL_REQ;
    CMD_ARRAY[2] = 0x01;
    CMD_ARRAY[3] = channel;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        channelVolatile = channel;
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_SET_CHANNEL_CNF, CMD_Status_Success, true);
        channelVolatile = CHANNELINVALID;
    }
    return ret;
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
bool TarvosIII_SetVolatile_DestNetID(uint8_t destnetid)
{
    bool ret = false;

    /* check for valid destnetid */
    if((destnetid < 0)||(destnetid > 255))
    {
        /* invalid destnetid */
        return false;
    }

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[5];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = TarvosIII_CMD_SET_DESTNETID_REQ;
    CMD_ARRAY[2] = 0x01;
    CMD_ARRAY[3] = destnetid;
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_SET_DESTNETID_CNF, CMD_Status_Success, true);
    }
    return ret;
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
bool TarvosIII_SetVolatile_DestAddr(uint8_t destaddr_lsb, uint8_t destaddr_msb)
{
    bool ret = false;

    if(destaddr_lsb < 0 || destaddr_lsb > 255)
    {
        return false;
    }

    if(destaddr_msb < 0 || destaddr_msb > 255)
    {
        return false;
    }

    uint8_t CMD_ARRAY[6];

    /* fill CMD_ARRAY packet */
    switch (addressmode)
    {
    case AddressMode_0:
    case AddressMode_1:
    case AddressMode_2:
    {
        CMD_ARRAY[0] = CMD_STX;
        CMD_ARRAY[1] = TarvosIII_CMD_SET_DESTADDR_REQ;
        CMD_ARRAY[2] = 0x01;
        CMD_ARRAY[3] = destaddr_lsb;
        ret = FillChecksum(CMD_ARRAY,5);
    }
    break;
    case AddressMode_3:
    {
        CMD_ARRAY[0] = CMD_STX;
        CMD_ARRAY[1] = TarvosIII_CMD_SET_DESTADDR_REQ;
        CMD_ARRAY[2] = 0x02;
        CMD_ARRAY[3] = destaddr_lsb;
        CMD_ARRAY[4] = destaddr_msb;
        ret = FillChecksum(CMD_ARRAY,6);
    }
    break;
    default:
        return false;
    }

    if(ret == true)
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_SET_DESTADDR_CNF, CMD_Status_Success, true);
    }
    return ret;
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
bool TarvosIII_Transmit(uint8_t* payload, uint8_t length)
{
    bool ret = false;

    if(length > MAX_PAYLOAD_LENGTH)
    {
        fprintf(stdout, "Data exceeds maximal payload length\n");
        return false;
    }

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[length + 4];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = TarvosIII_CMD_DATA_REQ;
    CMD_ARRAY[2] = length;
    memcpy(&CMD_ARRAY[3],payload,length);
    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {

        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_DATA_CNF, CMD_Status_Success, true);
    }
    return ret;
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
bool TarvosIII_Transmit_Extended(uint8_t* payload, uint8_t length, uint8_t channel, uint8_t dest_network_id, uint8_t dest_address_lsb, uint8_t dest_address_msb)
{
    bool ret = false;

    if(length > MAX_PAYLOAD_LENGTH)
    {
        fprintf(stdout, "Data exceeds maximal payload length\n");
        return false;
    }

    /* fill CMD_ARRAY packet */
    uint8_t CMD_ARRAY[length + addressmode + 4 + 1];
    CMD_ARRAY[0] = CMD_STX;
    CMD_ARRAY[1] = TarvosIII_CMD_DATAEX_REQ;

    switch (addressmode)
    {
    case AddressMode_0:
    {
        CMD_ARRAY[2] = (length + 1);
        CMD_ARRAY[3] = channel;
        memcpy(&CMD_ARRAY[4],payload,length);
    }
    break;

    case AddressMode_1:
    {
        CMD_ARRAY[2] = (length + 2);
        CMD_ARRAY[3] = channel;
        CMD_ARRAY[4] = dest_address_lsb;
        memcpy(&CMD_ARRAY[5],payload,length);
    }
    break;

    case AddressMode_2:
    {
        CMD_ARRAY[2] = (length + 3);
        CMD_ARRAY[3] = channel;
        CMD_ARRAY[4] = dest_network_id;
        CMD_ARRAY[5] = dest_address_lsb;
        memcpy(&CMD_ARRAY[6],payload,length);
    }
    break;

    case AddressMode_3:
    {
        CMD_ARRAY[2] = (length + 4);
        CMD_ARRAY[3] = channel;
        CMD_ARRAY[4] = dest_network_id;
        CMD_ARRAY[5] = dest_address_lsb;
        CMD_ARRAY[6] = dest_address_msb;
        memcpy(&CMD_ARRAY[7],payload,length);
    }
    break;

    default:
        /* wrong address mode */
        return false;
    }

    if(FillChecksum(CMD_ARRAY,sizeof(CMD_ARRAY)))
    {
        /* now send CMD_ARRAY */
        SendBytes(CMD_ARRAY,sizeof(CMD_ARRAY));

        /* wait for cnf */
        ret = Wait4CNF(CMD_WAIT_TIME, TarvosIII_CMD_DATA_CNF, CMD_Status_Success, true);
    }
    return ret;
}


/*
 *Use the ping test command
 *
 *
 *note: Do not use this command. Just used for internal purpose!
 *
 *return true if request succeeded
 *       false otherwise
*/
bool TarvosIII_Ping()
{
    /* rf-profil 5, ch134, +14dbm, 10 packets */
    uint8_t ping_command[] = {0x02,0x1F,0x08,0x20,0x05,0x86,0x0E,0x0A,0xFF,0xFF,0xFF,0x4D};

    /* now send the data */
    SendBytes(ping_command,sizeof(ping_command));

    /* wait for cnf */
    return Wait4CNF(10000 /*10s*/, TarvosIII_CMD_PINGDUT_CNF, CMD_Status_Success, true);
}

/*
 *Configure the TarvosIII
 *
 *input:
 * -config: pointer to the configuration struct
 * -config_length: length of the configuration struct
 * -factory_reset: apply a factory reset before or not
 *
 *return true if request succeeded
 *       false otherwise
*/
bool TarvosIII_Configure(TarvosIII_Configuration_t* config, uint8_t config_length, bool factory_reset)
{
    int i = 0;
    uint8_t help_length;
    uint8_t help[MAX_USERSETTING_LENGTH];

    if(factory_reset)
    {
        /* perform a factory reset */
        if(false == TarvosIII_FactoryReset())
        {
            /* error */
            return false;
        }
    }
    delay(500);

    /* now check all settings and update them if necessary */
    for(i=0; i<config_length; i++)
    {
        /* read current value */
        if(false == TarvosIII_Get(config[i].usersetting, help, &help_length))
        {
            /* error */
            return false;
        }
        delay(200);

        /* check the value read out */
        if(help_length != config[i].value_length)
        {
            /* error, length does not match */
            return false;
        }
        if(memcmp(help,config[i].value,config[i].value_length) != 0)
        {
            /* read value is not up to date, thus write the new value */
            if(false == TarvosIII_Set(config[i].usersetting, config[i].value, config[i].value_length))
            {
                /* error */
                return false;
            }
        }
        delay(200);
    }

    /* reset to take effect of the updated parameters */
    if(false == TarvosIII_PinReset())
    {
        return false;
    }
    return true;
}
