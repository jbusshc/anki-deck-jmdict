#pragma once

#include <string>
#include <vector>

// Idiomas
enum Language {
    ENGLISH = 1,
    SPANISH = 2,
    PORTUGUESE = 4,
    FRENCH = 8,
    GERMAN = 16,
    RUSSIAN = 32,
    CHINESE = 64,
};

Language operator|(Language a, Language b);

// Obtener el idioma
Language getLanguage(std::string lang);

// Crear un vector de idiomas
std::vector<std::string> createLangsVector(Language lang);