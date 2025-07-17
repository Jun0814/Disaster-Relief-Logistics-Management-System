#include <iostream>
#include <string>
using namespace std;

struct TNode
{
    Vehicle data;
    TNode *next;
};

class TransportScheduler
{

private:
    TNode *tail;

public:
    TransportScheduler() : tail(nullptr) {}

    ~TransportScheduler()
    {
        if (!tail)
            return;
        TNode *current = tail->next;
        TNode *nextNode;
        do
        {
            nextNode = current->next;
            delete current;
            current = nextNode;
        } while (current != tail->next);
    }

    void addVehicle(int id, string type)
    {
        TNode *newNode = new TNode{{id, type}, nullptr};
        if (!tail)
        {
            tail = newNode;
            tail->next = tail;
        }
        else
        {
            newNode->next = tail->next;
            tail->next = newNode;
            tail = newNode;
        }
        cout << "Vehicle added to schedule.\n";
    }

    void rotateShift()
    {
        if (tail)
        {
            tail = tail->next;
            cout << "Shift rotated.\n";
        }
        else
        {
            cout << "No vehicles to rotate.\n";
        }
    }

    void viewSchedule()
    {
        if (!tail)
        {
            cout << "No vehicles in schedule.\n";
            return;
        }
        cout << "Current Vehicle Schedule:\n";
        TNode *current = tail->next;
        do
        {
            current = current->next;
        } while (current != tail->next);
    }
};
