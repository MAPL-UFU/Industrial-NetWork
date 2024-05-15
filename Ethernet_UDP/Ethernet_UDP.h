#include <SPI.h>
#include <Ethernet.h>
#include <Udp.h>
#define port 8888

EthernetUDP Udp;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

void Ethernet_begin(byte mac[], byte ip[]) {
  volatile bool flag = false;

  while (!Serial);

  Ethernet.begin(mac,ip);

  while (Ethernet.hardwareStatus() == EthernetNoHardware)
    if (!flag) {
      Serial.println("Controlador de Ethernet não encontrado.");
      flag = true;
    };

  if (Ethernet.hardwareStatus() == EthernetW5100)
    Serial.println("Controlador de Ethernet W5100 detectado.");
  else {
    if (Ethernet.hardwareStatus() == EthernetW5200)
      Serial.println("Controlador de Ethernet W5200 detectado.");
    else if (Ethernet.hardwareStatus() == EthernetW5500)
      Serial.println("Controlador de Ethernet W5500 detectado.");

    flag = false;

    while (Ethernet.linkStatus() == LinkOFF)
      if (!flag) {
        Serial.println("Status do Link = DESLIGADO");
        flag = true;
      };

    Serial.println("Status do Link = LIGADO");
  }

  Udp.begin(port);
}

void EthernetRX() {
  int packetSize = Udp.parsePacket();

  if (packetSize)
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
}

void EthernetTX(byte remoteip[]) {
  Udp.beginPacket(remoteip, port);
  Udp.write(packetBuffer);
  Udp.endPacket();
}