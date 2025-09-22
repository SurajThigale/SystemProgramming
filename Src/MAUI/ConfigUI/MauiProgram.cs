using CommunityToolkit.Maui;
using CommunityToolkit.Maui.Storage;
using CommunityToolkit.Mvvm.Messaging;
using ConfigParser.Services;
using Microsoft.Extensions.Logging;
using Microsoft.Maui.LifecycleEvents;
using MudBlazor.Services;
using Serilog;
using UIParser;

namespace ConfigParser
{
    public static class MauiProgram
    {
        public static MauiApp CreateMauiApp()
        {
            var builder = MauiApp.CreateBuilder();

            builder.Services.AddSerilog();

            Log.Logger = new LoggerConfiguration()
             .WriteTo.File("Logger.txt")
             .CreateLogger();

            builder
                .UseMauiApp<App>()
                 .UseMauiCommunityToolkit()
                .ConfigureFonts(fonts =>
                {
                    fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
                });

            builder.ConfigureLifecycleEvents(events =>
            {
#if WINDOWS
                events.AddWindows(windowsLifecycleBuilder =>
                {
                    windowsLifecycleBuilder.OnWindowCreated(window =>
                    {
                        //use Microsoft.UI.Windowing functions for window
                        var handle = WinRT.Interop.WindowNative.GetWindowHandle(window);
                        var id = Microsoft.UI.Win32Interop.GetWindowIdFromWindow(handle);
                        var appWindow = Microsoft.UI.Windowing.AppWindow.GetFromWindowId(id);


                        //When user execute the closing method, we can push a display alert. If user click Yes, close this application, if click the cancel, display alert will dismiss.
                        appWindow.Closing += (s, e) =>
                        {
                            e.Cancel = true;

                            WeakReferenceMessenger.Default.Send<MessageData>(new MessageData("Close Application"));
                        };
                    });
                });
#endif
            });
            builder.Services.AddSingleton<IFileSaver>(FileSaver.Default);

            builder.Services.AddMauiBlazorWebView();

            builder.Services.AddConfigToolServices();

#if DEBUG
            builder.Services.AddBlazorWebViewDeveloperTools();
            builder.Logging.AddDebug();
#endif
            builder.Services.AddMudServices();

            return builder.Build();
        }
    }
}