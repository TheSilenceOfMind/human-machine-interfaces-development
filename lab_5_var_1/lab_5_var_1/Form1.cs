using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace lab_5_var_1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        MyProcess myProcess;
        int counter;
        private void button1_Click(object sender, EventArgs e)
        {
            button1.Enabled = false;
            myProcess = new MyProcess();
            myProcess.BindToRunningProcesses();
            
            RowStyle temp = tableLayoutPanel1.RowStyles[tableLayoutPanel1.RowCount - 1];
            counter = 0;
            foreach (Process i in myProcess.localAll)
            {
                if (counter > 10)
                    break;
                tableLayoutPanel1.RowCount = tableLayoutPanel1.RowCount + 1;
                Label l1 = new Label();
                l1.Text = i.ProcessName;
                l1.AutoSize = true;
                tableLayoutPanel1.Controls.Add(l1, 0, tableLayoutPanel1.RowCount - 1);
                Console.WriteLine(i.ProcessName);

                l1 = new Label();
                l1.Text = i.Id.ToString();
                l1.AutoSize = true;
                tableLayoutPanel1.Controls.Add(l1, 1, tableLayoutPanel1.RowCount - 1);
                Console.WriteLine(i.Id.ToString());

                Button b = new Button();
                b.Text = "x";
                b.Click += new EventHandler(openModulesInfo);
                tableLayoutPanel1.Controls.Add(b, 2, tableLayoutPanel1.RowCount - 1);
                counter++;
            }
        }

        private void openModulesInfo(object sender, EventArgs e)
        {
            Control b = (Control)sender;
            int row = this.tableLayoutPanel1.GetRow(b);
            Console.WriteLine(row);

            ProcessModuleCollection modules = myProcess.getAllModulesByIndexInString(row);
            Form2 f = new Form2();
            f.Text = this.tableLayoutPanel1.GetControlFromPosition(0, row).Text; // set title of window

            foreach (ProcessModule i in modules)
            {
                f.table.RowCount++;
                Label label = new Label();
                label.Text = i.ModuleName;
                label.AutoSize = true;
                f.table.Controls.Add(label, 0, f.table.RowCount - 1);

                label = new Label();
                label.Text = i.FileName;
                label.AutoSize = true;
                f.table.Controls.Add(label, 1, f.table.RowCount - 1);

                label = new Label();
                label.Text = i.BaseAddress.ToString();
                label.AutoSize = true;
                f.table.Controls.Add(label, 2, f.table.RowCount - 1);
            }
            f.Show();
            //MessageBox.Show(strModules, this.tableLayoutPanel1.GetControlFromPosition(0, row).Text + " (" + this.tableLayoutPanel1.GetControlFromPosition(1, row).Text + ")");
        }
    }
}
