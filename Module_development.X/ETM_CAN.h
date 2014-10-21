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
#define ETM_CAN_MASTER_RX0_MASK                  0b0001100000000001
#define ETM_CAN_MASTER_RX1_MASK                  0b0001100000000101

#define ETM_CAN_SLAVE_RX0_MASK                   0b0001100000000001
#define ETM_CAN_SLAVE_RX1_MASK                   0b0001100001111101

// Define RX SID Filters
#define ETM_CAN_MSG_LVL_FILTER                   0b0000000000000000
#define ETM_CAN_MSG_SYNC_FILTER                  0b0000100000000000
#define ETM_CAN_MSG_SLAVE_FILTER                 0b0001000000000000
#define ETM_CAN_MSG_MASTER_FILTER                0b0001000000000100
#define ETM_CAN_MSG_DATA_LOG_FILTER              0b0001100000000000
#define ETM_CAN_MSG_FILTER_OFF                   0b0001011111111100


// Define RX SID VALUES
#define ETM_CAN_MSG_MASTER_ADDR_MASK             0b0001111110000100
#define ETM_CAN_MSG_SLAVE_ADDR_MASK              0b0001110001111100

#define ETM_CAN_MSG_SET_3_RX                     0b0001000000000000
#define ETM_CAN_MSG_SET_2_RX                     0b0001010000000100
#define ETM_CAN_MSG_CMD_RX                       0b0001010010000000
#define ETM_CAN_MSG_STATUS_RX                    0b0001010100000100
#define ETM_CAN_MSG_SET_1_RX                     0b0001010110000000
#define ETM_CAN_MSG_REQUEST_RX                   0b0001011000000000


// Define TX SID VALUES
#define ETM_CAN_MSG_LVL_TX                       0b0000000000000000
#define ETM_CAN_MSG_SYNC_TX                      0b0100000000000000
#define ETM_CAN_MSG_SET_3_TX                     0b1000000000000000
#define ETM_CAN_MSG_SET_2_TX                     0b1010000000000100
#define ETM_CAN_MSG_CMD_TX                       0b1010000010000000
#define ETM_CAN_MSG_STATUS_TX                    0b1010100000000100
#define ETM_CAN_MSG_SET_1_TX                     0b1010100010000000
#define ETM_CAN_MSG_REQUEST_TX                   0b1011000000000000
#define ETM_CAN_MSG_DATA_LOG_TX                  0b1100000000000000
//#define ETM_CAN_MSG_ERROR_TX                     0b1011100011111000




// Can configuration parameters
//#define CXCTRL_CONFIG_MODE_VALUE                 0b0000010000000000      // This sets Fcan to 4xFcy
//#define CXCTRL_OPERATE_MODE_VALUE                0b0000000000000000      // This sets Fcan to 4xFcy
//#define CXCTRL_LOOPBACK_MODE_VALUE               0b0000001000000000      // This sets Fcan to 4xFcy

#define CXCTRL_CONFIG_MODE_VALUE                 0b0000110000000000      // This sets Fcan to 1xFcy
#define CXCTRL_OPERATE_MODE_VALUE                0b0000100000000000      // This sets Fcan to 1xFcy
#define CXCTRL_LOOPBACK_MODE_VALUE               0b0000101000000000      // This sets Fcan to 1xFcy

#define CXCFG1_10MHZ_FCY_VALUE                   0b0000000000000100      // This sets TQ to 4/Fcan


//#define CXCFG1_10MHZ_FCY_VALUE                   0b0000000000000001      // This sets TQ to 4/Fcan
#define CXCFG1_20MHZ_FCY_VALUE                   0b0000000000000011      // This sets TQ to 8/Fcan
#define CXCFG1_25MHZ_FCY_VALUE                   0b0000000000000100      // This sets TQ to 10/Fcan

#define CXCFG2_VALUE                             0b0000010110100011      // This will created a bit timing of 12x TQ

//#define CXCFG2_VALUE                             0b0000001110010001      // This will created a bit timing of 10x TQ

