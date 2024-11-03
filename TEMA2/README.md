### 2.1 Descriere
În această temă veți crea un joc asemănător cu TypeRacer.

### 2.2 Componente
- **Arduino UNO** (ATmega328P microcontroller)
- **1x LED RGB** (pentru a semnaliza dacă cuvântul corect e scris greșit sau nu)
- **2x Butoane** (pentru start/stop rundă și pentru selectarea dificultății)
- **5x Rezistoare** (3x 220/330 ohm, 2x 1000 ohm)
- **Breadboard**
- **Fire de legătură**

### 2.3 Detalii tehnice

**(1p) LED RGB - Indicator de stare:**
- În starea de repaus, LED-ul va avea culoarea albă.
- La apăsarea butonului de start, LED-ul va clipi timp de 3 secunde, indicând o numărătoare inversă până la începerea rundei.
- În timpul unei runde: LED-ul va fi verde dacă textul introdus este corect și va deveni roșu în caz de greșeală.

**(1p) Butonul Start/Stop:**
- **Modul de repaus:** Dacă jocul este oprit, apăsarea butonului inițiază o nouă rundă după o numărătoare inversă de 3 secunde.
- **În timpul unei runde:** Dacă runda este activă, apăsarea butonului o va opri imediat.

**(1p) Butonul de dificultate:**
- Butonul de dificultate controlează viteza cu care apar cuvintele și poate fi folosit doar în starea de repaus.
- La fiecare apăsare, dificultatea se schimbă ciclind între: **Easy, Medium, Hard**.
- La schimbarea dificultății, se trimite un mesaj prin serial: “Easy/Medium/Hard mode on!”.

Utilizați **debouncing** și **întreruperi** pentru gestionarea apăsărilor butoanelor. Timerele vor fi folosite pentru a seta frecvența apariției cuvintelor. Un site util, care include și un videoclip despre utilizarea întreruperilor și timerelor în Arduino, este disponibil aici.

**(3p) Generarea cuvintelor:**
- Se va crea un dicționar de cuvinte.
- În timpul unei runde, cuvintele vor fi afișate în terminal într-o ordine aleatorie.
- Dacă cuvântul curent a fost scris corect, un cuvânt nou va fi afișat imediat. Dacă nu, un nou cuvânt va apărea după intervalul de timp stabilit în funcție de dificultate.
- Pentru a genera numere aleatorii, trebuie să folosiți funcția **random()**.

**(1p) Alte observații:**
- Timpul alocat pentru o rundă este de **30 de secunde**.
- La sfârșitul fiecărei runde, se va afișa în terminal câte cuvinte au fost scrise corect.

### 2.4 Flow
1. Jocul este în repaus. LED-ul RGB are culoarea albă.
2. Se alege dificultatea jocului folosind butonul de dificultate, iar în terminal va apărea “Easy/Medium/Hard mode on!”.
3. Se apasă butonul de start/stop.
4. LED-ul clipește timp de 3 secunde, iar în terminal se va afișa numărătoarea înversă: **3, 2, 1**.
5. LED-ul devine verde și încep să apară cuvinte de tastat.
6. La tastarea corectă, următorul cuvânt apare imediat. Dacă nu se tastează cuvântul în timpul stabilit de dificultate, va apărea un nou cuvânt.
7. O greșeală face LED-ul să devină roșu. Pentru a corecta cuvântul, se va folosi tasta **BackSpace**.
8. După 30 de secunde, runda se termină, iar în terminal se va afișa scorul: numărul total de cuvinte scrise corect.
9. Jocul se poate opri oricând cu butonul de start/stop.

POZA CU MONTAJUL:
![WhatsApp Image 2024-11-03 at 21 09 22_9ffafd4f](https://github.com/user-attachments/assets/b071896d-4bd2-4fb6-9116-dd5e775c2c3a)

VIDEO FUNCTIONALITATE MONTAJ:
https://github.com/user-attachments/assets/1596bb18-f00f-479f-ae7d-544c08f1b776
