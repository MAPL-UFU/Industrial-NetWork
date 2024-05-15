/** Biblioteca de carregamento da comunicação I2C (Wire):
 *   .begin(): inicializa a biblioteca I2C (.begin() - sem endereçamento | .begin(0 - 127) - com endereçamento)
 *   .end(): finaliza a bibliioteca I2C
 *   .requestFrom(): requisita dados de um dipositivo endereçado (.requestFrom(address, quantity) - padrão stop falso |
 *     .requestFrom(address, quantity, stop) - com definição de stop || retorna o numero de bytes enviado pelo escravo)
 *   .beginTransmission(): inicializa a transmissão de dados entre o mestre e o escravo endereçado (beginTransmission(address))
 *   .endTransmission(): finaliza a transmissão de dados entre o mestre e o escravo (.endTransmission() | .endTransmission(stop) - libera a linha de transmissão)
 *   .write(): Escreve na linha de transmissão dados endereçados (.write(value) | .write(string)  | .write(data, length) || retorna o numero de bytes escritos)
 *   .available(): Retorna o número de bytes disponíveis
 *   .read(): Leitura dos dados enviados na linha de transmissão
 *   .setClock(): modifica a frequencia do clock para comunicação I2C (.setClock(clockFrequency))
 *   .onReceive(): registra uma função a ser chamada quando um escravo recebe uma transmissão de um mestre (.onReceive(handler))
 *   .onRequest(): registra uma função a ser chamada quando um mestre solicita dados de um escravo (.onReceive(handler))
 *   .setWireTimeout(): define o tempo limite para transmissões no modo mestre (.setWireTimeout(timeout, reset_on_timeout) |
 *     .setWireTimeout() - configura tempo limite padrão para transmissão)
 *   .clearWireTimeout(): limpa o sinalizador de tempo limite.
 *   .getWireTimeoutFlag(): verifica se ocorreu um tempo limite desde a última vez que o sinalizador foi apagado.
*/

#include <Wire.h>
#define DESLOCAMENTO_BIT 1
#define MULTI_VARIAVEL 2
#define MASTER 128
#define I2C_PACKET_MAX_SIZE 32

uint8_t leitura[I2C_PACKET_MAX_SIZE];

/** Definição de funções */
void I2C_begin(uint8_t slave = MASTER);
void slave_send();
void slave_read(int howMany);

/** Inicialização da comunicação I2C do tipo Mestre/Escravo */
void I2C_begin(uint8_t slave) {
  if (slave == MASTER)
    Wire.begin();
  else {
    Wire.begin(slave);
    Wire.onRequest(slave_send);
    Wire.onReceive(slave_read);
  }
}

/** Função de requisição de dados do escravo */
void requestfrom(uint8_t slave, uint8_t buffer[], uint8_t length){
  int num = Wire.requestFrom(slave, length);

  if(Wire.available()) {
    for (int i = 0; i < num; i++) buffer[i] = Wire.read();
  }
}

/** Função de envio de dados para o escravo */
void Transmission(uint8_t slave, uint8_t buffer[], int length) {
  Wire.beginTransmission(slave);
  Wire.write(buffer, length);
  Wire.endTransmission();
}

/** Função de envio de dados para o mestre */
void slave_send() {
  Wire.write(leitura, sizeof(leitura));
}

/** Função de leitura dos dados fornecidos pelo mestre */
void slave_read(int howMany) {
  int i = 0;

  while(Wire.available()) {
    leitura[i] = Wire.read();
    i++;
  }
}