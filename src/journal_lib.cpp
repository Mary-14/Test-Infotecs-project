#include "journal_lib.h"
#include <iostream>
#include <time.h>

std::string enumToString(Level level) {
    switch (level) {
        case Level::LOW: return "LOW";
        case Level::MEDIUM: return "MEDIUM";
        case Level::HIGH: return "HIGH";
        case Level::CRITICAL: return "CRITICAL";
        default: return "";
    }
}
    
Level stringToEnum(const std::string& s) {
    if (s == "LOW") return Level::LOW;
    if (s == "MEDIUM") return Level::MEDIUM;
    if (s == "HIGH") return Level::HIGH;
    if (s == "CRITICAL") return Level::CRITICAL;
    return Level::UNKNOWN;
}

 //создание журнала
Journal::Journal(const std::string& journalName, Level journalLevel): 
    name(journalName), journalLevel(journalLevel)  {
        fout.open(journalName, std::ios::out | std::ios::app);
        if (!fout.is_open()) {
            std::cout<<"Ошибка при открытии файла "<< journalName<<std::endl;
            exit(1);
    }
}

Journal::Journal(const std::string& journalName): Journal(journalName, Level::LOW)  {
}

//добавление сообщения в файл
void Journal::addNewMessage(Level messageLevel, const std::string& message){
    if (static_cast<int>(messageLevel) < static_cast<int>(journalLevel)) { // Не записываем сообщения с уровнем ниже установленного
        return; 
    }
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y.%m.%d %H:%M:%S", now);
    std::string timeStr(buffer);
    if(fout.is_open()){
        fout<<timeStr<<" "<<(messageLevel==Level::UNKNOWN?enumToString(journalLevel):enumToString(messageLevel))<<" "<<message <<std::endl;
    }
}

void Journal::addNewMessage(const std::string& message){
    time_t mytime = time(NULL);
    struct tm *now = localtime(&mytime);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y.%m.%d %H:%M:%S", now);
    std::string timeStr(buffer);
    if(fout.is_open()){
        fout<<timeStr<<" "<<enumToString(journalLevel)<<" "<<message <<std::endl;
    }
}

void Journal::changeLevel(Level newLevel){
    if(newLevel!=Level::UNKNOWN){
        journalLevel=newLevel;
    }
}
    



