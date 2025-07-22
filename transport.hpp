#include <iostream>
#include <string>
#include <fstream>
#include "structures.hpp"

using namespace std;

class TransportScheduler
{

private:
    VehicleNode* vehicleTail;
    SupplyRequestNode* requestTail;
    TransportOrderNode* orderTail;
    EmergencyRequestNode* emergencyTail;
    VolunteerAssignmentNode* volunteerAssignmentsTail;
    SupplyNode* supplyTail;
    VolunteerNode* volunteerTail;

    int incrementVehicleID = 1;
    int incrementTransportID = 1;

public:
    TransportScheduler()
    {
        vehicleTail = nullptr;
        requestTail = nullptr;
        orderTail = nullptr;
        emergencyTail = nullptr;
        volunteerAssignmentsTail = nullptr;
        supplyTail = nullptr;
        volunteerTail = nullptr;
        readVehiclesFromFile();
        readSupplyRequestsFromFile();
        readTransportOrdersFromFile();
        readEmergencyRequestsFromFile();
        readVolunteerAssignmentsFromFile();
        readSupplyBoxFromFile();
        readVolunteersFromFile();
    }

    ~TransportScheduler()
    {
        if (!vehicleTail)
            return;

        // Vehicle
        VehicleNode* start = vehicleTail->next;
        VehicleNode* current = start;
        do
        {
            VehicleNode* nextNode = current->next;
            delete current;
            current = nextNode;
        } while (current != start);
        vehicleTail = nullptr;

        // Supply Requests
        if (!requestTail)
            return;
        SupplyRequestNode* startRequest = requestTail->next;
        SupplyRequestNode* currentRequest = startRequest;
        do
        {
            SupplyRequestNode* nextNode = currentRequest->next;
            delete currentRequest;
            currentRequest = nextNode;
        } while (currentRequest != startRequest);
        requestTail = nullptr;

        // Transport Order
        if (!orderTail)
            return;
        TransportOrderNode* startOrder = orderTail->next;
        TransportOrderNode* currentOrder = startOrder;
        do
        {
            TransportOrderNode* nextNode = currentOrder->next;
            delete currentOrder;
            currentOrder = nextNode;
        } while (currentOrder != startOrder);
        orderTail = nullptr;

        // Emergency Requests
        if (!emergencyTail)
            return;
        EmergencyRequestNode* startEmergency = emergencyTail->next;
        EmergencyRequestNode* currentEmergency = startEmergency;
        do
        {
            EmergencyRequestNode* nextNode = currentEmergency->next;
            delete currentEmergency;
            currentEmergency = nextNode;
        } while (currentEmergency != startEmergency);
        emergencyTail = nullptr;

        // Volunteer Requests
        if (!volunteerAssignmentsTail)
            return;
        VolunteerAssignmentNode* startVolunteer = volunteerAssignmentsTail->next;
        VolunteerAssignmentNode* currentVolunteer = startVolunteer;
        do
        {
            VolunteerAssignmentNode* nextNode = currentVolunteer->next;
            delete currentVolunteer;
            currentVolunteer = nextNode;
        } while (currentVolunteer != startVolunteer);
        volunteerAssignmentsTail = nullptr;

        // Supply
        if (!supplyTail)
            return;
        SupplyNode* startSupply = supplyTail->next;
        SupplyNode* currentSupply = startSupply;
        do
        {
            SupplyNode* nextNode = currentSupply->next;
            delete currentSupply;
            currentSupply = nextNode;
        } while (currentSupply != startSupply);
        supplyTail = nullptr;

        // Volunteers
        if (!volunteerTail)
            return;
        VolunteerNode* startVolunteerNode = volunteerTail->next;
        VolunteerNode* currentVolunteerNode = startVolunteerNode;

        do
        {
            VolunteerNode* nextNode = currentVolunteerNode->next;
            delete currentVolunteerNode;
            currentVolunteerNode = nextNode;
        } while (currentVolunteerNode != startVolunteerNode);
        volunteerTail = nullptr;
    }

    void readVehiclesFromFile()
    {
        ifstream file(fileVehicle);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << fileVehicle << endl;
            return;
        }

