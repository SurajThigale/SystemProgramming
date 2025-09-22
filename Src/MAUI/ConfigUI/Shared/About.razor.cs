using Microsoft.AspNetCore.Components;
using Microsoft.JSInterop;
using MudBlazor;
using System.Reflection;

namespace ConfigParser.Shared
{
    public partial class About
    {
        [CascadingParameter] MudDialogInstance? MudDialog { get; set; }

        private string? ProductCopyright { get; set; }

        private string? ProductVersion { get; set; }

        private string? ProductName { get; set; }

        protected override void OnInitialized()
        {
            Assembly currentAssembly = Assembly.GetEntryAssembly()!;
            if (currentAssembly == null)
                currentAssembly = Assembly.GetCallingAssembly();

            var targetValue = currentAssembly.GetCustomAttributes(
                            typeof(AssemblyCopyrightAttribute), inherit: true);
            if (targetValue.Length != 0)
                ProductCopyright = ((AssemblyCopyrightAttribute)targetValue[0]).Copyright;

            System.Version versionNumber = currentAssembly!.GetName()?.Version!;

            if(versionNumber.Build != 0)
            {
                ProductVersion = string.Format(
                      "Version {0}.{1}.{2}",
                      versionNumber!.Major, versionNumber.Minor, versionNumber.Build);
            }
            else
            {
                ProductVersion = string.Format(
                      "Version {0}.{1}",
                      versionNumber!.Major, versionNumber.Minor);
            }


            targetValue = currentAssembly.GetCustomAttributes(
                            typeof(AssemblyProductAttribute), inherit: true);
            if (targetValue.Length != 0)
                ProductName = ((AssemblyProductAttribute)targetValue[0]).Product;
        }
    }
}
