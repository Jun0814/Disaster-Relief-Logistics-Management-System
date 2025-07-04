#include <iostream>
#include <string>
#include <limits>
using namespace std;

// ==================== DATA STRUCTURES ====================

// Stack for Supply Base Manager (LIFO - Last In, First Out)
struct SupplyBox {
    string id;
    string type;
    int quantity;
    SupplyBox* next;
};

class SupplyStack {
private:
    SupplyBox* top;
    int size;
    
public:
    SupplyStack() {
        top = nullptr;
        size = 0;
    }
    
    void push(string id, string type, int quantity) {
        SupplyBox* newBox = new SupplyBox;
        newBox->id = id;
        newBox->type = type;
        newBox->quantity = quantity;
        newBox->next = top;
        top = newBox;
        size++;
        cout << "Supply box " << id << " packed successfully!\n";
    }
    
    bool pop() {
        if (isEmpty()) {
            cout << "No supply boxes available for dispatch!\n";
            return false;
        }
        
        SupplyBox* temp = top;
        cout << "Dispatching supply box: " << temp->id << " (" << temp->type << ", Qty: " << temp->quantity << ")\n";
        top = top->next;
        delete temp;
        size--;
        return true;
    }
    
    void display() {
        if (isEmpty()) {
            cout << "No supply boxes packed.\n";
            return;
        }
        
        cout << "\n=== PACKED SUPPLY BOXES ===\n";
        SupplyBox* current = top;
        int count = 1;
        while (current != nullptr) {
            cout << count << ". ID: " << current->id << " | Type: " << current->type << " | Quantity: " << current->quantity << "\n";
            current = current->next;
            count++;
        }
        cout << "Total boxes: " << size << "\n";
    }
    
    bool isEmpty() {
        return top == nullptr;
    }
    
    int getSize() {
        return size;
    }
};

// Queue for Volunteer Operations Officer (FIFO - First In, First Out)
struct Volunteer {
    string name;
    string contact;
    string skillArea;
    Volunteer* next;
};

class VolunteerQueue {
private:
    Volunteer* front;
    Volunteer* rear;
    int size;
    
public:
    VolunteerQueue() {
        front = nullptr;
        rear = nullptr;
        size = 0;
    }
    
    void enqueue(string name, string contact, string skillArea) {
        Volunteer* newVolunteer = new Volunteer;
        newVolunteer->name = name;
        newVolunteer->contact = contact;
        newVolunteer->skillArea = skillArea;
        newVolunteer->next = nullptr;
        
        if (isEmpty()) {
            front = rear = newVolunteer;
        } else {
            rear->next = newVolunteer;
            rear = newVolunteer;
        }
        size++;
        cout << "Volunteer " << name << " registered successfully!\n";
    }
    
    bool dequeue() {
        if (isEmpty()) {
            cout << "No volunteers available for deployment!\n";
            return false;
        }
        
        Volunteer* temp = front;
        cout << "Deploying volunteer: " << temp->name << " (Contact: " << temp->contact << ", Skills: " << temp->skillArea << ")\n";
        front = front->next;
        
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        size--;
        return true;
    }
    
    void display() {
        if (isEmpty()) {
            cout << "No volunteers registered.\n";
            return;
        }
        
        cout << "\n=== REGISTERED VOLUNTEERS ===\n";
        Volunteer* current = front;
        int count = 1;
        while (current != nullptr) {
            cout << count << ". Name: " << current->name << " | Contact: " << current->contact << " | Skills: " << current->skillArea << "\n";
            current = current->next;
            count++;
        }
        cout << "Total volunteers: " << size << "\n";
    }
    
    bool isEmpty() {
        return front == nullptr;
    }
    
    int getSize() {
        return size;
    }
};

// Priority Queue for Emergency Request Coordinator
struct EmergencyRequest {
    string location;
    string type;
    int urgencyLevel; // 1 = Low, 2 = Medium, 3 = High, 4 = Critical
    EmergencyRequest* next;
};

class PriorityQueue {
private:
    EmergencyRequest* front;
    int size;
    
public:
    PriorityQueue() {
        front = nullptr;
        size = 0;
    }
    
