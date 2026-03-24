# BME280_PROAGRO
Optimized library for the BME280 sensor, developed by Proagro Technology for agricultural dataloggers.
This implementation removes external dependencies (such as the SparkFun library) and uses direct I2C communication together with Bosch’s official compensation, ensuring precision, stability, and full control of the code.

✨ Features
- Direct implementation of the I2C (Wire) protocol
- Full compensation based on the Bosch datasheet
- No external dependencies
- Simple interface compatible with existing systems
- Optimized for agricultural dataloggers with periodic sampling
- Low memory usage (flash and RAM)

📦 Installation
Option 1: ZIP
- Download the repository as ZIP
- In Arduino IDE:
Sketch → Include Library → Add .ZIP Library…
Option 2: Clone
git clone https://github.com/jvasquez05/BME280_PROAGRO.git



🚀 Basic Usage
#include <Wire.h>
#include <BME280_PROAGRO.h>

BME280 bme;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!bme.beginI2C()) {   // Default address: 0x77
    Serial.println("Error initializing BME280");
    while (1);
  }
}

void loop() {
  float t = bme.readTempC();
  float h = bme.readFloatHumidity();
  float p = bme.readFloatPressure();

  Serial.print("T: "); Serial.println(t);
  Serial.print("H: "); Serial.println(h);
  Serial.print("P: "); Serial.println(p);

  delay(1000);
}



⚙️ Initialization
The library offers two initialization methods:
bme.beginI2C();        // Uses address 0x77 (default)  
bme.beginI2C(0x76);    // Allows specifying address  



📊 Available Functions
- readTempC() → Temperature in °C
- readFloatHumidity() → Relative humidity (%)
- readFloatPressure() → Pressure (Pa)

🔧 Internal Configuration
Sensor configuration is handled internally:
- Balanced oversampling (temperature, humidity, and pressure)
- Normal operation mode
- Internal filter enabled
These parameters were selected to:
- Maximize temporal stability
- Reduce noise
- Maintain consistency with external averaging systems

📖 Notes
- Final precision depends on the acquisition system (averages, filters, etc.)
- Validated against reference implementation (SparkFun)
- Differences observed within the expected sensor range

🏷️ Version
v1.0.0 – First official release.

📧 Author
Proagro Technology
Developed by Jhonny Vásquez Jiménez


