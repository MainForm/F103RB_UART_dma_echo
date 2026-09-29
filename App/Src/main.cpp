#include "main.h"
#include "dma.h"
#include "gpio.h"
#include "usart.h"

#include <queue>
#include <string>

#define RX_BUFFER_SIZE  10

using namespace std;

// Defined in Core/Src/main.c
// extern "C" disables C++ name mangling so the C++ linker can find the C function.
extern "C" 
void SystemClock_Config(void);

// UART 수신 버퍼
string rxBuffer(RX_BUFFER_SIZE,'\0');
// 데이터의 첫번째 인덱스
int curIdx = 0;

queue<string> rxQueue;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if(huart->Instance == USART2){
        // rxBuffer에서 curIdx부터 Size만큼 데이터를 rxQueue에 넣음
        rxQueue.push(rxBuffer.substr(curIdx, Size - curIdx));

        // curIdx를 다음 시작 위치로 이동
        curIdx = Size % rxBuffer.size();
    }
}

int main(void){

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USART2_UART_Init();

    // Circular 모드로 동작하기 떄문에 한번만 호출하면 됨
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*)rxBuffer.c_str(), rxBuffer.size());
    // Half Transfer Interrupt 비활성화(필요 없는 인터럽트)
    __HAL_DMA_DISABLE_IT(huart2.hdmarx, DMA_IT_HT);

    while (1){
        // rxQueue에 데이터가 있으면 출력
        if(rxQueue.empty() == false){
            // 큐에서 맨 앞의 문자열을 꺼내서 출력
            string msg = rxQueue.front();
            rxQueue.pop();
            
            // 문자열 출력
            HAL_UART_Transmit(&huart2, (uint8_t*)msg.c_str(), msg.size(), HAL_MAX_DELAY);
        }
    }
}