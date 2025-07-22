#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

// Global file names
string fileSupplyBox = "database/SupplyBox.csv";
string fileVolunteer = "database/Volunteer.csv";
string fileEmergencyRequest = "database/EmergencyRequest.csv";
string fileSupplyRequest = "database/SupplyRequest.csv";
string fileVolunteerAssignment = "database/VolunteerAssignment.csv";
string fileVehicle = "database/Vehicle.csv";
string fileTransportOrder = "database/TransportOrder.csv";
string fileVolunteerRequest = "database/VolunteerRequest.csv";

// Structure definitions
struct SupplyBox
{
    int supplyID;
    string type;
    int quantity;
};

struct Volunteer
{
    int volunteerID;
    string name;
    string contact;
    string skill;
};

struct EmergencyRequest
{
    int requestID;
    string location;
    string type;
    int urgency;
    string status;
    string date;
};

struct SupplyRequest
{
    int supplyRequestID;
    int supplyID;
    int requestID;
    int quantity;
    string date;
    string status;
};

struct VolunteerRequest
{
    int volunteerRequestID;
    int requestID;
    int quantity;
    string date;
    string status;
};

struct VolunteerAssignment
{
    int assignmentID;
    int volunteerID;
    int requestID;
    string date;
    string status;
};

struct SupplyNode
{
    SupplyBox supply;
    SupplyNode *next;
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

struct VehicleNode
{
    Vehicle vehicle;
    VehicleNode *next;
};

struct VolunteerNode
{
    Volunteer volunteer;
    VolunteerNode *next;
};

struct VolunteerRequestNode
{
    VolunteerRequest request;
    VolunteerRequestNode *next;
};

struct VolunteerAssignmentNode
{
    VolunteerAssignment assignment;
    VolunteerAssignmentNode *next;
};

struct SupplyRequestNode
{
    SupplyRequest request;
    SupplyRequestNode *next;
};

struct TransportOrderNode
{
    TransportOrder order;
    TransportOrderNode *next;
};

struct EmergencyRequestNode
{
    EmergencyRequest request;
    EmergencyRequestNode *next;
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