//
// Created by kandabi on 1/7/20.
//

#include "../include/Header.h"

std::string Header::toString() {
    return name+":"+value;
}

Header::Header(std::string name, std::string value):name(),value() {
    this->name=name;
    this->value=value;
}