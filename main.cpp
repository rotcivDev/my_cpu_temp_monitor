#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;

const string CPU_TEMP_FILE = "/sys/class/thermal/thermal_zone0/temp"; // Adjust this path if necessary

float get_cpu_temperature() {
    ifstream tempFile(CPU_TEMP_FILE);
    if (!tempFile.is_open()) {
        cerr << "Error: Could not open CPU temperature file." << endl;
        return -1.0f;
    }
    
    string tempStr;
    getline(tempFile, tempStr);
    tempFile.close();
    
    // Convert temperature to Celsius
    float tempC = stof(tempStr) / 1000.0f;
    return tempC;
}

int main() {
    while (true) {
        system("clear");
        float tempC = get_cpu_temperature();
        if (tempC != -1.0f) {
            cout << "CPU Temperature: " << tempC << "°C" << endl;
        }
        
        // Sleep for 1 second before checking temperature again
        sleep(1);
    }
    
    return 0;
}
