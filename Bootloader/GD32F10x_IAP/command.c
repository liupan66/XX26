/***************************************************************************
*module name: command
*creat  date: 2021/5/18
*created  by：pan_liu
*   function：上位机与下位机协议通讯
***************************************************************************/
#include "gd32f10x.h"
#include "string.h"
#include "command.h"
#include "stdint.h"
#include "stdio.h"
#include "FLASH.h"
#include "main.h"
#include "gd32f10x_it.h"
#include "gd32f10x_fmc.h"
//0x7E7EAA55
uint8_t Packet_header[4]={0x55,0XAA,0X7E,0X7E};
//0x0D0DFE5A
uint8_t Packet_tail[4]={0x5A,0XFE,0X0D,0X0D};
uint16_t data_length=0;
uint16_t checksum_calc=0,checksum_recv=0;
uint16_t Information_label=0;
uint16_t Frame_number =0;
uint32_t start_write_addr=0x08002800;
extern uint16_t u2cnt;
//定义协议结构体
Self_check_type Self_check;
Self_check_feedback_type Self_check_feedback;
Online_update_order_type Online_update_order;
union Online_update_order_feedback_union Online_update_order_feedback_union_entity;
Online_update_data_type Online_update_data;
union Online_update_data_feedback_union Online_update_data_feedback_union_entity;

extern union Online_update_order_feedback_union Online_update_order_feedback_union_entity_1;
/***********************************************************************
*function name:calc_checksum
*creat  date: 2021/5/18
*created  by：pan_liu
*function:calc_checksum form receive command
*input:
*output:
************************************************************************/
uint16_t calc_checksum(uint8_t *command_buffer, uint16_t length)
{
    uint16_t checksum = 0;
    uint16_t i = 0;
    for(i = 0; i < length; i++)
    {
        checksum += *command_buffer++;
    }
    return checksum;
}

