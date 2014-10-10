#ifndef __ETM_CAN_H
#define __ETM_CAN_H
#include "ETM_CAN_UTILITY.h"
#include "ETM_CAN_CONFIG.h"

/*
  Can Resources
  SEE : https://docs.google.com/document/d/1KNxxG_HYJT6JKNi3hfjoc4e7S5AWfiagMMd_ywYbgFg/edit
*/



/*
  X = Not Implimented (Don't Care)
  C = Command
  A = Address

  SID REGISTER

  RECEIVE MODE                                  0bXXXCCCCCCAAAAMX0
  TRANSMIT MODE                                 0bCCCCCXXXCAAAAMX0

*/




// Define RX SID Masks
// RECEIVE MODE                                  0bXXXCCCCCCAAAAMX0
#define ETM_CAN_MASTER_RX0_MASK                  0b0001100000000000
#define ETM_CAN_SLAVE_RX0_MASK                   0b0001100000000000

#define ETM_CAN_MASTER_RX1_MASK                  0b0001100000000100
#define ETM_CAN_SLAVE_RX1_MASK                   0b0001100001111100

// Define RX SID Filters
#define ETM_CAN_CMD_LVL_FILTER                   0b0000000000000000
#define ETM_CAN_CMD_SYNC_FILTER                  0b0000100000000000
#define ETM_CAN_CMD_SLAVE_FILTER                 0b0001000000000000
#define ETM_CAN_CMD_MASTER_FILTER                0b0001000000000100
#define ETM_CAN_CMD_DATA_LOG_FILTER              0b0001100000000000

// Define RX SID VALUES

#define ETM_CAN_CMD_MASTER_ADDR_MASK             0b0001111110000100
#define ETM_CAN_CMD_SLAVE_ADDR_MASK              0b0001110001111100

#define ETM_CAN_CMD_SET_3_RX                     0b0001000000000000
#define ETM_CAN_CMD_SET_2_RX                     0b0001010000000000
#define ETM_CAN_CMD_CMD_RX                       0b0001010010000000
#define ETM_CAN_CMD_STATUS_RX                    0b0001010100000000
#define ETM_CAN_CMD_SET_1_RX                     0b0001010110000000
#define ETM_CAN_CMD_REQUEST_RX                   0b0001011000000000
#define ETM_CAN_CMD_ERROR_RX                     0b0001011111111000

// Define TX SID VALUES
#define ETM_CAN_CMD_LVL_TX                       0b0000000000000000
#define ETM_CAN_CMD_SYNC_TX                      0b0100000000000000
#define ETM_CAN_CMD_SET_3_TX                     0b1000000000000000
#define ETM_CAN_CMD_SET_2_TX                     0b1010000000000100
#define ETM_CAN_CMD_CMD_TX                       0b1010000010000000
#define ETM_CAN_CMD_STATUS_TX                    0b1010100000000100
#define ETM_CAN_CMD_SET_1_TX                     0b1010100010000000
#define ETM_CAN_CMD_REQUEST_TX                   0b1011000000000000
#define ETM_CAN_CMD_DATA_LOG_TX                  0b1100000000000000
#define ETM_CAN_CMD_ERROR_TX                     0b1011100011111000





#define CXCTRL_CONFIG_MODE_VALUE                 0b0000010000000000      // This sets Fcan to 4xFcy
#define CXCTRL_OPERATE_MODE_VALUE                0b0000000000000000      // This sets Fcan to 4xFcy
#define CXCTRL_LOOPBACK_MODE_VALUE               0b0000001000000000      // This sets Fcan to 4xFcy

#define CXCFG1_10MHZ_FCY_VALUE                   0b0000000000000001      // This sets TQ to 4/Fcan
#define CXCFG1_20MHZ_FCY_VALUE                   0b0000000000000011      // This sets TQ to 8/Fcan
#define CXCFG1_25MHZ_FCY_VALUE                   0b0000000000000100      // This sets TQ to 10/Fcan

#define CXCFG2_VALUE                             0b0000001110010001      // This will created a bit timing of 10x TQ

#define CXTXXCON_VALUE_HIGH_PRIORITY             0b0000000000000011
#define CXTXXCON_VALUE_MEDIUM_PRIORITY           0b0000000000000010
#define CXTXXCON_VALUE_LOW_PRIORITY              0b0000000000000001
#define CXRXXCON_VALUE                           0b0000000000000000

