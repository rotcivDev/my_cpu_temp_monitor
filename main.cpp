#include <gtk/gtk.h>
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

static gboolean update_temperature(gpointer data) {
    // Get CPU temperature
    float tempC = get_cpu_temperature();
    if (tempC != -1.0f) {
        // Update label text
        char tempStr[20];
        snprintf(tempStr, sizeof(tempStr), "%.1f°C", tempC);
        gtk_label_set_text(GTK_LABEL(data), tempStr);
    }
    return G_SOURCE_CONTINUE;
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CPU Temperature");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a label to display temperature
    GtkWidget *label = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(window), label);

    // Update temperature label every second
    g_timeout_add_seconds(1, update_temperature, label);

    // Show the window
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
