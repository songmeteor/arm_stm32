#ifndef __TETRIS_H
#define __TETRIS_H

#include "stm32f1xx_hal.h"

// 게임 보드 크기 정의
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

// 테트리스 블록 모양 정의
#define SHAPES_COUNT 7
#define SHAPE_SIZE 4

// 게임 상태
typedef enum {
    GAME_RUNNING,
    GAME_PAUSED,
    GAME_OVER
} GameState;

// 테트리스 블록 구조체
typedef struct {
    uint8_t shape[SHAPE_SIZE][SHAPE_SIZE];
    uint8_t width;
    uint8_t height;
} Tetromino;

// 게임 구조체
typedef struct {
    uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];
    Tetromino current_piece;
    uint8_t current_x;
    uint8_t current_y;
    uint32_t score;
    GameState state;
} TetrisGame;

// 함수 선언
void Tetris_Init(TetrisGame* game);
void Tetris_Update(TetrisGame* game);
void Tetris_MoveLeft(TetrisGame* game);
void Tetris_MoveRight(TetrisGame* game);
void Tetris_Rotate(TetrisGame* game);
void Tetris_Drop(TetrisGame* game);
void Tetris_Draw(TetrisGame* game);

#endif /* __TETRIS_H */ 