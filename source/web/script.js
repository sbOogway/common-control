let currentTargetTemperature = null;

async function fetchText(url) {
    const response = await fetch(url);
    if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
    }
    return response.text();
}

async function fetchTargetTemperature() {
    try {
        const data = await fetchText('/cgi-bin/get-target-temperature.sh');
        const targetTemp = parseFloat(data.trim());
        if (isNaN(targetTemp)) {
            throw new Error('Invalid target temperature data');
        }
        currentTargetTemperature = targetTemp;
        document.getElementById('target-temp').textContent = `${targetTemp.toFixed(1)}°C`;
        return targetTemp;
    } catch (error) {
        console.error("Failed to fetch target temperature:", error);
        document.getElementById('target-temp').textContent = '--';
        throw error;
    }
}

async function fetchTemperatureData() {
    try {
        const data = await fetchText('/cgi-bin/get-current-temperature.sh');
        const jsonData = JSON.parse(data.trim());
        const temperatures = Object.values(jsonData).map(t => parseFloat(t));
        const avgTemperature = temperatures.reduce((a, b) => a + b, 0) / temperatures.length;
        if (isNaN(avgTemperature)) {
            throw new Error('Invalid temperature data');
        }
        await fetchTargetTemperature();
        updateSensorsDisplay(jsonData);
        updateTemperatureDisplay(avgTemperature);
        updateTemperatureChart(avgTemperature);
        updateLastUpdate();
    } catch (error) {
        console.error("Failed to fetch temperature:", error);
        document.getElementById('current-temp').textContent = '--';
        document.getElementById('temp-status').textContent = 'Error';
    }
}

async function adjustTargetTemperature(delta) {
    try {
        let currentTemp = currentTargetTemperature;
        if (currentTemp === null) {
            currentTemp = await fetchTargetTemperature();
        }
        const newTemp = currentTemp + delta;
        console.debug(`Adjusting target temperature from ${currentTemp} to ${newTemp}`);
        await fetch('/cgi-bin/set-target-temperature.sh', {
            method: 'POST',
            body: newTemp.toFixed(1)
        });
        currentTargetTemperature = newTemp;
        document.getElementById('target-temp').textContent = `${newTemp.toFixed(1)}°C`;
        updateLastUpdate();
    } catch (error) {
        console.error("Failed to adjust target temperature:", error);
    }
}

function updateLastUpdate() {
    const now = new Date();
    document.getElementById('last-update').textContent = now.toLocaleTimeString();
}

function updateSensorsDisplay(sensors) {
    const sensorsList = document.getElementById('sensors-list');
    sensorsList.innerHTML = '';

    Object.entries(sensors).forEach(([sensorName, temp]) => {
        const sensorItem = document.createElement('div');
        sensorItem.className = 'sensor-item';

        const nameElement = document.createElement('span');
        nameElement.className = 'sensor-name';
        nameElement.textContent = sensorName.toUpperCase();

        const valueElement = document.createElement('span');
        valueElement.className = 'sensor-value';
        valueElement.textContent = `${parseFloat(temp).toFixed(1)}°C`;

        sensorItem.appendChild(nameElement);
        sensorItem.appendChild(valueElement);
        sensorsList.appendChild(sensorItem);
    });
}

function updateTemperatureDisplay(temperature) {
    const tempElement = document.getElementById('current-temp');
    const statusElement = document.getElementById('temp-status');

    tempElement.textContent = `${temperature.toFixed(1)}°C`;

    statusElement.className = 'temp-status';

    if (temperature > 80) {
        statusElement.classList.add('status-critical');
        statusElement.textContent = 'CRITICAL';
    } else if (temperature > 60) {
        statusElement.classList.add('status-warning');
        statusElement.textContent = 'WARNING';
    } else {
        statusElement.classList.add('status-normal');
        statusElement.textContent = 'NORMAL';
    }
}

let temperatureHistory = [];
const maxHistoryLength = 20;

function updateTemperatureChart(temperature) {
    temperatureHistory.push(temperature);

    if (temperatureHistory.length > maxHistoryLength) {
        temperatureHistory.shift();
    }

    drawChart();
}

function drawChart() {
    const canvas = document.getElementById('temperature-chart');
    const ctx = canvas.getContext('2d');
    const rect = canvas.getBoundingClientRect();

    canvas.width = rect.width;
    canvas.height = rect.height;

    ctx.clearRect(0, 0, canvas.width, canvas.height);

    if (temperatureHistory.length < 2) {
        ctx.fillStyle = '#666';
        ctx.font = '14px sans-serif';
        ctx.textAlign = 'center';
        ctx.fillText('Waiting for data...', canvas.width / 2, canvas.height / 2);
        return;
    }

    const padding = 60;
    const chartWidth = canvas.width - padding * 2;
    const chartHeight = canvas.height - padding * 2;

    const maxTemp = Math.max(...temperatureHistory);
    const minTemp = Math.min(...temperatureHistory);
    const tempRange = maxTemp - minTemp || 1;

    ctx.strokeStyle = '#007bff';
    ctx.lineWidth = 2;
    ctx.beginPath();

    temperatureHistory.forEach((temp, index) => {
        const x = padding + (index / (temperatureHistory.length - 1)) * chartWidth;
        const y = canvas.height - padding - ((temp - minTemp) / tempRange) * chartHeight;

        if (index === 0) {
            ctx.moveTo(x, y);
        } else {
            ctx.lineTo(x, y);
        }
    });

    ctx.stroke();

    ctx.fillStyle = '#007bff';
    temperatureHistory.forEach((temp, index) => {
        const x = padding + (index / (temperatureHistory.length - 1)) * chartWidth;
        const y = canvas.height - padding - ((temp - minTemp) / tempRange) * chartHeight;

        ctx.beginPath();
        ctx.arc(x, y, 4, 0, Math.PI * 2);
        ctx.fill();
    });

    ctx.fillStyle = '#333';
    ctx.font = '12px sans-serif';
    ctx.textAlign = 'right';
    ctx.fillText(`${maxTemp.toFixed(1)}°C`, padding - 5, padding);
    ctx.fillText(`${minTemp.toFixed(1)}°C`, padding - 5, canvas.height - padding);
}

let temperatureInterval;

function startTemperatureMonitoring() {
    fetchTemperatureData();
    temperatureInterval = setInterval(fetchTemperatureData, 5000);
}

function stopTemperatureMonitoring() {
    if (temperatureInterval) {
        clearInterval(temperatureInterval);
    }
}

window.onload = function () {
    document.getElementById('temp-down').addEventListener('click', () => adjustTargetTemperature(-1));
    document.getElementById('temp-up').addEventListener('click', () => adjustTargetTemperature(1));
    startTemperatureMonitoring();
};

window.onresize = function () {
    drawChart();
};