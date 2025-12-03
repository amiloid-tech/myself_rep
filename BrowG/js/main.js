// Глобальные переменные для игр
let snakeGame, doodleGame, minesweeperGame, tetrisGame;

document.addEventListener('DOMContentLoaded', function() {
    console.log('DOM загружен, инициализируем игры...');
    
    // Инициализируем превьюшки
    gameManager.initPreviews();

    // Создаем экземпляры игр с обработкой ошибок
    try {
        console.log('Создаем SnakeGame...');
        snakeGame = new SnakeGame();
        gameManager.registerGame('snake', snakeGame);
        console.log('SnakeGame создана успешно');
    } catch (error) {
        console.error('Ошибка при создании SnakeGame:', error);
    }

    try {
        console.log('Создаем DoodleGame...');
        doodleGame = new DoodleGame();
        gameManager.registerGame('doodle', doodleGame);
        console.log('DoodleGame создана успешно');
    } catch (error) {
        console.error('Ошибка при создании DoodleGame:', error);
    }

    try {
        console.log('Создаем MinesweeperGame...');
        minesweeperGame = new MinesweeperGame();
        gameManager.registerGame('minesweeper', minesweeperGame);
        console.log('MinesweeperGame создана успешно');
    } catch (error) {
        console.error('Ошибка при создании MinesweeperGame:', error);
    }

    try {
        console.log('Создаем TetrisGame...');
        tetrisGame = new TetrisGame();
        gameManager.registerGame('tetris', tetrisGame);
        console.log('TetrisGame создана успешно');
    } catch (error) {
        console.error('Ошибка при создании TetrisGame:', error);
    }

    console.log('Зарегистрированные игры:', Object.keys(gameManager.games));
    
    // Показываем главный экран
    gameManager.showScreen('main');
    
    // Делаем игры глобальными для доступа из HTML
    window.snakeGame = snakeGame;
    window.doodleGame = doodleGame;
    window.minesweeperGame = minesweeperGame;
    window.tetrisGame = tetrisGame;
    
    console.log('Приложение готово!');
});