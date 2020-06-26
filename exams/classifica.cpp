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
#include <atomic>
#include <vector>
class Classifica {
    std::mutex m;
    std::condition_variable cv;
    std::map<std::string,int> concorrenteToPunteggio;
    bool cambiamento;
    

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
    std::atomic<int> attesa{0};
    void AttendiCambiamento(){

            std::unique_lock lk(m);
            attesa.fetch_add(1);

            cv.wait(lk, [this]() { return cambiamento; });

            attesa.fetch_sub(1);
            std::cout << "cambiamento " << attesa <<"\n";
            if (attesa.load() == 0) {
                std::cout << "ultimo ";
                cambiamento = false;
            }

    }

    std::map<std::string,int> getClassifica(){
        std::lock_guard lk(m);
        return concorrenteToPunteggio;
    }

};
