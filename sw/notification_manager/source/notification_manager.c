/* =============================
 *   Includes of common headers
 * =============================*/
#include "stdlib.h"
#include "string.h"
/* =============================
 *  Includes of project headers
 * =============================*/
#include "notification_manager.h"
#include "notification_types.h"
#include "wifi_manager.h"
#include "inputs_board.h"
#include "relays_board.h"
#include "env_monitor.h"
#include "bathroom_fan.h"
#include "stairs_led_module.h"
#include "Logger.h"
#include "time_counter.h"
#include "string_formatter.h"
/* =============================
 *          Defines
 * =============================*/
#define WIFI_BROADCAST_ID 0xFF
/* =============================
 *       Internal types
 * =============================*/
typedef struct
{
   uint8_t data_size;
   const uint8_t* data;
} NTF_MESSAGE;
typedef struct
{
   NTF_CMD_ID id;
   NTF_REQ_TYPE type;
   RET_CODE (*process)(const NTF_MESSAGE*);
} NTF_HANDLE_ITEM;
/* =============================
 *   Internal module functions
 * =============================*/
RET_CODE ntfmgr_handle_get_system_time_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_set_system_time_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_get_system_status_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_get_inputs_state_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_get_all_inputs_state_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_get_relays_state_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_get_all_relays_state_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_set_relays_state_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_set_all_relays_state_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_set_fan_state_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_get_fan_state_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_get_slm_state_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_get_slm_program_id_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_set_slm_state_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_set_slm_program_id_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_get_env_sensor_data_cmd(const NTF_MESSAGE* msg);
RET_CODE ntfmgr_handle_get_env_sensor_rate_cmd(const NTF_MESSAGE* msg);

void ntfmgr_write_to_buffer(uint8_t byte);
void ntfmgr_write_inputs_to_buffer(const INPUT_STATUS* inputs, uint8_t size);
void ntfmgr_write_relays_to_buffer(const RELAY_STATUS* relays, uint8_t relays_no);
void ntfmgr_write_env_to_buffer(ENV_ITEM_ID id, const DHT_SENSOR_DATA* sensor);

void ntfmgr_prepare_header(NTF_CMD_ID id, NTF_REQ_TYPE req_type, uint8_t data_size);
void ntfmgr_send_data(uint8_t clientID);
uint16_t ntfmgr_convert_request_to_bytes(const char* data, uint8_t* buf);

void ntfmgr_on_inputs_change(INPUT_STATUS status);
void ntfmgr_on_relays_change(const RELAY_STATUS* status);
void ntfmgr_on_env_change(ENV_EVENT event, ENV_ITEM_ID id,  const DHT_SENSOR*);
void ntfmgr_on_fan_change(FAN_STATE state);
void ntfmgr_on_slm_change(SLM_STATE state);
//void ntfmgr_process_slm_cmd(uint8_t subcmd, NTF_REQ_TYPE req_type, uint8_t data_size, const uint8_t* args);
//void ntfmgr_process_env_cmd(uint8_t subcmd, NTF_REQ_TYPE req_type, uint8_t data_size, const uint8_t* args);

NTF_HANDLE_ITEM m_handlers[] = {{NTF_SYSTEM_TIME,      NTF_GET, &ntfmgr_handle_get_system_time_cmd},
                                {NTF_SYSTEM_TIME,      NTF_SET, &ntfmgr_handle_set_system_time_cmd},
                                {NTF_SYSTEM_STATUS,    NTF_GET, &ntfmgr_handle_get_system_status_cmd},
                                {NTF_INPUTS_STATE,     NTF_GET, &ntfmgr_handle_get_inputs_state_cmd},
                                {NTF_INPUTS_STATE_ALL, NTF_GET, &ntfmgr_handle_get_all_inputs_state_cmd},
                                {NTF_RELAYS_STATE,     NTF_GET, &ntfmgr_handle_get_relays_state_cmd},
                                {NTF_RELAYS_STATE,     NTF_SET, &ntfmgr_handle_set_relays_state_cmd},
                                {NTF_RELAYS_STATE_ALL, NTF_GET, &ntfmgr_handle_get_all_relays_state_cmd},
                                {NTF_RELAYS_STATE_ALL, NTF_SET, &ntfmgr_handle_set_all_relays_state_cmd},
                                {NTF_FAN_STATE,        NTF_GET, &ntfmgr_handle_get_fan_state_cmd},
                                {NTF_FAN_STATE,        NTF_SET, &ntfmgr_handle_set_fan_state_cmd},
                                {NTF_SLM_STATE,        NTF_SET, &ntfmgr_handle_set_slm_state_cmd},
                                {NTF_SLM_STATE,        NTF_GET, &ntfmgr_handle_get_slm_state_cmd},
                                {NTF_SLM_PROGRAM_ID,   NTF_SET, &ntfmgr_handle_set_slm_program_id_cmd},
                                {NTF_SLM_PROGRAM_ID,   NTF_GET, &ntfmgr_handle_get_slm_program_id_cmd},
                                {NTF_ENV_SENSOR_DATA,  NTF_GET, &ntfmgr_handle_get_env_sensor_data_cmd},
                                {NTF_ENV_SENSOR_ERROR, NTF_GET, &ntfmgr_handle_get_env_sensor_rate_cmd}};

