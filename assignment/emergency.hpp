#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <direct.h> // For _mkdir
#include "structures.hpp"

using namespace std;

class PriorityQueue {
private:
    EmergencyRequestNode* head;
    int nextID;
    int nextSupplyRequestID;
    int nextVolunteerRequestID;

    void ensureDirectoryExists(const string& path) {
        size_t pos = path.find_last_of("/\\");
        if (pos != string::npos) {
            string dir = path.substr(0, pos);
            _mkdir(dir.c_str());
        }
    }

    void saveToFile() {
        ensureDirectoryExists(fileEmergencyRequest);
        ofstream file(fileEmergencyRequest);
        if (!file.is_open()) {
            return;
        }
        // Write header
        file << "requestID,location,type,urgency,status,date\n";
        EmergencyRequestNode* curr = head;
        while (curr) {
            EmergencyRequest& req = curr->request;
            file << req.requestID << "," << req.location << "," << req.type << ","
                 << req.urgency << "," << req.status << "," << req.date << "\n";
            curr = curr->next;
        }
        file.close();
    }

    void saveSupplyRequest(int supplyID, int requestID, int quantity) {
        ensureDirectoryExists(fileSupplyRequest);
        ofstream file(fileSupplyRequest, ios::app);
        if (!file.is_open()) {
            return;
        }
        file << nextSupplyRequestID++ << "," << supplyID << "," << requestID << ","
             << quantity << "," << getCurrentDateTime() << ",Pending\n";
        file.close();
    }

    void saveVolunteerRequest(int requestID, int quantity) {
        ensureDirectoryExists(fileVolunteerRequest);
        ofstream file(fileVolunteerRequest, ios::app);
        if (!file.is_open()) {
            return;
        }
        file << nextVolunteerRequestID++ << "," << requestID << "," << quantity << ","
             << getCurrentDateTime() << ",Pending\n";
        file.close();
    }

public:
    PriorityQueue() : head(nullptr), nextID(1), nextSupplyRequestID(1), nextVolunteerRequestID(1) {
        loadFromFile();
    }

