#include <iostream>
#include <string>
using namespace std;

struct VNode
{
    Volunteer data;
    VNode *next;
};

class VolunteerManager
{
private:
    VNode *front;
    VNode *rear;

public:
    VolunteerManager() : front(nullptr), rear(nullptr) {}

    ~VolunteerManager()
    {
        while (front != nullptr)
        {
            VNode *temp = front;
            front = front->next;
            delete temp;
        }
    }

    void registerVolunteer(string name, string contact, string skill)
    {
        VNode *newNode = new VNode{{name, contact, skill}, nullptr};
        if (rear == nullptr)
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
        cout << "Volunteer registered.\n";
    }

    void deployVolunteer()
    {
        if (front == nullptr)
        {
            cout << "No volunteers to deploy.\n";
            return;
        }
        Volunteer v = front->data;
        VNode *temp = front;
        front = front->next;
        if (front == nullptr)
            rear = nullptr;
        delete temp;
        cout << "Deployed: " << v.name << " (" << v.skill << ")\n";
    }

    void viewVolunteers()
    {
        if (front == nullptr)
        {
            cout << "No volunteers available.\n";
            return;
        }
        cout << "Registered Volunteers:\n";
        VNode *current = front;
        while (current != nullptr)
        {
            Volunteer v = current->data;
            cout << "- " << v.name << " | " << v.contact << " | " << v.skill << "\n";
            current = current->next;
        }
    }
};