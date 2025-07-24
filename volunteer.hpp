#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "structures.hpp"
using namespace std;

struct VNode
{
    Volunteer data;
    VNode* next;
};

class VolunteerManager
{
private:
    VNode* front;
    VNode* rear;
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
            VNode* temp = front;
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
        getline(file, line);

        while (getline(file, line))
        {
            stringstream ss(line);
            string id, name, contact, skill, status;
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, contact, ',');
            getline(ss, skill, ',');
            getline(ss, status, ',');

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
            newVolunteer.status = status;

            // Add to queue
            VNode* newNode = new VNode{ newVolunteer, nullptr };
            if (rear == nullptr)
            {
                front = rear = newNode;
            }
            else
            {
                rear->next = newNode;
                rear = newNode;
            }

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

        file << "VolunteerID,Name,Contact,Skill,Status\n";

        // Write all volunteers in queue
        VNode* current = front;
        while (current != nullptr)
        {
            Volunteer v = current->data;
            file << v.volunteerID << "," << v.name << "," << v.contact << "," << v.skill << "," << v.status << "\n";
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
        newVolunteer.status = "Available";
        VNode* newNode = new VNode{ newVolunteer, nullptr };

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

    void viewPendingVolunteerRequests() {
        ifstream file(fileVolunteerRequest);
        string line;
        getline(file, line);
        bool found = false;
        cout << "\n----------------------------------- Pending Volunteer Requests -----------------------------------\n";
        cout << left
            << setw(18) << "VolunteerReqID"
            << setw(12) << "Quantity"
            << setw(25) << "Comment"
            << setw(25) << "Date"
            << setw(12) << "Status" << endl;
        cout << "--------------------------------------------------------------------------------------------------\n";
        while (getline(file, line)) {
            stringstream ss(line);
            string volunteerReqID, reqID, quantity, comment, date, status;
            getline(ss, volunteerReqID, ',');
            getline(ss, reqID, ',');
            getline(ss, quantity, ',');
            getline(ss, comment, ',');
            getline(ss, date, ',');
            getline(ss, status, ',');
            if (status == "Pending") {
                cout << left
                    << setw(18) << volunteerReqID
                    << setw(12) << quantity
                    << setw(25) << comment
                    << setw(25) << date
                    << setw(10) << status << endl;
                found = true;
            }
        }
        if (!found) cout << "No pending volunteer requests.\n";
        cout << "--------------------------------------------------------------------------------------------------\n";
        file.close();
    }

    // View Registered Volunteers
    void viewVolunteers()
    {
        if (front == nullptr)
        {
            cout << "No volunteers registered.\n";
            return;
        }

        cout << "\n----------------------------------- Available Volunteers -----------------------------------\n";
        cout << left
            << setw(12) << "ID"
            << setw(18) << "Name"
            << setw(15) << "Contact"
            << setw(20) << "Skill"
            << setw(12) << "Status" << endl;
        cout << "---------------------------------------------------------------------------------------------\n";

        VNode* current = front;
        int count = 0;

        while (current != nullptr)
        {
            Volunteer v = current->data;
            if (v.status == "Available") {
                cout << left
                    << setw(12) << v.volunteerID
                    << setw(18) << v.name
                    << setw(15) << v.contact
                    << setw(20) << v.skill
                    << setw(12) << v.status << endl;
                count++;
            }
            current = current->next;
        }

        cout << "---------------------------------------------------------------------------------------------\n";
        cout << "Total volunteers available: " << count << endl;
    }

    void viewAllVolunteers() {
        if (front == nullptr) {
            cout << "No volunteers registered.\n";
            return;
        }
        cout << "\n----------------------------------- All Volunteers -----------------------------------\n";
        cout << left
            << setw(12) << "ID"
            << setw(18) << "Name"
            << setw(15) << "Contact"
            << setw(20) << "Skill"
            << setw(12) << "Status" << endl;
        cout << "--------------------------------------------------------------------------------------\n";
        VNode* current = front;
        while (current != nullptr) {
            Volunteer v = current->data;
            cout << left
                << setw(12) << v.volunteerID
                << setw(18) << v.name
                << setw(15) << v.contact
                << setw(20) << v.skill
                << setw(12) << v.status << endl;
            current = current->next;
        }
        cout << "--------------------------------------------------------------------------------------\n";
        cout << "Total volunteers: " << getSize() << endl;
    }

    bool isEmpty()
    {
        return front == nullptr;
    }

    int getSize()
    {
        int count = 0;
        VNode* current = front;
        while (current != nullptr)
        {
            count++;
            current = current->next;
        }
        return count;
    }

    void deployVolunteersToRequest(int volunteerRequestID, const string& skill, int quantity) {
        // Read all volunteer requests into an array
        const int MAX_REQUESTS = 1000;
        string allRequests[MAX_REQUESTS];
        int reqCount = 0;
        ifstream reqFile(fileVolunteerRequest);
        string line, requestID, reqStatus;
        bool foundRequest = false;
        getline(reqFile, line);
        allRequests[reqCount++] = line;
        while (getline(reqFile, line) && reqCount < MAX_REQUESTS) {
            stringstream ss(line);
            string vReqID, reqID, qty, comment, date, status;
            getline(ss, vReqID, ',');
            getline(ss, reqID, ',');
            getline(ss, qty, ',');
            getline(ss, comment, ',');
            getline(ss, date, ',');
            getline(ss, status, ',');
            if (stoi(vReqID) == volunteerRequestID) {
                requestID = reqID;
                reqStatus = status;
                foundRequest = true;
                // Update status to Assigned later
                allRequests[reqCount++] = vReqID + "," + reqID + "," + qty + "," + comment + "," + date + ",Assigned";
            }
            else {
                allRequests[reqCount++] = line;
            }
        }
        reqFile.close();
        if (!foundRequest) {
            cout << "Volunteer request not found.\n";
            return;
        }
        if (reqStatus == "Assigned") {
            cout << "This volunteer request is already assigned.\n";
            return;
        }

        // Build a queue of available volunteers with the chosen skill
        struct QueueNode { VNode* volunteer; QueueNode* next; };
        QueueNode* qFront = nullptr;
        QueueNode* qRear = nullptr;
        VNode* curr = front;
        while (curr) {
            if (curr->data.status == "Available" && curr->data.skill == skill) {
                QueueNode* node = new QueueNode{ curr, nullptr };
                if (!qRear) qFront = qRear = node;
                else { qRear->next = node; qRear = node; }
            }
            curr = curr->next;
        }

        // Deploy the first N volunteers
        int deployed = 0;
        int assignmentID = 1;
        ifstream assignFile(fileVolunteerAssignment);
        getline(assignFile, line);
        while (getline(assignFile, line)) assignmentID++;
        assignFile.close();

        ofstream assignOut(fileVolunteerAssignment, ios::app);
        string today = getCurrentDateTime();
        while (qFront && deployed < quantity) {
            VNode* v = qFront->volunteer;
            v->data.status = "Assigned";
            string dateTime = getCurrentDateTime();
            string yyyy = dateTime.substr(0, 4);
            int mmInt = stoi(dateTime.substr(5, 2));
            int ddInt = stoi(dateTime.substr(8, 2));
            string formattedDate = to_string(ddInt) + "-" + to_string(mmInt) + "-" + yyyy;
            assignOut << assignmentID++ << "," << v->data.volunteerID << "," << requestID << "," << formattedDate << ",Assigned\n";
            QueueNode* temp = qFront;
            qFront = qFront->next;
            delete temp;
            deployed++;
        }
        assignOut.close();
        if (deployed < quantity) {
            cout << "Not enough available volunteers with skill " << skill << ". Only " << deployed << " assigned.\n";
        }
        else {
            cout << "Successfully assigned " << deployed << " volunteers with skill " << skill << ".\n";
        }

        // Update VolunteerRequest.csv status to Assigned
        ofstream reqOut(fileVolunteerRequest);
        for (int i = 0; i < reqCount; ++i) reqOut << allRequests[i] << "\n";
        reqOut.close();

        saveVolunteersToCSV();
    }

    void updateEmergencyRequestStatusIfReady(int requestID) {
        bool volunteerAssigned = false;
        ifstream vreqFile(fileVolunteerRequest);
        string line;
        getline(vreqFile, line);
        while (getline(vreqFile, line)) {
            stringstream ss(line);
            string vReqID, reqID, qty, comment, date, status;
            getline(ss, vReqID, ',');
            getline(ss, reqID, ',');
            getline(ss, qty, ',');
            getline(ss, comment, ',');
            getline(ss, date, ',');
            getline(ss, status, ',');
            if (stoi(reqID) == requestID && status == "Assigned") {
                volunteerAssigned = true;
                break;
            }
        }
        vreqFile.close();

        bool supplyAssigned = false;
        ifstream sreqFile(fileSupplyRequest);
        getline(sreqFile, line);
        while (getline(sreqFile, line)) {
            stringstream ss(line);
            string sReqID, supplyID, reqID, qty, date, status;
            getline(ss, sReqID, ',');
            getline(ss, supplyID, ',');
            getline(ss, reqID, ',');
            getline(ss, qty, ',');
            getline(ss, date, ',');
            getline(ss, status, ',');
            if (stoi(reqID) == requestID && status == "Assigned") {
                supplyAssigned = true;
                break;
            }
        }
        sreqFile.close();

        if (volunteerAssigned && supplyAssigned) {
            const int MAX_REQS = 1000;
            string allEmerg[MAX_REQS];
            int count = 0;
            ifstream ereqFile(fileEmergencyRequest);
            getline(ereqFile, line); // header
            allEmerg[count++] = line;
            while (getline(ereqFile, line) && count < MAX_REQS) {
                stringstream ss(line);
                string eReqID, location, type, urgency, status, date;
                getline(ss, eReqID, ',');
                getline(ss, location, ',');
                getline(ss, type, ',');
                getline(ss, urgency, ',');
                getline(ss, status, ',');
                getline(ss, date, ',');
                if (stoi(eReqID) == requestID && status == "Pending") {
                    allEmerg[count++] = eReqID + "," + location + "," + type + "," + urgency + ",Assigned," + date;
                } else {
                    allEmerg[count++] = line;
                }
            }
            ereqFile.close();

            ofstream outEreq(fileEmergencyRequest);
            for (int i = 0; i < count; ++i) outEreq << allEmerg[i] << "\n";
            outEreq.close();
        }
    }
};