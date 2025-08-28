#include <iostream>
#include "journal_lib.h"
#include <thread>
#include <queue>

class wrQueue{//общая очередь для чтения и записи
private:
    std::queue <std::pair<std::string, std::string>> queue;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> stop_flag{false};
public:
     void push(std::pair<std::string, std::string>  pair){
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(pair);
        cv.notify_one();
    } 

    bool pop(std::pair <std::string, std::string> &pair){
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !queue.empty() || stop_flag; });
        if (stop_flag && queue.empty()) {
            return false;
        }
        if (!queue.empty()) {
            pair = queue.front();
            queue.pop();
            return true;
        }
        return false;
    }

     void stop() {
        std::lock_guard<std::mutex> lock(mtx);
        stop_flag = true;
        cv.notify_all();
    }
    
    bool is_stopped() const {
        return stop_flag;
    }

};

void reader_thread(wrQueue& queue, Journal & journal) {
    std::string line;
    std::cout << "Введите строку (уровень + сообщение или только сообщение):" << std::endl;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string firstWord;
        iss >> firstWord;
        
        std::string message;
        std::string level;

        if (firstWord == "-exit") {
            queue.stop();
            break;
        }
        if(firstWord == "-change"){
            std::getline(iss, message);
            if (!message.empty() && message.front() == ' ') {
            message.erase(0, message.find_first_not_of(' '));
            journal.changeLevel(stringToEnum(message));
        }
        }
        else if (stringToEnum(firstWord)!=Level::UNKNOWN) {
            level = firstWord;
            std::getline(iss, message);
            if (!message.empty() && message.front() == ' ') {
            message.erase(0, message.find_first_not_of(' '));
            }
            queue.push(make_pair(level, message));
        } else {
            message = firstWord;
            std::string rest;
            std::getline(iss, rest);
            message += rest;
            queue.push(make_pair("", message));
        }
    }
}

void writer_thread(wrQueue& queue, Journal & journal){
    std::pair <std::string, std::string> pair;
    while (true) {
        if (queue.pop(pair)) {
            if(!pair.first.empty()){
                journal.addNewMessage(stringToEnum(pair.first),pair.second);
            }else{
                journal.addNewMessage(pair.second);
            }
        } else if (queue.is_stopped()) {
            break;
        }
    }
}


int main(int argc, char *argv[]){
    if(argc<2){
        return -1;
    }
    std::string journalName;
    journalName=argv[1];

    
    // Создание журнала с уровнем по умолчанию
    Journal journal(journalName);
        
    // Если указан уровень, изменяем его
    if (argc == 3) {
        Level customLevel = stringToEnum(argv[2]);
        journal.changeLevel(customLevel);
    }
    wrQueue queue;
    
    //поток для чтения сообщений
    std::thread reader(reader_thread, std::ref(queue), std::ref(journal));

    //поток дл записи сообщений
    std::thread writer(writer_thread, std::ref(queue), std::ref(journal));
    
    reader.join();
    writer.join();

}