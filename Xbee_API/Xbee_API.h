#include <XBee.h>
#define DESLOCAMENTO_BIT 1
#define MULTI_VARIAVEL 2

XBee xbee = XBee();

ZBTxStatusResponse txStatus = ZBTxStatusResponse();
ZBRxResponse zbRX =ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

bool Ack_print;
void Xbee_begin(Stream &sSerial = Serial, bool flag = false);
void Xbee_send(uint8_t msg, XBeeAddress64 CLIENT_ADD64, bool Ack_statusresponse = false);
void Xbee_read(uint8_t val[], int num, bool Ack_Modem_Status = false);

void Xbee_begin(Stream &sSerial, bool flag) {
  Ack_print = flag;
  xbee.setSerial(sSerial);
}

void Xbee_send(uint8_t msg, XBeeAddress64 CLIENT_ADD64, bool Ack_statusresponse) {
  if (Ack_print) Serial.print("Enviando Pacote");
  uint8_t payload[] = {msg};

  XBeeAddress64 destination = XBeeAddress64(CLIENT_ADD64);
  ZBTxRequest zbTX = ZBTxRequest(destination, payload, sizeof(payload));
  xbee.send(zbTX);

  if (Ack_statusresponse) {
    if (xbee.readPacket(500)) {
      if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
        xbee.getResponse().getZBTxStatusResponse(txStatus);
        if (Ack_print) Serial.print("Mensagem enviada");

        if (txStatus.getDeliveryStatus() == SUCCESS)
          if (Ack_print) Serial.print("Aviso de recebimento ok");
        else if (Ack_print) Serial.print("Erro de aviso de recebimento");
      }
    } else if (xbee.getResponse().isError()) {
      if (Ack_print) Serial.print("Erro na leitura do pacote.  Código de erro: ");
      if (Ack_print) Serial.println(xbee.getResponse().getErrorCode());
    } else if (Ack_print) Serial.print("Mensagem não enviada");
  }
}

void Xbee_read(uint8_t val[], int num, bool Ack_Modem_Status) {
  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      xbee.getResponse().getZBRxResponse(zbRX);
      if (Ack_print) Serial.println("Mensagem recebida");

      if (zbRX.getOption() == ZB_PACKET_ACKNOWLEDGED) {
        if (Ack_print) Serial.println("Ack enviado");
        for (int i = 0; i < num; i++) val[i] = zbRX.getData(i);
      } else if (Ack_print) Serial.println("Erro de Ack");
    } else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE && Ack_Modem_Status) {
      xbee.getResponse().getModemStatusResponse(msr);
      if (msr.getStatus() == ASSOCIATED)
        if (Ack_print) Serial.println("Shield Xbee conectado");
      else if (msr.getStatus() == DISASSOCIATED)
        if (Ack_print) Serial.println("Shield Xbee desconectado");
    }
  } else if (xbee.getResponse().isError()) {
    if (Ack_print) Serial.print("Erro na leitura do pacote.  Código de erro: ");
    if (Ack_print) Serial.println(xbee.getResponse().getErrorCode());
  } else if (Ack_print) Serial.println("Sem dados");
}