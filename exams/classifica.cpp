/*
* Una classe concorrente gestisce la classifica di un gioco online: ogni concorrente è identificato da un nickname e
* ha un punteggio associato. Un metodo ritorna una copia della classifica, un altro inserisce un nuovo punteggio relativo a
* un concorrente (inserendo un nuovo concorrente in classifica se non esistente o aggiornando il punteggio se maggiore di
* quello vecchio), un metodo si mette in attesa di variazioni sulla classifica.
*/

#include <iostream>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <vector>
class Classifica {
    std::mutex m;
    std::condition_variable cv;
    std::map<std::string,int> concorrenteToPunteggio;
    bool cambiamento;
    bool isFlush = false;
    std::condition_variable cvFlush;
    int attesa{0};

public:
    Classifica(): cambiamento(false){}
    void InserisciInClassifica(std::string newConcorrente, int punteggio){
        std::lock_guard lk(m);
        if(concorrenteToPunteggio.count(newConcorrente) == 0) {
            concorrenteToPunteggio.insert(std::pair<std::string, int>(newConcorrente, punteggio));
            cambiamento = true;
            cv.notify_all();
        }
        else
            if(concorrenteToPunteggio[newConcorrente] < punteggio) {
                concorrenteToPunteggio[newConcorrente] = punteggio;
                cambiamento = true;
                cv.notify_all();
            }
    }
    void AttendiCambiamento(){
            std::unique_lock lk(m);
            while(isFlush) cvFlush.wait(lk);
            attesa++;
            std::cout << "waiting..." << std::endl;
            while(!isFlush &&  !cambiamento ) cv.wait(lk);
            std::cout << "attesa terminata" << std::endl;
            if(!isFlush) {
                isFlush = true;
                cv.notify_all();
            }
            std::cout << "cambiamento " << attesa <<"\n";
            attesa--;
            if(attesa == 0){
                isFlush = false;
                cambiamento = false;
                cvFlush.notify_all();
            }
    }
    std::map<std::string,int> getClassifica(){
        std::lock_guard lk(m);
        return concorrenteToPunteggio;
    }
};
