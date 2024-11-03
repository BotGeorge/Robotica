#include <Arduino.h>

// Definirea pinilor pentru butoane si LED-uri
#define PIN_BUTON_OPRIRE 2
#define PIN_BUTON_INCEPUT 3
#define PIN_LED_RGB_VERDE 4
#define PIN_LED_RGB_ALBASTRU 5
#define PIN_LED_RGB_ROSU 6
#define PIN_100 7
#define PIN_75 8
#define PIN_50 9
#define PIN_25 10

// Definirea constantelor pentru timpi
#define TIMP_INCARCARE 12000
#define TIMP_OPRIRE_FORCE 1000

// Variabile pentru starea butoanelor
int stareButonInceput = HIGH;
int ultimaStareButonInceput = HIGH;

int stareButonOprire = HIGH;
long timestampStareConstantaOprire = 0;
int ultimaStareButonOprire = HIGH;

// Variabile pentru debounce
unsigned long ultimTimpDebounceInceput = 0;
unsigned long ultimTimpDebounceOprire = 0;
unsigned long int delayDebounce = 50;

// Variabile pentru gestionarea incarcarii
long startTimpIncarcare = 0;
int perioadaBlinking = 600; // Perioada de blinking a LED-urilor

// Variabile pentru starea LED-urilor
int blinkLed25 = false;
int UltimaStareBlink25 = false;
int ultimBlink25 = 0;

int blinkingLed50 = false;
int ultimaStareBlink50 = false;
int ultimBlink50 = 0;

int blinkingLed75 = false;
int ultimaStareBlink75 = false;
int ultimBlink75 = 0;

int blinkingLed100 = false;
int ultimaStareBlink100 = false;
int ultimBlink100 = 0;

// Variabila pentru starea incarcarii
int incarcare = false;

// Functia pentru a incepe incarcarea
void IncepeIncarcarea() {
    digitalWrite(PIN_LED_RGB_VERDE, LOW); // LED verde OFF
    digitalWrite(PIN_LED_RGB_ROSU, HIGH); // LED rosu ON
    startTimpIncarcare = millis(); // Salveaza timpul de inceput al incarcarii
    blinkLed25 = true; // Activeaza blinking pentru LED 25%
    incarcare = true; // Marcheaza ca incarcarea a inceput
    Serial.print(millis()); // Afiseaza timpul curent
    Serial.print("\n");
}

// Functia pentru a opri incarcarea
void OpresteIncarcarea() {
    blinkLed25 = false; // Deactiveaza blinking pentru LED 25%
    blinkingLed50 = false; 
    blinkingLed75 = false; 
    blinkingLed100 = false; 
    
    // Opreste toate LED-urile
    digitalWrite(PIN_25, LOW);
    digitalWrite(PIN_50, LOW);
    digitalWrite(PIN_75, LOW);
    digitalWrite(PIN_100, LOW);
    
    digitalWrite(PIN_LED_RGB_ROSU, LOW); // LED rosu OFF
    digitalWrite(PIN_LED_RGB_VERDE, HIGH); // LED verde ON
    incarcare = false; // Marcheaza ca incarcarea s-a oprit
}

// Functia pentru a opri incarcarea fortata
void OpresteIncarcareaFortata() {
    startTimpIncarcare = (int)millis() - TIMP_INCARCARE; // Seteaza timpul de incarcarea fortata
}

void setup() {
    Serial.begin(9600); // Initializeaza comunicatia seriala
    pinMode(PIN_BUTON_OPRIRE, INPUT_PULLUP); // Configureaza butonul de oprire ca input cu pull-up
    pinMode(PIN_BUTON_INCEPUT, INPUT_PULLUP); // Configureaza butonul de inceput ca input cu pull-up
    
    // Configurarea pinilor LED-urilor ca iesiri
    for (int i = 4; i <= 10; ++i) 
        pinMode(i, OUTPUT);

    digitalWrite(PIN_LED_RGB_VERDE, HIGH); // LED verde pornit la inceput
}

