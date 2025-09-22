using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace ConfigParser.Services;

public class AppInformationService
{
    public string? ProductCopyright { get; set; }

    public string? ProductVersion { get; set; }

    public string? ProductName { get; set; }

    public AppInformationService()
    {
        Assembly currentAssembly = Assembly.GetEntryAssembly()!;
        if (currentAssembly == null)
            currentAssembly = Assembly.GetCallingAssembly();
        var targetValue = currentAssembly.GetCustomAttributes(typeof(AssemblyCopyrightAttribute), inherit: true);
        if (targetValue.Length != 0)
            ProductCopyright = ((AssemblyCopyrightAttribute)targetValue[0]).Copyright;
        System.Version versionNumber = currentAssembly.GetName().Version!;

        if(versionNumber.Build != 0)
        {
            ProductVersion = string.Format("Version {0}.{1}.{2}", versionNumber.Major, versionNumber.Minor, versionNumber.Build);
        }
        else
        {
            ProductVersion = string.Format("Version {0}.{1}", versionNumber.Major, versionNumber.Minor);
        }
       
        targetValue = currentAssembly.GetCustomAttributes(typeof(AssemblyProductAttribute), inherit: true);
        if (targetValue.Length != 0)
            ProductName = "ConfigTool";
    }
}
