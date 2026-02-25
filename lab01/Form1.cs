using System.Windows.Forms;

namespace AbstractFabric
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            LoadTestData();
        }
        public enum UserRole
        {
            Manager,
            Executor
        }
        public class User
        {
            public int Id { get; set; }
            public string Name { get; set; }
            public UserRole Role { get; set; }
            public override string ToString() => Name;
        }
        public enum TaskStatus
        {
            New,
            InProgress,
            OnReview,
            Completed
        }
        public enum Priority
        {
            High,
            Medium,
            Low
        }
        public class Task
        {
            public int Id { get; set; }
            public string Title { get; set; }
            public string Description { get; set; }
            public DateTime Deadline { get; set; }
            public TaskStatus Status { get; set; }
            public Priority Priority { get; set; }
            public int ExecutorId { get; set; }
            public int PercentComplete { get; set; }
            public string Comment { get; set; }
        }


        private List<Task> tasks;
        private List<User> users;
        private User currentUser;

        private bool isManagerMode = true;

        private void LoadTestData()
        {
            users = new List<User>
            {
                new User { Id = 1, Name = "Иван Петров", Role = UserRole.Manager },
                new User { Id = 2, Name = "Мария Иванова", Role = UserRole.Executor },
                new User { Id = 3, Name = "Петр Сидоров", Role = UserRole.Executor },
                new User { Id = 4, Name = "Елена Смирнова", Role = UserRole.Executor }
            };

            tasks = new List<Task>
            {
                new Task
                {
                    Id = 1,
                    Title = "Разработать прототип интерфейса",
                    Description = "Создать макет главного экрана",
                    Deadline = DateTime.Now.AddDays(5),
                    Status = TaskStatus.InProgress,
                    Priority = Priority.High,
                    ExecutorId = 2,
                    PercentComplete = 30,
                    Comment = "Использовать Figma"
                },
                new Task
                {
                    Id = 2,
                    Title = "Написать отчет за месяц",
                    Description = "Собрать данные по продажам",
                    Deadline = DateTime.Now.AddDays(2),
                    Status = TaskStatus.New,
                    Priority = Priority.Medium,
                    ExecutorId = 3,
                    PercentComplete = 0,
                    Comment = "Формат Excel"
                },
                new Task
                {
                    Id = 3,
                    Title = "Исправить критические баги",
                    Description = "Ошибки в модуле авторизации",
                    Deadline = DateTime.Now.AddDays(1),
                    Status = TaskStatus.OnReview,
                    Priority = Priority.High,
                    ExecutorId = 2,
                    PercentComplete = 80,
                    Comment = "Проверить после исправления"
                },
                new Task
                {
                    Id = 4,
                    Title = "Создать базу данных",
                    Description = "Спроектировать схему БД",
                    Deadline = DateTime.Now.AddDays(7),
                    Status = TaskStatus.New,
                    Priority = Priority.Low,
                    ExecutorId = 4,
                    PercentComplete = 0,
                    Comment = "Использовать PostgreSQL"
                },
                new Task
                {
                    Id = 5,
                    Title = "Обновить документацию",
                    Description = "API документация",
                    Deadline = DateTime.Now.AddDays(-1), // Просрочено
                    Status = TaskStatus.New,
                    Priority = Priority.Medium,
                    ExecutorId = 3,
                    PercentComplete = 10,
                    Comment = "Срочно!"
                }
            };
            currentUser = users.FirstOrDefault(u => u.Role == UserRole.Executor);
        }
        private List<User> GetExecutors()
        {
            return users.Where(u => u.Role == UserRole.Executor).ToList();
        }
        private void dataGridView1_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            e.Cancel = true;
            e.ThrowException = false;
        }

        private void dataGridView1_CellParsing(object sender, DataGridViewCellParsingEventArgs e)
        {
            if (dataGridView1.Columns[e.ColumnIndex].Name == "Executor")
            {
                if (e.Value is User user)
                {
                    e.Value = user.Id;
                    e.ParsingApplied = true;
                }
            }
            else if (dataGridView1.Columns[e.ColumnIndex].Name == "Status")
            {
                if (e.Value is string statusStr && Enum.TryParse<TaskStatus>(statusStr, out var status))
                {
                    e.Value = status;
                    e.ParsingApplied = true;
                }
            }
            else if (dataGridView1.Columns[e.ColumnIndex].Name == "Priority")
            {
                if (e.Value is string priorityStr && Enum.TryParse<Priority>(priorityStr, out var priority))
                {
                    e.Value = priority;
                    e.ParsingApplied = true;
                }
            }
        }
        private void ConfigureManagerColumns()
        {
            dataGridView1.Columns.Clear();
            dataGridView1.AllowUserToAddRows = true;
            dataGridView1.DataError += dataGridView1_DataError;
            dataGridView1.CellParsing += dataGridView1_CellParsing;
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                Name = "Id",
                HeaderText = "ID",
                Width = 40,
                ReadOnly = true,
                Visible = false
            });
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                Name = "Title",
                HeaderText = "Задача",
                Width = 250,
                AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill
            });
            DataGridViewComboBoxColumn executorCol = new DataGridViewComboBoxColumn
            {
                Name = "Executor",
                HeaderText = "Исполнитель",
                Width = 150,
                DataSource = GetExecutors(),
                DisplayMember = "Name",
                ValueMember = "Id",
                FlatStyle = FlatStyle.Flat
            };
            executorCol.DefaultCellStyle.NullValue = "";
            dataGridView1.Columns.Add(executorCol);

            // Срок
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                Name = "Deadline",
                HeaderText = "Срок",
                Width = 100
            });

            // Статус
            DataGridViewComboBoxColumn statusCol = new DataGridViewComboBoxColumn
            {
                Name = "Status",
                HeaderText = "Статус",
                Width = 120,
                DataSource = Enum.GetValues(typeof(TaskStatus)),
                FlatStyle = FlatStyle.Flat
            };
            statusCol.DefaultCellStyle.NullValue = TaskStatus.New;
            dataGridView1.Columns.Add(statusCol);

            // Приоритет
            DataGridViewComboBoxColumn priorityCol = new DataGridViewComboBoxColumn
            {
                Name = "Priority",
                HeaderText = "Приоритет",
                Width = 100,
                DataSource = Enum.GetValues(typeof(Priority)),
                FlatStyle = FlatStyle.Flat
            };
            priorityCol.DefaultCellStyle.NullValue = Priority.Medium;
            dataGridView1.Columns.Add(priorityCol);

            // Выполнение
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                Name = "Progress",
                HeaderText = "Выполнение",
                Width = 80
            });

            // Контроль
            dataGridView1.Columns.Add(new DataGridViewCheckBoxColumn
            {
                Name = "Control",
                HeaderText = "Контроль",
                Width = 60
            });
        }
        private void ConfigureExecutorColumns()
        {
            dataGridView1.Columns.Clear();
            dataGridView1.ReadOnly = true;
            dataGridView1.AllowUserToAddRows = false;
            dataGridView1.MultiSelect = false;

            // Задача
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                Name = "Title",
                HeaderText = "Задача",
                Width = 300,
                AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill,
                ReadOnly = true
            });

            // Срок
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                Name = "Deadline",
                HeaderText = "Срок",
                Width = 100,
                ReadOnly = true
            });

            // Статус
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                Name = "Status",
                HeaderText = "Статус",
                Width = 120,
                ReadOnly = true
            });

            // Приоритет
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                Name = "Priority",
                HeaderText = "Приоритет",
                Width = 100,
                ReadOnly = true
            });

            // Комментарий
            dataGridView1.Columns.Add(new DataGridViewTextBoxColumn
            {
                Name = "Comment",
                HeaderText = "Комментарий",
                Width = 200,
                ReadOnly = true
            });
        }
        private void CreateManagerButtons()
        {
            flowLayoutPanel1.Controls.Clear();

            Button btnNew = new Button
            {
                Text = "Новая задача",
                Size = new Size(130, 40),
                BackColor = Color.FromArgb(39, 174, 96),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat,
                Margin = new Padding(5)
            };
            btnNew.FlatAppearance.BorderSize = 0;
            btnNew.Click += (s, e) => AddNewTask();

            Button btnAssign = new Button
            {
                Text = "Назначить",
                Size = new Size(120, 40),
                BackColor = Color.FromArgb(241, 196, 15),
                ForeColor = Color.Black,
                FlatStyle = FlatStyle.Flat,
                Margin = new Padding(5)
            };
            btnAssign.FlatAppearance.BorderSize = 0;
            btnAssign.Click += (s, e) => AssignExecutor();

            Button btnAccept = new Button
            {
                Text = "Принять",
                Size = new Size(110, 40),
                BackColor = Color.FromArgb(52, 152, 219),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat,
                Margin = new Padding(5)
            };
            btnAccept.FlatAppearance.BorderSize = 0;
            btnAccept.Click += (s, e) => AcceptTask();

            Button btnReport = new Button
            {
                Text = "Отчет",
                Size = new Size(100, 40),
                BackColor = Color.FromArgb(155, 89, 182),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat,
                Margin = new Padding(5)
            };
            btnReport.FlatAppearance.BorderSize = 0;
            btnReport.Click += (s, e) => ShowReport();

            flowLayoutPanel1.Controls.AddRange(new Control[] { btnNew, btnAssign, btnAccept, btnReport });
        }
        private void CreateExecutorButtons()
        {
            flowLayoutPanel1.Controls.Clear();

            Button btnTake = new Button
            {
                Text = "Взять в работу",
                Size = new Size(140, 40),
                BackColor = Color.FromArgb(52, 152, 219),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat,
                Margin = new Padding(5)
            };
            btnTake.FlatAppearance.BorderSize = 0;
            btnTake.Click += (s, e) => TakeTask();

            Button btnComplete = new Button
            {
                Text = "Выполнено",
                Size = new Size(120, 40),
                BackColor = Color.FromArgb(39, 174, 96),
                ForeColor = Color.White,
                FlatStyle = FlatStyle.Flat,
                Margin = new Padding(5)
            };
            btnComplete.FlatAppearance.BorderSize = 0;
            btnComplete.Click += (s, e) => CompleteTask();

            Button btnClarify = new Button
            {
                Text = "Уточнить",
                Size = new Size(110, 40),
                BackColor = Color.FromArgb(241, 196, 15),
                ForeColor = Color.Black,
                FlatStyle = FlatStyle.Flat,
                Margin = new Padding(5)
            };
            btnClarify.FlatAppearance.BorderSize = 0;
            btnClarify.Click += (s, e) => ClarifyTask();

            flowLayoutPanel1.Controls.AddRange(new Control[] { btnTake, btnComplete, btnClarify });

            // Активация/деактивация кнопок при выборе строки
            dataGridView1.SelectionChanged += (s, e) =>
            {
                bool hasSelection = dataGridView1.SelectedRows.Count > 0;
                btnTake.Enabled = hasSelection;
                btnComplete.Enabled = hasSelection;
                btnClarify.Enabled = hasSelection;
            };
        }
        private void FillTasks()
        {
            dataGridView1.Rows.Clear();

            if (isManagerMode)
            {
                foreach (var task in tasks)
                {
                    var executor = users.FirstOrDefault(u => u.Id == task.ExecutorId);
                    int rowIndex = dataGridView1.Rows.Add(
                        task.Id,
                        task.Title,
                        task.ExecutorId,
                        task.Deadline.ToShortDateString(),
                        task.Status.ToString(),
                        task.Priority.ToString(),
                        task.PercentComplete,
                        false
                    );
                    if (executor != null)
                    {
                        dataGridView1.Rows[rowIndex].Cells["Executor"].Value = task.ExecutorId;
                    }
                }
                UpdateManagerStatistics();
                UpdateWorkloadBars();
            }
            else
            {
                var myTasks = tasks.Where(t => t.ExecutorId == currentUser?.Id).ToList();
                foreach (var task in myTasks)
                {
                    dataGridView1.Rows.Add(
                        task.Title,
                        task.Deadline.ToShortDateString(),
                        task.Status.ToString(),
                        task.Priority.ToString(),
                        task.Comment
                    );
                }
                UpdateExecutorStatistics(myTasks);
            }
        }
        private void UpdateManagerStatistics()
        {
            if (label2 != null)
                label2.Text = $"Всего задач: {tasks.Count}";

            int completedToday = tasks.Count(t =>
                t.Status == TaskStatus.Completed &&
                t.Deadline.Date == DateTime.Today);

            if (label3 != null)
                label3.Text = $"Выполнено сегодня: {completedToday}";

            int overdue = tasks.Count(t =>
                t.Deadline < DateTime.Now &&
                t.Status != TaskStatus.Completed);

            if (label4 != null)
                label4.Text = $"Просрочено: {overdue}";
        }

        private void UpdateExecutorStatistics(List<Task> myTasks)
        {
            if (label2 != null)
                label2.Text = $"Мои задачи: {myTasks.Count}";

            int completedToday = myTasks.Count(t =>
                t.Status == TaskStatus.Completed &&
                t.Deadline.Date == DateTime.Today);

            if (label3 != null)
                label3.Text = $"Выполнено сегодня: {completedToday}";

            int overdue = myTasks.Count(t =>
                t.Deadline < DateTime.Now &&
                t.Status != TaskStatus.Completed);

            if (label4 != null)
                label4.Text = $"Просрочено: {overdue}";
        }

        private void UpdateWorkloadBars()
        {
            if (!isManagerMode)
                return;
            ClearWorkloadBars();

            if (isManagerMode)
            {
                int yPos = 160;
                foreach (var executor in GetExecutors())
                {
                    int taskCount = tasks.Count(t => t.ExecutorId == executor.Id);
                    int totalTasks = tasks.Count;
                    int percent = totalTasks > 0 ? (taskCount * 100) / totalTasks : 0;

                    Label nameLabel = new Label
                    {
                        Name = $"workloadName_{executor.Id}",
                        Text = executor.Name + ":",
                        Location = new Point(20, yPos),
                        AutoSize = true,
                        Font = new Font("Segoe UI", 8)
                    };

                    ProgressBar bar = new ProgressBar
                    {
                        Name = $"workloadBar_{executor.Id}",
                        Location = new Point(150, yPos - 2),
                        Size = new Size(100, 18),
                        Value = percent,
                        Minimum = 0,
                        Maximum = 100
                    };

                    Label percentLabel = new Label
                    {
                        Name = $"workloadPercent_{executor.Id}",
                        Text = $"{percent}% ({taskCount} задач)",
                        Location = new Point(260, yPos),
                        AutoSize = true,
                        Font = new Font("Segoe UI", 7)
                    };

                    groupBox1.Controls.Add(nameLabel);
                    groupBox1.Controls.Add(bar);
                    groupBox1.Controls.Add(percentLabel);

                    yPos += 30;
                }
            }
        }
        private void ClearWorkloadBars()
        {
            var controlsToRemove = groupBox1.Controls
                .OfType<Control>()
                .Where(c => c.Name.StartsWith("workload"))
                .ToList();

            foreach (var c in controlsToRemove)
                groupBox1.Controls.Remove(c);
        }
        private void RoleCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            isManagerMode = RoleCombo.SelectedIndex == 0;
            ClearWorkloadBars();
            if (isManagerMode)
            {
                ConfigureManagerColumns();
                CreateManagerButtons();
                groupBox1.Text = "Статистика команды";
            }
            else
            {
                ConfigureExecutorColumns();
                CreateExecutorButtons();
                groupBox1.Text = "Моя статистика";
            }

            FillTasks();
        }
        private void ExitBt_Click(object sender, EventArgs e)
        {
            Close();
        }
        private void AddNewTask() { MessageBox.Show("Добавление новой задачи"); }
        private void AssignExecutor() { MessageBox.Show("Назначение исполнителя"); }
        private void AcceptTask() { MessageBox.Show("Принятие задачи"); }
        private void ShowReport() { MessageBox.Show("Показ отчета"); }
        private void TakeTask() { MessageBox.Show("Взятие задачи в работу"); }
        private void CompleteTask() { MessageBox.Show("Завершение задачи"); }
        private void ClarifyTask() { MessageBox.Show("Уточнение задачи"); }

       
    }
}