void loop() {
    // Citeste starea butoanelor
    int RButonInceput = digitalRead(PIN_BUTON_INCEPUT);
    int RButonOprire = digitalRead(PIN_BUTON_OPRIRE);

    // Verifica schimbarea starii butonului de inceput
    if (RButonInceput != ultimaStareButonInceput) 
        ultimTimpDebounceInceput = millis(); // Actualizeaza timpul de debounce

    // Verifica schimbarea starii butonului de oprire
    if (RButonOprire != ultimaStareButonOprire)
        ultimTimpDebounceOprire = millis();

    // Verifica butonul de inceput dupa debounce
    if ((millis() - ultimTimpDebounceInceput) > delayDebounce) {
        if (RButonInceput != stareButonInceput) {
            stareButonInceput = RButonInceput; // Actualizeaza starea butonului

            // Daca butonul este apasat si nu este in stare de incarcare
            if (stareButonInceput == LOW && !incarcare) 
                IncepeIncarcarea(); // Incepe incarcarea
        }
    }

    // Verifica butonul de oprire dupa debounce
    if ((millis() - ultimTimpDebounceOprire) > delayDebounce) { 
        if (RButonOprire != stareButonOprire) {
            stareButonOprire = RButonOprire; // Actualizeaza starea butonului de oprire
            // Verifica daca butonul de oprire a fost apasat
            if (RButonOprire == LOW && ultimaStareButonOprire == HIGH)
                timestampStareConstantaOprire = millis(); // Salveaza timpul cand butonul a fost apasat
        } else if (incarcare && stareButonOprire == LOW && millis() - timestampStareConstantaOprire >= TIMP_OPRIRE_FORCE) {
            OpresteIncarcareaFortata(); // Opreste incarcarea fortata
            timestampStareConstantaOprire = millis(); // Reseteaza timestamp
        }
    }

    // Verifica daca a trecut timpul de incarcare
    if (millis() - startTimpIncarcare > TIMP_INCARCARE) {
        // Daca incarcarea s-a terminat dar este in perioada de notificare
        if (millis() - startTimpIncarcare <= TIMP_INCARCARE + 4000) {
            blinkLed25 = true; 
            blinkingLed50 = true; 
            blinkingLed75 = true; 
            blinkingLed100 = true; 
            if (!UltimaStareBlink25) {
                // Aprinde toate LED-urile
                digitalWrite(PIN_100, HIGH);
                digitalWrite(PIN_75, HIGH);
                digitalWrite(PIN_50, HIGH);
                digitalWrite(PIN_25, HIGH);
                ultimBlink25 = millis(); 
                ultimBlink50 = millis(); 
                ultimBlink75 = millis(); 
                ultimBlink100 = millis();
            }
        } else {
            OpresteIncarcarea(); // Opreste incarcarea
        }
    }

    // Daca incarcarea este activa
    if (incarcare) {
        if (millis() - startTimpIncarcare <= TIMP_INCARCARE) {
            // Verifica procentajul incarcarii pentru a activa LED-urile corespunzatoare
            if (millis() - startTimpIncarcare >= TIMP_INCARCARE / 4.0 * 3) {
                blinkingLed100 = true; // LED 100% blinking activat
                blinkingLed75 = false; 
                blinkingLed50 = false; 
                blinkLed25 = false; 
                if (!ultimaStareBlink100) {
                    ultimBlink100 = millis(); 
                    digitalWrite(PIN_100, HIGH); 
                    digitalWrite(PIN_75, HIGH);
                    digitalWrite(PIN_50, HIGH); 
                    digitalWrite(PIN_25, HIGH); 
                }
            } else if (millis() - startTimpIncarcare >= TIMP_INCARCARE / 2.0) {
                blinkingLed75 = true; // LED 75% blinking activat
                blinkingLed100 = false; 
                blinkingLed50 = false; 
                blinkLed25 = false; 
                if (!ultimaStareBlink75) {
                    ultimBlink75 = millis(); 
                    digitalWrite(PIN_75, HIGH); 
                    digitalWrite(PIN_50, HIGH); 
                    digitalWrite(PIN_25, HIGH); 
                }
            } else if (millis() - startTimpIncarcare >= TIMP_INCARCARE / 4.0) {
                blinkingLed50 = true; // LED 50% blinking activat
                blinkingLed75 = false; 
                blinkingLed100 = false; 
                blinkLed25 = false; 
                if (!ultimaStareBlink50) {
                    ultimBlink50 = millis(); 
                    digitalWrite(PIN_50, HIGH);
                    digitalWrite(PIN_25, HIGH); 
                }
            } else {
                blinkLed25 = true; // LED 25% blinking activat
                blinkingLed50 = false; 
                blinkingLed75 = false; 
                blinkingLed100 = false; 
                if (!UltimaStareBlink25) {
                    digitalWrite(PIN_25, HIGH); // Aprinde LED 25%
                    ultimBlink25 = millis(); 
                }
            }
        }
    }

    // Gestioneaza blinking-ul pentru fiecare LED
    if (blinkLed25) {
        if (millis() - ultimBlink25 >= perioadaBlinking) {
            ultimBlink25 = millis(); // Actualizeaza timpul de blinking
            digitalWrite(PIN_25, !digitalRead(PIN_25)); // Inverseaza starea LED-ului 25%
        }
    }

    if (blinkingLed50) {
        if (millis() - ultimBlink50 >= perioadaBlinking) {
            ultimBlink50 = millis();
            digitalWrite(PIN_50, !digitalRead(PIN_50)); 
        }
    }

    if (blinkingLed75) {
        if (millis() - ultimBlink75 >= perioadaBlinking) {
            ultimBlink75 = millis();
            digitalWrite(PIN_75, !digitalRead(PIN_75)); 
        }
    }

    if (blinkingLed100) {
        if (millis() - ultimBlink100 >= perioadaBlinking) {
            ultimBlink100 = millis(); // Actualizeaza timpul de blinking
            digitalWrite(PIN_100, !digitalRead(PIN_100)); // Inverseaza starea LED-ului 100%
        }
    }

    // Actualizeaza starea ultimelor butoane si LED-uri
    ultimaStareButonInceput = RButonInceput;
    ultimaStareButonOprire = RButonOprire;
    UltimaStareBlink25 = blinkLed25;
    ultimaStareBlink50 = blinkingLed50;
    ultimaStareBlink75 = blinkingLed75;
    ultimaStareBlink100 = blinkingLed100;
}