uint8_t m_handlers_size;
uint8_t m_buffer [NTF_MAX_MESSAGE_SIZE];
uint16_t m_bytes_count;
/* =============================
 *      Module variables
 * =============================*/

RET_CODE ntfmgr_init()
{
   RET_CODE result = RETURN_NOK;
   for (uint16_t i = 0; i < NTF_MAX_MESSAGE_SIZE; i++)
   {
      m_buffer[i] = 0;
   }
   m_handlers_size = sizeof(m_handlers) / sizeof(m_handlers[0]);

   if (wifimgr_register_data_callback(&ntfmgr_parse_request) == RETURN_OK)
   {
      result = inp_add_input_listener(&ntfmgr_on_inputs_change);
      logger_send_if(result != RETURN_OK, LOG_ERROR, __func__, "cannot subscribe for inputs");

      for (uint8_t i = 1; i <= ENV_SENSORS_COUNT; i++)
      {
         result = env_register_listener(&ntfmgr_on_env_change, (ENV_ITEM_ID)i);
         logger_send_if(result != RETURN_OK, LOG_ERROR, __func__, "cannot subscribe for env %d", (uint8_t)i);
      }

      result = rel_add_listener(&ntfmgr_on_relays_change);
      logger_send_if(result != RETURN_OK, LOG_ERROR, __func__, "cannot subscribe for relays");

      result = fan_add_listener(&ntfmgr_on_fan_change);
      logger_send_if(result != RETURN_OK, LOG_ERROR, __func__, "cannot subscribe for fan");

      result = slm_add_listener(&ntfmgr_on_slm_change);
      logger_send_if(result != RETURN_OK, LOG_ERROR, __func__, "cannot subscribe for SLM state");
   }
   logger_send_if(result != RETURN_OK, LOG_ERROR, __func__, "cannot init ntfmgr");
   return result;
}
void ntfmgr_parse_request(ServerClientID id, const char* data)
{
   m_bytes_count = 0;

   //convert string to bytes
   uint8_t buf [NTF_MAX_MESSAGE_SIZE];
   ntfmgr_convert_request_to_bytes(data, buf);

   NTF_CMD_ID cmd_id = (NTF_CMD_ID) buf[0];
   NTF_REQ_TYPE cmd_type = (NTF_REQ_TYPE) buf[1];
   uint8_t data_size = buf[2];
   for (uint8_t i = 0; i < m_handlers_size; i++)
   {
      if (m_handlers[i].id == cmd_id && m_handlers[i].type == cmd_type && m_handlers[i].process)
      {
         NTF_MESSAGE msg = {data_size, (uint8_t*)buf + NTF_HEADER_SIZE};
         if (m_handlers[i].process(&msg) != RETURN_OK)
         {
            ntfmgr_prepare_header(cmd_id, cmd_type, 1);
            ntfmgr_write_to_buffer((uint8_t) NTF_REPLY_NOK);
         }
         ntfmgr_send_data(id);
      }
   }
}
uint16_t ntfmgr_convert_request_to_bytes(const char* data, uint8_t* buf)
{
   uint16_t str_len = strlen(data);

   char local_buf [4] = {}; /* max byte as string is 255 - 3 chars + NULL */
   uint8_t local_buf_idx = 0;
   uint16_t ext_buf_idx = 0;

   for (uint16_t i = 0; i < str_len + 1; i++)
   {
      if (data[i] == ' ' || !data[i])
      {
         local_buf[local_buf_idx] = 0x00;
         buf[ext_buf_idx] = atoi(local_buf);
         ext_buf_idx++;
         local_buf_idx = 0;
      }
      else
      {
         local_buf[local_buf_idx] = data[i];
         local_buf_idx++;
      }
   }
   return ext_buf_idx;
}
void ntfmgr_send_data(uint8_t clientID)
{
   m_buffer[m_bytes_count - 1] = NTF_MESSAGE_DELIMITER;
   m_buffer[m_bytes_count] = 0x00;
   if (clientID == WIFI_BROADCAST_ID)
   {
      wifimgr_broadcast_bytes(m_buffer, m_bytes_count);
   }
   else
   {
      wifimgr_send_bytes(clientID, m_buffer, m_bytes_count);
   }
   m_bytes_count = 0;
}

