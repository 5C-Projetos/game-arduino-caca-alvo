#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte LINHAS = 4;
const byte COLUNAS = 3;
char teclas[LINHAS][COLUNAS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

// Mapeamento conforme a ligação da imagem
byte pinosLinhas[LINHAS] = {5, 6, 7, 8};
byte pinosColunas[COLUNAS] = {2, 3, 4};

Keypad teclado = Keypad(makeKeymap(teclas), pinosLinhas, pinosColunas, LINHAS, COLUNAS);

const char mapaTeclas[6] = {'1', '2', '3', '4', '5', '6'};

int alvoAtual = -1;
unsigned long tempoInicio = 0;
unsigned long tempoLimite = 2000;
int pontos = 0;
int vidas = 3;
bool jogando = true;

void desenharTabuleiro() {
  for (int i = 0; i < 6; i++) {
    int linha = (i < 3) ? 0 : 1;
    int coluna = (i % 3) * 2;
    lcd.setCursor(coluna, linha);
    lcd.print(i == alvoAtual ? '#' : '_');
  }
  
  lcd.setCursor(9, 0);
  lcd.print("Pts:");
  if (pontos < 100) lcd.print('0');
  if (pontos < 10) lcd.print('0');
  lcd.print(pontos);

  lcd.setCursor(9, 1);
  lcd.print("Vidas:");
  lcd.print(vidas);
}

void novoAlvo() {
  int proximo;
  do {
    proximo = random(0, 6);
  } while (proximo == alvoAtual);
  
  alvoAtual = proximo;
  desenharTabuleiro();
  tempoInicio = millis();
}

void iniciarJogo() {
  pontos = 0;
  vidas = 3;
  tempoLimite = 2000;
  jogando = true;
  lcd.clear();
  novoAlvo();
}

void perderVida() {
  vidas--;
  if (vidas <= 0) {
    jogando = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   GAME OVER!   ");
    lcd.setCursor(0, 1);
    lcd.print("Pts:");
    lcd.print(pontos);
    lcd.setCursor(11, 1);
    lcd.print("(#)  ");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("ERROU! ");
    delay(450);
    lcd.clear();
    novoAlvo();
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
  randomSeed(analogRead(A0));
  iniciarJogo();
}

void loop() {
  char tecla = teclado.getKey();

  if (!jogando) {
    if (tecla == '#') iniciarJogo();
    return;
  }

  if (millis() - tempoInicio >= tempoLimite) {
    perderVida();
    return;
  }

  if (tecla != NO_KEY) {
    if (tecla == mapaTeclas[alvoAtual]) {
      pontos++;
      if (tempoLimite > 550) {
        tempoLimite -= 65;
      }
      novoAlvo();
    } else {
      perderVida();
    }
  }
}
