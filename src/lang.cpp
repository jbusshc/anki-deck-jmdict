#include "lang.hpp"

Language operator|(Language a, Language b) {
    return static_cast<Language>(static_cast<int>(a) | static_cast<int>(b));
}

// Obtener el idioma
Language getLanguage(std::string lang) {
    if (lang == "eng") return Language::ENGLISH;
    if (lang == "spa") return Language::SPANISH;
    if (lang == "por") return Language::PORTUGUESE;
    if (lang == "fra") return Language::FRENCH;
    if (lang == "deu") return Language::GERMAN;
    if (lang == "rus") return Language::RUSSIAN;
    if (lang == "chi") return Language::CHINESE;
    return Language::ENGLISH;
}
// Crear un vector de idiomas
std::vector<std::string> createLangsVector(Language lang) {
    std::vector<std::string> languages;
    if (lang & Language::ENGLISH)
        languages.push_back("eng");
    if (lang & Language::SPANISH)
        languages.push_back("spa");
    if (lang & Language::PORTUGUESE)
        languages.push_back("por");
    if (lang & Language::FRENCH)
        languages.push_back("fre");
    if (lang & Language::GERMAN)
        languages.push_back("ger");
    if (lang & Language::RUSSIAN)
        languages.push_back("rus");
    if (lang & Language::CHINESE)
        languages.push_back("chi");
    return languages;
}

