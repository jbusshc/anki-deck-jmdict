#include "anki-deck.hpp"
#include "constant.hpp"
#include <windows.h>
int main() {

    //Set the console output to UTF-8
    SetConsoleOutputCP(CP_UTF8);
    system("chcp 65001 > nul");

    AnkiDeck deck;
    deck.setFilename("../data/deck.txt");
    //deck.generateJMdict(Language::ENGLISH, DUMMY);
    deck.generateJMdict(Language::SPANISH|Language::ENGLISH, JMDICT_XML);
    deck.generateJMdict(Language::ENGLISH, JMDICT_E_XML, MODE_EXAMPLE_EXTRACT);
    deck.generateAnkiDeck();

    return 0;
}