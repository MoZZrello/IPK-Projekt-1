Počítačové komunikace a sítě - Server v jazyku C <br />
Autor: Richard Harman (xharma05) <br />
Datum: 6.3.2022 <br />
Body: #TBD <br />


Cieľom projektu je server v jazyku C komunikujúci prostredníctvom protokolu HTTP. <br />
Poskytuje nasledujúce informácie o systéme: <br />
 \hostname - vracia sieťové meno počítaća vrátane domény <br />
 \cpu-name - vracia informácie o procesore <br />
 \load     - vráti výpočet aktuálnej záťaže procesora <br />

Projekt sa prekladá príkazom 'make' a následne sa spúšťa vo formáte: <br />
./hinfosvc port <br />

Program najprv vytvorí socket na zadanom porte na lokálnej sieti, následne skontroluje úspešnosť pripojenia. <br />
Potom očakáva inštrukcie na danej adrese. <br />

Následne mu posielame inštrukcie vo formáte: <br />
 curl http://servername:port/instruction <br />
alebo <br /> 
 GET http://servername:port/instruction <br />

Po dostaní príkazu, vyfiltruje o aký príkaz sa jedná a zavolá funkciu na jeho vykonanie. <br />


Príklad použitia: <br />
1. Spustím server v jednom terminály pomocou príkazu:  <br />
 ./hinfosvc 12345 <br />
2. V druhom terminály požiadam o hostname inštrukciou  <br />
 curl http://localhost:12345/hostname <br />
3. Druhý terminál vypíše hostname a ukončí proces <br />
4. Prvý terminál vypnem server pomocou CTRL+C
