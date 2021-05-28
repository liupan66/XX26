#ifndef __COMMAND_H
#define __COMMAND_H
#include "stdint.h"


/********************************自检指令********************************/
typedef struct {
	uint32_t Packet_header;
	uint16_t Data_length_1;
	uint16_t Data_length_2;
	uint16_t Information_label;
	uint16_t Frame_number;
	uint16_t Work_order;
	uint32_t Backup;
	uint16_t Checksum;
	uint32_t Packet_tail;
}Self_check_type;

/**************************************************************************/

/********************************自检回馈指令******************************/
typedef struct {
	uint32_t Packet_header;
	uint16_t Data_length_1;
	uint16_t Data_length_2;
	uint16_t Information_label;
	uint16_t Frame_number;
	uint16_t Self_check_status;
	uint8_t  Version_Information[6];
	uint16_t Backup;
	uint16_t Checksum;
	uint32_t Packet_tail;
}Self_check_feedback_type;
union Self_check_feedback_union
{
    Self_check_feedback_type  Self_check_feedback;
    uint8_t  str[40];
};
/**********************************************************************/

/********************************在线更新代码指令***********************/
typedef struct {
	uint32_t Packet_header;
	uint16_t Data_length_1;
	uint16_t Data_length_2;
	uint16_t Information_label;
	uint16_t Frame_number;
	uint16_t Online_programming_source;
	uint16_t Backup_1;
	uint16_t Software_configuration_item_name_1;
	uint16_t Software_configuration_item_name_2;
	uint16_t Order_status;
	uint16_t Reserve1;
	uint32_t Total_code_length;
	uint8_t  Reserve2[6];
	uint16_t Checksum;
	uint32_t Packet_tail;
}Online_update_order_type;
/**********************************************************************/

/********************************在线更新代码回馈指令*******************/
typedef struct {
	uint32_t Packet_header;
	uint16_t Data_length_1;
	uint16_t Data_length_2;
	uint16_t Information_label;
	uint16_t Frame_number;
	uint16_t Online_programming_source;
	uint16_t Backup_1;
	uint16_t Software_configuration_item_name_1;
	uint16_t Software_configuration_item_name_2;
	uint16_t Order_status;
	uint16_t Response_status;
	uint16_t Backup_2;
	uint8_t  Reserve[8];
	uint16_t Checksum;
	uint32_t Packet_tail;
}Online_update_order_feedback_type;
union Online_update_order_feedback_union
{
    Online_update_order_feedback_type Online_update_order_feedback;
    uint8_t  str[40];
};
/**********************************************************************/

/********************************在线更新代码数据指令*******************/
typedef struct {
	uint32_t Packet_header;
	uint16_t Data_length_1;
	uint16_t Data_length_2;
	uint16_t Information_label;
	uint16_t Frame_number;
	uint16_t Online_programming_source;
	uint16_t Configuration_item_name;
	uint16_t Current_packet_status;
	uint16_t Current_packet_valid_bytes_number;
	uint32_t  Reserve1;
	uint8_t  Code_data[512];
	uint16_t  Reserve2;
	uint16_t Checksum;
	uint32_t Packet_tail;
}Online_update_data_type; 
/**********************************************************************/

/********************************在线更新代码数据回馈指令****************/
typedef struct {
	uint32_t Packet_header;
	uint16_t Data_length_1;
	uint16_t Data_length_2;
	uint16_t Information_label;
	uint16_t Frame_number;
	uint16_t Online_programming_source;
	uint16_t Backup;
	uint16_t Configuration_item_name;
	uint16_t Current_packet_status;
	uint16_t Received_packet_frame_sequence_number;
	uint8_t  Reserve[12];
	uint16_t Checksum;
	uint32_t Packet_tail;
}Online_update_data_feedback_type; 

union Online_update_data_feedback_union
{
    Online_update_data_feedback_type Online_update_data_feedback;
    uint8_t  str[40];
};
/***********************************************************************/

extern Self_check_type Self_check;
extern union Self_check_feedback_union Self_check_feedback_union_entity;
extern Online_update_order_type Online_update_order;
extern union Online_update_order_feedback_union Online_update_order_feedback_union_entity;
extern Online_update_data_type Online_update_data;
extern union Online_update_data_feedback_union Online_update_data_feedback_union_entity;
extern uint8_t Version_Information[6];
extern uint16_t Self_check_status;
uint16_t calc_checksum(uint8_t *command_buffer, uint16_t length);
uint8_t process_command(uint8_t *command);
uint8_t compare_packet_header(uint8_t *command);
uint8_t compare_packet_tail(uint8_t *command);
uint8_t command_check(uint8_t *command);

#endif
