#include <iostream>
#include "emergency.hpp"

using namespace std;

int main()
{
    EmergencyCoordinator coordinator;

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
            int emergencyChoice;
            do
            {
                cout << "\n-- Emergency Request Coordinator Menu --" << endl;
                cout << "1. Log Emergency Request" << endl;
                cout << "2. Request Supply Packages" << endl;
                cout << "3. Request Volunteers" << endl;
                cout << "4. View Pending Requests" << endl;
                cout << "0. Back to Main Menu" << endl;
                cout << "Enter choice: ";
                cin >> emergencyChoice;

                switch (emergencyChoice)
                {
                case 1:
                {
                    string location, type;
                    int urgency;
                    cout << "Enter location: ";
                    cin >> location;
                    cout << "Enter type of emergency: ";
                    cin >> type;
                    cout << "Enter urgency level (1-10): ";
                    cin >> urgency;
                    coordinator.logRequest(location, type, urgency, "pending");
                    break;
                }
                case 2:
                    coordinator.requestSupplyPackage();
                    break;
                case 4:
                {
                    coordinator.viewPendingRequests();
                    break;
                }
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