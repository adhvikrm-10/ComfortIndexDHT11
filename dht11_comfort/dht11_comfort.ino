#include <DHT.h>

#define DHT_PIN 4
#define DHT_TYPE DHT11

const unsigned long READ_INTERVAL_MS = 15000;

DHT dht(DHT_PIN, DHT_TYPE);

float minTemp = 999.0;
float maxTemp = -999.0;
float minHumidity = 999.0;
float maxHumidity = -999.0;

int readCount = 0;
int failCount = 0;

unsigned long sessionStart = 0;
unsigned long lastRead = 0;

struct ComfortResult {
  String label;
  String emoji;
  String advice;
};

ComfortResult classifyComfort(float heatIndex, float humidity) {
  if (heatIndex < 16.0) {
    return {"Too Cold", ".", "Consider warming up"};
  } else if (heatIndex < 24.0) {
    return {"Cool", ".", "Slightly cool but fine"};
  } else if (heatIndex < 32.0 && humidity < 70.0) {
    return {"Comfortable", ".", "Ideal conditions"};
  } else if (heatIndex < 32.0 && humidity >= 70.0) {
    return {"Humid", ".", "High humidity - stay hydrated"};
  } else if (heatIndex < 39.0) {
    return {"Hot", ".", "Stay hydrated, use fan"};
  } else if (heatIndex < 46.0) {
    return {"Very Hot", ".", "Limit activity, drink water"};
  } else {
    return {"Dangerously Hot", ".", "Risk of heat stroke!"};
  }
}

String humidityBar(float h) {
  int filled = (int)(h / 10.0);
  filled = constrain(filled, 0, 10);

  String bar = "[";

  for (int i = 0; i < 10; i++) {
    if (i < filled - 1)
      bar += "=";
    else if (i == filled - 1)
      bar += ">";
    else
      bar += " ";
  }

  bar += "] ";

  if ((int)h < 10)
    bar += "  ";
  else if ((int)h < 100)
    bar += " ";

  bar += String((int)h) + "%";

  return bar;
}

void divider(bool thick) {
  (void)thick;
}

void dividerClose() {
}

String uptime() {
  unsigned long s = (millis() - sessionStart) / 1000;
  char b[12];

  sprintf(b, "%02lu:%02lu:%02lu", s / 3600, (s % 3600) / 60, s % 60);

  return String(b);
}

void setup() {
  Serial.begin(115200);
  delay(500);

  dht.begin();
  sessionStart = millis();

  Serial.println();

  divider(true);

  Serial.println("DHT11 Comfort Index Display");
  Serial.println("ESP32 WROOM-DA");

  divider(false);

  Serial.printf(
    "Sensor: DHT%d on GPIO%d\n",
    DHT_TYPE,
    DHT_PIN
  );

  Serial.printf(
    "Interval: every %lus\n",
    READ_INTERVAL_MS / 1000
  );

  dividerClose();

  Serial.println();

  Serial.println("Warming up sensor - first read in 2 seconds...");

  delay(2000);
}

void loop() {
  unsigned long now = millis();

  if (now - lastRead < READ_INTERVAL_MS)
    return;

  lastRead = now;

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    failCount++;

    Serial.printf(
      "[WARN] Read failed (#%d) - retrying next cycle\n",
      failCount
    );

    if (failCount >= 3) {
      Serial.println(
        "[ERROR] 3 consecutive failures - check wiring on GPIO4!"
      );
      failCount = 0;
    }

    return;
  }

  failCount = 0;
  readCount++;

  float heatIndex = dht.computeHeatIndex(
    temperature,
    humidity,
    false
  );

  bool newMinTemp = false;
  bool newMaxTemp = false;
  bool newMinHum = false;
  bool newMaxHum = false;

  if (temperature < minTemp) {
    minTemp = temperature;
    newMinTemp = true;
  }

  if (temperature > maxTemp) {
    maxTemp = temperature;
    newMaxTemp = true;
  }

  if (humidity < minHumidity) {
    minHumidity = humidity;
    newMinHum = true;
  }

  if (humidity > maxHumidity) {
    maxHumidity = humidity;
    newMaxHum = true;
  }

  ComfortResult comfort = classifyComfort(
    heatIndex,
    humidity
  );

  Serial.println();

  Serial.println("+------------------------------------------------");

  Serial.printf(
    "| Read #%-4d | Uptime: %-18s\n",
    readCount,
    uptime().c_str()
  );

  Serial.println("+------------------------------------------------");

  Serial.printf(
    "| Temperature: %5.1f C",
    temperature
  );

  if (newMaxTemp && readCount > 1)
    Serial.print(" ^ NEW HIGH");
  else if (newMinTemp && readCount > 1)
    Serial.print(" v NEW LOW");
  else
    Serial.print("          ");

  Serial.println();

  Serial.printf(
    "| Feels like: %5.1f C\n",
    heatIndex
  );

  Serial.printf(
    "| Humidity: %-16s\n",
    humidityBar(humidity).c_str()
  );

  Serial.println("+------------------------------------------------");

  Serial.printf(
    "| Status: %-38s\n",
    comfort.label.c_str()
  );

  Serial.printf(
    "| Advice: %-38s\n",
    comfort.advice.c_str()
  );

  Serial.println("+------------------------------------------------");

  Serial.printf(
    "| Session min: %5.1f C max: %5.1f C\n",
    minTemp,
    maxTemp
  );

  Serial.printf(
    "| Humidity min: %5.1f %% max: %5.1f %%\n",
    minHumidity,
    maxHumidity
  );

  Serial.printf(
    "| Total reads: %-4d\n",
    readCount
  );

  Serial.println("+------------------------------------------------");

  unsigned long uptimeSec =
    (millis() - sessionStart) / 1000;

  Serial.printf(
    "DATA,%lu,%.1f,%.1f,%.1f,%s\n",
    uptimeSec,
    temperature,
    humidity,
    heatIndex,
    comfort.label.c_str()
  );

  if (heatIndex >= 45.0) {
    Serial.println();
    Serial.println("DANGER: Heat stroke risk detected !!");
  }
}