#define CXTXXDLC_VALUE                           0b0000000011000000




#define ETM_CAN_ADDR_ETHERNET_BOARD              0
#define ETM_CAN_ADDR_IOP_PUMP_BOARD              1
#define ETM_CAN_ADDR_MAGNETRON_CURRENT_BOARD     2
#define ETM_CAN_ADDR_PULSE_SYNC_BOARD            3
#define ETM_CAN_ADDR_HV_LAMBDA_BOARD             4
#define ETM_CAN_ADDR_AFC_CONTROL_BOARD           5
#define ETM_CAN_ADDR_COOLING_INTERFACE_BOARD     6
#define ETM_CAN_ADDR_HEATER_MAGNET_BOARD         7
#define ETM_CAN_ADDR_GUN_DRIVER_BOARD            8
#define ETM_CAN_ADDR_ERROR                       15


typedef struct {
  unsigned int status_word_0;
  unsigned int status_word_1;
  unsigned int data_word_A;
  unsigned int data_word_B;
} ETMCanStatusRegister;

extern ETMCanStatusRegister etm_can_status_register;

typedef struct {
  unsigned int unknown_command_identifier;

  unsigned int command_not_addressed_to_this_board;
  unsigned int command_index_not_valid;
  unsigned int default_command_index_not_recognized;
  unsigned int board_specific_command_unknown;

  unsigned int set_value_not_addressed_to_this_board;
  unsigned int set_value_index_not_valid;
  unsigned int board_specific_set_value_unknown;

  unsigned int return_value_not_addressed_to_this_board;
  unsigned int return_value_index_not_valid;
  unsigned int board_specific_return_value_unknown;
} ETMCanErrorData;


typedef struct {
  unsigned int i2c_bus_error_count;
  unsigned int spi_bus_error_count;
  unsigned int can_bus_error_count;
  unsigned int scale_error_count;

  unsigned int reset_count;
  unsigned int self_test_result_register;
  unsigned int debug_0;
  unsigned int debug_1;

  unsigned int debug_2;
  unsigned int debug_3;
  unsigned int debug_4;
  unsigned int debug_5;

  unsigned int debug_6;
  unsigned int debug_7;
  unsigned int debug_8;
  unsigned int debug_9;
} ETMCanSystemDebugData;


extern ETMCanErrorData etm_can_can1_errors;
extern ETMCanSystemDebugData etm_can_system_debug_data;









void ETMCanInitialize(void);


void ETMCanSetValueBoardSpecific(ETMCanMessage* message_ptr);

extern ETMCanMessageBuffer etm_can_rx_message_buffer;
extern ETMCanMessageBuffer etm_can_tx_message_buffer;


#ifdef __MASTER_MODULE
extern ETMCanMessageBuffer etm_can_rx_data_log_buffer;
#endif



extern unsigned int etm_can_next_pulse_level;
extern unsigned int etm_can_next_pulse_count;


#ifndef __MASTER_MODULE
void ETMCanLogDefaultDebug(void);
void ETMCanLogConfig(void);

void ETMCanExecuteCMDBoardSpecific(ETMCanMessage* message_ptr);
void ETMCanReturnValueBoardSpecific(ETMCanMessage* message_ptr);

void ETMCanResetFaults(void);

void ETMCanSendStatus(void);

#endif









// Default Register Locations
#define ETM_CAN_REGISTER_DEFAULT_CMD_RESET_FAULTS                       0x0000
#define ETM_CAN_REGISTER_DEFAULT_CMD_RESET_MCU                          0x0001
#define ETM_CAN_REGISTER_DEFAULT_CMD_RESEND_CONFIG                      0x0002
#define ETM_CAN_REGISTER_DEFAULT_CMD_WRITE_EEPROM_PAGE                  0x0003
#define ETM_CAN_REGISTER_DEFAULT_CMD_DISABLE_HIGH_SPEED_DATA_LOGGING    0x0004
#define ETM_CAN_REGISTER_DEFAULT_CMD_ENABLE_HIGH_SPEED_DATA_LOGGING     0x0005

// Board Specific Register Locations

#define ETM_CAN_REGISTER_HV_LAMBDA_SET_1_LAMBDA_SET_POINT               0x4100
#define ETM_CAN_REGISTER_HV_LAMBDA_CMD_HV_ON                            0x4080
#define ETM_CAN_REGISTER_HV_LAMBDA_CMD_HV_OFF                           0x4081




#endif
