#include <iomanip>
#include "Core/Engine.h"
#include "Serializables/VulkanTypes.h"
#include "Serializables/GenericTypes.h"
#include "Core/ECS/Component.h"
#include "Math/Math.h"

ApplicationSettings readAppSettings() {

    std::ifstream file(PATH_CONFIG_FILES + "application.json");
    if (!file.is_open()) {
        Logger::failure("Could not read application settings file!");
    }

    std::stringstream appSettingsFileBuffer;
    appSettingsFileBuffer << file.rdbuf();

    auto appSettingsJSON = json::parse(appSettingsFileBuffer.str());

    ApplicationSettings applicationSettings = appSettingsJSON.get<ApplicationSettings>();
    file.close();

    return applicationSettings;
}

GraphicsSettings readGraphicsSettings() {


    std::ifstream file(PATH_CONFIG_FILES + "graphics.json");
    if (!file.is_open()) {
        Logger::failure("Could not read graphics settings file!");
    }

    std::stringstream graphicsSettingsBuffer;
    graphicsSettingsBuffer << file.rdbuf();

    auto graphicsSettingsJSON = json::parse(graphicsSettingsBuffer.str());

    GraphicsSettings graphicsSettings = graphicsSettingsJSON.get<GraphicsSettings>();
    file.close();

    return graphicsSettings;
}

int main() {

//    using namespace Math;
//
//    Matrix2x2f mat(3,4,5,6);
//    Matrix2x2f mat2(0,1,2,3);
//
//
//    json something;
//    something = multiply(mat,mat2);
//
//    std::cout << std::setw(4) << something << std::endl;

    Engine engine(readAppSettings(), readGraphicsSettings());
    engine.run();

    return 0;
}