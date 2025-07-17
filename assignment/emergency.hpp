#include <iostream>
#include <string>
#include "structures.hpp"
using namespace std;

struct ENode
{
    EmergencyRequest data;
    ENode *next;
};

class EmergencyCoordinator
{
private:
    ENode *head;
    int incrementID;

public:
    EmergencyCoordinator() : head(nullptr), incrementID(1) {}

    ~EmergencyCoordinator()
    {
        while (head != nullptr)
        {
            ENode *temp = head;
            head = head->next;
            delete temp;
        }
    }

    void readEmergencyRequests()
    {
        ifstream file(fileEmergencyRequest);
        if (!file)
        {
            cerr << "Error opening file: " << fileEmergencyRequest << endl;
            return;
        }
        ENode *current = head;
        while (current)
        {
            EmergencyRequest req;
            file >> req.requestID >> req.location >> req.type >> req.urgency >> req.status;
            ENode *newNode = new ENode{req, nullptr};
            if (!head)
            {
                head = newNode;
            }
            else
            {
                current->next = newNode;
            }
            current = newNode;
        }
    }

    // Log a new emergency request
    void logRequest(string location, string type, int urgency, string status)
    {
        ENode *newNode = new ENode{{incrementID++, location, type, urgency, status}, nullptr};

        if (!head || urgency > head->data.urgency)
        {
            newNode->next = head;
            head = newNode;
        }
        else
        {
            ENode *current = head;
            while (current->next && current->next->data.urgency >= urgency)
            {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        cout << "Emergency request logged with ID: " << incrementID - 1 << "\n";
    }

    void requestSupplyPackage()
    {
        if (!head)
        {
            cout << "No pending emergency requests.\n";
            return;
        }
        EmergencyRequest req = head->data;
        SupplyRequest supplyReq = {0, 0, req.requestID, getCurrentDateTime(), "pending"};

        cout << "Supply package requested for Emergency ID: " << req.requestID
             << ", Location: " << req.location
             << ", Type: " << req.type
             << ", Urgency: " << req.urgency
             << ", Status: " << supplyReq.status << "\n";
    }

    void viewPendingRequests()
    {
        if (!head)
        {
            cout << "No pending requests.\n";
            return;
        }
        cout << "Pending Requests:\n";
        ENode *current = head;
        cout << "No. | ID | Location | Type | Urgency | Status\n";
        int count = 0;
        while (current)
        {
            count++;
            EmergencyRequest r = current->data;
            cout << count << ".     " << r.requestID << " | " << r.location << " | " << r.type << " | " << r.urgency << " | " << r.status << "\n";
            current = current->next;
        }
    }
};