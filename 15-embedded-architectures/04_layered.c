/*
 * 架构四：分层架构 (HAL 接口 + 驱动 + 服务层 + 应用层 + 回调)
 * 对应文章：《嵌入式软件架构的"十八般武艺"》
 * 编译： gcc -Wall -Wextra -o layered 04_layered.c
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- 硬件抽象层 (HAL) 接口 ---- */
typedef struct {
    void (*init)(void);
    void (*read)(uint8_t *data, uint16_t length);
    void (*write)(const uint8_t *data, uint16_t length);
} HAL_Interface_t;

static void SPI_HW_Init(void) { puts("  [HAL] SPI 初始化"); }
static void SPI_HW_Read(uint8_t *d, uint16_t n)  { for (uint16_t i=0;i<n;i++) d[i]=0x40+i; }
static void SPI_HW_Write(const uint8_t *d, uint16_t n) { (void)d; printf("  [HAL] SPI 写 %u 字节\n", n); }

static const HAL_Interface_t SPI_HAL = { SPI_HW_Init, SPI_HW_Read, SPI_HW_Write };

/* ---- 传感器驱动层 ---- */
typedef struct {
    void (*init)(const HAL_Interface_t *hal);
    bool (*read_value)(float *value);
} Sensor_Driver_t;

static const HAL_Interface_t *s_hal;
static void TempSensor_Init(const HAL_Interface_t *hal) { s_hal = hal; s_hal->init(); }
static bool TempSensor_ReadValue(float *value)
{
    uint8_t raw[2];
    s_hal->read(raw, 2);
    *value = 20.0f + (float)raw[0] * 0.1f;   /* 假装换算 */
    return true;
}
static const Sensor_Driver_t TempSensor = { TempSensor_Init, TempSensor_ReadValue };

/* ---- 服务层 ---- */
static void SensorService_Init(void) { TempSensor.init(&SPI_HAL); }
static bool SensorService_GetTemperature(float *t) { return TempSensor.read_value(t); }

/* ---- 应用层 ---- */
int main(void)
{
    puts("=== 分层架构演示 ===");
    SensorService_Init();
    float temperature;
    if (SensorService_GetTemperature(&temperature))
        printf("  [App] 读到温度: %.1f ℃\n", temperature);
    return 0;
}
