#include <LiquidCrystal_I2C.h>

#define DLIN  4         // número de linhas (display)
#define DCOL  20        // número de colunas (display)
#define ADDR  0x3F      // Endereço do display.

LiquidCrystal_I2C lcd(ADDR, DCOL, DLIN);

void initDisplay() {
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void limpar() {
  lcd.clear();
}

void displayChar(int col, int lin, char t) {
  lcd.setCursor(col, lin);
  lcd.print(t);
}

void displayString(int col, int lin, String str) {
  lcd.setCursor(col, lin);
  lcd.print(str);
}

void piscar(int rep, int tempo) {

  bool x = true;

  for(int a=0; a < rep; a++) {
    if(x) 
      lcd.backlight(); 
    else 
      lcd.noBacklight();

    x = !x;
    delay(tempo);
  }
  lcd.backlight(); 
}
