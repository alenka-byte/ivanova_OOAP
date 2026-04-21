#pragma once
#include "Device.h"
#include <memory>
#include <vector>
#include <string>

using namespace std;

// Простой контроллер устройств без истории команд
class SimpleController {
public:
    // Прямое управление устройствами
    void turnOn(shared_ptr<Device> device) {
        device->turnOn();
    }

    void turnOff(shared_ptr<Device> device) {
        device->turnOff();
    }

    void turnOnRoom(Room* room) {
        if (room) {
            room->turnOn();
        }
    }

    void turnOffRoom(Room* room) {
        if (room) {
            room->turnOff();
        }
    }

    void turnOnHouse(House* house) {
        if (house) {
            house->turnOn();
        }
    }

    void turnOffHouse(House* house) {
        if (house) {
            house->turnOff();
        }
    }

    // Получение статуса устройства
    string getDeviceStatus(shared_ptr<Device> device) {
        return device->getName() + ": " + (device->getIsOn() ? "включен" : "выключен");
    }
};