        string line;
        getline(file, line);
        while (getline(file, line))
        {
            istringstream iss(line);
            string idStr, plate, status;
            if (getline(iss, idStr, ',') && getline(iss, plate, ',') && getline(iss, status))
            {
                try
                {
                    int id = stoi(idStr);
                    if (id <= 0)
                    {
                        cerr << "Invalid vehicle ID in line: " << line << endl;
                        continue;
                    }
                    if (plate.empty())
                    {
                        cerr << "Empty plate number in line: " << line << endl;
                        continue;
                    }
                    if (status != "available" && status != "unavailable")
                    {
                        cerr << "Invalid status in line: " << line << endl;
                        continue;
                    }
                    Vehicle vehicle = { id, plate, status };
                    VehicleNode* newNode = new VehicleNode{ vehicle, nullptr };
                    if (!vehicleTail)
                    {
                        vehicleTail = newNode;
                        vehicleTail->next = vehicleTail;
                    }
                    else
                    {
                        newNode->next = vehicleTail->next;
                        vehicleTail->next = newNode;
                        vehicleTail = newNode;
                    }
                    incrementVehicleID = max(incrementVehicleID, id + 1);
                }
                catch (const exception& e)
                {
                    cerr << "Error parsing line: " << line << " (" << e.what() << ")\n";
                }
            }
            else
            {
                cerr << "Malformed line: " << line << endl;
            }
        }
        file.close();
    }

    void readEmergencyRequestsFromFile()
    {
        ifstream file(fileEmergencyRequest);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << fileEmergencyRequest << endl;
            return;
        }

        string line;
        getline(file, line);
        while (getline(file, line))
        {
            istringstream iss(line);
            string requestIDStr, location, type, urgencyStr, status, date;
            if (getline(iss, requestIDStr, ',') &&
                getline(iss, location, ',') &&
                getline(iss, type, ',') &&
                getline(iss, urgencyStr, ',') &&
                getline(iss, status, ',') &&
                getline(iss, date))
            {
                try
                {
                    int requestID = stoi(requestIDStr);
                    int urgency = stoi(urgencyStr);
                    EmergencyRequest request = { requestID, location, type, urgency, status, date };
                    EmergencyRequestNode* newNode = new EmergencyRequestNode{ request, nullptr };
                    if (!emergencyTail)
                    {
                        emergencyTail = newNode;
                        emergencyTail->next = emergencyTail;
                    }
                    else
                    {
                        newNode->next = emergencyTail->next;
                        emergencyTail->next = newNode;
                        emergencyTail = newNode;
                    }
                }
                catch (const exception& e)
                {
                    cerr << "Error parsing line: " << line << " (" << e.what() << ")\n";
                }
            }
            else
            {
                cerr << "Malformed line: " << line << endl;
            }
        }
        file.close();
    }

    void readSupplyRequestsFromFile()
    {
        ifstream file(fileSupplyRequest);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << fileSupplyRequest << endl;
            return;
        }

        string line;
        getline(file, line);
        while (getline(file, line))
        {
            istringstream iss(line);
            string supplyRequestIDStr, supplyIDStr, requestIDStr, quantityStr, date, status;
            if (getline(iss, supplyRequestIDStr, ',') &&
                getline(iss, supplyIDStr, ',') &&
                getline(iss, requestIDStr, ',') &&
                getline(iss, quantityStr, ',') &&
                getline(iss, date, ',') &&
                getline(iss, status))
            {
                try
                {
                    int supplyRequestID = stoi(supplyRequestIDStr);
                    int supplyID = stoi(supplyIDStr);
                    int requestID = stoi(requestIDStr);
                    int quantity = stoi(quantityStr);
                    SupplyRequest request = { supplyRequestID, supplyID, requestID, quantity, date, status };
                    SupplyRequestNode* newNode = new SupplyRequestNode{ request, nullptr };
                    if (!requestTail)
                    {
                        requestTail = newNode;
                        requestTail->next = requestTail;
                    }
                    else
                    {
                        newNode->next = requestTail->next;
                        requestTail->next = newNode;
                        requestTail = newNode;
                    }
                }
                catch (const exception& e)
                {
                    cerr << "Error parsing line: " << line << " (" << e.what() << ")\n";
                }
            }
            else
            {
                cerr << "Malformed line: " << line << endl;
            }
        }
        file.close();
    }

    void readTransportOrdersFromFile()
    {
        ifstream file(fileTransportOrder);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << fileTransportOrder << endl;
            return;
        }

        string line;
        getline(file, line);
        while (getline(file, line))
        {
            istringstream iss(line);
            string transportIDStr, vehicleIDStr, supplyRequestIDStr, type, date, status;
            if (getline(iss, transportIDStr, ',') &&
                getline(iss, vehicleIDStr, ',') &&
                getline(iss, supplyRequestIDStr, ',') &&
                getline(iss, type, ',') &&
                getline(iss, date, ',') &&
                getline(iss, status))
            {
                try
                {
                    int transportID = stoi(transportIDStr);
                    int vehicleID = stoi(vehicleIDStr);
                    int supplyRequestID = stoi(supplyRequestIDStr);
                    TransportOrder order = { transportID, vehicleID, supplyRequestID, type, date, status };
                    TransportOrderNode* newNode = new TransportOrderNode{ order, nullptr };
                    if (!orderTail)
                    {
                        orderTail = newNode;
                        orderTail->next = orderTail;
                    }
                    else
                    {
                        newNode->next = orderTail->next;
                        orderTail->next = newNode;
                        orderTail = newNode;
                    }
                }
                catch (const exception& e)
                {
                    cerr << "Error parsing line: " << line << " (" << e.what() << ")\n";
                }
            }
            else
            {
                cerr << "Malformed line: " << line << endl;
            }
        }
        file.close();
    }

    void readVolunteerAssignmentsFromFile()
    {
        ifstream file(fileVolunteerAssignment);
        if (!file.is_open())
        {
            cerr << "Error opening file: Volunteer Assignment.csv" << endl;
            return;
        }

        string line;
        getline(file, line);
        while (getline(file, line))
        {
            istringstream iss(line);
            string assignmentIDStr, volunteerIDStr, requestIDStr, date, status;
            if (getline(iss, assignmentIDStr, ',') &&
                getline(iss, volunteerIDStr, ',') &&
                getline(iss, requestIDStr, ',') &&
                getline(iss, date, ',') &&
                getline(iss, status))
            {
                try
                {
                    int assignmentID = stoi(assignmentIDStr);
                    int volunteerID = stoi(volunteerIDStr);
                    int requestID = stoi(requestIDStr);
                    VolunteerAssignment assignment = { assignmentID, volunteerID, requestID, date, status };
                    VolunteerAssignmentNode* newNode = new VolunteerAssignmentNode{ assignment, nullptr };
                    if (!volunteerAssignmentsTail)
                    {
                        volunteerAssignmentsTail = newNode;
                        volunteerAssignmentsTail->next = volunteerAssignmentsTail;
                    }
                    else
                    {
                        newNode->next = volunteerAssignmentsTail->next;
                        volunteerAssignmentsTail->next = newNode;
                        volunteerAssignmentsTail = newNode;
                    }
                }
                catch (const exception& e)
                {
                    cerr << "Error parsing line: " << line << " (" << e.what() << ")\n";
                }
            }
            else
            {
                cerr << "Malformed line: " << line << endl;
            }
        }
        file.close();
    }

    void readSupplyBoxFromFile()
    {
        ifstream file(fileSupplyBox);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << fileSupplyBox << endl;
            return;
        }

        string line;
        getline(file, line);
        while (getline(file, line))
        {
            istringstream iss(line);
            string supplyIDStr, type, quantityStr;
            if (getline(iss, supplyIDStr, ',') && getline(iss, type) && getline(iss, quantityStr))
            {
                try
                {
                    int supplyID = stoi(supplyIDStr);
                    int quantity = stoi(quantityStr);
                    SupplyNode* newNode = new SupplyNode{ supplyID, type, quantity, nullptr };
                    if (!supplyTail)
                    {
                        supplyTail = newNode;
                        supplyTail->next = supplyTail;
                    }
                    else
                    {
                        newNode->next = supplyTail->next;
                        supplyTail->next = newNode;
                        supplyTail = newNode;
                    }
                }
                catch (const exception& e)
                {
                    cerr << "Error parsing line: " << line << " (" << e.what() << ")\n";
                }
            }
            else
            {
                cerr << "Malformed line: " << line << endl;
            }
        }
        file.close();
    }

    void readVolunteersFromFile()
    {
        ifstream file(fileVolunteer);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << fileVolunteer << endl;
            return;
        }

        string line;
        getline(file, line);
        while (getline(file, line))
        {
            istringstream iss(line);
            string idStr, name, contact, skill;
            if (getline(iss, idStr, ',') && getline(iss, name, ',') && getline(iss, contact, ',') && getline(iss, skill))
            {
                try
                {
                    int id = stoi(idStr);
                    VolunteerNode* newNode = new VolunteerNode{ id, name, contact, skill, nullptr };
                    if (!volunteerTail)
                    {
                        volunteerTail = newNode;
                        volunteerTail->next = volunteerTail;
                    }
                    else
                    {
                        newNode->next = volunteerTail->next;
                        volunteerTail->next = newNode;
                        volunteerTail = newNode;
                    }
                }
                catch (const exception& e)
                {
                    cerr << "Error parsing line: " << line << " (" << e.what() << ")\n";
                }
            }
            else
            {
                cerr << "Malformed line: " << line << endl;
            }
        }
        file.close();
    }

    void addVehicleIntoFile(const Vehicle& vehicle)
    {
        ofstream file(fileVehicle, ios::app);
        if (!file)
        {
            cerr << "Error opening file: vehicles.csv" << endl;
            return;
        }
        if (file.tellp() == 0)
        {
            file << "Vehicle ID,Plate Number,Status\n";
        }

        file << vehicle.vehicleID << ","
            << vehicle.plateNumber << ","
            << vehicle.status << endl;

        file.close();
    }

    void addVehicle(string plateNumber, string status = "available")
    {
        Vehicle newVehicle = { incrementVehicleID++, plateNumber, status };

        if (vehicleTail)
        {
            VehicleNode* curr = vehicleTail->next;
            do
            {
                if (curr->vehicle.plateNumber == newVehicle.plateNumber)
                {
                    cout << "Vehicle with car plate (" << newVehicle.plateNumber << ") already exists in the system!\n";
                    return;
                }
                curr = curr->next;
            } while (curr != vehicleTail->next);
        }

        VehicleNode* newNode = new VehicleNode{ newVehicle, nullptr };

        if (!vehicleTail)
        {
            vehicleTail = newNode;
            vehicleTail->next = vehicleTail;
        }
        else
        {
            newNode->next = vehicleTail->next;
            vehicleTail->next = newNode;
            vehicleTail = newNode;
        }

        addVehicleIntoFile(newVehicle);

        cout << "Vehicle added: ID " << newVehicle.vehicleID
            << ", Car Plate: " << newVehicle.plateNumber
            << ", Status: " << newVehicle.status << endl;
    }

    void rotateVehicleShift()
    {
        if (!vehicleTail || !vehicleTail->next)
        {
            cout << "No vehicles to rotate in schedule.\n";
            return;
        }

        vehicleTail = vehicleTail->next;
        cout << "Vehicle schedule rotated. Vehicle " << vehicleTail->next->vehicle.plateNumber
            << " (ID: " << vehicleTail->next->vehicle.vehicleID << ") is now on duty.\n";
    }

    void viewAllVehicles()
    {
        if (!vehicleTail)
        {
            cout << "No vehicles available.\n";
            return;
        }

        cout << "\n-- All Vehicles --\n";
        cout << "------------------------------------------------\n";
        cout << "| " << left << setw(12) << "Vehicle ID"
            << "| " << setw(15) << "Plate Number"
            << "| " << setw(13) << "Status" << "|\n";
        cout << "------------------------------------------------\n";

        VehicleNode* curr = vehicleTail->next;
        do
        {
            cout << "| " << left << setw(12) << curr->vehicle.vehicleID
                << "| " << setw(15) << curr->vehicle.plateNumber
                << "| " << setw(13) << curr->vehicle.status << "|\n";
            curr = curr->next;
        } while (curr != vehicleTail->next);

        cout << "------------------------------------------------\n";
    }

    void updateVehicle()
    {
        ofstream file(fileVehicle);
        if (!file)
        {
            cerr << "Error opening file: vehicles.csv" << endl;
            return;
        }

        file << "Vehicle ID,Plate Number,Status\n";
        if (vehicleTail)
        {
            VehicleNode* currWrite = vehicleTail->next;
            do
            {
                file << currWrite->vehicle.vehicleID << ","
                    << currWrite->vehicle.plateNumber << ","
                    << currWrite->vehicle.status << "\n";
                currWrite = currWrite->next;
            } while (currWrite != vehicleTail->next);
        }
        file.close();
    }

    void removeVehicle(int vehicleID)
    {
        VehicleNode* curr = vehicleTail->next;
        VehicleNode* prev = vehicleTail;

        do
        {
            if (curr->vehicle.vehicleID == vehicleID)
            {
                if (curr == vehicleTail)
                {
                    if (curr->next == curr)
                    {
                        delete curr;
                        vehicleTail = nullptr;
                    }
                    else
                    {
                        prev->next = curr->next;
                        delete curr;
                        vehicleTail = prev;
                    }
                }
                else
                {
                    prev->next = curr->next;
                    delete curr;
                }
                cout << "Vehicle with ID " << vehicleID << " removed successfully.\n";

                updateVehicle();
                return;
            }
            prev = curr;
            curr = curr->next;
        } while (curr != vehicleTail->next);

        cout << "Vehicle with ID " << vehicleID << " not found.\n";
    }

    void viewAllRequests()
    {
        if (!emergencyTail)
        {
            cout << "No ongoing emergency requests.\n";
            return;
        }

        cout << "\n-- All Ongoing Emergency Requests --\n";
        cout << "------------------------------------------------------------------------------------------\n";
        cout << "| " << left << setw(21) << "Emergency Request ID"
            << "| " << setw(15) << "Location"
            << "| " << setw(15) << "Date"
            << "| " << setw(12) << "Urgency"
            << "| " << setw(12) << "Status" << "|\n";
        cout << "------------------------------------------------------------------------------------------\n";

        EmergencyRequestNode* curr = emergencyTail->next;
        do
        {
            if (curr->request.status == "Assigned")
            {
                cout << "| " << left << setw(21) << curr->request.requestID
                    << "| " << setw(15) << curr->request.location
                    << "| " << setw(15) << curr->request.date
                    << "| " << setw(12) << curr->request.urgency
                    << "| " << setw(12) << curr->request.status << "|\n";
            }
            curr = curr->next;
        } while (curr != emergencyTail->next);

        cout << "------------------------------------------------------------------------------------------\n";
    }

    bool validatePendingRequests(int emergencyRequestID)
    {
        EmergencyRequestNode* currEmergency = emergencyTail->next;
        bool emergencyExists = false;
        do
        {
            if (currEmergency->request.requestID == emergencyRequestID &&
                currEmergency->request.status == "Assigned")
            {
                cout << "Emergency Request ID: " << emergencyRequestID
                    << " is currently assigned and has pending requests.\n";
                emergencyExists = true;
                return emergencyExists;
            }
            currEmergency = currEmergency->next;
        } while (currEmergency != emergencyTail->next);

        return false;
    }

    void viewSupplyRequests(int emergencyRequestID)
    {
        cout << "\n-- All Supply Requests for Emergency Request ID: "
            << emergencyRequestID
            << " --"
            << endl;
        cout << "---------------------------------------------------------------------------------\n";
        cout << "| " << left << setw(18) << "Supply Request ID"
            << "| " << setw(10) << "Supply ID"
            << "| " << setw(11) << "Request ID"
            << "| " << setw(9) << "Quantity"
            << "| " << setw(10) << "Date Time"
            << "| " << setw(10) << "Status" << "|\n";
        cout << "---------------------------------------------------------------------------------\n";

        SupplyRequestNode* curr = requestTail->next;
        do
        {
            if (curr->request.requestID == emergencyRequestID)
            {
                cout << "| " << left << setw(18) << curr->request.supplyRequestID
                    << "| " << setw(10) << curr->request.supplyID
                    << "| " << setw(11) << curr->request.requestID
                    << "| " << setw(9) << curr->request.quantity
                    << "| " << setw(10) << curr->request.date
                    << "| " << setw(10) << curr->request.status << "|\n";
            }
            curr = curr->next;
        } while (curr != requestTail->next);

        cout << "---------------------------------------------------------------------------------\n";
    }

    void viewVolunteerAssignments(int emergencyRequestID)
    {
        if (!volunteerAssignmentsTail || !volunteerAssignmentsTail->next)
        {
            cout << "No volunteer assignments available.\n";
            return;
        }

        cout << "\n-- All Volunteer Assignments "
            << "for Emergency Request ID: "
            << emergencyRequestID
            << " --\n";
        cout << "--------------------------------------------------------------------------------------\n";
        cout << "| " << left << setw(14) << "Assignment ID"
            << "| " << setw(13) << "Volunteer ID"
            << "| " << setw(15) << "Volunteer Name"
            << "| " << setw(11) << "Request ID"
            << "| " << setw(10) << "Date"
            << "| " << setw(10) << "Status" << "|\n";
        cout << "--------------------------------------------------------------------------------------\n";

        VolunteerAssignmentNode* curr = volunteerAssignmentsTail->next;
        do
        {
            if (curr->assignment.requestID == emergencyRequestID)
            {
                string volunteerName = "Unknown";
                VolunteerNode* vol = volunteerTail->next;
                do
                {
                    if (vol->volunteer.volunteerID == curr->assignment.volunteerID)
                    {
                        volunteerName = vol->volunteer.name;
                        break;
                    }
                    vol = vol->next;
                } while (vol && vol != volunteerTail->next);

                cout << "| " << left << setw(14) << curr->assignment.assignmentID
                    << "| " << setw(13) << curr->assignment.volunteerID
                    << "| " << setw(15) << volunteerName
                    << "| " << setw(11) << curr->assignment.requestID
                    << "| " << setw(10) << curr->assignment.date
                    << "| " << setw(10) << curr->assignment.status << "|\n";
            }
            curr = curr->next;
        } while (curr && curr != volunteerAssignmentsTail->next);

        cout << "--------------------------------------------------------------------------------------\n";
    }

    void addTransportOrderIntoFile(const TransportOrder& order)
    {
        ofstream file(fileTransportOrder, ios::app);
        if (!file)
        {
            cerr << "Error opening file: transport_order.csv" << endl;
            return;
        }
        if (file.tellp() == 0)
        {
            file << "Transport ID,Vehicle ID,Supply Request ID,Type,Date,Status\n";
        }
        file << order.orderID << ","
            << order.vehicleID << ","
            << order.requestID << ","
            << order.type << ","
            << order.date << ","
            << order.status << endl;

        file.close();
    }

    void updateSupplyRequest()
    {
        ofstream supplyFile(fileSupplyRequest);
        if (!supplyFile)
        {
            cerr << "Error opening file: supply_request.csv" << endl;
            return;
        }
        supplyFile << "supplyRequestID,supplyID,requestID,quantity,date,status\n";
        if (requestTail)
        {
            SupplyRequestNode* currWrite = requestTail->next;
            do
            {
                supplyFile << currWrite->request.supplyRequestID << ","
                    << currWrite->request.supplyID << ","
                    << currWrite->request.requestID << ","
                    << currWrite->request.quantity << ","
                    << currWrite->request.date << ","
                    << currWrite->request.status << "\n";
                currWrite = currWrite->next;
            } while (currWrite != requestTail->next);
        }
        supplyFile.close();
    }

    void updateVolunteerAssignment()
    {
        ofstream volunteerFile(fileVolunteerAssignment);
        if (!volunteerFile)
        {
            cerr << "Error opening file: volunteer_assignment.csv" << endl;
            return;
        }
        volunteerFile << "assignmentID,volunteerID,requestID,date,status\n";
        if (volunteerAssignmentsTail)
        {
            VolunteerAssignmentNode* currWrite = volunteerAssignmentsTail->next;
            do
            {
                volunteerFile << currWrite->assignment.assignmentID << ","
                    << currWrite->assignment.volunteerID << ","
                    << currWrite->assignment.requestID << ","
                    << currWrite->assignment.date << ","
                    << currWrite->assignment.status << "\n";
                currWrite = currWrite->next;
            } while (currWrite != volunteerAssignmentsTail->next);
        }
        volunteerFile.close();
    }

    void updateEmergencyRequest()
    {
        ofstream emergencyFile(fileEmergencyRequest);
        if (!emergencyFile)
        {
            cerr << "Error opening file: emergency_request.csv" << endl;
            return;
        }
        emergencyFile << "requestID,location,type,urgency,status,date\n";
        if (emergencyTail)
        {
            EmergencyRequestNode* currWrite = emergencyTail->next;
            do
            {
                emergencyFile << currWrite->request.requestID << ","
                    << currWrite->request.location << ","
                    << currWrite->request.type << ","
                    << currWrite->request.urgency << ","
                    << currWrite->request.status << ","
                    << currWrite->request.date << "\n";
                currWrite = currWrite->next;
            } while (currWrite != emergencyTail->next);
        }
        emergencyFile.close();
    }

    void assignVehicleToRequest(int emergencyRequestID)
    {
        if (!vehicleTail || !vehicleTail->next)
        {
            cout << "No vehicles available for assignment.\n";
            return;
        }

        VehicleNode* firstVehicle = vehicleTail->next;

        VehicleNode* curr = vehicleTail->next;
        bool unavailable = true;

        do
        {
            if (curr->vehicle.status == "available")
            {
                unavailable = false;
                break;
            }
            curr = curr->next;
        } while (curr != vehicleTail->next);

        if (unavailable)
        {
            cout << "All vehicles are currently unavailable for assignment.\n";
            return;
        }

        if (firstVehicle->vehicle.status != "available")
        {
            cout << "First vehicle (ID: " << firstVehicle->vehicle.vehicleID
                << ") is not available for assignment.\n";
            return;
        }

        firstVehicle->vehicle.status = "unavailable";
        cout << "Vehicle with ID " << firstVehicle->vehicle.vehicleID
            << " assigned to Emergency Request ID: "
            << emergencyRequestID << endl;

        rotateVehicleShift();

        updateVehicle();

        SupplyRequestNode* supplyCurr = requestTail->next;
        bool foundSupply = false;
        do
        {
            if (supplyCurr->request.requestID == emergencyRequestID)
            {
                supplyCurr->request.status = "Deployed";
                foundSupply = true;
            }
            supplyCurr = supplyCurr->next;
        } while (supplyCurr != requestTail->next);

        VolunteerAssignmentNode* volunteerCurr = volunteerAssignmentsTail->next;
        bool foundVolunteer = false;
        do
        {
            if (volunteerCurr->assignment.requestID == emergencyRequestID)
            {
                volunteerCurr->assignment.status = "Deployed";
                foundVolunteer = true;
            }
            volunteerCurr = volunteerCurr->next;
        } while (volunteerCurr != volunteerAssignmentsTail->next);

        string type;
        if (foundSupply && foundVolunteer)
        {
            type = "Both";
        }
        else if (foundSupply)
        {
            type = "Supply";
        }
        else if (foundVolunteer)
        {
            type = "Volunteer";
        }

        TransportOrder newOrder = { incrementTransportID++, firstVehicle->vehicle.vehicleID, emergencyRequestID, type, getCurrentDateTime(), "Deployed" };
        TransportOrderNode* newNode = new TransportOrderNode{ newOrder, nullptr };

        if (!orderTail)
        {
            orderTail = newNode;
            orderTail->next = orderTail;
        }
        else
        {
            newNode->next = orderTail->next;
            orderTail->next = newNode;
            orderTail = newNode;
        }

        EmergencyRequestNode* emergencyCurr = emergencyTail->next;
        do
        {
            if (emergencyCurr->request.requestID == emergencyRequestID)
            {
                emergencyCurr->request.status = "Deployed";
                break;
            }
            emergencyCurr = emergencyCurr->next;
        } while (emergencyCurr != emergencyTail->next);

        addTransportOrderIntoFile(newOrder);
        updateSupplyRequest();
        updateVolunteerAssignment();
        updateEmergencyRequest();
    }

    void viewTransportOrders()
    {
        if (!orderTail || !orderTail->next)
        {
            cout << "No Schedule orders available.\n";
            return;
        }

        cout << "\n-- All Schedule Orders --\n";
        cout << "-------------------------------------------------------------------------------------------------------\n";
        cout << "| " << left << setw(13) << "Schedule ID"
            << "| " << setw(13) << "Vehicle ID"
            << "| " << setw(23) << "Emergency Request ID"
            << "| " << setw(10) << "Type"
            << "| " << setw(21) << "Date"
            << "| " << setw(10) << "Status" << "|\n";
        cout << "-------------------------------------------------------------------------------------------------------\n";

        TransportOrderNode* curr = orderTail->next;
        do
        {
            cout << "| " << left << setw(13) << curr->order.orderID
                << "| " << setw(13) << curr->order.vehicleID
                << "| " << setw(23) << curr->order.requestID
                << "| " << setw(10) << curr->order.type
                << "| " << setw(21) << curr->order.date
                << "| " << setw(10) << curr->order.status << "|\n";
            curr = curr->next;
        } while (curr != orderTail->next);

        cout << "-------------------------------------------------------------------------------------------------------\n";
    }

    void updateTransportOrder()
    {
        ofstream file(fileTransportOrder);
        if (!file)
        {
            cerr << "Error opening file: transport_order.csv" << endl;
            return;
        }

        file << "Transport ID,Vehicle ID,Supply Request ID,Type,Date,Status\n";
        if (orderTail)
        {
            TransportOrderNode* currWrite = orderTail->next;
            do
            {
                file << currWrite->order.orderID << ","
                    << currWrite->order.vehicleID << ","
                    << currWrite->order.requestID << ","
                    << currWrite->order.type << ","
                    << currWrite->order.date << ","
                    << currWrite->order.status << "\n";
                currWrite = currWrite->next;
            } while (currWrite != orderTail->next);
        }
        file.close();
    }

    void freeVehicleFromTransportOrder(int scheduleID)
    {
        if (!orderTail || !orderTail->next)
        {
            cout << "No Schedule orders available.\n";
            return;
        }

        TransportOrderNode* curr = orderTail->next;
        TransportOrderNode* prev = orderTail;
        int vehicleID;

        do
        {
            if (curr->order.orderID == scheduleID)
            {
                vehicleID = curr->order.vehicleID;
                VehicleNode* vehicleCurr = vehicleTail->next;
                do
                {
                    if (vehicleCurr->vehicle.vehicleID == vehicleID)
                    {
                        vehicleCurr->vehicle.status = "available";
                        break;
                    }
                    vehicleCurr = vehicleCurr->next;
                } while (vehicleCurr != vehicleTail->next);
                updateVehicle();

                int emergencyRequestID = curr->order.requestID;
                EmergencyRequestNode* emergencyCurr = emergencyTail->next;
                do
                {
                    if (emergencyCurr->request.requestID == emergencyRequestID)
                    {
                        emergencyCurr->request.status = "Resolved";
                        break;
                    }
                    emergencyCurr = emergencyCurr->next;
                } while (emergencyCurr != emergencyTail->next);
                updateEmergencyRequest();

                if (curr->order.type == "Supply" || curr->order.type == "Both")
                {
                    SupplyRequestNode* supplyCurr = requestTail->next;
                    do
                    {
                        if (supplyCurr->request.requestID == emergencyRequestID)
                        {
                            supplyCurr->request.status = "Resolved";
                        }
                        supplyCurr = supplyCurr->next;
                    } while (supplyCurr != requestTail->next);
                    updateSupplyRequest();
                }
                if (curr->order.type == "Volunteer" || curr->order.type == "Both")
                {
                    VolunteerAssignmentNode* volunteerCurr = volunteerAssignmentsTail->next;
                    do
                    {
                        if (volunteerCurr->assignment.requestID == emergencyRequestID)
                        {
                            volunteerCurr->assignment.status = "Resolved";
                        }
                        volunteerCurr = volunteerCurr->next;
                    } while (volunteerCurr != volunteerAssignmentsTail->next);
                    updateVolunteerAssignment();
                }

                curr->order.status = "Resolved";
                cout << "Vehicle with ID " << vehicleID << " free from transport order.\n";
                updateTransportOrder();
                return;
            }
            prev = curr;
            curr = curr->next;
        } while (curr != orderTail->next);

        cout << "Vehicle with ID " << vehicleID << " not found in transport orders.\n";
    }

    bool validateStatus(int scheduleID)
    {
        TransportOrderNode* curr = orderTail->next;
        do
        {
            if (curr->order.orderID == scheduleID && curr->order.status == "Resolved")
            {
                return true;
            }
            curr = curr->next;
        } while (curr != orderTail->next);
        return false;
    }
};

