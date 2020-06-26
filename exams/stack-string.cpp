
/*
 * Un'applicazione Win32 presenta più thread che devono operare in modo concorrente su uno stack di stringhe unicode.
 * Si implementi una classe C++ che incapsuli il comportamento di tale stack in modalità thread-safe.
 * Deve essere permesso l'inserimento di nuove stringhe nello stack e l'estrazione dell'ultima stringa inserita
 * (notificando il caso di stack vuoto). Porre particolare attenzione alla gestione della memoria e della concorrenza.
 */


#include <iostream>
#include <stack>
#include <string>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <thread>
#include <vector>
#define N 10
class stack_string{

    std::mutex ms;
    std::stack<std::string> stackString;
    std::condition_variable cv;
public:
    stack_string(){}

    void pushString(std::string s){
        std::unique_lock lk(ms);
        stackString.push(s);
        std::cout << "push string" << s << std::endl;
    }

    std::optional<std::string > popString(){

        std::unique_lock lk(ms);
        if(stackString.empty()) return std::nullopt;
        std::string s = stackString.top();
        stackString.pop();
        std::cout << "pop string" << s << std::endl;
        return s;
    }
};
