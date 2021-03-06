/* =============================
 *   Includes of common headers
 * =============================*/
#include <stdlib.h>
/* =============================
 *  Includes of project headers
 * =============================*/
#include "inputs_board.h"
#include "task_scheduler.h"
#include "Logger.h"
#include "inputs_interrupt_handler.h"
/* =============================
 *          Defines
 * =============================*/
#define INPUTS_CALLBACK_MAX_SIZE 5
#define INPUTS_AUTOUPDATE_STATE 1
#define INPUTS_INTERRUPT_STATE 1
#define INPUTS_AUTOUPDATE_DEF_TIME_MS 5000
#define INPUTS_AUTOUPDATE_MIN_TIME_MS 100
#define INPUTS_AUTOUPDATE_MAX_TIME_MS 30000
#define INPUTS_DEBOUNCE_DEF_TIME_MS 100
#define INPUTS_DEBOUNCE_MIN_TIME_MS 20
#define INPUTS_DEBOUNCE_MAX_TIME_MS 3000
/* =============================
 *   Internal module functions
 * =============================*/
void inp_read_inputs();
void inp_on_timeout();
void inp_notify_inputs_change(const INPUT_STATUS* new_inputs);
uint16_t inp_input_to_mask(uint8_t input);
RET_CODE inp_validate_debounce_time(uint16_t time);
RET_CODE inp_validate_autoupdate_time(uint16_t time);
void inp_notify_callbacks(const INPUT_STATUS* status);
void inp_parse_state(uint16_t state, INPUT_STATUS* buffer);
/* =============================
 *       Internal types
 * =============================*/
typedef struct INPUT_MODULE
{
   INPUTS_CONFIG cfg;
   uint16_t current_inputs;
   uint8_t interrupt_enabled;
   uint16_t debounce_time;
   uint8_t autoupdate_enabled;
   uint16_t autoupdate_time;
   INPUT_STATUS current_states [INPUTS_MAX_INPUT_LINES];

} INPUT_MODULE;
/* =============================
 *      Module variables
 * =============================*/
INPUT_MODULE inp_module;
void (*INP_CALLBACKS[INPUTS_CALLBACK_MAX_SIZE])(INPUT_STATUS status);

RET_CODE inp_initialize(const INPUTS_CONFIG* config)
{
   logger_send(LOG_INPUTS, __func__, "");
   RET_CODE result = RETURN_NOK;
   inp_module.autoupdate_enabled = INPUTS_AUTOUPDATE_STATE;
   inp_module.autoupdate_time = INPUTS_AUTOUPDATE_DEF_TIME_MS;
   inp_module.interrupt_enabled = INPUTS_INTERRUPT_STATE;
   inp_module.debounce_time = INPUTS_DEBOUNCE_DEF_TIME_MS;
   if (config)
   {
      for (uint8_t i = 0; i < INPUTS_MAX_INPUT_LINES; i++)
      {
         inp_module.current_states[i].id = inp_module.cfg.items[i].item;
         inp_module.current_states[i].state = INPUT_STATE_INACTIVE;
      }
      inp_module.cfg = *config;
      /* task for inputs autoupdate */
      result = sch_subscribe_and_set(&inp_read_inputs, TASKPRIO_LOW, inp_module.autoupdate_time,
               inp_module.autoupdate_enabled? TASKSTATE_RUNNING : TASKSTATE_STOPPED, TASKTYPE_PERIODIC);
      if (result == RETURN_OK)
      {
         /* task for debouncing */
         result = sch_subscribe_and_set(&inp_on_timeout, TASKPRIO_LOW, inp_module.debounce_time,
                  inp_module.interrupt_enabled? TASKSTATE_RUNNING : TASKSTATE_STOPPED, TASKTYPE_TRIGGER);
      }
   }
   int_handler_init();
   logger_send_if(result != RETURN_OK, LOG_ERROR, __func__, "error");
   return result;
}
void inp_deinitialize()
{
   sch_unsubscribe(&inp_on_timeout);
   sch_unsubscribe(&inp_read_inputs);
   for (uint8_t i = 0; i < INPUTS_CALLBACK_MAX_SIZE; i++)
   {
      INP_CALLBACKS[i] = NULL;
   }
}

