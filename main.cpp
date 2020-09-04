#include <fstream>
#include <iostream>
#include <string>
#include <vector>


using std::cout;
using std::endl;
using std::string;
using std::vector;


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


    // Arrays -----------------------------------
    // int process_id[1000];
    // int burst_time[1000];
    // int arrival_time[1000];
    // int num = 0;

    // if (infile.fail()) {
    //     cout << "Error - could not open file" << endl;
    //     return EXIT_FAILURE;
    // } else {
    //     while (!infile.eof()) {
    //         infile >> process_id[num] >> burst_time[num] >> arrival_time[num];
    //         ++num;
    //     }
    // }
    // infile.close();
    // -------------------------------------------

    // Vectors -----------------------------------

    vector<int> process_id;
    vector<int> burst_time;
    vector<int> arrival_time;

    if (infile.fail()) {
        cout << "Error - could not open file" << endl;
        return EXIT_FAILURE;
    } else {
        while (!infile.eof()) {

            int a, b, c;

            infile >> a >> b >> c;
            process_id.push_back(a);
            burst_time.push_back(b);
            arrival_time.push_back(c);

        }
    }

    cout << process_id.size() << endl;

    infile.close();
    // -------------------------------------------

    // Check for which process scheduler
    if (argv[1] == string("-fcfs")) {
        cout << "FCFS" << endl;
        // fcfsSimulation();

        // for (int i = 0; i < process_id.size(); ++i) {
        //     cout << process_id[i] << " " << burst_time[i] << " " << arrival_time[i] << endl;
        // }

        for (int i : process_id) {
            cout << process_id[i-1] << " " << burst_time[i-1] << " " << arrival_time[i-1] <<endl;;
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