#define CXTXXCON_VALUE_HIGH_PRIORITY             0b0000000000000011
#define CXTXXCON_VALUE_MEDIUM_PRIORITY           0b0000000000000010
#define CXTXXCON_VALUE_LOW_PRIORITY              0b0000000000000001
#define CXRXXCON_VALUE                           0b0000000000000000

#define CXTXXDLC_VALUE                           0b0000000011000000





typedef struct {
  unsigned int status_word_0;
  unsigned int status_word_1;
  unsigned int data_word_A;
  unsigned int data_word_B;
} ETMCanStatusRegister;


typedef struct {
  unsigned int i2c_bus_error_count;
  unsigned int spi_bus_error_count;
  unsigned int can_bus_error_count;
  unsigned int scale_error_count;

  unsigned int reset_count;
  unsigned int self_test_result_register;
  unsigned int reserved_0;
  unsigned int reserved_1;

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
  unsigned int debug_A;
  unsigned int debug_B;

  unsigned int debug_C;
  unsigned int debug_D;
  unsigned int debug_E;
  unsigned int debug_F;
  
  unsigned int unknown_message_identifier;
  unsigned int message_index_does_not_match_board;
  unsigned int message_not_addressed_to_this_board;
  unsigned int update_status_unknown_board;

  unsigned int can_module_error_flag;
  unsigned int command_index_not_valid;
  unsigned int default_command_index_not_recognized;
  unsigned int board_specific_command_unknown;

  unsigned int set_value_index_not_valid;
  unsigned int board_specific_set_value_unknown;
  unsigned int return_value_index_not_valid;
  unsigned int board_specific_return_value_unknown;

  unsigned int can_commands_processed;

} ETMCanSystemDebugData;



typedef struct {
  unsigned int pulse_count;
  unsigned int high_low_energy;
  
  unsigned int hvlambda_readback_high_energy_lambda_program_voltage;
  unsigned int hvlambda_readback_low_energy_lambda_program_voltage;
  unsigned int hvlambda_readback_peak_lambda_voltage;

  unsigned int afc_readback_current_position;
  unsigned int afc_readback_target_position;
  unsigned int afc_readback_a_input;
  unsigned int afc_readback_b_input;
  unsigned int afc_readback_filtered_error_reading;

  unsigned int ionpump_readback_high_energy_target_current_reading;
  unsigned int ionpump_readback_low_energy_target_current_reading;

  unsigned int magmon_readback_magnetron_high_energy_current;
  unsigned int magmon_readback_magnetron_low_energy_current;
  unsigned int magmon_readback_arc_detected;

  unsigned char psync_readback_customer_trigger_width;
  unsigned char psync_readback_filtered_customer_trigger_width;
  unsigned char psync_readback_high_energy_grid_width;
  unsigned char psync_readback_high_energy_grid_delay;
  unsigned char psync_readback_low_energy_grid_width;
  unsigned char psync_readback_low_energy_grid_delay;
  
} ETMCanHighSpeedData;



