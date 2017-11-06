using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab_5_var_1
{
    class MyProcess
    {
        public Process[] localAll;
        public void BindToRunningProcesses()
        {
            // Get all processes running on the local computer.
            localAll = Process.GetProcesses();
        }

        public ProcessModuleCollection getModulesInfo(Process process)
        {
            return process.Modules;
        }
    }
}
