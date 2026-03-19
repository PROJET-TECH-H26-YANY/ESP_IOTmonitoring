#include "DistanceManager.h"

DistanceManager::DistanceManager(int rxPin, int txPin, HardwareSerial &serialPort)
    : _serialPort(serialPort)
{
  _rxPin = rxPin;
  _txPin = txPin;
}

void DistanceManager::begin()
{
  _serialPort.begin(9600, SERIAL_8N1, _rxPin, _txPin);
  Serial.println("Ultrasonic Sensor Ready");
}

// Logique de calcule de la distance à partir des données reçues du capteur
// source : https://www.youtube.com/watch?v=h4zfCEbOtn8

float DistanceManager::readDistanceCm()
{
  float distance = -1.0;

  while (_serialPort.available() >= 4)
  {
    if (_serialPort.read() == 0xFF)
    {
      unsigned char data_high = _serialPort.read();
      unsigned char data_low = _serialPort.read();
      unsigned char checksum = _serialPort.read();

      int sum = (0xFF + data_high + data_low) & 0x00FF;

      if (sum == checksum)
      {
        distance = (data_high << 8) + data_low;
        distance = distance / 10.0f;
      }
    }
  }
  return distance;
}
