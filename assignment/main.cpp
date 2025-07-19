#include <iostream>
#include "emergency.hpp"

using namespace std;

int main()
{
    // EmergencyList coordinator;

    int choice;

    while (true)
    {
        cout << "\n===== Disaster Relief Logistics Management System =====" << endl;
        cout << "1. Supply Base Manager" << endl;
        cout << "2. Volunteer Operations Officer" << endl;
        cout << "3. Emergency Request Coordinator" << endl;
        cout << "4. Transport Unit Scheduler" << endl;
        cout << "0. Exit" << endl;
        cout << "Select Role: ";
        cin >> choice;

        switch (choice)
        {
        case 3:
            PriorityQueue pq;
            int emergencyChoice;
            do
            {
                cout << "\n-- Emergency Request Coordinator Menu --" << endl;
                cout << "1. Log Emergency Request" << endl;
                cout << "2. Assign Emergency Request" << endl;
                cout << "3. Process Most Critical Request" << endl;
                cout << "4. View Pending Requests" << endl;
                cout << "5. View All Emergency Requests" << endl;
                cout << "0. Back to Main Menu" << endl;
                cout << "Enter choice: ";
                cin >> emergencyChoice;

                switch (emergencyChoice)
                {
                case 1: {
                    string loc, typ, dt;
                    int urg;
                    cout << "Enter Location: ";
                    cin.ignore();
                    getline(cin, loc);
                    cout << "Enter Type (Medical, Staff, Food): ";
                    getline(cin, typ);
                    cout << "Enter Urgency (1-10): ";
                    cin >> urg;
                    cout << "Enter Date (Logged Date): ";
                    cin.ignore();
                    getline(cin, dt);
                    pq.insert(loc, typ, urg, dt);
                    break;
                }
                case 2:
                    pq.assignSupplies();
                    break;
                case 3:
                    pq.processMostCritical();
                    break;
                case 4:
                    pq.viewPending();
                    break;
                case 5:
                    pq.viewAll();
                    break;
                case 0:
                    cout << "Returning to Main Menu...\n";
                    break;
                default:
                    cout << "Invalid emergency menu choice.\n";
                    break;
                }

            } while (emergencyChoice != 0);
        }
    }
}