#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- Definições e Constantes ---
#define pino_Leitor A0             // Pino onde o ponto entre os resistores está conectado
const float Vref = 5.0;            // Tensão de referência do Arduino (normalmente 5V)
const float R_conhecido = 1000.0;  // Valor do resistor conhecido em ohms (1kΩ)
const char* TABELA_CORES[] = { "Preto", "Marro.", "Verme.", "Laran.", "Amare.", "Verde", "Azul", "Viol.", "Cinza", "Bran." };

Adafruit_SSD1306 display(128, 64, &Wire, -1);  // Display OLED 128x64

// --- Protótipos ---
void mostrarResultado(float mediaVout, float resistencia);
void desenharResistor();
void imprimirSerial(float mediaVout, float resistencia);
void configTexto(int tamanho, int x, int y);

// --- Variáveis Globais ---
float mediaVout = 0;

// --- Setup ---
void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao inicializar o display SSD1306"));
    while (true)
      ;
  }
  display.setTextColor(WHITE);
  display.clearDisplay();
}

// --- Loop Principal ---
void loop() {
  display.clearDisplay();
  configTexto(2, 0, 0);

  // Faz a média de 100 leituras
  for (int i = 1; i < 100; i++) {
    int leituraADC = analogRead(pino_Leitor);
    float Vout = (leituraADC / 1023.0) * Vref;
    mediaVout = ((mediaVout + Vout) / 2);
    delay(10);

    if (mediaVout >= 4.995) {  // Analisa a presença de Resistor
      mediaVout = 0;
      display.println("Insira um Resistor");
      display.display();
      Serial.println("Erro: Vout igual ou maior que Vref");
      delay(500);
      return;
    }
  }

  float resistencia = R_conhecido * (mediaVout / (Vref - mediaVout));
  mostrarResultado(mediaVout, resistencia);

  display.display();  // Por final, Desenha a tela
  delay(100);         // Para evitar flicker e consumo excessivo
}

// --- Funções Auxiliares ---

void mostrarResultado(float mediaVout, float resistencia) {
  imprimirSerial(mediaVout, resistencia);

  // Mostra valor em KOhms ou Ohms
  if (resistencia > 1000) {
    float valorK = resistencia / 1000.0;
    display.println(String(valorK, 1) + " KOhms");
  } else {
    display.println(String(resistencia, 0) + " Ohms");
  }

  // Configura texto para as faixas do resistor
  configTexto(1, 1, 20);

  String valorStr = String((int)resistencia);
  int faixa1 = valorStr[0] - '0';                              // 1ª casa decimal
  int faixa2 = valorStr.length() > 1 ? valorStr[1] - '0' : 0;  // 2ª casa
  int multiplicador = valorStr.length() - 2;

  // Arredondamento simples
  if (valorStr.length() > 3 && (valorStr[2] - '0') >= 5) {
    faixa2 += 1;
    if (faixa2 >= 6) {
      faixa2 = 0;
      faixa1 += 1;
      if (faixa1 >= 6) {
        faixa1 = 1;
        multiplicador += 1;
      }
    }
  }

  desenharResistor();

  // Desenha as faixas e mostra as cores
  display.setCursor(1, 16);
  display.println(TABELA_CORES[faixa1]);

  display.setCursor(45, 55);
  display.println(TABELA_CORES[faixa2]);

  display.setCursor(78, 16);
  display.println(TABELA_CORES[multiplicador]);
}

void desenharResistor() {
  display.setTextSize(1);

  // Circuiferencias 
  display.fillCircle(25, 42, 14, WHITE);
  display.fillCircle(101, 42, 14, WHITE);

  // Corpo retangular
  display.fillRect(22, 33, 85, 19, WHITE);

  // Faixas pretas (onde serão as cores)
  display.fillRoundRect(37, 34, 8, 17, 2, BLACK);
  display.fillRoundRect(52, 34, 8, 17, 2, BLACK);
  display.fillRoundRect(67, 34, 8, 17, 2, BLACK);
  display.fillRoundRect(82, 34, 8, 17, 2, BLACK);

  // Linhas de orientação
  display.drawLine(41, 31, 41, 22, WHITE);
  display.drawLine(40, 22, 38, 22, WHITE);
  display.drawLine(56, 55, 56, 53, WHITE);
  display.drawLine(71, 31, 71, 22, WHITE);
  display.drawLine(72, 22, 74, 22, WHITE);
}

void imprimirSerial(float mediaVout, float resistencia) {
  Serial.print("(Vout): ");
  Serial.print(mediaVout, 3);
  Serial.print(" V\t");
  Serial.print("(Resistor): ");
  Serial.print(resistencia, 1);
  Serial.println(" ohms");
}

void configTexto(int tamanho, int x, int y) {
  display.setTextSize(tamanho);
  display.setCursor(x, y);
}
