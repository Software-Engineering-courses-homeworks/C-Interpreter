// Get DOM elements
const codeInput = document.getElementById('codeInput');
const runBtn = document.getElementById('runBtn');
const compileBtn = document.getElementById('compileBtn');
const output = document.getElementById('output');
const themeToggle = document.getElementById('themeToggle');
const themeIcon = document.querySelector('.theme-icon');
const helpToggle = document.getElementById('helpToggle');
const helpModal = document.getElementById('helpModal');
const closeModal = document.getElementById('closeModal');

// Theme management
let isDarkMode = localStorage.getItem('darkMode') !== 'false';

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

// Help modal functionality
helpToggle.addEventListener('click', () => {
    helpModal.style.display = 'block';
});

closeModal.addEventListener('click', () => {
    helpModal.style.display = 'none';
});

// Close modal when clicking outside of it
window.addEventListener('click', (event) => {
    if (event.target === helpModal) {
        helpModal.style.display = 'none';
    }
});

// Close modal with Escape key
document.addEventListener('keydown', (event) => {
    if (event.key === 'Escape' && helpModal.style.display === 'block') {
        helpModal.style.display = 'none';
    }
});

// Function to display output
function displayOutput(text, isError = false) {
    output.textContent = text;
    output.style.color = isError ? 'var(--error-color)' : 'var(--text-color)';
}

// Function to handle code execution
async function handleExecution(mode) {
    const code = codeInput.value.trim();

    if (!code) {
        displayOutput('Please enter some code to ' + mode + '.', true);
        return;
    }

    // Disable buttons during execution
    runBtn.disabled = true;
    compileBtn.disabled = true;

    displayOutput('Processing...');

    try {
        // Prepare JSON payload
        const payload = {
            code: code,
            debug: mode === 'run_debug' // Use debug mode for compile button
        };

        // Send POST request to backend
        const backendUrl = process.env.BACKEND_URL || 'http://localhost:8000';
        const response = await fetch(`${backendUrl}`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(payload)
        });

        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }

        const result = await response.json();

        // Display the result
        if (result.success) {
            displayOutput(result.output || 'Code executed successfully.');
        } else {
            displayOutput(result.error || 'An error occurred during execution.', true);
        }

    } catch (error) {
        console.error('Error calling interpreter:', error);
        displayOutput(`Network error: ${error.message}`, true);
    } finally {
        // Re-enable buttons
        runBtn.disabled = false;
        compileBtn.disabled = false;
    }
}

// Event listeners
runBtn.addEventListener('click', () => handleExecution('run'));
compileBtn.addEventListener('click', () => handleExecution('run_debug'));

// Allow Ctrl+Enter to run code
codeInput.addEventListener('keydown', (e) => {
    if (e.ctrlKey && e.key === 'Enter') {
        e.preventDefault();
        handleExecution('run');
    }
});

// Drag and drop functionality
codeInput.addEventListener('dragover', (e) => {
    e.preventDefault();
    e.stopPropagation();
    codeInput.style.backgroundColor = 'var(--accent-color)';
    codeInput.style.opacity = '0.7';
});

codeInput.addEventListener('dragleave', (e) => {
    e.preventDefault();
    e.stopPropagation();
    codeInput.style.backgroundColor = 'var(--input-bg)';
    codeInput.style.opacity = '1';
});

codeInput.addEventListener('drop', (e) => {
    e.preventDefault();
    e.stopPropagation();
    
    // Reset styles
    codeInput.style.backgroundColor = 'var(--input-bg)';
    codeInput.style.opacity = '1';
    
    const files = e.dataTransfer.files;
    
    if (files.length > 0) {
        const file = files[0];
        
        // Check if it's a text file
        if (file.type === 'text/plain' || file.name.endsWith('.txt')) {
            const reader = new FileReader();
            
            reader.onload = (event) => {
                codeInput.value = event.target.result;
                displayOutput('File loaded successfully! You can now run the code.');
            };
            
            reader.onerror = () => {
                displayOutput('Error reading file. Please try again.', true);
            };
            
            reader.readAsText(file);
        } else {
            displayOutput('Please drop a text file (.txt).', true);
        }
    }
});

// Initial message
displayOutput('Enter your code above and click "Run Code" or "Run Debug" to see the results here.');
console.log("hello inspector!");