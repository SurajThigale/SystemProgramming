using Microsoft.AspNetCore.Components;
using MudBlazor;
using System.Web;

namespace UIParser.Services;



public class ThemeService
{
    public ThemeService(RefreshService refreshService_)
    {
        refreshService = refreshService_;
    }

    bool firstCheck { get; set; } = false;

    public bool? bIsDarkTheme { get; set; }

    private RefreshService refreshService;

    private bool? SystemThemePreference = null;

    private readonly SemaphoreSlim themeProviderSemaphore = new SemaphoreSlim(1, 1);

    public void InjectSystemTheme(bool? systemTheme)
    {
        SystemThemePreference = systemTheme;
    }

    public async Task setTheme(string theme)
    {
        if (theme == "Dark")
        {
            bIsDarkTheme = true;
        }
        else if (theme == "Light")
        {
            bIsDarkTheme = false;
        }
        else if(theme == "System")
        {
            // As we are reading theme before the MainLayout set the theme,
            // we need to wait till MainLayout finishes loading
            await WaitForSystemThemePreference();

            // For System Theme, we will assign this to null

            if (SystemThemePreference is null)
            {
                bIsDarkTheme = false;
                return;
            }
            else 
                bIsDarkTheme = SystemThemePreference;

            // This should be checked at once per launch
            if (firstCheck) return;

            firstCheck = true;
        }
           

        refreshService.CallRequestRefresh();
    }

    private async Task WaitForSystemThemePreference()
    {
        while (SystemThemePreference is null)
        {
            // Wait for a short duration before checking again
            await Task.Delay(100); // Adjust the delay as needed
        }
    }
}
