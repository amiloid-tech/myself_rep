class TextAnalyzer {
    constructor() {
        this.previousStats = {};
        this.analysisHistory = [];
        this.init();
    }

    init() {
        this.bindEvents();
        this.loadTheme();
        this.setExampleText();
    }

    bindEvents() {
        // Основные элементы
        this.textInput = document.getElementById('text-input');
        this.themeBtn = document.getElementById('theme-btn');
        this.clearBtn = document.getElementById('clear-btn');
        this.pasteBtn = document.getElementById('paste-btn');
        this.exampleBtn = document.getElementById('example-btn');

        // Кнопки функций
        this.keywordBtn = document.getElementById('keyword-btn');
        this.exportBtn = document.getElementById('export-btn');
        this.historyBtn = document.getElementById('history-btn');
        this.settingsBtn = document.getElementById('settings-btn');

        // Модальные окна
        this.keywordModal = document.getElementById('keyword-modal');
        this.exportModal = document.getElementById('export-modal');

        // Слушатели событий
        this.textInput.addEventListener('input', () => this.updateStats());
        this.themeBtn.addEventListener('click', () => this.toggleTheme());
        this.clearBtn.addEventListener('click', () => this.clearText());
        this.pasteBtn.addEventListener('click', () => this.pasteText());
        this.exampleBtn.addEventListener('click', () => this.setExampleText());

        // Функциональные кнопки
        this.keywordBtn.addEventListener('click', () => this.showKeywords());
        this.exportBtn.addEventListener('click', () => this.showExportModal());
        this.historyBtn.addEventListener('click', () => this.showHistory());
        this.settingsBtn.addEventListener('click', () => this.showSettings());

        // Закрытие модальных окон
        document.querySelectorAll('.close').forEach(closeBtn => {
            closeBtn.addEventListener('click', (e) => {
                e.target.closest('.modal').style.display = 'none';
            });
        });

        // Клик вне модального окна
        window.addEventListener('click', (e) => {
            if (e.target.classList.contains('modal')) {
                e.target.style.display = 'none';
            }
        });

        // Экспорт
        document.getElementById('export-json').addEventListener('click', () => this.exportData('json'));
        document.getElementById('export-csv').addEventListener('click', () => this.exportData('csv'));
        document.getElementById('export-txt').addEventListener('click', () => this.exportData('txt'));

        // Сохранение при разгрузке страницы
        window.addEventListener('beforeunload', () => this.saveToLocalStorage());
        
        // Загрузка из localStorage
        this.loadFromLocalStorage();
    }

    updateStats() {
        const text = this.textInput.value;
        const stats = this.calculateStats(text);
        
        this.updateDisplay(stats);
        this.updateTrends(stats);
        this.updateProgressBars(stats);
        this.saveToLocalStorage();
        
        // Сохраняем в историю каждые 10 изменений
        if (this.analysisHistory.length === 0 || 
            this.analysisHistory[this.analysisHistory.length - 1].wordCount !== stats.wordCount) {
            this.analysisHistory.push({
                ...stats,
                timestamp: new Date().toISOString()
            });
            
            if (this.analysisHistory.length > 50) {
                this.analysisHistory.shift();
            }
        }
    }

    calculateStats(text) {
        // Основные метрики
        const charCount = text.length;
        const charNoSpaces = text.replace(/\s/g, '').length;
        const words = text.trim().split(/\s+/).filter(word => word.length > 0);
        const wordCount = text.trim() === '' ? 0 : words.length;
        
        // Предложения и абзацы
        const sentences = text.split(/[.!?]+/).filter(sentence => sentence.trim().length > 0);
        const sentenceCount = sentences.length;
        const paragraphs = text.split(/\n+/).filter(paragraph => paragraph.trim().length > 0);
        const paragraphCount = text.trim() === '' ? 0 : paragraphs.length;
        
        // Дополнительные расчеты
        const totalWordLength = words.reduce((total, word) => total + word.length, 0);
        const avgWordLength = wordCount > 0 ? (totalWordLength / wordCount) : 0;
        
        const totalSentenceLength = sentences.reduce((total, sentence) => {
            const sentenceWords = sentence.trim().split(/\s+/).filter(word => word.length > 0);
            return total + sentenceWords.length;
        }, 0);
        const avgSentenceLength = sentenceCount > 0 ? (totalSentenceLength / sentenceCount) : 0;
        
        const readingTime = wordCount > 0 ? Math.ceil(wordCount / 200) : 0;
        
        // Плотность текста (отношение символов без пробелов к общему количеству)
        const density = charCount > 0 ? (charNoSpaces / charCount) * 100 : 0;
        
        // Заполнение (процент от "идеального" текста в 1000 символов)
        const fillPercentage = Math.min((charCount / 1000) * 100, 100);

        return {
            charCount,
            charNoSpaces,
            wordCount,
            sentenceCount,
            paragraphCount,
            avgWordLength: Number(avgWordLength.toFixed(1)),
            avgSentenceLength: Number(avgSentenceLength.toFixed(1)),
            readingTime,
            density: Number(density.toFixed(1)),
            fillPercentage: Number(fillPercentage.toFixed(1)),
            words
        };
    }

    updateDisplay(stats) {
        // Основные карточки
        document.getElementById('word-count').textContent = stats.wordCount;
        document.getElementById('char-count').textContent = stats.charCount;
        document.getElementById('char-no-spaces-count').textContent = stats.charNoSpaces;
        document.getElementById('sentence-count').textContent = stats.sentenceCount;

        // Детальная статистика
        document.getElementById('detail-char-count').textContent = stats.charCount;
        document.getElementById('detail-char-no-spaces').textContent = stats.charNoSpaces;
        document.getElementById('detail-word-count').textContent = stats.wordCount;
        document.getElementById('detail-sentence-count').textContent = stats.sentenceCount;
        document.getElementById('paragraph-count').textContent = stats.paragraphCount;
        document.getElementById('avg-word-length').textContent = stats.avgWordLength;
        document.getElementById('avg-sentence-length').textContent = stats.avgSentenceLength;
        document.getElementById('reading-time').textContent = `${stats.readingTime} мин`;
    }

    updateTrends(stats) {
        if (Object.keys(this.previousStats).length === 0) {
            this.previousStats = stats;
            return;
        }

        const trends = {
            wordCount: this.calculateTrend(stats.wordCount, this.previousStats.wordCount),
            charCount: this.calculateTrend(stats.charCount, this.previousStats.charCount),
            charNoSpaces: this.calculateTrend(stats.charNoSpaces, this.previousStats.charNoSpaces),
            sentenceCount: this.calculateTrend(stats.sentenceCount, this.previousStats.sentenceCount)
        };

        this.updateTrendDisplay('word-trend', trends.wordCount);
        this.updateTrendDisplay('char-trend', trends.charCount);
        this.updateTrendDisplay('chars-no-spaces-trend', trends.charNoSpaces);
        this.updateTrendDisplay('sentence-trend', trends.sentenceCount);

        this.previousStats = stats;
    }

    calculateTrend(current, previous) {
        if (previous === 0) return current > 0 ? '↑' : '';
        const diff = current - previous;
        if (diff > 0) return '↑';
        if (diff < 0) return '↓';
        return '→';
    }

    updateTrendDisplay(elementId, trend) {
        const element = document.getElementById(elementId);
        if (!element) return;

        element.textContent = trend;
        element.className = 'stat-trend ' + 
            (trend === '↑' ? 'trend-up' : 
             trend === '↓' ? 'trend-down' : '');
    }

    updateProgressBars(stats) {
        const densityProgress = document.getElementById('density-progress');
        const fillProgress = document.getElementById('fill-progress');
        const densityValue = document.getElementById('density-value');
        const fillValue = document.getElementById('fill-value');

        if (densityProgress) {
            densityProgress.style.width = `${stats.density}%`;
            densityValue.textContent = `${stats.density}%`;
        }

        if (fillProgress) {
            fillProgress.style.width = `${stats.fillPercentage}%`;
            fillValue.textContent = `${stats.fillPercentage}%`;
        }
    }

    toggleTheme() {
        const currentTheme = document.body.getAttribute('data-theme') || 'light';
        const newTheme = currentTheme === 'light' ? 'dark' : 'light';
        
        document.body.setAttribute('data-theme', newTheme);
        localStorage.setItem('theme', newTheme);
        
        // Обновляем иконку темы
        const themeIcon = document.querySelector('.theme-icon');
        themeIcon.textContent = newTheme === 'light' ? '🌙' : '☀️';
    }

    loadTheme() {
        const savedTheme = localStorage.getItem('theme') || 'light';
        document.body.setAttribute('data-theme', savedTheme);
        
        const themeIcon = document.querySelector('.theme-icon');
        themeIcon.textContent = savedTheme === 'light' ? '🌙' : '☀️';
    }

    clearText() {
        this.textInput.value = '';
        this.updateStats();
        this.textInput.focus();
    }

    async pasteText() {
        try {
            const text = await navigator.clipboard.readText();
            this.textInput.value = text;
            this.updateStats();
        } catch (err) {
            // Fallback для браузеров без поддержки Clipboard API
            this.textInput.focus();
            document.execCommand('paste');
            setTimeout(() => this.updateStats(), 100);
        }
    }

    setExampleText() {
        const exampleText = `Добро пожаловать в Текст Анализатор Pro! 🚀

Это современный инструмент для анализа текста, который поможет вам:
• Подсчитать слова, символы и предложения
• Оценить время чтения
• Проанализировать структуру текста
• Выявить ключевые слова

Просто введите или вставьте свой текст, и все метрики будут рассчитаны автоматически в реальном времени!

Попробуйте изменить этот текст и посмотрите, как изменяется статистика. Этот инструмент особенно полезен для:
- Писателей и блогеров
- Студентов и исследователей
- Копирайтеров и маркетологов
- Всех, кто работает с текстом

Наслаждайтесь использованием! ✨`;
        
        this.textInput.value = exampleText;
        this.updateStats();
    }

    showKeywords() {
        const stats = this.calculateStats(this.textInput.value);
        const keywordList = document.getElementById('keyword-list');
        
        // Простой алгоритм для поиска ключевых слов (можно улучшить)
        const wordFreq = {};
        stats.words.forEach(word => {
            const cleanWord = word.toLowerCase().replace(/[^\wа-яё]/gi, '');
            if (cleanWord.length > 3) {
                wordFreq[cleanWord] = (wordFreq[cleanWord] || 0) + 1;
            }
        });
        
        const sortedKeywords = Object.entries(wordFreq)
            .sort(([,a], [,b]) => b - a)
            .slice(0, 10);
        
        keywordList.innerHTML = sortedKeywords.length > 0 
            ? sortedKeywords.map(([word, count]) => `
                <div class="detail-item">
                    <span class="detail-label">${word}</span>
                    <span class="detail-value">${count}</span>
                </div>
            `).join('')
            : '<p>Недостаточно данных для анализа ключевых слов</p>';
        
        this.keywordModal.style.display = 'block';
    }

    showExportModal() {
        this.exportModal.style.display = 'block';
    }

    exportData(format) {
        const stats = this.calculateStats(this.textInput.value);
        let content, mimeType, filename;

        switch (format) {
            case 'json':
                content = JSON.stringify(stats, null, 2);
                mimeType = 'application/json';
                filename = 'text-analysis.json';
                break;
            case 'csv':
                content = this.convertToCSV(stats);
                mimeType = 'text/csv';
                filename = 'text-analysis.csv';
                break;
            case 'txt':
                content = this.convertToTXT(stats);
                mimeType = 'text/plain';
                filename = 'text-analysis.txt';
                break;
        }

        this.downloadFile(content, mimeType, filename);
        this.exportModal.style.display = 'none';
    }

    convertToCSV(stats) {
        const headers = ['Метрика', 'Значение'];
        const rows = [
            ['Количество символов', stats.charCount],
            ['Количество символов (без пробелов)', stats.charNoSpaces],
            ['Количество слов', stats.wordCount],
            ['Количество предложений', stats.sentenceCount],
            ['Количество абзацев', stats.paragraphCount],
            ['Средняя длина слова', stats.avgWordLength],
            ['Средняя длина предложения', stats.avgSentenceLength],
            ['Время чтения (мин)', stats.readingTime]
        ];
        
        return [headers, ...rows].map(row => row.join(',')).join('\n');
    }

    convertToTXT(stats) {
        return `АНАЛИЗ ТЕКСТА
================

Основные метрики:
• Символы: ${stats.charCount}
• Символы (без пробелов): ${stats.charNoSpaces}
• Слова: ${stats.wordCount}
• Предложения: ${stats.sentenceCount}
• Абзацы: ${stats.paragraphCount}

Дополнительные метрики:
• Средняя длина слова: ${stats.avgWordLength}
• Средняя длина предложения: ${stats.avgSentenceLength}
• Время чтения: ${stats.readingTime} мин

Сгенерировано: ${new Date().toLocaleString()}`;
    }

    downloadFile(content, mimeType, filename) {
        const blob = new Blob([content], { type: mimeType });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = filename;
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(url);
    }

    showHistory() {
        alert(`История анализа: ${this.analysisHistory.length} записей\n\nЭта функция находится в разработке.`);
    }

    showSettings() {
        alert('Настройки находятся в разработке. Скоро здесь можно будет настроить параметры анализа.');
    }

    saveToLocalStorage() {
        const data = {
            text: this.textInput.value,
            theme: document.body.getAttribute('data-theme') || 'light',
            history: this.analysisHistory.slice(-10) // Сохраняем последние 10 записей
        };
        localStorage.setItem('textAnalyzer', JSON.stringify(data));
    }

    loadFromLocalStorage() {
        try {
            const data = JSON.parse(localStorage.getItem('textAnalyzer'));
            if (data) {
                if (data.text) {
                    this.textInput.value = data.text;
                    this.updateStats();
                }
                if (data.history) {
                    this.analysisHistory = data.history;
                }
            }
        } catch (e) {
            console.log('Не удалось загрузить данные из localStorage');
        }
    }
}

// Инициализация при загрузке страницы
document.addEventListener('DOMContentLoaded', () => {
    new TextAnalyzer();
});

// Добавляем обработчик для Ctrl+A
document.addEventListener('keydown', (e) => {
    if (e.ctrlKey && e.key === 'a') {
        e.preventDefault();
        const textInput = document.getElementById('text-input');
        textInput.select();
    }
});