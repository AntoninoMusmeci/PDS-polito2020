/* Scrivere un'applicazione Stack<T> che gestisca l'inserimento/prelievo di elementi <T> con un metodo
* "push(T elemento)" e "T pop()". Deve essere presente un metodo "int grandezza()" che restituisca il numero di elementi
* inseriti. Nel caso lo stack sia pieno, il thread di inserimento si deve bloccare, così come si deve bloccare il thread di
* prelievo se lo stack è vuoto. 
*/

template <class T>
class StackThreadSafe{

    int maxElement;
    std::mutex m;
    std::condition_variable cv_push;
    std::condition_variable cv_pop;
    std::stack<T> stackT;

public:
    StackThreadSafe(int maxElement): maxElement(maxElement){}

    void push(T element){
        std::unique_lock lk(m);
        if(stackT.size() == maxElement) {
            cv_push.wait(lk, [this]() { return stackT.size() != maxElement; });
        }
        std::string s = "push" + std::to_string(element) + '\n';
        std::cout << s;
        stackT.push(element);
        cv_pop.notify_one();
    }

    T pop(){
        std::unique_lock lk(m);
        if(stackT.empty())
            cv_pop.wait(lk, [this](){ return !stackT.empty();});
        auto element = stackT.top();
        stackT.pop();
        std::string s = "pop" + std::to_string(element) + '\n';
        std:: cout << s;
        cv_push.notify_one();
        return element;
    }

    int grandezza(){
        std::shared_guard lk(m);
        return stackT.size();
    }

};
