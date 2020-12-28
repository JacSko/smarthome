#ifndef ENV_MONITOR_H_
#define ENV_MONITOR_H_

/* ============================= */
/**
 * @file env_monitor.h
 *
 * @brief Module responsible for handling temperature and humidity measurement from multiple sensors
 *
 * @details
 * This module is responsible for gathering temperature and humidity data from outside and all rooms inside.
 * It is done via DHT driver.
 *
 * @author Jacek Skowronek
 * @date 27/12/2020
 */
/* ============================= */

/* =============================
 *          Defines
 * =============================*/
#define ENV_SENSORS_COUNT 6
/* =============================
 *   Includes of common headers
 * =============================*/

/* =============================
 *  Includes of project headers
 * =============================*/
#include "dht_driver.h"
/* =============================
 *      Global variables
 * =============================*/

/* =============================
 *       Data structures
 * =============================*/
typedef enum ENV_EVENT
{
   ENV_EV_NEW_DATA,
   ENV_EV_ERROR,
} ENV_EVENT;
typedef enum ENV_ITEM_ID
{
   ENV_UNKNOWN_ITEM,
   ENV_OUTSIDE,
   ENV_WARDROBE,
   ENV_BEDROOM,
   ENV_BATHROOM,
   ENV_KITCHEN,
   ENV_STAIRS,
} ENV_ITEM_ID;

typedef struct ENV_ERROR_RATE
{
   uint8_t nr_err_rate;
   uint8_t cs_err_rate;
} ENV_ERROR_RATE;

typedef struct ENV_DHT_MATCH
{
   ENV_ITEM_ID env_id;
   DHT_SENSOR_ID dht_id;
} ENV_DHT_MATCH;

typedef struct ENV_CONFIG
{
   uint8_t measure_running;
   uint8_t max_nr_rate;
   uint8_t max_cs_rate;
   ENV_DHT_MATCH items [ENV_SENSORS_COUNT];
} ENV_CONFIG;

typedef void(*ENV_CALLBACK)(ENV_EVENT event, ENV_ITEM_ID id,  const DHT_SENSOR*);

/**
 * @brief Initialize the ENV module.
 * @param[in] config - Configuration of ENV module
 * @return See RETURN_CODES.
 */
RET_CODE env_initialize(const ENV_CONFIG* cfg);
/**
 * @brief Deinitialize the ENV module.
 * @return None.
 */
void env_deinitialize();
/**
 * @brief Read data from defined sensor.
 * @param[in] id - the id of the sensor to be read
 * @param[out] buffer - place, where data will be written
 * @return Temperature and humidity data.
 */
RET_CODE env_read_sensor(ENV_ITEM_ID id, DHT_SENSOR* buffer);
/**
 * @brief Get current error stats for sensor.
 * @param[in] id - the id of the sensor.
 * @return Error rates data.
 */
ENV_ERROR_RATE env_get_error_stats(ENV_ITEM_ID id);
/**
 * @brief Set time between next sensor reading.
 * @param[in] period - the period between measurements in ms.
 * @return See RETURN_CODES.
 */
RET_CODE env_set_measurement_period(uint16_t period);
/**
 * @brief Get the measurement period.
 * @return Measurement period in ms.
 */
uint16_t env_get_measurement_period();
/**
 * @brief Register sensor data listener.
 * @param[in] callback - pointer to function to be called.
 * @param[in] id - the ID of sensor for which callback should be called
 * @return See RETURN_CODES.
 */
RET_CODE env_register_listener(ENV_CALLBACK callback, ENV_ITEM_ID id);
/**
 * @brief Unregister sensor data listener.
 * @param[in] callback - pointer to function.
 * @param[in] id - the ID of sensor.
 * @return None.
 */
void env_unregister_listener(ENV_CALLBACK callback, ENV_ITEM_ID id);





#endif
