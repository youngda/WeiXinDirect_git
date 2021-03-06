/**
  ******************************************************************************
  * @file    MICOAppDefine.h 
  * @author  William Xu
  * @version V1.0.0
  * @date    05-May-2014
  * @brief   This file create a TCP listener thread, accept every TCP client
  *          connection and create thread for them.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MXCHIP Inc. SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 MXCHIP Inc.</center></h2>
  ******************************************************************************
  */ 


#ifndef __HOMEKIT_H
#define __HOMEKIT_H

#include "Common.h"
#include "HomeKitPairList.h"
#include "json_c/json.h"

/*HomeKit definition*/
// ==== HKtatus ====
typedef int32_t         HkStatus;

#define kHKNoErr                           0   //! This specifies a success for the request.
#define kHKPrivilegeErr               -70401   //! Request denied due to insufficient privileges.
#define kHKCommunicateErr             -70402   //! Unable to communicate with requested service.
#define kHKBusyErr                    -70403   //! Resource is busy, try again.
#define kHKWriteToROErr               -70404   //! Cannot write to read only characteristic.
#define kHKReadFromWOErr              -70405   //! Cannot read from a write only characteristic.
#define kHKNotifyUnsupportErr         -70406   //! Notification is not supported for characteristic.
#define kHKResourceErr                -70407   //! Out of resource to process request.
#define kHKTimeOutErr                 -70408   //! Operation timed out.
#define kHKNotExistErr                -70409   //! Resource does not exist.
#define kHKInvalidErr                 -70410   //! Accessory received an invalid value in a write request.

// CATEGORY_IDENTIFIER value
typedef enum{
  CI_ORHER = 1,  
  CI_BRIDGE,
  CI_FAN,
  CI_GARAGE_DOOR_OPENER,    
  CI_LIGHTBULB,
  CI_DOOR_LOCK,
  CI_OUTLET,
  CI_SWITCH,
  CI_THERMOSTAT,
  CI_RESERVED,
} hk_ci_e;

typedef enum _valueType{
  ValueType_bool,
  ValueType_int,
  ValueType_float,
  ValueType_string,
  ValueType_date,
  ValueType_tlv8,
  ValueType_data,
  ValueType_array,
  ValueType_dict,
  ValueType_null,
} valueType;

typedef union {
    bool        boolValue;
    int         intValue;
    double      floatValue;
    char        *stringValue;
    char        *dateValue;
    json_object *array;
    json_object *object;
  } value_union;


struct _hapCharacteristic_t {
  char   *type;

  bool   hasStaticValue;
  valueType valueType;
  value_union value;

  bool   secureRead;
  bool   secureWrite;
  bool   hasEvents;

  bool   hasMinimumValue;
  union {
    int         intValue;
    float       floatValue;
  }      minimumValue;

  bool   hasMaximumValue;
  union {
    int         intValue;
    float       floatValue;
  }      maximumValue;

  bool   hasMinimumStep;
  union {
    int         intValue;
    float       floatValue;
  }      minimumStep;

  bool   hasMaxLength;
  int    maxLength;

  bool   hasMaxDataLength;
  int    maxDataLength;

  char   *description;

  char   *format;
  
  char   *unit;
};

struct _hapService_t {
  char    *type;
  uint32_t num_of_characteristics;
  struct  _hapCharacteristic_t *characteristic;
};

struct _hapAccessory_t {
  uint32_t num_of_services;
  struct _hapService_t  *services;
};

struct _hapProduct_t {
  uint32_t num_of_accessories;
  struct _hapAccessory_t  *accessories;
};

typedef struct _hapProduct_t hapProduct_t;

typedef OSStatus (*hk_key_storage_cb)(unsigned char *pk, unsigned char *sk, bool write);

/*Running status*/
typedef struct _hk_init_t {
  hk_ci_e             ci;
  char *              model;
  int                 config_number;
  mico_i2c_t          mfi_cp_port;
  hapProduct_t *      hap_product;
  uint8_t *           password;
  int                 password_len;
  uint8_t *           verifier;
  int                 verifier_len;
  uint8_t *           salt;
  int                 salt_len;
  hk_key_storage_cb   key_storage;
} hk_init_t;

void hk_server_lib_version( uint8_t *major, uint8_t *minor, uint8_t *revision );

OSStatus hk_server_start( hk_init_t init );

OSStatus hk_server_update_attri_db( hapProduct_t *hap_product, int config_number );

OSStatus hk_server_notify_by_bonjour( void );

/* HomeKit callback functions */
HkStatus HKReadCharacteristicValue(int accessoryID, int serviceID, int characteristicID, value_union *value );

void HKWriteCharacteristicValue(int accessoryID, int serviceID, int characteristicID, value_union value, bool moreComing );

HkStatus HKReadCharacteristicStatus(int accessoryID, int serviceID, int characteristicID );

HkStatus HKExcuteUnpairedIdentityRoutine( void );


#endif

