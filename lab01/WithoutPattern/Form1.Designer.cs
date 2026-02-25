namespace AbstractFabric
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            topPanel = new Panel();
            ExitBt = new Button();
            RoleCombo = new ComboBox();
            label1 = new Label();
            CenterPanel = new Panel();
            dataGridView1 = new DataGridView();
            StatPanel = new Panel();
            groupBox1 = new GroupBox();
            label4 = new Label();
            label3 = new Label();
            label2 = new Label();
            flowLayoutPanel1 = new FlowLayoutPanel();
            GoPanel = new Panel();
            topPanel.SuspendLayout();
            CenterPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)dataGridView1).BeginInit();
            StatPanel.SuspendLayout();
            groupBox1.SuspendLayout();
            GoPanel.SuspendLayout();
            SuspendLayout();
            // 
            // topPanel
            // 
            topPanel.Controls.Add(ExitBt);
            topPanel.Controls.Add(RoleCombo);
            topPanel.Controls.Add(label1);
            topPanel.Location = new Point(5, 5);
            topPanel.Name = "topPanel";
            topPanel.Size = new Size(1004, 55);
            topPanel.TabIndex = 0;
            // 
            // ExitBt
            // 
            ExitBt.Location = new Point(879, 12);
            ExitBt.Name = "ExitBt";
            ExitBt.Size = new Size(94, 29);
            ExitBt.TabIndex = 3;
            ExitBt.Text = "Выход";
            ExitBt.UseVisualStyleBackColor = true;
            ExitBt.Click += ExitBt_Click;
            // 
            // RoleCombo
            // 
            RoleCombo.DropDownStyle = ComboBoxStyle.DropDownList;
            RoleCombo.FormattingEnabled = true;
            RoleCombo.Items.AddRange(new object[] { "Руководитель", "Исполнитель" });
            RoleCombo.Location = new Point(105, 12);
            RoleCombo.Name = "RoleCombo";
            RoleCombo.Size = new Size(151, 28);
            RoleCombo.TabIndex = 1;
            RoleCombo.SelectedIndexChanged += RoleCombo_SelectedIndexChanged;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new Point(18, 15);
            label1.Name = "label1";
            label1.Size = new Size(56, 20);
            label1.TabIndex = 0;
            label1.Text = "Режим";
            // 
            // CenterPanel
            // 
            CenterPanel.Controls.Add(dataGridView1);
            CenterPanel.Location = new Point(5, 86);
            CenterPanel.Name = "CenterPanel";
            CenterPanel.Size = new Size(596, 227);
            CenterPanel.TabIndex = 2;
            // 
            // dataGridView1
            // 
            dataGridView1.AllowUserToAddRows = false;
            dataGridView1.AllowUserToDeleteRows = false;
            dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            dataGridView1.ColumnHeadersHeight = 29;
            dataGridView1.Location = new Point(0, 0);
            dataGridView1.Name = "dataGridView1";
            dataGridView1.RowHeadersVisible = false;
            dataGridView1.RowHeadersWidth = 51;
            dataGridView1.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
            dataGridView1.Size = new Size(593, 218);
            dataGridView1.TabIndex = 0;
            dataGridView1.CellParsing += dataGridView1_CellParsing;
            dataGridView1.DataError += dataGridView1_DataError;
            // 
            // StatPanel
            // 
            StatPanel.Controls.Add(groupBox1);
            StatPanel.Location = new Point(607, 86);
            StatPanel.Name = "StatPanel";
            StatPanel.Size = new Size(402, 423);
            StatPanel.TabIndex = 3;
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(label4);
            groupBox1.Controls.Add(label3);
            groupBox1.Controls.Add(label2);
            groupBox1.Location = new Point(3, 3);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new Size(391, 405);
            groupBox1.TabIndex = 0;
            groupBox1.TabStop = false;
            groupBox1.Text = "Статистика";
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(6, 98);
            label4.Name = "label4";
            label4.Size = new Size(0, 20);
            label4.TabIndex = 2;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new Point(6, 68);
            label3.Name = "label3";
            label3.Size = new Size(0, 20);
            label3.TabIndex = 1;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new Point(6, 37);
            label2.Name = "label2";
            label2.Size = new Size(0, 20);
            label2.TabIndex = 0;
            // 
            // flowLayoutPanel1
            // 
            flowLayoutPanel1.Location = new Point(9, 51);
            flowLayoutPanel1.Name = "flowLayoutPanel1";
            flowLayoutPanel1.Size = new Size(277, 159);
            flowLayoutPanel1.TabIndex = 0;
            // 
            // GoPanel
            // 
            GoPanel.Controls.Add(flowLayoutPanel1);
            GoPanel.Location = new Point(5, 331);
            GoPanel.Name = "GoPanel";
            GoPanel.Size = new Size(402, 163);
            GoPanel.TabIndex = 4;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(8F, 20F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1013, 506);
            Controls.Add(GoPanel);
            Controls.Add(StatPanel);
            Controls.Add(CenterPanel);
            Controls.Add(topPanel);
            Name = "Form1";
            Text = "Form1";
            topPanel.ResumeLayout(false);
            topPanel.PerformLayout();
            CenterPanel.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)dataGridView1).EndInit();
            StatPanel.ResumeLayout(false);
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            GoPanel.ResumeLayout(false);
            ResumeLayout(false);
        }

        #endregion

        private Panel topPanel;
        private Panel CenterPanel;
        private Panel StatPanel;
        private ComboBox RoleCombo;
        private Label label1;
        private Button ExitBt;
        private DataGridView dataGridView1;
        private GroupBox groupBox1;
        private Label label4;
        private Label label3;
        private Label label2;
        private FlowLayoutPanel flowLayoutPanel1;
        private Panel GoPanel;
    }
}
