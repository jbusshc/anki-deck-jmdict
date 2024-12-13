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
                // Do nothing
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
    //std::cout << "post searchEntry" << std::endl;
    int index = searchEntry(key);
    //std::cout << "search entry" << std::endl;
    if (index != -1) {
        entries_[index] = mergeEntry(entries_[index], entry, mode);
    }
    else {
        entries_.push_back(entry);
        indexMap_[key] = entries_.size() - 1;
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

// Fusionar dos entradas distintas. Devuelve la entrada fusionada.
JMDictEntry JMDict::mergeEntry(const JMDictEntry &entry, const JMDictEntry &newEntry, int mode) {
    JMDictEntry mergedEntry;
    bool exit = false;
    if (mode == 0) {
        for (auto kanji : entry.kanji) {
            mergedEntry.kanji.push_back(kanji);
        }
        for (auto kanji : newEntry.kanji) {
            for (auto k : mergedEntry.kanji) {
                if (k.keb == kanji.keb) {
                    exit = true;
                    break;
                }
            }
            if (exit) {
                exit = false;
                continue;
            }
            if (mergedEntry.kanji.size() > 0) {
                mergedEntry.kanji.push_back(kanji);
            }
        }
        for (auto reading : entry.reading) {
            mergedEntry.reading.push_back(reading);
        }
        for (auto reading : newEntry.reading) {
            for (auto r : mergedEntry.reading) {
                if (r.reb == reading.reb) {
                    exit = true;
                    break;
                }
            }
            if (exit) {
                exit = false;
                continue;
            }
            if (mergedEntry.reading.size() > 0) {
                mergedEntry.reading.push_back(reading);
            }
        }
        for (auto sense : entry.sense) {
            mergedEntry.sense.push_back(sense);
        }
        for (auto sense : newEntry.sense) {
            for (auto s : mergedEntry.sense) {
                if (s.gloss == sense.gloss) {
                    exit = true;
                    break;
                }
            }
            if (exit) {
                exit = false;
                continue;
            }
            if (mergedEntry.sense.size() > 0) {
                mergedEntry.sense.push_back(sense);
            }
        }
    }
    else {
        for (auto kanji : entry.kanji) {
            mergedEntry.kanji.push_back(kanji);
        }
        for (auto reading : entry.reading) {
            mergedEntry.reading.push_back(reading);
        }
        for (auto sense : entry.sense) {
            mergedEntry.sense.push_back(sense);
        }
        ///*
        for (auto newSense: newEntry.sense) {
            for (auto example : newSense.example) {
                if (mergedEntry.sense.size() > 0) {
                    mergedEntry.sense[0].example.push_back(example);
                }
                else {
                    mergedEntry.sense.push_back(newSense);
                }
            }
        }
        //*/

    }

    return mergedEntry;
}

// Imprimir las entradas del diccionario
void JMDict::print() const {
    for (auto entry : entries_) 
        printEntry_(entry);
}

void JMDict::generateTxt(const std::string &filename) const {
    std::ofstream file (filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo " << filename << std::endl;
        return;
    }
    //int n = 0;
    file << "Kanji; Reading; Gloss\n";
    for (auto entry : entries_) {
        if (entry.kanji.size() > 0) {
            std::string kanji;
            std::string reading;
            std::string gloss;
            /*
            for (auto k : entry.kanji) {
                kanji = k.keb;
                for(auto r : entry.reading) {
                    reading = r.reb;
                    for (auto s : entry.sense) {
                        if (s.lang == Language::ENGLISH) {
                            if (s.gloss.size() > 0) {
                                gloss = s.gloss[0];
                                file << kanji << "; " << reading << "; " << gloss << "\n";
                            }
                            else {
                                std::cout << "No hay gloss en " << kanji << " " << reading << std::endl;
                            }
                        }
                    }
                }
            }*/
            /* 
            for (auto k : entry.kanji) {
                kanji = k.keb;
                for(auto r : entry.reading) {
                    reading = r.reb;
                    for (auto s : entry.sense) {
                        if (s.lang == Language::ENGLISH) {
                            for (auto g : s.gloss) {
                                gloss = g;
                                file << kanji << "; " << reading << "; " << gloss << "\n";
                            }
                        }
                    }
                }
            }
            */
            //file << entry.kanji[0].keb << "; " << entry.reading[0].reb << "; " << entry.sense[0].gloss[0] << "\n";

            //para todos los kanjis y readings pero solo un sense.
            
            for (auto k : entry.kanji) {
                kanji = k.keb;
                for(auto r : entry.reading) {
                    reading = r.reb;
                    for (auto s : entry.sense) {
                        if (s.lang == Language::ENGLISH) {
                            if (s.gloss.size() > 0) {
                                gloss = s.gloss[0];
                                file << kanji << "; " << reading << "; " << gloss << "\n";
                                break;
                            }
                        }
                    }
                }
            }
        }
        else {
            /*
            for (auto r : entry.reading) {
                std::string reading = r.reb;
                for (auto s : entry.sense) {
                    if (s.lang == Language::ENGLISH) {
                        if (s.gloss.size() > 0) {
                            std::string gloss = s.gloss[0];
                            file << " ; " << reading << "; " << gloss << "\n";
                        }
                    }
                }
            }
            */
           // todos los readings pero solo un sense
            for (auto r : entry.reading) {
                std::string reading = r.reb;
                for (auto s : entry.sense) {
                    if (s.lang == Language::ENGLISH) {
                        if (s.gloss.size() > 0) {
                            std::string gloss = s.gloss[0];
                            file << " ; " << reading << "; " << gloss << "\n";
                            break;
                        }
                    }
                }
            }
        }
    }
}