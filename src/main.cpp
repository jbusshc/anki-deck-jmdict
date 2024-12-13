#include "anki-deck.hpp"

#define DUMMY "../data/dummy.xml"
#define JMDICT_XML "../data/JMdict.xml"
#define JMDICT_E_XML "../data/JMdict_e_examp.xml"
#define EXAMPLE_EXTRACT 1

int main() {
   /* std::vector<std::string> langs = createLangsVector( Language::SPANISH);
    std::vector<std::string> langs_e = createLangsVector( Language::ENGLISH);


    JMDict dict("../data/JMdict.xml", langs);
    dict.openXML("../data/JMdict_e_examp.xml");
    dict.setLang(Language::ENGLISH);
    dict.parse();

    std::cout << "anki-deck" << std::endl;
    AnkiDeck deck(dict, "../data/deck.txt");
    //deck.generateAnkiDeck();
    */

    AnkiDeck deck;
    deck.setFilename("../data/deck.txt");
    //deck.generateJMdict(Language::ENGLISH, DUMMY);
    deck.generateJMdict(Language::SPANISH|Language::ENGLISH, JMDICT_XML);
    deck.generateJMdict(Language::ENGLISH, JMDICT_E_XML, EXAMPLE_EXTRACT);
    //deck.getJMDict().print();
    deck.getJMDict().generateTxt("../data/all-datos-first-sense.txt");

    return 0;
}
