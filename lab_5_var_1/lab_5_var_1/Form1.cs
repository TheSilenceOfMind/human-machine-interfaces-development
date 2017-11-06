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
        
        private void button1_Click(object sender, EventArgs e)
        {
            MyProcess myProcess = new MyProcess();
            myProcess.BindToRunningProcesses();
            
            Process j = myProcess.localAll.ElementAt(0);
            RowStyle temp = tableLayoutPanel1.RowStyles[tableLayoutPanel1.RowCount - 1];
            foreach (Process i in myProcess.localAll)
            {
                tableLayoutPanel1.RowCount = tableLayoutPanel1.RowCount + 1;
                tableLayoutPanel1.RowStyles.Add(new RowStyle(temp.SizeType, temp.Height));
                tableLayoutPanel1.Controls.Add(new Label() { Text = i.ProcessName }, 0, tableLayoutPanel1.RowCount - 1);
                tableLayoutPanel1.Controls.Add(new Label() { Text = i.Id.ToString() }, 1, tableLayoutPanel1.RowCount - 1);
            }
        }
    }
}
