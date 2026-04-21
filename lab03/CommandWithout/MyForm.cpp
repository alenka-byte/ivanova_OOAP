#include "MyForm.h"
#include <windows.h>

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main()
{
    // Устанавливаем русскую локаль для консоли (если нужно)
    setlocale(LC_ALL, "Russian");

    // Включаем визуальные стили
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Создаем и запускаем форму
    SmartHomeUI::MainForm^ form = gcnew SmartHomeUI::MainForm();
    Application::Run(form);

    return 0;
}