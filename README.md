# Sistem interactiv pentru exersarea alfabetului Braille

Proiect realizat pentru disciplina SIO, folosind o placa
Arduino UNO cu microcontroler ATmega328P.

Dispozitivul reda vocal o litera, iar utilizatorul introduce
combinatia Braille prin selectarea punctelor cu ajutorul
butoanelor. La confirmare, programul verifica raspunsul si
ofera feedback audio si vizual.

## Functionalitati propuse
- Mod Invatare, cu indicatii
- Mod Test, fara indicatii
- Afisarea punctelor selectate si a rezultatului
- Masurarea timpului de raspuns
- Salvarea progresului in memoria interna
- Repetarea mai frecventa a literelor gresite

## Componente
- Arduino UNO cu ATmega328P
- Doua module cu cate patru butoane (sau modul cu 8 butoane, pe 2 coloane)
- Display LCD
- Modul audio DFPlayer Mini
- Difuzor si card microSD
Sase butoane reprezinta punctele Braille, iar celelalte doua
sunt folosite pentru Confirmare si Mod/Repeta.
