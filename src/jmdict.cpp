#include "jmdict.hpp"


// Inicializar la biblioteca de libxml2 y cargar el archivo XML
void JMDict::parse( int mode) {
    // Inicializar la biblioteca de libxml2
    xmlInitParser();
    LIBXML_TEST_VERSION

    // Abrir el archivo XML
    document_ = xmlReadFile(filename_.c_str(), NULL, XML_PARSE_NOBLANKS|XML_PARSE_NOENT);
    if (document_ == NULL) {
        std::cerr << "Error al abrir el archivo XML" << std::endl;
        return;
    }

    // Obtener el root element
    rootElement_ = xmlDocGetRootElement(document_);
    if (rootElement_ == NULL) {
        std::cerr << "Error al obtener el root element" << std::endl;
        return;
    }

    // Procesar el root element
    processXML_(rootElement_->children, mode);
}

// Procesar el XML
void JMDict::processXML_(xmlNode *node, int mode) {
    xmlNode *cur_node = NULL;
    std::cout << filename_ <<":Procesando entradas..." << std::endl;
    for (cur_node = node; cur_node; cur_node = cur_node->next) {
        JMDictEntry entry;
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"entry") == 0) {
                processEntry_(cur_node, entry);
            }
            else {
                std::cerr << "_processXML-> Elemento desconocido: " << cur_node->name << std::endl;
            }
        }
        // Insertar la entrada en el diccionario
        insertEntry_(entry, mode);
    }
    std::cout << filename_<<":Entradas procesadas." << std::endl;
}

// Procesar una entrada del diccionario
void JMDict::processEntry_(xmlNode *node, JMDictEntry &entry) {
    xmlNode *cur_node = NULL;
    for (cur_node = node->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"k_ele") == 0) {
                processKanji_(cur_node, entry);
                //std::cout << "Procesando kanji..." << std::endl;
            }
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"r_ele") == 0) {
                processReading_(cur_node, entry);
                //std::cout << "Procesando reading..." << std::endl;
            }
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"sense") == 0) {
                processSense_(cur_node, entry);
                //std::cout << "Procesando sense..." << std::endl;
            }
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"ent_seq") == 0) {
                xmlChar *content = xmlNodeGetContent(cur_node);
                entry.ent_seq = std::string((const char *)content);
                xmlFree(content);
            }
            else {
                std::cerr << "_processEntry-> Elemento desconocido: " << cur_node->name << std::endl;
            }
            
        }
    }
}
// Procesar los elementos de kanji
void JMDict::processKanji_(xmlNode *node, JMDictEntry &entry) {
    JMDictKanjiElement kanji;
    xmlNode *cur_node = NULL;
    for (cur_node = node->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {

            //std::cout << "_processKanji-> Element Name: " << cur_node->name << std::endl;
            xmlChar *content = xmlNodeGetContent(cur_node);
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"keb") == 0) {
                kanji.keb = std::string((const char *)content);
            }
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"ke_inf") == 0) {
                kanji.ke_inf = std::string((const char *)content);
            }
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"ke_pri") == 0) {
                kanji.ke_pri = std::string((const char *)content);
            }
            xmlFree(content);
        }
        else {
            std::cerr << "_processKanji-> Elemento desconocido: " << cur_node->name << std::endl;
        }
    }
    entry.kanji.push_back(kanji);
}
// Procesar los elementos de lectura
void JMDict::processReading_(xmlNode *node, JMDictEntry &entry) {
    JMDictReadingElement reading;
    xmlNode *cur_node = NULL;
    for (cur_node = node->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            xmlChar *content = xmlNodeGetContent(cur_node);
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"reb") == 0) {
                reading.reb = std::string((const char *)content);
            }
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"re_nokanji") == 0) {
                reading.re_nokanji = std::string((const char *)content);
            }
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"re_restr") == 0) {
                reading.re_restr = std::string((const char *)content);
            }
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"re_inf") == 0) {
                reading.re_inf = std::string((const char *)content);
            }
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"re_pri") == 0) {
                reading.re_pri = std::string((const char *)content);
            }
            xmlFree(content);
        }
        else 
            std::cerr << "_processReading-> Elemento desconocido: " << cur_node->name << std::endl;
    }
    entry.reading.push_back(reading);
}
// Procesar los elementos de sentido
void JMDict::processSense_(xmlNode *node, JMDictEntry &entry) {
    JMDictSenseElement sense;
    xmlNode *cur_node = NULL;
    bool isOkay = true;
    for (cur_node = node->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            xmlChar *content = xmlNodeGetContent(cur_node); 
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"gloss") == 0) {
                if (isLangSupported((const char *)xmlNodeGetLang(cur_node))) {
                    sense.gloss.push_back(std::string((const char *)content));
                    sense.lang = getLanguage((const char *)xmlNodeGetLang(cur_node));
                }
                else 
                    isOkay = false;
            }
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"stagk") == 0) 
                sense.stagk.push_back(std::string((const char *)content));
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"stagr") == 0) 
                sense.stagr.push_back(std::string((const char *)content));
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"pos") == 0) 
                sense.pos.push_back(std::string((const char *)content));
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"xref") == 0) 
                sense.xref.push_back(std::string((const char *)content));  
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"ant") == 0) 
                sense.ant.push_back(std::string((const char *)content));
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"field") == 0) 
                sense.field.push_back(std::string((const char *)content));
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"misc") == 0) 
                sense.misc.push_back(std::string((const char *)content));
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"s_inf") == 0) 
                sense.s_inf.push_back(std::string((const char *)content));
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"lsource") == 0) 
                sense.lsource.push_back(std::string((const char *)content));
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"dial") == 0) 
                sense.dial.push_back(std::string((const char *)content));
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"example") == 0) 
                processExample_(cur_node, sense);
            else 
                std::cerr << "_processSense-> Elemento sin procesar: " << cur_node->name << std::endl;
            xmlFree(content);

        }
        else 
            std::cerr << "_processSense-> Elemento desconocido: " << cur_node->name << std::endl;
        
    }
    if (isOkay)
        entry.sense.push_back(sense);
}
// Procesar ejemplo
void JMDict::processExample_(xmlNode *node, JMDictSenseElement &sense) {
    JMDictExampleElement example;
    xmlNode *cur_node = NULL;
    for (cur_node = node->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            xmlChar *content = xmlNodeGetContent(cur_node);
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"ex_srce") == 0) 
                example.ex_srce = std::string((const char *)content);
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"ex_text") == 0) 
                example.ex_text = std::string((const char *)content);
            else if (xmlStrcmp(cur_node->name, (const xmlChar *)"ex_sent") == 0) 
                example.ex_sent.push_back(std::string((const char *)content));
            else 
                std::cerr << "_processExample-> Elemento desconocido: " << cur_node->name << std::endl;
            xmlFree(content);
        }
    }
    sense.example.push_back(example);
}



