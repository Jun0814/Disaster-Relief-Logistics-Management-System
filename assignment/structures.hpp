#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

// Global file names
string fileSupplyBox = "SupplyBox.csv";
string fileVolunteer = "Volunteer.csv";
string fileEmergencyRequest = "EmergencyRequest.csv";
string fileSupplyRequest = "SupplyRequest.csv";
string fileVolunteerAssignment = "VolunteerAssignment.csv";
string fileVehicle = "Vehicle.csv";
string fileTransportOrder = "TransportOrder.csv";

// Structure definitions
struct SupplyBox
{
    int supplyID;
    string type;
    int quantity;
    string status;
    string date;
};

struct Volunteer
{
    int volunteerID;
    string name;
    string contact;
    string skill;
};

// struct EmergencyRequest
// {
//     int requestID;
//     string location;
//     string type;
//     int urgency;
//     string status;
// };

// struct SupplyRequest
// {
//     int supplyRequestID;
//     int supplyID;
//     int requestID;
//     string date;
//     string status;
// };

struct SupplyNode {
    int supplyID;
    int quantity;
    SupplyNode* next;
};

struct EmergencyRequest {
    int requestID;
    string location;
    string type;
    int urgency;
    string status;
    string date;
    SupplyNode* supplies;
    EmergencyRequest* next;
};


// struct SupplyItem {
//     int supplyID;
//     int quantity;
// };

// struct EmergencySupplyRequest {
//     int requestID;                // Unique ID for the emergency request
//     std::string location;        // Location of the emergency
//     std::string type;            // Type of emergency (e.g., fire, medical)
//     int urgency;                 // Urgency level (1-10)
//     std::string status;          // Status of the request (e.g., pending, approved)
//     std::string date;            // Date of the request
//     std::map<std::string, SupplyItem> supplies; // Map of supply type to supply details
// };

struct VolunteerAssignment
{
    int assignmentID;
    int volunteerID;
    int requestID;
    string date;
    string status;
};

struct Vehicle
{
    int vehicleID;
    string plateNumber;
    string status;
};

struct TransportOrder
{
    int orderID;
    int vehicleID;
    int requestID;
    string type;
    string date;
    string status;
};

string getCurrentDateTime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << setfill('0') << setw(2) << 1 + ltm->tm_mon << "-"
       << setfill('0') << setw(2) << ltm->tm_mday << " "
       << setfill('0') << setw(2) << ltm->tm_hour << ":"
       << setfill('0') << setw(2) << ltm->tm_min << ":"
       << setfill('0') << setw(2) << ltm->tm_sec;
    return ss.str();
}

#endif