#include <iostream>
#include <limits>
#include "emergency.hpp"
#include "volunteer.hpp"

using namespace std;

// Helper function to clear cin buffer
void clearCinBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

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
        
        if (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number.\n";
            clearCinBuffer();
            continue;
        }

        switch (choice)
        {
        case 2:
            {
                VolunteerManager vm;
                int volunteerChoice;
                do
                {
                    cout << "\n-- Volunteer Operations Officer Menu --" << endl;
                    cout << "1. Register Volunteer" << endl;
                    cout << "2. Deploy Volunteer to Field" << endl;
                    cout << "3. View Registered Volunteers" << endl;
                    cout << "0. Back to Main Menu" << endl;
                    cout << "Enter choice: ";
                    
                    if (!(cin >> volunteerChoice)) {
                        cout << "Invalid input! Please enter a number.\n";
                        clearCinBuffer();
                        continue;
                    }

                    switch (volunteerChoice)
                    {
                    case 1: {
                        string name, contact, skill;
                        cout << "Enter Volunteer Name: ";
                        cin.ignore();
                        getline(cin, name);
                        cout << "Enter Contact Number: ";
                        getline(cin, contact);
                        cout << "Enter Skill Area (Medical, Logistics, Communication, etc.): ";
                        getline(cin, skill);
                        vm.registerVolunteer(name, contact, skill);
                        break;
                    }
                    case 2:
                        vm.deployVolunteer();
                        break;
                    case 3:
                        vm.viewVolunteers();
                        break;
                    case 0:
                        cout << "Returning to Main Menu...\n";
                        break;
                    default:
                        cout << "Invalid volunteer menu choice.\n";
                        break;
                    }

                } while (volunteerChoice != 0);
                break;
            }
        case 3: {
            PriorityQueue pq;
            int emergencyChoice;
            do {
                cout << "\n-- Emergency Request Coordinator Menu --\n";
                cout << "1. Log Emergency Request\n";
                cout << "2. Request Emergency Items\n";
                cout << "3. Process Most Critical Request\n";
                cout << "4. View Pending Requests\n";
                cout << "5. View All Emergency Requests\n";
                cout << "0. Back to Main Menu\n";
                cout << "Enter choice: ";

                if (!(cin >> emergencyChoice)) {
                    cout << "Invalid input! Please enter a number.\n";
                    clearCinBuffer();
                    continue;
                }

                switch (emergencyChoice) {
                    case 1: {
                        string loc, typ, dt;
                        int urg;
                        cout << "Enter Location: ";
                        cin.ignore();
                        getline(cin, loc);
                        cout << "Enter Type (Volunteer, Supply): ";
                        getline(cin, typ);
                        cout << "Enter Urgency (1-10): ";
                        cin >> urg;
                        cout << "Enter Date (DD-MM-YYYY): ";
                        cin.ignore();
                        getline(cin, dt);
                        pq.insert(loc, typ, urg, dt);
                        break;
                    }
                    case 2:
                        pq.requestEmergencyItems();
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
            break;
        }
        case 0:
            cout << "Exiting Disaster Relief Logistics Management System...\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}