#include "anki-deck.hpp"
#include "constant.hpp"

int main() {

    AnkiDeck deck;
    deck.setFilename("../data/deck.txt");
    //deck.generateJMdict(Language::ENGLISH, DUMMY);
    deck.generateJMdict(Language::SPANISH|Language::ENGLISH, JMDICT_XML);
    deck.generateJMdict(Language::ENGLISH, JMDICT_E_XML, MODE_EXAMPLE_EXTRACT);
    deck.generateAnkiDeck();

    return 0;
}
