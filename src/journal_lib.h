#include <string>
#include <fstream>
enum class Level {
    LOW,
    MEDIUM,
    HIGH,
    CRITICAL, 
    UNKNOWN
};
std::string enumToString(Level level);
Level stringToEnum(const std::string& s);
class Journal{
private:
    std::string name;
    Level journalLevel;
    std::ofstream fout;

public:
    Journal(const std::string& journalName, Level journalLevel);
    Journal(const std::string& journalName);
    std::ofstream createJournalFile();
   
    
    void changeLevel(Level newLevel);   
    void addNewMessage(Level messageLevel, const std::string& message);//файл должен быть уже открыт, принимаем поток записи
    void addNewMessage(const std::string& message);


    ~Journal() {
        if (fout.is_open()) {
            fout.close();
        }
    }

};
