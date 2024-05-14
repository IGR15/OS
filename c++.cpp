#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

// Process Control Block (PCB) structure
struct PCB {
    int id;
    int arrivalTime;
    int cpuBurst;
    int remainingTime;
    int startTime;
    int finishTime;
    int waitingTime;
    int turnaroundTime;
};

// Comparator for priority queue based on remaining time for SRT
struct CompareRemainingTime {
    bool operator()(const PCB& p1, const PCB& p2) {
        return p1.remainingTime > p2.remainingTime;
    }
};

// Function to simulate FCFS scheduling
void simulateFCFS(vector<PCB>& processes, int contextSwitchTime) {
    int currentTime = 0;
    cout << "FCFS Scheduling:\n";
    for (const PCB& process : processes) {
        if (currentTime < process.arrivalTime) {
            currentTime = process.arrivalTime;
        }
        cout << "Executing process " << process.id << " from time " << currentTime;
        currentTime += process.cpuBurst;
        cout << " to " << currentTime << endl;
    }
}

// Function to simulate SRT scheduling
void simulateSRT(vector<PCB>& processes, int contextSwitchTime) {
    priority_queue<PCB, vector<PCB>, CompareRemainingTime> readyQueue;
    int currentTime = 0;
    int remainingProcesses = processes.size();
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    cout << "SRT Scheduling:\n";

    while (remainingProcesses > 0) {
        // Add processes that have arrived to the ready queue
        for (PCB& process : processes) {
            if (process.arrivalTime <= currentTime && process.remainingTime > 0) {
                readyQueue.push(process);
            }
        }

        if (!readyQueue.empty()) {
            PCB currentProcess = readyQueue.top();
            readyQueue.pop();

            cout << "Executing process " << currentProcess.id << " from time " << currentTime;
            int executionTime = min(currentProcess.remainingTime, contextSwitchTime);
            currentTime += executionTime;
            currentProcess.remainingTime -= executionTime;
            cout << " to " << currentTime << endl;

            if (currentProcess.remainingTime == 0) {
                currentProcess.finishTime = currentTime;
                currentProcess.turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;
                currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.cpuBurst;
                totalWaitingTime += currentProcess.waitingTime;
                totalTurnaroundTime += currentProcess.turnaroundTime;
                remainingProcesses--;
            } else {
                readyQueue.push(currentProcess); // Re-insert the process with remaining time
            }
        } else {
            currentTime++; // No processes available, increment time
        }
    }

    cout << "Average Waiting Time: " << (double)totalWaitingTime / processes.size() << endl;
    cout << "Average Turnaround Time: " << (double)totalTurnaroundTime / processes.size() << endl;
}

// Function to simulate RR scheduling
void simulateRR(vector<PCB>& processes, int contextSwitchTime, int quantum) {
    queue<PCB> readyQueue;
    int currentTime = 0;
    int remainingProcesses = processes.size();
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    cout << "RR Scheduling with Quantum " << quantum << ":\n";

    while (remainingProcesses > 0) {
        // Add processes that have arrived to the ready queue
        for (PCB& process : processes) {
            if (process.arrivalTime <= currentTime && process.remainingTime > 0) {
                readyQueue.push(process);
            }
        }

        if (!readyQueue.empty()) {
            PCB currentProcess = readyQueue.front();
            readyQueue.pop();

            cout << "Executing process " << currentProcess.id << " from time " << currentTime;
            int executionTime = min(quantum, currentProcess.remainingTime);
            currentTime += executionTime;
            currentProcess.remainingTime -= executionTime;
            cout << " to " << currentTime << endl;

            if (currentProcess.remainingTime == 0) {
                currentProcess.finishTime = currentTime;
                currentProcess.turnaroundTime = currentProcess.finishTime - currentProcess.arrivalTime;
                currentProcess.waitingTime = currentProcess.turnaroundTime - currentProcess.cpuBurst;
                totalWaitingTime += currentProcess.waitingTime;
                totalTurnaroundTime += currentProcess.turnaroundTime;
                remainingProcesses--;
            } else {
                readyQueue.push(currentProcess); // Re-insert the process with remaining time
            }

            // Simulate context switch
            currentTime += contextSwitchTime;
        } else {
            currentTime++; // No processes available, increment time
        }
    }

    cout << "Average Waiting Time: " << (double)totalWaitingTime / processes.size() << endl;
    cout << "Average Turnaround Time: " << (double)totalTurnaroundTime / processes.size() << endl;
}

int main() {
    // Create input file
    ofstream inputFile("processes.txt");
    if (!inputFile) {
        cerr << "Error: Unable to create file.\n";
        return 1;
    }

    // Write process information to the file
    inputFile << "1 0 10\n";
    inputFile << "2 2 5\n";
    inputFile << "3 3 7\n";
    inputFile << "4 5 3\n";
    inputFile << "5 6 6\n";
    inputFile.close();

    // Read parameters from user or file
    int contextSwitchTime;
    int quantum;

    cout << "Enter context switch time (CS): ";
    cin >> contextSwitchTime;

    cout << "Enter time quantum (Q) for RR: ";
    cin >> quantum;

    // Read process information from file
    ifstream processFile("processes.txt");
    if (!processFile) {
        cerr << "Error: Unable to open file.\n";
        return 1;
    }

    vector<PCB> processes;
    int id, arrivalTime, cpuBurst;
    while (processFile >> id >> arrivalTime >> cpuBurst) {
        PCB process;
        process.id = id;
        process.arrivalTime = arrivalTime;
        process.cpuBurst = cpuBurst;
        process.remainingTime = cpuBurst;
        processes.push_back(process);
    }

    processFile.close();

    // Simulate scheduling algorithms
    simulateFCFS(processes, contextSwitchTime);
    simulateSRT(processes, contextSwitchTime);
    simulateRR(processes, contextSwitchTime, quantum);

    return 0;
}
