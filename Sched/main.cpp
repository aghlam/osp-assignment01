#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <list>


using std::cout;
using std::endl;
using std::string;
using std::list;


struct Process {

    int process_id;
    double burst_time;
    double arrival_time;
    double waiting_time;
    double turnaround_time;
    double original_bt;

};


void fcfsSimulation(list<Process> processes);
void sjfSimulation(list<Process> processes);
void rrSimulation(list<Process> processes);
void outputCSV(list<Process> processes, string filename);


int main(int argc, char** argv) {

    std::ifstream infile;

    list<Process> processes;

    // Check for correct number of arguments
    if (argc != 3) {
        cout << "Error - requires two arguments to run." << endl;
        return EXIT_FAILURE;
    } 

    // Read in file
    infile.open(argv[2]);

    if (infile.fail()) {
        cout << "Error - could not open file" << endl;
        return EXIT_FAILURE;

    } else {
        while (!infile.eof()) {

            int a, b, c;

            infile >> a >> b >> c;

            Process p;
            p.process_id = a;
            p.burst_time = b;
            p.arrival_time = c;
            p.waiting_time = 0;
            p.turnaround_time = 0;
            p.original_bt = b;

            processes.push_back(p);

        }
    }

    infile.close();

    // Check for which process scheduler
    if (argv[1] == string("-fcfs")) {
        cout << "Running First Come First Served Scheduler simulation" << endl;
        cout << "Outputting data to FCFS-output.csv" << endl;
        fcfsSimulation(processes);

    } else if (argv[1] == string("-sjf")) {
        cout << "Running Shortest Job First Scheduler simulation" << endl;
        cout << "Outputting data to SJF-output.csv" << endl;
        sjfSimulation(processes);

    } else if (argv[1] == string("-rr")) {
        cout << "Running Round Robin Scheduler simulation" << endl;
        cout << "Outputting data to RR-output.csv" << endl;
        rrSimulation(processes);

    } else {
        cout << "Error - incorrect scheduling input - please try again" << endl;
        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;
}


void rrSimulation(list<Process> processes) {

    list<Process> readyQ;

    int timer = 0;
    // Set quantum and context switch
    int quantum = 2;
    double context_switch = 0.1;

    bool running = true;

    // RR loop
    while (running) {

        if (processes.empty()) {
            running = false;
        }

        for (Process process : readyQ) {
            if (process.burst_time != 0) {
                running = true;
                break;
            }
        }

        if (!readyQ.empty()) {
            if ((timer != 0) && (timer % quantum == 0)) {
                // Skip over any process that has finished execution
                while (readyQ.front().burst_time == 0) {
                    readyQ.splice(readyQ.end(), readyQ, readyQ.begin());
                }

                if (readyQ.front().burst_time >= quantum) {
                    // Minus from burst time
                    readyQ.front().burst_time -= ((double)quantum - context_switch);
                    // Move process to end of the list
                    readyQ.splice(readyQ.end(), readyQ, readyQ.begin());

                } else {
                    // Finish process
                    readyQ.front().burst_time = 0;
                    // Calculate turnaround time
                    readyQ.front().turnaround_time = (double)timer - readyQ.front().arrival_time;
                    // Calculate waiting time
                    readyQ.front().waiting_time = readyQ.front().turnaround_time - readyQ.front().original_bt;
                    // Move process to end of the list
                    readyQ.splice(readyQ.end(), readyQ, readyQ.begin());

                }
            }
        }

        // Add in any new process
        if (!processes.empty()) {
            if (processes.front().arrival_time <= timer) {
                readyQ.push_back(processes.front());
                processes.erase(processes.begin());
            }
        }

        // Increment timer
        ++timer;

    }

    // Sort the final list
    readyQ.sort([](Process a, Process b) {
        return a.process_id < b.process_id;
    });

    // Output to file
    outputCSV(readyQ, "RR-output.csv" );

}


void fcfsSimulation(list<Process> processes) {

    list<Process> readyQueue;
    list<Process> finishedProcess;

    int timer = 0;
    int burstTimer = 0;

    while (!readyQueue.empty() || !processes.empty()) {

        // Check for process arrival time - assumes arrival times are listed in order in the file
        if (!processes.empty()) {
            if (processes.front().arrival_time <= timer) {
                readyQueue.push_back(processes.front());
                processes.erase(processes.begin());
            }
        }

        if (!readyQueue.empty()) {
            // Check if current running process has finished burst time
            if (burstTimer == readyQueue.front().burst_time) {
                // Calculate turnaround time
                readyQueue.front().turnaround_time = timer - readyQueue.front().arrival_time;
                // Calculate waiting time
                readyQueue.front().waiting_time = readyQueue.front().turnaround_time - readyQueue.front().original_bt;
                // Add to finished processes, remove from ready queue
                finishedProcess.push_back(readyQueue.front());
                readyQueue.erase(readyQueue.begin());
                // Reset burst timer
                burstTimer = 0;
            }

            // Increment burst timer
            if (!readyQueue.empty()) {
                ++burstTimer;
            }

        }

        // Increment timer
        ++timer;

    }

    // Output to file
    outputCSV(finishedProcess, "FCFS-output.csv");
    
}


void sjfSimulation(list<Process> processes) {

    list<Process> readyQueue;
    list<Process> finishedProcess;

    int burstTimer = 0;
    int timer = 0;

    // SJF loop
    while (!processes.empty() || !readyQueue.empty()) {

        // Check for process arrival time - assumes arrival times are listed in order in the file
        if (!processes.empty()) {
            if (processes.front().arrival_time <= timer) {
                readyQueue.push_back(processes.front());
                processes.erase(processes.begin());
            }
        }

        if (!readyQueue.empty()) {
            if (burstTimer == readyQueue.front().burst_time) {
                // Calculate turnaround time
                readyQueue.front().turnaround_time = timer - readyQueue.front().arrival_time;
                // Calculate waiting time
                readyQueue.front().waiting_time = readyQueue.front().turnaround_time - readyQueue.front().original_bt;
                // Add to finished processes, remove from ready queue
                finishedProcess.push_back(readyQueue.front());
                readyQueue.erase(readyQueue.begin());
                // Reset burst timer
                burstTimer = 0;

                // Sort processes to find shortest job
                readyQueue.sort([](Process a, Process b) {
                    return a.burst_time < b.burst_time;
                });

            }
            
            // Increment burst timer
            if (!readyQueue.empty()) {
            }

                ++burstTimer;
        }

        ++timer;

    }

    finishedProcess.sort([](Process a, Process b) {
        return a.process_id < b.process_id;
    });

    // Output to file
    outputCSV(finishedProcess, "SJF-output.csv");

}


void outputCSV(list<Process> processes, string filename) {

    std::ofstream outfile;

    // Output CSV file to filename
    outfile.open(filename);

    outfile << "Process_id," << "Burst_time," << "Arrival_time," << "Waiting_time," << "Turnaround_time" << endl;

    for (Process process : processes) {
        outfile << process.process_id << "," 
                << process.original_bt << "," 
                << process.arrival_time << "," 
                << process.waiting_time << "," 
                << process.turnaround_time 
                << endl;
    }

    outfile.close();

}