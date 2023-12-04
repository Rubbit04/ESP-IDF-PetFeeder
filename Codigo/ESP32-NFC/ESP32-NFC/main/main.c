#include <esp_log.h>
#include <inttypes.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <ultrasonic.h>
#include "rc522.h"
#include "esp_log.h"

static const char* TAG = "rc522-demo";
static rc522_handle_t scanner;
uint64_t nfc1 = 956488379431;
uint64_t nfc2 = 896033555592;
int detectado = 0;


#define MOTOR GPIO_NUM_15
#define STACK_SIZE 1024 * 12

#define MAX_DISTANCE_CM 500 // 5m max

esp_err_t create_tasks(void);
void vTaskNFC(void *parametroN);
void vTaskMotor(void *parametroM);
void vTaskUltra(void *parametroU);

static void rc522_handler(void* arg, esp_event_base_t base, int32_t event_id, void* event_data)
{
    rc522_event_data_t* data = (rc522_event_data_t*) event_data;
    
    switch(event_id) {
        case RC522_EVENT_TAG_SCANNED: {
                rc522_tag_t* tag = (rc522_tag_t*) data->ptr;
                ESP_LOGI(TAG, "Tag scanned (sn: %" PRIu64 ")", tag->serial_number);
               
                if (nfc1==tag->serial_number ) {
                    printf("Funciono\n");
                    detectado = 1;

                }else if(nfc2==tag->serial_number){
                    printf("Funcionox2\n");
                    detectado = 1;

                }else printf("Daniel putero\n");
                        }
        break;
}
}
static void inic_NFC()
{
    rc522_config_t config = {
        .spi.host = VSPI_HOST,
        .spi.miso_gpio = 25,
        .spi.mosi_gpio = 23,
        .spi.sck_gpio = 19,
        .spi.sda_gpio = 22,
    };
    rc522_create(&config, &scanner);
}

static void inic_MOTOR()
{
    gpio_reset_pin(MOTOR);
    gpio_set_direction(MOTOR,GPIO_MODE_OUTPUT);
    gpio_set_level(MOTOR, 0);
}


void app_main()
{
    inic_NFC();
    inic_MOTOR();
    create_tasks();

       
}

esp_err_t create_tasks(void)
{
    static uint8_t parametroNFC;
    static uint8_t parametroMotor;
    static uint8_t parametroUltrasonido;
    TaskHandle_t xHandlerN = NULL;
    TaskHandle_t xHandlerM = NULL;
    TaskHandle_t xHandlerU = NULL;

    xTaskCreate(vTaskNFC,"vTaskNFC",STACK_SIZE, &parametroNFC,3,&xHandlerN);
    xTaskCreate(vTaskMotor, "vTaskMotor", STACK_SIZE, &parametroMotor, 2, &xHandlerM);
    xTaskCreate(vTaskUltra, "ultrasonic_test", STACK_SIZE, &parametroUltrasonido, 1, &xHandlerU);
    return ESP_OK;

}

void vTaskNFC(void *parametroN)
{
    while (1)
    {
        
        rc522_register_events(scanner, RC522_EVENT_ANY, rc522_handler, NULL);
        rc522_start(scanner);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
    
}

void vTaskMotor(void *parametroM)
{
    while (1)
    {
        
        if(detectado==1)
        {
            printf("Activar motor");
            gpio_set_level(MOTOR, detectado);
            
            detectado = 0;
        }else
        {
            gpio_set_level(MOTOR,detectado);
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void vTaskUltra(void *parametroU)
{
    ultrasonic_sensor_t sensor = {
        .trigger_pin = 5,
        .echo_pin = 18
    };
    ultrasonic_init(&sensor);
    while (1)
    {
        float distance;
        float ultrasonido;

        esp_err_t res = ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);
        if (res != ESP_OK)
        {
            printf("Error %d: ", res);
            switch (res)
            {
                case ESP_ERR_ULTRASONIC_PING:
                    printf("Cannot ping (device is in invalid state)\n");
                    break;
                case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                    printf("Ping timeout (no device found)\n");
                    break;
                case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                    printf("Echo timeout (i.e. distance too big)\n");
                    break;
                default:
                    printf("%s\n", esp_err_to_name(res));
            }
        }
        else{
           //printf("Distance: %0.04f cm\n", distance*100);
           ultrasonido = distance*100;
           printf("Ultrasonido: %0.04f cm\n", ultrasonido);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
}