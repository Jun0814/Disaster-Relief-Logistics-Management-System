#include <iostream>
#include <limits>
#include "emergency.hpp"
#include "volunteer.hpp"
#include "supply.hpp"
#include "transport.hpp"

using namespace std;

// Helper function to clear cin buffer
void clearCinBuffer()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main()
{
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

        if (!(cin >> choice))
        {
            cout << "Invalid input! Please enter a number.\n";
            clearCinBuffer();
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            SupplyManager sm;
            int supplyChoice;
            do
            {
                cout << "\n-- Supply Base Manager Menu --\n";
                cout << "1. Add/Update Supply\n";
                cout << "2. Delete Supply\n";
                cout << "3. View All Supplies\n";
                cout << "4. View Pending Supply Requests\n";
                cout << "5. Assign Supply to Request\n";
                cout << "0. Back to Main Menu\n";
                cout << "Enter choice: ";
                cin >> supplyChoice;
                switch (supplyChoice)
                {
                case 1:
                {
                    string type;
                    int qty;
                    cout << "Enter Supply Type: ";
                    cin.ignore();
                    getline(cin, type);
                    cout << "Enter Quantity: ";
                    cin >> qty;
                    sm.addSupply(type, qty);
                    break;
                }
                case 2:
                {
                    string type;
                    cout << "Enter Supply Type to delete: ";
                    cin.ignore();
                    getline(cin, type);
                    sm.deleteSupplyType(type);
                    break;
                }
                case 3:
                    sm.viewSupplies();
                    break;
                case 4:
                    sm.viewPendingSupplyRequests();
                    break;
                case 5:
                {
                    int reqID;
                    cout << "Enter Supply Request ID to assign: ";
                    cin >> reqID;
                    sm.assignSupplyToRequest(reqID);
                    int emergencyRequestID = sm.getEmergencyRequestIDFromSupplyRequestID(reqID);
                    if (emergencyRequestID != -1) {
                        sm.updateEmergencyRequestStatusIfReady(emergencyRequestID);
                    } else {
                        cout << "Could not find Emergency Request ID for this Supply Request.\n";
                    }
                    break;
                }
                case 0:
                    cout << "Returning to Main Menu...\n";
                    break;
                default:
                    cout << "Invalid supply menu choice.\n";
                    break;
                }
            } while (supplyChoice != 0);
            break;
        }
        case 2:
        {
            VolunteerManager vm;
            int volunteerChoice;
            do
            {
                cout << "\n-- Volunteer Operations Officer Menu --" << endl;
                cout << "1. Register Volunteer" << endl;
                cout << "2. Deploy Volunteer to Field" << endl;
                cout << "3. View Available Volunteers" << endl;
                cout << "4. View Pending Volunteer Requests" << endl;
                cout << "5. View All Volunteers" << endl;
                cout << "0. Back to Main Menu" << endl;
                cout << "Enter choice: ";

                if (!(cin >> volunteerChoice))
                {
                    cout << "Invalid input! Please enter a number.\n";
                    clearCinBuffer();
                    continue;
                }

                switch (volunteerChoice)
                {
                case 1:
                {
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
                {
                    int volunteerRequestID;
                    string skill;
                    int quantity;
                    cout << "Enter Volunteer Request ID: ";
                    cin >> volunteerRequestID;
                    cout << "Enter Skill: ";
                    cin >> skill;
                    cout << "Enter Quantity: ";
                    cin >> quantity;
                    vm.deployVolunteersToRequest(volunteerRequestID, skill, quantity);
                    int emergencyRequestID = vm.getEmergencyRequestIDFromVolunteerRequestID(volunteerRequestID);
                    if (emergencyRequestID != -1) {
                        vm.updateEmergencyRequestStatusIfReady(emergencyRequestID);
                    } else {
                        cout << "Could not find Emergency Request ID for this Volunteer Request.\n";
                    }
                    break;
                }
                case 3:
                    vm.viewVolunteers();
                    break;
                case 4:
                    vm.viewPendingVolunteerRequests();
                    break;
                case 5:
                    vm.viewAllVolunteers();
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
        case 3:
        {
            PriorityQueue pq;
            int emergencyChoice;
            do
            {
                cout << "\n-- Emergency Request Coordinator Menu --\n";
                cout << "1. Log Emergency Request\n";
                cout << "2. Request Emergency Items\n";
                cout << "3. Process Most Critical Request\n";
                cout << "4. View Pending Requests\n";
                cout << "5. View All Emergency Requests\n";
                cout << "0. Back to Main Menu\n";
                cout << "Enter choice: ";

                if (!(cin >> emergencyChoice))
                {
                    cout << "Invalid input! Please enter a number.\n";
                    clearCinBuffer();
                    continue;
                }

                switch (emergencyChoice)
                {
                case 1:
                {
                    string loc, typ, dt;
                    int urg;
                    cout << "Enter Location: ";
                    cin.ignore();
                    getline(cin, loc);
                    cout << "Enter Emergency Type (Flood, Food, etc.): ";
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
        case 4:
        {
            TransportScheduler ts;
            int transportChoice;
            do
            {
                cout << "\n-- Transport Unit Scheduler Menu --" << endl;
                cout << "1. Manage Vehicle" << endl;
                cout << "2. Assign Requests to Transport Schedule" << endl;
                cout << "3. View Current Schedule" << endl;
                cout << "0. Back to Main Menu" << endl;
                cout << "Enter choice: ";

                if (!(cin >> transportChoice))
                {
                    cout << "Invalid input! Please enter a number.\n";
                    clearCinBuffer();
                    continue;
                }

                switch (transportChoice)
                {
                case 1:
                {
                    int vehicleChoice;
                    do
                    {
                        cout << "\n-- Manage Vehicle --" << endl;
                        cout << "1. View All Vehicles" << endl;
                        cout << "2. Add New Vehicle" << endl;
                        cout << "3. Remove Vehicle" << endl;
                        cout << "0. Back to Transport Menu" << endl;
                        cout << "Enter choice: ";

                        if (!(cin >> vehicleChoice))
                        {
                            cout << "Invalid input! Please enter a number.\n";
                            clearCinBuffer();
                            continue;
                        }

                        switch (vehicleChoice)
                        {
                        case 1:
                            ts.viewAllVehicles();
                            break;
                        case 2:
                        {
                            string plateNumber;
                            cout << "Enter Car Plate Number: ";
                            cin >> plateNumber;
                            ts.addVehicle(plateNumber, "available");
                            break;
                        }
                        case 3:
                        {
                            ts.viewAllVehicles();
                            int vehicleID;
                            cout << "Enter Vehicle ID to remove: ";
                            if (!(cin >> vehicleID))
                            {
                                cout << "Invalid input! Please enter a number.\n";
                                clearCinBuffer();
                                continue;
                            }
                            ts.removeVehicle(vehicleID);
                            break;
                        }
                        case 0:
                            cout << "Returning to Transport Menu...\n";
                            break;
                        default:
                            cout << "Invalid vehicle choice.\n";
                            break;
                        }
                    } while (vehicleChoice != 0);
                    break; // This was missing!
                }
                case 2:
                {
                    ts.viewAllRequests();
                    int emergencyRequestID;
                    cout << "Enter Emergency Request ID to view the order requests: ";
                    if (!(cin >> emergencyRequestID))
                    {
                        cout << "Invalid input! Please enter a number.\n";
                        clearCinBuffer();
                        continue;
                    }

                    bool exists = ts.validatePendingRequests(emergencyRequestID);
                    if (!exists)
                    {
                        cout << "No pending requests found for Emergency Request ID: " << emergencyRequestID << endl;
                        continue;
                    }
                    else
                    {
                        ts.viewSupplyRequests(emergencyRequestID);
                        ts.viewVolunteerAssignments(emergencyRequestID);
                        int choice;
                        cout << "1. Assign Vehicle to Request\n";
                        cout << "0. Back to Transport Menu\n";
                        cout << "Enter choice: ";
                        if (!(cin >> choice))
                        {
                            cout << "Invalid input! Please enter a number.\n";
                            clearCinBuffer();
                            continue;
                        }
                        if (choice == 1)
                        {
                            ts.assignVehicleToRequest(emergencyRequestID);
                        }
                        else
                        {
                            cout << "Returning to Transport Menu...\n";
                        }
                    }
                    break;
                }
                case 3:
                {
                    ts.viewTransportOrders();
                    int choiceTransportOrder;
                    cout << "1. Free Vehicle from Transport Order\n";
                    cout << "0. Back to Transport Menu\n";
                    cout << "Enter choice: ";
                    if (!(cin >> choiceTransportOrder))
                    {
                        cout << "Invalid input! Please enter a number.\n";
                        clearCinBuffer();
                        continue;
                    }
                    if (choiceTransportOrder == 1)
                    {
                        int scheduleID;
                        cout << "Enter Schedule ID to free vehicle from transport order: ";
                        if (!(cin >> scheduleID))
                        {
                            cout << "Invalid input! Please enter a number.\n";
                            clearCinBuffer();
                            continue;
                        }
                        bool solve = ts.validateStatus(scheduleID);
                        if (!solve)
                        {
                            ts.freeVehicleFromTransportOrder(scheduleID);
                        }
                        else
                        {
                            cout << "The schedule is already resolved.\n";
                        }
                    }
                    else
                    {
                        cout << "Returning to Transport Menu...\n";
                    }
                    break;
                }
                case 0:
                    cout << "Returning to Main Menu...\n";
                    break;
                default:
                    cout << "Invalid transport menu choice.\n";
                    break;
                }

            } while (transportChoice != 0);
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
