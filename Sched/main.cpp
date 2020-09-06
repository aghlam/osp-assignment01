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
    int burst_time;
    int arrival_time;
    int waiting_time;
    int turnaround_time;
    bool running;

};


void fcfsSimulation(vector<Process> processes);
void sjfSimulation(vector<Process> processes);


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
            p.running = false;

            processes.push_back(p);

        }
    }

    infile.close();


    // Check for which process scheduler
    if (argv[1] == string("-fcfs")) {
        cout << "Running FCFS" << endl;
        fcfsSimulation(processes);

    } else if (argv[1] == string("-sjf")) {
        cout << "Running SJF" << endl;
        sjfSimulation(processes);

    } else if (argv[1] == string("-rr")) {
        cout << "RR" << endl;
    } else {
        cout << "Incorrect scheduling input" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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

        // Delete after - printing for checking
        // cout << "Waiting time for process using Struct " << processes[i].process_id << ": " << processes[i].waiting_time << endl;
    }

    // Calculating turnaround time
    for(unsigned int i = 0; i < processes.size(); ++i) {
        processes[i].turnaround_time = processes[i].burst_time + processes[i].waiting_time;

        // Delete after - printing for checking
        // cout << "Turnaround time for process using Struct " << processes[i].process_id << ": " << processes[i].turnaround_time << endl;
    }

    // Output CSV file with waiting time and turnaround time
    outfile.open("FCFS-output.csv");

    outfile << "Process_id," << "Waiting_time," << "Turnaround_time" << endl;

    for (unsigned int i = 0; i < processes.size(); ++i) {
        outfile << processes[i].process_id << "," << processes[i].waiting_time << "," << processes[i].turnaround_time << endl;
    }

    outfile.close();

}


void sjfSimulation(vector<Process> processes) {

    vector<Process> runningProcess;
    int totalBurstTime = 0;
    int totalRunningTime = 0;
    int currentRunningTime = 0;
    int processCounter = 0;

    // First process to be run added to running queue
    runningProcess.push_back(processes[0]);
    runningProcess[0].waiting_time = 0;
    runningProcess[0].running = true;
    currentRunningTime += runningProcess[0].burst_time;
    

    for (unsigned int i = 0; i < processes.size(); ++i) {
        totalBurstTime += processes[i].burst_time;
    }

    // Timer for loop
    for (int timer = 0; timer < totalBurstTime; ++timer) {

        if (timer == currentRunningTime) {
            ++processCounter;
            runningProcess[processCounter].running = true;
            currentRunningTime += runningProcess[processCounter].burst_time;
        }

        // if(currentRunningTime == runningProcess[processCounter].burst_time) {
        //     currentRunningTime = 0;
        //     ++processCounter;
        //     runningProcess[processCounter].running = true;
        // }


        for (unsigned int i = 1; i < processes.size(); ++i) {
            // Add process to running process according to arrival time
            if (timer == processes[i].arrival_time) {
                runningProcess.push_back(processes[i]);

                for (unsigned int j = 0; j < runningProcess.size(); ++j) {
                    cout  << "Process id: " << runningProcess[j].process_id << " | Burst time: " << runningProcess[j].burst_time << " | Arrival time: " << runningProcess[j].arrival_time << endl;
                }

                cout << endl;
            }
        }

        std::sort(runningProcess.begin(), runningProcess.end(), [](Process a, Process b) -> bool {
            if (a.running == false && b.running == false) {
                return a.burst_time < b.burst_time;
            } else {
                return false;
            }
        });


        ++totalRunningTime;

    }

    for (unsigned int j = 0; j < runningProcess.size(); ++j) {
        cout  << "Process id: " << runningProcess[j].process_id << " | Burst time: " << runningProcess[j].burst_time << " | Arrival time: " << runningProcess[j].arrival_time << endl;
    }

        cout << endl;

}