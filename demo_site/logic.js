// Get DOM elements
const codeInput = document.getElementById('codeInput');
const runBtn = document.getElementById('runBtn');
const compileBtn = document.getElementById('compileBtn');
const output = document.getElementById('output');
const themeToggle = document.getElementById('themeToggle');
const themeIcon = document.querySelector('.theme-icon');

// Theme management
let isDarkMode = localStorage.getItem('darkMode') === 'true';

function updateTheme() {
    if (isDarkMode) {
        document.body.classList.add('dark-mode');
        themeIcon.textContent = '☀️';
    } else {
        document.body.classList.remove('dark-mode');
        themeIcon.textContent = '🌙';
    }
    localStorage.setItem('darkMode', isDarkMode);
}

// Initialize theme
updateTheme();

// Theme toggle event listener
themeToggle.addEventListener('click', () => {
    isDarkMode = !isDarkMode;
    updateTheme();
});

// Function to display output
function displayOutput(text, isError = false) {
    output.textContent = text;
    output.style.color = isError ? 'var(--error-color)' : 'var(--text-color)';
}

// Function to handle code execution
function handleExecution(mode) {
    const code = codeInput.value.trim();

    if (!code) {
        displayOutput('Please enter some code to ' + mode + '.', true);
        return;
    }

    // Disable buttons during execution
    runBtn.disabled = true;
    compileBtn.disabled = true;

    displayOutput('Processing...');

    // Simulate processing (replace this with actual interpreter call)
    setTimeout(() => {
        if (mode === 'run') {
            displayOutput(`Running code:\n${code}\n\n[This is a demo - integrate with your C interpreter here]`);
        } else {
            displayOutput(`Compiling code:\n${code}\n\n[This is a demo - integrate with your C compiler here]`);
        }

        // Re-enable buttons
        runBtn.disabled = false;
        compileBtn.disabled = false;
    }, 1000);
}

// Event listeners
runBtn.addEventListener('click', () => handleExecution('run'));
compileBtn.addEventListener('click', () => handleExecution('compile'));

// Allow Ctrl+Enter to run code
codeInput.addEventListener('keydown', (e) => {
    if (e.ctrlKey && e.key === 'Enter') {
        e.preventDefault();
        handleExecution('run');
    }
});

// Initial message
displayOutput('Enter your code above and click "Run Code" or "Run Debug" to see the results here.');