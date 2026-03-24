# BME280_PROAGRO

Librería optimizada para el sensor **BME280**, desarrollada por Proagro Technology para dataloggers agrícolas.

Esta implementación elimina dependencias externas (como la librería de SparkFun) y utiliza comunicación I2C directa junto con la compensación oficial de Bosch, garantizando **precisión, estabilidad y control total del código**.

---

## ✨ Características

- Implementación directa del protocolo I2C (Wire)
- Compensación completa basada en el datasheet de Bosch
- Sin dependencias externas
- Interfaz simple y compatible con sistemas existentes
- Optimizada para dataloggers agrícolas con muestreo periódico
- Bajo uso de memoria (flash y RAM)

---

## 📦 Instalación

### Opción 1: ZIP
1. Descargar el repositorio como ZIP
2. En Arduino IDE:  
   `Sketch → Include Library → Add .ZIP Library…`

### Opción 2: Clonar
```bash
git clone https://github.com/jvasquez05/BME280_PROAGRO.git


🚀 Uso básico
#include <Wire.h>
#include <BME280_PROAGRO.h>

BME280 bme;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!bme.beginI2C()) {   // Dirección por defecto: 0x77
    Serial.println("Error inicializando BME280");
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
⚙️ Inicialización

La librería ofrece dos formas de inicialización:
bme.beginI2C();        // Usa dirección 0x77 (por defecto)
bme.beginI2C(0x76);   // Permite especificar dirección

📊 Funciones disponibles
readTempC() → Temperatura en °C
readFloatHumidity() → Humedad relativa (%)
readFloatPressure() → Presión (Pa)

🔧 Configuración interna

La configuración del sensor se realiza internamente:

Oversampling balanceado (temperatura, humedad y presión)
Modo normal de operación
Filtro interno activado

Estos parámetros fueron seleccionados para:

Maximizar estabilidad temporal
Reducir ruido
Mantener coherencia con sistemas de promediado externos


📖 Notas
La precisión final depende del sistema de adquisición (promedios, filtros, etc.)
Validada contra implementación de referencia (SparkFun)
Diferencias observadas dentro del rango esperado del sensor

🏷️ Versión

v1.0.0 – Primera versión oficial.

📧 Autor

Proagro Technology
Desarrollada por Jhonny Vásquez Jiménez
