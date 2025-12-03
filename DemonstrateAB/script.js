class MathEditor {
    constructor() {
        this.currentExpression = '';
        this.isRendered = false;
        this.init();
    }

    init() {
        this.setupEventListeners();
        this.loadInitialExample();
    }

    setupEventListeners() {
        // Кнопки
        document.getElementById('renderBtn').addEventListener('click', () => this.renderMath());
        document.getElementById('convertBtn').addEventListener('click', () => this.convertAndRender());
        document.getElementById('downloadBtn').addEventListener('click', () => this.downloadImage());
        
        // Поле ввода
        const mathInput = document.getElementById('mathInput');
        mathInput.addEventListener('keydown', (e) => {
            if (e.key === 'Enter' && !e.shiftKey) {
                e.preventDefault();
                this.renderMath();
            }
        });

        // Примеры выражений
        document.querySelectorAll('.example-item').forEach(item => {
            item.addEventListener('click', () => {
                mathInput.value = item.getAttribute('data-expr');
                this.renderMath();
            });
        });

        // Модальное окно
        const modal = document.getElementById('docsModal');
        const openBtn = document.getElementById('openDocs');
        const closeBtn = document.querySelector('.close-modal');

        openBtn.addEventListener('click', () => {
            modal.style.display = 'flex';
        });

        closeBtn.addEventListener('click', () => {
            modal.style.display = 'none';
        });

        window.addEventListener('click', (event) => {
            if (event.target === modal) {
                modal.style.display = 'none';
            }
        });
    }

    loadInitialExample() {
        document.getElementById('mathInput').value = '\\frac{x+y}{b+n} \\cdot \\frac{a+b}{c+d}';
        this.renderMath();
    }

    renderMath() {
        const input = document.getElementById('mathInput').value;
        const container = document.getElementById('mathContainer');
        
        if (!input.trim()) {
            container.innerHTML = '<p style="color: #999;">Введите выражение для преобразования</p>';
            this.isRendered = false;
            return;
        }
        
        this.currentExpression = input;
        this.isRendered = false;
        
        try {
            const expressions = input.split('\n').filter(expr => expr.trim() !== '');
            
            if (expressions.length === 0) {
                container.innerHTML = '<p style="color: #999;">Введите выражение для преобразования</p>';
                this.isRendered = false;
                return;
            }
            
            let htmlContent = '';
            expressions.forEach(expr => {
                if (expr.trim()) {
                    htmlContent += `<div class="math-expression">\\[${expr}\\]</div>`;
                }
            });
            
            container.innerHTML = htmlContent;
            
            MathJax.typesetPromise([container]).then(() => {
                console.log('MathJax rendering completed');
                this.isRendered = true;
            }).catch((err) => {
                console.error('MathJax error:', err);
                container.innerHTML = `<p style="color: red;">Ошибка рендеринга: ${err.message}</p>`;
                this.isRendered = false;
            });
            
        } catch (error) {
            container.innerHTML = `<p style="color: red;">Ошибка: ${error.message}</p>`;
            this.isRendered = false;
        }
    }

    convertAndRender() {
        const input = document.getElementById('mathInput').value;
        const container = document.getElementById('mathContainer');
        
        if (!input.trim()) {
            container.innerHTML = '<p style="color: #999;">Введите выражение для преобразования</p>';
            this.isRendered = false;
            return;
        }
        
        this.currentExpression = input;
        this.isRendered = false;
        
        try {
            const expressions = input.split('\n').filter(expr => expr.trim() !== '');
            
            if (expressions.length === 0) {
                container.innerHTML = '<p style="color: #999;">Введите выражение для преобразования</p>';
                this.isRendered = false;
                return;
            }
            
            let htmlContent = '';
            expressions.forEach(expr => {
                if (expr.trim()) {
                    const latexExpr = this.convertToLatex(expr);
                    htmlContent += `<div class="math-expression">\\[${latexExpr}\\]</div>`;
                }
            });
            
            container.innerHTML = htmlContent;
            
            MathJax.typesetPromise([container]).then(() => {
                console.log('MathJax rendering completed');
                this.isRendered = true;
            }).catch((err) => {
                console.error('MathJax error:', err);
                container.innerHTML = `<p style="color: red;">Ошибка рендеринга: ${err.message}</p>`;
                this.isRendered = false;
            });
            
        } catch (error) {
            container.innerHTML = `<p style="color: red;">Ошибка: ${error.message}</p>`;
            this.isRendered = false;
        }
    }

    convertToLatex(expr) {
        expr = expr.replace(/\s/g, '');
        expr = expr.replace(/\*/g, '\\cdot ');
        expr = expr.replace(/\(([^()]+)\)\/\(([^()]+)\)/g, '\\frac{$1}{$2}');
        expr = expr.replace(/([a-zA-Z0-9]+)\/([a-zA-Z0-9]+)/g, '\\frac{$1}{$2}');
        expr = expr.replace(/(\w+)\^(\d+)/g, '{$1}^{$2}');
        expr = expr.replace(/(\w+)_(\d+)/g, '{$1}_{$2}');
        return expr;
    }

    downloadImage() {
        if (!this.currentExpression) {
            alert('Сначала введите выражение!');
            return;
        }
        
        if (!this.isRendered) {
            alert('Подождите, выражение еще преобразуется...');
            return;
        }
        
        const mathContainer = document.getElementById('mathContainer');
        
        // Проверяем наличие отрендеренных элементов MathJax
        const mathElements = mathContainer.querySelectorAll('.mjx-chtml, .MathJax, mjx-container');
        
        if (!mathElements || mathElements.length === 0) {
            // Если MathJax элементы не найдены, проверяем есть ли обычный текст
            const hasContent = mathContainer.textContent && mathContainer.textContent.trim() !== '' && 
                              !mathContainer.textContent.includes('Введите выражение');
            
            if (!hasContent) {
                alert('Сначала преобразуйте выражение!');
                return;
            }
            
            // Если есть обычный текст, но нет MathJax, рендерим как есть
            this.renderContainerAsImage(mathContainer);
            return;
        }
        
        this.renderMathJaxAsImage(mathElements);
    }

    renderMathJaxAsImage(mathElements) {
    const tempContainer = document.createElement('div');
    tempContainer.style.position = 'absolute';
    tempContainer.style.left = '-9999px';
    tempContainer.style.top = '0';
    tempContainer.style.background = 'white';
    tempContainer.style.padding = '40px';
    tempContainer.style.borderRadius = '10px';
    tempContainer.style.boxShadow = '0 10px 30px rgba(0,0,0,0.2)';
    tempContainer.style.display = 'flex';
    tempContainer.style.flexDirection = 'column';
    tempContainer.style.alignItems = 'center';
    tempContainer.style.justifyContent = 'center';
    tempContainer.style.gap = '20px';
    
    // Создаем чистые SVG из MathJax
    mathElements.forEach(element => {
        const svgElement = element.querySelector('svg');
        if (svgElement) {
            // Создаем новый чистый контейнер только с SVG
            const cleanContainer = document.createElement('div');
            cleanContainer.style.display = 'flex';
            cleanContainer.style.justifyContent = 'center';
            cleanContainer.style.alignItems = 'center';
            
            const svgClone = svgElement.cloneNode(true);
            cleanContainer.appendChild(svgClone);
            tempContainer.appendChild(cleanContainer);
        }
    });
    
    // Если нет SVG элементов, используем обычный подход
    if (tempContainer.children.length === 0) {
        mathElements.forEach(element => {
            const clone = element.cloneNode(true);
            tempContainer.appendChild(clone);
        });
    }
    
    document.body.appendChild(tempContainer);
    
    html2canvas(tempContainer, {
        scale: 2,
        backgroundColor: '#ffffff',
        useCORS: true,
        logging: false,
        ignoreElements: (element) => {
            // Игнорируем скрытые элементы
            return element.getAttribute('aria-hidden') === 'true' || 
                   element.style.display === 'none' ||
                   element.style.visibility === 'hidden';
        }
    }).then(canvas => {
        const link = document.createElement('a');
        link.download = `math_expression_${Date.now()}.png`;
        link.href = canvas.toDataURL('image/png', 1.0);
        link.click();
        
        document.body.removeChild(tempContainer);
    }).catch(error => {
        console.error('Error generating image:', error);
        alert('Ошибка при создании изображения: ' + error.message);
        document.body.removeChild(tempContainer);
    });
}

    renderContainerAsImage(container) {
        const tempContainer = document.createElement('div');
        tempContainer.style.position = 'absolute';
        tempContainer.style.left = '-9999px';
        tempContainer.style.top = '0';
        tempContainer.style.background = 'white';
        tempContainer.style.padding = '40px';
        tempContainer.style.borderRadius = '10px';
        tempContainer.style.boxShadow = '0 10px 30px rgba(0,0,0,0.2)';
        tempContainer.style.fontSize = '24px';
        tempContainer.style.fontFamily = "'Segoe UI', Tahoma, Geneva, Verdana, sans-serif";
        tempContainer.style.color = '#333';
        
        const clone = container.cloneNode(true);
        tempContainer.appendChild(clone);
        
        document.body.appendChild(tempContainer);
        
        html2canvas(tempContainer, {
            scale: 2,
            backgroundColor: '#ffffff'
        }).then(canvas => {
            const link = document.createElement('a');
            link.download = `math_expression_${Date.now()}.png`;
            link.href = canvas.toDataURL();
            link.click();
            
            document.body.removeChild(tempContainer);
        }).catch(error => {
            console.error('Error generating image:', error);
            alert('Ошибка при создании изображения: ' + error.message);
            document.body.removeChild(tempContainer);
        });
    }
}

// Инициализация при загрузке страницы
document.addEventListener('DOMContentLoaded', () => {
    new MathEditor();
});