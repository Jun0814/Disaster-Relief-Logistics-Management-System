#include <iostream>
#include <string>
#include <fstream>
#include "structures.hpp"
using namespace std;

class PriorityQueue {
private:
    EmergencyRequest* head;
    int nextID = 1;

public:
    PriorityQueue() {
        head = nullptr;
        loadFromFile();
    }

    void insert(string location, string type, int urgency, string date) {
        EmergencyRequest* newRequest = new EmergencyRequest;
        newRequest->requestID = nextID++;
        newRequest->location = location;
        newRequest->type = type;
        newRequest->urgency = urgency;
        newRequest->status = "Pending";
        newRequest->date = date;
        newRequest->supplies = nullptr;
        newRequest->next = nullptr;

        if (!head || urgency > head->urgency) {
            newRequest->next = head;
            head = newRequest;
        } else {
            EmergencyRequest* curr = head;
            while (curr->next && curr->next->urgency >= urgency) {
                curr = curr->next;
            }
            newRequest->next = curr->next;
            curr->next = newRequest;
        }

        cout << "Logged Emergency Request ID: " << newRequest->requestID << "\n";
        saveToFile();

        cout << "\n-- All Emergency Requests --\n";
        cout << "-----------------------------------------------------------------------------------\n";
        cout << "| ID | Location     | Type         | Urgency | Status    | Date       |\n";
        cout << "-----------------------------------------------------------------------------------\n";

        EmergencyRequest* curr = head;
        while (curr) {
            printf("| %-2d | %-12s | %-12s | %-7d | %-9s | %-10s |\n",
                curr->requestID,
                curr->location.c_str(),
                curr->type.c_str(),
                curr->urgency,
                curr->status.c_str(),
                curr->date.c_str());
            curr = curr->next;
        }
        cout << "-----------------------------------------------------------------------------------\n";
    }

    void assignSupplies() {
        if (!head) {
            cout << "No requests available.\n";
            return;
        }

        // 1) Show only Pending requests
        cout << "\n-- Pending Emergency Requests --\n";
        cout << "------------------------------------------------------------\n";
        cout << "| ID | Location     | Type         | Urgency | Status  |\n";
        cout << "------------------------------------------------------------\n";

        EmergencyRequest* curr = head;
        bool found = false;

        while (curr) {
            if (curr->status == "Pending") {
                printf("| %-2d | %-12s | %-12s | %-7d | %-7s |\n",
                    curr->requestID,
                    curr->location.c_str(),
                    curr->type.c_str(),
                    curr->urgency,
                    curr->status.c_str());
                found = true;
            }
            curr = curr->next;
        }
        cout << "------------------------------------------------------------\n";

        if (!found) {
            cout << "No Pending requests found.\n";
            return;
        }

        // 2) Ask user to pick which ID
        int selectID;
        cout << "Enter the Request ID you want to assign supplies to: ";
        cin >> selectID;

        // 3) Find the selected request
        EmergencyRequest* target = head;
        while (target) {
            if (target->requestID == selectID && target->status == "Pending") {
                break;
            }
            target = target->next;
        }

        if (!target) {
            cout << "No matching Pending request found with ID " << selectID << ".\n";
            return;
        }

        cout << "Assigning supplies to Request ID: " << target->requestID
            << " | Location: " << target->location << endl;

        char more = 'y';
        while (more == 'y' || more == 'Y') {
            SupplyNode* newSupply = new SupplyNode;
            cout << "Enter Supply ID: ";
            cin >> newSupply->supplyID;
            cout << "Enter Quantity: ";
            cin >> newSupply->quantity;
            newSupply->next = target->supplies;
            target->supplies = newSupply;

            cout << "Add more supplies? (y/n): ";
            cin >> more;
        }

        target->status = "Assigned";
        cout << "Supplies assigned. Status updated to Assigned.\n";
        saveToFile();
    }

    void processMostCritical() {
        if (!head) {
            cout << "No requests in the queue.\n";
            return;
        }

        EmergencyRequest* prev = nullptr;
        EmergencyRequest* curr = head;

        // Find the first Pending request (highest urgency first)
        while (curr && curr->status != "Pending") {
            prev = curr;
            curr = curr->next;
        }

        if (!curr) {
            cout << "No pending requests to process.\n";
            return;
        }

        // Remove it from queue
        if (curr == head) {
            head = curr->next;
        } else {
            prev->next = curr->next;
        }

        // Show as table
        cout << "\n-- Processing Most Critical Request --\n";
        cout << "----------------------------------------------------------------------------\n";
        cout << "| ID | Location     | Type         | Urgency | Status    | Date       |\n";
        cout << "----------------------------------------------------------------------------\n";
        printf("| %-2d | %-12s | %-12s | %-7d | %-9s | %-10s |\n",
            curr->requestID,
            curr->location.c_str(),
            curr->type.c_str(),
            curr->urgency,
            curr->status.c_str(),
            curr->date.c_str());
        cout << "----------------------------------------------------------------------------\n";

        // Optionally mark as completed
        curr->status = "Completed";

        // Clean up
        deleteSupplies(curr->supplies);
        delete curr;

        cout << "Request processed and removed from queue.\n";
        saveToFile();
    }