typedef struct {
  // ------------------- HV LAMBDA BOARD --------------------- //
  // Standard Registers for all Boards
  ETMCanStatusRegister hvlamdba_status_data; 
  ETMCanSystemDebugData hvlambda_debug_data;
  
  // Values that the Ethernet control board sets on HV Lambda
  unsigned int hvlambda_high_energy_set_point;
  unsigned int hvlambda_low_energy_set_point;

  // "SLOW" Data that the Ethernet control board reads back from HV Lambda
  unsigned int hvlambda_readback_high_energy_set_point;
  unsigned int hvlambda_readback_low_energy_set_point;
  unsigned int hvlambda_readback_base_plate_temp;




  // ------------------- ION PUMP BOARD --------------------- //
  // Standard Registers for all Boards
  ETMCanStatusRegister ionpump_status_data; 
  ETMCanSystemDebugData ionpump_debug_data;
  
  // Values that the Ethernet control board sets 
  // NONE!!!!
  
  // "SLOW" Data that the Ethernet control board reads back
  unsigned int ionpump_readback_ion_pump_volage_monitor;
  unsigned int ionpump_readback_ion_pump_current_monitor;
  unsigned int ionpump_readback_filtered_high_energy_target_current;
  unsigned int ionpump_readback_filtered_low_energy_target_current;


  // -------------------- AFC CONTROL BOARD ---------------//
  // Standard Registers for all Boards
  ETMCanStatusRegister afc_status_data; 
  ETMCanSystemDebugData afc_debug_data;
  
  // Values that the Ethernet control board sets 
  unsigned int afc_home_position;
  int          afc_offset;
    
  // "SLOW" Data that the Ethernet control board reads back
  unsigned int afc_readback_home_position;
  unsigned int afc_readback_offset;
  unsigned int afc_readback_current_position;

  // -------------------- COOLING INTERFACE BOARD ---------------//
  // Standard Registers for all Boards
  ETMCanStatusRegister cool_status_data; 
  ETMCanSystemDebugData cool_debug_data;
  
  // Values that the Ethernet control board sets 
  // NONE!!!!
    
  // "SLOW" Data that the Ethernet control board reads back
  unsigned int cool_readback_hvps_coolant_flow;
  unsigned int cool_readback_magnetron_coolant_flow;
  unsigned int cool_readback_linac_coolant_flow;
  unsigned int cool_readback_circulator_coolant_flow;
  unsigned int cool_readback_spare_word_0;
  unsigned int cool_readback_spare_word_1;
  unsigned int cool_readback_hx_coolant_flow;
  unsigned int cool_readback_spare_coolant_flow;
  unsigned int cool_readback_coolant_temperature;
  unsigned int cool_readback_sf6_pressure;
  unsigned int cool_readback_cabinet_temperature;
  unsigned int cool_readback_linac_temperature;
  
  // -------------------- HEATER/MAGNET INTERFACE BOARD ---------------//
  // Standard Registers for all Boards
  ETMCanStatusRegister htrmag_status_data; 
  ETMCanSystemDebugData htrmag_debug_data;
  
  // Values that the Ethernet control board sets 
  unsigned int htrmag_magnet_current_set_point;
  unsigned int htrmag_heater_current_set_point;
      
  // "SLOW" Data that the Ethernet control board reads back
  unsigned int htrmag_readback_heater_current;
  unsigned int htrmag_readback_heater_voltage;
  unsigned int htrmag_readback_magnet_current;
  unsigned int htrmag_readback_magnet_voltage;
  unsigned int htrmag_readback_heater_current_set_point;
  unsigned int htrmag_readback_heater_voltage_set_point;
  unsigned int htrmag_readback_magnet_current_set_point;
  unsigned int htrmag_readback_magnet_voltage_set_point;
  
  
  // -------------------- GUN DRIVER INTERFACE BOARD ---------------//
  // Standard Registers for all Boards
  ETMCanStatusRegister gun_status_data; 
  ETMCanSystemDebugData gun_debug_data;
  
  // Values that the Ethernet control board sets 
  unsigned int gun_high_energy_pulse_top_voltage_set_point;
  unsigned int gun_low_energy_pulse_top_voltage_set_point;
  unsigned int gun_heater_voltage_set_point;
  unsigned int gun_cathode_voltage_set_point;
  
  // "SLOW" Data that the Ethernet control board reads back
  unsigned int gun_readback_high_energy_pulse_top_voltage_monitor;
  unsigned int gun_readback_low_energy_pulse_top_voltage_monitor;
  unsigned int gun_readback_cathode_voltage_monitor;
  unsigned int gun_readback_peak_beam_current;
  unsigned int gun_readback_heater_voltage_monitor;
  unsigned int gun_readback_heater_current_monitor;
  unsigned int gun_readback_heater_time_delay_remaining;
  unsigned int gun_readback_driver_temperature;
  unsigned int gun_readback_high_energy_pulse_top_set_point;
  unsigned int gun_readback_low_energy_pulse_top_set_point;
  unsigned int gun_readback_heater_voltage_set_point;
  unsigned int gun_readback_cathode_voltage_set_point;


  // -------------------- MAGNETRON CURRENT MONITOR BOARD ---------------//
  // Standard Registers for all Boards
  ETMCanStatusRegister magmon_status_data; 
  ETMCanSystemDebugData magmon_debug_data;
  
  // Values that the Ethernet control board sets 
  // NONE!!!!
  
  // "SLOW" Data that the Ethernet control board reads back
  unsigned int magmon_readback_spare;
  unsigned int magmon_readback_arcs_this_hv_on;
  unsigned int magmon_filtered_high_energy_pulse_current;
  unsigned int magmon_filtered_low_energy_pulse_current;
  unsigned long magmon_arcs_lifetime;
  unsigned long magmon_pulses_this_hv_on;
  unsigned long long magmon_pulses_lifetime;

  // -------------------- PULSE SYNC BOARD ---------------//
  // Standard Registers for all Boards
  ETMCanStatusRegister psync_status_data; 
  ETMCanSystemDebugData psync_debug_data;
  
  // Values that the Ethernet control board sets 
  unsigned char psync_grid_delay_high_intensity_3;
  unsigned char psync_grid_delay_high_intensity_2;
  unsigned char psync_grid_delay_high_intensity_1;
  unsigned char psync_grid_delay_high_intensity_0;
  unsigned char psync_pfn_delay_high;
  unsigned char psync_rf_delay_high;

  unsigned char psync_grid_width_high_intensity_3;
  unsigned char psync_grid_width_high_intensity_2;
  unsigned char psync_grid_width_high_intensity_1;
  unsigned char psync_grid_width_high_intensity_0;
  unsigned char psync_afc_delay_high;
  unsigned char psync_spare_delay_high;

  unsigned char psync_grid_delay_low_intensity_3;
  unsigned char psync_grid_delay_low_intensity_2;
  unsigned char psync_grid_delay_low_intensity_1;
  unsigned char psync_grid_delay_low_intensity_0;
  unsigned char psync_pfn_delay_low;
  unsigned char psync_rf_delay_low;
 
  unsigned char psync_grid_width_low_intensity_3;
  unsigned char psync_grid_width_low_intensity_2;
  unsigned char psync_grid_width_low_intensity_1;
  unsigned char psync_grid_width_low_intensity_0;
  unsigned char psync_afc_delay_low;
  unsigned char psync_spare_delay_low;

  unsigned int  psync_customer_led;

  // "SLOW" Data that the Ethernet control board reads back
  // NONE!!!!!!

} ETMCanDataStructure;





