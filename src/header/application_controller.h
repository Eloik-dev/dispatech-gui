#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <gtk/gtk.h>
#include <filesystem>
#include <iostream>
#include "file_manager.h"

class Controller
{
private:
    GtkApplication *application;
    GtkWidget *window;
    FileManager *_fileManager;

    /**
     * Callback d'activation de l'application
     */
    static void initializeWindow(GtkApplication *, gpointer);

    static gboolean timeoutCallback(gpointer);

    static gboolean onRestartSlideshow(gpointer arguments);

    /**
     * Shows the next file to be shown
     */
    void startSlideshow();

    static int getFileExtensionCode(std::string);
    static GtkWidget *getImageWidget(File *);
    static GtkWidget *getVideoWidget(File *);

    /**
     * Validates that a file exists
     */
    static bool file_exists(std::string fileName);

public:
    Controller(FileManager *fileManager);

    /**
     * Fait l'initialisation de l'application GTK4
     */
    void initializeApplication(int, char **);
};

#endif