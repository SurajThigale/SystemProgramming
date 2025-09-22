using global::System;
using global::System.Collections.Generic;
using global::System.Linq;
using global::System.Threading.Tasks;
using global::Microsoft.AspNetCore.Components;
using Microsoft.AspNetCore.Components.Web;
using Microsoft.AspNetCore.Components.Rendering;
using UIParser.Components;
using UIParser.Models;
using System.Reflection;
using UIParser.Services;
using ConfigParser.Services;

namespace UIParser;

public partial class Parser
{
    public GridMetaData? UIRoot { get; set; }

    [Inject]
    private ThemeService? themeService { get; set; }

    [Inject]
    private LoggerService? logger { get; set; }

    protected override async Task OnInitializedAsync()
    {
        try
        {
            var uiFile = new UIFileParser();

            var assemplyPath = Assembly.GetExecutingAssembly().Location;
            var uiFilesPath = Path.Combine(Path.GetDirectoryName(assemplyPath)!,
                 "Resources/UIFiles/");

            UIFileParser.uiRootPath = uiFilesPath;

            UIRoot = await uiFile.parseXML("main.tkxml");

            themeService?.setTheme(ParserHelper.uiTheme);
        }
        catch (Exception ex)
        {
            await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
        }
    }
}