# Testarea literelor Braille prin Serial Monitor
1 4
2 5
3 6

In braille.cpp am pus partea de conversie, care nu depinde de Arduino, iar in .ino am lasat doar comunicarea prin USB serial. Mai tarziu, o sa folosim si butoanele ca sa formam aceeasi masca de 6 biti, pe care o trimitem la identificaLitera().

Referinta alfabet: https://www.brailleauthority.org/alphabet/alphabet.pdf
Structura sketch-ului: https://docs.arduino.cc/arduino-cli/sketch-specification/
