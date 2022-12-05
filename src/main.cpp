#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <mutex>
#include <thread>

using namespace std;

template<class T>
struct Node
{
    Node(T x)
    {
        value = x;
        next = 0;
    }
    T value;
    Node<T>* next;
    mutex candado;
};

template<class T>
struct Lista
{
    Lista()
    {
        head = new Node<T>(-1);
        tail = new Node<T>(-2);
        head->next = tail;
        nelem = 0;

    }
    bool Add(T x)
    {
        while (true) {

            Node<T>* n = new Node<T>(x);
            Node<T>* predecesor = head;
            Node<T>* actual = head->next;


            predecesor->candado.lock();
            actual->candado.lock();

            while (actual->value != -2 && actual->value < x)
            {
                predecesor->candado.unlock();
                actual->candado.unlock();

                predecesor = actual;
                actual = actual->next;

                predecesor->candado.lock();
                actual->candado.lock();
            }

            if (actual->value == x)
            {
                predecesor->candado.unlock();
                actual->candado.unlock();
                return false;
            }

            predecesor->next = n;
            n->next = actual;
            nelem++;
            predecesor->candado.unlock();
            actual->candado.unlock();
            
            return true;
        }

    }
 
    bool borrar(T x)
    {
        while (true) {

            Node<T>* predecesor = head;
            Node<T>* actual = head->next;


            predecesor->candado.lock();
            actual->candado.lock();

            while (actual->value != -2 && actual->value < x)
            {
                predecesor->candado.unlock();
                actual->candado.unlock();

                predecesor = actual;
                actual = actual->next;

                predecesor->candado.lock();
                actual->candado.lock();
            }

            if (actual->value != x)
            {
                predecesor->candado.unlock();
                actual->candado.unlock();
                return false;
            }

            predecesor->next = actual->next;
            
            nelem--;
            predecesor->candado.unlock();
            actual->candado.unlock();
            delete actual;
            return true;
        }

    }

    void print()
    {
        Node<T>* aux1;
        aux1 = head->next;
        cout << "head";
        while (aux1->value != -2)
        {
            cout << "->" << aux1->value;
            aux1 = aux1->next;
        }
        cout << endl;
    }
    
    Node<T>* n;
    Node<T>* head;
    Node<T>* tail;

    

    int nelem;
};

template<class T>
struct ADD
{

    Lista<T>* list_;
    int min_, max_;
    ADD(Lista<T>& list, int min, int max)
    {
        list_ = &list;
        min_ = min;
        max_ = max;
    }

    void operator()(int n_operations)
    {
        for (int x = 0; x < n_operations; x++)
        {
            int number = min_ + rand() % (max_);
            cout << "voy a añadir: " << number << endl;
            list_->Add(number);
        }
    }
};

template<class T>
struct BORRAR
{

    Lista<T>* list_;
    int min_, max_;
    BORRAR(Lista<T>& list, int min, int max)
    {
        list_ = &list;
        min_ = min;
        max_ = max;
    }

    void operator()(int n_operations)
    {
        for (int x = 0; x < n_operations; x++)
        {
            int number = min_ + rand() % (max_);
            cout << "voy a borrar: " << number << endl;
            list_->borrar(number);
        }
    }
};


int main()
{
    srand(time(NULL));
    thread* threads[2];
    Lista<int> A;
    ADD<int> Add1(A, 1, 100);
    BORRAR<int> borrar1(A, 1, 100);

    thread first(Add1, 100);
    thread second(borrar1, 80);

    threads[0] = &first;
    threads[1] = &second;

    threads[0]->join();
    threads[1]->join();

    A.print();



}
