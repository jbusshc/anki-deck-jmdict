#include "anki-deck.hpp"

bool inSubstr(const std::string& str, const std::string& substr);

bool isCommon(const std::string& text) {
    if (inSubstr(text, "ichi1")) {
        return true;
    }
    if (inSubstr(text, "news1")) {
        return true;
    }
    if (inSubstr(text, "spec1")) {
        return true;
    }
    if (inSubstr(text, "gai1")) {
        return true;
    }
    return false;
}

void replaceWhiteSpaces(std::string& text) {
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == ' ') {
            text[i] = '_';
        }
    }
}

void cleanText(std::string& text) {
    if (text == "work of art, literature, music, etc. name")
        text = "work_of_art";
    if (text == "art, aesthetics")
        text = "art";
    if (text == "electricity, elec. eng.")
        text = "electricity";
    if (text == "food, cooking")
        text = "food";
    if (text == "gardening, horticulture")
        text = "gardening";
    if (text == "jocular, humorous term")
        text = "jocular";
    if (text == "male term or language")
        text = "male_language";
    if (text == "female term or language")
        text = "female_language";
    if (text == "rude or X-rated term (not displayed in educational software)")
        text = "x_rated";
    
}

void AnkiDeck::generateAnkiDeck() const {
    std::ofstream file(filename_);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo " << filename_ << std::endl;
        return;
    }
    
    std::cout << "Generando baraja de Anki en "<< filename_ << "..." << std::endl;


for (const JMDictEntry& entry : jmdict_.getEntries()) {
        //std::cout << "preEntry " << nEntry << std::endl;
        //std::cout << "Entry " << nEntry << std::endl;
        // Kanji word
        if (entry.kanji.size() > 0) {
            file << "<div class=\"front\">"+entry.kanji[0].keb + "</div>" << "\t";
            // Reading
            for (const JMDictKanjiElement &kanji : entry.kanji) {
                file << "<div class=\"word\">" + kanji.keb + "</div> <br>";
                for (const JMDictReadingElement &reading : entry.reading) {
                    if (reading.re_restr.size() == 0 || reading.re_restr == kanji.keb) {
                        file << "<div class=\"reading\">" + reading.reb + "</div>";
                    }
                }

                file << "<br>";
            }

            // Sense
            for (const JMDictSenseElement &sense : entry.sense) {
                // Pos
                for (const std::string &pos : sense.pos) 
                    file << "<div class=\"pos\">" + pos + "</div>";
                
                // Gloss
                file << "<div class=\"gloss\">";
                for (const std::string &gloss : sense.gloss) 
                    file << "• " + gloss + "<br>";
                
                file << "</div>";

            }

            for (const JMDictSenseElement &sense : entry.sense) {
                for (const JMDictExampleElement &example : sense.example) {
                    //file << "<div class=\"example\">" + example.ex_text + "</div> <br>";
                    for (const std::string &sent : example.ex_sent) {
                        file << "<div class=\"sentence\">" + sent + "</div>";
                    }
                    file << "<br>";
                }
            }

            file << "\t";
            std::string tags = "";
            std::string str = "";
            tags = getTagKanjiKanken(entry.kanji[0].keb) + " ";
            for (const JMDictKanjiElement &kanji : entry.kanji) {
                str = kanji.ke_pri;
                if (str.empty())
                    continue;
                if (!inSubstr(tags, str)) {
                    tags += str + " ";
                }
            }

            for (const JMDictSenseElement &sense : entry.sense) {
                for (const auto &misc : sense.misc) {
                    str = misc;
                    cleanText(str);
                    replaceWhiteSpaces(str);
                    if (!inSubstr(tags, str)) {
                        tags += str + " ";
                    }
                }
    
                for (const auto &dial : sense.dial) {
                    str = dial;
                    replaceWhiteSpaces(str);
                    if (!inSubstr(tags, str)) {
                        tags += str + " ";
                    }
                }

                for (const auto &field : sense.field) {
                    str = field;
                    replaceWhiteSpaces(str);
                    if (!inSubstr(tags, str)) {
                        tags += str + " ";
                    }
                }

                for (const auto &lsource : sense.lsource) {
                    str = lsource;
                    replaceWhiteSpaces(str);
                    if (!inSubstr(tags, str)) {
                        tags += str + " ";
                    }
                }
            }
            if (isCommon(tags)) {
                tags += "common";
            }
            file << tags;

        }
        // Kana word
        else {
            if (entry.reading.size() == 0) {
                std::cerr << "No reading for entry: " << entry.kanji[0].keb << std::endl;
                continue;
            }
            file << "<div class=\"front\">"+entry.reading[0].reb + "</div>" << "\t";
            for (const JMDictReadingElement &reading : entry.reading) {
                file << "<div class=\"word\">" + reading.reb + "</div> <br>";
            }
            for (const JMDictSenseElement &sense : entry.sense) {
                // Pos
                for (const std::string &pos : sense.pos) 
                    file << "<div class=\"pos\">" + pos + "</div>";
                
                // Gloss
                file << "<div class=\"gloss\">";
                for (const std::string &gloss : sense.gloss) 
                    file << "• " + gloss + "<br>";
                
                file << "</div>";

            }
            for (const JMDictSenseElement &sense : entry.sense) {
                for (const JMDictExampleElement &example : sense.example) {
                    //file << "<div class=\"example\">" + example.ex_text + "</div> <br>";
                    for (const std::string &sent : example.ex_sent) {
                        file << "<div class=\"sentence\">" + sent + "</div>";
                    }
                    file << "<br>";
                }
            }
            file << "\t";
            std::string tags = "";
            std::string str = "";
            tags = getTagKanjiKanken(entry.reading[0].reb) + " ";
            for (const JMDictReadingElement &reading : entry.reading) {
                str = reading.re_pri;
                if (str.empty())
                    continue;
                if (!inSubstr(tags, str)) {
                    tags += str + " ";
                }
            }       
            for (const JMDictSenseElement &sense : entry.sense) {
                for (const auto &misc : sense.misc) {
                    str = misc;
                    cleanText(str);
                    replaceWhiteSpaces(str);
                    if (!inSubstr(tags, str)) {
                        tags += str + " ";
                    }
                }
    
                for (const auto &dial : sense.dial) {
                    str = dial;
                    replaceWhiteSpaces(str);
                    if (!inSubstr(tags, str)) {
                        tags += str + " ";
                    }
                }

                for (const auto &field : sense.field) {
                    str = field;
                    replaceWhiteSpaces(str);
                    if (!inSubstr(tags, str)) {
                        tags += str + " ";
                    }
                }

                for (const auto &lsource : sense.lsource) {
                    str = lsource;
                    replaceWhiteSpaces(str);
                    if (!inSubstr(tags, str)) {
                        tags += str + " ";
                    }
                }
            }
            if (isCommon(tags)) {
                tags += "common";
            }
            file << tags;
        } 
        file << std::endl;
        //jmdict_.printEntryAt(nEntry-1, PrintMode::KANJI | PrintMode::READING);
    }

    file.close();
}

