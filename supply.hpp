#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "structures.hpp"
using namespace std;

class SupplyManager {
private:
    SupplyStackNode* top;
    int nextID;

    // Helper to get supply type by ID from SupplyBox.csv
    string getSupplyTypeByID(const string& supplyID) {
        ifstream file(fileSupplyBox);
        string line;
        getline(file, line); // skip header
        while (getline(file, line)) {
            stringstream ss(line);
            string id, type, quantity;
            getline(ss, id, ',');
            getline(ss, type, ',');
            getline(ss, quantity, ',');
            if (id == supplyID) return type;
        }
        return "unknown";
    }

    void loadFromCSV() {
        top = nullptr;
        nextID = 1;
        ifstream file(fileSupplyBox);
        if (!file.is_open()) return;
        string line;
        getline(file, line);

        // Read all supplies into a temp array
        Supply supplies[1000];
        int count = 0;
        while (getline(file, line) && count < 1000) {
            stringstream ss(line);
            string id, type, quantity;
            getline(ss, id, ',');
            getline(ss, type, ',');
            getline(ss, quantity, ',');
            supplies[count].supplyID = stoi(id);
            supplies[count].type = type;
            supplies[count].quantity = stoi(quantity);
            if (supplies[count].supplyID >= nextID) nextID = supplies[count].supplyID + 1;
            count++;
        }
        file.close();

        // Push onto stack in reverse order (so first row is top)
        for (int i = count - 1; i >= 0; --i) {
            SupplyStackNode* node = new SupplyStackNode{ supplies[i], top };
            top = node;
        }
    }

    // Save from top to bottom
    void saveToCSV() {
        ofstream file(fileSupplyBox);
        if (!file.is_open()) {
            cout << "Error: Cannot open supply file for writing.\n";
            return;
        }
        file << "supplyID,type,quantity\n";
        SupplyStackNode* curr = top;
        while (curr) {
            file << curr->supply.supplyID << "," << curr->supply.type << "," << curr->supply.quantity << "\n";
            curr = curr->next;
        }
        file.close();
    }