// Public Buffers
extern ETMCanMessageBuffer etm_can_rx_message_buffer;
extern ETMCanMessageBuffer etm_can_tx_message_buffer;
#ifdef __ETM_CAN_MASTER_MODULE
extern ETMCanMessageBuffer etm_can_rx_data_log_buffer;
#endif


// Public Variables
extern unsigned int etm_can_next_pulse_level;
extern unsigned int etm_can_next_pulse_count;
#ifdef __ETM_CAN_MASTER_MODULE
extern ETMCanDataStructure etm_can_slave_data;
#else
extern unsigned int etm_can_high_speed_data_logging_enabled;
#endif

// Public Debug and Status registers
extern ETMCanSystemDebugData etm_can_system_debug_data;
extern ETMCanStatusRegister etm_can_status_register;



// Public Functions
void ETMCanInitialize(void);
void ETMCanProcessMessage(void);
void ETMCanSetValueBoardSpecific(ETMCanMessage* message_ptr);

#ifdef __ETM_CAN_MASTER_MODULE
void ETMCanUpdateStatusBoardSpecific(ETMCanMessage* message_ptr);
#else
void ETMCanResetFaults(void);
void ETMCanSendStatus(void);
void ETMCanLogData(unsigned char packet_id, unsigned int word0, unsigned int word1, unsigned int word2, unsigned int word3);
void ETMCanLogDefaultDebug(void);
void ETMCanLogConfig(void);
void ETMCanExecuteCMDBoardSpecific(ETMCanMessage* message_ptr);
void ETMCanReturnValueBoardSpecific(ETMCanMessage* message_ptr);
#endif











