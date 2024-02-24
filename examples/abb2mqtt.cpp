/*

abb2mqtt

comes from example_dsp.cpp

reads inverter values, prints them to stdout and publishes to mqtt server

needs mosquitto a libabbaurora libraries
*/
#include <iostream>
#include <iomanip>
#include <memory>
#include <cstring>
#include <thread>
#include <chrono>
#include "ABBAurora.h"
#include <mosquitto.h>

void publishValue(float value, const char *topic, struct mosquitto *mosq)
{
    // std::cout << "Value: " << value << std::endl;

    const char *value_str = std::to_string(value).c_str();

    int rc = mosquitto_publish(mosq, NULL, topic, strlen(value_str), value_str, 0, false);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        std::cerr << "Error publishing message: " << mosquitto_strerror(rc) << std::endl;
    }
}

int main(int argc, char *argv[])
{

    //-----------argument handling-------------------------------------------------------------------------------------

    if (argc != 3)
    {

        std::cerr << "Usage: " << argv[0] << " <device_path> <integer_argument>" << std::endl;
        return EXIT_FAILURE;
    }

    // Get the device path from command-line argument (/dev/ttyUSBx)
    const std::string device = argv[1];
    // get inverter RS485 address
    int address485 = std::atoi(argv[2]);

    //-----------mosquitto init-------------------------------------------------------------------------------------
    mosquitto_lib_init();
    struct mosquitto *mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq)
    {
        std::cerr << "Error creating Mosquitto client." << std::endl;
        return 1;
    }
    int rc = mosquitto_connect(mosq, "net.ad.kolins.cz", 1883, 60);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        std::cerr << "Error connecting to broker: " << mosquitto_strerror(rc) << std::endl;
        return 1;
    }

    const char *message = "Hello, MQTT!";
    rc = mosquitto_publish(mosq, NULL, "aurora/mytopic", strlen(message), message, 0, false);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        std::cerr << "Error publishing message: " << mosquitto_strerror(rc) << std::endl;
        return 1;
    }
    //-----------inverter comm init-------------------------------------------------------------------------------------
    std::unique_ptr<ABBAurora> inverter(new ABBAurora(address485));

    if (!inverter->Setup(device))
    {
        std::cout << "Inverter setup failed: " << inverter->GetErrorMessage() << std::endl;

        return EXIT_FAILURE;
    }

    std::ios::fmtflags old_settings = std::cout.flags();
    std::cout.precision(2);
    std::cout.setf(std::ios::fixed, std::ios::floatfield);

    while (true)
    {
        //-----------get single parameter-------------------------------------------------------------------------------------

        float voltage_in_1;
        if (!inverter->ReadDspValue(voltage_in_1, DspValueEnum::V_IN_1))
        {
            std::cout << inverter->GetErrorMessage() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Input 1 Voltage: " << voltage_in_1 << " V" << std::endl;
        publishValue(voltage_in_1, "aurora/voltage_in_1", mosq);

        //-----------get single parameter-------------------------------------------------------------------------------------
        float current_in_1;
        if (!inverter->ReadDspValue(current_in_1, DspValueEnum::I_IN_1))
        {
            std::cout << inverter->GetErrorMessage() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Input 1 Current: " << current_in_1 << " A" << std::endl;
        publishValue(current_in_1, "aurora/current_in_1", mosq);

        //-----------get single parameter-------------------------------------------------------------------------------------
        float power_in_1;
        if (!inverter->ReadDspValue(power_in_1, DspValueEnum::POWER_IN_1))
        {
            std::cout << inverter->GetErrorMessage() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Input 1 Power: " << power_in_1 << " W" << std::endl;
        publishValue(power_in_1, "aurora/power_in_1", mosq);
        //-----------get single parameter-------------------------------------------------------------------------------------
        float voltage_in_2;
        if (!inverter->ReadDspValue(voltage_in_2, DspValueEnum::V_IN_2))
        {
            std::cout << inverter->GetErrorMessage() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Input 2 Voltage: " << voltage_in_2 << " V" << std::endl;
        publishValue(voltage_in_2, "aurora/voltage_in_2", mosq);

        //-----------get single parameter-------------------------------------------------------------------------------------
        float current_in_2;
        if (!inverter->ReadDspValue(current_in_2, DspValueEnum::I_IN_2))
        {
            std::cout << inverter->GetErrorMessage() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Input 2 Current: " << current_in_2 << " A" << std::endl;
        publishValue(current_in_2, "aurora/current_in_2", mosq);

        //-----------get single parameter-------------------------------------------------------------------------------------
        float power_in_2;
        if (!inverter->ReadDspValue(power_in_2, DspValueEnum::POWER_IN_2))
        {
            std::cout << inverter->GetErrorMessage() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Input 2 Power: " << power_in_2 << " W" << std::endl;
        publishValue(power_in_2, "aurora/power_in_2", mosq);

        //-----------get single parameter-------------------------------------------------------------------------------------
        float grid_voltage;
        if (!inverter->ReadDspValue(grid_voltage, DspValueEnum::GRID_VOLTAGE))
        {
            std::cout << inverter->GetErrorMessage() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Grid Voltage Reading: " << grid_voltage << " V" << std::endl;
        publishValue(grid_voltage, "aurora/grid_voltage", mosq);

        //-----------get single parameter-------------------------------------------------------------------------------------
        float grid_current;
        if (!inverter->ReadDspValue(grid_current, DspValueEnum::GRID_CURRENT))
        {
            std::cout << inverter->GetErrorMessage() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Grid Current Reading: " << grid_current << " A" << std::endl;
        publishValue(grid_current, "aurora/grid_current", mosq);

        //-----------get single parameter-------------------------------------------------------------------------------------
        float grid_power;
        if (!inverter->ReadDspValue(grid_power, DspValueEnum::GRID_POWER))
        {
            std::cout << inverter->GetErrorMessage() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Grid Power Reading: " << grid_power << " W" << std::endl;
        publishValue(grid_power, "aurora/grid_power", mosq);

        //-----------get single parameter-------------------------------------------------------------------------------------
        float freq;
        if (!inverter->ReadDspValue(freq, DspValueEnum::FREQUENCY))
        {
            std::cout << inverter->GetErrorMessage() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Frequency Reading: " << freq << " Hz" << std::endl;
        publishValue(freq, "aurora/freq", mosq);

        //-----------get single parameter-------------------------------------------------------------------------------------
        float efficiency = (grid_power / (power_in_1 + power_in_2) * 100);
        std::cout << "DC/AC Conversion Efficiency: " << efficiency << " %" << std::endl;
        publishValue(efficiency, "aurora/efficiency", mosq);

        //-----------get single parameter-------------------------------------------------------------------------------------
        float inverter_temp;
        if (!inverter->ReadDspValue(inverter_temp, DspValueEnum::TEMPERATURE_INVERTER))
        {
            std::cout << inverter->GetErrorMessage() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Inverter Temperature: " << inverter_temp << " °C" << std::endl;
        publishValue(inverter_temp, "aurora/inverter_temp", mosq);

        //-----------get single parameter-------------------------------------------------------------------------------------
        float booster_temp;
        if (!inverter->ReadDspValue(booster_temp, DspValueEnum::TEMPERATURE_BOOSTER))
        {
            std::cout << inverter->GetErrorMessage() << std::endl;
            return EXIT_FAILURE;
        }
        std::cout << "Booster Temperature: " << booster_temp << " °C" << std::endl;
        publishValue(booster_temp, "aurora/booster_temp", mosq);

        //-----------wait here one second-------------------------------------------------------------------------------------
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    //-----------end-------------------------------------------------------------------------------------
    std::cout.flags(old_settings);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return EXIT_SUCCESS;
}