void ntfmgr_prepare_header(NTF_CMD_ID id, NTF_REQ_TYPE req_type, uint8_t data_size)
{
   m_bytes_count = 0;
   m_bytes_count += string_format((char*)(m_buffer + m_bytes_count), "%.2u %.2u %.2u ", id, req_type, data_size);
}
void ntfmgr_write_to_buffer(uint8_t byte)
{
   m_bytes_count += string_format((char*)(m_buffer + m_bytes_count), "%.2u ", byte);
}
void ntfmgr_write_inputs_to_buffer(const INPUT_STATUS* inputs, uint8_t input_no)
{
   for (uint8_t i = 0; i < input_no; i++)
   {
      ntfmgr_write_to_buffer((uint8_t) inputs[i].id);
      ntfmgr_write_to_buffer((uint8_t) inputs[i].state);
   }
}
void ntfmgr_write_relays_to_buffer(const RELAY_STATUS* relays, uint8_t relays_no)
{
   for (uint8_t i = 0; i < relays_no; i++)
   {
      ntfmgr_write_to_buffer((uint8_t) relays[i].id);
      ntfmgr_write_to_buffer((uint8_t) relays[i].state);
   }
}
void ntfmgr_write_env_to_buffer(ENV_ITEM_ID id, const DHT_SENSOR_DATA* sensor)
{
   ntfmgr_write_to_buffer((uint8_t)id);
   ntfmgr_write_to_buffer(sensor->hum_h);
   ntfmgr_write_to_buffer(sensor->hum_l);
   ntfmgr_write_to_buffer(sensor->temp_h);
   ntfmgr_write_to_buffer(sensor->temp_l);
}

void ntfmgr_on_inputs_change(INPUT_STATUS status)
{
   ntfmgr_prepare_header(NTF_INPUTS_STATE, NTF_NTF, 2);
   ntfmgr_write_to_buffer(status.id);
   ntfmgr_write_to_buffer(status.state);
   ntfmgr_send_data(WIFI_BROADCAST_ID);
}
void ntfmgr_on_relays_change(const RELAY_STATUS* status)
{
   ntfmgr_prepare_header(NTF_RELAYS_STATE, NTF_NTF, 2);
   ntfmgr_write_to_buffer(status->id);
   ntfmgr_write_to_buffer(status->state);
   ntfmgr_send_data(WIFI_BROADCAST_ID);
}
void ntfmgr_on_env_change(ENV_EVENT event, ENV_ITEM_ID id,  const DHT_SENSOR* sensor)
{
   ntfmgr_prepare_header(event == ENV_EV_ERROR? NTF_ENV_SENSOR_ERROR : NTF_ENV_SENSOR_DATA, NTF_NTF, 6);
   ntfmgr_write_to_buffer((uint8_t)id);
   ntfmgr_write_to_buffer((uint8_t)sensor->type);
   ntfmgr_write_to_buffer((uint8_t)sensor->data.hum_h);
   ntfmgr_write_to_buffer((uint8_t)sensor->data.hum_l);
   ntfmgr_write_to_buffer((uint8_t)sensor->data.temp_h);
   ntfmgr_write_to_buffer((uint8_t)sensor->data.temp_l);
   ntfmgr_send_data(WIFI_BROADCAST_ID);
}
void ntfmgr_on_fan_change(FAN_STATE state)
{
   ntfmgr_prepare_header(NTF_FAN_STATE, NTF_NTF, 1);
   ntfmgr_write_to_buffer((uint8_t) state);
   ntfmgr_send_data(WIFI_BROADCAST_ID);
}
void ntfmgr_on_slm_change(SLM_STATE state)
{
   ntfmgr_prepare_header(NTF_SLM_STATE, NTF_NTF, 1);
   ntfmgr_write_to_buffer((uint8_t) state);
   ntfmgr_send_data(WIFI_BROADCAST_ID);
}

