# DHT11 Comfort Index Display 

Reads temperature and humidity from a DHT11 sensor every 15 seconds, calculates heat index (feels like temperature), 
classifies comfort level into 7 zones, tracks session min/max, and displays a live formatted dashboard in Serial Monitor.

---

## What It Does

- Reads real temperature and humidity from DHT11 sensor
- Calculates heat index — what it actually feels like accounting for humidity
- Classifies into 7 comfort zones from Too Cold to Dangerously Hot
- Shows a live humidity bar visualiser
- Tracks session minimum and maximum temperature and humidity
- Flags new highs and lows as they occur
- Alerts on dangerous heat conditions

---

## Hardware Required

| Component | Details |
|-----------|---------|
| ESP32 WROOM-DA | Main board |
| DHT11 sensor module | 3-pin PCB module version |
| 3x jumper wires | Female-to-female |
| USB cable | Data cable (not charge-only) |

---

## Wiring

```
DHT11 Pin     →    ESP32 Pin
──────────────────────────────
VCC           →    3.3V
GND           →    GND
DATA (middle) →    GPIO 4
```

---

## Project Files

| File | Purpose |
|------|---------|
| `dht11_comfort.ino` | Arduino sketch — runs on ESP32 |

---

## Setup — Arduino IDE

### Step 1 — Install Libraries
- Open Arduino IDE
- Go to **Tools → Manage Libraries**
- Search `DHT sensor library` → install by **Adafruit** (v1.4+)
- Search `Adafruit Unified Sensor` → install latest version

### Step 2 — Wire the DHT11
Connect 3 female-to-female jumper wires as shown in the wiring table above.

### Step 3 — Open the sketch
- **File → Open** → select `dht11_comfort.ino`
- No configuration needed — GPIO4 is already set

### Step 4 — Select board and port
- **Tools → Board → ESP32 Arduino → ESP32 Dev Module**
- **Tools → Port → COM15** (or whichever port your ESP32 is on)

### Step 5 — Upload
- Click **→ Upload**
- Hold **BOOT button** when you see `Connecting......`
- Release BOOT when percentage passes 10%
- Wait for `Hard resetting via RTS pin...`

### Step 6 — Open Serial Monitor
- Press **Ctrl+Shift+M**
- Set baud rate to **115200**
- Press **EN button** on ESP32 to restart
- Wait 2 seconds for sensor warmup
- Dashboard prints every 15 seconds

---

## Setup — VS Code (with Arduino extension)

### Step 1 — Install VS Code
Download from https://code.visualstudio.com/

### Step 2 — Install Arduino Extension
- Open VS Code
- Press **Ctrl+Shift+X** to open Extensions panel
- Search `Arduino`
- Install **Arduino** by Microsoft
- Restart VS Code after install

### Step 3 — Install Arduino CLI (if prompted)
- VS Code will prompt to install Arduino CLI automatically
- Click **Install** and wait for it to finish
- Or install manually from https://arduino.github.io/arduino-cli/

### Step 4 — Configure Arduino extension
- Press **Ctrl+Shift+P** → type `Arduino: Board Manager`
- Search `esp32` → install **esp32 by Espressif Systems**
- This is the same board package as Arduino IDE

### Step 5 — Install DHT Libraries
- Press **Ctrl+Shift+P** → type `Arduino: Library Manager`
- Search `DHT sensor library` → install by Adafruit
- Search `Adafruit Unified Sensor` → install

### Step 6 — Open the sketch
- **File → Open Folder** → open the folder containing `dht11_comfort.ino`
- VS Code will recognise the `.ino` file automatically

### Step 7 — Select board and port
At the bottom blue status bar of VS Code:
- Click `<Select Board>` → type `ESP32 Dev Module` → select it
- Click `<Select Serial Port>` → select **COM14**

### Step 8 — Upload
- Press **Ctrl+Shift+P** → type `Arduino: Upload`
- Or click the **→ Upload** button in the top right of the editor
- Hold **BOOT button** on ESP32 when you see `Connecting......`
- Release when percentage passes 10%

### Step 9 — Open Serial Monitor in VS Code
- Press **Ctrl+Shift+M** → type `Arduino: Open Serial Monitor`
- Set baud rate to **115200**
- Press **EN button** on ESP32
- Dashboard appears every 15 seconds

---

## Comfort Zones

| Zone | Heat Index | Humidity | Advice |
|------|-----------|----------|--------|
| Too Cold | < 16°C | any | Consider warming up |
| Cool | 16–24°C | any | Slightly cool but fine |
| Comfortable | 24–32°C | < 70% | Ideal conditions |
| Humid | 24–32°C | ≥ 70% | High humidity — stay hydrated |
| Hot | 32–39°C | any | Stay hydrated, use fan |
| Very Hot | 39–45°C | any | Limit activity, drink water |
| Dangerously Hot | > 45°C | any | Risk of heat stroke! |

---

## Troubleshooting

| Symptom | Cause | Fix |
|---------|-------|-----|
| `Read failed` warnings | Bad wiring or loose connection | Check DATA wire on GPIO4 |
| Temperature reads 0°C | Wrong library version | Use Adafruit DHT v1.4+, NOT v2.x |
| Arduino extension not finding board | ESP32 package not installed | Run Arduino: Board Manager → install esp32 by Espressif |
| Upload fails in VS Code | Wrong port selected | Check Device Manager for correct COM port |
| Serial Monitor garbled | Wrong baud rate | Set exactly 115200 |
| `Adafruit_Sensor.h` not found | Missing dependency | Install Adafruit Unified Sensor library |

---

## Comfort Zones for Chennai

Chennai typically sits in these zones:
- **Morning** (6–9 AM) → Humid to Hot (28–32°C, 75–85% RH)
- **Afternoon** (12–3 PM) → Very Hot (36–42°C feels like)
- **Evening** (6–9 PM) → Hot (32–36°C feels like)
- **Night** (10 PM–5 AM) → Hot to Humid (28–30°C)

---

## Built With

- [Arduino IDE 2.x](https://www.arduino.cc/en/software) or 
  [VS Code + Arduino Extension](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino)
- [DHT sensor library by Adafruit](https://github.com/adafruit/DHT-sensor-library)
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Unified_Sensor)
