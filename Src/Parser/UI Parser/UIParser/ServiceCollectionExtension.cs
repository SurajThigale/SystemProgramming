using ConfigParser;
using ConfigParser.Services;
using Microsoft.Extensions.DependencyInjection;
using System.Reflection;
using UIParser.Services;

namespace UIParser;

public static class ServiceCollectionExtension
{
    public static void AddConfigToolServices(this IServiceCollection serviceCollection)
    {
        var assemplyPath = Assembly.GetExecutingAssembly().Location;
        var configFilePath = Path.Combine(Path.GetDirectoryName(assemplyPath)!,
             "Resources/config.xml");

        serviceCollection.AddSingleton<AppInformationService>();
        serviceCollection.AddSingleton<RefreshService>();
        serviceCollection.AddSingleton<LoggerService>();
        serviceCollection.AddSingleton<ThemeService>();
        serviceCollection.AddSingleton<StateManager>();
        serviceCollection.AddSingleton<NotificationsService>();
        serviceCollection.AddSingleton<XmlCompatibilityCheckerService>();
        serviceCollection.AddSingleton<ConfigService>();
        serviceCollection.AddSingleton<ConfigNativeService>();
    }
}