    void enqueue(string location, string type, int urgencyLevel) {
        EmergencyRequest* newRequest = new EmergencyRequest;
        newRequest->location = location;
        newRequest->type = type;
        newRequest->urgencyLevel = urgencyLevel;
        newRequest->next = nullptr;
        
        // Insert based on priority (highest urgency first)
        if (isEmpty() || urgencyLevel > front->urgencyLevel) {
            newRequest->next = front;
            front = newRequest;
        } else {
            EmergencyRequest* current = front;
            while (current->next != nullptr && current->next->urgencyLevel >= urgencyLevel) {
                current = current->next;
            }
            newRequest->next = current->next;
            current->next = newRequest;
        }
        size++;
        cout << "Emergency request logged successfully!\n";
    }
    
    bool dequeue() {
        if (isEmpty()) {
            cout << "No emergency requests pending!\n";
            return false;
        }
        
        EmergencyRequest* temp = front;
        string urgencyText;
        switch (temp->urgencyLevel) {
            case 1: urgencyText = "Low"; break;
            case 2: urgencyText = "Medium"; break;
            case 3: urgencyText = "High"; break;
            case 4: urgencyText = "Critical"; break;
        }
        
        cout << "Processing critical request: " << temp->location << " (" << temp->type << ", Urgency: " << urgencyText << ")\n";
        front = front->next;
        delete temp;
        size--;
        return true;
    }
    
    void display() {
        if (isEmpty()) {
            cout << "No emergency requests pending.\n";
            return;
        }
        
        cout << "\n=== PENDING EMERGENCY REQUESTS ===\n";
        EmergencyRequest* current = front;
        int count = 1;
        while (current != nullptr) {
            string urgencyText;
            switch (current->urgencyLevel) {
                case 1: urgencyText = "Low"; break;
                case 2: urgencyText = "Medium"; break;
                case 3: urgencyText = "High"; break;
                case 4: urgencyText = "Critical"; break;
            }
            cout << count << ". Location: " << current->location << " | Type: " << current->type << " | Urgency: " << urgencyText << "\n";
            current = current->next;
            count++;
        }
        cout << "Total requests: " << size << "\n";
    }
    
    bool isEmpty() {
        return front == nullptr;
    }
    
    int getSize() {
        return size;
    }
};

// Circular Queue for Transport Unit Scheduler
struct Vehicle {
    string id;
    string type;
    string driver;
    Vehicle* next;
};

class CircularQueue {
private:
    Vehicle* front;
    Vehicle* rear;
    int size;
    int capacity;
    
public:
    CircularQueue(int maxCapacity = 10) {
        front = nullptr;
        rear = nullptr;
        size = 0;
        capacity = maxCapacity;
    }
    
    void enqueue(string id, string type, string driver) {
        if (size >= capacity) {
            cout << "Vehicle schedule is full!\n";
            return;
        }
        
        Vehicle* newVehicle = new Vehicle;
        newVehicle->id = id;
        newVehicle->type = type;
        newVehicle->driver = driver;
        newVehicle->next = nullptr;
        
        if (isEmpty()) {
            front = rear = newVehicle;
            rear->next = front; // Circular link
        } else {
            rear->next = newVehicle;
            rear = newVehicle;
            rear->next = front; // Circular link
        }
        size++;
        cout << "Vehicle " << id << " added to schedule successfully!\n";
    }
    
    void rotate() {
        if (isEmpty() || size == 1) {
            cout << "No vehicles to rotate or only one vehicle in schedule.\n";
            return;
        }
        
        front = front->next;
        rear = rear->next;
        cout << "Vehicle schedule rotated. " << front->id << " is now on duty.\n";
    }
    
    void display() {
        if (isEmpty()) {
            cout << "No vehicles in schedule.\n";
            return;
        }
        
        cout << "\n=== VEHICLE SCHEDULE ===\n";
        Vehicle* current = front;
        int count = 1;
        do {
            cout << count << ". ID: " << current->id << " | Type: " << current->type << " | Driver: " << current->driver;
            if (count == 1) cout << " [ON DUTY]";
            cout << "\n";
            current = current->next;
            count++;
        } while (current != front);
        cout << "Total vehicles: " << size << " | Capacity: " << capacity << "\n";
    }
    
