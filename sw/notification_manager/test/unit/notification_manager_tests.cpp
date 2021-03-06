#include "gtest/gtest.h"
#include "gmock/gmock.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "../../source/notification_manager.c"
#ifdef __cplusplus
}
#endif

#include "inputs_board_mock.h"
#include "relays_board_mock.h"
#include "bathroom_fan_mock.h"
#include "stairs_led_module_mock.h"
#include "env_monitor_mock.h"
#include "time_counter_mock.h"
#include "wifimanager_mock.h"
#include "logger_mock.h"

/* ============================= */
/**
 * @file notification_manager_tests.cpp
 *
 * @brief Unit tests of Notification manager module
 *
 * @details
 * This tests verifies behavior of Notification manager module
 *
 * @author Jacek Skowronek
 * @date 23/01/2021
 */
/* ============================= */


using namespace ::testing;

struct ntfmgrFixture : public ::testing::Test
{
   virtual void SetUp()
   {
      mock_env_init();
      mock_inp_init();
      mock_rel_init();
      mock_fan_init();
      mock_slm_init();
      mock_time_counter_init();
      mock_wifimgr_init();
      mock_logger_init();
      EXPECT_CALL(*inp_mock, inp_add_input_listener(_)).WillOnce(Return(RETURN_OK));
      EXPECT_CALL(*env_mock, env_register_listener(_, _)).Times(ENV_SENSORS_COUNT).WillRepeatedly(Return(RETURN_OK));
      EXPECT_CALL(*rel_mock, rel_add_listener(_)).WillOnce(Return(RETURN_OK));
      EXPECT_CALL(*fan_mock, fan_add_listener(_)).WillOnce(Return(RETURN_OK));
      EXPECT_CALL(*slm_mock, slm_add_listener(_)).WillOnce(Return(RETURN_OK));
      EXPECT_CALL(*wifimgr_mock, wifimgr_register_data_callback(_)).WillOnce(Return(RETURN_OK));
      ntfmgr_init();
   }

   virtual void TearDown()
   {
      mock_env_deinit();
      mock_inp_deinit();
      mock_rel_deinit();
      mock_fan_deinit();
      mock_slm_deinit();
      mock_time_counter_deinit();
      mock_wifimgr_deinit();
      mock_logger_deinit();
   }
};

/**
 * @test Initialization of Notification manager module
 */
TEST(ntf_mgr, initialization)
{
   mock_env_init();
   mock_inp_init();
   mock_rel_init();
   mock_fan_init();
   mock_slm_init();
   mock_wifimgr_init();
   mock_logger_init();

   /**
    * <b>scenario</b>: Module initialization.<br>
    * <b>expected</b>: Module registers callbacks in all modules.<br>
    * ************************************************
    */
   EXPECT_CALL(*inp_mock, inp_add_input_listener(_)).WillOnce(Return(RETURN_OK));
   EXPECT_CALL(*env_mock, env_register_listener(_, _)).Times(ENV_SENSORS_COUNT).WillRepeatedly(Return(RETURN_OK));
   EXPECT_CALL(*rel_mock, rel_add_listener(_)).WillOnce(Return(RETURN_OK));
   EXPECT_CALL(*fan_mock, fan_add_listener(_)).WillOnce(Return(RETURN_OK));
   EXPECT_CALL(*slm_mock, slm_add_listener(_)).WillOnce(Return(RETURN_OK));
   EXPECT_CALL(*wifimgr_mock, wifimgr_register_data_callback(_)).WillOnce(Return(RETURN_OK));

   EXPECT_EQ(RETURN_OK, ntfmgr_init());

   mock_env_deinit();
   mock_inp_deinit();
   mock_rel_deinit();
   mock_fan_deinit();
   mock_slm_deinit();
   mock_wifimgr_deinit();

}

/**
 * @test This test case covers sending periodic notifications on particular module event
 */
