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

        // Update text color based on temperature thresholds
        GdkRGBA color;
        if (tempC < 50.0f)
            gdk_rgba_parse(&color, "green");
        else if (tempC < 65.0f)
            gdk_rgba_parse(&color, "orangered");
        else
            gdk_rgba_parse(&color, "red");

        gtk_widget_override_color(GTK_WIDGET(data), GTK_STATE_FLAG_NORMAL, &color);
    }
    return G_SOURCE_CONTINUE;
}

static void close_window(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CPU Temperature");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE); // Remove window decorations

    // Set window to always be on top
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);

    // Set window to fixed position at top right corner
    GdkScreen *screen = gdk_screen_get_default();
    gint width = gdk_screen_get_width(screen);
    gtk_window_move(GTK_WINDOW(window), width - 200, 0);

    // Set window background to transparent
    GdkRGBA color = {0, 0, 0, 0}; // Transparent RGBA color
    gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &color);

    // Create a label to display temperature
    GtkWidget *label = gtk_label_new("");
    gtk_container_add(GTK_CONTAINER(window), label);

    // Update temperature label every second
    g_timeout_add_seconds(1, update_temperature, label);

    // Create a button to close the window
    GtkWidget *button = gtk_button_new_with_label("Close");
    g_signal_connect(button, "clicked", G_CALLBACK(close_window), NULL);
    gtk_container_add(GTK_CONTAINER(window), button);

    // Show the window and its contents
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
