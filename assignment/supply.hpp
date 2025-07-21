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
    SupplyNode* front;
    SupplyNode* rear;
    int nextID;

    void loadFromCSV() {
        front = rear = nullptr;
        nextID = 1;
        ifstream file(fileSupplyBox);
        if (!file.is_open()) return;
        string line;
        getline(file, line);
        while (getline(file, line)) {
            stringstream ss(line);
            string id, type, quantity;
            getline(ss, id, ',');
            getline(ss, type, ',');
            getline(ss, quantity, ',');
            Supply supply;
            supply.supplyID = stoi(id);
            supply.type = type;
            supply.quantity = stoi(quantity);
            SupplyNode* newNode = new SupplyNode{ supply, nullptr };
            if (!rear) front = rear = newNode;
            else { rear->next = newNode; rear = newNode; }
            if (supply.supplyID >= nextID) nextID = supply.supplyID + 1;
        }
        file.close();
    }

    void saveToCSV() {
        ofstream file(fileSupplyBox);
        if (!file.is_open()) {
            cout << "Error: Cannot open supply file for writing.\n";
            return;
        }
        file << "supplyID,type,quantity\n";
        SupplyNode* curr = front;
        while (curr) {
            file << curr->supply.supplyID << "," << curr->supply.type << "," << curr->supply.quantity << "\n";
            curr = curr->next;
        }
        file.close();
    }

    SupplyNode* findSupplyByType(const string& type) {
        SupplyNode* curr = front;
        while (curr) {
            if (curr->supply.type == type) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    SupplyNode* findPrevSupplyByType(const string& type) {
        SupplyNode* curr = front;
        SupplyNode* prev = nullptr;
        while (curr) {
            if (curr->supply.type == type) return prev;
            prev = curr;
            curr = curr->next;
        }
        return nullptr;
    }

    SupplyNode* findSupplyByID(int supplyID) {
        SupplyNode* curr = front;
        while (curr) {
            if (curr->supply.supplyID == supplyID) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

public:
    SupplyManager() : front(nullptr), rear(nullptr), nextID(1) { loadFromCSV(); }
    ~SupplyManager() {
        saveToCSV();
        while (front) { SupplyNode* temp = front; front = front->next; delete temp; }
    }

    // 1. Add or update supply by type
    void addSupplyByType(const string& type, int quantity) {
        SupplyNode* node = findSupplyByType(type);
        if (node) {
            node->supply.quantity += quantity;
            cout << "Updated supply: " << type << " now has quantity " << node->supply.quantity << "\n";
        } else {
            Supply supply{ nextID++, type, quantity };
            SupplyNode* newNode = new SupplyNode{ supply, nullptr };
            if (!rear) front = rear = newNode;
            else { rear->next = newNode; rear = newNode; }
            cout << "Added new supply: " << type << " (ID: " << supply.supplyID << ") with quantity " << quantity << "\n";
        }
        saveToCSV();
    }

    // 2. Delete a supply type
    void deleteSupplyType(const string& type) {
        SupplyNode* prev = findPrevSupplyByType(type);
        SupplyNode* toDelete = nullptr;
        if (!prev && front && front->supply.type == type) {
            toDelete = front;
            front = front->next;
            if (!front) rear = nullptr;
        } else if (prev && prev->next) {
            toDelete = prev->next;
            prev->next = toDelete->next;
            if (toDelete == rear) rear = prev;
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
        if (!front) { cout << "No supplies in inventory.\n"; return; }
        cout << "\n--- Current Supplies ---\n";
        cout << "ID\tType\t\tQuantity\n";
        cout << "-----------------------------\n";
        SupplyNode* curr = front;
        while (curr) {
            cout << curr->supply.supplyID << "\t" << curr->supply.type << "\t\t" << curr->supply.quantity << "\n";
            curr = curr->next;
        }
        cout << "-----------------------------\n";
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
                string type = supplyID == "1" ? "food" : supplyID == "2" ? "beverage" : supplyID == "3" ? "clothes" : "other";
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
        SupplyNode* node = findSupplyByID(supplyID);
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
};