RET_CODE ntfmgr_handle_get_system_time_cmd(const NTF_MESSAGE* msg)
{
   RET_CODE result = RETURN_NOK;
   TimeItem* item = time_get();
   if (item)
   {
      ntfmgr_prepare_header(NTF_SYSTEM_TIME, NTF_GET, 8);
      ntfmgr_write_to_buffer((uint8_t) NTF_REPLY_OK);
      ntfmgr_write_to_buffer(item->day);
      ntfmgr_write_to_buffer(item->month);
      ntfmgr_write_to_buffer(((item->year) >> 8) & 0xFF);
      ntfmgr_write_to_buffer((item->year) & 0xFF);
      ntfmgr_write_to_buffer(item->hour);
      ntfmgr_write_to_buffer(item->minute);
      ntfmgr_write_to_buffer(item->second);
      result = RETURN_OK;
   }
   return result;
}
RET_CODE ntfmgr_handle_set_system_time_cmd(const NTF_MESSAGE* msg)
{
   RET_CODE result = RETURN_NOK;
   if (msg->data_size == 7)
   {
      TimeItem item = {};
      item.day = msg->data[0];
      item.month = msg->data[1];
      item.year = msg->data[2] << 8;
      item.year |= msg->data[3];
      item.hour = msg->data[4];
      item.minute = msg->data[5];
      item.second = msg->data[6];
      result = time_set_utc(&item);
      ntfmgr_prepare_header(NTF_SYSTEM_TIME, NTF_SET, 1);
      ntfmgr_write_to_buffer(NTF_REPLY_OK);
   }
   return result;
}
RET_CODE ntfmgr_handle_get_system_status_cmd(const NTF_MESSAGE* msg)
{
   RET_CODE result = RETURN_NOK;
   if (msg->data_size == 0)
   {
      INPUT_STATUS inp_status [INPUTS_MAX_INPUT_LINES];
      RELAY_STATUS rel_status [RELAYS_BOARD_COUNT];
      ENV_CONFIG env_cfg = {};
      result = inp_get_all(inp_status);
      if (result == RETURN_OK)
      {
         result = rel_get_all(rel_status);
         if (result == RETURN_OK)
         {
            result = env_get_config(&env_cfg);
            if (result == RETURN_OK)
            {
               uint8_t bytes_count = 1 + 1 + (INPUTS_MAX_INPUT_LINES * 2) + 1 + (RELAYS_BOARD_COUNT * 2) + 1 + (ENV_SENSORS_COUNT * 5) + 2;
               ntfmgr_prepare_header(NTF_SYSTEM_STATUS, NTF_GET, bytes_count);
               ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
               ntfmgr_write_to_buffer(INPUTS_MAX_INPUT_LINES * 2);
               ntfmgr_write_inputs_to_buffer(inp_status, INPUTS_MAX_INPUT_LINES);
               ntfmgr_write_to_buffer(RELAYS_BOARD_COUNT * 2);
               ntfmgr_write_relays_to_buffer(rel_status, RELAYS_BOARD_COUNT);

               DHT_SENSOR env_data;
               ntfmgr_write_to_buffer(ENV_SENSORS_COUNT * 5);
               for (uint8_t i = 0; i < ENV_SENSORS_COUNT; i++)
               {
                  env_get_sensor_data(env_cfg.items[i].env_id, &env_data);
                  ntfmgr_write_env_to_buffer(env_cfg.items[i].env_id, &env_data.data);
               }
               ntfmgr_write_to_buffer(0x01); /* FAN status bytes count */
               ntfmgr_write_to_buffer((uint8_t) fan_get_state());
            }

         }
      }
   }
   return result;
}
RET_CODE ntfmgr_handle_get_inputs_state_cmd(const NTF_MESSAGE* msg)
{
   RET_CODE result = RETURN_NOK;
   if (msg->data_size == 1)
   {
      INPUT_STATE state = inp_get((INPUT_ID)msg->data[0]);
      ntfmgr_prepare_header(NTF_INPUTS_STATE, NTF_GET, 2);
      ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
      ntfmgr_write_to_buffer((uint8_t)state);
      result = RETURN_OK;
   }
   return result;
}
RET_CODE ntfmgr_handle_get_all_inputs_state_cmd(const NTF_MESSAGE* msg)
{
   INPUT_STATUS inp_status [INPUTS_MAX_INPUT_LINES];
   RET_CODE result = inp_get_all(inp_status);
   if (result == RETURN_OK)
   {
      uint8_t bytes_count = 1 + (INPUTS_MAX_INPUT_LINES * 2);
      ntfmgr_prepare_header(NTF_INPUTS_STATE_ALL, NTF_GET, bytes_count);
      ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
      ntfmgr_write_inputs_to_buffer(inp_status, INPUTS_MAX_INPUT_LINES);
   }
   return result;
}

