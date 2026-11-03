#include <iostream>
#include <string>
using namespace std;

const int MAX_VEHICLES = 100;
const int MAX_STATIONS = 20;
const int MAX_QUEUE = 100;
const int MAX_STACK = 100;

// Structure to store vehicle details
struct Vehicle
{
    int id;
    string name;
    int battery;
    string connector;
    int priority;
    string status;
};

// Structure to store charging station details
struct Station
{
    int slotId;
    string connector;
    bool available;
    int vehicleId;
};

// Linked List Node for Assignment History
struct Node
{
    string details;
    Node* next;
};

// Global arrays
Vehicle vehicles[MAX_VEHICLES];
Station stations[MAX_STATIONS];

int vehicleCount = 0;
int stationCount = 0;

// Queue for waiting vehicles
int waitingQueue[MAX_QUEUE];
int front = 0;
int rear = -1;
int queueCount = 0;

// Stack for recent operations
string operations[MAX_STACK];
int top = -1;

// Linked List for assignment history
Node* head = NULL;


// Function to add operation to stack
void pushOperation(string operation)
{
    if (top < MAX_STACK - 1)
    {
        operations[++top] = operation;
    }
}

// Function to display recent operations
void displayOperations()
{
    cout << "\n--- Recent Operations ---\n";

    if (top == -1)
    {
        cout << "No operations recorded.\n";
        return;
    }

    for (int i = top; i >= 0; i--)
    {
        cout << operations[i] << endl;
    }
}


// Function to add assignment history
void addHistory(string details)
{
    Node* newNode = new Node;

    newNode->details = details;
    newNode->next = head;

    head = newNode;
}

// Function to display assignment history
void displayHistory()
{
    cout << "\n--- Assignment History ---\n";

    Node* temp = head;

    if (temp == NULL)
    {
        cout << "No history available.\n";
        return;
    }

    while (temp != NULL)
    {
        cout << temp->details << endl;
        temp = temp->next;
    }
}


// Find vehicle using ID
int findVehicle(int id)
{
    for (int i = 0; i < vehicleCount; i++)
    {
        if (vehicles[i].id == id)
        {
            return i;
        }
    }

    return -1;
}


// Function to register vehicle
void registerVehicle()
{
    if (vehicleCount >= MAX_VEHICLES)
    {
        cout << "Vehicle limit reached!\n";
        return;
    }

    Vehicle v;

    cout << "\nEnter Vehicle ID: ";
    cin >> v.id;

    if (findVehicle(v.id) != -1)
    {
        cout << "Vehicle ID already exists!\n";
        return;
    }

    cout << "Enter Vehicle Name: ";
    cin >> v.name;

    cout << "Enter Battery Level (%): ";
    cin >> v.battery;

    if (v.battery < 0 || v.battery > 100)
    {
        cout << "Invalid battery level!\n";
        return;
    }

    cout << "Enter Connector Type (CCS/Type2/CHAdeMO): ";
    cin >> v.connector;

    cout << "\nSelect Priority:\n";
    cout << "1. Emergency\n";
    cout << "2. High\n";
    cout << "3. Normal\n";
    cout << "Enter priority: ";

    cin >> v.priority;

    if (v.priority < 1 || v.priority > 3)
    {
        cout << "Invalid priority!\n";
        return;
    }

    v.status = "Registered";

    vehicles[vehicleCount++] = v;

    pushOperation("Vehicle Registered: " + v.name);

    cout << "\nVehicle registered successfully!\n";
}


// Function to display vehicles
void displayVehicles()
{
    cout << "\n========== REGISTERED VEHICLES ==========\n";

    if (vehicleCount == 0)
    {
        cout << "No vehicles registered.\n";
        return;
    }

    cout << "ID\tName\tBattery\tConnector\tPriority\tStatus\n";

    for (int i = 0; i < vehicleCount; i++)
    {
        cout << vehicles[i].id << "\t"
             << vehicles[i].name << "\t"
             << vehicles[i].battery << "%\t"
             << vehicles[i].connector << "\t\t"
             << vehicles[i].priority << "\t\t"
             << vehicles[i].status << endl;
    }
}


// Function to add charging station
void addStation()
{
    if (stationCount >= MAX_STATIONS)
    {
        cout << "Station limit reached!\n";
        return;
    }

    Station s;

    cout << "\nEnter Charging Slot ID: ";
    cin >> s.slotId;

    for (int i = 0; i < stationCount; i++)
    {
        if (stations[i].slotId == s.slotId)
        {
            cout << "Slot ID already exists!\n";
            return;
        }
    }

    cout << "Enter Connector Type (CCS/Type2/CHAdeMO): ";
    cin >> s.connector;

    s.available = true;
    s.vehicleId = -1;

    stations[stationCount++] = s;

    pushOperation("Charging Station Added");

    cout << "\nCharging station added successfully!\n";
}


