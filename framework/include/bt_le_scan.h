/****************************************************************************
 *  Copyright (C) 2022 Xiaomi Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ***************************************************************************/
#ifndef __BT_LE_SCAN_H_
#define __BT_LE_SCAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "bluetooth.h"
#include "bt_le_advertiser.h"
#ifndef BTSYMBOLS
#define BTSYMBOLS(s) s
#endif

#define BLE_SCAN_FILTER_UUID_MAX_NUM 2

/**
 * @cond
 */

/**
 * @brief Scan start status code
 *
 */
enum {
    BT_SCAN_STATUS_SUCCESS = 0,
    BT_SCAN_STATUS_START_FAIL,
    BT_SCAN_STATUS_NO_PERMISSION,
    BT_SCAN_STATUS_SCANNER_REG_NOMEM,
    BT_SCAN_STATUS_SCANNER_EXISTED,
    BT_SCAN_STATUS_SCANNER_NOT_FOUND,
    BT_SCAN_STATUS_SCANNER_REMOVED
};

/**
 * @brief Scan mode
 *
 */
enum {
    BT_SCAN_MODE_LOW_POWER = 0,
    BT_SCAN_MODE_BALANCED,
    BT_SCAN_MODE_LOW_LATENCY,
};

#define SCAN_MODE_LOW_POWER_INTERVAL 0x1000
#define SCAN_MODE_LOW_POWER_WINDOW 0x100
#define SCAN_MODE_BALANCED_INTERVAL 0x500
#define SCAN_MODE_BALANCED_WINDOW 0x140
#define SCAN_MODE_LOW_LATENCY_INTERVAL 0xA0
#define SCAN_MODE_LOW_LATENCY_WINDOW 0xA0

typedef void bt_scanner_t;

#ifdef CONFIG_BLUETOOTH_STACK_LE_ZBLUE
#define BT_LE_SCAN_TYPE_PASSIVE BT_LE_SCAN_TYPE_PASSIVE_MODE
#define BT_LE_SCAN_TYPE_ACTIVE BT_LE_SCAN_TYPE_ACTIVE_MODE
typedef enum {
    BT_LE_SCAN_TYPE_PASSIVE_MODE = 0,
    BT_LE_SCAN_TYPE_ACTIVE_MODE
} ble_scan_type_t;
#else
typedef enum {
    BT_LE_SCAN_TYPE_PASSIVE = 0,
    BT_LE_SCAN_TYPE_ACTIVE
} ble_scan_type_t;
#endif

/**
 * @brief Scan result structure
 *
 */
typedef struct {
    bt_address_t addr;
    uint8_t dev_type; /* bt_device_type_t */
    int8_t rssi;
    uint8_t addr_type; /* ble_addr_type_t */
    uint8_t adv_type; /* ble_adv_type_t */
    uint8_t length;
    uint8_t pad[1];
    uint8_t adv_data[1];
} ble_scan_result_t;

/**
 * @brief Scan filter policy structure
 *
 */
typedef struct {
    uint8_t policy;
} ble_scan_filter_policy_t;

/**
 * @brief Scan settings structure
 *
 */
typedef struct {
    uint8_t scan_mode;
    uint8_t legacy;
    uint8_t scan_type; /* ble_scan_type_t */
    uint8_t scan_phy; /* ble_phy_type_t */
    ble_scan_filter_policy_t policy;
} ble_scan_settings_t;

/**
 * @brief Useless
 *
 */
typedef struct {
    int scan_interval;
    int scan_window;
    ble_scan_type_t scan_type;
    ble_phy_type_t scan_phy;
} ble_scan_params_t;

typedef struct {
    uint32_t duration;
    uint32_t period;
    uint16_t uuids[BLE_SCAN_FILTER_UUID_MAX_NUM];
    uint8_t active;
    uint8_t duplicated;
} ble_scan_filter_t;
/**
 * @endcond
 */

/**
 * @brief Scan result callback function.
 *
 * Callback function called when a scan result is available.
 *
 * @param scanner - Scanner handle.
 * @param result - Pointer to the scan result, see @ref ble_scan_result_t.
 *
 * **Example:**
 * @code
void le_scan_result_callback(bt_scanner_t* scanner, ble_scan_result_t* result)
{
    // Process scan result
}

static scanner_callbacks_t lescan_cbs = {
    .size = sizeof(lescan_cbs),
    .on_scan_result = le_scan_result_callback,
};
 * @endcode
 */
typedef void (*on_scan_result_cb_t)(bt_scanner_t* scanner, ble_scan_result_t* result);

/**
 * @brief Scan start status callback function.
 *
 * Callback function called when the scan starts or fails to start.
 *
 * @param scanner - Scanner handle.
 * @param status - Scan start status code, see scan status codes.
 *
 * **Example:**
 * @code
void on_scan_status(bt_scanner_t* scanner, uint8_t status)
{
    if (status == BT_SCAN_STATUS_SUCCESS) {
        // Scan started successfully
    } else {
        // Handle scan start failure
    }
}
 * @endcode
 */
