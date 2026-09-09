#pragma once
#include <iostream>
#include <string>

// Movie Class: Represents a movie playing in the cinema
// OOP Concept: Encapsulation (private fields accessed via public getters)
class Movie {
private:
    int id;
    std::string title;
    std::string language;
    int durationMins;

public:
    // OOP Concept: this Keyword (distinguishing parameter names from instance variables)
    Movie(int id, std::string title, std::string language, int durationMins) {
        this->id = id;
        this->title = title;
        this->language = language;
        this->durationMins = durationMins;
    }

    int getId() const { return this->id; }
    std::string getTitle() const { return this->title; }
    std::string getLanguage() const { return this->language; }
    int getDurationMins() const { return this->durationMins; }

    void displayInfo() const {
        std::cout << "[" << id << "] " << title << " | " << language << " | " << durationMins << " min\n";
    }
};