//------------------------------- P1298 Specific Board and Command Defines -------------------------- // 

#define ETM_CAN_ADDR_ETHERNET_BOARD                                     14
#define ETM_CAN_ADDR_ION_PUMP_BOARD                                     1
#define ETM_CAN_ADDR_MAGNETRON_CURRENT_BOARD                            2
#define ETM_CAN_ADDR_PULSE_SYNC_BOARD                                   3
#define ETM_CAN_ADDR_HV_LAMBDA_BOARD                                    4
#define ETM_CAN_ADDR_AFC_CONTROL_BOARD                                  5
#define ETM_CAN_ADDR_COOLING_INTERFACE_BOARD                            6
#define ETM_CAN_ADDR_HEATER_MAGNET_BOARD                                7
#define ETM_CAN_ADDR_GUN_DRIVER_BOARD                                   8



// Default Register Locations
#define ETM_CAN_REGISTER_DEFAULT_CMD_RESET_FAULTS                       0x000
#define ETM_CAN_REGISTER_DEFAULT_CMD_RESET_MCU                          0x001
#define ETM_CAN_REGISTER_DEFAULT_CMD_RESEND_CONFIG                      0x002
#define ETM_CAN_REGISTER_DEFAULT_CMD_WRITE_EEPROM_PAGE                  0x003
#define ETM_CAN_REGISTER_DEFAULT_CMD_DISABLE_HIGH_SPEED_DATA_LOGGING    0x004
#define ETM_CAN_REGISTER_DEFAULT_CMD_ENABLE_HIGH_SPEED_DATA_LOGGING     0x005

// Default Calibration Locations
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN0                       0x400 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN1                       0x402 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN2                       0x404 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN3                       0x406 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN4                       0x408 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN5                       0x40A 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN6                       0x40C 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN7                       0x40E 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN8                       0x410 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN9                       0x412 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN10                      0x414 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN11                      0x416 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN12                      0x418 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN13                      0x41A 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN14                      0x41C 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_AN15                      0x41E 

#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN0                       0x420 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN1                       0x422 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN2                       0x424 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN3                       0x426 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN4                       0x428 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN5                       0x42A 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN6                       0x42C 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN7                       0x42E 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN8                       0x430 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN9                       0x432 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN10                      0x434 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN11                      0x436 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN12                      0x438 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN13                      0x43A 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN14                      0x43C 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_AN15                      0x43E 

#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_DAC0                      0x440 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_DAC1                      0x442 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_DAC2                      0x444 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_DAC3                      0x446 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_DAC4                      0x448
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_DAC5                      0x44A 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_DAC6                      0x44C 
#define ETM_CAN_CALIBRATION_REGISTER_INTERNAL_DAC7                      0x44E 

#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_DAC0                      0x460 
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_DAC1                      0x462
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_DAC2                      0x464
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_DAC3                      0x466
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_DAC4                      0x468
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_DAC5                      0x46A
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_DAC6                      0x46C
#define ETM_CAN_CALIBRATION_REGISTER_EXTERNAL_DAC7                      0x46E

#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_0_1                      0x480
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_2_3                      0x482
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_4_5                      0x484
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_6_7                      0x486
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_8_9                      0x488
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_10_11                    0x48A
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_12_13                    0x48C
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_14_15                    0x48E
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_16_17                    0x490
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_18_19                    0x492
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_20_21                    0x494
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_22_23                    0x496
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_24_25                    0x498
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_26_27                    0x49A
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_28_29                    0x49C
#define ETM_CAN_CALIBRATION_REGISTER_PARAMETER_30_31                    0x49E







// Board Specific Register Locations
#define ETM_CAN_REGISTER_HV_LAMBDA_SET_1_LAMBDA_SET_POINT               0x4100
#define ETM_CAN_REGISTER_HV_LAMBDA_CMD_HV_ON                            0x4080
#define ETM_CAN_REGISTER_HV_LAMBDA_CMD_HV_OFF                           0x4081

