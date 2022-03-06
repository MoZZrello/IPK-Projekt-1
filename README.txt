Počítačové komunikace a sítě - Server v jazyku C
Autor: Richard Harman (xharma05)
Datum: 6.3.2022
Body: #TBD


Cieľom projektu je server v jazyku C komunikujúci prostredníctvom protokolu HTTP.
Poskytuje nasledujúce informácie o systéme:
 \hostname - vracia sieťové meno počítaća vrátane domény
 \cpu-name - vracia informácie o procesore
 \load     - vráti výpočet aktuálnej záťaže procesora

Projekt sa prekladá príkazom 'make' a následne sa spúšťa vo formáte:
./hinfosvc port

Program najprv vytvorí socket na zadanom porte na lokálnej sieti, následne skontroluje úspešnosť pripojenia.
Potom očakáva inštrukcie na danej adrese.

Následne mu posielame inštrukcie vo formáte:
 curl http://servername:port/instruction
alebo
 GET http://servername:port/instruction

Po dostaní príkazu, vyfiltruje o aký príkaz sa jedná a zavolá funkciu na jeho vykonanie.


Príklad použitia:
1. Spustím server v jednom terminály pomocou príkazu: 
 ./hinfosvc 12345
2. V druhom terminály požiadam o hostname inštrukciou 
 curl http://localhost:12345/hostname
3. Druhý terminál vypíše hostname a ukončí proces
4. Prvý terminál vypnem server pomocou CTRL+C