bool inSubstr(const std::string& str, const std::string& substr) {
    return str.find(substr) != std::string::npos;
}



void AnkiDeck::readKanjiKanken(const std::string& filename, int pos) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo " << filename << std::endl;
        return;
    }
    std::string kanji;
    while(file >> kanji) {
        kanjiKanken_[pos].push_back(kanji);
    }
}

void AnkiDeck::readAllKanjiKanken() {
    for (int i = 10; i > 0; i--) {
        std::string filename;
        if (i == 1) {
            filename = "../data/kanji-k1.5.txt";
            readKanjiKanken(filename, 1);

            filename = "../data/kanji-k1.txt";
            readKanjiKanken(filename, 0);
        }
        else if (i == 2) {
            filename = "../data/kanji-k2.5.txt";
            readKanjiKanken(filename, 3);

            filename = "../data/kanji-k2.txt";
            readKanjiKanken(filename, 2);
        }
        else {
            filename = "../data/kanji-k" + std::to_string(i) + ".txt";
            readKanjiKanken(filename, i+1);
        }
        
    }
}

int AnkiDeck::getWordKanjiKanken(const std::string& word) const {
    for (int i = 0; i < kanjiKanken_.size(); i++) {
        for (const std::string &kanji : kanjiKanken_[i]) {
            if (inSubstr(word, kanji)) {
                return i;
            }
        }
    }
    return -1;
}

std::string AnkiDeck::getStringKanjiKanken(int pos) const {
    if (pos > 3) {
        return "kanken_" + std::to_string(pos-2);
    }
    else if (pos == 3) {
        return "kanken_2.5";
    }
    else if (pos == 2) {
        return "kanken_2";
    }
    else if (pos == 1) {
        return "kanken_1.5";
    }
    else if (pos == 0) {
        return "kanken_1";
    }
    return "nokanji";
}

std::string AnkiDeck::getTagKanjiKanken(std::string word) const {
    int pos = getWordKanjiKanken(word);
    return getStringKanjiKanken(pos);
}
