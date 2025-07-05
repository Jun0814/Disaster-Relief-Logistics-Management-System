#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct EmergencyRequest {
  int id;
  string location;
  string type;
  int urgency;
};

class PriorityQueue {
private:
  static const int MAX_SIZE = 100;
  EmergencyRequest heap[MAX_SIZE];
  int size;

  string filename = "emergency_requests.csv";

  void swap(int i, int j) {
    EmergencyRequest temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
  }

  void heapifyUp(int index) {
    if (index && heap[parent(index)].urgency < heap[index].urgency) {
      swap(index, parent(index));
      heapifyUp(parent(index));
    }
  }

  void heapifyDown(int index) {
    int left = leftChild(index);
    int right = rightChild(index);
    int largest = index;

    if (left < size && heap[left].urgency > heap[largest].urgency)
      largest = left;
    if (right < size && heap[right].urgency > heap[largest].urgency)
      largest = right;

    if (largest != index) {
      swap(index, largest);
      heapifyDown(largest);
    }
  }

  int parent(int i) { return (i - 1) / 2; }
  int leftChild(int i) { return 2 * i + 1; }
  int rightChild(int i) { return 2 * i + 2; }

public:
  PriorityQueue() { size = 0; }

  void insert(EmergencyRequest req) {
      if (size >= MAX_SIZE) {
          cout << "Queue is full." << endl;
          return;
      }
      heap[size] = req;
      heapifyUp(size);
    size++;
    cout << "Request logged. ID: " << req.id << endl;
    saveToFile();
}

void processMostCritical() {
    if (size == 0) {
        cout << "No pending requests." << endl;
        return;
    }
    cout << "Processing request ID: " << heap[0].id
          << " | Location: " << heap[0].location
          << " | Type: " << heap[0].type
         << " | Urgency: " << heap[0].urgency << endl;

  heap[0] = heap[size - 1];
  size--;
  heapifyDown(0);
  saveToFile();
}

void viewPending() {
  if (size == 0) {
    cout << "No pending requests." << endl;
    return;
  }
  cout << "Pending Emergency Requests:" << endl;
  for (int i = 0; i < size; i++) {
    cout << "ID: " << heap[i].id
         << " | Location: " << heap[i].location
         << " | Type: " << heap[i].type
         << " | Urgency: " << heap[i].urgency << endl;
  }
}

void saveToFile() {
  ofstream file(filename);
  if (!file.is_open()) {
      cout << "Error saving to file." << endl;
      return;
  }
  for (int i = 0; i < size; i++) {
      file << heap[i].id << ","
            << heap[i].location << ","
            << heap[i].type << ","
            << heap[i].urgency << "\n";
  }
  file.close();
}

void loadFromFile() {
  ifstream file(filename);
  if (!file.is_open()) {
      cout << "No saved file found. Starting fresh." << endl;
      return;
  }
  size = 0;
  while (!file.eof()) {
      EmergencyRequest req;
      string line;
      getline(file, line);
      if (line.empty()) continue;

      size_t pos1 = line.find(',');
      size_t pos2 = line.find(',', pos1 + 1);
      size_t pos3 = line.find(',', pos2 + 1);

      req.id = stoi(line.substr(0, pos1));
      req.location = line.substr(pos1 + 1, pos2 - pos1 - 1);
      req.type = line.substr(pos2 + 1, pos3 - pos2 - 1);
      req.urgency = stoi(line.substr(pos3 + 1));

      if (size < MAX_SIZE) {
        heap[size] = req;
        heapifyUp(size);
        size++;
      }
    }
    file.close();
    cout << "Loaded " << size << " requests from file." << endl;
  }

  int getSize() { return size; }
  int getLastID() { return size > 0 ? heap[size - 1].id : 0; }
};

void runEmergencyCoordinator() {
  PriorityQueue pq;
  pq.loadFromFile();

  int idCounter = pq.getLastID() + 1;

  bool running = true;
  int choice;

  while (running) {
    cout << "\n===== Emergency Request Coordinator =====" << endl;
    cout << "1. Log Emergency Request" << endl;
    cout << "2. Process Most Critical Request" << endl;
    cout << "3. View Pending Requests" << endl;
    cout << "4. Exit" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
      case 1: {
        EmergencyRequest req;
        req.id = idCounter++;
        cout << "Enter Location: ";
        cin.ignore();
        getline(cin, req.location);
        cout << "Enter Type (medical, food, etc.): ";
        getline(cin, req.type);
        cout << "Enter Urgency Level (1-10): ";
        cin >> req.urgency;
        pq.insert(req);
        break;
      }
      case 2:
        pq.processMostCritical();
        break;
      case 3:
        pq.viewPending();
        break;
      case 4:
        running = false;
        cout << "Exiting Emergency Coordinator Module." << endl;
        break;
      default:
        cout << "Invalid option. Please try again." << endl;
    }
  }
}

int main() {
  runEmergencyCoordinator();
  return 0;
}
