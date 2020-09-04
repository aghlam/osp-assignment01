#include <fstream>
#include <iostream>
#include <string>


using std::cout;
using std::endl;
using std::string;


void fcfsSimulation();


int main(int argc, char** argv) {

    // string processFile = "";
    std::ifstream infile;

    // Check for correct number of arguments
    if (argc != 3) {
        cout << "Error - requires two arguments to run." << endl;
        return EXIT_FAILURE;
    } 

    // Read in file
    infile.open(argv[2]);

    // Test arrays
    int process_id[1000];
    int burst_time[1000];
    int arrival_time[1000];
    int num = 0;

    if (infile.fail()) {
        cout << "Error - could not open file" << endl;
        return EXIT_FAILURE;
    } else {
        while (!infile.eof()) {
            infile >> process_id[num] >> burst_time[num] >> arrival_time[num];
            ++num;
        }
    }

    infile.close();

    // Check for which process scheduler
    if (argv[1] == string("-fcfs")) {
        cout << "FCFS" << endl;
        // fcfsSimulation();

        for (int i = 0; i < num; ++i) {
            cout << process_id[i] << " " << burst_time[i] << " " << arrival_time[i] << endl;
        }
    } else if (argv[1] == string("-sjf")) {
        cout << "SJF" << endl;
    } else if (argv[1] == string("-rr")) {
        cout << "RR" << endl;
    } else {
        cout << "Incorrect scheduling input" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


void fcfsSimulation() {

}