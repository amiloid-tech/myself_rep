class Snake {
    constructor(canvas, ctx) {
        this.canvas = canvas;
        this.ctx = ctx;
        this.gridSize = 20;
        this.tileCount = canvas.width / this.gridSize;
        
        this.reset();
    }

    reset() {
        this.body = [
            {x: 10, y: 10},
            {x: 9, y: 10}, 
            {x: 8, y: 10}
        ];
        this.direction = {x: 1, y: 0};
        this.nextDirection = {x: 1, y: 0};
        this.food = this.generateFood();
        this.score = 0;
        this.gameOver = false;
        this.isRunning = false;
        this.hasMoved = false;
    }

    generateFood() {
        let newFood;
        let foodOnSnake;
        
        do {
            newFood = {
                x: Math.floor(Math.random() * this.tileCount),
                y: Math.floor(Math.random() * this.tileCount)
            };
            
            foodOnSnake = this.body.some(segment => 
                segment.x === newFood.x && segment.y === newFood.y
            );
        } while (foodOnSnake);
        
        return newFood;
    }

    move() {
        if (!this.isRunning || this.gameOver) return;

        this.direction = {...this.nextDirection};
        this.hasMoved = true;

        const head = {
            x: this.body[0].x + this.direction.x,
            y: this.body[0].y + this.direction.y
        };

        if (head.x < 0 || head.x >= this.tileCount || 
            head.y < 0 || head.y >= this.tileCount) {
            this.gameOver = true;
            return;
        }

        for (let i = 0; i < this.body.length - 1; i++) {
            if (head.x === this.body[i].x && head.y === this.body[i].y) {
                this.gameOver = true;
                return;
            }
        }

        this.body.unshift(head);

        if (head.x === this.food.x && head.y === this.food.y) {
            this.score += 10;
            this.food = this.generateFood();
        } else {
            this.body.pop();
        }
    }

    changeDirection(dx, dy) {
        if (!this.hasMoved) return;
        
        if ((this.direction.x !== -dx || this.direction.y !== -dy)) {
            this.nextDirection.x = dx;
            this.nextDirection.y = dy;
            this.hasMoved = false;
        }
    }

    draw() {
        this.ctx.fillStyle = '#111';
        this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);

        this.drawGrid();
        this.drawFood();
        this.drawSnake();
        this.drawScore();
    }

    drawGrid() {
        this.ctx.strokeStyle = '#222';
        this.ctx.lineWidth = 0.5;
        
        for (let x = 0; x < this.canvas.width; x += this.gridSize) {
            this.ctx.beginPath();
            this.ctx.moveTo(x, 0);
            this.ctx.lineTo(x, this.canvas.height);
            this.ctx.stroke();
        }
        
        for (let y = 0; y < this.canvas.height; y += this.gridSize) {
            this.ctx.beginPath();
            this.ctx.moveTo(0, y);
            this.ctx.lineTo(this.canvas.width, y);
            this.ctx.stroke();
        }
    }

    drawFood() {
        this.ctx.fillStyle = '#FF5252';
        this.ctx.strokeStyle = '#D32F2F';
        this.ctx.lineWidth = 2;
        
        const foodSize = this.gridSize - 4;
        const offset = (this.gridSize - foodSize) / 2;
        
        this.ctx.fillRect(
            this.food.x * this.gridSize + offset, 
            this.food.y * this.gridSize + offset, 
            foodSize, 
            foodSize
        );
        this.ctx.strokeRect(
            this.food.x * this.gridSize + offset, 
            this.food.y * this.gridSize + offset, 
            foodSize, 
            foodSize
        );
    }

    drawSnake() {
        for (let i = 0; i < this.body.length; i++) {
            const segment = this.body[i];
            const segmentSize = this.gridSize - 2;
            const offset = (this.gridSize - segmentSize) / 2;
            
            if (i === 0) {
                this.ctx.fillStyle = '#4CAF50';
                this.ctx.strokeStyle = '#388E3C';
                this.ctx.lineWidth = 2;
                
                this.ctx.fillRect(
                    segment.x * this.gridSize + offset, 
                    segment.y * this.gridSize + offset, 
                    segmentSize, 
                    segmentSize
                );
                this.ctx.strokeRect(
                    segment.x * this.gridSize + offset, 
                    segment.y * this.gridSize + offset, 
                    segmentSize, 
                    segmentSize
                );
            } else {
                const intensity = 255 - (i * 10);
                this.ctx.fillStyle = `rgb(76, ${Math.max(100, intensity)}, 50)`;
                this.ctx.strokeStyle = '#388E3C';
                
                this.ctx.fillRect(
                    segment.x * this.gridSize + offset, 
                    segment.y * this.gridSize + offset, 
                    segmentSize, 
                    segmentSize
                );
                this.ctx.strokeRect(
                    segment.x * this.gridSize + offset, 
                    segment.y * this.gridSize + offset, 
                    segmentSize, 
                    segmentSize
                );
            }
        }
    }

    drawScore() {
        this.ctx.fillStyle = 'rgba(255, 255, 255, 0.8)';
        this.ctx.font = '16px Arial';
        this.ctx.textAlign = 'left';
        this.ctx.fillText(`Счет: ${this.score}`, 10, 25);
        this.ctx.fillText(`Длина: ${this.body.length}`, 10, 45);
    }

    start() {
        this.isRunning = true;
        this.hasMoved = false;
    }

    pause() {
        this.isRunning = !this.isRunning;
    }

    getScore() {
        return this.score;
    }

    isGameOver() {
        return this.gameOver;
    }
}