using Microsoft.VisualBasic.ApplicationServices;
using System.Drawing.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static AbstractFabric.Form1;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace AbstractFabric
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            LoadTestData();
            DataGridView dataGridView1;
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

        private IFactory _currentFactory;
        private ITaskTable _taskTable;
        private IActionPanel _actionPanel;
        private IStatPanel _statPanel;

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
        public List<User> GetExecutors()
        {
            return users.Where(u => u.Role == UserRole.Executor).ToList();
        }
        public interface IFactory
        {
            ITaskTable CreateTaskTable();
            IActionPanel CreateActionPanel();
            IStatPanel CreateStatPanel();
        }
        public class ManagerFactory : IFactory
        {
            private Form1 _form;

            public ManagerFactory(Form1 form)
            {
                _form = form;
            }
            
            public ITaskTable CreateTaskTable()
            {
                return new ManagerTaskTable(_form);
            }

            public IActionPanel CreateActionPanel()
            {
                return new ManagerActionPanel(_form);
            }

            public IStatPanel CreateStatPanel()
            {
                return new ManagerStatPanel(_form);
            }

        }
        public class ExecutorFactory : IFactory
        {
            private Form1 _form;

            public ExecutorFactory(Form1 form)
            {
                _form = form;
            }
            public ITaskTable CreateTaskTable()
            {
                return new ExecutorTaskTable(_form);
            }

            public IActionPanel CreateActionPanel()
            {
                return new ExecutorActionPanel(_form);
            }

            public IStatPanel CreateStatPanel()
            {
                return new ExecutorStatPanel(_form);
            }

        }
        public interface ITaskTable
        {
            void ConfigureColumns();
            void FillTasks();           
        }
        public interface IActionPanel
        {
            void CreateButtons();
        }
        public interface IStatPanel
        {
            void UpdateStatistics();
        }
        public class ManagerTaskTable : ITaskTable
        {
            private Form1 _form;
            public ManagerTaskTable(Form1 form)
            {
                _form = form;
            }
            public void ConfigureColumns()
            {
                var dataGridView1 = _form.dataGridView1;
                dataGridView1.Columns.Clear();
                dataGridView1.AllowUserToAddRows = true;
                dataGridView1.DataError += _form.dataGridView1_DataError;
                dataGridView1.CellParsing += _form.dataGridView1_CellParsing;
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
                    DataSource = _form.GetExecutors(),
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
            public void FillTasks()
            {
                var dataGridView1 = _form.dataGridView1; ;
                dataGridView1.Rows.Clear();

                foreach (var task in _form.tasks)
                {
                    var executor = _form.users.FirstOrDefault(u => u.Id == task.ExecutorId);
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
            }

        }
        
        public class ExecutorTaskTable : ITaskTable
        {
            private Form1 _form;

            public ExecutorTaskTable(Form1 form)
            {
                _form = form;
            }
            public void ConfigureColumns()
            {
                var dataGridView1 = _form.dataGridView1;
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
            public void FillTasks()
            {
                var dataGridView1 = _form.dataGridView1;
                dataGridView1.Rows.Clear();              
                var myTasks = _form.tasks.Where(t => t.ExecutorId == _form.currentUser?.Id).ToList();

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
            }
        }
        
        public class ManagerActionPanel : IActionPanel
        {
            private Form1 _form;
            public ManagerActionPanel(Form1 form)
            {
                _form = form;
            }
            public void CreateButtons()
            {
                _form.flowLayoutPanel1.Controls.Clear();

                System.Windows.Forms.Button btnNew = new System.Windows.Forms.Button
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

                System.Windows.Forms.Button btnAssign = new System.Windows.Forms.Button
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

                System.Windows.Forms.Button btnAccept = new System.Windows.Forms.Button
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

                System.Windows.Forms.Button btnReport = new System.Windows.Forms.Button
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

                _form.flowLayoutPanel1.Controls.AddRange(new Control[] { btnNew, btnAssign, btnAccept, btnReport });
            }
            private void AddNewTask() { MessageBox.Show("Добавление новой задачи"); }
            private void AssignExecutor() { MessageBox.Show("Назначение исполнителя"); }
            private void AcceptTask() { MessageBox.Show("Принятие задачи"); }
            private void ShowReport() { MessageBox.Show("Показ отчета"); }
        }

        public class ExecutorActionPanel : IActionPanel
        {
            private Form1 _form;

            public ExecutorActionPanel(Form1 form)
            {
                _form = form;
            }
            public void CreateButtons()
            {
                _form.flowLayoutPanel1.Controls.Clear();

                System.Windows.Forms.Button btnTake = new System.Windows.Forms.Button
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

                System.Windows.Forms.Button btnComplete = new System.Windows.Forms.Button
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

                System.Windows.Forms.Button btnClarify = new System.Windows.Forms.Button
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

                _form.flowLayoutPanel1.Controls.AddRange(new Control[] { btnTake, btnComplete, btnClarify });

                // Активация/деактивация кнопок при выборе строки
                _form.dataGridView1.SelectionChanged += (s, e) =>
                {
                    bool hasSelection = _form.dataGridView1.SelectedRows.Count > 0;
                    btnTake.Enabled = hasSelection;
                    btnComplete.Enabled = hasSelection;
                    btnClarify.Enabled = hasSelection;
                };
            }

            private void TakeTask() { MessageBox.Show("Взятие задачи в работу"); }
            private void CompleteTask() { MessageBox.Show("Завершение задачи"); }
            private void ClarifyTask() { MessageBox.Show("Уточнение задачи"); }
        }

        public class ManagerStatPanel : IStatPanel
        {
            private Form1 _form;
            public ManagerStatPanel(Form1 form)
            {
                _form = form;
            }
            public void UpdateStatistics()
            {
                _form.groupBox1.Text = "Статистика команды";
                if (_form.label2 != null)
                    _form.label2.Text = $"Всего задач: {_form.tasks.Count}";

                int completedToday = _form.tasks.Count(t =>
                    t.Status == TaskStatus.Completed &&
                    t.Deadline.Date == DateTime.Today);

                if (_form.label3 != null)
                    _form.label3.Text = $"Выполнено сегодня: {completedToday}";

                int overdue = _form.tasks.Count(t =>
                    t.Deadline < DateTime.Now &&
                    t.Status != TaskStatus.Completed);

                if (_form.label4 != null)
                    _form.label4.Text = $"Просрочено: {overdue}";
                UpdateWorkloadBars();
            }
            private void UpdateWorkloadBars()
            {
                ClearWorkloadBars();
                int yPos = 160;
                List<User> list = _form.GetExecutors();
                for (int i = 0; i < list.Count; i++)
                {
                    User? executor = list[i];
                    int taskCount = _form.tasks.Count(t => t.ExecutorId == executor.Id);
                    int totalTasks = _form.tasks.Count;
                    int percent = totalTasks > 0 ? (taskCount * 100) / totalTasks : 0;

                    Label nameLabel = new Label
                    {
                        Name = $"workloadName_{executor.Id}",
                        Text = executor.Name + ":",
                        Location = new Point(20, yPos),
                        AutoSize = true,
                        Font = new Font("Segoe UI", 8)
                    };

                    System.Windows.Forms.ProgressBar bar = new System.Windows.Forms.ProgressBar
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

                    _form.groupBox1.Controls.Add(nameLabel);
                    _form.groupBox1.Controls.Add(bar);
                    _form.groupBox1.Controls.Add(percentLabel);

                    yPos += 30;
                }
            }
            private void ClearWorkloadBars()
            {
                var controlsToRemove = _form.groupBox1.Controls
                    .OfType<Control>()
                    .Where(c => c.Name.StartsWith("workload"))
                    .ToList();

                foreach (var c in controlsToRemove)
                    _form.groupBox1.Controls.Remove(c);
            }
        }

        public class ExecutorStatPanel : IStatPanel
        {
            private Form1 _form;
            public ExecutorStatPanel(Form1 form)
            {
                _form = form;
            }
            public void UpdateStatistics()
            {
                var myTasks = _form.tasks.Where(t => t.ExecutorId == _form.currentUser?.Id).ToList();
                _form.groupBox1.Text = "Моя статистика";
                if (_form.label2 != null)
                    _form.label2.Text = $"Мои задачи: {myTasks.Count}";

                int completedToday = myTasks.Count(t =>
                    t.Status == TaskStatus.Completed &&
                    t.Deadline.Date == DateTime.Today);

                if (_form.label3 != null)
                    _form.label3.Text = $"Выполнено сегодня: {completedToday}";

                int overdue = myTasks.Count(t =>
                    t.Deadline < DateTime.Now &&
                    t.Status != TaskStatus.Completed);

                if (_form.label4 != null)
                    _form.label4.Text = $"Просрочено: {overdue}";
                ClearWorkloadBars();
            }
            private void ClearWorkloadBars()
            {
                var controlsToRemove = _form.groupBox1.Controls
                    .OfType<Control>()
                    .Where(c => c.Name.StartsWith("workload"))
                    .ToList();

                foreach (var c in controlsToRemove)
                    _form.groupBox1.Controls.Remove(c);
            }
        }


        private void RoleCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            isManagerMode = RoleCombo.SelectedIndex == 0;
            if (isManagerMode)
            {
                _currentFactory = new ManagerFactory(this);
            }
            else
            {
                _currentFactory = new ExecutorFactory(this);
            }
            _taskTable = _currentFactory.CreateTaskTable();
            _actionPanel = _currentFactory.CreateActionPanel();
            _statPanel = _currentFactory.CreateStatPanel();

            _taskTable.ConfigureColumns();
            _actionPanel.CreateButtons();
            _taskTable.FillTasks();
            _statPanel.UpdateStatistics();
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

        private void ExitBt_Click(object sender, EventArgs e)
        {
            Close();
        }
               
    }
}