INPUT_STATE inp_get(INPUT_ID id)
{
   INPUT_STATE result = INPUT_STATE_INACTIVE;
   for (uint8_t i = 0; i < INPUTS_MAX_INPUT_LINES; i++)
   {
      if (inp_module.current_states[i].id == id)
      {
         result = inp_module.current_states[i].state;
         break;
      }
   }
   return result;
}
RET_CODE inp_get_all(INPUT_STATUS* buffer)
{
   RET_CODE result = RETURN_ERROR;
   if (buffer)
   {
      for (uint8_t i = 0; i < INPUTS_MAX_INPUT_LINES; i++)
      {
         buffer[i] = inp_module.current_states[i];
      }
      result = RETURN_OK;
   }
   return result;
}
RET_CODE inp_read_all(INPUT_STATUS* buffer)
{
   RET_CODE result = RETURN_NOK;
   uint8_t data [2];
   if (buffer)
   {
      if (i2c_read(inp_module.cfg.address + 1, data, 2) == I2C_STATUS_OK)
      {
         uint16_t new_state = data[0];
         new_state |= (data[1] << 8);
         new_state = ~new_state;
         inp_parse_state(new_state, buffer);
         result = RETURN_OK;
      }
   }
   return result;
}
RET_CODE inp_get_config(INPUTS_CONFIG* buffer)
{
   RET_CODE result = RETURN_ERROR;
   if (buffer)
   {
      *buffer = inp_module.cfg;
      result = RETURN_OK;
   }
   return result;
}
void inp_read_inputs()
{
   logger_send(LOG_INPUTS, __func__, "");
   uint8_t data [2];
   I2C_STATUS result = i2c_read(inp_module.cfg.address + 1, data, 2);

   if (result == I2C_STATUS_OK)
   {
      INPUT_STATUS new_states [INPUTS_MAX_INPUT_LINES];

      uint16_t state = data[0];
      state |= (data[1] << 8);
      state = ~state;
      inp_parse_state(state, new_states);
      inp_notify_inputs_change(new_states);
      inp_module.current_inputs = state;
   }
   else
   {
      logger_send(LOG_ERROR, __func__, "cannot read inputs");
   }
}

void inp_parse_state(uint16_t state, INPUT_STATUS* buffer)
{
   logger_send(LOG_INPUTS, __func__, "current %x new %x", inp_module.current_inputs, state);
   for (uint8_t i = 0; i < INPUTS_MAX_INPUT_LINES; i++)
   {
      uint16_t inp_mask = inp_input_to_mask(inp_module.cfg.items[i].input_no);
      buffer[i].id = inp_module.cfg.items[i].item;
      buffer[i].state = (state & inp_mask) > 0? INPUT_STATE_ACTIVE : INPUT_STATE_INACTIVE;
   }
}

void inp_on_timeout()
{
   logger_send(LOG_INPUTS, __func__, "reading after timeout");
   inp_read_inputs();
}
void inp_notify_inputs_change(const INPUT_STATUS* new_inputs)
{
   for (uint8_t i = 0; i < INPUTS_MAX_INPUT_LINES; i++)
   {
      if (new_inputs[i].state != inp_module.current_states[i].state)
      {
         inp_module.current_states[i] = new_inputs[i];
         logger_send(LOG_INPUTS, __func__, "changed %u to %u", inp_module.cfg.items[i].input_no, inp_module.current_states[i].state);
         inp_notify_callbacks(&inp_module.current_states[i]);
      }
   }
}

void inp_notify_callbacks(const INPUT_STATUS* status)
{
   for (uint8_t i = 0; i < INPUTS_CALLBACK_MAX_SIZE; i++)
   {
      if (INP_CALLBACKS[i])
      {
         INP_CALLBACKS[i](*status);
      }
   }
}

