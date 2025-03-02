#pragma once 

#include "jmdict.hpp"
#include <fstream>

class AnkiDeck {
    public:
        AnkiDeck() { init(); }
        void init() {
            for(int i=0; i < 12; i++) kanjiKanken_.push_back(std::vector<std::string>());
            readAllKanjiKanken();
        }
        void setFilename(const std::string &filename) {
            filename_ = filename;
        }
        AnkiDeck(const std::string& filename) : filename_(filename) {}
        JMDict& getJMDict() { return jmdict_; }
        const std::string& getFilename() { return filename_; }
        void generateJMdict(Language lang, const std::string& filename, int mode = 0) {
            jmdict_.readJMDict(filename, lang, mode);
        }
        void generateAnkiDeck() const ;
        void readKanjiKanken(const std::string& filename, int pos);
        void readAllKanjiKanken();
        int getWordKanjiKanken(const std::string& word) const;
        std::string getStringKanjiKanken(int pos) const;
        std::string getTagKanjiKanken(std::string word) const;
    private:
        JMDict jmdict_;
        std::string filename_;
        std::vector<std::vector<std::string>> kanjiKanken_;
};