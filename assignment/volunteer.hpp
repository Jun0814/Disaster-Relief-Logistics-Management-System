#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "structures.hpp"
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
    int volunteerIDCounter;

public:
    VolunteerManager() : front(nullptr), rear(nullptr), volunteerIDCounter(1) 
    {
        loadVolunteersFromCSV();
    }

    ~VolunteerManager()
    {
        saveVolunteersToCSV();
        while (front != nullptr)
        {
            VNode *temp = front;
            front = front->next;
            delete temp;
        }
    }

    void loadVolunteersFromCSV()
    {
        ifstream file(fileVolunteer);
        if (!file.is_open())
        {
            cout << "Creating new volunteer file...\n";
            return;
        }

        string line;
        // Skip header line
        getline(file, line);
        
        while (getline(file, line))
        {
            stringstream ss(line);
            string id, name, contact, skill;
            
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, contact, ',');
            getline(ss, skill, ',');
            
            // Remove quotes if present
            if (name.front() == '"' && name.back() == '"')
                name = name.substr(1, name.length() - 2);
            if (contact.front() == '"' && contact.back() == '"')
                contact = contact.substr(1, contact.length() - 2);
            if (skill.front() == '"' && skill.back() == '"')
                skill = skill.substr(1, skill.length() - 2);
            
            Volunteer newVolunteer;
            newVolunteer.volunteerID = stoi(id);
            newVolunteer.name = name;
            newVolunteer.contact = contact;
            newVolunteer.skill = skill;
            
            // Add to queue
            VNode *newNode = new VNode{newVolunteer, nullptr};
            if (rear == nullptr)
            {
                front = rear = newNode;
            }
            else
            {
                rear->next = newNode;
                rear = newNode;
            }
            
            // Update counter
            if (newVolunteer.volunteerID >= volunteerIDCounter)
                volunteerIDCounter = newVolunteer.volunteerID + 1;
        }
        file.close();
    }

    void saveVolunteersToCSV()
    {
        ofstream file(fileVolunteer);
        if (!file.is_open())
        {
            cout << "Error: Cannot open volunteer file for writing.\n";
            return;
        }

        file << "VolunteerID,Name,Contact,Skill\n";
        
        // Write all volunteers in queue
        VNode *current = front;
        while (current != nullptr)
        {
            Volunteer v = current->data;
            file << v.volunteerID << ",\"" << v.name << "\",\"" << v.contact << "\",\"" << v.skill << "\"\n";
            current = current->next;
        }
        
        file.close();
    }

    // Register Volunteer
    void registerVolunteer(string name, string contact, string skill)
    {
        Volunteer newVolunteer;
        newVolunteer.volunteerID = volunteerIDCounter++;
        newVolunteer.name = name;
        newVolunteer.contact = contact;
        newVolunteer.skill = skill;

        VNode *newNode = new VNode{newVolunteer, nullptr};
        
        if (rear == nullptr)
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
        
        cout << "Volunteer registered successfully!\n";
        cout << "Volunteer ID: " << newVolunteer.volunteerID << endl;
        cout << "Name: " << newVolunteer.name << endl;
        cout << "Contact: " << newVolunteer.contact << endl;
        cout << "Skill: " << newVolunteer.skill << endl;
        
        saveVolunteersToCSV();
    }

    // Deploy Volunteer to Field
    void deployVolunteer()
    {
        if (front == nullptr)
        {
            cout << "No volunteers available for deployment.\n";
            return;
        }
        
        Volunteer v = front->data;
        VNode *temp = front;
        front = front->next;
        
        if (front == nullptr)
            rear = nullptr;
            
        delete temp;
        
        cout << "Volunteer deployed successfully!\n";
        cout << "Deployed Volunteer Details:\n";
        cout << "ID: " << v.volunteerID << endl;
        cout << "Name: " << v.name << endl;
        cout << "Contact: " << v.contact << endl;
        cout << "Skill: " << v.skill << endl;
        cout << "Deployment Time: " << getCurrentDateTime() << endl;
        
        saveVolunteersToCSV();
    }

    // View Registered Volunteers
    void viewVolunteers()
    {
        if (front == nullptr)
        {
            cout << "No volunteers registered.\n";
            return;
        }
        
        cout << "\n=== Registered Volunteers (Available for Deployment) ===" << endl;
        cout << "ID\tName\t\t\tContact\t\t\tSkill" << endl;
        cout << "----------------------------------------------------------------------" << endl;
        
        VNode *current = front;
        int count = 0;
        
        while (current != nullptr)
        {
            Volunteer v = current->data;
            cout << v.volunteerID << "\t" << v.name << "\t\t" << v.contact << "\t\t" << v.skill << endl;
            current = current->next;
            count++;
        }
        
        cout << "----------------------------------------------------------------------" << endl;
        cout << "Total volunteers available: " << count << endl;
    }

    bool isEmpty()
    {
        return front == nullptr;
    }

    int getSize()
    {
        int count = 0;
        VNode *current = front;
        while (current != nullptr)
        {
            count++;
            current = current->next;
        }
        return count;
    }
};