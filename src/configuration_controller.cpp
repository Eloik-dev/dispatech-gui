#include "header/configuration_controller.h"

ConfigurationController::ConfigurationController(Settings *settings)
{
    this->_settings = settings;
    this->updateLocalConfig();
    this->updateFiles();
}

void ConfigurationController::updateLocalConfig()
{
    RequestController *requestController = new RequestController(this->_settings);
    json result = requestController->get(this->_settings->hostname + "/api/playlist/getPartitionConfig");
    int status = result.at("status").get<int>();

    switch (status)
    {
    case HttpCodes::SUCCESS:
        std::ofstream configFile(CONFIGURATION_FILE);
        if (configFile.is_open())
        {
            configFile << result.at("data").dump(4);
            configFile.close();
        }
        else
        {
            std::cerr << "Unable to open file for writing" << std::endl;
        }
        break;
    }
}

void ConfigurationController::updateFiles()
{
    RequestController *requestController = new RequestController(this->_settings);
    json result = requestController->get(this->_settings->hostname + "/api/playlist/getPartitionLinks");
    int status = result.at("status").get<int>();
    json links = result.at("data");

    switch (status)
    {
    case HttpCodes::SUCCESS:
        for (json::iterator it = links.begin(); it != links.end(); ++it)
        {
            json linkData = it.value();
            std::string fileName = linkData.at("name");
            std::string filePath = TEMP_DIRECTORY + fileName;

            bool success = requestController->getFile(linkData.at("link"), filePath);
            if (success == false)
            {
                std::cerr << "Failed to download file at: " << filePath << std::endl;
                continue;
            }

            std::cout << "File saved to " << filePath << std::endl;
        }

        break;
    }

    std::filesystem::remove_all(FILES_DIRECTORY);
    std::filesystem::copy(TEMP_DIRECTORY, FILES_DIRECTORY);
    std::filesystem::remove_all(TEMP_DIRECTORY);
}