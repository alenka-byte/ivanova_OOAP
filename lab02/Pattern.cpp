#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

// ������� ����������� ����� (���������)
class Device {
protected:
    string name;
    bool isOn;

public:
    Device(const string& deviceName) : name(deviceName), isOn(false) {}
    virtual ~Device() = default;

    // ������� ��������
    virtual void turnOn() {
        isOn = true;
        cout << name << " �������" << endl;
    }

    virtual void turnOff() {
        isOn = false;
        cout << name << " ��������" << endl;
    }

    virtual void showStatus() {
        cout << name << ": " << (isOn ? "�������" : "��������") << endl;
    }

    virtual void add(shared_ptr<Device> device) {}

    virtual void remove(shared_ptr<Device> device) {}

    virtual string getName() { return name; }
};

// ����: ����
class Light : public Device {
public:
    Light(const string& lightName) : Device(lightName) {}

    void turnOn() override {
        Device::turnOn();
    }

    void turnOff() override {
        Device::turnOff();
    }
};

// ����: �����������
class AirConditioner : public Device {
public:
    AirConditioner(const string& acName) : Device(acName) {}

    void turnOn() override {
        Device::turnOn();
    }

    void turnOff() override {
        Device::turnOff();
    }
};

// �����������: ������� (����� ��������� ����������)
class Room : public Device {
private:
    vector<shared_ptr<Device>> devices;

public:
    Room(const string& roomName) : Device(roomName) {}

    void add(shared_ptr<Device> device) override {
        devices.push_back(device);
        cout << "���������� '" << device->getName() << "' ��������� � ������� '" << name << "'" << endl;
    }

    void remove(shared_ptr<Device> device) override {
        for (auto it = devices.begin(); it != devices.end(); ++it) {
            if ((*it)->getName() == device->getName()) {
                devices.erase(it);
                cout << "���������� '" << device->getName() << "' ������� �� ������� '" << name << "'" << endl;
                return;
            }
        }
    }

    void turnOn() override {
        cout << "\n--- �������� ��� ���������� � ������� '" << name << "' ---" << endl;
        for (auto& device : devices) {
            device->turnOn();
        }
    }

    void turnOff() override {
        cout << "\n--- ��������� ��� ���������� � ������� '" << name << "' ---" << endl;
        for (auto& device : devices) {
            device->turnOff();
        }
    }

    void showStatus() override {
        cout << "\n=== ��������� ������� '" << name << "' ===" << endl;
        if (devices.empty()) {
            cout << "� ������� ��� ���������" << endl;
            return;
        }
        for (auto& device : devices) {
            device->showStatus();
        }
    }
};

// �����������: ��� (����� ��������� �������)
class House : public Device {
private:
    vector<shared_ptr<Device>> rooms;

public:
    House(const string& houseName) : Device(houseName) {}

    void add(shared_ptr<Device> room) override {
        rooms.push_back(room);
        cout << "������� '" << room->getName() << "' ��������� � ��� '" << name << "'" << endl;
    }

    void remove(shared_ptr<Device> room) override {
        for (auto it = rooms.begin(); it != rooms.end(); ++it) {
            if ((*it)->getName() == room->getName()) {
                rooms.erase(it);
                cout << "������� '" << room->getName() << "' ������� �� ���� '" << name << "'" << endl;
                return;
            }
        }
    }

    void turnOn() override {
        cout << "\n========== �������� �Ѩ � ���� '" << name << "' ==========" << endl;
        for (auto& room : rooms) {
            room->turnOn();
        }
    }

    void turnOff() override {
        cout << "\n========== ��������� �Ѩ � ���� '" << name << "' ==========" << endl;
        for (auto& room : rooms) {
            room->turnOff();
        }
    }

    void showStatus() override {
        cout << "\n========== ��������� ���� '" << name << "' ==========" << endl;
        if (rooms.empty()) {
            cout << "� ���� ��� ������" << endl;
            return;
        }
        for (auto& room : rooms) {
            room->showStatus();
        }
    }

    // ����� ������� �� �����
    shared_ptr<Device> findRoom(const string& roomName) {
        for (auto& room : rooms) {
            if (room->getName() == roomName) {
                return room;
            }
        }
        return nullptr;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    return 0;
}