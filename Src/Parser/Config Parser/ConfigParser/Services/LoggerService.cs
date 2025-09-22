using Microsoft.Extensions.Logging;
using Serilog;
using Serilog.Core;
using System.Collections.Concurrent;
using System.IO;
using System.Reflection;
using ILogger = Serilog.ILogger;

namespace ConfigParser.Services;

public class LoggerService
{
    private readonly ConcurrentDictionary<string, ILogger> loggerCache = new ConcurrentDictionary<string, ILogger>();
    private readonly ILogger defaultLogger;
    private readonly object lockObject = new object();
    private readonly string appLogFolderPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + "\\Logs\\AppLogs";
    private string baseConfigLogFolderPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + "\\Logs\\ConfigErrorLogs";
    private string configLogFolderPath;

    public AppInformationService? AppService { get; private set; }

    public LoggerService(AppInformationService? appService)
    {
        AppService = appService;

        string timestamp = DateTime.Now.ToString("yyyyMMdd_HHmmss");
        // Define the log file path with the timestamp
        configLogFolderPath = $"{baseConfigLogFolderPath}\\Config_{timestamp}";

        // Delete the log directories and its contents recursively
        DeleteDirectory(baseConfigLogFolderPath);

        // Define the log file path with the timestamp
        string logFilePath = $"ConfigTool_{timestamp}.log.txt";

        var path = Path.Combine(appLogFolderPath, logFilePath);
        defaultLogger = new LoggerConfiguration()
            .WriteTo.File(path)
            .CreateLogger();

        Task.Run(async () => await LogInformationAsync($"{AppService!.ProductName}:{AppService!.ProductVersion}")).GetAwaiter().GetResult();
    }

    static void DeleteDirectory(string targetDirectory)
    {
        try
        {
            // Delete the target directory
            if (Directory.Exists(targetDirectory))
                Directory.Delete(targetDirectory, true);
        }
        catch (Exception ex)
        {
            _ = ex;
        }
    }

    public async Task LogInformationAsync(string message, string fileName = null!)
    {
        await LogAsync(LogLevel.Information, message, fileName);
    }

    public async Task LogWarningAsync(string message, string fileName = null!)
    {
        await LogAsync(LogLevel.Warning, message, fileName);
    }

    public async Task LogErrorAsync(string message, string fileName = null!)
    {
        await LogAsync(LogLevel.Error, message, fileName);
    }

    public async Task LogCriticalAsync(string message, string fileName = null!)
    {
        await LogAsync(LogLevel.Critical, message, fileName);
    }

    private async Task LogAsync(LogLevel logLevel, string message, string fileName)
    {
        await Task.Run(() =>
        {
            var (logger, isNewFile) = GetOrCreateLogger(fileName);


            if (isNewFile)
            {
                LogWithLevel(logger, LogLevel.Information, $"{AppService!.ProductName}:{AppService!.ProductVersion}");
            }

            LogWithLevel(logger, logLevel, message);
        });
    }

    private (ILogger, bool) GetOrCreateLogger(string fileName)
    {
        if (!string.IsNullOrWhiteSpace(fileName))
        {
            bool isNewLogger = false;
            var logger = loggerCache.GetOrAdd(fileName, _ =>
            {
                // Define the log file path
                string logFilePath = $"{fileName}.log.txt";

                var path = Path.Combine(configLogFolderPath, logFilePath);

                isNewLogger = true;

                return new LoggerConfiguration()
                    .WriteTo.File(path)
                    .CreateLogger();
            });

            return (logger, isNewLogger);
        }
        return (defaultLogger, false);
    }

    private void LogWithLevel(ILogger logger, LogLevel logLevel, string message)
    {
        lock (lockObject)
        {
            switch (logLevel)
            {
                case LogLevel.Information:
                    logger.Information(message);
                    break;
                case LogLevel.Warning:
                    logger.Warning(message);
                    break;
                case LogLevel.Error:
                    logger.Error(message);
                    break;
                case LogLevel.Critical:
                    logger.Fatal(message);
                    break;
            }
        }
    }

    public string getConfigLogFolderPath()
    {
        Directory.CreateDirectory(configLogFolderPath);

        return configLogFolderPath;
    }

    public async Task DeleteOldLogFilesForNewConfig(string fileName) 
    {
        try
        {
            foreach (var logger in loggerCache)
            {
                (logger.Value as IDisposable)?.Dispose();
            }
            loggerCache.Clear();

            //var logFiles = Directory.GetFiles(configLogFolderPath);
            //foreach (var file in logFiles)
            //{
            //    File.Delete(file);
            //}

            string timestamp = DateTime.Now.ToString("yyyyMMdd_HHmmss");
            // Define the log file path with the timestamp
            configLogFolderPath = $"{baseConfigLogFolderPath}\\Config_{timestamp}";
        }
        catch (Exception ex)
        {
            await LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
        }
    }
}
