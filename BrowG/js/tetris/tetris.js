class Tetris {
    constructor(canvas, ctx) {
        this.canvas = canvas;
        this.ctx = ctx;
        
        // Сначала инициализируем pieces, потом вызываем reset
        this.pieces = [
            { shape: [[1,1,1,1]], color: '#00FFFF' }, // I
            { shape: [[1,1],[1,1]], color: '#FFFF00' }, // O
            { shape: [[0,1,0],[1,1,1]], color: '#800080' }, // T
            { shape: [[1,0,0],[1,1,1]], color: '#FFA500' }, // L
            { shape: [[0,0,1],[1,1,1]], color: '#0000FF' }, // J
            { shape: [[0,1,1],[1,1,0]], color: '#00FF00' }, // S
            { shape: [[1,1,0],[0,1,1]], color: '#FF0000' }  // Z
        ];
        
        this.reset();
    }

    reset() {
        this.board = Array(20).fill().map(() => Array(10).fill(0));
        this.currentPiece = this.getRandomPiece();
        this.nextPiece = this.getRandomPiece();
        this.piecePosition = { x: 3, y: 0 };
        this.score = 0;
        this.lines = 0;
        this.level = 1;
        this.gameOver = false;
        this.isRunning = false;
        this.isPaused = false;
    }

    getRandomPiece() {
        // Безопасное копирование фигуры
        const piece = this.pieces[Math.floor(Math.random() * this.pieces.length)];
        return {
            shape: JSON.parse(JSON.stringify(piece.shape)),
            color: piece.color
        };
    }

    moveLeft() {
        if (this.isPaused || this.gameOver) return;
        
        this.piecePosition.x--;
        if (this.checkCollision()) {
            this.piecePosition.x++;
        }
    }

    moveRight() {
        if (this.isPaused || this.gameOver) return;
        
        this.piecePosition.x++;
        if (this.checkCollision()) {
            this.piecePosition.x--;
        }
    }

    moveDown() {
        if (this.isPaused || this.gameOver) return;
        
        this.piecePosition.y++;
        if (this.checkCollision()) {
            this.piecePosition.y--;
            this.lockPiece();
            this.clearLines();
            this.spawnPiece();
        }
        return !this.checkCollision();
    }

    rotate() {
        if (this.isPaused || this.gameOver) return;
        
        const originalShape = this.currentPiece.shape;
        const rows = originalShape.length;
        const cols = originalShape[0].length;
        
        // Создаем новую матрицу для поворота
        const newShape = Array(cols).fill().map(() => Array(rows).fill(0));
        for (let y = 0; y < rows; y++) {
            for (let x = 0; x < cols; x++) {
                newShape[x][rows - 1 - y] = originalShape[y][x];
            }
        }
        
        // Сохраняем оригинальную фигуру на случай отката
        const originalPiece = { ...this.currentPiece };
        this.currentPiece.shape = newShape;
        
        // Если после поворота есть коллизия, возвращаем обратно
        if (this.checkCollision()) {
            this.currentPiece = originalPiece;
        }
    }

    hardDrop() {
        if (this.isPaused || this.gameOver) return;
        
        while (this.moveDown()) {
            // Продолжаем двигать вниз пока возможно
        }
    }

    checkCollision() {
        const shape = this.currentPiece.shape;
        for (let y = 0; y < shape.length; y++) {
            for (let x = 0; x < shape[y].length; x++) {
                if (shape[y][x]) {
                    const boardX = this.piecePosition.x + x;
                    const boardY = this.piecePosition.y + y;
                    
                    // Проверка границ
                    if (boardX < 0 || boardX >= 10 || boardY >= 20) {
                        return true;
                    }
                    
                    // Проверка столкновения с другими фигурами
                    if (boardY >= 0 && this.board[boardY][boardX]) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    lockPiece() {
        const shape = this.currentPiece.shape;
        for (let y = 0; y < shape.length; y++) {
            for (let x = 0; x < shape[y].length; x++) {
                if (shape[y][x]) {
                    const boardY = this.piecePosition.y + y;
                    const boardX = this.piecePosition.x + x;
                    
                    if (boardY >= 0) {
                        this.board[boardY][boardX] = this.currentPiece.color;
                    }
                }
            }
        }
    }

    clearLines() {
        let linesCleared = 0;
        
        for (let y = 19; y >= 0; y--) {
            if (this.board[y].every(cell => cell !== 0)) {
                // Удаляем заполненную линию
                this.board.splice(y, 1);
                // Добавляем новую пустую линию сверху
                this.board.unshift(Array(10).fill(0));
                linesCleared++;
                y++; // Проверяем ту же строку снова (так как массив сдвинулся)
            }
        }
        
        if (linesCleared > 0) {
            this.lines += linesCleared;
            this.score += this.calculateScore(linesCleared);
            this.level = Math.floor(this.lines / 10) + 1;
        }
    }

    calculateScore(linesCleared) {
        const baseScores = [0, 40, 100, 300, 1200];
        return baseScores[linesCleared] * this.level;
    }

    spawnPiece() {
        this.currentPiece = this.nextPiece;
        this.nextPiece = this.getRandomPiece();
        this.piecePosition = { x: 3, y: 0 };
        
        // Проверяем game over
        if (this.checkCollision()) {
            this.gameOver = true;
        }
    }

    draw() {
        // Очистка canvas
        this.ctx.fillStyle = '#000';
        this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);
        
        // Отрисовка сетки
        this.drawGrid();
        
        // Отрисовка заблокированных фигур
        this.drawBoard();
        
        // Отрисовка текущей фигуры
        this.drawPiece(this.currentPiece, this.piecePosition.x, this.piecePosition.y);
        
        // Отрисовка призрачной фигуры (предпросмотр)
        this.drawGhostPiece();
    }

    drawGrid() {
        const blockSize = this.canvas.height / 20;
        
        this.ctx.strokeStyle = '#333';
        this.ctx.lineWidth = 0.5;
        
        // Вертикальные линии
        for (let x = 0; x <= 10; x++) {
            this.ctx.beginPath();
            this.ctx.moveTo(x * blockSize, 0);
            this.ctx.lineTo(x * blockSize, this.canvas.height);
            this.ctx.stroke();
        }
        
        // Горизонтальные линии
        for (let y = 0; y <= 20; y++) {
            this.ctx.beginPath();
            this.ctx.moveTo(0, y * blockSize);
            this.ctx.lineTo(10 * blockSize, y * blockSize);
            this.ctx.stroke();
        }
    }

    drawBoard() {
        const blockSize = this.canvas.height / 20;
        
        for (let y = 0; y < 20; y++) {
            for (let x = 0; x < 10; x++) {
                if (this.board[y][x]) {
                    this.ctx.fillStyle = this.board[y][x];
                    this.ctx.fillRect(x * blockSize, y * blockSize, blockSize, blockSize);
                    
                    // Обводка для блоков
                    this.ctx.strokeStyle = 'rgba(255,255,255,0.3)';
                    this.ctx.lineWidth = 1;
                    this.ctx.strokeRect(x * blockSize, y * blockSize, blockSize, blockSize);
                }
            }
        }
    }

    drawPiece(piece, offsetX, offsetY) {
        const blockSize = this.canvas.height / 20;
        const shape = piece.shape;
        
        for (let y = 0; y < shape.length; y++) {
            for (let x = 0; x < shape[y].length; x++) {
                if (shape[y][x]) {
                    this.ctx.fillStyle = piece.color;
                    this.ctx.fillRect(
                        (offsetX + x) * blockSize,
                        (offsetY + y) * blockSize,
                        blockSize,
                        blockSize
                    );
                    
                    // Обводка
                    this.ctx.strokeStyle = 'rgba(255,255,255,0.3)';
                    this.ctx.lineWidth = 1;
                    this.ctx.strokeRect(
                        (offsetX + x) * blockSize,
                        (offsetY + y) * blockSize,
                        blockSize,
                        blockSize
                    );
                }
            }
        }
    }

    drawGhostPiece() {
        if (this.isPaused || this.gameOver) return;
        
        // Находим позицию призрачной фигуры
        const ghostPosition = { ...this.piecePosition };
        while (!this.checkCollisionAt(ghostPosition.x, ghostPosition.y + 1)) {
            ghostPosition.y++;
        }
        
        const blockSize = this.canvas.height / 20;
        const shape = this.currentPiece.shape;
        
        // Рисуем полупрозрачную фигуру
        this.ctx.globalAlpha = 0.3;
        for (let y = 0; y < shape.length; y++) {
            for (let x = 0; x < shape[y].length; x++) {
                if (shape[y][x]) {
                    this.ctx.fillStyle = this.currentPiece.color;
                    this.ctx.fillRect(
                        (ghostPosition.x + x) * blockSize,
                        (ghostPosition.y + y) * blockSize,
                        blockSize,
                        blockSize
                    );
                }
            }
        }
        this.ctx.globalAlpha = 1.0;
    }

    checkCollisionAt(x, y) {
        const shape = this.currentPiece.shape;
        for (let row = 0; row < shape.length; row++) {
            for (let col = 0; col < shape[row].length; col++) {
                if (shape[row][col]) {
                    const boardX = x + col;
                    const boardY = y + row;
                    
                    if (boardX < 0 || boardX >= 10 || boardY >= 20) {
                        return true;
                    }
                    
                    if (boardY >= 0 && this.board[boardY][boardX]) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    drawNextPiece(canvas, ctx) {
        const piece = this.nextPiece;
        const blockSize = 20;
        
        // Очистка canvas
        ctx.fillStyle = '#000';
        ctx.fillRect(0, 0, canvas.width, canvas.height);
        
        // Центрируем фигуру
        const offsetX = (canvas.width / blockSize - piece.shape[0].length) / 2;
        const offsetY = (canvas.height / blockSize - piece.shape.length) / 2;
        
        // Рисуем фигуру
        for (let y = 0; y < piece.shape.length; y++) {
            for (let x = 0; x < piece.shape[y].length; x++) {
                if (piece.shape[y][x]) {
                    ctx.fillStyle = piece.color;
                    ctx.fillRect(
                        (offsetX + x) * blockSize,
                        (offsetY + y) * blockSize,
                        blockSize,
                        blockSize
                    );
                    
                    // Обводка
                    ctx.strokeStyle = 'rgba(255,255,255,0.3)';
                    ctx.lineWidth = 1;
                    ctx.strokeRect(
                        (offsetX + x) * blockSize,
                        (offsetY + y) * blockSize,
                        blockSize,
                        blockSize
                    );
                }
            }
        }
    }

    start() {
        this.isRunning = true;
        this.isPaused = false;
    }

    pause() {
        this.isPaused = !this.isPaused;
    }

    getScore() {
        return this.score;
    }

    getLines() {
        return this.lines;
    }

    getLevel() {
        return this.level;
    }

    isGameOver() {
        return this.gameOver;
    }
}