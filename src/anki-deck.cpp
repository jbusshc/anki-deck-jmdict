#include "anki-deck.hpp"

void AnkiDeck::generateAnkiDeck() const {
    std::ofstream file(filename_);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo " << filename_ << std::endl;
        return;
    }
    
    std::cout << "Generando baraja de Anki en "<< filename_ << "..." << std::endl;
    for (const std::vector<JMDictEntry> &entries: jmdict_.getEntries()) {
        bool frontWritten = false;
        for (const JMDictEntry& entry : entries ) {
            //std::cout << "preEntry " << nEntry << std::endl;
            //std::cout << "Entry " << nEntry << std::endl;
            // Kanji word
            if (entry.kanji.size() > 0) {
                if (!frontWritten) {
                    file << "<div class=\"front\">"+entry.kanji[0].keb + "</div>" << "\t";
                    frontWritten = true;
                }
                // Reading
                for (const JMDictKanjiElement &kanji : entry.kanji) {
                    file << "<div class=\"word\">" + kanji.keb + "</div> <br>";
                    for (const JMDictReadingElement &reading : entry.reading) {
                        if (reading.re_restr.size() == 0 || inSubstr(kanji.keb, reading.re_restr) || reading.re_restr == "") {
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
                    if (!isMonoLang()) {
                        file << "<div class=\"flag\"> ";
                        file << getLangFlag(sense.lang);
                        file << "</div>";
                    }
                    // Gloss
                    file << "<div class=\"gloss\">";
                    for (const std::string &gloss : sense.gloss) 
                        file << "• " + gloss + "<br>";
                    
                    file << "</div>";
                    file << "<div class=\"example\">";
                    for (const JMDictExampleElement &example : sense.example) {
                        //file << "<div class=\"example\">" + example.ex_text + "</div> <br>";
                        for (const std::string &sent : example.ex_sent) {
                            file << "<div class=\"sentence\">" + getHighlightedWord(sent, entry)  + "</div>";
                        }
                    }
                    file << "</div>";
                }
            }
            // Kana word
            else {
                if (entry.reading.size() == 0) {
                    std::cerr << "No reading for entry: " << entry.kanji[0].keb << std::endl;
                    continue;
                }
                if (!frontWritten) {
                    file << "<div class=\"front\">"+entry.reading[0].reb + "</div>" << "\t";
                    frontWritten = true;
                }
                for (const JMDictReadingElement &reading : entry.reading) {
                    file << "<div class=\"word\">" + reading.reb + "</div> <br>";
                }
                for (const JMDictSenseElement &sense : entry.sense) {
                    // Pos
                    for (const std::string &pos : sense.pos) 
                        file << "<div class=\"pos\">" + pos + "</div>";
                    if (!isMonoLang()) {
                        file << "<div class=\"flag\"> ";
                        file << getLangFlag(sense.lang);
                        file << "</div>";
                    }
                    // Gloss
                    file << "<div class=\"gloss\">";
                    for (const std::string &gloss : sense.gloss) 
                        file << "• " + gloss + "<br>";
                    
                    file << "</div>";
                    file << "<div class=\"example\">";
                    for (const JMDictExampleElement &example : sense.example) {
                        //file << "<div class=\"example\">" + example.ex_text + "</div> <br>";
                        for (const std::string &sent : example.ex_sent) {
                            file << "<div class=\"sentence\">" + getHighlightedWord(sent, entry)  + "</div>";
                        }
                    }
                    file << "</div>";
                }
            }
        }
        
        // write tags
        file << "\t";
        std::string tags = "";
        std::string str = "";
        if (entries[0].kanji.size() > 0) {
            tags = getTagKanjiKanken(entries[0].kanji[0].keb) + " ";
            for (const JMDictKanjiElement &kanji : entries[0].kanji) {
                str = kanji.ke_pri;
                if (str.empty())
                    continue;
                if (!inSubstr(tags, str)) {
                    tags += str + " ";
                }
            }
        }
        else {
            tags = getTagKanjiKanken(entries[0].reading[0].reb) + " ";
            for (const JMDictReadingElement &reading : entries[0].reading) {
                str = reading.re_pri;
                if (str.empty())
                    continue;
                if (!inSubstr(tags, str)) {
                    tags += str + " ";
                }
            }
        }
        for (const JMDictEntry &entry : entries) {
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
        }
        if (isCommon(tags)) {
            tags += "common";
        }
        file << tags;
        file << std::endl;
    }


    file.close();
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
    for (size_t i = 0; i < kanjiKanken_.size(); i++) {
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

std::string AnkiDeck::getHighlightedWord(const std::string& example, const JMDictEntry& word) const {
    if (word.kanji.size() > 0) {
        for (const JMDictKanjiElement &kanji : word.kanji) {
            if (inSubstr(example, kanji.keb)) {
                std::string highlighted = "<span class=\"highlighted\">" + kanji.keb + "</span>";
                std::string result = example.substr(0, example.find(kanji.keb));
                result += highlighted;
                result += example.substr(example.find(kanji.keb) + kanji.keb.size());
                return result;
            }
        }
        for (const JMDictReadingElement &reading : word.reading) {
            if (inSubstr(example, reading.reb)) {
                std::string highlighted = "<span class=\"highlighted\">" + reading.reb + "</span>";
                std::string result = example.substr(0, example.find(reading.reb));
                result += highlighted;
                result += example.substr(example.find(reading.reb) + reading.reb.size());
                return result;
            }
        }
    } else {
        for (const JMDictReadingElement &reading : word.reading) {
            if (inSubstr(example, reading.reb)) {
                std::string highlighted = "<span class=\"highlighted\">" + reading.reb + "</span>";
                std::string result = example.substr(0, example.find(reading.reb));
                result += highlighted;
                result += example.substr(example.find(reading.reb) + reading.reb.size());
                return result;
            }
        }
    }
    return example;
}
