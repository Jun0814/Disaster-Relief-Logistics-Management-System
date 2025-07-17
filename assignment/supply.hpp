#include <iostream>
#include <string>
using namespace std;

struct Node
{
    SupplyBox data;
    Node *next;
};

class SupplyManager
{
private:
    Node *top;

public:
    SupplyManager()
    {
        top = nullptr;
    }

    ~SupplyManager()
    {
        while (top != nullptr)
        {
            Node *temp = top;
            top = top->next;
            delete temp;
        }
    }

    void packSupplyBox(int id, string type, int quantity)
    {
        Node *newNode = new Node{{id, type, quantity}, top};
        top = newNode;
        cout << "Supply box packed.\n";
    }

    void sendSupplyPackage()
    {
        if (top == nullptr)
        {
            cout << "No supply boxes to dispatch.\n";
            return;
        }
        SupplyBox box = top->data;
        Node *temp = top;
        top = top->next;
        delete temp;
        cout << "Dispatched box ID: " << box
             << ", Type: " << box.type
             << ", Quantity: " << box.quantity << "\n";
    }

    void viewPackedSupplies()
    {
        if (top == nullptr)
        {
            cout << "No supplies packed.\n";
            return;
        }
        cout << "Packed Supplies:\n";
        Node *current = top;
        while (current != nullptr)
        {
            SupplyBox box = current->data;
            cout << "ID: " << box
                 << ", Type: " << box.type
                 << ", Quantity: " << box.quantity << "\n";
            current = current->next;
        }
    }
};
