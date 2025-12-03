class MinesweeperGame {
    constructor() {
        this.canvas = document.getElementById('minesweeper-canvas');
        this.ctx = this.canvas.getContext('2d');
        this.minesweeper = new Minesweeper(this.canvas, this.ctx);
        
        this.timeElement = document.getElementById('minesweeper-time');
        this.minesElement = document.getElementById('minesweeper-mines');
        this.highScoreElement = document.getElementById('minesweeper-high-score');
        this.finalTimeElement = document.getElementById('minesweeper-final-time');
        this.winTimeElement = document.getElementById('minesweeper-win-time');
        this.gameOverElement = document.getElementById('minesweeper-game-over');
        this.gameWonElement = document.getElementById('minesweeper-game-won');
        
        this.highScore = localStorage.getItem('minesweeperHighScore') || 999;
        this.highScoreElement.textContent = this.highScore === 999 ? '--' : this.highScore;
        
        this.init();
    }

    init() {
        this.setupControls();
        this.minesweeper.draw();
        this.updateDisplay();
    }

    setupControls() {
        this.canvas.addEventListener('click', (e) => {
            if (!this.minesweeper.firstClick && !this.minesweeper.isRunning) return;
            
            const rect = this.canvas.getBoundingClientRect();
            const x = e.clientX - rect.left;
            const y = e.clientY - rect.top;
            
            const cell = this.minesweeper.getCellFromCoordinates(x, y);
            if (cell) {
                this.minesweeper.reveal(cell.row, cell.col);
                this.minesweeper.draw();
                this.updateDisplay();
                this.checkGameState();
            }
        });

        this.canvas.addEventListener('contextmenu', (e) => {
            e.preventDefault();
            if (!this.minesweeper.firstClick && !this.minesweeper.isRunning) return;
            
            const rect = this.canvas.getBoundingClientRect();
            const x = e.clientX - rect.left;
            const y = e.clientY - rect.top;
            
            const cell = this.minesweeper.getCellFromCoordinates(x, y);
            if (cell) {
                this.minesweeper.toggleFlag(cell.row, cell.col);
                this.minesweeper.draw();
                this.updateDisplay();
            }
        });
    }

    setDifficulty(level) {
        this.minesweeper.setDifficulty(level);
        this.minesweeper.draw();
        this.updateDisplay();
        this.hideGameMessages();
    }

    start() {
        this.minesweeper.start();
        this.hideGameMessages();
    }

    reset() {
        this.minesweeper.reset();
        this.minesweeper.draw();
        this.updateDisplay();
        this.hideGameMessages();
    }

    updateDisplay() {
        this.timeElement.textContent = this.minesweeper.getTime();
        this.minesElement.textContent = this.minesweeper.getRemainingMines();
    }

    checkGameState() {
        if (this.minesweeper.isGameOver()) {
            this.showGameOver();
        } else if (this.minesweeper.isGameWon()) {
            this.showGameWon();
        }
    }

    showGameOver() {
        this.finalTimeElement.textContent = this.minesweeper.getTime();
        this.gameOverElement.style.display = 'flex';
    }

    showGameWon() {
        const time = this.minesweeper.getTime();
        this.winTimeElement.textContent = time;
        
        if (time < this.highScore) {
            this.highScore = time;
            localStorage.setItem('minesweeperHighScore', this.highScore);
            this.highScoreElement.textContent = this.highScore;
            gameManager.updatePreviewScore('minesweeper');
        }
        
        this.gameWonElement.style.display = 'flex';
    }

    hideGameMessages() {
        this.gameOverElement.style.display = 'none';
        this.gameWonElement.style.display = 'none';
    }
}