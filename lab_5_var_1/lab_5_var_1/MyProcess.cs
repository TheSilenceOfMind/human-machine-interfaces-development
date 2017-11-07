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
        private List<ProcessModuleCollection> allModules;

        public void BindToRunningProcesses()
        {
            // Get all processes running on the local computer.
            localAll = Process.GetProcesses();
            allModules = getAllModules();
        }

        public ProcessModuleCollection getAllModulesByIndexInString (int ind)
        {
            StringBuilder str = new StringBuilder();
            ProcessModuleCollection pc = allModules.ElementAt(ind);
            //foreach(ProcessModule pm in pc)
            //{
            //    String name = pm.ModuleName;
            //    String file = pm.FileName;
            //    String ba = pm.BaseAddress.ToString();
            //}
            return pc;
        }

        private List<ProcessModuleCollection> getAllModules()
        {
            if (!(localAll == null)) {
                List<ProcessModuleCollection> m = new List<ProcessModuleCollection>();
                foreach (Process i in localAll)
                {
                    try
                    {
                        m.Add(i.Modules);
                    }
                    catch (Exception e)
                    {
                        Console.WriteLine(i.ProcessName + " " + e.Message);
                    }
                }
                return m;
            }
            return null;
        }
    }
}
