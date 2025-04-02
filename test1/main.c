#include "main.h"
#include "tetris.h"

// 전역 변수
TetrisGame game;
uint32_t last_update = 0;
uint32_t game_speed = 500; // 블록이 떨어지는 속도 (ms)

// 버튼 입력 처리
void ProcessInput(void) {
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) { // 왼쪽 버튼
        Tetris_MoveLeft(&game);
    }
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET) { // 오른쪽 버튼
        Tetris_MoveRight(&game);
    }
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET) { // 회전 버튼
        Tetris_Rotate(&game);
    }
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) { // 빠른 하강 버튼
        Tetris_Drop(&game);
    }
}

int main(void) {
    // HAL 초기화
    HAL_Init();
    
    // 시스템 클럭 설정
    SystemClock_Config();
    
    // GPIO 초기화
    GPIO_Init();
    
    // LCD 초기화
    // LCD_Init();
    
    // 게임 초기화
    Tetris_Init(&game);
    
    while(1) {
        // 입력 처리
        ProcessInput();
        
        // 게임 업데이트
        uint32_t current_time = HAL_GetTick();
        if(current_time - last_update >= game_speed) {
            Tetris_Update(&game);
            last_update = current_time;
        }
        
        // 화면 갱신
        Tetris_Draw(&game);
        
        // 게임 오버 체크
        if(game.state == GAME_OVER) {
            // 게임 오버 처리
            HAL_Delay(2000);
            Tetris_Init(&game);
        }
        
        HAL_Delay(10); // CPU 부하 감소
    }
} 