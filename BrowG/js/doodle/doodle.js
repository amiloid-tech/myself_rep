class Doodle {
    constructor(canvas, ctx) {
        this.canvas = canvas;
        this.ctx = ctx;
        
        this.reset();
    }

    reset() {
        this.platforms = [];
        this.createPlatforms();
        
        const spawnPlatform = this.findSpawnPlatform();
        
        this.player = {
            x: spawnPlatform.x + spawnPlatform.width / 2 - 25,
            y: spawnPlatform.y - 50,
            width: 50,
            height: 50,
            velocityY: 0,
            velocityX: 0,
            isOnPlatform: false
        };
        
        this.score = 0;
        this.scrollOffset = 0;
        this.gameOver = false;
        this.isRunning = false;
        this.gravity = 0.4;
        this.jumpForce = -12;
        this.maxFallSpeed = 15;
    }

    findSpawnPlatform() {
        const bottomPlatforms = this.platforms.filter(platform => 
            platform.y > this.canvas.height - 200
        );
        
        if (bottomPlatforms.length > 0) {
            return bottomPlatforms.reduce((highest, platform) => 
                platform.y < highest.y ? platform : highest
            );
        }
        
        return {
            x: this.canvas.width / 2 - 35,
            y: this.canvas.height - 100,
            width: 70,
            height: 15
        };
    }

    createPlatforms() {
        const spawnPlatform = {
            x: this.canvas.width / 2 - 35,
            y: this.canvas.height - 100,
            width: 70,
            height: 15
        };
        this.platforms.push(spawnPlatform);

        for (let i = 1; i < 12; i++) {
            this.platforms.push({
                x: Math.random() * (this.canvas.width - 70),
                y: this.canvas.height - i * 80 - 50,
                width: 70,
                height: 15
            });
        }
    }

    update() {
        if (!this.isRunning || this.gameOver) return;

        this.player.velocityY = Math.min(this.player.velocityY + this.gravity, this.maxFallSpeed);
        this.player.y += this.player.velocityY;
        this.player.x += this.player.velocityX;
        
        if (this.player.x < -this.player.width) {
            this.player.x = this.canvas.width;
        } else if (this.player.x > this.canvas.width) {
            this.player.x = -this.player.width;
        }
        
        this.player.isOnPlatform = false;
        for (let platform of this.platforms) {
            if (this.checkPlatformCollision(platform)) {
                this.player.isOnPlatform = true;
                this.player.y = platform.y - this.player.height;
                this.player.velocityY = 0;
                
                if (!this.player.wasOnPlatform) {
                    this.player.velocityY = this.jumpForce;
                }
                break;
            }
        }
        
        this.player.wasOnPlatform = this.player.isOnPlatform;
        
        if (this.player.y < this.canvas.height / 3) {
            const scrollAmount = this.canvas.height / 3 - this.player.y;
            this.scrollOffset += scrollAmount;
            this.player.y = this.canvas.height / 3;
            
            for (let i = 0; i < this.platforms.length; i++) {
                this.platforms[i].y += scrollAmount;
                
                if (this.platforms[i].y > this.canvas.height) {
                    this.platforms[i] = this.generateNewPlatform();
                }
            }
            
            this.score += Math.floor(scrollAmount / 5);
        }
        
        if (this.player.y > this.canvas.height) {
            this.gameOver = true;
        }
    }

    checkPlatformCollision(platform) {
        return (
            this.player.velocityY > 0 &&
            this.player.x + this.player.width > platform.x &&
            this.player.x < platform.x + platform.width &&
            this.player.y + this.player.height > platform.y &&
            this.player.y + this.player.height < platform.y + platform.height + 5
        );
    }

    generateNewPlatform() {
        return {
            x: Math.random() * (this.canvas.width - 70),
            y: -20,
            width: 70,
            height: 15
        };
    }

    moveLeft() {
        this.player.velocityX = -7;
    }

    moveRight() {
        this.player.velocityX = 7;
    }

    stopMoving() {
        this.player.velocityX = 0;
    }

    jump() {
        if (this.player.isOnPlatform) {
            this.player.velocityY = this.jumpForce;
        }
    }

    draw() {
        this.drawBackground();
        
        this.ctx.fillStyle = '#4CAF50';
        this.ctx.strokeStyle = '#2E7D32';
        this.ctx.lineWidth = 2;
        
        for (let platform of this.platforms) {
            this.ctx.fillRect(platform.x, platform.y, platform.width, platform.height);
            this.ctx.strokeRect(platform.x, platform.y, platform.width, platform.height);
        }

        this.drawPlayer();
    }

    drawBackground() {
        const gradient = this.ctx.createLinearGradient(0, 0, 0, this.canvas.height);
        gradient.addColorStop(0, '#87CEEB');
        gradient.addColorStop(1, '#E0F7FA');
        this.ctx.fillStyle = gradient;
        this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);
        
        this.ctx.fillStyle = 'rgba(255, 255, 255, 0.7)';
        for (let i = 0; i < 3; i++) {
            const x = (Date.now() / 3000 + i * 2) % (this.canvas.width + 100) - 50;
            const y = 50 + i * 40;
            this.drawCloud(x, y, 30 + i * 10);
        }
    }

    drawCloud(x, y, size) {
        this.ctx.beginPath();
        this.ctx.arc(x, y, size, 0, Math.PI * 2);
        this.ctx.arc(x + size * 0.8, y - size * 0.2, size * 0.8, 0, Math.PI * 2);
        this.ctx.arc(x + size * 1.5, y, size * 0.7, 0, Math.PI * 2);
        this.ctx.arc(x + size * 1.2, y + size * 0.3, size * 0.9, 0, Math.PI * 2);
        this.ctx.fill();
    }

    drawPlayer() {
        this.ctx.fillStyle = '#FFCC00';
        this.ctx.strokeStyle = '#FF9800';
        this.ctx.lineWidth = 3;
        
        this.ctx.fillRect(this.player.x, this.player.y, this.player.width, this.player.height);
        this.ctx.strokeRect(this.player.x, this.player.y, this.player.width, this.player.height);
        
        this.ctx.fillStyle = '#000';
        const eyeOffset = this.player.velocityX > 0 ? 5 : -5;
        
        this.ctx.fillRect(this.player.x + 15, this.player.y + 15, 8, 8);
        this.ctx.fillRect(this.player.x + this.player.width - 23, this.player.y + 15, 8, 8);
        
        this.ctx.fillRect(this.player.x + 20, this.player.y + 35, this.player.width - 40, 4);
        
        if (this.player.velocityY !== 0) {
            this.ctx.fillStyle = '#FF9800';
            const legOffset = Math.sin(Date.now() / 100) * 3;
            this.ctx.fillRect(this.player.x + 10, this.player.y + this.player.height, 8, 10 + legOffset);
            this.ctx.fillRect(this.player.x + this.player.width - 18, this.player.y + this.player.height, 8, 10 - legOffset);
        }
    }

    start() {
        this.isRunning = true;
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