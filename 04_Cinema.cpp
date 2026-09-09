#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "03_Screen.cpp"

// Cinema Class: Represents the theatre venue
// OOP Concept: Composition (Cinema contains and owns a collection of Screen objects)
class Cinema {
private:
    std::string name;
    std::string location;
    std::vector<Screen> screens;

public:
    Cinema(std::string name, std::string location) {
        this->name = name;
        this->location = location;
    }

    std::string getName() const { return this->name; }
    std::string getLocation() const { return this->location; }
    const std::vector<Screen>& getScreens() const { return this->screens; }

    void addScreen(const Screen& screen) {
        this->screens.push_back(screen);
    }

    Screen* getScreenByNumber(int screenNum) {
        for (auto& sc : screens) {
            if (sc.getScreenNumber() == screenNum) {
                return &sc;
            }
        }
        return nullptr;
    }
};
