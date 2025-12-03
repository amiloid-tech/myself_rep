class TetrisGame {
    constructor() {
        this.canvas = document.getElementById('tetris-canvas');
        this.ctx = this.canvas.getContext('2d');
        this.nextCanvas = document.getElementById('tetris-next-canvas');
        this.nextCtx = this.nextCanvas.getContext('2d');
        
        this.tetris = new Tetris(this.canvas, this.ctx);
        
        this.scoreElement = document.getElementById('tetris-score');
        this.linesElement = document.getElementById('tetris-lines');
        this.levelElement = document.getElementById('tetris-level');
        this.highScoreElement = document.getElementById('tetris-high-score');
        this.finalScoreElement = document.getElementById('tetris-final-score');
        this.gameOverElement = document.getElementById('tetris-game-over');
        
        this.highScore = localStorage.getItem('tetrisHighScore') || 0;
        this.highScoreElement.textContent = this.highScore;
        
        this.gameLoop = null;
        this.dropInterval = 1000;
        this.lastDropTime = 0;
        
        this.init();
    }

    init() {
        this.setupControls();
        this.tetris.draw();
        this.drawNextPiece();
        this.updateDisplay();
    }

    setupControls() {
        document.addEventListener('keydown', (e) => {
            if (!this.tetris.isRunning || this.tetris.isPaused) return;
            
            switch(e.code) {
                case 'ArrowLeft': this.tetris.moveLeft(); break;
                case 'ArrowRight': this.tetris.moveRight(); break;
                case 'ArrowDown': this.tetris.moveDown(); break;
                case 'ArrowUp': this.tetris.rotate(); break;
                case 'Space': 
                    e.preventDefault();
                    this.tetris.hardDrop(); 
                    break;
            }
            this.tetris.draw();
            this.drawNextPiece();
        });

        document.addEventListener('keydown', (e) => {
            if (e.code === 'Space' && this.tetris.isRunning && !this.tetris.gameOver) {
                e.preventDefault();
                this.pause();
            }
        });
    }

    start() {
        this.tetris.start();
        this.hideGameOver();
        this.lastDropTime = performance.now();
        this.startGameLoop();
    }

    pause() {
        this.tetris.pause();
        if (this.tetris.isPaused) {
            cancelAnimationFrame(this.gameLoop);
        } else {
            this.startGameLoop();
        }
    }

    reset() {
        cancelAnimationFrame(this.gameLoop);
        this.tetris.reset();
        this.tetris.draw();
        this.drawNextPiece();
        this.updateDisplay();
        this.hideGameOver();
        this.dropInterval = 1000;
    }

    startGameLoop() {
        const gameStep = (currentTime) => {
            if (!this.tetris.isRunning || this.tetris.isPaused || this.tetris.gameOver) {
                return;
            }
            
            const deltaTime = currentTime - this.lastDropTime;
            
            if (deltaTime > this.dropInterval) {
                this.tetris.moveDown();
                this.lastDropTime = currentTime;
                this.dropInterval = Math.max(100, 1000 - (this.tetris.level - 1) * 100);
            }
            
            this.tetris.draw();
            this.drawNextPiece();
            this.updateDisplay();
            this.checkGameOver();
            
            this.gameLoop = requestAnimationFrame(gameStep);
        };
        
        this.gameLoop = requestAnimationFrame(gameStep);
    }

    drawNextPiece() {
        this.tetris.drawNextPiece(this.nextCanvas, this.nextCtx);
    }

    updateDisplay() {
        this.scoreElement.textContent = this.tetris.getScore();
        this.linesElement.textContent = this.tetris.getLines();
        this.levelElement.textContent = this.tetris.getLevel();
    }

    checkGameOver() {
        if (this.tetris.isGameOver()) {
            cancelAnimationFrame(this.gameLoop);
            this.showGameOver();
            
            const score = this.tetris.getScore();
            if (score > this.highScore) {
                this.highScore = score;
                localStorage.setItem('tetrisHighScore', this.highScore);
                this.highScoreElement.textContent = this.highScore;
                gameManager.updatePreviewScore('tetris');
            }
        }
    }

    showGameOver() {
        this.finalScoreElement.textContent = this.tetris.getScore();
        this.gameOverElement.style.display = 'flex';
    }

    hideGameOver() {
        this.gameOverElement.style.display = 'none';
    }
}