// Insertar una entrada en el diccionario
void JMDict::insertEntry_(const JMDictEntry &entry, int mode) {
    std::string key;
    //std::cout << "pre searchEntry" << std::endl;
    if (entry.kanji.size() > 0) {
        key = entry.kanji[0].keb;
    }
    else {
        key = entry.reading[0].reb;
    }

    if (key == "ケチ")  {
        std::cout << "found" << std::endl;
        printEntry_(entry);
        std::cout << std::endl;
    }
    //std::cout << "post searchEntry" << std::endl;
    int index = searchEntry(key);
    //std::cout << "search entry" << std::endl;
    if (mode == MODE_JMDICT_CREATE) {   
        if (index != -1) {
            entries_[index].push_back(entry);
        }
        else {
            std::vector<JMDictEntry> entries;
            entries.push_back(entry);
            entries_.push_back(entries);
            indexMap_[key] = entries_.size() - 1;
        }
    } else {
        bool exit = false;
        for (auto &e: entries_[index]) {
            if (e.ent_seq == entry.ent_seq){
                for (auto &eSense: e.sense) {
                    for (auto &sense: entry.sense) {
                        if (eSense.gloss[0] == sense.gloss[0]) {
                            for (auto &ex: sense.example) {
                                eSense.example.push_back(ex);
                            }
                            exit = true;
                            break;
                        }
                    }
                    if (exit) {
                        break;
                    }
                } 
            }
            if (exit) {
                break;
            }
        }
    }
        //std::cout << "merged" << std::endl;
}

// Imprimir una entrada
void JMDict::printEntry_(const JMDictEntry &entry) const {
    for (auto kanji : entry.kanji) {
        std::cout << "\tKanji: " << kanji.keb << std::endl;
    }

    for (auto reading : entry.reading) {
        std::cout << "\tReading: " << reading.reb << std::endl;
    }

    for (auto sense : entry.sense) {
        for (auto gloss : sense.gloss) {
            std::cout << "\tGloss: " << gloss << std::endl;
        }
        for (auto example : sense.example) {
            std::cout << "\tExample: " << example.ex_text << std::endl;
        }
    }
}

// Verificar si un idioma es soportado
bool JMDict::isLangSupported(const std::string& lang) const {
    for (auto l : langs_) {
        if (l == lang) return true;
    }
    return false;
}

// Buscar una entrada. Devuelve índice de entries_, -1 si no existe.
int JMDict::searchEntry(const std::string& key) const {
    if (indexMap_.empty()) {
        return -1;
    }
    auto it = indexMap_.find(key);
    if (it != indexMap_.end()) {
        return it->second;
    } 
    return -1;
}

// Imprimir las entradas del diccionario
void JMDict::print() const {
    //for (auto entry : entries_) 
        //printEntry_(entry);
}