TEST_F(ntfmgrFixture, periodic_notification_tests)
{
   /**
    * <b>scenario</b>: Event from Relays module received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      RELAY_STATUS status = {RELAY_BEDROOM_AC, RELAY_STATE_ON};
      const char* exp = "05 02 02 10 01\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_broadcast_bytes(_,_)).WillOnce(Invoke([&](const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      ntfmgr_on_relays_change(&status);
   }
   /**
    * <b>scenario</b>: Event from Inputs module received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      INPUT_STATUS status = {INPUT_BEDROOM_AC, INPUT_STATE_ACTIVE};
      const char* exp = "03 02 02 02 01\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_broadcast_bytes(_,_)).WillOnce(Invoke([&](const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      ntfmgr_on_inputs_change(status);
   }
   /**
    * <b>scenario</b>: Event from ENV module received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      DHT_SENSOR sensor = {DHT_SENSOR1, DHT_TYPE_DHT22, {22, 5, 60, 1}};
      const char* exp = "07 02 06 05 01 60 01 22 05\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_broadcast_bytes(_,_)).WillOnce(Invoke([&](const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      ntfmgr_on_env_change(ENV_EV_NEW_DATA, ENV_KITCHEN, &sensor);
   }
   /**
    * <b>scenario</b>: Event from FAN module received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* exp = "09 02 01 02\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_broadcast_bytes(_,_)).WillOnce(Invoke([&](const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      ntfmgr_on_fan_change(FAN_STATE_SUSPEND);
   }
   /**
    * <b>scenario</b>: Event from SLM module received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* exp = "10 02 01 05\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_broadcast_bytes(_,_)).WillOnce(Invoke([&](const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      ntfmgr_on_slm_change(SLM_STATE_ONGOING_OFF);
   }
}

/**
 * @test This test case covers commands related to System module
 */
TEST_F(ntfmgrFixture, system_module_commands)
{
   /**
    * <b>scenario</b>: Set SYSTEM_TIME command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "01 01 07 15 11 07 229 10 11 12";
      const char* exp = "01 01 01 01\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*time_cnt_mock, time_set_utc(_)).WillOnce(Invoke([&](TimeItem* time) -> RET_CODE
      {
         EXPECT_EQ(time->day, 15);
         EXPECT_EQ(time->month, 11);
         EXPECT_EQ(time->year, 2021);
         EXPECT_EQ(time->hour, 10);
         EXPECT_EQ(time->minute, 11);
         EXPECT_EQ(time->second, 12);
         return RETURN_OK;
      }));
      ntfmgr_parse_request(1, (char*) command);
   }

   /**
    * <b>scenario</b>: Set SYSTEM_TIME command received, but cannot set.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "01 01 07 15 11 07 229 10 11 12";
      const char* exp = "01 01 01 02\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*time_cnt_mock, time_set_utc(_)).WillOnce(Return(RETURN_NOK));
      ntfmgr_parse_request(1, (char*) command);
   }

   /**
    * <b>scenario</b>: Get SYSTEM_TIME command received, but cannot to get time<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "01 00 00";
      const char* exp = "01 00 01 02\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));

      EXPECT_CALL(*time_cnt_mock, time_get()).WillOnce(Return(nullptr));
      ntfmgr_parse_request(1, (char*) command);
   }

   /**
    * <b>scenario</b>: Get SYSTEM_TIME command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "01 00 00";
      const char* exp = "01 00 08 01 15 11 07 229 10 11 12\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));

      TimeItem item = {15, 11, 2021, 10, 11, 12};
      EXPECT_CALL(*time_cnt_mock, time_get()).WillOnce(Return(&item));
      ntfmgr_parse_request(1, (char*) command);
   }
}

/**
 * @test This test case covers commands related to Relays module
 */
TEST_F(ntfmgrFixture, relays_module_commands)
{
   /**
    * <b>scenario</b>: Set RELAYS_STATE command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "05 01 02 03 01";
      const char* exp = "05 01 01 01\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*rel_mock, rel_set(RELAY_BATHROOM_LED, RELAY_STATE_ON)).WillOnce(Return(RETURN_OK));
      ntfmgr_parse_request(1, (char*) command);
   }

   /**
    * <b>scenario</b>: Set RELAYS_STATE_ALL command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "06 01 10 04 01 06 01 10 01 09 01 02 01";
      const char* exp = "06 01 01 01\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*rel_mock, rel_set(RELAY_BATHROOM_AC, RELAY_STATE_ON)).WillOnce(Return(RETURN_OK));
      EXPECT_CALL(*rel_mock, rel_set(RELAY_STAIRCASE_AC, RELAY_STATE_ON)).WillOnce(Return(RETURN_OK));
      EXPECT_CALL(*rel_mock, rel_set(RELAY_BEDROOM_AC, RELAY_STATE_ON)).WillOnce(Return(RETURN_OK));
      EXPECT_CALL(*rel_mock, rel_set(RELAY_KITCHEN_AC, RELAY_STATE_ON)).WillOnce(Return(RETURN_OK));
      EXPECT_CALL(*rel_mock, rel_set(RELAY_WARDROBE_AC, RELAY_STATE_ON)).WillOnce(Return(RETURN_OK));
      ntfmgr_parse_request(1, (char*) command);
   }

   /**
    * <b>scenario</b>: Get RELAYS_STATE command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "05 00 01 07";
      const char* exp = "05 00 02 01 01\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*rel_mock, rel_get(RELAY_SOCKETS)).WillOnce(Return(RELAY_STATE_ON));
      ntfmgr_parse_request(1, (char*) command);
   }

   /**
    * <b>scenario</b>: Get RELAYS_STATE_ALL command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "06 00 00";
      const char* exp = "06 00 33 01 04 01 06 01 12 01 10 01 09 01 02 01 12 01 12 01 12 01 12 01 12 01 12 01 12 01 12 01 12 01 12 01\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*rel_mock, rel_get_all(_)).WillOnce(Invoke([&](RELAY_STATUS* status)
            {
                  status[0].id = RELAY_BATHROOM_AC; status[0].state = RELAY_STATE_ON;
                  status[1].id = RELAY_STAIRCASE_AC; status[1].state = RELAY_STATE_ON;
                  status[2].id = RELAY_ID_ENUM_MAX; status[2].state = RELAY_STATE_ON;
                  status[3].id = RELAY_BEDROOM_AC; status[3].state = RELAY_STATE_ON;
                  status[4].id = RELAY_KITCHEN_AC; status[4].state = RELAY_STATE_ON;
                  status[5].id = RELAY_WARDROBE_AC; status[5].state = RELAY_STATE_ON;
                  status[6].id = RELAY_ID_ENUM_MAX; status[6].state = RELAY_STATE_ON;
                  status[7].id = RELAY_ID_ENUM_MAX; status[7].state = RELAY_STATE_ON;
                  status[8].id = RELAY_ID_ENUM_MAX; status[8].state = RELAY_STATE_ON;
                  status[9].id = RELAY_ID_ENUM_MAX; status[9].state = RELAY_STATE_ON;
                  status[10].id = RELAY_ID_ENUM_MAX; status[10].state = RELAY_STATE_ON;
                  status[11].id = RELAY_ID_ENUM_MAX; status[11].state = RELAY_STATE_ON;
                  status[12].id = RELAY_ID_ENUM_MAX; status[12].state = RELAY_STATE_ON;
                  status[13].id = RELAY_ID_ENUM_MAX; status[13].state = RELAY_STATE_ON;
                  status[14].id = RELAY_ID_ENUM_MAX; status[14].state = RELAY_STATE_ON;
                  status[15].id = RELAY_ID_ENUM_MAX; status[15].state = RELAY_STATE_ON;
                  return RETURN_OK;
            }));
      ntfmgr_parse_request(1, (char*) command);
   }
}

/**
 * @test This test case covers commands related to Inputs module
 */
TEST_F(ntfmgrFixture, inputs_module_commands)
{
   /**
    * <b>scenario</b>: Get INPUTS_STATE command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "03 00 01 02";
      const char* exp = "03 00 02 01 01\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*inp_mock, inp_get(INPUT_BEDROOM_AC)).WillOnce(Return(INPUT_STATE_ACTIVE));
      ntfmgr_parse_request(1, (char*) command);

   }

   /**
    * <b>scenario</b>: Get INPUTS_STATE_ALL command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "04 00 00";
      const char* exp = "04 00 33 01 03 01 07 01 10 01 02 01 05 01 00 01 10 01 10 01 10 01 10 01 10 01 10 01 10 01 10 01 10 01 10 01\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*inp_mock, inp_get_all(_)).WillOnce(Invoke([&](INPUT_STATUS* status)
      {
            status[0].id = INPUT_BATHROOM_AC; status[0].state = INPUT_STATE_ACTIVE;
            status[1].id = INPUT_STAIRS_AC; status[1].state = INPUT_STATE_ACTIVE;
            status[2].id = INPUT_ENUM_COUNT; status[2].state = INPUT_STATE_ACTIVE;
            status[3].id = INPUT_BEDROOM_AC; status[3].state = INPUT_STATE_ACTIVE;
            status[4].id = INPUT_KITCHEN_AC; status[4].state = INPUT_STATE_ACTIVE;
            status[5].id = INPUT_WARDROBE_AC; status[5].state = INPUT_STATE_ACTIVE;
            status[6].id = INPUT_ENUM_COUNT; status[6].state = INPUT_STATE_ACTIVE;
            status[7].id = INPUT_ENUM_COUNT; status[7].state = INPUT_STATE_ACTIVE;
            status[8].id = INPUT_ENUM_COUNT; status[8].state = INPUT_STATE_ACTIVE;
            status[9].id = INPUT_ENUM_COUNT; status[9].state = INPUT_STATE_ACTIVE;
            status[10].id = INPUT_ENUM_COUNT; status[10].state = INPUT_STATE_ACTIVE;
            status[11].id = INPUT_ENUM_COUNT; status[11].state = INPUT_STATE_ACTIVE;
            status[12].id = INPUT_ENUM_COUNT; status[12].state = INPUT_STATE_ACTIVE;
            status[13].id = INPUT_ENUM_COUNT; status[13].state = INPUT_STATE_ACTIVE;
            status[14].id = INPUT_ENUM_COUNT; status[14].state = INPUT_STATE_ACTIVE;
            status[15].id = INPUT_ENUM_COUNT; status[15].state = INPUT_STATE_ACTIVE;
            return RETURN_OK;
      }));
      ntfmgr_parse_request(1, (char*) command);
   }

}


/**
 * @test This test case covers commands related to Fan module
 */
TEST_F(ntfmgrFixture, fan_module_commands)
{
   /**
    * <b>scenario</b>: Get FAN_STATE command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "09 00 00";
      const char* exp = "09 00 02 01 01\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*fan_mock, fan_get_state()).WillOnce(Return(FAN_STATE_ON));
      ntfmgr_parse_request(1, (char*) command);

   }

   /**
    * <b>scenario</b>: Set FAN_STATE command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "09 01 01 01";
      const char* exp = "09 01 01 02\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*fan_mock, fan_start()).WillOnce(Return(RETURN_NOK));
      ntfmgr_parse_request(1, (char*) command);
   }

   {
      const char* command = "09 01 01 00";
      const char* exp = "09 01 01 02\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*fan_mock, fan_stop()).WillOnce(Return(RETURN_NOK));
      ntfmgr_parse_request(1, (char*) command);
   }

}

/**
 * @test This test case covers commands related to SLM module
 */
TEST_F(ntfmgrFixture, slm_module_commands)
{
   /**
    * <b>scenario</b>: Get SLM_STATE command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "10 00 00";
      const char* exp = "10 00 02 01 05\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*slm_mock, slm_get_state()).WillOnce(Return(SLM_STATE_ONGOING_OFF));
      ntfmgr_parse_request(1, (char*) command);

   }

   /**
    * <b>scenario</b>: Get SLM_PROGRAM_ID command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "11 00 00";
      const char* exp = "11 00 02 01 02\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*slm_mock, slm_get_current_program_id()).WillOnce(Return(SLM_PROGRAM3));
      ntfmgr_parse_request(1, (char*) command);

   }

   /**
    * <b>scenario</b>: Set SLM_STATE command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "10 01 01 02";
      const char* exp = "10 01 01 01\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*slm_mock, slm_start_program()).WillOnce(Return(RETURN_OK));
      ntfmgr_parse_request(1, (char*) command);
   }

   /**
    * <b>scenario</b>: Set SLM_STATE command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "11 01 01 01";
      const char* exp = "11 01 01 01\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*slm_mock, slm_set_current_program_id(SLM_PROGRAM2)).WillOnce(Return(RETURN_OK));
      ntfmgr_parse_request(1, (char*) command);
   }
}

/**
 * @test This test case covers commands related to ENV module
 */
TEST_F(ntfmgrFixture, env_module_commands)
{
   /**
    * <b>scenario</b>: Get ENV_SENSOR_DATA command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "07 00 01 01";
      const char* exp = "07 00 06 01 01 10 20 30 40\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*env_mock, env_get_sensor_data(ENV_OUTSIDE, _)).WillOnce(Invoke([&](ENV_ITEM_ID id, DHT_SENSOR* buffer) -> RET_CODE
      {
         buffer->type = DHT_TYPE_DHT22;
         buffer->data.hum_h = 10;
         buffer->data.hum_l = 20;
         buffer->data.temp_h = 30;
         buffer->data.temp_l = 40;
         return RETURN_OK;
      }));
      ntfmgr_parse_request(1, (char*) command);
   }

   /**
    * <b>scenario</b>: Get ENV_SENSOR_ERROR command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "08 00 01 01";
      const char* exp = "08 00 03 01 20 30\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      ENV_ERROR_RATE sensor_error = {30, 20};
      EXPECT_CALL(*env_mock, env_get_error_stats(ENV_OUTSIDE)).WillOnce(Return(sensor_error));
      ntfmgr_parse_request(1, (char*) command);
   }
}

/**
 * @test This test case covers commands related to System get status.
 */
TEST_F(ntfmgrFixture, system_status_commands)
{
   /**
    * <b>scenario</b>: Get system status command received.<br>
    * <b>expected</b>: Correct NTF message sent.<br>
    * ************************************************
    */
   {
      const char* command = "02 00 00";
      const char* exp = "02 00 100 01 32 02 01 07 01 10 01 03 01 05 01 10 01 10 01 10 01 10 01 10 01 10 01 10 01 10 01 10 01 10 01 10 01 "
                                     "32 04 01 06 01 12 01 10 01 12 01 12 01 12 01 12 01 12 01 12 01 12 01 12 01 12 01 12 01 12 01 12 01 "
                                     "30 04 01 02 03 04 05 05 06 07 08 03 09 10 11 12 06 13 14 15 16 02 17 18 19 20 01 21 22 23 24 "
                                     "01 02\n";
      EXPECT_CALL(*wifimgr_mock, wifimgr_send_bytes(1, _,_)).WillOnce(Invoke([&](ServerClientID, const uint8_t* data, uint16_t size) -> RET_CODE
      {
         EXPECT_STREQ((char*)data, exp);
         return RETURN_OK;
      }));
      EXPECT_CALL(*fan_mock, fan_get_state()).WillOnce(Return(FAN_STATE_SUSPEND));
      EXPECT_CALL(*inp_mock, inp_get_all(_)).WillOnce(Invoke([&](INPUT_STATUS* status)
      {
            status[0].id = INPUT_BEDROOM_AC; status[0].state = INPUT_STATE_ACTIVE;
            status[1].id = INPUT_STAIRS_AC; status[1].state = INPUT_STATE_ACTIVE;
            status[2].id = INPUT_ENUM_COUNT; status[2].state = INPUT_STATE_ACTIVE;
            status[3].id = INPUT_BATHROOM_AC; status[3].state = INPUT_STATE_ACTIVE;
            status[4].id = INPUT_KITCHEN_AC; status[4].state = INPUT_STATE_ACTIVE;
            status[5].id = INPUT_ENUM_COUNT; status[5].state = INPUT_STATE_ACTIVE;
            status[6].id = INPUT_ENUM_COUNT; status[6].state = INPUT_STATE_ACTIVE;
            status[7].id = INPUT_ENUM_COUNT; status[7].state = INPUT_STATE_ACTIVE;
            status[8].id = INPUT_ENUM_COUNT; status[8].state = INPUT_STATE_ACTIVE;
            status[9].id = INPUT_ENUM_COUNT; status[9].state = INPUT_STATE_ACTIVE;
            status[10].id = INPUT_ENUM_COUNT; status[10].state = INPUT_STATE_ACTIVE;
            status[11].id = INPUT_ENUM_COUNT; status[11].state = INPUT_STATE_ACTIVE;
            status[12].id = INPUT_ENUM_COUNT; status[12].state = INPUT_STATE_ACTIVE;
            status[13].id = INPUT_ENUM_COUNT; status[13].state = INPUT_STATE_ACTIVE;
            status[14].id = INPUT_ENUM_COUNT; status[14].state = INPUT_STATE_ACTIVE;
            status[15].id = INPUT_ENUM_COUNT; status[15].state = INPUT_STATE_ACTIVE;
            return RETURN_OK;
      }));
      EXPECT_CALL(*rel_mock, rel_get_all(_)).WillOnce(Invoke([&](RELAY_STATUS* status)
      {
            status[0].id = RELAY_BATHROOM_AC; status[0].state = RELAY_STATE_ON;
            status[1].id = RELAY_STAIRCASE_AC; status[1].state = RELAY_STATE_ON;
            status[2].id = RELAY_ID_ENUM_MAX; status[2].state = RELAY_STATE_ON;
            status[3].id = RELAY_BEDROOM_AC; status[3].state = RELAY_STATE_ON;
            status[4].id = RELAY_ID_ENUM_MAX; status[4].state = RELAY_STATE_ON;
            status[5].id = RELAY_ID_ENUM_MAX; status[5].state = RELAY_STATE_ON;
            status[6].id = RELAY_ID_ENUM_MAX; status[6].state = RELAY_STATE_ON;
            status[7].id = RELAY_ID_ENUM_MAX; status[7].state = RELAY_STATE_ON;
            status[8].id = RELAY_ID_ENUM_MAX; status[8].state = RELAY_STATE_ON;
            status[9].id = RELAY_ID_ENUM_MAX; status[9].state = RELAY_STATE_ON;
            status[10].id = RELAY_ID_ENUM_MAX; status[10].state = RELAY_STATE_ON;
            status[11].id = RELAY_ID_ENUM_MAX; status[11].state = RELAY_STATE_ON;
            status[12].id = RELAY_ID_ENUM_MAX; status[12].state = RELAY_STATE_ON;
            status[13].id = RELAY_ID_ENUM_MAX; status[13].state = RELAY_STATE_ON;
            status[14].id = RELAY_ID_ENUM_MAX; status[14].state = RELAY_STATE_ON;
            status[15].id = RELAY_ID_ENUM_MAX; status[15].state = RELAY_STATE_ON;
            return RETURN_OK;
      }));
      EXPECT_CALL(*env_mock, env_get_config(_)).WillOnce(Invoke([&](ENV_CONFIG* cfg) -> RET_CODE
      {
         cfg->items[0] = {ENV_BATHROOM, DHT_SENSOR1};
         cfg->items[1] = {ENV_KITCHEN, DHT_SENSOR2};
         cfg->items[2] = {ENV_BEDROOM, DHT_SENSOR3};
         cfg->items[3] = {ENV_STAIRS, DHT_SENSOR4};
         cfg->items[4] = {ENV_WARDROBE, DHT_SENSOR5};
         cfg->items[5] = {ENV_OUTSIDE, DHT_SENSOR6};
         return RETURN_OK;
      }));
      EXPECT_CALL(*env_mock, env_get_sensor_data(_, _))
      .WillOnce(Invoke([&](ENV_ITEM_ID id, DHT_SENSOR* buffer) -> RET_CODE
      {
         buffer->type = DHT_TYPE_DHT22;
         buffer->data.hum_h = 1;
         buffer->data.hum_l = 2;
         buffer->data.temp_h = 3;
         buffer->data.temp_l = 4;
         return RETURN_OK;
      }))
      .WillOnce(Invoke([&](ENV_ITEM_ID id, DHT_SENSOR* buffer) -> RET_CODE
      {
         buffer->type = DHT_TYPE_DHT22;
         buffer->data.hum_h = 5;
         buffer->data.hum_l = 6;
         buffer->data.temp_h = 7;
         buffer->data.temp_l = 8;
         return RETURN_OK;
      }))
      .WillOnce(Invoke([&](ENV_ITEM_ID id, DHT_SENSOR* buffer) -> RET_CODE
      {
         buffer->type = DHT_TYPE_DHT22;
         buffer->data.hum_h = 9;
         buffer->data.hum_l = 10;
         buffer->data.temp_h = 11;
         buffer->data.temp_l = 12;
         return RETURN_OK;
      }))
      .WillOnce(Invoke([&](ENV_ITEM_ID id, DHT_SENSOR* buffer) -> RET_CODE
      {
         buffer->type = DHT_TYPE_DHT22;
         buffer->data.hum_h = 13;
         buffer->data.hum_l = 14;
         buffer->data.temp_h = 15;
         buffer->data.temp_l = 16;
         return RETURN_OK;
      }))
      .WillOnce(Invoke([&](ENV_ITEM_ID id, DHT_SENSOR* buffer) -> RET_CODE
      {
         buffer->type = DHT_TYPE_DHT22;
         buffer->data.hum_h = 17;
         buffer->data.hum_l = 18;
         buffer->data.temp_h = 19;
         buffer->data.temp_l = 20;
         return RETURN_OK;
      }))
      .WillOnce(Invoke([&](ENV_ITEM_ID id, DHT_SENSOR* buffer) -> RET_CODE
      {
         buffer->type = DHT_TYPE_DHT22;
         buffer->data.hum_h = 21;
         buffer->data.hum_l = 22;
         buffer->data.temp_h = 23;
         buffer->data.temp_l = 24;
         return RETURN_OK;
      }));
      ntfmgr_parse_request(1, command);
   }
}
