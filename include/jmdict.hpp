#pragma once

#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <iostream>
#include <unordered_map>
#include <fstream>

#include "lang.hpp"

/***********************************************************************
    Estructuras de datos para almacenar los elementos del diccionario 
************************************************************************/

// Elemento de kanji
struct JMDictKanjiElement {
    std::string keb;
    std::string ke_inf;
    std::string ke_pri;
};
// Elemento de lectura
struct JMDictReadingElement {
    std::string reb;
    std::string re_nokanji;
    std::string re_restr;
    std::string re_inf;
    std::string re_pri;
};
// Elemento de ejemplo
struct JMDictExampleElement {
    std::string ex_srce;
    std::string ex_text;
    std::vector<std::string> ex_sent;
};
// Elemento de sentido
struct JMDictSenseElement {
    std::vector<std::string> stagk;
    std::vector<std::string> stagr;
    std::vector<std::string> pos;
    std::vector<std::string> xref;
    std::vector<std::string> ant;
    std::vector<std::string> field;
    std::vector<std::string> misc;
    std::vector<std::string> s_inf;
    std::vector<std::string> lsource;
    std::vector<std::string> dial;
    std::vector<std::string> gloss;
    std::vector<JMDictExampleElement> example;
    Language lang;
};
// Entrada del diccionario
struct JMDictEntry {
    std::string ent_seq;
    std::vector<JMDictKanjiElement> kanji;
    std::vector<JMDictReadingElement> reading;
    std::vector<JMDictSenseElement> sense;
};

/***********************************************************************
    Clase JMDict
************************************************************************/

class JMDict {
    public:
        JMDict() {}

        void setFilename(const std::string &filename) {
            filename_ = filename;
        }
        void setLangs(const std::vector<std::string> &langs) {
            langs_ = langs;
        }

        void readJMDict(const std::string &filename, Language lang, int mode) {
            setFilename(filename);
            setLang(lang);
            openXML(filename);
            parse(mode);
            xmlFreeDoc(document_);
            xmlCleanupParser();
            document_ = NULL;
        }

        JMDict(const std::string &filename, const std::vector<std::string> &langs): filename_(filename), langs_(langs) {
            openXML(filename_);
            parse();
        }
        ~JMDict() {
            if (document_ != NULL) {
                xmlFreeDoc(document_);
                xmlCleanupParser();
            }
        }
        void openXML(const std::string &filename) { 
            filename_ = filename;
            document_ = xmlReadFile(filename_.c_str(), NULL, 0);
            if (document_ == NULL) {
                std::cerr << "Error al abrir el archivo " << filename_ << std::endl;
                exit(1);
            }
            rootElement_ = xmlDocGetRootElement(document_);
            if (rootElement_ == NULL) {
                std::cerr << "Error al obtener el elemento raíz del archivo " << filename_ << std::endl;
                exit(1);
            }
        }
        void parse(int mode = 0);
        bool isLangSupported(const std::string &lang) const;
        const std::vector<JMDictEntry> &getEntry(int index) const {
            return entries_[index];
        }
        const std::vector<std::string> &getLangs() const {
            return langs_;
        }
        int searchEntry(const std::string&) const;
        JMDictEntry mergeEntry(const JMDictEntry &entry, const JMDictEntry &, int mode);
        void print() const;
        //void parseExamplesFromXML(const std::string &str);
        void setLang(Language lang) {
            langs_ = createLangsVector(lang);
        }
        const std::vector<std::vector<JMDictEntry>> &getEntries() const {
            return entries_;
        }


    private:
        std::string filename_;
        xmlDoc *document_;
        xmlNode *rootElement_;
        std::vector<std::vector<JMDictEntry>> entries_;
        std::unordered_map<std::string, int> indexMap_;
        std::vector<std::string> langs_;

        void processXML_(xmlNode *node, int mode);
        void processEntry_(xmlNode *node, JMDictEntry &entry);
        void processKanji_(xmlNode *node, JMDictEntry &entry);
        void processReading_(xmlNode *node, JMDictEntry &entry);
        void processSense_(xmlNode *node, JMDictEntry &entry);
        void processExample_(xmlNode *node, JMDictSenseElement &sense);
        void insertEntry_(const JMDictEntry &entry, int mode);
        void printEntry_(const JMDictEntry &entry) const;

};