#define ETM_CAN_REGISTER_AFC_SET_1_HOME_POSITION                        0x5100
#define ETM_CAN_REGISTER_AFC_SET_1_AFC_OFFSET                           0x5101
#define ETM_CAN_REGISTER_AFC_CMD_DO_AUTO_ZERO                           0x5080
#define ETM_CAN_REGISTER_AFC_CMD_ENTER_AFC_MODE                         0x5081
#define ETM_CAN_REGISTER_AFC_CMD_ENTER_MANUAL_MODE                      0x5082
#define ETM_CAN_REGISTER_AFC_CMD_SET_TARGET_POSITION                    0x5083
#define ETM_CAN_REGISTER_AFC_CMD_DO_RELATIVE_MOVE                       0x5084


#define ETM_CAN_REGISTER_COOLING_CMD_OPEN_SF6_SOLENOID_RELAY            0x6080
#define ETM_CAN_REGISTER_COOLING_CMD_CLOSE_SF6_SOLENOID_RELAY           0x6081


#define ETM_CAN_REGISTER_MAGNET_FILAMENT_SET_1_CURRENT_SET_POINT        0x7100
#define ETM_CAN_REGISTER_MAGNET_FILAMENT_CMD_OUTPUT_ENABLE              0x7080
#define ETM_CAN_REGISTER_MAGNET_FILAMENT_CMD_OUTPUT_DISABLE             0x7081


#define ETM_CAN_REGISTER_GUN_DRIVER_SET_1_GRID_TOP_SET_POINT            0x8100
#define ETM_CAN_REGISTER_GUN_DRIVER_SET_1_HEATER_CATHODE_SET_POINT      0x8101
#define ETM_CAN_REGISTER_GUN_DRIVER_CMD_ENABLE_HEATER                   0x8080
#define ETM_CAN_REGISTER_GUN_DRIVER_CMD_DISABLE_HEATER                  0x8081
#define ETM_CAN_REGISTER_GUN_DRIVER_CMD_ENABLE_TRIGGER                  0x8082
#define ETM_CAN_REGISTER_GUN_DRIVER_CMD_DISABLE_TRIGGER                 0x8083
#define ETM_CAN_REGISTER_GUN_DRIVER_CMD_ENABLE_PULSE_TOP                0x8084
#define ETM_CAN_REGISTER_GUN_DRIVER_CMD_DISABLE_PULSE_TOP               0x8085
#define ETM_CAN_REGISTER_GUN_DRIVER_CMD_ENABLE_HV                       0x8086
#define ETM_CAN_REGISTER_GUN_DRIVER_CMD_DISABLE_HV                      0x8087
#define ETM_CAN_REGISTER_GUN_DRIVER_CMD_ZERO_HEATER_TIME_DELAY          0x8088

#define ETM_CAN_REGISTER_PULSE_SYNC_SET_1_HIGH_ENERGY_TIMING_REG_0      0x3100
#define ETM_CAN_REGISTER_PULSE_SYNC_SET_1_HIGH_ENERGY_TIMING_REG_1      0x3101
#define ETM_CAN_REGISTER_PULSE_SYNC_SET_1_LOW_ENERGY_TIMING_REG_0       0x3102
#define ETM_CAN_REGISTER_PULSE_SYNC_SET_1_LOW_ENERGY_TIMING_REG_1       0x3103
#define ETM_CAN_REGISTER_PULSE_SYNC_SET_1_CUSTOMER_LED_OUTPUT           0x3110
#define ETM_CAN_REGISTER_PULSE_SYNC_REQUEST_PERSONALITY_MODULE          0x3120
#define ETM_CAN_REGISTER_PULSE_SYNC_CMD_ENABLE_PULSES                   0x3080
#define ETM_CAN_REGISTER_PULSE_SYNC_CMD_DISABLE_PULSES                  0x3081


