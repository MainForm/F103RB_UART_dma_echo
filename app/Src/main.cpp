#include "main.h"
#include "gpio.h"
#include "usart.h"

// Defined in Core/Src/main.c
// extern "C" disables C++ name mangling so the C++ linker can find the C function.
extern "C" 
void SystemClock_Config(void);

int main(void){

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    while(1){
        HAL_UART_Transmit(&huart2, (uint8_t*)"Hello World!\r\n", 14, HAL_MAX_DELAY);
        HAL_Delay(1000);
    }
}