// Function to display stations
void displayStations()
{
    cout << "\n========== CHARGING STATIONS ==========\n";

    if (stationCount == 0)
    {
        cout << "No charging stations available.\n";
        return;
    }

    cout << "Slot ID\tConnector\tStatus\t\tVehicle ID\n";

    for (int i = 0; i < stationCount; i++)
    {
        cout << stations[i].slotId << "\t"
             << stations[i].connector << "\t\t";

        if (stations[i].available)
        {
            cout << "Available";
        }
        else
        {
            cout << "Occupied";
        }

        cout << "\t\t" << stations[i].vehicleId << endl;
    }
}


// Function to find compatible charging slot
int findAvailableSlot(string connector)
{
    for (int i = 0; i < stationCount; i++)
    {
        if (stations[i].available &&
            stations[i].connector == connector)
        {
            return i;
        }
    }

    return -1;
}


// Function to assign charging slot
void assignSlot(int vehicleIndex, int stationIndex)
{
    stations[stationIndex].available = false;

    stations[stationIndex].vehicleId =
        vehicles[vehicleIndex].id;

    vehicles[vehicleIndex].status = "Charging";

    string message =
        "Vehicle " + vehicles[vehicleIndex].name +
        " assigned to Slot " +
        to_string(stations[stationIndex].slotId);

    addHistory(message);
    pushOperation(message);

    cout << "\nCharging slot assigned successfully!\n";
    cout << message << endl;
}


// Function to check if vehicle is already waiting
bool isAlreadyWaiting(int id)
{
    for (int i = 0; i < queueCount; i++)
    {
        int index = (front + i) % MAX_QUEUE;

        if (waitingQueue[index] == id)
        {
            return true;
        }
    }

    return false;
}


// Function to add vehicle to waiting queue
void enqueue(int id)
{
    if (queueCount == MAX_QUEUE)
    {
        cout << "Waiting queue is full!\n";
        return;
    }

    if (isAlreadyWaiting(id))
    {
        cout << "Vehicle is already in waiting queue.\n";
        return;
    }

    rear = (rear + 1) % MAX_QUEUE;

    waitingQueue[rear] = id;

    queueCount++;

    int index = findVehicle(id);

    if (index != -1)
    {
        vehicles[index].status = "Waiting";
    }

    cout << "No suitable slot available.\n";
    cout << "Vehicle added to waiting queue.\n";
}


// Function to remove vehicle from queue
void dequeue()
{
    if (queueCount == 0)
    {
        return;
    }

    front = (front + 1) % MAX_QUEUE;

    queueCount--;
}


// Function to assign a slot manually
void requestCharging()
{
    int id;

    cout << "\nEnter Vehicle ID: ";
    cin >> id;

    int index = findVehicle(id);

    if (index == -1)
    {
        cout << "Vehicle not found!\n";
        return;
    }

    if (vehicles[index].status == "Charging" ||
        vehicles[index].status == "Waiting")
    {
        cout << "Vehicle already has an active request.\n";
        return;
    }

    int slot = findAvailableSlot(vehicles[index].connector);

    if (slot != -1)
    {
        assignSlot(index, slot);
    }
    else
    {
        enqueue(id);
    }
}


// Function to display waiting queue
void displayQueue()
{
    cout << "\n========== WAITING QUEUE ==========\n";

    if (queueCount == 0)
    {
        cout << "No vehicles waiting.\n";
        return;
    }

    for (int i = 0; i < queueCount; i++)
    {
        int index = (front + i) % MAX_QUEUE;

        int id = waitingQueue[index];

        int v = findVehicle(id);

        if (v != -1)
        {
            cout << "Vehicle ID: " << id
                 << " | Name: " << vehicles[v].name
                 << " | Priority: " << vehicles[v].priority
                 << endl;
        }
    }
}


