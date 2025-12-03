class Minesweeper {
    constructor(canvas, ctx) {
        this.canvas = canvas;
        this.ctx = ctx;
        
        this.difficulties = {
            beginner: { rows: 9, cols: 9, mines: 10 },
            intermediate: { rows: 16, cols: 16, mines: 40 },
            expert: { rows: 16, cols: 30, mines: 99 }
        };
        
        this.reset();
    }

    reset() {
        this.setDifficulty('beginner');
        this.gameOver = false;
        this.gameWon = false;
        this.firstClick = true;
        this.startTime = null;
        this.currentTime = 0;
        this.timer = null;
    }

    setDifficulty(level) {
        const config = this.difficulties[level];
        this.rows = config.rows;
        this.cols = config.cols;
        this.mines = config.mines;
        this.flags = 0;
        
        this.cellSize = Math.min(this.canvas.width / this.cols, this.canvas.height / this.rows);
        this.board = this.createBoard();
        this.gameOver = false;
        this.gameWon = false;
        this.firstClick = true;
        
        if (this.timer) {
            clearInterval(this.timer);
            this.timer = null;
        }
        this.currentTime = 0;
    }

    createBoard() {
        return Array(this.rows).fill().map(() => 
            Array(this.cols).fill().map(() => ({
                isMine: false,
                isRevealed: false,
                isFlagged: false,
                neighborMines: 0
            }))
        );
    }

    placeMines(firstClickRow, firstClickCol) {
        let minesPlaced = 0;
        
        while (minesPlaced < this.mines) {
            const row = Math.floor(Math.random() * this.rows);
            const col = Math.floor(Math.random() * this.cols);
            
            if (Math.abs(row - firstClickRow) <= 1 && Math.abs(col - firstClickCol) <= 1) {
                continue;
            }
            
            if (!this.board[row][col].isMine) {
                this.board[row][col].isMine = true;
                minesPlaced++;
            }
        }
        
        this.calculateNeighbors();
    }

    calculateNeighbors() {
        for (let row = 0; row < this.rows; row++) {
            for (let col = 0; col < this.cols; col++) {
                if (!this.board[row][col].isMine) {
                    let count = 0;
                    for (let dr = -1; dr <= 1; dr++) {
                        for (let dc = -1; dc <= 1; dc++) {
                            const newRow = row + dr;
                            const newCol = col + dc;
                            if (newRow >= 0 && newRow < this.rows && 
                                newCol >= 0 && newCol < this.cols && 
                                this.board[newRow][newCol].isMine) {
                                count++;
                            }
                        }
                    }
                    this.board[row][col].neighborMines = count;
                }
            }
        }
    }

    reveal(row, col) {
        if (this.gameOver || this.gameWon) return;
        
        if (this.firstClick) {
            this.firstClick = false;
            this.placeMines(row, col);
            this.startTime = Date.now();
            this.startTimer();
        }
        
        const cell = this.board[row][col];
        
        if (cell.isRevealed || cell.isFlagged) return;
        
        cell.isRevealed = true;
        
        if (cell.isMine) {
            this.gameOver = true;
            this.revealAllMines();
            if (this.timer) {
                clearInterval(this.timer);
            }
            return;
        }
        
        if (cell.neighborMines === 0) {
            for (let dr = -1; dr <= 1; dr++) {
                for (let dc = -1; dc <= 1; dc++) {
                    const newRow = row + dr;
                    const newCol = col + dc;
                    if (newRow >= 0 && newRow < this.rows && 
                        newCol >= 0 && newCol < this.cols) {
                        this.reveal(newRow, newCol);
                    }
                }
            }
        }
        
        this.checkWin();
    }

    toggleFlag(row, col) {
        if (this.gameOver || this.gameWon || this.firstClick) return;
        
        const cell = this.board[row][col];
        if (cell.isRevealed) return;
        
        cell.isFlagged = !cell.isFlagged;
        this.flags += cell.isFlagged ? 1 : -1;
        this.checkWin();
    }

    revealAllMines() {
        for (let row = 0; row < this.rows; row++) {
            for (let col = 0; col < this.cols; col++) {
                if (this.board[row][col].isMine) {
                    this.board[row][col].isRevealed = true;
                }
            }
        }
    }

    checkWin() {
        let unrevealedSafeCells = 0;
        
        for (let row = 0; row < this.rows; row++) {
            for (let col = 0; col < this.cols; col++) {
                const cell = this.board[row][col];
                if (!cell.isRevealed && !cell.isMine) {
                    unrevealedSafeCells++;
                }
            }
        }
        
        if (unrevealedSafeCells === 0) {
            this.gameWon = true;
            if (this.timer) {
                clearInterval(this.timer);
            }
        }
    }

    startTimer() {
        this.timer = setInterval(() => {
            this.currentTime = Math.floor((Date.now() - this.startTime) / 1000);
        }, 1000);
    }

    draw() {
        this.ctx.fillStyle = '#1a1a1a';
        this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);
        
        for (let row = 0; row < this.rows; row++) {
            for (let col = 0; col < this.cols; col++) {
                this.drawCell(row, col);
            }
        }
    }

    drawCell(row, col) {
        const cell = this.board[row][col];
        const x = col * this.cellSize;
        const y = row * this.cellSize;
        
        if (cell.isRevealed) {
            this.ctx.fillStyle = cell.isMine ? '#ff4444' : '#444';
            this.ctx.fillRect(x, y, this.cellSize, this.cellSize);
            
            if (cell.isMine) {
                this.ctx.fillStyle = '#000';
                this.ctx.beginPath();
                this.ctx.arc(x + this.cellSize/2, y + this.cellSize/2, this.cellSize/3, 0, Math.PI * 2);
                this.ctx.fill();
            } else if (cell.neighborMines > 0) {
                const colors = ['', 'blue', 'green', 'red', 'darkblue', 'brown', 'cyan', 'black', 'gray'];
                this.ctx.fillStyle = colors[cell.neighborMines];
                this.ctx.font = `${this.cellSize * 0.6}px Arial`;
                this.ctx.textAlign = 'center';
                this.ctx.textBaseline = 'middle';
                this.ctx.fillText(
                    cell.neighborMines.toString(),
                    x + this.cellSize/2,
                    y + this.cellSize/2
                );
            }
        } else {
            this.ctx.fillStyle = '#888';
            this.ctx.fillRect(x, y, this.cellSize, this.cellSize);
            
            this.ctx.strokeStyle = '#fff';
            this.ctx.lineWidth = 2;
            this.ctx.beginPath();
            this.ctx.moveTo(x, y);
            this.ctx.lineTo(x + this.cellSize, y);
            this.ctx.lineTo(x + this.cellSize, y + this.cellSize);
            this.ctx.stroke();
            
            this.ctx.strokeStyle = '#555';
            this.ctx.beginPath();
            this.ctx.moveTo(x, y);
            this.ctx.lineTo(x, y + this.cellSize);
            this.ctx.lineTo(x + this.cellSize, y + this.cellSize);
            this.ctx.stroke();
            
            if (cell.isFlagged) {
                this.ctx.fillStyle = 'red';
                this.ctx.beginPath();
                this.ctx.moveTo(x + this.cellSize/2, y + 5);
                this.ctx.lineTo(x + this.cellSize/2, y + this.cellSize - 5);
                this.ctx.stroke();
                
                this.ctx.beginPath();
                this.ctx.moveTo(x + this.cellSize/2, y + 5);
                this.ctx.lineTo(x + this.cellSize - 5, y + 15);
                this.ctx.lineTo(x + 5, y + 15);
                this.ctx.closePath();
                this.ctx.fill();
            }
        }
        
        this.ctx.strokeStyle = '#333';
        this.ctx.lineWidth = 1;
        this.ctx.strokeRect(x, y, this.cellSize, this.cellSize);
    }

    getCellFromCoordinates(x, y) {
        const col = Math.floor(x / this.cellSize);
        const row = Math.floor(y / this.cellSize);
        
        if (row >= 0 && row < this.rows && col >= 0 && col < this.cols) {
            return { row, col };
        }
        return null;
    }

    getTime() {
        return this.currentTime;
    }

    getRemainingMines() {
        return this.mines - this.flags;
    }

    isGameOver() {
        return this.gameOver;
    }

    isGameWon() {
        return this.gameWon;
    }

    start() {
        if (this.firstClick) {
            this.startTime = Date.now();
        }
    }
}