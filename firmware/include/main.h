/*****************************************************************************
 *
 * Copyright (c) 2015-2016 jnsbyr
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *****************************************************************************
 *
 * project: WLAN control unit for Gardena solenoid irrigation valve no. 1251
 *
 * file:    main.h
 *
 * created: 11.04.2015
 *
 *****************************************************************************/

#ifndef __USER_MAIN_H__
#define __USER_MAIN_H__

#include <c_types.h>
#include <ip_addr.h>

#define SLEEPER_BOOTTIME            87 // [ms] bootloader runtime after reset
#define SLEEPER_COMMANDTIME        600 // [ms] 0.6 s, typical time runtime (boot, AP connect and TCP handshake)
#define SLEEPER_MIN_DOWNTIME      1000 // [ms] 1.0 s, minimum time between shutdown and restart

#define DEFAULT_DEEP_SLEEP_SCALE 10375 // extend deep sleep duration by 3.75% to compensate for early wakeup by RTC

#define MIN_BATTERY_VOLTAGE       3270 // [mV] minimum supply voltage before shutting down operation (nominal regulated voltage is 3320 mV)

#define MAX_WLAN_TIME             8000 // [ms] timeout
#define MAX_UPLINK_TIME           2000 // [ms] timeout
#define WLAN_TIMER_PERIOD          500 // [ms] interval
#define UPLINK_TIMER_PERIOD        200 // [ms] interval

#define MAX_ACTIVITIES 32


enum SleeperMode {MODE_OFF         = 0,
                  MODE_MANUAL      = 1,
                  MODE_AUTO        = 2};

enum ValveState {VALVE_CLOSED      = 0,
                 VALVE_OPEN        = 1,
                 VALVE_LOW_BAT     = 2};

enum ActivityDay {DAY_INVALID = 0,
                  DAY_EVERY   = 1,
                  DAY_SECOND  = 2,
                  DAY_THIRD   = 3,
                  DAY_SUNDAY  = 4};


typedef struct          // 5 Byte
{
  uint8 day;            // 0 = invalid, 1 = every, 2 = Sunday, 3 = Monday, ...
  uint16 startTime;     // minutes since midnight
  uint16 duration;      // seconds
} ActivityT;

typedef struct                          // 62 + N*5 Byte
{
  uint16 magic;                         // static
  uint8  mode;                          // config
  uint8  valveOpen;                     // state, bool, valve state
  uint8  offMode;                       // state, mode to set after valve is switched off
  uint8  lowBattery;                    // state, bool, vdd33 voltage is below hard coded limit
  uint8  override;                      // state, bool, manual override of current activity
  uint8  overriddenMode;                // state, mode to set when override ends
  uint8  valveCloseTimeEstimated;       // state, bool, valve close time is only estimated
  uint8  overrideEndTimeEstimated;      // state, bool, override end time is only estimated
  uint16 totalOpenCount;                // state, total number valve was opened since init
  uint16 boottime;                      // config, milliseconds
  uint16 defaultDuration;               // config, seconds, default duration to open vale (manual mode, override)
  uint16 downtimeScale;                 // config, 10000 = 1.0
  sint16 batteryOffset;                 // config, millivolt
  uint32 activityProgramId;             // config
  uint32 downtime;                      // config, milliseconds
  uint32 lastDowntime;                  // state, milliseconds, last sleep duration
  uint32 totalOpenDuration;             // state, seconds, total duration the valve was open since init
  uint64 valveOpenTime;                 // state, milliseconds, time when valve was opened
  uint64 valveCloseTime;                // state, milliseconds, time when valve must be closed
  uint64 lastShutdownTime;              // state, milliseconds, time when last os shutdown was initiated
  uint64 overrideEndTime;               // state, milliseconds, time when override is reset
  struct ip_info ipConfig;              // state
  ActivityT activities[MAX_ACTIVITIES]; // config
} PersistentStateT;

typedef struct
{
  PersistentStateT rtcMem; // persistent values
  sint16 batteryVoltage;   // supply voltage at time of boot
  sint8  rssi;             // RSSI at time of connect
  uint8  timeSynchronized; // bool, current time is synchronized with server
} SleeperStateT;


uint64 getTime();
void comProcessing();

#endif /* __USER_MAIN_H__ */