// Function to process waiting vehicles
void processWaitingQueue()
{
    while (queueCount > 0)
    {
        int bestPosition = -1;
        int bestPriority = 4;

        // Find highest priority compatible vehicle
        for (int i = 0; i < queueCount; i++)
        {
            int qIndex = (front + i) % MAX_QUEUE;

            int id = waitingQueue[qIndex];

            int vIndex = findVehicle(id);

            if (vIndex != -1)
            {
                int slot = findAvailableSlot(
                    vehicles[vIndex].connector
                );

                if (slot != -1 &&
                    vehicles[vIndex].priority < bestPriority)
                {
                    bestPriority = vehicles[vIndex].priority;
                    bestPosition = i;
                }
            }
        }

        // No compatible vehicle found
        if (bestPosition == -1)
        {
            break;
        }

        // Remove selected vehicle from queue
        int selectedIndex = (front + bestPosition) % MAX_QUEUE;

        int selectedID = waitingQueue[selectedIndex];

        for (int i = bestPosition; i < queueCount - 1; i++)
        {
            int current = (front + i) % MAX_QUEUE;
            int next = (front + i + 1) % MAX_QUEUE;

            waitingQueue[current] = waitingQueue[next];
        }

        rear = (rear - 1 + MAX_QUEUE) % MAX_QUEUE;

        queueCount--;

        int vIndex = findVehicle(selectedID);

        int slot = findAvailableSlot(
            vehicles[vIndex].connector
        );

        if (slot != -1)
        {
            cout << "\nAssigning waiting vehicle: "
                 << selectedID << endl;

            assignSlot(vIndex, slot);
        }
    }
}


// Function to release charging slot
void releaseSlot()
{
    int id;

    cout << "\nEnter Charging Slot ID to release: ";
    cin >> id;

    for (int i = 0; i < stationCount; i++)
    {
        if (stations[i].slotId == id)
        {
            if (stations[i].available)
            {
                cout << "This slot is already available.\n";
                return;
            }

            int vehicleID = stations[i].vehicleId;

            int vIndex = findVehicle(vehicleID);

            if (vIndex != -1)
            {
                vehicles[vIndex].status = "Completed";
            }

            stations[i].available = true;
            stations[i].vehicleId = -1;

            cout << "\nCharging slot released successfully!\n";

            addHistory(
                "Slot " + to_string(id) + " released."
            );

            pushOperation("Charging Slot Released");

            // Assign waiting vehicles
            processWaitingQueue();

            return;
        }
    }

    cout << "Charging slot not found!\n";
}


// Function to search vehicle
void searchVehicle()
{
    int id;

    cout << "\nEnter Vehicle ID to search: ";
    cin >> id;

    int index = findVehicle(id);

    if (index == -1)
    {
        cout << "Vehicle not found!\n";
        return;
    }

    cout << "\nVehicle Found!\n";

    cout << "ID: " << vehicles[index].id << endl;
    cout << "Name: " << vehicles[index].name << endl;
    cout << "Battery: " << vehicles[index].battery << "%" << endl;
    cout << "Connector: " << vehicles[index].connector << endl;
    cout << "Priority: " << vehicles[index].priority << endl;
    cout << "Status: " << vehicles[index].status << endl;
}


// Function to sort vehicles by battery level
void sortVehicles()
{
    if (vehicleCount == 0)
    {
        cout << "No vehicles to sort.\n";
        return;
    }

    // Bubble Sort
    for (int i = 0; i < vehicleCount - 1; i++)
    {
        for (int j = 0; j < vehicleCount - i - 1; j++)
        {
            if (vehicles[j].battery > vehicles[j + 1].battery)
            {
                Vehicle temp = vehicles[j];

                vehicles[j] = vehicles[j + 1];

                vehicles[j + 1] = temp;
            }
        }
    }

    cout << "\nVehicles sorted by battery level!\n";

    displayVehicles();
}


// Main Function
int main()
{
    int choice;

    do
    {
        cout << "\n";
        cout << "========================================\n";
        cout << "     INTELLIGENT EV CHARGING SYSTEM\n";
        cout << "========================================\n";

        cout << "1. Register Vehicle\n";
        cout << "2. View All Vehicles\n";
        cout << "3. Add Charging Station\n";
        cout << "4. View Charging Stations\n";
        cout << "5. Assign Charging Slot\n";
        cout << "6. View Waiting Queue\n";
        cout << "7. Release Charging Slot\n";
        cout << "8. Search Vehicle\n";
        cout << "9. Sort Vehicles\n";
        cout << "10. View Assignment History\n";
        cout << "11. View Recent Operations\n";
        cout << "0. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                registerVehicle();
                break;

            case 2:
                displayVehicles();
                break;

            case 3:
                addStation();
                break;

            case 4:
                displayStations();
                break;

            case 5:
                requestCharging();
                break;

            case 6:
                displayQueue();
                break;

            case 7:
                releaseSlot();
                break;

            case 8:
                searchVehicle();
                break;

            case 9:
                sortVehicles();
                break;

            case 10:
                displayHistory();
                break;

            case 11:
                displayOperations();
                break;

            case 0:
                cout << "\nThank you for using EV Charging System!\n";
                break;

            default:
                cout << "\nInvalid choice! Try again.\n";
        }

    } while (choice != 0);

    // Free linked list memory
    Node* temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}