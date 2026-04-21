#pragma once

#include "Device.h"
#include "Commands.h"
#include <msclr/marshal_cppstd.h>
#include <memory>
#include <vector>
#include <string>
#include <map>

using namespace std;

namespace SmartHomeUI {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    // Вспомогательный управляемый класс-обертка для хранения неуправляемых данных
    ref class SmartHomeData
    {
    public:
        House* house;
        SimpleController* controller;
        vector<pair<string, shared_ptr<Device>>>* availableCommands;
        map<string, vector<shared_ptr<Device>>>* deviceCommandsMap;

        SmartHomeData()
        {
            house = nullptr;
            controller = nullptr;
            availableCommands = nullptr;
            deviceCommandsMap = new map<string, vector<shared_ptr<Device>>>();
        }

        ~SmartHomeData()
        {
            delete house;
            delete controller;
            delete availableCommands;
            delete deviceCommandsMap;
        }
    };

    public ref class MainForm : public Form
    {
    private:
        String^ ShowInputDialog(String^ prompt, String^ title, String^ defaultValue)
        {
            Form^ dialog = gcnew Form();
            Label^ lblPrompt = gcnew Label();
            TextBox^ txtInput = gcnew TextBox();
            Button^ btnOk = gcnew Button();
            Button^ btnCancel = gcnew Button();

            dialog->Text = title;
            dialog->StartPosition = FormStartPosition::CenterParent;

            dialog->ControlBox = false;
            dialog->ClientSize = System::Drawing::Size(320, 130);
            dialog->BackColor = Color::White;

            lblPrompt->Text = prompt;
            lblPrompt->Location = Point(12, 15);
            lblPrompt->Size = System::Drawing::Size(280, 25);
            lblPrompt->Font = gcnew System::Drawing::Font(L"Segoe UI", 10);

            txtInput->Text = defaultValue;
            txtInput->Location = Point(12, 45);
            txtInput->Size = System::Drawing::Size(280, 23);
            txtInput->Font = gcnew System::Drawing::Font(L"Segoe UI", 10);

            btnOk->Text = L"OK";
            btnOk->Location = Point(150, 85);
            btnOk->Size = System::Drawing::Size(70, 30);
            btnOk->BackColor = Color::LightGreen;
            btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;

            btnCancel->Text = L"Отмена";
            btnCancel->Location = Point(230, 85);
            btnCancel->Size = System::Drawing::Size(70, 30);
            btnCancel->BackColor = Color::LightCoral;
            btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

            dialog->AcceptButton = btnOk;
            dialog->CancelButton = btnCancel;

            dialog->Controls->Add(lblPrompt);
            dialog->Controls->Add(txtInput);
            dialog->Controls->Add(btnOk);
            dialog->Controls->Add(btnCancel);

            if (dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                return txtInput->Text;
            }
            return nullptr;
        }

        // Элементы управления
        TreeView^ tvDevices;
        ListBox^ lbCommands;
        Button^ btnExecute;
        Button^ btnUndo;
        Button^ btnShowStatus;
        Button^ btnTurnOnAll;
        Button^ btnTurnOffAll;
        Label^ lblTitle;

        // Новые кнопки для управления структурой
        Button^ btnAddDevice;
        Button^ btnAddRoom;
        Button^ btnAddHouse;
        Button^ btnDeleteDevice;
        Button^ btnDeleteRoom;
        Button^ btnDeleteHouse;

        // Логика умного дома
        SmartHomeData^ smartHome;

        // Текущий выбранный узел
        TreeNode^ selectedNode;

        // Список последних действий для отмены
        vector<pair<string, shared_ptr<Device>>>* actionHistory;

    public:
        MainForm(void)
        {
            InitializeComponent();
            InitializeSmartHome();
            UpdateDeviceTree();
        }

        ~MainForm()
        {
            delete smartHome;
        }

    private:
        void InitializeComponent(void)
        {
            this->Text = L"Умный дом - Система управления";
            this->Size = System::Drawing::Size(900, 650);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->BackColor = Color::LightSteelBlue;

            // Заголовок
            lblTitle = gcnew Label();
            lblTitle->Text = L"🏠 Умный дом 🏠";
            lblTitle->Font = gcnew System::Drawing::Font(L"Arial", 18, FontStyle::Bold);
            lblTitle->Location = Point(350, 20);
            lblTitle->Size = System::Drawing::Size(200, 40);
            lblTitle->TextAlign = ContentAlignment::MiddleCenter;

            // Дерево устройств
            tvDevices = gcnew TreeView();
            tvDevices->Location = Point(20, 70);
            tvDevices->Size = System::Drawing::Size(300, 450);
            tvDevices->Font = gcnew System::Drawing::Font(L"Segoe UI", 10);
            tvDevices->AfterSelect += gcnew TreeViewEventHandler(this, &MainForm::OnDeviceSelected);

            // Список команд
            Label^ lblCommands = gcnew Label();
            lblCommands->Text = L"Доступные команды:";
            lblCommands->Location = Point(340, 70);
            lblCommands->Size = System::Drawing::Size(150, 25);

            lbCommands = gcnew ListBox();
            lbCommands->Location = Point(340, 100);
            lbCommands->Size = System::Drawing::Size(520, 200);
            lbCommands->Font = gcnew System::Drawing::Font(L"Segoe UI", 10);
            lbCommands->SelectedIndexChanged += gcnew EventHandler(this, &MainForm::OnCommandSelected);

            // Панель управления устройствами
            GroupBox^ gbManage = gcnew GroupBox();
            gbManage->Text = L"Управление структурой";
            gbManage->Location = Point(340, 320);
            gbManage->Size = System::Drawing::Size(520, 120);
            gbManage->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
            gbManage->BackColor = Color::LightGoldenrodYellow;

            // Кнопки добавления
            btnAddDevice = gcnew Button();
            btnAddDevice->Text = L"➕ Добавить устройство";
            btnAddDevice->Location = Point(15, 25);
            btnAddDevice->Size = System::Drawing::Size(150, 35);
            btnAddDevice->BackColor = Color::LightGreen;
            btnAddDevice->Click += gcnew EventHandler(this, &MainForm::OnAddDeviceClick);

            btnAddRoom = gcnew Button();
            btnAddRoom->Text = L"🏠 Добавить комнату";
            btnAddRoom->Location = Point(180, 25);
            btnAddRoom->Size = System::Drawing::Size(150, 35);
            btnAddRoom->BackColor = Color::LightGreen;
            btnAddRoom->Click += gcnew EventHandler(this, &MainForm::OnAddRoomClick);

            btnAddHouse = gcnew Button();
            btnAddHouse->Text = L"🏢 Добавить дом";
            btnAddHouse->Location = Point(345, 25);
            btnAddHouse->Size = System::Drawing::Size(150, 35);
            btnAddHouse->BackColor = Color::LightGreen;
            btnAddHouse->Click += gcnew EventHandler(this, &MainForm::OnAddHouseClick);

            // Кнопки удаления
            btnDeleteDevice = gcnew Button();
            btnDeleteDevice->Text = L"❌ Удалить устройство";
            btnDeleteDevice->Location = Point(15, 70);
            btnDeleteDevice->Size = System::Drawing::Size(150, 35);
            btnDeleteDevice->BackColor = Color::LightCoral;
            btnDeleteDevice->Click += gcnew EventHandler(this, &MainForm::OnDeleteDeviceClick);

            btnDeleteRoom = gcnew Button();
            btnDeleteRoom->Text = L"🏠 Удалить комнату";
            btnDeleteRoom->Location = Point(180, 70);
            btnDeleteRoom->Size = System::Drawing::Size(150, 35);
            btnDeleteRoom->BackColor = Color::LightCoral;
            btnDeleteRoom->Click += gcnew EventHandler(this, &MainForm::OnDeleteRoomClick);

            btnDeleteHouse = gcnew Button();
            btnDeleteHouse->Text = L"🏢 Удалить дом";
            btnDeleteHouse->Location = Point(345, 70);
            btnDeleteHouse->Size = System::Drawing::Size(150, 35);
            btnDeleteHouse->BackColor = Color::LightCoral;
            btnDeleteHouse->Click += gcnew EventHandler(this, &MainForm::OnDeleteHouseClick);

            gbManage->Controls->Add(btnAddDevice);
            gbManage->Controls->Add(btnAddRoom);
            gbManage->Controls->Add(btnAddHouse);
            gbManage->Controls->Add(btnDeleteDevice);
            gbManage->Controls->Add(btnDeleteRoom);
            gbManage->Controls->Add(btnDeleteHouse);

            // Кнопки управления командами
            btnExecute = gcnew Button();
            btnExecute->Text = L"▶ Выполнить команду";
            btnExecute->Location = Point(340, 460);
            btnExecute->Size = System::Drawing::Size(120, 40);
            btnExecute->BackColor = Color::LightGreen;
            btnExecute->Click += gcnew EventHandler(this, &MainForm::OnExecuteClick);

            btnUndo = gcnew Button();
            btnUndo->Text = L"↩ Отменить последнюю";
            btnUndo->Location = Point(470, 460);
            btnUndo->Size = System::Drawing::Size(120, 40);
            btnUndo->BackColor = Color::LightCoral;
            btnUndo->Click += gcnew EventHandler(this, &MainForm::OnUndoClick);

            btnTurnOnAll = gcnew Button();
            btnTurnOnAll->Text = L"🔆 Включить всё";
            btnTurnOnAll->Location = Point(340, 510);
            btnTurnOnAll->Size = System::Drawing::Size(120, 40);
            btnTurnOnAll->BackColor = Color::LightYellow;
            btnTurnOnAll->Click += gcnew EventHandler(this, &MainForm::OnTurnOnAllClick);

            btnTurnOffAll = gcnew Button();
            btnTurnOffAll->Text = L"💤 Выключить всё";
            btnTurnOffAll->Location = Point(470, 510);
            btnTurnOffAll->Size = System::Drawing::Size(120, 40);
            btnTurnOffAll->BackColor = Color::LightYellow;
            btnTurnOffAll->Click += gcnew EventHandler(this, &MainForm::OnTurnOffAllClick);

            btnShowStatus = gcnew Button();
            btnShowStatus->Text = L"📊 Показать статус";
            btnShowStatus->Location = Point(600, 460);
            btnShowStatus->Size = System::Drawing::Size(130, 90);
            btnShowStatus->BackColor = Color::LightBlue;
            btnShowStatus->Click += gcnew EventHandler(this, &MainForm::OnShowStatusClick);

            // Добавляем элементы на форму
            this->Controls->Add(lblTitle);
            this->Controls->Add(tvDevices);
            this->Controls->Add(lblCommands);
            this->Controls->Add(lbCommands);
            this->Controls->Add(gbManage);
            this->Controls->Add(btnExecute);
            this->Controls->Add(btnUndo);
            this->Controls->Add(btnTurnOnAll);
            this->Controls->Add(btnTurnOffAll);
            this->Controls->Add(btnShowStatus);
        }

        void InitializeSmartHome()
        {
            smartHome = gcnew SmartHomeData();
            smartHome->controller = new SimpleController();

            // Создаем дом
            smartHome->house = new House("Мой Умный Дом");

            // Создаем комнаты
            Room* livingRoom = new Room("Гостиная");
            Room* bedroom = new Room("Спальня");
            Room* kitchen = new Room("Кухня");

            // Добавляем устройства в гостиную
            Light* light1 = new Light("Люстра");
            Light* light2 = new Light("Торшер");
            AirConditioner* ac = new AirConditioner("Кондиционер");
            TV* tv = new TV("Телевизор");

            livingRoom->add(shared_ptr<Device>(light1));
            livingRoom->add(shared_ptr<Device>(light2));
            livingRoom->add(shared_ptr<Device>(ac));
            livingRoom->add(shared_ptr<Device>(tv));

            // Добавляем устройства в спальню
            Light* bedroomLight = new Light("Ночник");
            AirConditioner* bedroomAc = new AirConditioner("Сплит-система");

            bedroom->add(shared_ptr<Device>(bedroomLight));
            bedroom->add(shared_ptr<Device>(bedroomAc));

            // Добавляем устройства на кухню
            Light* kitchenLight = new Light("Кухонный свет");
            AirConditioner* kitchenAc = new AirConditioner("Кухонный кондиционер");

            kitchen->add(shared_ptr<Device>(kitchenLight));
            kitchen->add(shared_ptr<Device>(kitchenAc));

            // Добавляем комнаты в дом
            smartHome->house->add(shared_ptr<Device>(livingRoom));
            smartHome->house->add(shared_ptr<Device>(bedroom));
            smartHome->house->add(shared_ptr<Device>(kitchen));

            // Создаем доступные команды
            smartHome->availableCommands = new vector<pair<string, shared_ptr<Device>>>();
            CreateAvailableCommands();
            BuildDeviceCommandsMap();
        }

        void CreateAvailableCommands()
        {
            smartHome->availableCommands->clear();

            // Получаем все комнаты и устройства
            for (auto& room : smartHome->house->getRooms()) {
                Room* roomPtr = dynamic_cast<Room*>(room.get());
                if (roomPtr) {
                    // Команда для всей комнаты
                    smartHome->availableCommands->push_back(make_pair("Включить всё в " + room->getName(), room));
                    smartHome->availableCommands->push_back(make_pair("Выключить всё в " + room->getName(), room));

                    // Команды для каждого устройства
                    for (auto& device : roomPtr->getDevices()) {
                        smartHome->availableCommands->push_back(make_pair("Включить " + device->getName(), device));
                        smartHome->availableCommands->push_back(make_pair("Выключить " + device->getName(), device));
                    }
                }
            }
        }

        void BuildDeviceCommandsMap()
        {
            smartHome->deviceCommandsMap->clear();

            for (auto& cmd : *(smartHome->availableCommands)) {
                string cmdName = cmd.first;
                // Извлекаем имя устройства из команды (после пробела)
                size_t pos = cmdName.find(" ");
                if (pos != string::npos) {
                    string deviceName = cmdName.substr(pos + 1);
                    (*smartHome->deviceCommandsMap)[deviceName].push_back(cmd.second);
                }
            }
        }

        void UpdateDeviceTree()
        {
            tvDevices->Nodes->Clear();

            TreeNode^ root = gcnew TreeNode(gcnew String(smartHome->house->getName().c_str()));

            for (auto& room : smartHome->house->getRooms()) {
                TreeNode^ roomNode = gcnew TreeNode(gcnew String(room->getName().c_str()));

                Room* roomPtr = dynamic_cast<Room*>(room.get());
                if (roomPtr) {
                    for (auto& device : roomPtr->getDevices()) {
                        String^ deviceName = gcnew String(device->getName().c_str());
                        bool isOn = device->getIsOn();

                        // Создаем узел с цветным индикатором
                        String^ displayText = isOn ? (deviceName + L" 🟢") : (deviceName + L" 🔴");
                        TreeNode^ deviceNode = gcnew TreeNode(displayText);
                        deviceNode->Tag = deviceName;
                        deviceNode->ForeColor = isOn ? Color::Green : Color::Red;

                        roomNode->Nodes->Add(deviceNode);
                    }
                }

                root->Nodes->Add(roomNode);
            }

            tvDevices->Nodes->Add(root);
            root->Expand();
        }

        // Методы для добавления элементов
        void OnAddDeviceClick(Object^ sender, EventArgs^ e)
        {
            if (selectedNode == nullptr || selectedNode->Parent == nullptr) {
                MessageBox::Show(L"Пожалуйста, выберите комнату для добавления устройства!",
                    L"Предупреждение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            // Проверяем, что выбран узел комнаты
            if (selectedNode->Level != 1) {
                MessageBox::Show(L"Пожалуйста, выберите комнату, в которую хотите добавить устройство!",
                    L"Предупреждение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            String^ roomName = selectedNode->Text;
            String^ deviceName = ShowInputDialog(
                L"Введите название устройства:", L"Добавление устройства", L"Новое устройство");

            if (!String::IsNullOrEmpty(deviceName)) {
                // Определяем тип устройства
                String^ deviceType = ShowInputDialog(
                    L"Выберите тип устройства (Light/AirConditioner/TV):",
                    L"Тип устройства", L"Light");

                string roomNameStr = msclr::interop::marshal_as<string>(roomName);
                string deviceNameStr = msclr::interop::marshal_as<string>(deviceName);
                string deviceTypeStr = msclr::interop::marshal_as<string>(deviceType);

                // Находим комнату и добавляем устройство
                for (auto& room : smartHome->house->getRooms()) {
                    if (room->getName() == roomNameStr) {
                        Room* roomPtr = dynamic_cast<Room*>(room.get());
                        if (roomPtr) {
                            Device* newDevice = nullptr;
                            if (deviceTypeStr == "Light") {
                                newDevice = new Light(deviceNameStr);
                            }
                            else if (deviceTypeStr == "AirConditioner") {
                                newDevice = new AirConditioner(deviceNameStr);
                            }
                            else if (deviceTypeStr == "TV") {
                                newDevice = new TV(deviceNameStr);
                            }
                            else {
                                newDevice = new Light(deviceNameStr);
                            }

                            roomPtr->add(shared_ptr<Device>(newDevice));
                            break;
                        }
                    }
                }

                // Обновляем команды
                CreateAvailableCommands();
                BuildDeviceCommandsMap();
                UpdateDeviceTree();

                MessageBox::Show(L"Устройство успешно добавлено!", L"Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void OnAddRoomClick(Object^ sender, EventArgs^ e)
        {
            String^ roomName = ShowInputDialog(
                L"Введите название комнаты:", L"Добавление комнаты", L"Новая комната");

            if (!String::IsNullOrEmpty(roomName)) {
                string roomNameStr = msclr::interop::marshal_as<string>(roomName);
                Room* newRoom = new Room(roomNameStr);
                smartHome->house->add(shared_ptr<Device>(newRoom));

                // Обновляем команды
                CreateAvailableCommands();
                BuildDeviceCommandsMap();
                UpdateDeviceTree();

                MessageBox::Show(L"Комната успешно добавлена!", L"Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void OnAddHouseClick(Object^ sender, EventArgs^ e)
        {
            String^ houseName = ShowInputDialog(
                L"Введите название дома:", L"Добавление дома", L"Новый дом");

            if (!String::IsNullOrEmpty(houseName)) {
                System::Windows::Forms::DialogResult result = MessageBox::Show(
                    L"Создание нового дома удалит текущий дом и все его содержимое. Продолжить?",
                    L"Подтверждение", MessageBoxButtons::YesNo, MessageBoxIcon::Warning);

                if (result == System::Windows::Forms::DialogResult::Yes) {
                    string houseNameStr = msclr::interop::marshal_as<string>(houseName);
                    delete smartHome->house;
                    smartHome->house = new House(houseNameStr);

                    // Очищаем историю действий
                    actionHistory->clear();

                    // Обновляем команды
                    CreateAvailableCommands();
                    BuildDeviceCommandsMap();
                    UpdateDeviceTree();

                    MessageBox::Show(L"Новый дом успешно создан!", L"Успех",
                        MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
            }
        }

        // Методы для удаления элементов
        void OnDeleteDeviceClick(Object^ sender, EventArgs^ e)
        {
            if (selectedNode == nullptr || selectedNode->Level != 2) {
                MessageBox::Show(L"Пожалуйста, выберите устройство для удаления!",
                    L"Предупреждение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            System::Windows::Forms::DialogResult result = MessageBox::Show(
                L"Вы уверены, что хотите удалить это устройство?",
                L"Подтверждение", MessageBoxButtons::YesNo, MessageBoxIcon::Question);

            if (result == System::Windows::Forms::DialogResult::Yes) {
                String^ deviceName = safe_cast<String^>(selectedNode->Tag);
                String^ roomName = selectedNode->Parent->Text;

                string deviceNameStr = msclr::interop::marshal_as<string>(deviceName);
                string roomNameStr = msclr::interop::marshal_as<string>(roomName);

                // Находим комнату и удаляем устройство
                for (auto& room : smartHome->house->getRooms()) {
                    if (room->getName() == roomNameStr) {
                        Room* roomPtr = dynamic_cast<Room*>(room.get());
                        if (roomPtr) {
                            // Находим устройство по имени и удаляем
                            for (auto& device : roomPtr->getDevices()) {
                                if (device->getName() == deviceNameStr) {
                                    roomPtr->remove(device);
                                    break;
                                }
                            }
                            break;
                        }
                    }
                }

                // Обновляем команды
                CreateAvailableCommands();
                BuildDeviceCommandsMap();
                UpdateDeviceTree();

                MessageBox::Show(L"Устройство успешно удалено!", L"Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void OnDeleteRoomClick(Object^ sender, EventArgs^ e)
        {
            if (selectedNode == nullptr || selectedNode->Level != 1) {
                MessageBox::Show(L"Пожалуйста, выберите комнату для удаления!",
                    L"Предупреждение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            System::Windows::Forms::DialogResult result = MessageBox::Show(
                L"Вы уверены, что хотите удалить эту комнату и все устройства в ней?",
                L"Подтверждение", MessageBoxButtons::YesNo, MessageBoxIcon::Question);

            if (result == System::Windows::Forms::DialogResult::Yes) {
                String^ roomName = selectedNode->Text;
                string roomNameStr = msclr::interop::marshal_as<string>(roomName);

                // Находим комнату по имени и удаляем
                for (auto& room : smartHome->house->getRooms()) {
                    if (room->getName() == roomNameStr) {
                        smartHome->house->remove(room);
                        break;
                    }
                }

                // Обновляем команды
                CreateAvailableCommands();
                BuildDeviceCommandsMap();
                UpdateDeviceTree();

                MessageBox::Show(L"Комната успешно удалена!", L"Успех",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void OnDeleteHouseClick(Object^ sender, EventArgs^ e)
        {
            if (tvDevices->Nodes->Count == 0) {
                MessageBox::Show(L"Нет дома для удаления!", L"Предупреждение",
                    MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            System::Windows::Forms::DialogResult result = MessageBox::Show(
                L"Вы уверены, что хотите удалить текущий дом и все его содержимое?",
                L"Подтверждение", MessageBoxButtons::YesNo, MessageBoxIcon::Warning);

            if (result == System::Windows::Forms::DialogResult::Yes) {
                delete smartHome->house;
                smartHome->house = new House("Новый дом");

                // Очищаем историю действий
                actionHistory->clear();

                // Обновляем команды
                CreateAvailableCommands();
                BuildDeviceCommandsMap();
                UpdateDeviceTree();

                MessageBox::Show(L"Дом успешно удален! Создан новый пустой дом.",
                    L"Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void OnDeviceSelected(Object^ sender, TreeViewEventArgs^ e)
        {
            selectedNode = e->Node;
            lbCommands->Items->Clear();

            // Проверяем, что выбран не корневой узел и не комната
            if (selectedNode->Level == 2) {
                // Выбрано устройство - получаем имя устройства из Tag
                String^ deviceName = safe_cast<String^>(selectedNode->Tag);

                if (deviceName != nullptr) {
                    // Добавляем команды только один раз для каждого устройства
                    lbCommands->Items->Add(L"Включить " + deviceName);
                    lbCommands->Items->Add(L"Выключить " + deviceName);
                }
            }
            else if (selectedNode->Level == 1) {
                // Выбрана комната - показываем команды для комнаты
                lbCommands->Items->Add(L"Включить всё в " + selectedNode->Text);
                lbCommands->Items->Add(L"Выключить всё в " + selectedNode->Text);
            }
            else if (selectedNode->Level == 0) {
                // Выбран корень (дом) - показываем все команды
                for (auto& cmd : *(smartHome->availableCommands)) {
                    String^ cmdName = gcnew String(cmd.first.c_str());
                    // Проверяем, чтобы не добавлять дубликаты
                    if (lbCommands->Items->IndexOf(cmdName) == -1) {
                        lbCommands->Items->Add(cmdName);
                    }
                }
            }
        }

        void OnCommandSelected(Object^ sender, EventArgs^ e)
        {
            // Пустой обработчик
        }

        void OnExecuteClick(Object^ sender, EventArgs^ e)
        {
            if (lbCommands->SelectedIndex >= 0) {
                String^ selectedCmd = lbCommands->SelectedItem->ToString();
                string selectedCmdStr = msclr::interop::marshal_as<string>(selectedCmd);

                // Парсим команду и выполняем
                if (selectedCmdStr.find("Включить всё в ") != string::npos) {
                    string roomName = selectedCmdStr.substr(selectedCmdStr.find("Включить всё в ") + 15);
                    // Находим комнату и включаем
                    for (auto& room : smartHome->house->getRooms()) {
                        if (room->getName() == roomName) {
                            smartHome->controller->turnOnRoom(dynamic_cast<Room*>(room.get()));
                            actionHistory->push_back(make_pair("turnOnRoom", room));
                            break;
                        }
                    }
                }
                else if (selectedCmdStr.find("Выключить всё в ") != string::npos) {
                    string roomName = selectedCmdStr.substr(selectedCmdStr.find("Выключить всё в ") + 16);
                    // Находим комнату и выключаем
                    for (auto& room : smartHome->house->getRooms()) {
                        if (room->getName() == roomName) {
                            smartHome->controller->turnOffRoom(dynamic_cast<Room*>(room.get()));
                            actionHistory->push_back(make_pair("turnOffRoom", room));
                            break;
                        }
                    }
                }
                else if (selectedCmdStr.find("Включить ") != string::npos && selectedCmdStr.find("всё") == string::npos) {
                    string deviceName = selectedCmdStr.substr(selectedCmdStr.find("Включить ") + 9);
                    // Находим устройство и включаем
                    for (auto& room : smartHome->house->getRooms()) {
                        Room* roomPtr = dynamic_cast<Room*>(room.get());
                        if (roomPtr) {
                            for (auto& device : roomPtr->getDevices()) {
                                if (device->getName() == deviceName) {
                                    smartHome->controller->turnOn(device);
                                    actionHistory->push_back(make_pair("turnOn", device));
                                    break;
                                }
                            }
                        }
                    }
                }
                else if (selectedCmdStr.find("Выключить ") != string::npos && selectedCmdStr.find("всё") == string::npos) {
                    string deviceName = selectedCmdStr.substr(selectedCmdStr.find("Выключить ") + 10);
                    // Находим устройство и выключаем
                    for (auto& room : smartHome->house->getRooms()) {
                        Room* roomPtr = dynamic_cast<Room*>(room.get());
                        if (roomPtr) {
                            for (auto& device : roomPtr->getDevices()) {
                                if (device->getName() == deviceName) {
                                    smartHome->controller->turnOff(device);
                                    actionHistory->push_back(make_pair("turnOff", device));
                                    break;
                                }
                            }
                        }
                    }
                }

                UpdateDeviceTree();
            }
            else {
                MessageBox::Show(L"Пожалуйста, выберите команду из списка!",
                    L"Предупреждение", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            }
        }

        void OnUndoClick(Object^ sender, EventArgs^ e)
        {
            if (actionHistory->empty()) {
                MessageBox::Show(L"Нет действий для отмены!",
                    L"Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }

            auto lastAction = actionHistory->back();
            actionHistory->pop_back();

            if (lastAction.first == "turnOn") {
                smartHome->controller->turnOff(lastAction.second);
                MessageBox::Show(L"Отменено: выключение устройства",
                    L"Отмена", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            else if (lastAction.first == "turnOff") {
                smartHome->controller->turnOn(lastAction.second);
                MessageBox::Show(L"Отменено: включение устройства",
                    L"Отмена", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            else if (lastAction.first == "turnOnRoom") {
                smartHome->controller->turnOffRoom(dynamic_cast<Room*>(lastAction.second.get()));
                MessageBox::Show(L"Отменено: выключение всех устройств в комнате",
                    L"Отмена", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
            else if (lastAction.first == "turnOffRoom") {
                smartHome->controller->turnOnRoom(dynamic_cast<Room*>(lastAction.second.get()));
                MessageBox::Show(L"Отменено: включение всех устройств в комнате",
                    L"Отмена", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }

            UpdateDeviceTree();
        }

        void OnTurnOnAllClick(Object^ sender, EventArgs^ e)
        {
            smartHome->controller->turnOnHouse(smartHome->house);
            actionHistory->push_back(make_pair("turnOnHouse", shared_ptr<Device>(smartHome->house)));
            UpdateDeviceTree();
        }

        void OnTurnOffAllClick(Object^ sender, EventArgs^ e)
        {
            smartHome->controller->turnOffHouse(smartHome->house);
            actionHistory->push_back(make_pair("turnOffHouse", shared_ptr<Device>(smartHome->house)));
            UpdateDeviceTree();
        }

        void OnShowStatusClick(Object^ sender, EventArgs^ e)
        {
            ShowStatusMessage();
        }

        void ShowStatusMessage()
        {
            String^ status = L"\n=== ТЕКУЩЕЕ СОСТОЯНИЕ ===\n";

            for (auto& room : smartHome->house->getRooms()) {
                String^ roomStatus = gcnew String(room->getName().c_str());
                status += roomStatus + L":\n";

                Room* roomPtr = dynamic_cast<Room*>(room.get());
                if (roomPtr) {
                    for (auto& device : roomPtr->getDevices()) {
                        String^ deviceStatus = device->getIsOn() ? L"  🟢 ВКЛЮЧЕН" : L"  🔴 ВЫКЛЮЧЕН";
                        String^ deviceName = gcnew String(device->getName().c_str());
                        status += deviceStatus + L" - " + deviceName + L"\n";
                    }
                }
            }
            status += L"=====================\n";

            MessageBox::Show(status, L"Статус устройств", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }
    };
}