uint8_t process_command(uint8_t *command)
{
	int16_t i=0;
	uint32_t va0;
	uint8_t str1[8]={0};
	uint8_t str2[12]={0};
	uint16_t code_data_length=0;
	uint16_t checksum=0;
	if(command_check(command)==ERROR)
		return ERROR;
		
	//获取信息标签
	Information_label= ((command[9]<<8) | command[8]);
	switch(Information_label)
	{
		case 0x3801://自检指令
			break;
		case 0xF001://软件在线更新指令
			//解析指令数据
			Online_update_order.Packet_header=command[3]<<24|command[2]<<16|command[1]<<8|command[0];
			Online_update_order.Data_length_1=command[5]<<8|command[4];
			Online_update_order.Data_length_2=command[7]<<8|command[6];
			Online_update_order.Information_label=command[9]<<8|command[8];
			Online_update_order.Frame_number=command[11]<<8|command[10];
			Online_update_order.Online_programming_source=command[13]<<8|command[12];
			Online_update_order.Backup_1=command[15]<<8|command[14];
			Online_update_order.Software_configuration_item_name_1=command[17]<<8|command[16];
			Online_update_order.Software_configuration_item_name_2=command[19]<<8|command[18];
			Online_update_order.Order_status=command[21]<<8|command[20];
			Online_update_order.Reserve1=command[23]<<8|command[22];
			Online_update_order.Total_code_length=command[27]<<24|command[26]<<16|command[25]<<8|command[24];
			for(i=0;i<6;i++)
			{
				Online_update_order.Reserve2[i]=command[28+i];
			}
			Online_update_order.Checksum=command[35]<<8|command[34];
			Online_update_order.Packet_tail=command[39]<<24|command[38]<<16|command[37]<<8|command[36];
			//usart_rx_clear();
			//准备回馈指令数据
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Packet_header=0x7E7EAA55;						//Packet_header	
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Data_length_1=0x0020;							//Data_length_1
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Data_length_2=0x0020;							//Data_length_2
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Information_label=0x0F01;						//Information_label
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Frame_number=Online_update_order.Frame_number;									//Frame_number
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Online_programming_source=Online_update_order.Online_programming_source;				//Online_programming_source
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Backup_1=Online_update_order.Backup_1;			//Backup_1
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Software_configuration_item_name_1=Online_update_order.Software_configuration_item_name_1;		//Software_configuration_item_name_1
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Software_configuration_item_name_2=Online_update_order.Software_configuration_item_name_2;		//Software_configuration_item_name_2
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Order_status=Online_update_order.Order_status;								//Order_status
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Response_status=0xAAAA;							//Response_status
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Backup_2=0;										//Backup_2
			for(i=0;i<8;i++)
			{
				Online_update_order_feedback_union_entity.Online_update_order_feedback.Reserve[i]=str1[i];							//Reserve[8];
			}
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Checksum=calc_checksum(&Online_update_order_feedback_union_entity.str[4],30);	//Checksum
			Online_update_order_feedback_union_entity.Online_update_order_feedback.Packet_tail=0x0D0DFE5A; 							//Packet_tail		
			switch(Online_update_order.Order_status)
			{
				case 0x5555:
					for(i=0;i<40;i++)//发送回馈指令数据
					{
						usart_data_transmit(USART2,Online_update_order_feedback_union_entity_1.str[i]);
						while(RESET == usart_flag_get(USART2, USART_FLAG_TC));
					}
					break;
				case 0xAAAA:
					for(i=0;i<40;i++)//发送回馈指令数据
					{
						usart_data_transmit(USART2,Online_update_order_feedback_union_entity.str[i]);
						while(RESET == usart_flag_get(USART2, USART_FLAG_TC));
					}
					break;
				default:
					break;
			}
			break;
		case 0xF002://软件在线更新代码数据
			//解析指令数据
			Online_update_data.Packet_header=command[3]<<24|command[2]<<16|command[1]<<8|command[0];
			Online_update_data.Data_length_1=command[5]<<8|command[4];
			Online_update_data.Data_length_2=command[7]<<8|command[6];
			Online_update_data.Information_label=command[9]<<8|command[8];
			Online_update_data.Frame_number=command[11]<<8|command[10];
			Online_update_data.Online_programming_source=command[13]<<8|command[12];
			Online_update_data.Configuration_item_name=command[15]<<8|command[14];
			Online_update_data.Current_packet_status=command[17]<<8|command[16];
			Online_update_data.Current_packet_valid_bytes_number=command[19]<<8|command[18];
		    Online_update_data.Reserve1=command[23]<<24|command[22]<<16|command[21]<<8|command[20];
			for(i=0;i<512;i++)
			{
				
				Online_update_data.Code_data[i]=command[24+i];
			}
			Online_update_data.Reserve2=command[537]<<8|command[536];
			Online_update_data.Checksum=command[539]<<8|command[538];
			Online_update_data.Packet_tail=command[543]<<24|command[542]<<16|command[541]<<8|command[540];
			code_data_length=command[19]<<8|command[18];
			
			usart_rx_clear();
			data_length=0;
			
			if(Online_update_data.Frame_number==1)
			{
				va0 = 1;
				erase_page(1,4);
				if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED)
				{
					if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED)
					{
						printf("\t写标志位失败!\n");
					}
				}
				start_write_addr=0x08002800;
				erase_page(100,5);//从0x08002800开始擦除100KB空间
				
			}
			//处理代码数据
			
			if(FLASH_Program(start_write_addr,code_data_length, (uint32_t *)Online_update_data.Code_data))
			{
				if(Online_update_data.Current_packet_status==0x0043)
				{
					uint32_t va0 = 0;
					erase_page(1,4);
					if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED)
					{
						if(FLASH_Program(ApplicationATarget, 4, &va0) != PASSED)
						{
							printf("\t程序更新标志更新失败\n");
						}
					}
					if(FLASH_Program(ApplicationATarget+4, 4, &va0) != PASSED)//清除APP跳转Boot标记位
					{
						if(FLASH_Program(ApplicationATarget+4, 4, &va0) != PASSED)
						{
							printf("\t写标志位失败!\n");
						}
					}
				}
				start_write_addr=start_write_addr+ Online_update_data.Current_packet_valid_bytes_number;
				
				//准备回馈指令数据
				Online_update_data_feedback_union_entity.Online_update_data_feedback.Packet_header=0x7E7EAA55;
				Online_update_data_feedback_union_entity.Online_update_data_feedback.Data_length_1=0x0020;
				Online_update_data_feedback_union_entity.Online_update_data_feedback.Data_length_2=0x0020;
				Online_update_data_feedback_union_entity.Online_update_data_feedback.Information_label=0x0F02;
				Online_update_data_feedback_union_entity.Online_update_data_feedback.Frame_number=Online_update_data.Frame_number;
				Online_update_data_feedback_union_entity.Online_update_data_feedback.Online_programming_source=Online_update_data.Online_programming_source;
				Online_update_data_feedback_union_entity.Online_update_data_feedback.Backup=0;
				Online_update_data_feedback_union_entity.Online_update_data_feedback.Configuration_item_name=Online_update_data.Configuration_item_name;
				Online_update_data_feedback_union_entity.Online_update_data_feedback.Current_packet_status=Online_update_data.Current_packet_status;
				Online_update_data_feedback_union_entity.Online_update_data_feedback.Received_packet_frame_sequence_number=Online_update_data.Current_packet_valid_bytes_number;
				for(i=0;i<12;i++)
				{
					Online_update_data_feedback_union_entity.Online_update_data_feedback.Reserve[i]=str2[i];
				}			
				checksum=calc_checksum(&Online_update_data_feedback_union_entity.str[4],30);
				Online_update_data_feedback_union_entity.Online_update_data_feedback.Checksum=checksum;
				Online_update_data_feedback_union_entity.Online_update_data_feedback.Packet_tail=0x0D0DFE5A;
				for(i=0;i<40;i++)//发送回馈指令数据
				{
					usart_data_transmit(USART2,Online_update_data_feedback_union_entity.str[i]);
					while(RESET == usart_flag_get(USART2, USART_FLAG_TC));
				}
				break;
			}
			break;
		default:
			break;
	}
	return 0;
}

uint8_t compare_packet_header(uint8_t *command)
{	
	uint8_t i;
	for(i=0;i<4;i++)
	{
		if(command[i]!=Packet_header[i])
		{		
			return ERROR;
		}
		
	}
	
}

uint8_t compare_packet_tail(uint8_t *command)
{	
	uint8_t i;
	for(i=0;i<4;i++)
	{
		if(command[data_length+4+i]!=Packet_tail[i])
		{
			return ERROR;
		}

	}
}
uint8_t command_check(uint8_t *command)
{
	//判断数据包头是否正确
	if(compare_packet_header(command)==ERROR)
	{
		return ERROR;
	}
	else
	{
		printf("\t程序更新标志更新失败\n");
	}

	//判断数据长度是否正确并计算数据长度
	if((command[4]==command[6])&&(command[5]==command[7]))
	{
		data_length=((command[5]<<8)|command[4]);
	}
	else
	{
		return ERROR;
	}
	//计算校验和并判断校验和是否与上位机发送的一致
	checksum_recv=((command[data_length+3]<<8) | command[data_length+2]);
	checksum_calc=calc_checksum(&command[4],data_length-2);
	if(checksum_calc!=checksum_recv)
	{
		return ERROR;
	}

	//判断数据包尾是否正确
	if(compare_packet_tail(command)==ERROR)
	{
		return ERROR;
	}


}