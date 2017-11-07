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
        private Dictionary<int, ProcessModuleCollection> allModules;

        public void BindToRunningProcesses()
        {
            localAll = Process.GetProcesses();
            allModules = getAllModules();
        }

        public ProcessModuleCollection getAllProcessModulesById (int id)
        {
            ProcessModuleCollection pc = allModules[id];
            return pc;
        }

        private Dictionary<int, ProcessModuleCollection> getAllModules()
        {
            if (!(localAll == null)) {
                Dictionary<int, ProcessModuleCollection> d = new Dictionary<int, ProcessModuleCollection>();
                foreach (Process i in localAll)
                {
                    try
                    {
                        d.Add(i.Id, i.Modules);
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine(i.ProcessName + " " + e.Message);
                    }
                }
                return d;
            }
            return null;
        }
    }
}
