#include "tetris.h"

// 테트리스 블록 모양 정의
static const uint8_t SHAPES[SHAPES_COUNT][SHAPE_SIZE][SHAPE_SIZE] = {
    // I 모양
    {
        {0,0,0,0},
        {1,1,1,1},
        {0,0,0,0},
        {0,0,0,0}
    },
    // O 모양
    {
        {0,0,0,0},
        {0,1,1,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    // T 모양
    {
        {0,0,0,0},
        {0,1,0,0},
        {1,1,1,0},
        {0,0,0,0}
    },
    // S 모양
    {
        {0,0,0,0},
        {0,1,1,0},
        {1,1,0,0},
        {0,0,0,0}
    },
    // Z 모양
    {
        {0,0,0,0},
        {1,1,0,0},
        {0,1,1,0},
        {0,0,0,0}
    },
    // J 모양
    {
        {0,0,0,0},
        {1,0,0,0},
        {1,1,1,0},
        {0,0,0,0}
    },
    // L 모양
    {
        {0,0,0,0},
        {0,0,1,0},
        {1,1,1,0},
        {0,0,0,0}
    }
};

// 게임 초기화
void Tetris_Init(TetrisGame* game) {
    // 보드 초기화
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for(int j = 0; j < BOARD_WIDTH; j++) {
            game->board[i][j] = 0;
        }
    }
    
    game->score = 0;
    game->state = GAME_RUNNING;
    
    // 첫 블록 생성
    uint8_t shape_idx = rand() % SHAPES_COUNT;
    for(int i = 0; i < SHAPE_SIZE; i++) {
        for(int j = 0; j < SHAPE_SIZE; j++) {
            game->current_piece.shape[i][j] = SHAPES[shape_idx][i][j];
        }
    }
    
    game->current_x = BOARD_WIDTH / 2 - 1;
    game->current_y = 0;
}

// 충돌 검사
static uint8_t CheckCollision(TetrisGame* game, int8_t offset_x, int8_t offset_y) {
    for(int i = 0; i < SHAPE_SIZE; i++) {
        for(int j = 0; j < SHAPE_SIZE; j++) {
            if(game->current_piece.shape[i][j]) {
                int8_t new_x = game->current_x + j + offset_x;
                int8_t new_y = game->current_y + i + offset_y;
                
                if(new_x < 0 || new_x >= BOARD_WIDTH || 
                   new_y >= BOARD_HEIGHT ||
                   (new_y >= 0 && game->board[new_y][new_x])) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// 블록 회전
void Tetris_Rotate(TetrisGame* game) {
    uint8_t temp[SHAPE_SIZE][SHAPE_SIZE];
    
    // 임시 배열에 회전된 모양 저장
    for(int i = 0; i < SHAPE_SIZE; i++) {
        for(int j = 0; j < SHAPE_SIZE; j++) {
            temp[i][j] = game->current_piece.shape[SHAPE_SIZE-1-j][i];
        }
    }
    
    // 원래 배열에 회전된 모양 복사
    for(int i = 0; i < SHAPE_SIZE; i++) {
        for(int j = 0; j < SHAPE_SIZE; j++) {
            game->current_piece.shape[i][j] = temp[i][j];
        }
    }
    
    // 충돌 검사
    if(CheckCollision(game, 0, 0)) {
        // 충돌이 발생하면 원래대로 되돌림
        for(int i = 0; i < SHAPE_SIZE; i++) {
            for(int j = 0; j < SHAPE_SIZE; j++) {
                game->current_piece.shape[i][j] = temp[SHAPE_SIZE-1-j][i];
            }
        }
    }
}

// 블록 이동
void Tetris_MoveLeft(TetrisGame* game) {
    if(!CheckCollision(game, -1, 0)) {
        game->current_x--;
    }
}

void Tetris_MoveRight(TetrisGame* game) {
    if(!CheckCollision(game, 1, 0)) {
        game->current_x++;
    }
}

void Tetris_Drop(TetrisGame* game) {
    if(!CheckCollision(game, 0, 1)) {
        game->current_y++;
    } else {
        // 블록을 보드에 고정
        for(int i = 0; i < SHAPE_SIZE; i++) {
            for(int j = 0; j < SHAPE_SIZE; j++) {
                if(game->current_piece.shape[i][j]) {
                    game->board[game->current_y + i][game->current_x + j] = 1;
                }
            }
        }
        
        // 새로운 블록 생성
        uint8_t shape_idx = rand() % SHAPES_COUNT;
        for(int i = 0; i < SHAPE_SIZE; i++) {
            for(int j = 0; j < SHAPE_SIZE; j++) {
                game->current_piece.shape[i][j] = SHAPES[shape_idx][i][j];
            }
        }
        
        game->current_x = BOARD_WIDTH / 2 - 1;
        game->current_y = 0;
        
        // 게임 오버 체크
        if(CheckCollision(game, 0, 0)) {
            game->state = GAME_OVER;
        }
    }
}

// 게임 업데이트
void Tetris_Update(TetrisGame* game) {
    if(game->state == GAME_RUNNING) {
        Tetris_Drop(game);
    }
}

// 게임 화면 그리기
void Tetris_Draw(TetrisGame* game) {
    // LCD 화면 초기화
    // LCD_Clear();
    
    // 보드 그리기
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for(int j = 0; j < BOARD_WIDTH; j++) {
            if(game->board[i][j]) {
                // LCD_DrawPixel(j, i);
            }
        }
    }
    
    // 현재 블록 그리기
    for(int i = 0; i < SHAPE_SIZE; i++) {
        for(int j = 0; j < SHAPE_SIZE; j++) {
            if(game->current_piece.shape[i][j]) {
                // LCD_DrawPixel(game->current_x + j, game->current_y + i);
            }
        }
    }
    
    // LCD_Update();
} 