#define ETM_CAN_REGISTER_ECB_SET_2_HIGH_ENERGY_TARGET_CURRENT_MON       0xE100
#define ETM_CAN_REGISTER_ECB_SET_2_LOW_ENERGY_TARGET_CURRENT_MON        0xE101
#define ETM_CAN_REGISTER_ECB_SET_2_PERSONAILITY_MODULE                  ETM_CAN_REGISTER_PULSE_SYNC_REQUEST_PERSONALITY_MODULE


//------------------ DATA LOGGING REGISTERS --------------------------//

// Default data logging registers
#define ETM_CAN_DATA_LOG_REGISTER_DEFAULT_ERROR_0                       0x0
#define ETM_CAN_DATA_LOG_REGISTER_DEFAULT_ERROR_1                       0x1
#define ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_0                       0x2
#define ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_1                       0x3
#define ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_2                       0x4
#define ETM_CAN_DATA_LOG_REGISTER_DEFAULT_DEBUG_3                       0x5
#define ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CONFIG_0                      0x6
#define ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CONFIG_1                      0x7
#define ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_0                   0x8
#define ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_1                   0x9
#define ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_2                   0xA
#define ETM_CAN_DATA_LOG_REGISTER_DEFAULT_CAN_ERROR_3                   0xB




#define ETM_CAN_DATA_LOG_REGISTER_HV_LAMBDA_FAST_PROGRAM_VOLTAGE        0x4C
#define ETM_CAN_DATA_LOG_REGISTER_HV_LAMBDA_SLOW_SET_POINT              0x4D

#define ETM_CAN_DATA_LOG_REGISTER_ION_PUMP_SLOW_MONITORS                0x4C

#define ETM_CAN_DATA_LOG_REGISTER_AFC_FAST_POSITION                     0x5C
#define ETM_CAN_DATA_LOG_REGISTER_AFC_FAST_READINGS                     0x5D
#define ETM_CAN_DATA_LOG_REGISTER_AFC_SLOW_SETTINGS                     0x5E

#define ETM_CAN_DATA_LOG_REGISTER_MAGNETRON_MON_FAST_PREVIOUS_PULSE     0x2C
#define ETM_CAN_DATA_LOG_REGISTER_MAGNETRON_MON_SLOW_FILTERED_PULSE     0x2D
#define ETM_CAN_DATA_LOG_REGISTER_MAGNETRON_MON_SLOW_ARCS               0x2E
#define ETM_CAN_DATA_LOG_REGISTER_MAGNETRON_MON_SLOW_PULSE_COUNT        0x2F

#define ETM_CAN_DATA_LOG_REGISTER_COOLING_SLOW_FLOW_0                   0x6C
#define ETM_CAN_DATA_LOG_REGISTER_COOLING_SLOW_FLOW_1                   0x6D
#define ETM_CAN_DATA_LOG_REGISTER_COOLING_SLOW_ANALOG_READINGS          0x6E

#define ETM_CAN_DATA_LOG_REGISTER_MAGNET_FILAMENT_SLOW_READINGS         0x7C
#define ETM_CAN_DATA_LOG_REGISTER_MAGNET_FILAMENT_SLOW_SET_POINTS       0x7D

#define ETM_CAN_DATA_LOG_REGISTER_GUN_DRIVER_SLOW_PULSE_TOP_MON         0x8C
#define ETM_CAN_DATA_LOG_REGISTER_GUN_DRIVER_SLOW_HEATER_MON            0x8D
#define ETM_CAN_DATA_LOG_REGISTER_GUN_DRIVER_SLOW_SET_POINTS            0x8E

#define ETM_CAN_DATA_LOG_REGISTER_PULSE_TOP_FAST_TRIGGER_DATA           0x3C
#define ETM_CAN_DATA_LOG_REGISTER_PULSE_TOP_SLOW_TIMING_DATA_0          0x3D
#define ETM_CAN_DATA_LOG_REGISTER_PULSE_TOP_SLOW_TIMING_DATA_1          0x3E
#define ETM_CAN_DATA_LOG_REGISTER_PULSE_TOP_SLOW_TIMING_DATA_2          0x3F


#endif