    bool isEmpty() {
        return front == nullptr;
    }
    
    int getSize() {
        return size;
    }
};

// ==================== GLOBAL OBJECTS ====================
SupplyStack supplyStack;
VolunteerQueue volunteerQueue;
PriorityQueue emergencyQueue;
CircularQueue transportQueue(5);

// ==================== ROLE FUNCTIONS ====================

void supplyBaseManager() {
    int choice;
    string id, type;
    int quantity;
    
    do {
        cout << "\n=== SUPPLY BASE MANAGER ===\n";
        cout << "1. Pack Supply Box\n";
        cout << "2. Send Supply Package\n";
        cout << "3. View Packed Supplies\n";
        cout << "4. Return to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer after number input
        
        switch (choice) {
            case 1: {
                do {
                    cout << "Enter supply box ID: ";
                    getline(cin, id);
                    
                    if (id.empty() || id.find_first_not_of(" \t\n\r") == string::npos) {
                        cout << "Error: Supply box ID cannot be empty! Please enter a valid ID.\n";
                        continue;
                    }
                    
                    if (id.length() > 50) {
                        cout << "Error: Supply box ID is too long! Please enter a shorter ID (max 50 characters).\n";
                        continue;
                    }
                    
                    break;
                } while (true);
                
                do {
                    cout << "Select supply type:\n";
                    cout << "1. Food & Water\n";
                    cout << "2. Medical Supplies\n";
                    cout << "3. Shelter & Clothing\n";
                    cout << "4. Tools & Equipment\n";
                    cout << "5. Communication Devices\n";
                    cout << "6. Fuel & Energy\n";
                    cout << "Enter your choice (1-6): ";
                    
                    int typeChoice;
                    if (!(cin >> typeChoice)) {
                        cout << "Error: Please enter a valid number!\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                    
                    if (typeChoice < 1 || typeChoice > 6) {
                        cout << "Error: Please select a valid option (1-6)!\n";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                    
                    switch (typeChoice) {
                        case 1: type = "Food & Water"; break;
                        case 2: type = "Medical Supplies"; break;
                        case 3: type = "Shelter & Clothing"; break;
                        case 4: type = "Tools & Equipment"; break;
                        case 5: type = "Communication Devices"; break;
                        case 6: type = "Fuel & Energy"; break;
                    }
                    
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                } while (true);
                
                do {
                    cout << "Enter quantity: ";
                    
                    if (!(cin >> quantity)) {
                        cout << "Error: Please enter a valid number!\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                    
                    if (quantity <= 0) {
                        cout << "Error: Quantity must be greater than 0!\n";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                    
                    if (quantity > 10000) {
                        cout << "Error: Quantity is too large! Please enter a reasonable quantity (max 10000).\n";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                    
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                } while (true);
                
                supplyStack.push(id, type, quantity);
                break;
            }
                
            case 2:
                supplyStack.pop();
                break;
                
            case 3:
                supplyStack.display();
                break;
                
            case 4:
                cout << "Returning to main menu...\n";
                break;
                
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 4);
}

void volunteerOperationsOfficer() {
    int choice;
    string name, contact, skillArea;
    
    do {
        cout << "\n=== VOLUNTEER OPERATIONS OFFICER ===\n";
        cout << "1. Register Volunteer\n";
        cout << "2. Deploy Volunteer to Field\n";
        cout << "3. View Registered Volunteers\n";
        cout << "4. Return to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        
        switch (choice) {
            case 1: {
                do {
                    cout << "Enter volunteer name: ";
                    getline(cin, name);
                    
                    if (name.empty() || name.find_first_not_of(" \t\n\r") == string::npos) {
                        cout << "Error: Volunteer name cannot be empty! Please enter a valid name.\n";
                        continue;
                    }
                    
                    if (name.length() > 100) {
                        cout << "Error: Volunteer name is too long! Please enter a shorter name (max 100 characters).\n";
                        continue;
                    }
                    
                    break;
                } while (true);
                
                do {
                    cout << "Enter contact number: ";
                    getline(cin, contact);
                    
                    if (contact.empty() || contact.find_first_not_of(" \t\n\r") == string::npos) {
                        cout << "Error: Contact number cannot be empty! Please enter a valid contact number.\n";
                        continue;
                    }
                    
                    if (contact.length() > 20) {
                        cout << "Error: Contact number is too long! Please enter a shorter contact number (max 20 characters).\n";
                        continue;
                    }
                    
                    // Check if contact contains only digits, spaces, dashes, and plus signs
                    bool validContact = true;
                    for (char c : contact) {
                        if (!isdigit(c) && c != ' ' && c != '-' && c != '+' && c != '(' && c != ')') {
                            validContact = false;
                            break;
                        }
                    }
                    
                    if (!validContact) {
                        cout << "Error: Contact number should contain only numbers, spaces, dashes, and plus signs.\n";
                        continue;
                    }
                    
                    break;
                } while (true);
                
                do {
                    cout << "Enter skill area: ";
                    getline(cin, skillArea);
                    
                    if (skillArea.empty() || skillArea.find_first_not_of(" \t\n\r") == string::npos) {
                        cout << "Error: Skill area cannot be empty! Please enter a valid skill area.\n";
                        continue;
                    }
                    
                    if (skillArea.length() > 100) {
                        cout << "Error: Skill area is too long! Please enter a shorter skill area (max 100 characters).\n";
                        continue;
                    }
                    
                    break;
                } while (true);
                
                volunteerQueue.enqueue(name, contact, skillArea);
                break;
            }
                
            case 2:
                volunteerQueue.dequeue();
                break;
                
            case 3:
                volunteerQueue.display();
                break;
                
            case 4:
                cout << "Returning to main menu...\n";
                break;
                
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 4);
}

void emergencyRequestCoordinator() {
    int choice;
    string location, type;
    int urgencyLevel;
    
    do {
        cout << "\n=== EMERGENCY REQUEST COORDINATOR ===\n";
        cout << "1. Log Emergency Request\n";
        cout << "2. Process Most Critical Request\n";
        cout << "3. View Pending Requests\n";
        cout << "4. Return to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                do {
                    cout << "Enter location: ";
                    getline(cin, location);
                    
                    if (location.empty() || location.find_first_not_of(" \t\n\r") == string::npos) {
                        cout << "Error: Location cannot be empty! Please enter a valid location.\n";
                        continue;
                    }
                    
                    if (location.length() > 100) {
                        cout << "Error: Location is too long! Please enter a shorter location (max 100 characters).\n";
                        continue;
                    }
                    
                    break; 
                } while (true);
                
                do {
                    cout << "Enter request type (medical/food/shelter/water): ";
                    getline(cin, type);
                    
                    if (type.empty() || type.find_first_not_of(" \t\n\r") == string::npos) {
                        cout << "Error: Request type cannot be empty! Please enter a valid type.\n";
                        continue;
                    }
                    
                    string typeLower = type;
                    for (char& c : typeLower) {
                        c = tolower(c);
                    }
                    
                    if (typeLower != "medical" && typeLower != "food" && typeLower != "shelter" && typeLower != "water") {
                        cout << "Error: Invalid request type!\n";
                        cout << "Valid types: Medical, Food, Shelter, Water\n";
                        continue;
                    }
                    
                    type = typeLower;
                    type[0] = toupper(type[0]); 
                    
                    break; 
                } while (true);
                
                do {
                    cout << "Enter urgency level (1=Low, 2=Medium, 3=High, 4=Critical): ";
                    
                    if (!(cin >> urgencyLevel)) {
                        cout << "Error: Please enter a valid number!\n";
                        cin.clear(); 
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                        continue;
                    }
                    
                    if (urgencyLevel < 1 || urgencyLevel > 4) {
                        cout << "Error: Urgency level must be between 1 and 4!\n";
                        cout << "1 = Low, 2 = Medium, 3 = High, 4 = Critical\n";
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                        continue;
                    }
                    
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                    break; 
                } while (true);
                
                emergencyQueue.enqueue(location, type, urgencyLevel);
                break;
            }
                
            case 2:
                emergencyQueue.dequeue();
                break;
                
            case 3:
                emergencyQueue.display();
                break;
                
            case 4:
                cout << "Returning to main menu...\n";
                break;
                
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 4);
}

void transportUnitScheduler() {
    int choice;
    string id, type, driver;
    
    do {
        cout << "\n=== TRANSPORT UNIT SCHEDULER ===\n";
        cout << "1. Add Vehicle to Schedule\n";
        cout << "2. Rotate Vehicle Shift\n";
        cout << "3. Display Vehicle Schedule\n";
        cout << "4. Return to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                do {
                    cout << "Enter vehicle ID: ";
                    getline(cin, id);
                    
                    if (id.empty() || id.find_first_not_of(" \t\n\r") == string::npos) {
                        cout << "Error: Vehicle ID cannot be empty! Please enter a valid ID.\n";
                        continue;
                    }
                    
                    if (id.length() > 20) {
                        cout << "Error: Vehicle ID is too long! Please enter a shorter ID (max 20 characters).\n";
                        continue;
                    }
                    
                    break;
                } while (true);
                
                do {
                    cout << "Enter vehicle type: ";
                    getline(cin, type);
                    
                    if (type.empty() || type.find_first_not_of(" \t\n\r") == string::npos) {
                        cout << "Error: Vehicle type cannot be empty! Please enter a valid type.\n";
                        continue;
                    }
                    
                    if (type.length() > 50) {
                        cout << "Error: Vehicle type is too long! Please enter a shorter type (max 50 characters).\n";
                        continue;
                    }
                    
                    break;
                } while (true);
                
                do {
                    cout << "Enter driver name: ";
                    getline(cin, driver);
                    
                    if (driver.empty() || driver.find_first_not_of(" \t\n\r") == string::npos) {
                        cout << "Error: Driver name cannot be empty! Please enter a valid name.\n";
                        continue;
                    }
                    
                    if (driver.length() > 100) {
                        cout << "Error: Driver name is too long! Please enter a shorter name (max 100 characters).\n";
                        continue;
                    }
                    
                    break;
                } while (true);
                
                transportQueue.enqueue(id, type, driver);
                break;
            }
                
            case 2:
                transportQueue.rotate();
                break;
                
            case 3:
                transportQueue.display();
                break;
                
            case 4:
                cout << "Returning to main menu...\n";
                break;
                
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 4);
}

void displaySystemStatus() {
    cout << "\n=== SYSTEM STATUS ===\n";
    cout << "Supply Base: " << supplyStack.getSize() << " boxes packed\n";
    cout << "Volunteers: " << volunteerQueue.getSize() << " registered\n";
    cout << "Emergency Requests: " << emergencyQueue.getSize() << " pending\n";
    cout << "Transport Units: " << transportQueue.getSize() << " scheduled\n";
}

// ==================== MAIN FUNCTION ====================

int main() {
    int choice;
    
    cout << "==========================================\n";
    cout << "  DISASTER RELIEF LOGISTICS MANAGEMENT\n";
    cout << "              SYSTEM\n";
    cout << "==========================================\n";
    
    do {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Supply Base Manager\n";
        cout << "2. Volunteer Operations Officer\n";
        cout << "3. Emergency Request Coordinator\n";
        cout << "4. Transport Unit Scheduler\n";
        cout << "5. System Status\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer after number input
        
        switch (choice) {
            case 1:
                supplyBaseManager();
                break;
                
            case 2:
                volunteerOperationsOfficer();
                break;
                
            case 3:
                emergencyRequestCoordinator();
                break;
                
            case 4:
                transportUnitScheduler();
                break;
                
            case 5:
                displaySystemStatus();
                break;
                
            case 6:
                cout << "Thank you for using Disaster Relief Logistics Management System!\n";
                cout << "Exiting...\n";
                break;
                
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 6);
    
    return 0;
}
