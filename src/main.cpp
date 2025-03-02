#include "anki-deck.hpp"

#define DUMMY "../data/dummy.xml"
#define JMDICT_XML "../data/JMdict"
#define JMDICT_E_XML "../data/JMdict_e_examp"
#define EXAMPLE_EXTRACT 1

int main() {

    AnkiDeck deck;
    deck.setFilename("../data/deck.txt");
    //deck.generateJMdict(Language::ENGLISH, DUMMY);
    deck.generateJMdict(Language::SPANISH|Language::ENGLISH, JMDICT_XML);
    //deck.generateJMdict(Language::ENGLISH, JMDICT_E_XML, EXAMPLE_EXTRACT);
    deck.generateAnkiDeck();

    return 0;
}
