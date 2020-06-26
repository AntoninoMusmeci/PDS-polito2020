/*
*Tema: La classe generica Exchanger<T> permette a due thread di scambiarsi un valore di tipo T. Essa offre il metodo T
*exchange(T t) che blocca il thread corrente fino a che un altro thread non invoca lo stesso metodo sulla stessa istanza.
*Quando questo avviene, il metodo restituisce l'oggetto passato come parametro dal thread opposto.
*Si implementi la classe C++, usando la libreria standard C++11.
*/

#include <iostream>
#include <mutex>
#include <condition_variable>
template <class T>
class Exchanger {

    std::mutex m;
    std::condition_variable  cv;
    bool secondThread = false;
    T element{};
public:
    T exchange(T t){
        std::unique_lock lk(m);
        if(!secondThread) {
            element = t;
            secondThread = true;
            cv.wait(lk);
            return element;
        }
        if(secondThread) {
            std::swap(t,element);
            secondThread = true;
            cv.notify_one();
            return t;
        }
    }
};

