#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Класс для света
class Light {
private:
    string name;
    bool isOn;

public:
    Light(const string& lightName) : name(lightName), isOn(false) {}

    void turnOn() {
        isOn = true;
        cout << name << ": Свет включен" << endl;
    }

    void turnOff() {
        isOn = false;
        cout << name << ": Свет выключен" << endl;
    }

    void showStatus() {
        cout << name << ": " << (isOn ? "Включен" : "Выключен") << endl;
    }
};

// Класс для кондиционера
class AirConditioner {
private:
    string name;
    bool isOn;

public:
    AirConditioner(const string& acName) : name(acName), isOn(false) {}

    void turnOn() {
        isOn = true;
        cout << name << ": Кондиционер включен" << endl;
    }

    void turnOff() {
        isOn = false;
        cout << name << ": Кондиционер выключен" << endl;
    }

    void showStatus() {
        cout << name << ": " << (isOn ? "Включен" : "Выключен") << endl;
    }
};

// Класс для комнаты (содержит устройства)
class Room {
private:
    string name;
    vector<Light> lights;
    vector<AirConditioner> airConditioners;

public:
    Room(const string& roomName) : name(roomName) {}

    void addLight(const string& lightName) {
        lights.push_back(Light(lightName));
        cout << "Свет '" << lightName << "' добавлен в " << name << endl;
    }

    void addAirConditioner(const string& acName) {
        airConditioners.push_back(AirConditioner(acName));
        cout << "Кондиционер '" << acName << "' добавлен в " << name << endl;
    }

    void turnOnAllLights() {
        cout << "\n--- Включаем весь свет в " << name << " ---" << endl;
        for (auto& light : lights) {
            light.turnOn();
        }
    }

    void turnOffAllLights() {
        cout << "\n--- Выключаем весь свет в " << name << " ---" << endl;
        for (auto& light : lights) {
            light.turnOff();
        }
    }

    void turnOnAllACs() {
        cout << "\n--- Включаем все кондиционеры в " << name << " ---" << endl;
        for (auto& ac : airConditioners) {
            ac.turnOn();
        }
    }

    void turnOffAllACs() {
        cout << "\n--- Выключаем все кондиционеры в " << name << " ---" << endl;
        for (auto& ac : airConditioners) {
            ac.turnOff();
        }
    }

    void turnOnAll() {
        cout << "\n=== Включаем всё в " << name << " ===" << endl;
        turnOnAllLights();
        turnOnAllACs();
    }

    void turnOffAll() {
        cout << "\n=== Выключаем всё в " << name << " ===" << endl;
        turnOffAllLights();
        turnOffAllACs();
    }

    void showStatus() {
        cout << "\n=== Состояние комнаты '" << name << "' ===" << endl;

        if (lights.empty() && airConditioners.empty()) {
            cout << "В комнате нет устройств" << endl;
            return;
        }

        if (!lights.empty()) {
            cout << "--- Освещение ---" << endl;
            for (auto& light : lights) {
                light.showStatus();
            }
        }

        if (!airConditioners.empty()) {
            cout << "--- Кондиционеры ---" << endl;
            for (auto& ac : airConditioners) {
                ac.showStatus();
            }
        }
    }
};

// Класс для дома
class House {
private:
    string name;
    vector<Room> rooms;

public:
    House(const string& houseName) : name(houseName) {}

    void addRoom(const string& roomName) {
        rooms.push_back(Room(roomName));
        cout << "Комната '" << roomName << "' добавлена в дом" << endl;
    }

    Room* getRoom(const string& roomName) {
        for (auto& room : rooms) {
            // В реальной программе нужно сравнивать имена
            // Здесь для простоты возвращаем первую комнату
            if (rooms.size() > 0) return &rooms[0];
        }
        return nullptr;
    }

    void turnOnEverything() {
        cout << "\n========== Включаем ВСЁ в доме ==========" << endl;
        for (auto& room : rooms) {
            room.turnOnAll();
        }
    }

    void turnOffEverything() {
        cout << "\n========== Выключаем ВСЁ в доме ==========" << endl;
        for (auto& room : rooms) {
            room.turnOffAll();
        }
    }

    void showFullStatus() {
        cout << "\n========== СОСТОЯНИЕ ДОМА '" << name << "' ==========" << endl;
        if (rooms.empty()) {
            cout << "В доме нет комнат" << endl;
            return;
        }

        for (auto& room : rooms) {
            room.showStatus();
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");

    return 0;
}