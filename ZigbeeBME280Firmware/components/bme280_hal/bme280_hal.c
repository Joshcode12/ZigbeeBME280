#include "bme280_hal.h"
#include "driver/i2c_master.h"
#include "rom/ets_sys.h"

#define BME280_SHUTTLE_ID UINT8_C(0x33)

static uint8_t dev_addr;

BME280_INTF_RET_TYPE bme280_i2c_read(uint8_t reg_addr, uint8_t *reg_data,
                                     uint32_t length, void *intf_ptr) {
  i2c_master_dev_handle_t dev_handle = (i2c_master_dev_handle_t)intf_ptr;

  return i2c_master_transmit_receive(dev_handle, &reg_addr, 1, reg_data, len,
                                     -1);
}

BME280_INTF_RET_TYPE bme280_i2c_write(uint8_t reg_addr, const uint8_t *reg_data,
                                      uint32_t length, void *intf_ptr) {
  dev_addr = *(uint8_t *)intf_ptr;

  return coines_write_i2c(COINES_I2C_BUS_0, dev_addr, reg_addr,
                          (uint8_t *)reg_data, (uint16_t)length);
}

void bme280_delay_us(uint32_t period, void *intf_ptr) { ets_delay_us(period); }

void bme280_error_codes_print_result(const char api_name[], int8_t rslt) {
  if (rslt != BME280_OK) {
    printf("%s\t", api_name);

    switch (rslt) {
    case BME280_E_NULL_PTR:
      printf("Error [%d] : Null pointer error.", rslt);
      printf("It occurs when the user tries to assign value (not address) to a "
             "pointer, which has been initialized to NULL.\r\n");
      break;

    case BME280_E_COMM_FAIL:
      printf("Error [%d] : Communication failure error.", rslt);
      printf("It occurs due to read/write operation failure and also due to "
             "power failure during communication\r\n");
      break;

    case BME280_E_DEV_NOT_FOUND:
      printf("Error [%d] : Device not found error. It occurs when the device "
             "chip id is incorrectly read\r\n",
             rslt);
      break;

    case BME280_E_INVALID_LEN:
      printf("Error [%d] : Invalid length error. It occurs when write is done "
             "with invalid length\r\n",
             rslt);
      break;

    default:
      printf("Error [%d] : Unknown error code\r\n", rslt);
      break;
    }
  }
}