uint16_t inp_input_to_mask(uint8_t input)
{
   uint16_t result = 0;

   if (input < 9){
       result = 1 << (16 - input);
   }
   else
   {
       result = 1 << (input-9);
   }

   return result;
}
RET_CODE inp_validate_debounce_time(uint16_t time)
{
   RET_CODE result = RETURN_NOK;
   if (time >= INPUTS_DEBOUNCE_MIN_TIME_MS && time <= INPUTS_DEBOUNCE_MAX_TIME_MS)
   {
      result = RETURN_OK;
   }
   return result;
}

RET_CODE inp_validate_autoupdate_time(uint16_t time)
{
   RET_CODE result = RETURN_NOK;
   if (time >= INPUTS_AUTOUPDATE_MIN_TIME_MS && time <= INPUTS_AUTOUPDATE_MAX_TIME_MS)
   {
      result = RETURN_OK;
   }
   return result;
}

void inp_enable_interrupt()
{
   logger_send(LOG_INPUTS, __func__, "enabling interrupts");
   inp_module.interrupt_enabled = 1;
}
void inp_disable_interrupt()
{
   logger_send(LOG_INPUTS, __func__, "disabling interrupts");
   inp_module.interrupt_enabled = 0;
}
RET_CODE inp_set_debounce_time(uint16_t time)
{
   RET_CODE result = RETURN_NOK;
   if (inp_validate_debounce_time(time))
   {
      if (sch_set_task_period(&inp_on_timeout, time) == RETURN_OK)
      {
         result = RETURN_OK;
         inp_module.debounce_time = time;
         logger_send(LOG_INPUTS, __func__, "new debounce time %u", time);
      }
   }
   logger_send_if(result != RETURN_OK, LOG_ERROR, __func__, "invalid time %d", time);
   return result;
}
uint16_t inp_get_debounce_time()
{
   return inp_module.debounce_time;
}
uint16_t inp_get_periodic_update_time()
{
   return inp_module.autoupdate_time;
}
RET_CODE inp_set_periodic_update_time(uint16_t period)
{
   RET_CODE result = RETURN_NOK;
   if (inp_validate_autoupdate_time(period))
   {
      if (sch_set_task_period(&inp_read_inputs, period) == RETURN_OK)
      {
         result = RETURN_OK;
         inp_module.autoupdate_time = period;
         logger_send(LOG_INPUTS, __func__, "new autoupdate time %u", period);
      }
   }
   logger_send_if(result != RETURN_OK, LOG_ERROR, __func__, "invalid time %d", period);
   return result;
}
void inp_enable_periodic_update()
{
   inp_module.autoupdate_enabled = 1;
   sch_set_task_state(&inp_read_inputs, TASKSTATE_RUNNING);
   logger_send(LOG_INPUTS, __func__, "enabling autoupdate");
}
void inp_disable_periodic_update()
{
   inp_module.autoupdate_enabled = 0;
   sch_set_task_state(&inp_read_inputs, TASKSTATE_STOPPED);
   logger_send(LOG_INPUTS, __func__, "disabling autoupdate");
}
RET_CODE inp_add_input_listener(INPUT_LISTENER callback)
{
   RET_CODE result = RETURN_NOK;
   for (uint8_t i = 0; i < INPUTS_CALLBACK_MAX_SIZE; i++)
   {
      if (INP_CALLBACKS[i] == NULL)
      {
         INP_CALLBACKS[i] = callback;
         result = RETURN_OK;
         break;
      }
   }
   return result;
}

void inp_remove_input_listener(INPUT_LISTENER callback)
{
   for (uint8_t i = 0; i < INPUTS_CALLBACK_MAX_SIZE; i++)
   {
      if (INP_CALLBACKS[i] == callback)
      {
         INP_CALLBACKS[i] = NULL;
         break;
      }
   }
}

void inp_on_interrupt_recevied()
{
   if (inp_module.interrupt_enabled)
   {
      sch_trigger_task(&inp_on_timeout);
   }
}
