#include "util.hpp"

bool inSubstr(const std::string& str, const std::string& substr) {
    return str.find(substr) != std::string::npos;
}

bool isEmpty(const std::string str) {
    if (str.length() == 0){
        return true;
    }

    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] != ' ' || str[i] != '\n' || str[i] != '\t') {
            return false;
        }
    }
    return true;
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
    if (inSubstr(text, "ichi2")) {
        return true;
    }
    if (inSubstr(text, "news2")) {
        return true;
    }
    if (inSubstr(text, "spec2")) {
        return true;
    }
    if (inSubstr(text, "gai2")) {
        return true;
    }

    if (inSubstr(text, "nf")) {

        for (int i = 0; i <= 48; i++) {
            if (inSubstr(text, "nf" + std::to_string(i))) {
                return true;
            }
        }
    }
    return false;
}
