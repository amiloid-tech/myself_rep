class DoodleGame {
    constructor() {
        this.canvas = document.getElementById('doodle-canvas');
        this.ctx = this.canvas.getContext('2d');
        this.doodle = new Doodle(this.canvas, this.ctx);
        
        this.scoreElement = document.getElementById('doodle-score');
        this.highScoreElement = document.getElementById('doodle-high-score');
        this.finalScoreElement = document.getElementById('doodle-final-score');
        this.gameOverElement = document.getElementById('doodle-game-over');
        
        this.highScore = localStorage.getItem('doodleHighScore') || 0;
        this.highScoreElement.textContent = this.highScore;
        
        this.gameLoop = null;
        this.lastTimestamp = 0;
        this.init();
    }

    init() {
        this.setupControls();
        this.doodle.draw();
    }

    setupControls() {
        document.addEventListener('keydown', (e) => {
            if (!this.doodle.isRunning) return;

            switch(e.key) {
                case 'ArrowLeft':
                    this.doodle.moveLeft();
                    break;
                case 'ArrowRight':
                    this.doodle.moveRight();
                    break;
                case 'ArrowUp':
                case ' ':
                    this.doodle.jump();
                    break;
            }
        });

        document.addEventListener('keyup', (e) => {
            if (e.key === 'ArrowLeft' || e.key === 'ArrowRight') {
                this.doodle.stopMoving();
            }
        });
    }

    start() {
        if (this.gameLoop) return;
        
        this.doodle.start();
        this.lastTimestamp = performance.now();
        this.gameLoop = requestAnimationFrame(this.animate.bind(this));
    }

    pause() {
        this.doodle.pause();
        if (!this.doodle.isRunning && this.gameLoop) {
            cancelAnimationFrame(this.gameLoop);
            this.gameLoop = null;
        } else if (this.doodle.isRunning && !this.gameLoop) {
            this.start();
        }
    }

    reset() {
        if (this.gameLoop) {
            cancelAnimationFrame(this.gameLoop);
            this.gameLoop = null;
        }
        this.doodle.reset();
        this.gameOverElement.style.display = 'none';
        this.updateScore();
        this.doodle.draw();
    }

    animate(timestamp) {
        const deltaTime = timestamp - this.lastTimestamp;
        this.lastTimestamp = timestamp;

        this.update();
        this.render();
        
        if (this.doodle.isRunning && !this.doodle.isGameOver()) {
            this.gameLoop = requestAnimationFrame(this.animate.bind(this));
        } else if (this.doodle.isGameOver()) {
            this.endGame();
        }
    }

    update() {
        this.doodle.update();
        this.updateScore();
    }

    render() {
        this.doodle.draw();
    }

    updateScore() {
        this.scoreElement.textContent = this.doodle.getScore();
    }

    endGame() {
        if (this.gameLoop) {
            cancelAnimationFrame(this.gameLoop);
            this.gameLoop = null;
        }
        
        const score = this.doodle.getScore();
        this.finalScoreElement.textContent = score;
        
        if (score > this.highScore) {
            this.highScore = score;
            localStorage.setItem('doodleHighScore', this.highScore);
            this.highScoreElement.textContent = this.highScore;
            gameManager.updatePreviewScore('doodle');
        }
        
        this.gameOverElement.style.display = 'flex';
    }
}