RET_CODE ntfmgr_handle_get_relays_state_cmd(const NTF_MESSAGE* msg)
{
   RET_CODE result = RETURN_NOK;
   if (msg->data_size == 1)
   {
      RELAY_STATE status = rel_get((RELAY_ID)msg->data[0]);
      if (status != RELAY_STATE_ENUM_MAX)
      {
         ntfmgr_prepare_header(NTF_RELAYS_STATE, NTF_GET, 2);
         ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
         ntfmgr_write_to_buffer((uint8_t)status);
         result = RETURN_OK;
      }
   }
   return result;
}
RET_CODE ntfmgr_handle_get_all_relays_state_cmd(const NTF_MESSAGE* msg)
{
   RELAY_STATUS rel_status [RELAYS_BOARD_COUNT];
   RET_CODE result = rel_get_all(rel_status);
   if (result == RETURN_OK)
   {
      uint8_t bytes_count = 1 + (RELAYS_BOARD_COUNT * 2);
      ntfmgr_prepare_header(NTF_RELAYS_STATE_ALL, NTF_GET, bytes_count);
      ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
      ntfmgr_write_relays_to_buffer(rel_status, RELAYS_BOARD_COUNT);
   }
   return result;
}
RET_CODE ntfmgr_handle_set_relays_state_cmd(const NTF_MESSAGE* msg)
{
   RET_CODE result = RETURN_NOK;
   if (msg->data_size == 2)
   {
      result = rel_set((RELAY_ID)msg->data[0], (RELAY_STATE)msg->data[1]);
      ntfmgr_prepare_header(NTF_RELAYS_STATE, NTF_SET, 1);
      ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
   }
   return result;
}
RET_CODE ntfmgr_handle_set_all_relays_state_cmd(const NTF_MESSAGE* msg)
{
   RET_CODE result = RETURN_OK;
   if ((msg->data_size%2) != 1)
   {
      for (uint8_t i = 0; i < msg->data_size / 2; i++)
      {
         RELAY_ID id = (RELAY_ID)msg->data[(i*2)];
         RELAY_STATE state = (RELAY_STATE)msg->data[(i*2)+1];
         result = rel_set(id, state);
         if (result != RETURN_OK)
         {
            break;
         }
      }
      if (result == RETURN_OK)
      {
         ntfmgr_prepare_header(NTF_RELAYS_STATE_ALL, NTF_SET, 1);
         ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
      }
   }
   return result;
}
RET_CODE ntfmgr_handle_set_fan_state_cmd(const NTF_MESSAGE* msg)
{
   RET_CODE result = RETURN_NOK;
   if (msg->data_size == 1)
   {
      FAN_STATE state = (FAN_STATE)msg->data[0];
      if (state == FAN_STATE_ON)
      {
         result = fan_start();
      }
      else if (state == FAN_STATE_OFF)
      {
         result = fan_stop();
      }

      if (result == RETURN_OK)
      {
         ntfmgr_prepare_header(NTF_FAN_STATE, NTF_SET, 1);
         ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
      }
   }
   return result;
}
RET_CODE ntfmgr_handle_get_fan_state_cmd(const NTF_MESSAGE* msg)
{
   RET_CODE result = RETURN_NOK;
   FAN_STATE state = fan_get_state();
   if (state != FAN_STATE_UNKNOWN)
   {
      ntfmgr_prepare_header(NTF_FAN_STATE, NTF_GET, 2);
      ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
      ntfmgr_write_to_buffer((uint8_t)state);
      result = RETURN_OK;
   }
   return result;
}
RET_CODE ntfmgr_handle_get_slm_state_cmd(const NTF_MESSAGE* msg)
{
   SLM_STATE state = slm_get_state();
   ntfmgr_prepare_header(NTF_SLM_STATE, NTF_GET, 2);
   ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
   ntfmgr_write_to_buffer((uint8_t)state);
   return RETURN_OK;
}
RET_CODE ntfmgr_handle_get_slm_program_id_cmd(const NTF_MESSAGE* msg)
{
   SLM_PROGRAM_ID id = slm_get_current_program_id();
   ntfmgr_prepare_header(NTF_SLM_PROGRAM_ID, NTF_GET, 2);
   ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
   ntfmgr_write_to_buffer((uint8_t)id);
   return RETURN_OK;
}
RET_CODE ntfmgr_handle_set_slm_state_cmd(const NTF_MESSAGE* msg)
{
   RET_CODE result = RETURN_NOK;
   if (msg->data_size == 1)
   {
      SLM_STATE state = (SLM_STATE)msg->data[0];
      if (state == SLM_STATE_OFF)
      {
         result = slm_stop_program();
      }
      else if (state == SLM_STATE_ON)
      {
         result = slm_start_program();
      }

      if (result == RETURN_OK)
      {
         ntfmgr_prepare_header(NTF_SLM_STATE, NTF_SET, 1);
         ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
      }
   }
   return result;
}
RET_CODE ntfmgr_handle_set_slm_program_id_cmd(const NTF_MESSAGE* msg)
{
   RET_CODE result = RETURN_NOK;
   if (msg->data_size == 1)
   {
      SLM_PROGRAM_ID id = (SLM_PROGRAM_ID)msg->data[0];
      result = slm_set_current_program_id(id);
      if (result == RETURN_OK)
      {
         ntfmgr_prepare_header(NTF_SLM_PROGRAM_ID, NTF_SET, 1);
         ntfmgr_write_to_buffer((uint8_t)NTF_REPLY_OK);
      }
   }
   return result;
}

