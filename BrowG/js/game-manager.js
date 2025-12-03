class GameManager {
    constructor() {
        this.games = {};
        this.currentGame = null;
        this.screens = {
            main: document.getElementById('main-screen'),
            snake: document.getElementById('snake-screen'),
            doodle: document.getElementById('doodle-screen'),
            minesweeper: document.getElementById('minesweeper-screen'),
            tetris: document.getElementById('tetris-screen')
        };
        
        console.log('GameManager создан');
    }

    registerGame(gameName, gameInstance) {
        this.games[gameName] = gameInstance;
        console.log(`Игра "${gameName}" зарегистрирована`);
        this.updatePreviewScore(gameName);
    }

    showScreen(screenName) {
        console.log('Показываем экран:', screenName);
        
        // Скрываем все экраны
        Object.values(this.screens).forEach(screen => {
            screen.classList.remove('active');
        });
        
        // Показываем нужный экран
        if (this.screens[screenName]) {
            this.screens[screenName].classList.add('active');
            this.currentGame = screenName;
        }
    }

    showMainScreen() {
        this.showScreen('main');
    }

    showGameScreen(gameName) {
        if (this.games[gameName]) {
            this.showScreen(gameName);
        } else {
            console.error('Игра не найдена:', gameName);
        }
    }

    startGame(gameName) {
        if (this.games[gameName]) {
            console.log('Запускаем игру:', gameName);
            this.games[gameName].start();
        }
    }

    pauseGame(gameName) {
        if (this.games[gameName]) {
            this.games[gameName].pause();
        }
    }

    resetGame(gameName) {
        if (this.games[gameName]) {
            this.games[gameName].reset();
        }
    }

    updatePreviewScore(gameName) {
        const highScore = localStorage.getItem(`${gameName}HighScore`) || 0;
        const element = document.getElementById(`${gameName}-preview-score`);
        if (element) {
            element.textContent = highScore;
        }
    }

    // Инициализация превью игр
    initPreviews() {
        console.log('Инициализация превью...');
        
        // Обработчики для превьюшек
        document.querySelectorAll('.game-preview').forEach(preview => {
            preview.addEventListener('click', () => {
                const gameName = preview.dataset.game;
                console.log('Клик по игре:', gameName);
                this.showGameScreen(gameName);
            });
        });

        // Создаем анимированные превью
        this.createSnakePreview();
        this.createDoodlePreview();
        this.createMinesweeperPreview();
        this.createTetrisPreview();
        
        console.log('Превью инициализированы');
    }

    createSnakePreview() {
        const canvas = document.getElementById('snake-preview');
        if (!canvas) return;
        
        const ctx = canvas.getContext('2d');
        const gridSize = 10;
        const tileCount = canvas.width / gridSize;

        let snake = [
            {x: 5, y: 5},
            {x: 4, y: 5},
            {x: 3, y: 5}
        ];
        let direction = {x: 1, y: 0};
        let food = {x: 8, y: 8};

        function draw() {
            ctx.fillStyle = '#111';
            ctx.fillRect(0, 0, canvas.width, canvas.height);

            ctx.fillStyle = '#FF5252';
            ctx.fillRect(food.x * gridSize, food.y * gridSize, gridSize-1, gridSize-1);

            ctx.fillStyle = '#4CAF50';
            snake.forEach(segment => {
                ctx.fillRect(segment.x * gridSize, segment.y * gridSize, gridSize-1, gridSize-1);
            });
        }

        function update() {
            const head = {x: snake[0].x + direction.x, y: snake[0].y + direction.y};

            if (head.x < 0) head.x = tileCount - 1;
            if (head.x >= tileCount) head.x = 0;
            if (head.y < 0) head.y = tileCount - 1;
            if (head.y >= tileCount) head.y = 0;

            snake.unshift(head);
            
            if (head.x === food.x && head.y === food.y) {
                food = {
                    x: Math.floor(Math.random() * tileCount),
                    y: Math.floor(Math.random() * tileCount)
                };
            } else {
                snake.pop();
            }

            if (Math.random() < 0.1) {
                const directions = [{x:1,y:0}, {x:-1,y:0}, {x:0,y:1}, {x:0,y:-1}];
                const newDirection = directions[Math.floor(Math.random() * directions.length)];
                
                if (newDirection.x !== -direction.x && newDirection.y !== -direction.y) {
                    direction = newDirection;
                }
            }
        }

        setInterval(() => {
            update();
            draw();
        }, 200);

        draw();
    }

    createDoodlePreview() {
        const canvas = document.getElementById('doodle-preview');
        if (!canvas) return;
        
        const ctx = canvas.getContext('2d');
        
        const player = {
            x: canvas.width / 2 - 15,
            y: canvas.height - 80,
            width: 30,
            height: 30,
            velocityX: 2,
            velocityY: 0
        };

        const platforms = [
            {x: canvas.width / 2 - 20, y: canvas.height - 50, width: 40, height: 8},
            {x: 30, y: canvas.height - 100, width: 40, height: 8},
            {x: 100, y: canvas.height - 150, width: 40, height: 8},
            {x: 50, y: canvas.height - 200, width: 40, height: 8}
        ];

        function draw() {
            const gradient = ctx.createLinearGradient(0, 0, 0, canvas.height);
            gradient.addColorStop(0, '#87CEEB');
            gradient.addColorStop(1, '#E0F7FA');
            ctx.fillStyle = gradient;
            ctx.fillRect(0, 0, canvas.width, canvas.height);

            ctx.fillStyle = '#4CAF50';
            platforms.forEach(platform => {
                ctx.fillRect(platform.x, platform.y, platform.width, platform.height);
            });

            ctx.fillStyle = '#FFCC00';
            ctx.fillRect(player.x, player.y, player.width, player.height);
            
            ctx.fillStyle = '#000';
            ctx.fillRect(player.x + 8, player.y + 8, 5, 5);
            ctx.fillRect(player.x + player.width - 13, player.y + 8, 5, 5);
            ctx.fillRect(player.x + 10, player.y + 20, player.width - 20, 3);
        }

        function update() {
            player.x += player.velocityX;
            player.velocityY += 0.2;
            player.y += player.velocityY;

            if (player.x <= 0 || player.x + player.width >= canvas.width) {
                player.velocityX = -player.velocityX;
            }

            platforms.forEach(platform => {
                if (player.velocityY > 0 &&
                    player.x + player.width > platform.x &&
                    player.x < platform.x + platform.width &&
                    player.y + player.height > platform.y &&
                    player.y + player.height < platform.y + platform.height + 5) {
                    
                    player.y = platform.y - player.height;
                    player.velocityY = -4;
                }
            });

            if (player.y > canvas.height - 50) {
                player.y = canvas.height - 80;
                player.velocityY = -4;
            }
        }

        setInterval(() => {
            update();
            draw();
        }, 100);

        draw();
    }

    createMinesweeperPreview() {
        const canvas = document.getElementById('minesweeper-preview');
        if (!canvas) return;
        
        const ctx = canvas.getContext('2d');
        const size = 8;
        const cellSize = canvas.width / size;

        function draw() {
            ctx.fillStyle = '#1a1a1a';
            ctx.fillRect(0, 0, canvas.width, canvas.height);

            ctx.strokeStyle = '#333';
            ctx.lineWidth = 1;
            
            for (let i = 0; i <= size; i++) {
                ctx.beginPath();
                ctx.moveTo(i * cellSize, 0);
                ctx.lineTo(i * cellSize, canvas.height);
                ctx.stroke();
                
                ctx.beginPath();
                ctx.moveTo(0, i * cellSize);
                ctx.lineTo(canvas.width, i * cellSize);
                ctx.stroke();
            }

            for (let i = 0; i < 5; i++) {
                const x = Math.floor(Math.random() * size);
                const y = Math.floor(Math.random() * size);
                
                ctx.fillStyle = '#444';
                ctx.fillRect(x * cellSize + 1, y * cellSize + 1, cellSize - 2, cellSize - 2);
                
                if (Math.random() > 0.7) {
                    ctx.fillStyle = ['blue', 'green', 'red', 'darkblue'][Math.floor(Math.random() * 4)];
                    ctx.font = '12px Arial';
                    ctx.textAlign = 'center';
                    ctx.textBaseline = 'middle';
                    ctx.fillText(
                        Math.floor(Math.random() * 5 + 1).toString(),
                        x * cellSize + cellSize / 2,
                        y * cellSize + cellSize / 2
                    );
                }
            }

            for (let i = 0; i < 3; i++) {
                const x = Math.floor(Math.random() * size);
                const y = Math.floor(Math.random() * size);
                
                ctx.fillStyle = 'red';
                ctx.beginPath();
                ctx.moveTo(x * cellSize + cellSize / 2, y * cellSize + 5);
                ctx.lineTo(x * cellSize + cellSize / 2, y * cellSize + cellSize - 5);
                ctx.stroke();
                
                ctx.beginPath();
                ctx.moveTo(x * cellSize + cellSize / 2, y * cellSize + 5);
                ctx.lineTo(x * cellSize + cellSize - 5, y * cellSize + 15);
                ctx.lineTo(x * cellSize + 5, y * cellSize + 15);
                ctx.closePath();
                ctx.fill();
            }
        }

        draw();
        setInterval(draw, 2000);
    }

    createTetrisPreview() {
        const canvas = document.getElementById('tetris-preview');
        if (!canvas) return;
        
        const ctx = canvas.getContext('2d');
        const blockSize = 15;
        const cols = 10;
        const rows = 16;

        const pieces = [
            { shape: [[1,1,1,1]], color: '#00FFFF' },
            { shape: [[1,1],[1,1]], color: '#FFFF00' },
            { shape: [[1,1,1],[0,1,0]], color: '#800080' },
            { shape: [[1,1,1],[1,0,0]], color: '#FFA500' },
            { shape: [[1,1,1],[0,0,1]], color: '#0000FF' },
            { shape: [[1,1,0],[0,1,1]], color: '#00FF00' },
            { shape: [[0,1,1],[1,1,0]], color: '#FF0000' }
        ];

        let currentPiece = null;
        let pieceX = 0;
        let pieceY = 0;

        function getRandomPiece() {
            return JSON.parse(JSON.stringify(pieces[Math.floor(Math.random() * pieces.length)]));
        }

        function draw() {
            ctx.fillStyle = '#000';
            ctx.fillRect(0, 0, canvas.width, canvas.height);

            ctx.strokeStyle = '#333';
            for (let x = 0; x <= cols; x++) {
                ctx.beginPath();
                ctx.moveTo(x * blockSize, 0);
                ctx.lineTo(x * blockSize, rows * blockSize);
                ctx.stroke();
            }
            for (let y = 0; y <= rows; y++) {
                ctx.beginPath();
                ctx.moveTo(0, y * blockSize);
                ctx.lineTo(cols * blockSize, y * blockSize);
                ctx.stroke();
            }

            if (currentPiece) {
                ctx.fillStyle = currentPiece.color;
                for (let y = 0; y < currentPiece.shape.length; y++) {
                    for (let x = 0; x < currentPiece.shape[y].length; x++) {
                        if (currentPiece.shape[y][x]) {
                            ctx.fillRect(
                                (pieceX + x) * blockSize,
                                (pieceY + y) * blockSize,
                                blockSize - 1,
                                blockSize - 1
                            );
                        }
                    }
                }
            }
        }

        function update() {
            if (!currentPiece) {
                currentPiece = getRandomPiece();
                pieceX = Math.floor((cols - currentPiece.shape[0].length) / 2);
                pieceY = 0;
            }

            pieceY++;

            if (pieceY + currentPiece.shape.length > rows) {
                currentPiece = null;
            }
        }

        setInterval(() => {
            update();
            draw();
        }, 500);

        draw();
    }
}

// Создаем глобальный экземпляр
const gameManager = new GameManager();