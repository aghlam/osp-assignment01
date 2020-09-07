#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using std::cout;
using std::endl;
using std::string;
using std::vector;


struct Process {

    int process_id;
    double burst_time;
    double arrival_time;
    double waiting_time;
    double turnaround_time;

};


void fcfsSimulation(vector<Process> processes);
void sjfSimulation(vector<Process> processes);
void rrSimulation(vector<Process> processes);


int main(int argc, char** argv) {

    std::ifstream infile;

    vector<Process> processes;

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

            processes.push_back(p);

        }
    }

    infile.close();


    // Check for which process scheduler
    if (argv[1] == string("-fcfs")) {
        cout << "Running First Come First Served Scheduler simulation" << endl;
        fcfsSimulation(processes);

    } else if (argv[1] == string("-sjf")) {
        cout << "Running Shortest Job First Scheduler simulation" << endl;
        sjfSimulation(processes);

    } else if (argv[1] == string("-rr")) {
        cout << "Running Round Robin Scheduler simulation" << endl;
        rrSimulation(processes);

    } else {
        cout << "Incorrect scheduling input" << endl;
        return EXIT_FAILURE;

    }

    return EXIT_SUCCESS;
}

void rrSimulation(vector<Process> processes) {

    std::ofstream outfile;

    double timer = 0;
    // Set quantum and context switch
    double quantum = 2;
    double context_switch = 0.1;

    // Vector to hold processes in rr queue
    vector<Process> processesCopy;
    vector<double> runningQueue;

    // Create copy of burst times minus the initial process
    for (unsigned int i = 1; i < processes.size(); ++i) {
        processesCopy.push_back(processes[i]);
    }

    // Add initial process to runningQueue
    runningQueue.push_back(processes[0].burst_time);


    while(true) {
        // For when there are no more processes to be run
        bool running = false;

        ++timer;

        if ((int)timer % (int)quantum == 0) {

            // Loop through processes
            for (unsigned int i = 0; i < runningQueue.size(); ++i) {

                if (runningQueue[i] > 0) {

                    running = true;

                    if (runningQueue[i] > quantum) {
                        // timer += quantum;

                        runningQueue[i] -= (quantum - context_switch);

                    } else {
                        // Even when process is finished cpu is idle until end of quantum 
                        // so timer adds ticks equal to quantum
                        // timer += quantum;

                        runningQueue[i] = 0;

                        // Calculate waiting time for the process
                        processes[i].waiting_time = timer - processes[i].burst_time;
                    }

                }
            }
        }

        // Check for new processes arriving in queue
        for (unsigned int i = 0; i < processesCopy.size(); ++i) {
            if (timer >= processesCopy[i].arrival_time) {
                runningQueue.push_back(processesCopy[i].burst_time);
                processesCopy.erase(processesCopy.begin());
            }
        }



    }

    // Calculate turnaround time
    for (unsigned int i = 0; i < processes.size(); ++i) {
        processes[i].turnaround_time = processes[i].burst_time + processes[i].waiting_time;
    }


    // For testing - delete after
    for (unsigned int i = 0; i < processes.size(); ++i) {
        cout << "ID: " << processes[i].process_id << "\t| Waiting: " << processes[i].waiting_time << "\t|Turnaround: " << processes[i].turnaround_time<< endl;
    }


    // Output CSV file with waiting time and turnaround time
    outfile.open("RR-output.csv");

    outfile << "Process_id," << "Burst_time," << "Arrival_time" << "Waiting_time," << "Turnaround_time" << endl;

    for (unsigned int i = 0; i < processes.size(); ++i) {
        outfile << processes[i].process_id << "," 
                << processes[i].burst_time << "," 
                << processes[i].arrival_time << "," 
                << processes[i].waiting_time << "," 
                << processes[i].turnaround_time 
                << endl;
    }

    outfile.close();

}

void fcfsSimulation(vector<Process> processes) {

    std::ofstream outfile;

    vector<int> running_time;
    
    // Initial running time when starting first process is 0
    running_time.push_back(0);
    // Waiting time of first process is 0
    processes[0].waiting_time = 0;


    // Calculating waiting time
    for(unsigned int i = 1; i < processes.size(); ++i) {

        // Calculate the current running time by adding burst time of previous process
        running_time.push_back(running_time[i-1] + processes[i-1].burst_time);

        // Calculate waiting time for current process
        processes[i].waiting_time = running_time[i] - processes[i].arrival_time;

        /**
         * If the waiting time for a process is in the negative that means it is already 
         * in the ready queue before CPU becomes idle so its waiting time is 0
         * Source: https://www.geeksforgeeks.org/program-for-fcfs-cpu-scheduling-set-1/
         */
        if (processes[i].waiting_time < 0) {
            processes[i].waiting_time = 0;
        }

    }

    // Calculating turnaround time
    for(unsigned int i = 0; i < processes.size(); ++i) {
        processes[i].turnaround_time = processes[i].burst_time + processes[i].waiting_time;
    }

    // Output CSV file with waiting time and turnaround time
    outfile.open("FCFS-output.csv");

    outfile << "Process_id," << "Burst_time," << "Arrival_time" << "Waiting_time," << "Turnaround_time" << endl;

    for (unsigned int i = 0; i < processes.size(); ++i) {
        outfile << processes[i].process_id << "," 
                << processes[i].burst_time << "," 
                << processes[i].arrival_time << "," 
                << processes[i].waiting_time << "," 
                << processes[i].turnaround_time 
                << endl;
    }

    outfile.close();

}

void sjfSimulation(vector<Process> processes) {

    std::ofstream outfile;

    vector<Process> runningProcess;
    vector<Process> queueProcess;
    int currentRunningTime = 0;
    int timer = 0;

    // First process to be run added to running queue
    runningProcess.push_back(processes[0]);
    runningProcess[0].waiting_time = 0;
    currentRunningTime += runningProcess[0].burst_time;
    processes.erase(processes.begin());

    // Timer loop
    while (!processes.empty() || !queueProcess.empty()) {
        // Tracks when process is finished
        // Moves process from queueProcess to runningProcess
        if (timer == currentRunningTime) {
            runningProcess.push_back(queueProcess[0]);
            queueProcess.erase(queueProcess.begin());
            // Calculate the waiting time
            runningProcess.back().waiting_time = currentRunningTime - runningProcess.back().arrival_time;
            currentRunningTime += runningProcess.back().burst_time;

        }

        // Check for process arrival time - assumes arrival times are listed in order in the file
        if (timer == processes[0].arrival_time) {
            queueProcess.push_back(processes[0]);
            processes.erase(processes.begin());
        }

        // Lambda for sorting
        std::stable_sort(queueProcess.begin(), queueProcess.end(), [](Process a, Process b) {
            return a.burst_time < b.burst_time;
        });

        // Increment timer tick
        ++timer;

    }


    // Calculate turnaround time
    for(unsigned int i = 0; i < runningProcess.size(); ++i) {
        runningProcess[i].turnaround_time = runningProcess[i].burst_time + runningProcess[i].waiting_time;
    }


    // Output CSV file with waiting time and turnaround time
    outfile.open("SJF-output.csv");

    outfile << "Process_id," << "Burst_time," << "Arrival_time" << "Waiting_time," << "Turnaround_time" << endl;

    for (unsigned int i = 0; i < runningProcess.size(); ++i) {
        outfile << runningProcess[i].process_id << "," 
                << runningProcess[i].burst_time << "," 
                << runningProcess[i].arrival_time << "," 
                << runningProcess[i].waiting_time << "," 
                << runningProcess[i].turnaround_time 
                << endl;
    }

    outfile.close();

}