RET_CODE ntfmgr_handle_get_env_sensor_data_cmd(const NTF_MESSAGE* msg)
{
   DHT_SENSOR sensor = {};
   RET_CODE result = RETURN_NOK;
   if (msg->data_size == 1)
   {
      result = env_get_sensor_data((ENV_ITEM_ID)msg->data[0], &sensor);
      ntfmgr_prepare_header(NTF_ENV_SENSOR_DATA, NTF_GET, 6);
      if (result == RETURN_OK)
      {
         ntfmgr_write_to_buffer((uint8_t) NTF_REPLY_OK);
         ntfmgr_write_to_buffer((uint8_t) sensor.type);
         ntfmgr_write_to_buffer((uint8_t) sensor.data.hum_h);
         ntfmgr_write_to_buffer((uint8_t) sensor.data.hum_l);
         ntfmgr_write_to_buffer((uint8_t) sensor.data.temp_h);
         ntfmgr_write_to_buffer((uint8_t) sensor.data.temp_l);
      }
   }
   return result;
}
RET_CODE ntfmgr_handle_get_env_sensor_rate_cmd(const NTF_MESSAGE* msg)
{
   RET_CODE result = RETURN_NOK;
   if (msg->data_size == 1)
   {
      ENV_ERROR_RATE rate = env_get_error_stats((ENV_ITEM_ID)msg->data[0]);
      ntfmgr_prepare_header(NTF_ENV_SENSOR_ERROR, NTF_GET, 3);
      ntfmgr_write_to_buffer((uint8_t) NTF_REPLY_OK);
      ntfmgr_write_to_buffer((uint8_t) rate.cs_err_rate);
      ntfmgr_write_to_buffer((uint8_t) rate.nr_err_rate);
      result = RETURN_OK;
   }
   return result;
}
