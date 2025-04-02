// 게임 상수 정의
const BOARD_WIDTH = 10;
const BOARD_HEIGHT = 20;
const BLOCK_SIZE = 30;
const SHAPES = [
    // I 모양
    [[0,0,0,0],
     [1,1,1,1],
     [0,0,0,0],
     [0,0,0,0]],
    // O 모양
    [[0,0,0,0],
     [0,1,1,0],
     [0,1,1,0],
     [0,0,0,0]],
    // T 모양
    [[0,0,0,0],
     [0,1,0,0],
     [1,1,1,0],
     [0,0,0,0]],
    // S 모양
    [[0,0,0,0],
     [0,1,1,0],
     [1,1,0,0],
     [0,0,0,0]],
    // Z 모양
    [[0,0,0,0],
     [1,1,0,0],
     [0,1,1,0],
     [0,0,0,0]],
    // J 모양
    [[0,0,0,0],
     [1,0,0,0],
     [1,1,1,0],
     [0,0,0,0]],
    // L 모양
    [[0,0,0,0],
     [0,0,1,0],
     [1,1,1,0],
     [0,0,0,0]]
];

// 게임 상태
let gameState = {
    board: Array(BOARD_HEIGHT).fill().map(() => Array(BOARD_WIDTH).fill(0)),
    currentPiece: null,
    currentX: 0,
    currentY: 0,
    score: 0,
    gameOver: false
};

// 캔버스 설정
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');
const scoreElement = document.getElementById('score');

// 새로운 블록 생성
function createNewPiece() {
    const shape = SHAPES[Math.floor(Math.random() * SHAPES.length)];
    gameState.currentPiece = shape;
    gameState.currentX = Math.floor(BOARD_WIDTH / 2) - Math.floor(shape[0].length / 2);
    gameState.currentY = 0;
    
    if (checkCollision()) {
        gameState.gameOver = true;
    }
}

// 충돌 검사
function checkCollision(offsetX = 0, offsetY = 0) {
    for (let y = 0; y < gameState.currentPiece.length; y++) {
        for (let x = 0; x < gameState.currentPiece[y].length; x++) {
            if (gameState.currentPiece[y][x]) {
                const newX = gameState.currentX + x + offsetX;
                const newY = gameState.currentY + y + offsetY;
                
                if (newX < 0 || newX >= BOARD_WIDTH || 
                    newY >= BOARD_HEIGHT ||
                    (newY >= 0 && gameState.board[newY][newX])) {
                    return true;
                }
            }
        }
    }
    return false;
}

// 블록 회전
function rotatePiece() {
    const newPiece = gameState.currentPiece[0].map((_, i) =>
        gameState.currentPiece.map(row => row[i]).reverse()
    );
    
    const oldPiece = gameState.currentPiece;
    gameState.currentPiece = newPiece;
    
    if (checkCollision()) {
        gameState.currentPiece = oldPiece;
    }
}

// 블록 이동
function movePiece(dx, dy) {
    if (!checkCollision(dx, dy)) {
        gameState.currentX += dx;
        gameState.currentY += dy;
        return true;
    }
    return false;
}

// 완성된 줄 제거
function clearLines() {
    let linesCleared = 0;
    
    for (let y = BOARD_HEIGHT - 1; y >= 0; y--) {
        if (gameState.board[y].every(cell => cell === 1)) {
            gameState.board.splice(y, 1);
            gameState.board.unshift(Array(BOARD_WIDTH).fill(0));
            linesCleared++;
            y++;
        }
    }
    
    if (linesCleared > 0) {
        gameState.score += linesCleared * 100;
        scoreElement.textContent = gameState.score;
    }
}

// 게임 화면 그리기
function draw() {
    // 캔버스 초기화
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // 보드 그리기
    for (let y = 0; y < BOARD_HEIGHT; y++) {
        for (let x = 0; x < BOARD_WIDTH; x++) {
            if (gameState.board[y][x]) {
                ctx.fillStyle = '#333';
                ctx.fillRect(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1);
            }
        }
    }
    
    // 현재 블록 그리기
    if (gameState.currentPiece) {
        ctx.fillStyle = '#F00';
        for (let y = 0; y < gameState.currentPiece.length; y++) {
            for (let x = 0; x < gameState.currentPiece[y].length; x++) {
                if (gameState.currentPiece[y][x]) {
                    ctx.fillRect(
                        (gameState.currentX + x) * BLOCK_SIZE,
                        (gameState.currentY + y) * BLOCK_SIZE,
                        BLOCK_SIZE - 1,
                        BLOCK_SIZE - 1
                    );
                }
            }
        }
    }
}

// 게임 업데이트
function update() {
    if (!gameState.gameOver) {
        if (!movePiece(0, 1)) {
            // 블록을 보드에 고정
            for (let y = 0; y < gameState.currentPiece.length; y++) {
                for (let x = 0; x < gameState.currentPiece[y].length; x++) {
                    if (gameState.currentPiece[y][x]) {
                        gameState.board[gameState.currentY + y][gameState.currentX + x] = 1;
                    }
                }
            }
            
            clearLines();
            createNewPiece();
        }
    }
}

// 키보드 입력 처리
document.addEventListener('keydown', (event) => {
    if (gameState.gameOver) return;
    
    switch (event.keyCode) {
        case 37: // 왼쪽
            movePiece(-1, 0);
            break;
        case 39: // 오른쪽
            movePiece(1, 0);
            break;
        case 40: // 아래
            movePiece(0, 1);
            break;
        case 38: // 위 (회전)
            rotatePiece();
            break;
    }
    draw();
});

// 게임 초기화
function initGame() {
    gameState.board = Array(BOARD_HEIGHT).fill().map(() => Array(BOARD_WIDTH).fill(0));
    gameState.score = 0;
    gameState.gameOver = false;
    scoreElement.textContent = '0';
    createNewPiece();
}

// 게임 루프
function gameLoop() {
    update();
    draw();
    if (!gameState.gameOver) {
        setTimeout(gameLoop, 1000);
    } else {
        alert('게임 오버! 점수: ' + gameState.score);
        initGame();
        gameLoop();
    }
}

// 게임 시작
initGame();
gameLoop(); 