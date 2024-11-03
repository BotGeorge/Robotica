#include <Arduino.h>
#include <string.h>

#define VITEZA_BAUD 28800

#define PIN_LED_ROSU 6
#define PIN_LED_VERDE 4
#define PIN_LED_ALBASTRU 5

#define BUTON_PORNIRE 3
#define BUTON_NIVEL 2

#define ASCII_BACKSPACE 8


unsigned long durateTastare[3] = {6000, 5000, 4000};
unsigned long durataJoc = 33000;
unsigned long momentCuvantAles = 0;
unsigned long durataDebounce = 300;
unsigned long durataPregatire = 3000; 
unsigned long eroareStart = 50;

char cuvantCurent[30], cuvantAles[30];


char listaCuvinte[10][30] = {"smecheria", "multi", "mitocondrie", "alfabet", "george", "este", "cel", "mai", "smecher", "bani"};

int pozitie = 0; 
int nivelDificultate = 0; 
int stareCurentaLed = 0;  
int numarClipiri = 0;
int punctaj = 0;

char denumireNiveluri[3][10] = {"\nUsor\n", "\nMediu\n", "\nGreu\n"};

volatile unsigned long momentApasareNivel = 0, momentApasarePornire = 0, inceputJoc = 0, ultimaClipire = 0;

bool pauza = true, inDesfasurare = false;

void seteazaLED(int rosu, int verde, int albastru) {
  analogWrite(PIN_LED_ROSU, rosu);
  analogWrite(PIN_LED_VERDE, verde);
  analogWrite(PIN_LED_ALBASTRU, albastru);
}

int verificaText(const char *textIntrat) {
  int lungimeText = strlen(textIntrat);
  for (int i = 0; i < lungimeText; i++) {
    if (textIntrat[i] != cuvantAles[i])
      return -1;
  }
  if (lungimeText == int(strlen(cuvantAles)))
    return 1;
  return 0;
}

void afiseazaPunctajFinal() {
  Serial.println("\nJoc terminat!\nPunctaj:");
  Serial.println(punctaj);
  Serial.println("\n");
  seteazaLED(255, 255, 255);
}

void verificaTimp() {
  if (inDesfasurare && millis() - inceputJoc > durataJoc) {
    pauza = true;
    inDesfasurare = false;
    afiseazaPunctajFinal();
  }
}

void initializareJoc() {
  inceputJoc = millis();
  punctaj = 0;
  ultimaClipire = millis();
  numarClipiri = 0;
}

void verificaButonNivel() {
  if (pauza) {
    if (millis() - momentApasareNivel > durataDebounce && digitalRead(BUTON_NIVEL) == LOW) {
      momentApasareNivel = millis();
      nivelDificultate = (nivelDificultate + 1) % 3;
      Serial.println(denumireNiveluri[nivelDificultate]);
    }
  }
}

void verificaButonPornire() {
  if (millis() - momentApasarePornire > durataDebounce && digitalRead(BUTON_PORNIRE) == LOW) {
    momentApasarePornire = millis();
    pauza = !pauza;
    inDesfasurare = !inDesfasurare;
    if (inDesfasurare) {
      initializareJoc();
    } else {
      afiseazaPunctajFinal();
    }
  }
}

void citesteCaracter() {
  char caracterCitit = Serial.read();
  if (int(caracterCitit) == ASCII_BACKSPACE) {
    if (pozitie > 0) {
      cuvantCurent[pozitie - 1] = NULL;
      pozitie--;
    }
  } else {
    cuvantCurent[pozitie] = caracterCitit;
    pozitie++;
    cuvantCurent[pozitie] = NULL;
  }
}

void alegeCuvantNou(int rezultat) {
  if (rezultat) {
    punctaj++;
    Serial.println("\n");
  } else {
    Serial.println("\nTimpul a expirat!\n");
  }
  strcpy(cuvantCurent, " ");
  pozitie = 0;
  strcpy(cuvantAles, listaCuvinte[(random(10) + inceputJoc) % 10]);
  Serial.println(cuvantAles);
  momentCuvantAles = millis();
}

void numaratoareInversa() {
  if (millis() - ultimaClipire > durataPregatire / 6) {
    ultimaClipire = millis();
    seteazaLED(255 * stareCurentaLed, 255 * stareCurentaLed, 255 * stareCurentaLed);
    stareCurentaLed = (stareCurentaLed + 1) % 2;

    if (!(numarClipiri % 2)) {
      Serial.println(char(3 - numarClipiri / 2 + 48));
    }

    numarClipiri++;
  }

  if (numarClipiri == 6) {
    pozitie = 0;
    strcpy(cuvantCurent, "");
    strcpy(cuvantAles, listaCuvinte[(random(10) + inceputJoc) % 10]);
    Serial.println(cuvantAles);
    momentCuvantAles = millis();
    seteazaLED(0, 255, 0);
    numarClipiri++;
  }
}

void setup() {
  Serial.begin(VITEZA_BAUD);

  pinMode(PIN_LED_ROSU, OUTPUT);
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_ALBASTRU, OUTPUT);
  seteazaLED(255, 255, 255);

  pinMode(BUTON_PORNIRE, INPUT_PULLUP);
  pinMode(BUTON_NIVEL, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTON_PORNIRE), verificaButonPornire, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTON_NIVEL), verificaButonNivel, FALLING);

  Serial.println("\nConfigurare completa\n");
}

void loop() {
  verificaTimp();

  if (inDesfasurare) {
    if (millis() - inceputJoc <= durataPregatire + eroareStart) {
      numaratoareInversa();
    } else {
      if (Serial.available()) {
        citesteCaracter();
        
        if (verificaText(cuvantCurent) == 1) {
          alegeCuvantNou(1);
        } else if (verificaText(cuvantCurent) == -1) {
          seteazaLED(255, 0, 0);
        } else {
          seteazaLED(0, 255, 0);
        }
      }
      if (millis() - momentCuvantAles > durateTastare[nivelDificultate]) {
        alegeCuvantNou(0);
      }
    }
  }
}
