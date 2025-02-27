let maze, rows, cols;
let playerPosition = { x: 0, y: 0 }; 

function showMenu() {
    document.getElementById('menu').style.display = 'block';
    document.getElementById('authors').style.display = 'none';
    document.getElementById('playButton').style.display = 'none'; 
    document.getElementById('authorsButton').style.display = 'none'; 
}

function showAuthors() {
    document.getElementById('authors').style.display = 'block';
    document.getElementById('menu').style.display = 'none';
}

function hideAuthors() {
    document.getElementById('authors').style.display = 'none';
    document.getElementById('menu').style.display = 'none';
    document.getElementById('playButton').style.display = 'none'; 
    document.getElementById('authorsButton').style.display = 'none'; 
}

function startCustomGame() {
    rows = parseInt(document.getElementById('rowsInput').value);
    cols = parseInt(document.getElementById('colsInput').value);

    if (rows < 5 || cols < 5 || rows > 50 || cols > 50) {
        alert("Введите значения от 5 до 50 для строк и столбцов.");
        return;
    }

    generateMaze();
    playerPosition = { x: 0, y: 0 }; 
    renderMaze();
    document.getElementById('gameArea').style.display = 'block';

    
    document.addEventListener('keydown', handleKeyPress);
}

function generateMaze() {
    maze = Array.from({ length: rows }, () => Array(cols).fill(1)); 

 
    maze[0][0] = 0;

    const directions = [
        { dx: 0, dy: -2 }, 
        { dx: 0, dy: 2 },  
        { dx: -2, dy: 0 }, 
        { dx: 2, dy: 0 }   
    ];

    const stack = [{ x: 0, y: 0 }];

    while (stack.length > 0) {
        const current = stack.pop();
        const { x, y } = current;

        
        const availableDirections = directions.filter(({ dx, dy }) => {
            const newX = x + dx;
            const newY = y + dy;
            return (
                newX >= 0 && newX < cols &&
                newY >= 0 && newY < rows &&
                maze[newY][newX] === 1
            );
        });

        if (availableDirections.length > 0) {
            stack.push(current); 
            
            const randomDirection = availableDirections[Math.floor(Math.random() * availableDirections.length)];
            const newX = x + randomDirection.dx;
            const newY = y + randomDirection.dy;

            maze[newY][newX] = 0; 
            maze[y + randomDirection.dy / 2][x + randomDirection.dx / 2] = 0; 
            stack.push({ x: newX, y: newY }); 

        }
    }

   
    if (rows > 1 && cols > 1) {
        maze[rows - 1][cols - 1] = 0; 
        maze[rows - 2][cols - 1] = 0; 
        maze[rows - 1][cols - 2] = 0; 
    }
}

function renderMaze() {
    const mazeDiv = document.getElementById('maze');
    mazeDiv.style.gridTemplateRows = `repeat(${rows}, 20px)`;
    mazeDiv.style.gridTemplateColumns = `repeat(${cols}, 20px)`;
    mazeDiv.innerHTML = '';

    for (let i = 0; i < rows; i++) {
        for (let j = 0; j < cols; j++) {
            const cell = document.createElement('div');
            cell.classList.add('cell');
            if (maze[i][j] === 1) {
                cell.classList.add('wall');
            }
            if (i === playerPosition.y && j === playerPosition.x) {
                cell.classList.add('start'); 
            }
            if (i === rows - 1 && j === cols - 1) {
                cell.classList.add('end'); 
            }
            mazeDiv.appendChild(cell);
        }
    }
}

function handleKeyPress(event) {
    const { key } = event;

    let newX = playerPosition.x;
    let newY = playerPosition.y;

    if (key === 'ArrowUp') newY -= 1;
    if (key === 'ArrowDown') newY += 1;
    if (key === 'ArrowLeft') newX -= 1;
    if (key === 'ArrowRight') newX += 1;

    if (newX >= 0 && newX < cols && newY >= 0 && newY < rows && maze[newY][newX] !== 1) {
        playerPosition.x = newX;
        playerPosition.y = newY;
    }

    renderMaze();

 
    if (playerPosition.x === cols - 1 && playerPosition.y === rows - 1) {
        showWinMessage();
        document.removeEventListener('keydown', handleKeyPress); 
    } else {
        document.getElementById('status').innerText = '';
    }
}

function showWinMessage() {
    alert('Вы прошли лабиринт!');
    flashMaze(); 
}

let flashing = false;

function flashMaze() {
    if (flashing) return;

    flashing = true;
    const cells = document.querySelectorAll('#maze .cell');

    const interval = setInterval(() => {
        cells.forEach(cell => {
            if (cell.classList.contains('flashing')) {
                cell.classList.remove('flashing');
            } else {
                cell.classList.add('flashing');
            }
        });
    }, 500);

  
    setTimeout(() => {
        clearInterval(interval);
        cells.forEach(cell => cell.classList.remove('flashing'));
        flashing = false;
    }, 5000);
}

function showHelp() {
    const path = findPath(playerPosition.y, playerPosition.x);
    if (path.length === 0) {
        alert('Нет доступного пути к выходу!');
        return;
    }

    path.forEach(({ y, x }) => {
        const cell = document.querySelector(`#maze .cell:nth-child(${y * cols + x + 1})`);
        cell.classList.add('path');
    });
}

function findPath(startY, startX) {
    const directionVectors = [
        { dy: 0, dx: 1 },  
        { dy: 1, dx: 0 },  
        { dy: 0, dx: -1 }, 
        { dy: -1, dx: 0 }  
    ];

    const queue = [{ y: startY, x: startX, path: [] }];
    const visited = Array.from({ length: rows }, () => Array(cols).fill(false));
    visited[startY][startX] = true;

    while (queue.length > 0) {
        const current = queue.shift();


        if (current.y === rows - 1 && current.x === cols - 1) {
            return [...current.path, { y: current.y, x: current.x }];
        }

        for (const { dy, dx } of directionVectors) {
            const newY = current.y + dy;
            const newX = current.x + dx;

           
            if (
                newY >= 0 && newY < rows &&
                newX >= 0 && newX < cols &&
                maze[newY][newX] === 0 &&
                !visited[newY][newX]
            ) {
                visited[newY][newX] = true;
                queue.push({ y: newY, x: newX, path: [...current.path, { y: current.y, x: current.x }] });
            }
        }
    }

    return []; 
}
