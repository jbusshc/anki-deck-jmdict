#ifndef KANJI_HPP
#define KANJI_HPP

#include <string>
#include <vector>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>

// Estructura KanjiEntry para almacenar los datos de un kanji de la base de datos KanjiDic2
struct KanjiEntry {
    std::string literal; // Kanji literal character
    std::vector<std::pair<std::string, std::string>> codepoints; // cp_type, cp_value
    std::vector<std::pair<std::string, std::string>> radicals; // rad_type, rad_value

    struct Misc {
        int grade; // Grade level of the Kanji
        std::vector<int> stroke_counts; // Stroke counts
        std::vector<std::pair<std::string, std::string>> variants; // var_type, variant
        int freq; // Frequency of the Kanji
        std::vector<std::string> rad_names; // Radical names
        int jlpt; // JLPT level of the Kanji
    } misc;

    struct DicNumber {
        std::string dr_type; // Type of dictionary reference
        std::string dic_ref; // Dictionary reference
        int m_vol; // Volume number in the dictionary
        int m_page; // Page number in the dictionary
    };
    std::vector<DicNumber> dic_numbers; // Dictionary reference numbers

    struct QueryCode {
        std::string qc_type; // Type of query code
        std::string q_code; // Query code
        std::string skip_misclass; // SKIP misclassification
    };
    std::vector<QueryCode> query_codes; // Query codes

    struct ReadingMeaning {
        struct RMGroup {
            struct Reading {
                std::string r_type; // Type of reading
                std::string reading; // Reading value
                std::string on_type; // On reading type
                std::string r_status; // Reading status
            };
            std::vector<Reading> readings; // Readings

            struct Meaning {
                std::string m_lang; // Language of the meaning
                std::string meaning; // Meaning value
            };
            std::vector<Meaning> meanings; // Meanings
        };
        std::vector<RMGroup> rmgroups; // Reading and meaning groups
        std::vector<std::string> nanori; // Nanori readings
    } reading_meaning;
};

// Clase KanjiDic2
class KanjiDic2 {
    public:
    

};

#endif // KANJI_HPP