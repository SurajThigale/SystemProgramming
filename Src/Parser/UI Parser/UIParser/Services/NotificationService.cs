using ConfigParser.Services;
using Microsoft.AspNetCore.Components;
using MudBlazor;
using System.ComponentModel;
using static MudBlazor.Defaults.Classes;

namespace UIParser.Services;


public enum Alerts
{
    [Description("normal")]
    Normal,
    [Description("info")]
    Info,
    [Description("success")]
    Success,
    [Description("warning")]
    Warning,
    [Description("error")]
    Error
}

public struct NotificationSchema
{
    public NotificationSchema() => DateTime = DateTime.Now;
    public NotificationSchema(string title, string description, DateTime dateTime, Alerts alert, bool showOpenLogButton = false)
    {
        Title = title;
        Description = description;
        DateTime = dateTime;
        Alert = alert;
        ShowOpenLogButton = showOpenLogButton;
    }

    public string Title { get; set; } = default!;
    public string Description { get; set; } = default!;
    public DateTime DateTime { get; set; } = DateTime.Now;
    public bool ShowOpenLogButton { get; set; } 

    public Alerts Alert { get; set; } = Alerts.Normal;

    public string GetTextColor()
    {
        switch (Alert)
        {
            case Alerts.Normal:
                return "color:black"; // Set your color for normal alerts
            case Alerts.Info:
                return "color:deepskyblue"; // Set your color for info alerts
            case Alerts.Success:
                return "color:forestgreen"; // Set your color for success alerts
            case Alerts.Warning:
                return "color:orange"; // Set your color for warning alerts
            case Alerts.Error:
                return "color:#FF4433"; // Set your color for error alerts
            default:
                return "color:white"; // Default color
        }
    }

    public string GetClass()
    {
        return $"mud-{Alert.ToString().ToLower()}";
    }

    internal int CompareTo(NotificationSchema a)
    {
        return DateTime.CompareTo(a.DateTime);
    }
}

public class NotificationsService
{
    public List<NotificationSchema> Messages { get; private set; }

    public ISnackbar? SnackbarService { get; private set; }

    RefreshService? refresh { get; set; }

    private LoggerService? loggerService;

    public bool UnreadMessages { get; set; }

    public NotificationsService(RefreshService? refresh, LoggerService? logger)
    {
        this.Messages = new List<NotificationSchema>();
        this.refresh = refresh;
        this.loggerService = logger;
        UnreadMessages = false;
    }


    public async Task AddNotificationMessage(ISnackbar? SnackbarService, NotificationSchema alert, bool showToast)
    {
        SnackbarService!.Configuration.PositionClass = Defaults.Classes.Position.BottomLeft + " mb-5";

        UnreadMessages = true;
        this.Messages.Add(alert);

        Messages.Sort((a, b) => b.CompareTo(a));

        if (showToast)
        {
            SnackbarService!.Add(alert.Title, (Severity)alert.Alert, (options) =>
            {
                options.CloseAfterNavigation = true;
            });
        }

        switch(alert.Alert)
        {
            case Alerts.Normal:
                await loggerService!.LogInformationAsync($"{alert.Title} : {alert.Description}");
                break;
            case Alerts.Info:
                await loggerService!.LogInformationAsync($"{alert.Title} : {alert.Description}");
                break;
            case Alerts.Success:
                await loggerService!.LogInformationAsync($"{alert.Title} : {alert.Description}");
                break;
            case Alerts.Warning:
                await loggerService!.LogWarningAsync($"{alert.Title} : {alert.Description}");
                break;
            case Alerts.Error:
                await loggerService!.LogErrorAsync($"{alert.Title} : {alert.Description}");
                break;
        }

        refresh!.CallRequestRefresh();
    }
}
