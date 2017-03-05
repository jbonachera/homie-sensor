#include <Homie.h>
#include <DHT.h>


#define DHTPIN 13
#define DHTTYPE DHT22

const int TEMPERATURE_INTERVAL = 25;

unsigned long lastTemperatureSent = 0;

HomieSetting<const char*> roomSetting("room", "The room this sensor will monitor");  // id, description

HomieNode temperatureNode("temperature", "temperature");
HomieNode humidityNode("humidity", "humidity");
HomieNode heatIndexNode("heatindex", "temperature");
DHT dht(DHTPIN, DHTTYPE);


void setupHandler() {
  dht.begin();
  temperatureNode.setProperty("unit").send("c");
  temperatureNode.setProperty("room").send(roomSetting.get());
  heatIndexNode.setProperty("unit").send("c");
  heatIndexNode.setProperty("room").send(roomSetting.get());
  humidityNode.setProperty("unit").send("%");
  humidityNode.setProperty("room").send(roomSetting.get());

}

void temperatureLoopHander(){
  if (millis() - lastTemperatureSent >= TEMPERATURE_INTERVAL * 1000UL || lastTemperatureSent == 0) {
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();

      lastTemperatureSent = millis();
      if (isnan(temperature) || isnan(humidity)) {
        Homie.getLogger() << "Failed to read from DHT sensor!"<< endl;;
      } else {
        Homie.getLogger() << "Temperature: " << temperature << " °C - ";
        temperatureNode.setProperty("degrees").send(String(temperature));

        Homie.getLogger() << "Humidity: " << humidity << " % - ";
        humidityNode.setProperty("percent").send(String(humidity));

        Homie.getLogger() << "heatIndex: " << dht.computeHeatIndex(temperature, humidity, false) << " °C" << endl;
        heatIndexNode.setProperty("degrees").send(String(dht.computeHeatIndex(temperature, humidity, false)));
      }
      Homie.getLogger() << "Powering OFF DHT sensor" << endl;
  }
}

void loopHandler() {
  temperatureLoopHander();
}
#ifndef UNIT_TEST

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware("temperature-sensor", "1.0.6");
  Homie_setBrand("vx-labs");
  temperatureNode.advertise("unit");
  temperatureNode.advertise("room");
  temperatureNode.advertise("degrees");
  heatIndexNode.advertise("unit");
  heatIndexNode.advertise("room");
  heatIndexNode.advertise("degrees");
  humidityNode.advertise("unit");
  humidityNode.advertise("room");
  humidityNode.advertise("percent");
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  roomSetting.setDefaultValue("NoRoom").setValidator([] (const char* candidate) {
    return true;
  });

  Homie.disableLedFeedback();
  Homie.setup();
}

void loop() {
  Homie.loop();

  // Used to limit self-heating.. we will see if it's efficient.
  delay(200);
}
#endif
