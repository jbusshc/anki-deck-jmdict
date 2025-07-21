#include "anki-deck.hpp"
#include "constant.hpp"

// return 0 ichidan 1 godan 2 otherwise
int isVerb(const JMDictSenseElement &sense) {

    for (const std::string &pos : sense.pos) {
        std::string lowerPos = toLower(pos);
        //std::cout << "DEBUG: Checking pos: " << lowerPos << std::endl;
        if (lowerPos.find("ichidan") != std::string::npos) {
            return 1; // Ichidan verb
        }
        if (lowerPos.find("godan") != std::string::npos) {
            return 0; // Godan verb
        }
        if (lowerPos.find("suru") != std::string::npos) {
            return 2; // Suru verb
        }
        if (lowerPos.find("kuru") != std::string::npos) {
            return 3; // Kuru verb
        }
    }
    return 4; // Other
}

int isAdjective(const JMDictSenseElement &sense) {
    for (const std::string &pos : sense.pos) {
        std::string lowerPos = toLower(pos);
        if (lowerPos.find("adj-i") != std::string::npos || lowerPos.find("adjective (keiyoushi)") != std::string::npos) {
            return 1; // Adjective
        }
    }
    return 0; // Not an adjective
}

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
            Language lastLang = entry.sense[0].lang;
            if (!frontWritten) {
                std::string key = "<p class=\"f\">" + trim(make_key(entry)) + "</p>";
                file << key << "\t";
                frontWritten = true;
            }
            if (entry.kanji.size() > 0) {
                file << "<p class=\"f\">";
                for (size_t k = 0; k < entry.kanji.size(); ++k) {
                    file << trim(entry.kanji[k].keb);
                    if (k < entry.kanji.size() - 1) {
                        file << "<br>";
                    }
                }
                file << "</p>"; // f
                file << "<p class=\"r\">";
                for (const JMDictReadingElement &reading : entry.reading) {
                    file << "<span>" << trim(reading.reb) << "</span>";
                }
                file << "</p>"; // r
                size_t nSense = 1;
                // Sense
                for (size_t senseIdx = 0; senseIdx < entry.sense.size(); ++senseIdx) {
                    const JMDictSenseElement &sense = entry.sense[senseIdx];
                    file << "<p class=\"x\">";
                    if (!isMonoLang()) {
                        file << getLangFlag(sense.lang) << "<br>";
                    }
                    if (lastLang != sense.lang) {
                        lastLang = sense.lang;
                        nSense = 1;
                    }
                    for (size_t i = 0; i < sense.pos.size(); ++i) {
                        file << trim(sense.pos[i]);
                        if (i < sense.pos.size() - 1) {
                            file << "<br>";
                        }
                    }
                    file << "</p>"; // x
                    // Gloss
                    file << "<p class=\"z\">";
                    file << "<span>";
                    file << std::to_string(nSense) + ". ";
                    nSense++;
                    file << "</span>";
                    size_t iter = 0;
                    for (const std::string &gloss : sense.gloss) {
                        file << trim(gloss);
                        if (iter < sense.gloss.size() - 1) {
                            file << "; ";
                        }
                        iter++;
                    }
                    file << "</p>"; // z
                    if (sense.example.size() > 0) {
                        for (const JMDictExampleElement &example : sense.example) {
                            file << "<p class=\"s\">";
                            for (size_t i = 0; i < example.ex_sent.size(); ++i) {
                                file << trim(getHighlightedWord(example.ex_sent[i], entry, sense));
                                if (i < example.ex_sent.size() - 1) {
                                    file << "<br>";
                                }
                            }
                            file << "</p>"; // s
                        }
                    }   
                    if (sense.misc.size() > 0 || sense.xref.size() > 0 || sense.ant.size() > 0) {
                        file << "<p class=\"x\">";
                        if (sense.misc.size() > 0) {
                            for (size_t i = 0; i < sense.misc.size(); ++i) {
                                std::string cleanMisc = trim(sense.misc[i]);
                                if (!cleanMisc.empty()) {
                                    file << cleanMisc;
                                    if (i < sense.misc.size() - 1) {
                                        file << "<br> ";
                                    }
                                }
                            }
                            if (sense.xref.size() > 0 || sense.ant.size() > 0) {
                                file << "<br>";
                            }
                        }
                        if (sense.xref.size() > 0) {
                            for (size_t i = 0; i < sense.xref.size(); ++i) {
                                std::string cleanXref = trim(sense.xref[i]);
                                if (!cleanXref.empty()) {
                                    file << "See also: " + cleanXref;
                                    if (i < sense.xref.size() - 1) {
                                        file << "<br> ";
                                    }
                                }
                            }
                            if (sense.ant.size() > 0) {
                                file << "<br>";
                            }
                        }
                        if (sense.ant.size() > 0) {
                            for (size_t i = 0; i < sense.ant.size(); ++i) {
                                std::string cleanAnt = trim(sense.ant[i]);
                                if (!cleanAnt.empty()) {
                                    file << "Antonym: " + cleanAnt;
                                    if (i < sense.ant.size() - 1) {
                                        file << "<br> ";
                                    }
                                }
                            }
                        }
                        file << "</p>"; // x
                    }
                    if (senseIdx < entry.sense.size() - 1) {
                        file << "<hr>";
                    }
                }
            }
            // Kana word
            else {
                if (entry.reading.size() == 0) {
                    std::cerr << "No reading for entry: " << entry.kanji[0].keb << std::endl;
                    continue;
                }
                if (!frontWritten) {
                    std::string key = "<p class=\"f\">" + trim(make_key(entry)) + "</p>";
                    file << key << "\t";
                    frontWritten = true;
                }
                file << "<p class=\"f\">";
                for (size_t k = 0; k < entry.reading.size(); ++k) {
                    file << trim(entry.reading[k].reb);
                    if (k < entry.reading.size() - 1) {
                        file << "<br>";
                    }
                }
                file << "</p>"; // f 
                size_t nSense = 1;
                for (size_t senseIdx = 0; senseIdx < entry.sense.size(); ++senseIdx) {
                    const JMDictSenseElement &sense = entry.sense[senseIdx];
                    file << "<p class=\"x\">";
                    if (!isMonoLang()) {
                        file << getLangFlag(sense.lang) << "<br>";
                    }
                    if (lastLang != sense.lang) {
                        lastLang = sense.lang;
                        nSense = 1;
                    }
                    for (size_t i = 0; i < sense.pos.size(); ++i) {
                        file << trim(sense.pos[i]);
                        if (i < sense.pos.size() - 1) {
                            file << "<br>";
                        }
                    }
                    file << "</p>"; // x
                    // Gloss
                    file << "<p class=\"z\">";
                    file << "<span>";
                    file << std::to_string(nSense) + ". ";
                    nSense++;
                    file << "</span>";
                    size_t iter = 0;
                    for (const std::string &gloss : sense.gloss) {
                        file << trim(gloss);
                        if (iter < sense.gloss.size() - 1) {
                            file << "; ";
                        }
                        iter++;
                    }
                    file << "</p>"; // z
                    if (sense.example.size() > 0) {
                        for (const JMDictExampleElement &example : sense.example) {
                            file << "<p class=\"s\">";
                            for (size_t i = 0; i < example.ex_sent.size(); ++i) {
                                file << trim(getHighlightedWord(example.ex_sent[i], entry, sense));
                                if (i < example.ex_sent.size() - 1) {
                                    file << "<br>";
                                }
                            }
                            file << "</p>"; // s
                        }
                    }
                    if (sense.misc.size() > 0 || sense.xref.size() > 0 || sense.ant.size() > 0) {
                        file << "<p class=\"x\">";
                        if (sense.misc.size() > 0) {
                            for (size_t i = 0; i < sense.misc.size(); ++i) {
                                std::string cleanMisc = trim(sense.misc[i]);
                                if (!cleanMisc.empty()) {
                                    file << cleanMisc;
                                    if (i < sense.misc.size() - 1) {
                                        file << "<br> ";
                                    }
                                }
                            }
                            if (sense.xref.size() > 0 || sense.ant.size() > 0) {
                                file << "<br>";
                            }
                        }
                        if (sense.xref.size() > 0) {
                            for (size_t i = 0; i < sense.xref.size(); ++i) {
                                std::string cleanXref = trim(sense.xref[i]);
                                if (!cleanXref.empty()) {
                                    file << "See also: " + cleanXref;
                                    if (i < sense.xref.size() - 1) {
                                        file << "<br> ";
                                    }
                                }
                            }
                            if (sense.ant.size() > 0) {
                                file << "<br>";
                            }
                        }
                        if (sense.ant.size() > 0) {
                            for (size_t i = 0; i < sense.ant.size(); ++i) {
                                std::string cleanAnt = trim(sense.ant[i]);
                                if (!cleanAnt.empty()) {
                                    file << "Antonym: " + cleanAnt;
                                    if (i < sense.ant.size() - 1) {
                                        file << "<br> ";
                                    }
                                }
                            }
                        }
                        file << "</p>"; // x
                    }
                    if (senseIdx < entry.sense.size() - 1) {
                        file << "<hr>";
                    }
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
            filename = DATA_KANKEN_1_5;
            readKanjiKanken(filename, 1);

            filename = DATA_KANKEN_1;
            readKanjiKanken(filename, 0);
        }
        else if (i == 2) {
            filename = DATA_KANKEN_2_5;
            readKanjiKanken(filename, 3);

            filename = DATA_KANKEN_2;
            readKanjiKanken(filename, 2);
        }
        else {
            switch(i) {
                case 3: filename = DATA_KANKEN_3; break;
                case 4: filename = DATA_KANKEN_4; break;
                case 5: filename = DATA_KANKEN_5; break;
                case 6: filename = DATA_KANKEN_6; break;
                case 7: filename = DATA_KANKEN_7; break;
                case 8: filename = DATA_KANKEN_8; break;
                case 9: filename = DATA_KANKEN_9; break;
                case 10: filename = DATA_KANKEN_10; break;
            }
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

std::string get_utf8_string_minus_one(const std::string& str) {
    if (str.empty()) return "";

    size_t i = str.size();

    // Retrocede al inicio del último carácter UTF-8
    do {
        --i;
    } while (i > 0 && (static_cast<unsigned char>(str[i]) & 0xC0) == 0x80);

    return str.substr(0, i);
}

// type: 0 = godan, 1 = ichidan, 2 = suru, 3 = kuru
// conjugationIndex: 
// 0: diccionario, 1: masu, 2: te, 3: ta, 4: nai, 5: potencial, 6: volitivo, 7: imperativo, 8: progresivo, 9: pasivo, 10: causativo, 11: causativo pasivo, 12: condicional, 13: deseo
// 14: negativo pasado, 15: negativo te, 16: negativo potencial, 17: negativo volitivo, 18: negativo imperativo, 19: negativo progresivo, 20: negativo pasivo, 21: negativo causativo, 22: negativo causativo pasivo, 23: negativo condicional, 24: negativo deseo
std::string AnkiDeck::conjugateVerb(const std::string& dictionaryForm, int conjugationIndex, int type) const {
    if (dictionaryForm.size() < 2) return dictionaryForm;
    std::string stem = get_utf8_string_minus_one(dictionaryForm);
    std::string ending = dictionaryForm.substr(get_utf8_string_minus_one(dictionaryForm).size());

    // Handle suru (type == 2) and kuru (type == 3) irregular verbs
    if (type == 2) { // suru
        switch (conjugationIndex) {
            case 0: return dictionaryForm; // する
            case 1: return "します";
            case 2: return "して";
            case 3: return "した";
            case 4: return "しない";
            case 5: return "できる";
            case 6: return "しよう";
            case 7: return "しろ";
            case 8: return "している";
            case 9: return "される";
            case 10: return "させる";
            case 11: return "させられる";
            case 12: return "すれば";
            case 13: return "したい";
            case 14: return "しなかった";
            case 15: return "しなくて";
            case 16: return "できない";
            case 17: return "しようとしない";
            case 18: return "しないでください";
            case 19: return "していない";
            case 20: return "されない";
            case 21: return "させない";
            case 22: return "させられない";
            case 23: return "しなければ";
            case 24: return "したくない";
            case 25: return "しました";
            case 26: return "しません";
            case 27: return "しませんでした";
            case 28: return "しましょう";
            case 29: return "しましょうか";
            case 30: return "してください";
            case 31: return "しないでください";
            default: return dictionaryForm;
        }
    }
    if (type == 3) { // kuru
        switch (conjugationIndex) {
            case 0: return dictionaryForm; // くる
            case 1: return "きます";
            case 2: return "きて";
            case 3: return "きた";
            case 4: return "こない";
            case 5: return "こられる";
            case 6: return "こよう";
            case 7: return "こい";
            case 8: return "きている";
            case 9: return "こられる";
            case 10: return "こさせる";
            case 11: return "こさせられる";
            case 12: return "くれば";
            case 13: return "きたい";
            case 14: return "こなかった";
            case 15: return "こなくて";
            case 16: return "こられない";
            case 17: return "こようとしない";
            case 18: return "こないでください";
            case 19: return "きていない";
            case 20: return "こられない";
            case 21: return "こさせない";
            case 22: return "こさせられない";
            case 23: return "こなければ";
            case 24: return "きたくない";
            case 25: return "きました";
            case 26: return "きません";
            case 27: return "きませんでした";
            case 28: return "きましょう";
            case 29: return "きましょうか";
            case 30: return "きてください";
            case 31: return "こないでください";
            default: return dictionaryForm;
        }
    }

    switch (conjugationIndex) {
        case 0: // Forma diccionario
            return dictionaryForm;
        case 1: // Forma masu
            if (type == 1) return stem + "ます";
            if (ending == "う") return stem + "います";
            if (ending == "く") return stem + "きます";
            if (ending == "ぐ") return stem + "ぎます";
            if (ending == "す") return stem + "します";
            if (ending == "つ") return stem + "ちます";
            if (ending == "ぬ") return stem + "にます";
            if (ending == "ぶ") return stem + "びます";
            if (ending == "む") return stem + "みます";
            if (ending == "る") return stem + "ります";
            break;
        case 2: // Forma te
            if (type == 1) return stem + "て";
            if (ending == "う" || ending == "つ" || ending == "る") return stem + "って";
            if (ending == "む" || ending == "ぶ" || ending == "ぬ") return stem + "んで";
            if (ending == "く") return stem + "いて";
            if (ending == "ぐ") return stem + "いで";
            if (ending == "す") return stem + "して";
            break;
        case 3: // Forma ta (pasado)
            if (type == 1) return stem + "た";
            if (ending == "う" || ending == "つ" || ending == "る") return stem + "った";
            if (ending == "む" || ending == "ぶ" || ending == "ぬ") return stem + "んだ";
            if (ending == "く") return stem + "いた";
            if (ending == "ぐ") return stem + "いだ";
            if (ending == "す") return stem + "した";
            break;
        case 4: // Forma nai
            if (type == 1) return stem + "ない";
            if (ending == "う") return stem + "わない";
            if (ending == "く") return stem + "かない";
            if (ending == "ぐ") return stem + "がない";
            if (ending == "す") return stem + "さない";
            if (ending == "つ") return stem + "たない";
            if (ending == "ぬ") return stem + "なない";
            if (ending == "ぶ") return stem + "ばない";
            if (ending == "む") return stem + "まない";
            if (ending == "る") return stem + "らない";
            break;
        case 5: // Potencial
            if (type == 1) return stem + "られる";
            if (ending == "う") return stem + "える";
            if (ending == "く") return stem + "ける";
            if (ending == "ぐ") return stem + "げる";
            if (ending == "す") return stem + "せる";
            if (ending == "つ") return stem + "てる";
            if (ending == "ぬ") return stem + "ねる";
            if (ending == "ぶ") return stem + "べる";
            if (ending == "む") return stem + "める";
            if (ending == "る") return stem + "れる";
            break;
        case 6: // Volitivo informal
            if (type == 1) return stem + "よう";
            if (ending == "う") return stem + "おう";
            if (ending == "く") return stem + "こう";
            if (ending == "ぐ") return stem + "ごう";
            if (ending == "す") return stem + "そう";
            if (ending == "つ") return stem + "とう";
            if (ending == "ぬ") return stem + "のう";
            if (ending == "ぶ") return stem + "ぼう";
            if (ending == "む") return stem + "もう";
            if (ending == "る") return stem + "ろう";
            break;
        case 7: // Imperativo
            if (type == 1) return stem + "ろ";
            if (ending == "う") return stem + "え";
            if (ending == "く") return stem + "け";
            if (ending == "ぐ") return stem + "げ";
            if (ending == "す") return stem + "せ";
            if (ending == "つ") return stem + "て";
            if (ending == "ぬ") return stem + "ね";
            if (ending == "ぶ") return stem + "べ";
            if (ending == "む") return stem + "め";
            if (ending == "る") return stem + "れ";
            break;
        case 8: // Progresivo / continuo
            return conjugateVerb(dictionaryForm, 2, type) + "いる";
        case 9: // Pasivo
            if (type == 1) return stem + "られる";
            if (ending == "う") return stem + "われる";
            if (ending == "く") return stem + "かれる";
            if (ending == "ぐ") return stem + "がれる";
            if (ending == "す") return stem + "される";
            if (ending == "つ") return stem + "たれる";
            if (ending == "ぬ") return stem + "なれる";
            if (ending == "ぶ") return stem + "ばれる";
            if (ending == "む") return stem + "まれる";
            if (ending == "る") return stem + "られる";
            break;
        case 10: // Causativo
            if (type == 1) return stem + "させる";
            if (ending == "う") return stem + "わせる";
            if (ending == "く") return stem + "かせる";
            if (ending == "ぐ") return stem + "がせる";
            if (ending == "す") return stem + "させる";
            if (ending == "つ") return stem + "たせる";
            if (ending == "ぬ") return stem + "なせる";
            if (ending == "ぶ") return stem + "ばせる";
            if (ending == "む") return stem + "ませる";
            if (ending == "る") return stem + "らせる";
            break;
        case 11: // Causativo pasivo
            if (type == 1) return stem + "させられる";
            if (ending == "う") return stem + "わされる";
            if (ending == "く") return stem + "かされる";
            if (ending == "ぐ") return stem + "がされる";
            if (ending == "す") return stem + "させられる";
            if (ending == "つ") return stem + "たされる";
            if (ending == "ぬ") return stem + "なされる";
            if (ending == "ぶ") return stem + "ばされる";
            if (ending == "む") return stem + "まされる";
            if (ending == "る") return stem + "らされる";
            break;
        case 12: // Condicional (仮定形（ば）)
            if (type == 1) return stem + "れば";
            if (ending == "う") return stem + "えば";
            if (ending == "く") return stem + "けば";
            if (ending == "ぐ") return stem + "げば";
            if (ending == "す") return stem + "せば";
            if (ending == "つ") return stem + "てば";
            if (ending == "ぬ") return stem + "ねば";
            if (ending == "ぶ") return stem + "べば";
            if (ending == "む") return stem + "めば";
            if (ending == "る") return stem + "れば";
            break;
        case 13: // Deseo (たい形)
            return stem + "たい";
        // Negativos de cada forma
        case 14: // Pasado negativo
        {
            std::string naiForm = conjugateVerb(dictionaryForm, 4, type);
            std::string naiStem = get_utf8_string_minus_one(get_utf8_string_minus_one(naiForm));
            return naiStem + "なかった";
        }
        case 15: // Te negativo
        {
            std::string naiForm = conjugateVerb(dictionaryForm, 4, type);
            std::string naiStem = get_utf8_string_minus_one(get_utf8_string_minus_one(naiForm));
            return naiStem + "なくて";
        }
        case 16: // Potencial negativo
        {
            std::string potForm = conjugateVerb(dictionaryForm, 5, type);
            std::string potStem = get_utf8_string_minus_one(get_utf8_string_minus_one(potForm));
            return potStem + "ない";
        }
        case 17: // Volitivo negativo
        {
            std::string volForm = conjugateVerb(dictionaryForm, 6, type);
            std::string volStem = get_utf8_string_minus_one(volForm);
            return volStem + "くない";
        }
        case 18: // Imperativo negativo
            return conjugateVerb(dictionaryForm, 4, type) + "でください";
        case 19: // Progresivo negativo
            return conjugateVerb(dictionaryForm, 2, type) + "いない";
        case 20: // Pasivo negativo
        {
            std::string pasForm = conjugateVerb(dictionaryForm, 9, type);
            std::string pasStem = get_utf8_string_minus_one(get_utf8_string_minus_one(pasForm));
            return pasStem + "ない";
        }
        case 21: // Causativo negativo
        {
            std::string causForm = conjugateVerb(dictionaryForm, 10, type);
            std::string causStem = get_utf8_string_minus_one(get_utf8_string_minus_one(causForm));
            return causStem + "ない";
        }
        case 22: // Causativo pasivo negativo
        {
            std::string causPasForm = conjugateVerb(dictionaryForm, 11, type);
            std::string causPasStem = get_utf8_string_minus_one(get_utf8_string_minus_one(causPasForm));
            return causPasStem + "ない";
        }
        case 23: // Condicional negativo
        {
            std::string naiForm = conjugateVerb(dictionaryForm, 4, type);
            std::string naiStem = get_utf8_string_minus_one(get_utf8_string_minus_one(naiForm));
            return naiStem + "なければ";
        }
        case 24: // Deseo negativo
            return stem + "たくない";
        case 25: // masu pasado
        {
            std::string masuForm = conjugateVerb(dictionaryForm, 1, type);
            std::string masuStem = get_utf8_string_minus_one(get_utf8_string_minus_one(masuForm));
            return masuStem + "ました";
        }
        case 26: // masu negativo
        {
            std::string masuForm = conjugateVerb(dictionaryForm, 1, type);
            std::string masuStem = get_utf8_string_minus_one(get_utf8_string_minus_one(masuForm));
            return masuStem + "ません";
        }
        case 27: // masu pasado negativo
        {
            std::string masuForm = conjugateVerb(dictionaryForm, 1, type);
            std::string masuStem = get_utf8_string_minus_one(get_utf8_string_minus_one(masuForm));
            return masuStem + "ませんでした";
        }
        case 28: // volitivo formal
        {
            std::string masuForm = conjugateVerb(dictionaryForm, 1, type);
            std::string masuStem = get_utf8_string_minus_one(get_utf8_string_minus_one(masuForm));
            return masuStem + "ましょう";
        }
        case 29: // volitivo formal interrogativo
        {
            std::string masuForm = conjugateVerb(dictionaryForm, 1, type);
            std::string masuStem = get_utf8_string_minus_one(get_utf8_string_minus_one(masuForm));
            return masuStem + "ましょうか";
        }
        case 30: // imperativo formal
            return conjugateVerb(dictionaryForm, 2, type) + "ください";
        case 31: // imperativo formal negativo
            return conjugateVerb(dictionaryForm, 4, type) + "でください";
    }
    return dictionaryForm;
}

std::string AnkiDeck::conjugateAdjI(const std::string& dictionaryForm, int conjugationIndex) const {
    if (dictionaryForm.size() < 2) return dictionaryForm;
    std::string stem = get_utf8_string_minus_one(dictionaryForm);
    
    switch (conjugationIndex) {
        case 0: return dictionaryForm; // Diccionario
        case 1: return stem + "いです"; // Masu
        case 2: return stem + "くて"; // Te
        case 3: return stem + "かった"; // Ta
        case 4: return stem + "くない"; // Nai
        case 5: return stem + "くなれる"; // Potencial (no estándar, pero válida si usas "なる")
        case 6: return dictionaryForm; // Volitivo (no aplica)
        case 7: return dictionaryForm; // Imperativo (no aplica)
        case 8: return stem + "くなっている"; // Progresivo (estar volviéndose)
        case 9: return dictionaryForm; // Pasivo (no aplica)
        case 10: return dictionaryForm; // Causativo (no aplica)
        case 11: return dictionaryForm; // Causativo pasivo (no aplica)
        case 12: return stem + "ければ"; // Condicional
        case 13: return stem + "くありたい"; // Deseo
        case 14: return stem + "くなかった"; // Pasado negativo
        case 15: return stem + "くなくて"; // Te negativo
        case 16: return stem + "くなれない"; // Potencial negativo
        case 17: return dictionaryForm; // Volitivo negativo (no aplica)
        case 18: return dictionaryForm; // Imperativo negativo (no aplica)
        case 19: return stem + "くなっていない"; // Progresivo negativo
        case 20: return dictionaryForm; // Pasivo negativo (no aplica)
        case 21: return dictionaryForm; // Causativo negativo (no aplica)
        case 22: return dictionaryForm; // Causativo pasivo negativo (no aplica)
        case 23: return stem + "くなければ"; // Condicional negativo
        case 24: return stem + "くありたくない"; // Deseo negativo
        default: return dictionaryForm;
    }
}

std::string AnkiDeck::getHighlightedWord(const std::string& example, const JMDictEntry& word,const JMDictSenseElement& sense) const {
    std::string key;
    std::string result;
    std::string resultFinal = "";
    bool hasHighlighted = false; // for deciding if we need to search kana in kanji words
    //bool debugFound = false; // for debugging purposes

    if(isAdjective(sense) == 1) {
        for(int i = 0; i < 25; i++) {
            result = "";
            // kanji word
            if (word.kanji.size() > 0) {
                for (const JMDictKanjiElement &kanji : word.kanji) {
                    key = conjugateAdjI(kanji.keb, i);
                    if (inSubstr(example, key)) {
                        std::string highlighted = "<span>" + trim(key) + "</span>";
                        result = trim(example.substr(0, example.find(key)));
                        result += highlighted;
                        result += trim(example.substr(example.find(key) + key.size()));
                        if (resultFinal.length() < result.length()) {
                            resultFinal = result;
                        }
                        //debugFound = true; // for debugging purposes
                    }
                }
                for (const JMDictReadingElement &reading : word.reading) {
                    key = conjugateAdjI(reading.reb, i);
                    if (inSubstr(example, key)) {
                        std::string highlighted = "<span>" + trim(key) + "</span>";
                        result = trim(example.substr(0, example.find(key)));
                        result += highlighted;
                        result += trim(example.substr(example.find(key) + key.size()));
                        if (resultFinal.length() < result.length()) {
                            resultFinal = result;
                        }
                        //debugFound = true; // for debugging purposes
                    }
                }
            } else { // kana word
                for (const JMDictReadingElement &reading : word.reading) {
                    key = conjugateAdjI(reading.reb, i);
                    if (inSubstr(example, key)) {
                        std::string highlighted = "<span>" + trim(key) + "</span>";
                        result = trim(example.substr(0, example.find(key)));
                        result += highlighted;
                        result += trim(example.substr(example.find(key) + key.size()));
                        //debugFound = true; // for debugging purposes
                        if (resultFinal.length() < result.length()) {
                            resultFinal = result;
                        }
                    }
                }
            }
        }
    } else if(isVerb(sense) == 0) {
        for(int i = 0; i < 32; i++) {
            result = "";
            // kanji word   
            if (word.kanji.size() > 0) {
                for (const JMDictKanjiElement &kanji : word.kanji) {
                    key = conjugateVerb(kanji.keb, i, 0);
                    if (inSubstr(example, key)) {
                        std::string highlighted = "<span>" + trim(key) + "</span>";
                        result = trim(example.substr(0, example.find(key)));
                        result += highlighted;
                        result += trim(example.substr(example.find(key) + key.size()));
                        hasHighlighted = true; // found kanji conjugation
                        //debugFound = true; // for debugging purposes
                        if (resultFinal.length() < result.length()) {
                            resultFinal = result;
                        }
                    }
                }
                // If we found a kanji conjugation, we don't need to search for kana conjugations
                if (hasHighlighted) {
                    continue;
                }
                for (const JMDictReadingElement &reading : word.reading) {
                    key = conjugateVerb(reading.reb, i, 0);
                    if (inSubstr(example, key)) {
                        std::string highlighted = "<span>" + trim(key) + "</span>";
                        result = trim(example.substr(0, example.find(key)));
                        result += highlighted;
                        result += trim(example.substr(example.find(key) + key.size()));
                        //debugFound = true; // for debugging purposes
                        if (resultFinal.length() < result.length()) {
                            resultFinal = result;
                        }
                    }
                }
            } else { // kana word
                for (const JMDictReadingElement &reading : word.reading) {
                    key = conjugateVerb(reading.reb, i, 0);
                    if (inSubstr(example, key)) {
                        std::string highlighted = "<span>" + trim(key) + "</span>";
                        result = trim(example.substr(0, example.find(key)));
                        result += highlighted;
                        result += trim(example.substr(example.find(key) + key.size()));
                        //debugFound = true; // for debugging purposes
                        if (resultFinal.length() < result.length()) {
                            resultFinal = result;
                        }
                    }
                }
            }
        }
    } else if(isVerb(sense) == 1) {
        for(int i = 0; i < 32; i++) {
            result = "";
            // kanji word
            if (word.kanji.size() > 0) {
                for (const JMDictKanjiElement &kanji : word.kanji) {
                    key = conjugateVerb(kanji.keb, i, 1);
                    if (inSubstr(example, key)) {
                        std::string highlighted = "<span>" + trim(key) + "</span>";
                        result = trim(example.substr(0, example.find(key)));
                        result += highlighted;
                        result += trim(example.substr(example.find(key) + key.size()));
                        hasHighlighted = true; // found kanji conjugation
                        //debugFound = true; // for debugging purposes
                        if (resultFinal.length() < result.length()) {
                            resultFinal = result;
                        }
                    }
                }
                // If we found a kanji conjugation, we don't need to search for kana conjugations
                if (hasHighlighted) {
                    continue;
                }
                
                for (const JMDictReadingElement &reading : word.reading) {
                    key = conjugateVerb(reading.reb, i, 1);
                    if (inSubstr(example, key)) {
                        std::string highlighted = "<span>" + trim(key) + "</span>";
                        result = trim(example.substr(0, example.find(key)));
                        result += highlighted;
                        result += trim(example.substr(example.find(key) + key.size()));
                        //debugFound = true; // for debugging purposes
                        if (resultFinal.length() < result.length()) {
                            resultFinal = result;
                        }
                    }
                }
            } else { // kana word
                for (const JMDictReadingElement &reading : word.reading) {
                    key = conjugateVerb(reading.reb, i, 1);
                    if (inSubstr(example, key)) {
                        std::string highlighted = "<span>" + trim(key) + "</span>";
                        result = trim(example.substr(0, example.find(key)));
                        result += highlighted;
                        result += trim(example.substr(example.find(key) + key.size()));
                        //debugFound = true; // for debugging purposes
                        if (resultFinal.length() < result.length()) {
                            resultFinal = result;
                        }
                    }
                }
            }
        }
    } else { // not a verb or adjective
        if (word.kanji.size() > 0) {
            for (const JMDictKanjiElement &kanji : word.kanji) {
                key = kanji.keb;
                if (inSubstr(example, key)) {
                    std::string highlighted = "<span>" + trim(key) + "</span>";
                    result = trim(example.substr(0, example.find(key)));
                    result += highlighted;
                    result += trim(example.substr(example.find(key) + key.size()));
                    //debugFound = true; // for debugging purposes
                    hasHighlighted = true; // found kanji word
                    if (resultFinal.length() < result.length()) {
                        resultFinal = result;
                    }
                }
            }

            if (!hasHighlighted) {
                for (const JMDictReadingElement &reading : word.reading) {
                    key = reading.reb;
                    if (inSubstr(example, key)) {
                        std::string highlighted = "<span>" + trim(key) + "</span>";
                        result = trim(example.substr(0, example.find(key)));
                        result += highlighted;
                        result += trim(example.substr(example.find(key) + key.size()));
                        //debugFound = true; // for debugging purposes
                        if (resultFinal.length() < result.length()) {
                            resultFinal = result;
                        }
                    }
                }
            }   
        } else { // kana word
            for (const JMDictReadingElement &reading : word.reading) {
                key = reading.reb;
                if (inSubstr(example, key)) {
                    std::string highlighted = "<span>" + trim(key) + "</span>";
                    result = trim(example.substr(0, example.find(key)));
                    result += highlighted;
                    result += trim(example.substr(example.find(key) + key.size()));
                    //debugFound = true; // for debugging purposes
                    if (resultFinal.length() < result.length()) {
                        resultFinal = result;
                    }
                }
            }
        }
    }
    // If a conjugation was found, return the example with the highlighted word
    /*
    if (!debugFound) {
        std::cerr << "Warning: No conjugation found for example: " << example << " with word: " << word.kanji[0].keb << std::endl;
        // print all conjugations for debugging
        if (word.kanji.size() > 0) {
            if( isVerb(sense) == 0) {
                for(int i = 0; i < 32; i++) {
                    std::string conjugation = conjugateVerb(word.kanji[0].keb, i, 0);
                    std::cerr << "Conjugation " << i << ": " << conjugation << std::endl;
                }
            } else if(isVerb(sense) == 1) {
                for(int i = 0; i < 32; i++) {
                    std::string conjugation = conjugateVerb(word.kanji[0].keb, i, 1);
                    std::cerr << "Conjugation " << i << ": " << conjugation << std::endl;
                }
            } else if(isAdjective(sense) == 1) {
                for(int i = 0; i < 25; i++) {
                    std::string conjugation = conjugateAdjI(word.kanji[0].keb, i);
                    std::cerr << "Conjugation " << i << ": " << conjugation << std::endl;
                }
            } else {
                std::cerr << "No conjugation found for word: " << word.kanji[0].keb << std::endl;
            }
        } else {
            std::cerr << "No conjugation found for word: " << word.reading[0].reb << std::endl;
            if( isVerb(sense) == 0) {
                for(int i = 0; i < 32; i++) {
                    std::string conjugation = conjugateVerb(word.reading[0].reb, i, 0);
                    std::cerr << "Conjugation " << i << ": " << conjugation << std::endl;
                }
            } else if(isVerb(sense) == 1) {
                for(int i = 0; i < 32; i++) {
                    std::string conjugation = conjugateVerb(word.reading[0].reb, i, 1);
                    std::cerr << "Conjugation " << i << ": " << conjugation << std::endl;
                }
            } else if(isAdjective(sense) == 1) {
                for(int i = 0; i < 25; i++) {
                    std::string conjugation = conjugateAdjI(word.reading[0].reb, i);
                    std::cerr << "Conjugation " << i << ": " << conjugation << std::endl;
                }
            } else {
                std::cerr << "No conjugation found for word: " << word.reading[0].reb << std::endl;
            }
        }
    }

    if (result.empty()) {
        // print word ent_seq
        
        std::cerr << "ent_seq: " << word.ent_seq << std::endl;
        if (word.kanji.size() > 0) {
            std::cerr << "kanji: " << word.kanji[0].keb << std::endl;
            if (isVerb(sense) == 0) {
            for (int i = 0; i < 32; i++) {
                std::string conjugation = conjugateVerb(word.kanji[0].keb, i, 0);
                std::cerr << "Conjugation " << i << ": " << conjugation << std::endl;
            }
            } else if (isVerb(sense) == 1) {
            for (int i = 0; i < 32; i++) {
                std::string conjugation = conjugateVerb(word.kanji[0].keb, i, 1);
                std::cerr << "Conjugation " << i << ": " << conjugation << std::endl;
            }
            } else if (isAdjective(sense) == 1) {
            for (int i = 0; i < 25; i++) {
                std::string conjugation = conjugateAdjI(word.kanji[0].keb, i);
                std::cerr << "Conjugation " << i << ": " << conjugation << std::endl;
            }
            }
        } else {
            std::cerr << "reading: " << word.reading[0].reb << std::endl;
            if (isVerb(sense) == 0) {
            for (int i = 0; i < 32; i++) {
                std::string conjugation = conjugateVerb(word.reading[0].reb, i, 0);
                std::cerr << "Conjugation " << i << ": " << conjugation << std::endl;
            }
            } else if (isVerb(sense) == 1) {
            for (int i = 0; i < 32; i++) {
                std::string conjugation = conjugateVerb(word.reading[0].reb, i, 1);
                std::cerr << "Conjugation " << i << ": " << conjugation << std::endl;
            }
            } else if (isAdjective(sense) == 1) {
            for (int i = 0; i < 25; i++) {
                std::string conjugation = conjugateAdjI(word.reading[0].reb, i);
                std::cerr << "Conjugation " << i << ": " << conjugation << std::endl;
            }
            }
        }
        std::cerr << "Example: " << example << std::endl;
    }
    */
    return resultFinal.length() > 0 ? resultFinal : result;
}