    SupplyStackNode* findSupplyByType(const string& type) {
        SupplyStackNode* curr = top;
        while (curr) {
            if (curr->supply.type == type) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    SupplyStackNode* findPrevSupplyByType(const string& type) {
        SupplyStackNode* curr = top;
        SupplyStackNode* prev = nullptr;
        while (curr) {
            if (curr->supply.type == type) return prev;
            prev = curr;
            curr = curr->next;
        }
        return nullptr;
    }

//     SupplyNode* findSupplyByID(int supplyID) {
//         SupplyNode* curr = front;
//         while (curr) {
//             if (curr->supply.supplyID == supplyID) return curr;
//             curr = curr->next;
//         }
//         return nullptr;
//     }
  
public:
    SupplyManager() : top(nullptr), nextID(1) { loadFromCSV(); }
    ~SupplyManager() {
        saveToCSV();
        while (top) { SupplyStackNode* temp = top; top = top->next; delete temp; }
    }

    // 1. Add supply (push)
    void addSupply(const string& type, int quantity) {
        SupplyStackNode* node = findSupplyByType(type);
        if (node) {
            node->supply.quantity += quantity;
            cout << "Updated supply: " << type << " (ID: " << node->supply.supplyID << ") now has quantity " << node->supply.quantity << "\n";
        } else {
            Supply supply{ nextID++, type, quantity };
            SupplyStackNode* newNode = new SupplyStackNode{ supply, top };
            top = newNode;
            cout << "Added new supply: " << type << " (ID: " << supply.supplyID << ") with quantity " << quantity << "\n";
        }
        saveToCSV();
    }

    // 2. Delete a supply type
    void deleteSupplyType(const string& type) {
        SupplyStackNode* prev = findPrevSupplyByType(type);
        SupplyStackNode* toDelete = nullptr;
        if (!prev && top && top->supply.type == type) {
            toDelete = top;
            top = top->next;
            if (!top) top = nullptr;
        } else if (prev && prev->next) {
            toDelete = prev->next;
            prev->next = toDelete->next;
            if (toDelete == top) top = prev;
        }
        if (toDelete) {
            cout << "Deleted supply type: " << type << "\n";
            delete toDelete;
            saveToCSV();
        } else {
            cout << "Supply type not found.\n";
        }
    }

    // 3. View all supplies
    void viewSupplies() {
        if (!top) { cout << "No supplies found.\n"; return; }
        cout << "\n------------ Supply ---------------\n";
        cout << "ID\tType\t\tQuantity\n";
        cout << "-----------------------------------\n";
        SupplyStackNode* curr = top;
        while (curr) {
            cout << curr->supply.supplyID << "\t" << curr->supply.type << "\t\t" << curr->supply.quantity << "\n";
            curr = curr->next;
        }
        cout << "-----------------------------------\n";
    }

    // 4. View pending supply requests
    void viewPendingSupplyRequests() {
        ifstream file(fileSupplyRequest);
        string line;
        getline(file, line); 
        bool found = false;
        cout << "\n--- Pending Supply Requests ---\n";
        cout << left
             << setw(8)  << "ReqID"
             << setw(10) << "SupplyID"
             << setw(12) << "Type"
             << setw(10) << "Quantity"
             << setw(10) << "Status" << endl;
        cout << "-------------------------------------------------------------\n";
        while (getline(file, line)) {
            stringstream ss(line);
            string reqID, supplyID, requestID, quantity, date, status;
            getline(ss, reqID, ',');
            getline(ss, supplyID, ',');
            getline(ss, requestID, ',');
            getline(ss, quantity, ',');
            getline(ss, date, ',');
            getline(ss, status, ',');
            if (status == "Pending") {
                string type = getSupplyTypeByID(supplyID);
                cout << left
                     << setw(8)  << reqID
                     << setw(10) << supplyID
                     << setw(12) << type
                     << setw(10) << quantity
                     << setw(10) << status << endl;
                found = true;
            }
        }
        if (!found) cout << "No pending supply requests.\n";
        cout << "-------------------------------------------------------------\n";
        file.close();
    }

    // 5. Assign supply to request
    void assignSupplyToRequest(int supplyRequestID) {
        // Count requests
        ifstream file(fileSupplyRequest);
        string line;
        getline(file, line); 
        int count = 0;
        while (getline(file, line)) count++;
        file.clear();
        file.seekg(0, ios::beg);

        // Read all requests into array
        string* lines = new string[count];
        getline(file, line); 
        int idx = 0;
        while (getline(file, line)) {
            lines[idx++] = line;
        }
        file.close();

        // Find the request
        int foundIdx = -1;
        int supplyID = 0, quantityNeeded = 0;
        for (int i = 0; i < count; ++i) {
            stringstream ss(lines[i]);
            string reqID, sID, requestID, qty, date, status;
            getline(ss, reqID, ',');
            getline(ss, sID, ',');
            getline(ss, requestID, ',');
            getline(ss, qty, ',');
            getline(ss, date, ',');
            getline(ss, status, ',');
            if (stoi(reqID) == supplyRequestID && status == "Pending") {
                foundIdx = i;
                supplyID = stoi(sID);
                quantityNeeded = stoi(qty);
                break;
            }
        }
        if (foundIdx == -1) {
            cout << "Request not found or already assigned.\n";
            delete[] lines;
            return;
        }

        // Find supply
        SupplyStackNode* node = findSupplyByID(supplyID);
        if (!node || node->supply.quantity < quantityNeeded) {
            cout << "Not enough supply available for this request.\n";
            delete[] lines;
            return;
        }
        node->supply.quantity -= quantityNeeded;

        // Update request status
        // Rebuild the line with status = Assigned
        stringstream ss(lines[foundIdx]);
        string reqID, sID, requestID, qty, date, status;
        getline(ss, reqID, ',');
        getline(ss, sID, ',');
        getline(ss, requestID, ',');
        getline(ss, qty, ',');
        getline(ss, date, ',');
        getline(ss, status, ',');
        lines[foundIdx] = reqID + "," + sID + "," + requestID + "," + qty + "," + date + ",Assigned";

        cout << "Assigned " << quantityNeeded << " units of supply ID " << supplyID << " to request " << supplyRequestID << ".\n";

        // Save supplies
        saveToCSV();

        // Save requests
        ofstream rfile(fileSupplyRequest);
        rfile << "supplyRequestID,supplyID,requestID,quantity,date,status\n";
        for (int i = 0; i < count; ++i) {
            rfile << lines[i] << "\n";
        }
        rfile.close();
        delete[] lines;
    }
    
    SupplyStackNode *findSupplyByID(int supplyID)
    {
        SupplyStackNode *curr = top;
        while (curr)
        {
            if (curr->supply.supplyID == supplyID)
                return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    // Get Emergency Request ID from Supply Request ID
    int getEmergencyRequestIDFromSupplyRequestID(int supplyRequestID) {
        ifstream sreqFile(fileSupplyRequest);
        string line;
        getline(sreqFile, line); // skip header
        while (getline(sreqFile, line)) {
            stringstream ss(line);
            string sReqID, supplyID, reqID, qty, date, status;
            getline(ss, sReqID, ',');
            getline(ss, supplyID, ',');
            getline(ss, reqID, ',');
            if (stoi(sReqID) == supplyRequestID) {
                sreqFile.close();
                return stoi(reqID);
            }
        }
        sreqFile.close();
        return -1;
    }

    void updateEmergencyRequestStatusIfReady(int requestID) {
        // Check if there are any volunteer requests for this emergency
        bool hasVolunteerRequest = false;
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
            if (stoi(reqID) == requestID) {
                hasVolunteerRequest = true;
                if (status == "Assigned") {
                    volunteerAssigned = true;
                }
            }
        }
        vreqFile.close();

        // Check if there are any supply requests for this emergency
        bool hasSupplyRequest = false;
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
            if (stoi(reqID) == requestID) {
                hasSupplyRequest = true;
                if (status == "Assigned") {
                    supplyAssigned = true;
                }
            }
        }
        sreqFile.close();

        // Logic: If both exist, require both assigned. If only one exists, require only that one assigned.
        bool ready = false;
        if (hasVolunteerRequest && hasSupplyRequest) {
            ready = volunteerAssigned && supplyAssigned;
        } else if (hasVolunteerRequest) {
            ready = volunteerAssigned;
        } else if (hasSupplyRequest) {
            ready = supplyAssigned;
        }

        if (ready) {
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
