#ifndef __ETM_CAN_H
#define __ETM_CAN_H

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
#define ETM_CAN_CMD_SET_3_RX                     0b0001000000000000
#define ETM_CAN_CMD_SET_3_RX_MASK                0b0001110001111100
#define ETM_CAN_CMD_SET_2_RX                     0b0001010000000000
#define ETM_CAN_CMD_SET_2_RX_MASK                // DPARKER DEFINE
#define ETM_CAN_CMD_CMD_RX                       0b0001010010000000
#define ETM_CAN_CMD_STATUS_RX                    0b0001010100000000
#define ETM_CAN_CMD_SET_1_RX                     0b0001010110000000
#define ETM_CAN_CMD_REQUEST_RX                   0b0001011000000000
#define ETM_CAN_CMD_ERROR_RX                     0b0001011111111000

// Define TX SID VALUES
#define ETM_CAN_CMD_LVL_TX                       0b0000000000000000
#define ETM_CAN_CMD_SYNC_TX                      0b0100000000000000
#define ETM_CAN_CMD_SET_3_TX                     0b1000000000000000
#define ETM_CAN_CMD_SET_2_TX                     0b1010000000000000
#define ETM_CAN_CMD_CMD_TX                       0b1010000010000000
#define ETM_CAN_CMD_STATUS_TX                    0b1010100000000000
#define ETM_CAN_CMD_SET_1_TX                     0b1010100010000000
#define ETM_CAN_CMD_REQUEST_TX                   0b1011000000000000
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








void ETMCanInitialize(void);


#ifdef __MASTER_MODULE
//extern ETMCanMessageBuffer etm_can_rx_data_log_buffer;
#endif








#endif