    ~PriorityQueue() {
        while (head) {
            EmergencyRequestNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insert(string location, string type, int urgency, string date) {
        EmergencyRequestNode* newNode = new EmergencyRequestNode;
        newNode->request = EmergencyRequest{nextID++, location, type, urgency, "Pending", date};
        newNode->next = nullptr;

        if (!head || urgency > head->request.urgency) {
            newNode->next = head;
            head = newNode;
        } else {
            EmergencyRequestNode* curr = head;
            while (curr->next && curr->next->request.urgency >= urgency) {
                curr = curr->next;
            }
            newNode->next = curr->next;
            curr->next = newNode;
        }

        cout << "Logged Emergency Request ID: " << newNode->request.requestID << "\n";

        // Request supplies
        char more = 'y';
        cout << "Request supplies for this emergency? (y/n): ";
        cin >> more;
        while (more == 'y' || more == 'Y') {
            int supplyID, quantity;
            cout << "Enter Supply ID: ";
            cin >> supplyID;
            cout << "Enter Quantity: ";
            cin >> quantity;
            saveSupplyRequest(supplyID, newNode->request.requestID, quantity);
            cout << "Add more supplies? (y/n): ";
            cin >> more;
        }

        // Request volunteers
        cout << "Request volunteers for this emergency? (y/n): ";
        cin >> more;
        if (more == 'y' || more == 'Y') {
            int quantity;
            cout << "Enter Number of Volunteers Needed: ";
            cin >> quantity;
            saveVolunteerRequest(newNode->request.requestID, quantity);
        }

        saveToFile();
        viewAll();
    }

    void requestEmergencyItems() {
        if (!head) {
            cout << "No requests available.\n";
            return;
        }

        // Show pending requests
        cout << "\n-- Pending Emergency Requests --\n";
        cout << "------------------------------------------------------------\n";
        cout << "| ID | Location     | Type         | Urgency | Status  |\n";
        cout << "------------------------------------------------------------\n";

        EmergencyRequestNode* curr = head;
        bool found = false;
        while (curr) {
            if (curr->request.status == "Pending") {
                printf("| %-2d | %-12s | %-12s | %-7d | %-7s |\n",
                    curr->request.requestID, curr->request.location.c_str(),
                    curr->request.type.c_str(), curr->request.urgency, curr->request.status.c_str());
                found = true;
            }
            curr = curr->next;
        }
        cout << "------------------------------------------------------------\n";

        if (!found) {
            cout << "No Pending requests found.\n";
            return;
        }

        // Get request ID
        int selectID;
        cout << "Enter the Request ID to request items for: ";
        cin >> selectID;

        // Find the request
        curr = head;
        while (curr && (curr->request.requestID != selectID || curr->request.status != "Pending")) {
            curr = curr->next;
        }
        if (!curr) {
            cout << "No matching Pending request found with ID " << selectID << ".\n";
            return;
        }

        cout << "Requesting items for Request ID: " << curr->request.requestID
             << " | Location: " << curr->request.location << endl;

        // Request supplies
        char more = 'y';
        cout << "Request supplies? (y/n): ";
        cin >> more;
        while (more == 'y' || more == 'Y') {
            int supplyID, quantity;
            cout << "Enter Supply ID: ";
            cin >> supplyID;
            cout << "Enter Quantity: ";
            cin >> quantity;
            saveSupplyRequest(supplyID, curr->request.requestID, quantity);
            cout << "Add more supplies? (y/n): ";
            cin >> more;
        }

        // Request volunteers
        cout << "Request volunteers? (y/n): ";
        cin >> more;
        if (more == 'y' || more == 'Y') {
            int quantity;
            cout << "Enter Number of Volunteers Needed: ";
            cin >> quantity;
            saveVolunteerRequest(curr->request.requestID, quantity);
        }

        curr->request.status = "Pending";
        cout << "Items requested.\n";
        saveToFile();
    }

    void processMostCritical() {
        if (!head) {
            cout << "No requests in the queue.\n";
            return;
        }
        
        EmergencyRequestNode* curr = head;
        while (curr && curr->request.status != "Pending") {
            curr = curr->next;
        }
        if (!curr) {
            cout << "No pending requests to process.\n";
            return;
        }
        cout << "\n-- Most Critical Request --\n";
        cout << "----------------------------------------------------------------------------\n";
        cout << "| ID | Location     | Type         | Urgency | Status    | Date       |\n";
        cout << "----------------------------------------------------------------------------\n";
        printf("| %-2d | %-12s | %-12s | %-7d | %-9s | %-10s |\n",
            curr->request.requestID, curr->request.location.c_str(),
            curr->request.type.c_str(), curr->request.urgency,
            curr->request.status.c_str(), curr->request.date.c_str());
        cout << "----------------------------------------------------------------------------\n";
    }

    void viewPending() {
        if (!head) {
            cout << "No requests in the system.\n";
            return;
        }

        cout << "\n-- Pending Emergency Requests --\n";
        cout << "--------------------------------------------------------------------------------\n";
        cout << "| ID | Location     | Type         | Urgency | Status    | Date       |\n";
        cout << "--------------------------------------------------------------------------------\n";

        EmergencyRequestNode* curr = head;
        bool found = false;
        while (curr) {
            if (curr->request.status == "Pending") {
                printf("| %-2d | %-12s | %-12s | %-7d | %-9s | %-10s |\n",
                    curr->request.requestID, curr->request.location.c_str(),
                    curr->request.type.c_str(), curr->request.urgency,
                    curr->request.status.c_str(), curr->request.date.c_str());

                // Show supplies
                ifstream supplyFile(fileSupplyRequest);
                string line;
                bool hasSupplies = false;
                bool firstLine = true;
                cout << "   Supplies Requested:\n";
                while (getline(supplyFile, line)) {
                    if (firstLine) {
                        firstLine = false;
                        continue;
                    }
                    stringstream ss(line);
                    string token;
                    int supplyRequestID, supplyID, requestID, quantity;
                    string date, status;
                    getline(ss, token, ','); supplyRequestID = stoi(token);
                    getline(ss, token, ','); supplyID = stoi(token);
                    getline(ss, token, ','); requestID = stoi(token);
                    getline(ss, token, ','); quantity = stoi(token);
                    getline(ss, date, ',');
                    getline(ss, status, ',');
                    if (requestID == curr->request.requestID) {
                        cout << "     - Supply Req ID: " << supplyRequestID
                             << ", Supply ID: " << supplyID << ", Quantity: " << quantity
                             << ", Status: " << status << "\n";
                        hasSupplies = true;
                    }
                }
                supplyFile.close();
                if (!hasSupplies) cout << "     (No supplies requested)\n";

                // Show volunteers
                ifstream volunteerFile(fileVolunteerRequest);
                bool hasVolunteers = false;
                firstLine = true;
                cout << "   Volunteers Requested:\n";
                while (getline(volunteerFile, line)) {
                    if (firstLine) {
                        firstLine = false;
                        continue;
                    }
                    stringstream ss(line);
                    string token;
                    int volunteerRequestID, requestID, quantity;
                    string date, status;
                    getline(ss, token, ','); volunteerRequestID = stoi(token);
                    getline(ss, token, ','); requestID = stoi(token);
                    getline(ss, token, ','); quantity = stoi(token);
                    getline(ss, date, ',');
                    getline(ss, status, ',');
                    if (requestID == curr->request.requestID) {
                        cout << "     - Volunteer Req ID: " << volunteerRequestID
                             << ", Quantity: " << quantity << ", Status: " << status << "\n";
                        hasVolunteers = true;
                    }
                }
                volunteerFile.close();
                if (!hasVolunteers) cout << "     (No volunteers requested)\n";

                found = true;
            }
            curr = curr->next;
        }
        cout << "--------------------------------------------------------------------------------\n";

        if (!found) {
            cout << "No Pending requests found.\n";
        }
    }

    void viewAll() {
        if (!head) {
            cout << "No requests in the system.\n";
            return;
        }

        cout << "\n-- All Emergency Requests --\n";
        cout << "----------------------------------------------------------------------------------------------------------------\n";
        cout << "| ID | Location     | Type         | Urgency | Status     | Date       | Supply Req ID | Supply ID | Quantity |\n";
        cout << "----------------------------------------------------------------------------------------------------------------\n";

        EmergencyRequestNode* curr = head;
        while (curr) {
            bool hasSupplies = false;
            ifstream supplyFile(fileSupplyRequest);
            string line;
            bool firstLine = true;
            while (getline(supplyFile, line)) {
                if (firstLine) {
                    firstLine = false;
                    continue;
                }
                stringstream ss(line);
                string token;
                int supplyRequestID, supplyID, requestID, quantity;
                string date, status;
                getline(ss, token, ','); supplyRequestID = stoi(token);
                getline(ss, token, ','); supplyID = stoi(token);
                getline(ss, token, ','); requestID = stoi(token);
                getline(ss, token, ','); quantity = stoi(token);
                getline(ss, date, ',');
                getline(ss, status, ',');
                if (requestID == curr->request.requestID) {
                    printf("| %-2d | %-12s | %-12s | %-7d | %-10s | %-10s | %-13d | %-9d | %-8d |\n",
                        curr->request.requestID, curr->request.location.c_str(),
                        curr->request.type.c_str(), curr->request.urgency,
                        curr->request.status.c_str(), curr->request.date.c_str(),
                        supplyRequestID, supplyID, quantity);
                    hasSupplies = true;
                }
            }
            supplyFile.close();

            if (!hasSupplies) {
                printf("| %-2d | %-12s | %-12s | %-7d | %-10s | %-10s | %-13s | %-9s | %-8s |\n",
                    curr->request.requestID, curr->request.location.c_str(),
                    curr->request.type.c_str(), curr->request.urgency,
                    curr->request.status.c_str(), curr->request.date.c_str(),
                    "-", "-", "-");
            }

            // Show volunteers
            ifstream volunteerFile(fileVolunteerRequest);
            bool hasVolunteers = false;
            firstLine = true;
            cout << "   Volunteers Requested:\n";
            while (getline(volunteerFile, line)) {
                if (firstLine) {
                    firstLine = false;
                    continue;
                }
                stringstream ss(line);
                string token;
                int volunteerRequestID, requestID, quantity;
                string date, status;
                getline(ss, token, ','); volunteerRequestID = stoi(token);
                getline(ss, token, ','); requestID = stoi(token);
                getline(ss, token, ','); quantity = stoi(token);
                getline(ss, date, ',');
                getline(ss, status, ',');
                if (requestID == curr->request.requestID) {
                    cout << "     - Volunteer Req ID: " << volunteerRequestID
                         << ", Quantity: " << quantity << ", Status: " << status << "\n";
                    hasVolunteers = true;
                }
            }
            volunteerFile.close();
            if (!hasVolunteers) cout << "     (No volunteers requested)\n";

            curr = curr->next;
        }
        cout << "----------------------------------------------------------------------------------------------------------------\n";
    }

    void loadFromFile() {
        ensureDirectoryExists(fileEmergencyRequest);
        ifstream file(fileEmergencyRequest);
        if (!file.is_open()) {
            return;
        }
        string line;
        bool firstLine = true;
        while (getline(file, line)) {
            if (firstLine) {
                firstLine = false;
                continue;
            }
            if (line.empty()) continue;

            EmergencyRequestNode* node = new EmergencyRequestNode;
            node->request = EmergencyRequest();
            node->next = nullptr;

            stringstream ss(line);
            string token;
            getline(ss, token, ','); node->request.requestID = stoi(token);
            getline(ss, node->request.location, ',');
            getline(ss, node->request.type, ',');
            getline(ss, token, ','); node->request.urgency = stoi(token);
            getline(ss, node->request.status, ',');
            getline(ss, node->request.date, ',');

            if (!head || node->request.urgency > head->request.urgency) {
                node->next = head;
                head = node;
            } else {
                EmergencyRequestNode* curr = head;
                while (curr->next && curr->next->request.urgency >= node->request.urgency) {
                    curr = curr->next;
                }
                node->next = curr->next;
                curr->next = node;
            }
            if (node->request.requestID >= nextID) nextID = node->request.requestID + 1;
        }
        file.close();

        // Update nextSupplyRequestID
        ensureDirectoryExists(fileSupplyRequest);
        ifstream supplyFile(fileSupplyRequest);
        firstLine = true;
        while (getline(supplyFile, line)) {
            if (firstLine) {
                firstLine = false;
                continue;
            }
            stringstream ss(line);
            string token;
            int supplyRequestID;
            getline(ss, token, ','); supplyRequestID = stoi(token);
            if (supplyRequestID >= nextSupplyRequestID) nextSupplyRequestID = supplyRequestID + 1;
        }
        supplyFile.close();

        // Update nextVolunteerRequestID
        ensureDirectoryExists(fileVolunteerRequest);
        ifstream volunteerFile(fileVolunteerRequest);
        firstLine = true;
        while (getline(volunteerFile, line)) {
            if (firstLine) {
                firstLine = false;
                continue;
            }
            stringstream ss(line);
            string token;
            int volunteerRequestID;
            getline(ss, token, ','); volunteerRequestID = stoi(token);
            if (volunteerRequestID >= nextVolunteerRequestID) nextVolunteerRequestID = volunteerRequestID + 1;
        }
        volunteerFile.close();
    }
};