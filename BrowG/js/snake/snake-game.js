class SnakeGame {
    constructor() {
        this.canvas = document.getElementById('snake-canvas');
        this.ctx = this.canvas.getContext('2d');
        this.snake = new Snake(this.canvas, this.ctx);
        
        this.scoreElement = document.getElementById('snake-score');
        this.highScoreElement = document.getElementById('snake-high-score');
        this.finalScoreElement = document.getElementById('snake-final-score');
        this.gameOverElement = document.getElementById('snake-game-over');
        
        this.highScore = localStorage.getItem('snakeHighScore') || 0;
        this.highScoreElement.textContent = this.highScore;
        
        this.gameLoop = null;
        this.gameSpeed = 100;
        this.init();
    }

    init() {
        this.setupControls();
        this.snake.draw();
    }

    setupControls() {
        document.addEventListener('keydown', (e) => {
            if (!this.snake.isRunning) return;

            switch(e.key) {
                case 'ArrowLeft': this.snake.changeDirection(-1, 0); break;
                case 'ArrowUp': this.snake.changeDirection(0, -1); break;
                case 'ArrowRight': this.snake.changeDirection(1, 0); break;
                case 'ArrowDown': this.snake.changeDirection(0, 1); break;
            }
        });
    }

    start() {
        if (this.gameLoop) return;
        
        this.snake.start();
        this.gameLoop = setInterval(() => {
            this.update();
            this.render();
        }, this.gameSpeed);
    }

    pause() {
        this.snake.pause();
        if (!this.snake.isRunning && this.gameLoop) {
            clearInterval(this.gameLoop);
            this.gameLoop = null;
        } else if (this.snake.isRunning && !this.gameLoop) {
            this.start();
        }
    }

    reset() {
        if (this.gameLoop) {
            clearInterval(this.gameLoop);
            this.gameLoop = null;
        }
        this.snake.reset();
        this.gameSpeed = 100;
        this.gameOverElement.style.display = 'none';
        this.updateScore();
        this.snake.draw();
    }

    update() {
        this.snake.move();
        
        if (this.snake.isGameOver()) {
            this.endGame();
        } else {
            const newSpeed = Math.max(50, 100 - Math.floor(this.snake.getScore() / 50) * 5);
            if (newSpeed !== this.gameSpeed) {
                this.gameSpeed = newSpeed;
                this.restartGameLoop();
            }
        }
        
        this.updateScore();
    }

    restartGameLoop() {
        if (this.gameLoop) {
            clearInterval(this.gameLoop);
            this.gameLoop = setInterval(() => {
                this.update();
                this.render();
            }, this.gameSpeed);
        }
    }

    render() {
        this.snake.draw();
    }

    updateScore() {
        this.scoreElement.textContent = this.snake.getScore();
    }

    endGame() {
        if (this.gameLoop) {
            clearInterval(this.gameLoop);
            this.gameLoop = null;
        }
        
        const score = this.snake.getScore();
        this.finalScoreElement.textContent = score;
        
        if (score > this.highScore) {
            this.highScore = score;
            localStorage.setItem('snakeHighScore', this.highScore);
            this.highScoreElement.textContent = this.highScore;
            gameManager.updatePreviewScore('snake');
        }
        
        this.gameOverElement.style.display = 'flex';
    }
}