typedef void (*on_scan_status_cb_t)(bt_scanner_t* scanner, uint8_t status);

/**
 * @brief Scan stopped callback function.
 *
 * Callback function called when the scan is stopped.
 *
 * @param scanner - Scanner handle.
 *
 * **Example:**
 * @code
void on_scan_stopped(bt_scanner_t* scanner)
{
    // Handle scan stopped event
}
 * @endcode
 */
typedef void (*on_scan_stopped_cb_t)(bt_scanner_t* scanner);

/**
 * @cond
 */

/**
 * @brief Scanner callback structure
 *
 */
typedef struct {
    uint32_t size;
    on_scan_result_cb_t on_scan_result;
    on_scan_status_cb_t on_scan_start_status;
    on_scan_stopped_cb_t on_scan_stopped;
} scanner_callbacks_t;
/**
 * @endcond
 */

/**
 * @brief Start BLE scan.
 *
 * Initiates a BLE scan with default settings.
 *
 * @param ins - Bluetooth client instance, see @ref bt_instance_t.
 * @param cbs - Pointer to scanner callbacks, see @ref scanner_callbacks_t.
 * @return bt_scanner_t* - Scanner handle generated by the scan manager.
 *
 * **Example:**
 * @code
bt_scanner_t* scanner = bt_le_start_scan(ins, &my_scanner_callbacks);
if (scanner == NULL) {
    // Handle error
}
 * @endcode
 */
bt_scanner_t* BTSYMBOLS(bt_le_start_scan)(bt_instance_t* ins, const scanner_callbacks_t* cbs);

/**
 * @brief Start BLE scan with specific settings.
 *
 * Initiates a BLE scan with provided scan settings.
 *
 * @param ins - Bluetooth client instance, see @ref bt_instance_t.
 * @param settings - Pointer to scan settings, see @ref ble_scan_settings_t.
 * @param cbs - Pointer to scanner callbacks, see @ref scanner_callbacks_t.
 * @return bt_scanner_t* - Scanner handle generated by the scan manager.
 *
 * **Example:**
 * @code
ble_scan_settings_t settings = {
    .scan_mode = BT_SCAN_MODE_LOW_LATENCY,
    .scan_type = BT_LE_SCAN_TYPE_ACTIVE,
    // Additional settings...
};
bt_scanner_t* scanner = bt_le_start_scan_settings(ins, &settings, &my_scanner_callbacks);
if (scanner == NULL) {
    // Handle error
}
 * @endcode
 */
bt_scanner_t* BTSYMBOLS(bt_le_start_scan_settings)(bt_instance_t* ins,
    ble_scan_settings_t* settings,
    const scanner_callbacks_t* cbs);

/**
 * @brief Start BLE scan with filters.
 *
 * Initiates a BLE scan with specific settings and filters.
 *
 * @param ins - Bluetooth client instance, see @ref bt_instance_t.
 * @param settings - Pointer to scan settings, see @ref ble_scan_settings_t.
 * @param filter_data - Pointer to filter data, see @ref ble_scan_filter_t.
 * @param cbs - Pointer to scanner callbacks, see @ref scanner_callbacks_t.
 * @return bt_scanner_t* - Scanner handle generated by the scan manager.
 *
 * **Example:**
 * @code
ble_scan_filter_t filter = {
    .uuids = {0x180D, 0x180F}, // Heart Rate and Battery Service UUIDs
    .active = 1,
    // Additional filter settings...
};
bt_scanner_t* scanner = bt_le_start_scan_with_filters(ins, &settings, &filter, &my_scanner_callbacks);
if (scanner == NULL) {
    // Handle error
}
 * @endcode
 */
bt_scanner_t* BTSYMBOLS(bt_le_start_scan_with_filters)(bt_instance_t* ins,
    ble_scan_settings_t* settings,
    ble_scan_filter_t* filter_data,
    const scanner_callbacks_t* cbs);

/**
 * @brief Stop BLE scan.
 *
 * Stops an ongoing BLE scan.
 *
 * @param ins - Bluetooth client instance, see @ref bt_instance_t.
 * @param scanner - Scanner handle generated by the scan manager.
 *
 * **Example:**
 * @code
bt_le_stop_scan(ins, scanner);
 * @endcode
 */
void BTSYMBOLS(bt_le_stop_scan)(bt_instance_t* ins, bt_scanner_t* scanner);

/**
 * @brief Check if BLE scanning is supported.
 *
 * Determines whether BLE scanning is supported by the adapter.
 *
 * @param ins - Bluetooth client instance, see @ref bt_instance_t.
 * @return true - Scanning is supported.
 * @return false - Scanning is not supported.
 *
 * **Example:**
 * @code
if (bt_le_scan_is_supported(ins)) {
    // Scanning is supported
} else {
    // Scanning is not supported
}
 * @endcode
 */
bool BTSYMBOLS(bt_le_scan_is_supported)(bt_instance_t* ins);

#ifdef __cplusplus
}
#endif

#endif /* __BT_LE_SCAN_H_ */