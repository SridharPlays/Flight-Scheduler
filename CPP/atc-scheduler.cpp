#include <iostream>
#include <conio.h>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>   
#include <chrono>   
#include <iomanip>
using namespace std;

enum class FlightType {
    EMERGENCY,
    LOW_FUEL,
    VIP,
    INTERNATIONAL,
    DOMESTIC,
    CARGO
};

string flightTypeToString(FlightType type) {
    switch (type) {
        case FlightType::EMERGENCY:     return "Emergency";
        case FlightType::LOW_FUEL:      return "Low Fuel";
        case FlightType::VIP:           return "VIP";
        case FlightType::INTERNATIONAL: return "International";
        case FlightType::DOMESTIC:      return "Domestic";
        case FlightType::CARGO:         return "Cargo";
        default:                        return "Unknown";
    }
}

// Represents an aircraft as a process with various attributes.
struct Flight {
    string id;
    FlightType type;
    int arrivalTime; // The simulation cycle when the flight arrived
    int burstTime;   // Time required on the runway (CPU burst time)
    int waitingTime; // Time spent waiting in the queue
    int priority;    // Calculated priority score

    // Constructor to initialize a new flight.
    Flight(string id, FlightType type, int arrivalTime, int burstTime)
        : id(id), type(type), arrivalTime(arrivalTime), burstTime(burstTime), waitingTime(0), priority(0) {}
};

// The main scheduler class that manages the airport's operations.
class ATCScheduler {
private:
    vector<Flight> waitingQueue; // Holds all flights waiting for the runway
    bool isRunwayBusy;
    Flight currentFlightOnRunway;
    int runwayTimeLeft;
    int simulationCycle;

    // Calculates the priority of a flight.
    // Higher number means higher priority.
    void calculatePriority(Flight& flight) {
        int basePriority = 0;
        switch (flight.type) {
            case FlightType::EMERGENCY:     basePriority = 600; break;
            case FlightType::LOW_FUEL:      basePriority = 500; break;
            case FlightType::VIP:           basePriority = 400; break;
            case FlightType::INTERNATIONAL: basePriority = 300; break;
            case FlightType::DOMESTIC:      basePriority = 200; break;
            case FlightType::CARGO:         basePriority = 100; break;
        }
        // Aging mechanism: priority increases with waiting time.
        // Each cycle spent waiting adds 10 to the priority score.
        flight.priority = basePriority + (flight.waitingTime * 10);
    }

public:
    ATCScheduler() : isRunwayBusy(false), currentFlightOnRunway("", FlightType::DOMESTIC, 0, 0), runwayTimeLeft(0), simulationCycle(0) {}

    // Adds a new flight to the simulation.
    void addFlight(const Flight& flight) {
        waitingQueue.push_back(flight);
        cout << "[CYCLE " << simulationCycle << "] INFO: Flight " << flight.id << " (" << flightTypeToString(flight.type) << ") has entered the system and is waiting.\n";
    }

    // Prints the current status of the simulation.
    void printStatus() {
        cout << "--------------------------------------------------------------------------------\n";
        cout << "[CYCLE " << setw(2) << simulationCycle << "] STATUS:\n";

        if (isRunwayBusy) {
            cout << "  Runway: BUSY with Flight " << currentFlightOnRunway.id
                      << " (" << flightTypeToString(currentFlightOnRunway.type) << "). "
                      << "Time left: " << runwayTimeLeft << " cycles.\n";
        } else {
            cout << "  Runway: FREE\n";
        }

        cout << "  Waiting Queue (" << waitingQueue.size() << " flights):\n";
        if (waitingQueue.empty()) {
            cout << "    <Empty>\n";
        } else {
            // Display flights sorted by their current priority
            vector<Flight> sortedQueue = waitingQueue;
            sort(sortedQueue.begin(), sortedQueue.end(), [](const Flight& a, const Flight& b) {
                return a.priority > b.priority;
            });

            cout << "    " << left << setw(10) << "ID"
                      << setw(17) << "Type"
                      << setw(15) << "Wait Time"
                      << setw(10) << "Priority" << "\n";
            for (const auto& flight : sortedQueue) {
                cout << "    " << left << setw(10) << flight.id
                          << setw(17) << flightTypeToString(flight.type)
                          << setw(15) << flight.waitingTime
                          << setw(10) << flight.priority << "\n";
            }
        }
        cout << "--------------------------------------------------------------------------------\n\n";
    }


    // Runs a single cycle of the simulation.
    void runCycle() {
        simulationCycle++;

        // 1. Update the runway status
        if (isRunwayBusy) {
            runwayTimeLeft--;
            if (runwayTimeLeft <= 0) {
                cout << "[CYCLE " << simulationCycle << "] SUCCESS: Flight " << currentFlightOnRunway.id << " has cleared the runway.\n";
                isRunwayBusy = false;
            }
        }

        // 2. Update all waiting flights (Aging)
        for (auto& flight : waitingQueue) {
            flight.waitingTime++;
            calculatePriority(flight);
        }

        // 3. Schedule a new flight if the runway is free
        if (!isRunwayBusy && !waitingQueue.empty()) {
            // Sort by priority to find the highest priority flight
            sort(waitingQueue.begin(), waitingQueue.end(), [](const Flight& a, const Flight& b) {
                return a.priority > b.priority;
            });

            // The highest priority flight is the first one in the sorted vector
            currentFlightOnRunway = waitingQueue.front();
            waitingQueue.erase(waitingQueue.begin());

            isRunwayBusy = true;
            runwayTimeLeft = currentFlightOnRunway.burstTime;

            cout << "[CYCLE " << simulationCycle << "] ACTION: Scheduling Flight " << currentFlightOnRunway.id
                      << " onto the runway. Waited for " << currentFlightOnRunway.waitingTime << " cycles.\n";
        }
        
        printStatus();
    }
    
    // Checks if the simulation should continue.
    bool hasPendingFlights() const {
        return isRunwayBusy || !waitingQueue.empty();
    }
};

int main() {
    ATCScheduler scheduler;

    // Add some initial flights to the system
    scheduler.addFlight(Flight("DL456", FlightType::DOMESTIC, 0, 4));
    scheduler.addFlight(Flight("BA288", FlightType::INTERNATIONAL, 0, 6));
    scheduler.addFlight(Flight("FX123", FlightType::CARGO, 1, 5));
    scheduler.addFlight(Flight("AA789", FlightType::DOMESTIC, 1, 4));
    scheduler.printStatus(); // Initial state before simulation starts

    int cycle = 0;
    while(scheduler.hasPendingFlights()) {
        // Add new flights dynamically to simulate a real airport
        if (cycle == 2) {
             scheduler.addFlight(Flight("UA901", FlightType::INTERNATIONAL, 2, 6));
        }
        if (cycle == 4) {
            scheduler.addFlight(Flight("EMD01", FlightType::EMERGENCY, 4, 3));
        }
        
        scheduler.runCycle();
        this_thread::sleep_for(chrono::seconds(2));
        cycle++;
    }
    
    cout << "All flights have been processed. Simulation finished.\n";

    getch();
}