    void viewPending() {
        if (!head) {
            cout << "No requests in the system.\n";
            return;
        }

        EmergencyRequest* curr = head;
        bool found = false;

        cout << "\n-- Pending Emergency Requests --\n";
        cout << "--------------------------------------------------------------------------------\n";
        cout << "| ID | Location     | Type         | Urgency | Status    | Date       |\n";
        cout << "--------------------------------------------------------------------------------\n";

        while (curr) {
            if (curr->status == "Pending") {
                printf("| %-2d | %-12s | %-12s | %-7d | %-9s | %-10s |\n",
                    curr->requestID,
                    curr->location.c_str(),
                    curr->type.c_str(),
                    curr->urgency,
                    curr->status.c_str(),
                    curr->date.c_str());

                // Show any assigned supplies
                SupplyNode* s = curr->supplies;
                if (s) {
                    cout << "   Assigned Supplies:\n";
                    while (s) {
                        cout << "     - Supply ID: " << s->supplyID
                            << ", Quantity: " << s->quantity << "\n";
                        s = s->next;
                    }
                } else {
                    cout << "   (No supplies assigned yet)\n";
                }

                found = true;
            }
            curr = curr->next;
        }
        cout << "--------------------------------------------------------------------------------\n";

        if (!found) {
            cout << "No Pending requests found.\n";
        }
    }
    
    void saveToFile() {
        ofstream file(fileEmergencyRequest);
        if (!file.is_open()) {
            cout << "Error saving to file.\n";
            return;
        }
        EmergencyRequest* curr = head;
        while (curr) {
            file << curr->requestID << ","
                 << curr->location << ","
                 << curr->type << ","
                 << curr->urgency << ","
                 << curr->status << ","
                 << curr->date << ",";
            // Write supplies inline (ID|Qty;ID|Qty)
            SupplyNode* s = curr->supplies;
            while (s) {
                file << s->supplyID << "|" << s->quantity;
                if (s->next) file << ";";
                s = s->next;
            }
            file << "\n";
            curr = curr->next;
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file(fileEmergencyRequest);
        if (!file.is_open()) {
            cout << "No previous data found.\n";
            return;
        }
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            EmergencyRequest* req = new EmergencyRequest;
            req->supplies = nullptr;
            req->next = nullptr;

            size_t pos = 0, prev = 0;
            int field = 0;
            while ((pos = line.find(',', prev)) != string::npos) {
                string token = line.substr(prev, pos - prev);
                switch (field) {
                    case 0: req->requestID = stoi(token); break;
                    case 1: req->location = token; break;
                    case 2: req->type = token; break;
                    case 3: req->urgency = stoi(token); break;
                    case 4: req->status = token; break;
                    case 5: req->date = token; break;
                }
                prev = pos + 1;
                field++;
                if (field == 6) break;
            }
            if (prev < line.size()) {
                string suppliesStr = line.substr(prev);
                size_t sPos = 0, sPrev = 0;
                while ((sPos = suppliesStr.find(';', sPrev)) != string::npos) {
                    string pair = suppliesStr.substr(sPrev, sPos - sPrev);
                    size_t bar = pair.find('|');
                    SupplyNode* s = new SupplyNode;
                    s->supplyID = stoi(pair.substr(0, bar));
                    s->quantity = stoi(pair.substr(bar + 1));
                    s->next = req->supplies;
                    req->supplies = s;
                    sPrev = sPos + 1;
                }
                if (sPrev < suppliesStr.size()) {
                    string pair = suppliesStr.substr(sPrev);
                    size_t bar = pair.find('|');
                    SupplyNode* s = new SupplyNode;
                    s->supplyID = stoi(pair.substr(0, bar));
                    s->quantity = stoi(pair.substr(bar + 1));
                    s->next = req->supplies;
                    req->supplies = s;
                }
            }
            // Insert maintaining priority
            if (!head || req->urgency > head->urgency) {
                req->next = head;
                head = req;
            } else {
                EmergencyRequest* curr = head;
                while (curr->next && curr->next->urgency >= req->urgency) {
                    curr = curr->next;
                }
                req->next = curr->next;
                curr->next = req;
            }
            if (req->requestID >= nextID) nextID = req->requestID + 1;
        }
        file.close();
    }

    void viewAll() {
        if (!head) {
            cout << "No requests in the system.\n";
            return;
        }

        EmergencyRequest* curr = head;

        cout << "\n-- All Emergency Requests --\n";
        cout << "--------------------------------------------------------------------------------------------------\n";
        cout << "| ID | Location     | Type         | Urgency | Status     | Date       | Supply ID | Quantity |\n";
        cout << "--------------------------------------------------------------------------------------------------\n";

        while (curr) {
            // If there are supplies, print one row per supply
            if (curr->supplies) {
                SupplyNode* s = curr->supplies;
                while (s) {
                    printf("| %-2d | %-12s | %-12s | %-7d | %-10s | %-10s | %-9d | %-8d |\n",
                        curr->requestID,
                        curr->location.c_str(),
                        curr->type.c_str(),
                        curr->urgency,
                        curr->status.c_str(),
                        curr->date.c_str(),
                        s->supplyID,
                        s->quantity);
                    s = s->next;
                }
            } else {
                // If no supplies, print one row with dash
                printf("| %-2d | %-12s | %-12s | %-7d | %-10s | %-10s | %-9s | %-8s |\n",
                    curr->requestID,
                    curr->location.c_str(),
                    curr->type.c_str(),
                    curr->urgency,
                    curr->status.c_str(),
                    curr->date.c_str(),
                    "-",
                    "-");
            }

            curr = curr->next;
        }

        cout << "--------------------------------------------------------------------------------------------------\n";
    }

    void deleteSupplies(SupplyNode* s) {
        while (s) {
            SupplyNode* temp = s;
            s = s->next;
            delete temp;
        }
    }
};