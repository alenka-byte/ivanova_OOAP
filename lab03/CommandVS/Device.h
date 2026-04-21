#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

// Базовый абстрактный класс (компонент)
class Device {
protected:
    string name;
    bool isOn;

public:
    Device(const string& deviceName) : name(deviceName), isOn(false) {}
    virtual ~Device() = default;

    virtual void turnOn() {
        isOn = true;
        cout << name << " включен" << endl;
    }

    virtual void turnOff() {
        isOn = false;
        cout << name << " выключен" << endl;
    }

    virtual void showStatus() {
        cout << name << ": " << (isOn ? "включен" : "выключен") << endl;
    }

    virtual void add(shared_ptr<Device> device) {}

    virtual void remove(shared_ptr<Device> device) {}

    virtual string getName() { return name; }

    virtual bool getIsOn() { return isOn; }
};

// Лист: Свет
class Light : public Device {
public:
    Light(const string& lightName) : Device(lightName) {}
};

// Лист: Кондиционер
class AirConditioner : public Device {
public:
    AirConditioner(const string& acName) : Device(acName) {}
};


// Компоновщик: Комната
class Room : public Device {
private:
    vector<shared_ptr<Device>> devices;

public:
    Room(const string& roomName) : Device(roomName) {}

    void add(shared_ptr<Device> device) override {
        devices.push_back(device);
        cout << "Устройство '" << device->getName() << "' добавлено в комнату '" << name << "'" << endl;
    }

    void remove(shared_ptr<Device> device) override {
        for (auto it = devices.begin(); it != devices.end(); ++it) {
            if ((*it)->getName() == device->getName()) {
                devices.erase(it);
                cout << "Устройство '" << device->getName() << "' удалено из комнаты '" << name << "'" << endl;
                return;
            }
        }
    }

    void turnOn() override {
        cout << "\n--- Включение всех устройств в комнате '" << name << "' ---" << endl;
        for (auto& device : devices) {
            device->turnOn();
        }
    }

    void turnOff() override {
        cout << "\n--- Выключение всех устройств в комнате '" << name << "' ---" << endl;
        for (auto& device : devices) {
            device->turnOff();
        }
    }

    void showStatus() override {
        cout << "\n=== Состояние комнаты '" << name << "' ===" << endl;
        if (devices.empty()) {
            cout << "В комнате нет устройств" << endl;
            return;
        }
        for (auto& device : devices) {
            device->showStatus();
        }
    }

    vector<shared_ptr<Device>> getDevices() {
        return devices;
    }
};

// Компоновщик: Дом
class House : public Device {
private:
    vector<shared_ptr<Device>> rooms;

public:
    House(const string& houseName) : Device(houseName) {}

    void add(shared_ptr<Device> room) override {
        rooms.push_back(room);
        cout << "Комната '" << room->getName() << "' добавлена в дом '" << name << "'" << endl;
    }

    void remove(shared_ptr<Device> room) override {
        for (auto it = rooms.begin(); it != rooms.end(); ++it) {
            if ((*it)->getName() == room->getName()) {
                rooms.erase(it);
                cout << "Комната '" << room->getName() << "' удалена из дома '" << name << "'" << endl;
                return;
            }
        }
    }

    void turnOn() override {
        cout << "\n========== Включение ВСЕГО в доме '" << name << "' ==========" << endl;
        for (auto& room : rooms) {
            room->turnOn();
        }
    }

    void turnOff() override {
        cout << "\n========== Выключение ВСЕГО в доме '" << name << "' ==========" << endl;
        for (auto& room : rooms) {
            room->turnOff();
        }
    }

    void showStatus() override {
        cout << "\n========== Состояние дома '" << name << "' ==========" << endl;
        if (rooms.empty()) {
            cout << "В доме нет комнат" << endl;
            return;
        }
        for (auto& room : rooms) {
            room->showStatus();
        }
    }

    shared_ptr<Device> findRoom(const string& roomName) {
        for (auto& room : rooms) {
            if (room->getName() == roomName) {
                return room;
            }
        }
        return nullptr;
    }

    vector<shared_ptr<Device>> getRooms() {
        return rooms;
    }
};