#pragma once
#include "Device.h"
#include <memory>
#include <vector>
#include <stack>
#include <string>

using namespace std;

// Абстрактная команда
class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual string getName() = 0;  
};

// Команда включения устройства
class TurnOnCommand : public ICommand {
private:
    shared_ptr<Device> device;
    string commandName;

public:
    TurnOnCommand(shared_ptr<Device> dev) : device(dev) {
        commandName = "Включить " + device->getName();
    }

    void execute() override {
        device->turnOn();
    }

    void undo() override {
        device->turnOff();
    }

    string getName() override {
        return commandName;
    }
};

// Команда выключения устройства
class TurnOffCommand : public ICommand {
private:
    shared_ptr<Device> device;
    string commandName;  

public:
    TurnOffCommand(shared_ptr<Device> dev) : device(dev) {
        commandName = "Выключить " + device->getName();
    }

    void execute() override {
        device->turnOff();
    }

    void undo() override {
        device->turnOn();
    }

    string getName() override {
        return commandName;
    }
};

// Макрокоманда
class MacroCommand : public ICommand {
private:
    vector<shared_ptr<ICommand>> commands;
    string commandName;  

public:
    MacroCommand(const string& name) : commandName(name) {}

    void addCommand(shared_ptr<ICommand> cmd) {
        commands.push_back(cmd);
    }

    void execute() override {
        cout << "\n--- Выполнение макрокоманды: " << commandName << " ---" << endl;
        for (auto& cmd : commands) {
            cmd->execute();
        }
    }

    void undo() override {
        cout << "\n--- Отмена макрокоманды: " << commandName << " ---" << endl;
        for (auto it = commands.rbegin(); it != commands.rend(); ++it) {
            (*it)->undo();
        }
    }

    string getName() override {
        return commandName;
    }
};

// Пульт управления
class RemoteControl {
private:
    vector<shared_ptr<ICommand>> commandHistory;
    shared_ptr<ICommand> lastCommand;

public:
    void pressButton(shared_ptr<ICommand> command) {
        command->execute();
        commandHistory.push_back(command);
        lastCommand = command;
    }

    void undoLastCommand() {
        if (lastCommand) {
            cout << "\n<<< ОТМЕНА: " << lastCommand->getName() << " >>>" << endl;
            lastCommand->undo();
            if (!commandHistory.empty()) {
                commandHistory.pop_back();
            }
            if (!commandHistory.empty()) {
                lastCommand = commandHistory.back();
            }
            else {
                lastCommand = nullptr;
            }
        }
        else {
            cout << "Нет команд для отмены" << endl;
        }
    }

    void showHistory() {
        cout << "\n=== История команд ===" << endl;
        if (commandHistory.empty()) {
            cout << "История пуста" << endl;
            return;
        }
        for (size_t i = 0; i < commandHistory.size(); i++) {
            cout << i + 1 << ". " << commandHistory[i]->getName() << endl;
        }
    }
};