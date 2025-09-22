using CommunityToolkit.Maui.Storage;
using ConfigParser.Services;
using global::Microsoft.AspNetCore.Components;
using Microsoft.AspNetCore.Components.Web;
using Microsoft.JSInterop;
using MudBlazor;
using System.Reflection;
using System.Xml.Linq;
using UIParser.Services;
using Radzen.Blazor;
using System.Diagnostics;
using CommunityToolkit.Mvvm.Messaging;
using Microsoft.Maui.ApplicationModel.Communication;

namespace ConfigParser.Shared
{
    public partial class MainLayout
    {
        private ErrorBoundary? _errorBoundary = default;

        protected override void OnParametersSet()
        {
            _errorBoundary?.Recover();
        }

        [Inject]
        NavigationManager? _navigationManager { get; set; }

        [Inject]
        private ThemeService? themeService { get; set; }

        [Inject]
        private RefreshService? refreshService { get; set; }

        [Inject]
        private ConfigService? configService { get; set; }

        [Inject]
        private ConfigNativeService? configNativeService { get; set; }

        [Inject]
        private LoggerService? logger { get; set; }

        [Inject]
        XmlCompatibilityCheckerService? xmlCheckerService { get; set; }

        [Inject]
        NotificationsService? AlertService { get; set; }

        [Inject]
        AppInformationService? AppService { get; set; }

        [Inject]
        IDialogService? dialogService { get; set; }

        [Inject]
        ISnackbar? Snackbar { get; set; }

        [Inject]
        StateManager? AppState { get; set; }

        private MudThemeProvider? _mudThemeProvider;

        private string? AlertMessage { get; set; }

        private MudTheme _theme = new();

        private bool _isDarkMode = false;
        private string? FooterFilePathDisplay { get; set; }

        protected override async Task OnAfterRenderAsync(bool firstRender)
        {
            if (firstRender)
            {
                var systemTheme = await _mudThemeProvider!.GetSystemPreference();
                themeService?.InjectSystemTheme(systemTheme);
                await _mudThemeProvider!.WatchSystemPreference(OnSystemPreferenceChanged);
                refreshService!.CallRequestRefresh();
            }
        }

        private async Task OnSystemPreferenceChanged(bool newValue)
        {
            await Task.Delay(0);

            _isDarkMode = newValue;
            StateHasChanged();
        }

        protected override async Task OnInitializedAsync()
        {
            refreshService!.RefreshRequested += () =>
            {
                _isDarkMode = themeService?.bIsDarkTheme ?? false;

                StateHasChanged();
            };

            WeakReferenceMessenger.Default.Register<MessageData>(this, async (r, m) =>
            {
                await onApplicationClose();
            });

            await logger!.LogInformationAsync($"Program Version : {AppService!.ProductVersion}");

            if (configService?.getConfigFileName() == null)
            {
                AlertMessage = "Please load config file.";

                await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                {
                    Alert = Alerts.Info,
                    Title = AlertMessage,
                    Description = "Please open config file"
                }, true);
            }
        }

        private void ChangeTheme(bool bDarkTheme)
        {
            _isDarkMode = bDarkTheme;

            if (themeService != null)
            {
                themeService.bIsDarkTheme = bDarkTheme;
            }

            StateHasChanged();
        }

        private async Task OnOpenLogs(MouseEventArgs args)
        {
            try
            {
                System.Diagnostics.Process.Start("explorer.exe", logger!.getConfigLogFolderPath());
            }
            catch (Exception ex)
            {
                await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                {
                    Alert = Alerts.Error,
                    Title = "Open log files failed!!",
                    Description = "Please check config file path."
                }, true);

                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }

        public async Task ShowAboutDialog()
        {
            var options = new DialogOptions() { CloseButton = true, MaxWidth = MaxWidth.ExtraSmall, FullWidth=true };

            var dialog = await dialogService!.ShowAsync<About>("ConfigTool", options);
            var result = await dialog.Result;

            if (!result.Canceled)
            {
                dialog.Close();
            }
        }

        private async Task OnUserManual(MouseEventArgs args)
        {
            try
            {
                string UserManualPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location) + "\\Resources\\Docs\\UserManual.pdf";
                if (System.IO.File.Exists(UserManualPath))
                {
                    // Start the default PDF viewer with the specified file
                    Process.Start(new ProcessStartInfo
                    {
                        FileName = UserManualPath,
                        UseShellExecute = true
                    });
                }
                else
                {
                    Console.WriteLine("The specified PDF file does not exist.");
                }
            }
            catch (Exception ex)
            {
                await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                {
                    Alert = Alerts.Error,
                    Title = "Open User Manual failed!!",
                    Description = "Please check user manual folder path."
                }, true);

                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }

        void Reload()
        {
            _navigationManager!.NavigateTo(_navigationManager.Uri, true);
        }

        void SetNoficationBadge(bool bValue)
        {
            AlertService!.UnreadMessages = bValue;
        }

        private async Task CheckXmlInBackground()
        {
            try
            {
                configService!.isFileUpdated = false;

                var mismatchFound = await xmlCheckerService!.CheckXmlBindings(configService!.getXMLDoc()) ?? null;


                if ((mismatchFound is not null) && (mismatchFound?.Any() ?? false))
                {
                    string value = string.Join("\n", mismatchFound);
                    await logger!.LogErrorAsync($"Following entries are not bind with UI but these are present in XML file {configService!.getConfigFileName()} : \n{value}", "Mismatch_log_XML_to_UI");
                }

                mismatchFound = await configNativeService!.GetNonCompatibileNodes();

                if ((mismatchFound is not null) && (mismatchFound?.Any() ?? false))
                {
                    string value = string.Join("\n", mismatchFound);
                    await logger!.LogErrorAsync($"Following entries are not populated with C Header Structures but these are present in XML  file {configService!.getConfigFileName()}  :\n{value}", "Mismatch_log_XML_to_C_Headers");
                }
            }
            catch (Exception ex)
            {
                await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                {
                    Alert = Alerts.Error,
                    Title = "Error occurred while checking the XML Bindings!!",
                    Description = "Please reload the file."
                }, true);

                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }

        private async Task OnXMLOpenClicked(MouseEventArgs args)
        {
            try
            {
                (bool isAllowed, string message) = AppState!.CheckIfProcessAllowed();

                if (!isAllowed)
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = message,
                        Description = "Please wait for the operation to finish",
                    }, true);

                    return;
                }

                if (configService!.isFileUpdated)
                {
                    await onApplicationNewFileOpen();
                }

                var customFileType = new FilePickerFileType(
                   new Dictionary<DevicePlatform, IEnumerable<string>>
                   {
                        { DevicePlatform.WinUI, new[] { ".xml" } },
                   });

                var result = await FilePicker.PickAsync(new PickOptions
                {
                    PickerTitle = "Please select a file",
                    FileTypes = customFileType
                });

                if (result != null)
                {
                    string filePath = result.FullPath;

                    AlertService!.Messages.Clear();
                    configNativeService?.ClearGeneratedData();
                    StateHasChanged();

                    if (Path.GetExtension(filePath) != ".xml")
                    {
                        await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                        {
                            Alert = Alerts.Error,
                            Title = "Wrong file type selected!!",
                            Description = "Please check config file format."
                        }, true);

                        return;
                    }

                    FooterFilePathDisplay = filePath;

                    bool bResult = await configService!.LoadConfigFile(filePath);

                    if (bResult)
                    {
                        await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                        {
                            Alert = Alerts.Success,
                            Title = "Config File Loaded Successfully",
                            Description = "Config file loaded successfully."
                        }, true);

                        await logger!.LogInformationAsync($"Config File Loaded Successfully : {filePath}");

                        await CheckXmlInBackground();
                    }
                    else
                    {
                        await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                        {
                            Alert = Alerts.Error,
                            Title = "Config File Corrupted!!",
                            Description = "Please check config file format."
                        }, true);
                    }
                }
            }
            catch (Exception ex)
            {
                await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                {
                    Alert = Alerts.Error,
                    Title = "Config File Corrupted!!",
                    Description = "Please check config file format."
                }, true);

                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }

            AppState!.ConfigGenerationState = EGenerationState.None;

            refreshService!.CallRequestRefresh();
        }

        private async Task OnExcelOpenClicked(MouseEventArgs args)
        {
            try
            {
                if (configService!.isFileUpdated)
                {
                    await onApplicationNewFileOpen();
                }

                (bool isAllowed, string message) = AppState!.CheckIfProcessAllowed();

                if (!isAllowed)
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = message,
                        Description = "Please wait for the operation to finish",
                    }, true);

                    return;
                }

                var customFileType = new FilePickerFileType(
                   new Dictionary<DevicePlatform, IEnumerable<string>>
                   {
                        { DevicePlatform.WinUI, new[] { ".xlsm" } },
                   });

                var result = await FilePicker.PickAsync(new PickOptions
                {
                    PickerTitle = "Please select a file",
                    FileTypes = customFileType
                });

                if (result != null)
                {
                    string filePath = result.FullPath;

                    AlertService!.Messages.Clear();
                    configNativeService?.ClearGeneratedData();
                    StateHasChanged();

                    if (Path.GetExtension(filePath) != ".xlsm")
                    {
                        await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                        {
                            Alert = Alerts.Error,
                            Title = "Wrong file type selected!!",
                            Description = "Please check config file format."
                        }, true);

                        return;
                    }

                    FooterFilePathDisplay = filePath;

                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Info,
                        Title = $"Excel parsing in progress..",
                        Description = $"Excel parsing in progress.."
                    }, true);
                    
                    await InvokeAsync(() => RunExcelToXMLConverter(filePath));
                }
            }
            catch (Exception ex)
            {
                await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                {
                    Alert = Alerts.Error,
                    Title = "Error in opening Excel file",
                    Description = "Please check log file for details."
                }, true);

                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }

        private async Task RunExcelToXMLConverter(string filePath)
        {
            try
            {
                var currentBinPath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location)?.Replace("\\", "/");

                // The script arguments should be like this:
                // ExcelToXMLConverter.exe <excelFilePath> <xmlOutputPath>
                string embeddedPythonExePath = $"{currentBinPath}/Resources/ExcelToXMLConverter/ExcelToXMLConverter.exe";

                // Specify the parameters to pass to the Python executable
                string excelFilePath = filePath.Replace("\\", "/");

                string convertedXMLDir = $"{currentBinPath}/Resources";
                Directory.CreateDirectory(convertedXMLDir);

                string xmlOutFilePath = $"{convertedXMLDir}/config.xml";

                // Create a new process start info
                ProcessStartInfo psi = new ProcessStartInfo
                {
                    FileName = embeddedPythonExePath,
                    WorkingDirectory = Path.GetDirectoryName(embeddedPythonExePath),
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    CreateNoWindow = true
                };
                psi.ArgumentList.Add(excelFilePath);
                psi.ArgumentList.Add(xmlOutFilePath);
                psi.ArgumentList.Add(logger!.getConfigLogFolderPath());

                // Start the process
                string output = "";
                using (Process? process = Process.Start(psi))
                {
                    if (process is not null)
                    {
                        // Read the output (if needed)
                        output = process!.StandardOutput.ReadToEnd();

                        // Wait for the process to exit
                        process?.WaitForExit();

                        // Display the output (if needed)
                        await logger!.LogInformationAsync($"Output of ExcelToXMLConverter : {output}");
                    }
                }

                var excelParseSuccess = output.ToLower().Contains("success");
                await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                {
                    Alert = excelParseSuccess ? Alerts.Success : Alerts.Error,
                    Title = $"Excel Read {(excelParseSuccess ? " Successfully" : " Failed.")}",
                    Description = $"Excel Read {(excelParseSuccess ? " Successfully" : " Failed. Check 'Excel Parse failed.log' file for details")}",
                    ShowOpenLogButton = !excelParseSuccess
                }, true);
                if (!excelParseSuccess)
                {
                    _ = configService!.LoadConfigFile("");
                    await Task.Delay(100); //Delay required for UI refresh
                    AppState!.ConfigGenerationState = EGenerationState.None;
                    refreshService!.CallRequestRefresh();
                    return;
                }

                bool bResult = await configService!.LoadConfigFile(xmlOutFilePath);

                if (!bResult)
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = "Config File Corrupted!!",
                        Description = "Please check config file format."
                    }, true);

                    return;
                }

                await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                {
                    Alert = Alerts.Success,
                    Title = "Config File Loaded Successfully",
                    Description = "Config file loaded successfully."
                }, true);

                await logger!.LogInformationAsync($"Config File Loaded Successfully : {xmlOutFilePath}");

                await CheckXmlInBackground();

                AppState!.ConfigGenerationState = EGenerationState.None;

                refreshService!.CallRequestRefresh();
            }
            catch (Exception ex)
            {
                await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                {
                    Alert = Alerts.Error,
                    Title = "Config File Corrupted!!",
                    Description = "Please check config file format."
                }, true);

                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }

        private async Task OnExportClicked(MouseEventArgs args)
        {
            try
            {
                _ = args;

                if (String.IsNullOrEmpty(configService?.getConfigFileName()))
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = "No File Opened!!",
                        Description = "Please open config xml file"
                    }, true);

                    return;
                }

                (bool isAllowed, string message) = AppState!.CheckIfProcessAllowed();

                if (!isAllowed)
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = message,
                        Description = "Please wait for the operation to finish",
                    }, true);

                    return;
                }

                AppState!.ExportInProgress = true;

                StateHasChanged();

                await ConfigCommsService.Generate();
                AppState!.ConfigGenerationState = EGenerationState.Generated;

                Stream stream = new MemoryStream();
                var fileSaverResult = await FileSaver.Default.SaveAsync("config.xml", stream, CancellationToken.None);
                if (fileSaverResult.IsSuccessful)
                {
                    await ConfigCommsService.WriteXMLAsync(fileSaverResult.FilePath);

                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Success,
                        Title = "Export Completed",
                        Description = "Config exported successfully"
                    }, true);

                    await logger!.LogInformationAsync($"Exported XML into Path : {fileSaverResult.FilePath}");

                }
                else
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = "Export Failed!!",
                        Description = "Config exported failed. Please check config XML file format."
                    }, true);
                }

                AppState!.ExportInProgress = false;

                refreshService!.CallRequestRefresh();
            }
            catch (Exception ex)
            {
                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }

        private async Task OnSaveAs(MouseEventArgs args)
        {
            try
            {
                _ = args;
                Stream stream = new MemoryStream();

                if (String.IsNullOrEmpty(configService?.getConfigFileName()))
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = "No File Opened!!",
                        Description = "Please open config xml file"
                    }, true);

                    return;
                }

                await configService.getXMLDoc().SaveAsync(stream, SaveOptions.None, CancellationToken.None);
                var fileSaverResult = await FileSaver.Default.SaveAsync("config.xml", stream, CancellationToken.None);
                if (fileSaverResult.IsSuccessful)
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Success,
                        Title = "File Save Completed",
                        Description = "Config file saved successfully"
                    }, true);

                    await logger!.LogInformationAsync($"Saved XML into Path : {fileSaverResult.FilePath}");

                }
                else
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = "File Save Failed!!",
                        Description = "Config file save failed. Please check config XML file format."
                    }, true);
                }
            }
            catch (Exception ex)
            {
                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }

        private async Task OnExportHFiles(MouseEventArgs args)
        {
            await Task.Delay(0);
        }

        private async Task OnGenerateClicked(MouseEventArgs args)
        {
            try
            {
                if (String.IsNullOrEmpty(configService?.getConfigFileName()))
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = "No File Opened!!",
                        Description = "Please open config xml file"
                    }, true);

                    return;
                }

                (bool isAllowed, string message) = AppState!.CheckIfProcessAllowed();

                if (!isAllowed)
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = message,
                        Description = "Please wait for the operation to finish",
                    }, true);

                    return;
                }

                // Set the shared flag and use the shared version_no
                AppState!.IsGenerateClicked = true;
                string version_no = AppState!.VersionNo;
                
                var xmlDoc = configService!.getXMLDoc();
                var generalNode = xmlDoc.Descendants("general").FirstOrDefault();
                
                if (generalNode != null)
                {
                    var configXMLVersionParam = generalNode.Elements("parameter")
                        .FirstOrDefault(e => e.Attribute("name")?.Value == "ConfigXMLVersion");
                        
                        if (configXMLVersionParam != null)
                        {
                            configXMLVersionParam.Attribute("value")!.Value = version_no;
                        }
                        else
                        {
                            // If the parameter doesn't exist, create it
                            generalNode.Add(new System.Xml.Linq.XElement("parameter",
                                new System.Xml.Linq.XAttribute("name", "ConfigXMLVersion"),
                                new System.Xml.Linq.XAttribute("value", version_no)));
                        }
                }

                // Refresh the UI immediately to show the updated ConfigXMLVersion
                refreshService!.CallRequestRefresh();
                StateHasChanged();
                
                AppState!.GenerationInProgrss = true;

                StateHasChanged();

                await ConfigCommsService.Generate();

                AppState!.ConfigGenerationState = EGenerationState.Generated;

                refreshService!.CallRequestRefresh();

                StateHasChanged();

                await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                {
                    Alert = Alerts.Success,
                    Title = "Config Generation Completed",
                    Description = "Config generated successfully"
                }, true);

                await logger!.LogInformationAsync($"Generated Binary for file : {configService.getConfigFileName()}");

                AppState!.GenerationInProgrss = false;

                StateHasChanged();
            }
            catch
            {
                await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                {
                    Alert = Alerts.Error,
                    Title = "Config Generation Failed!!",
                    Description = ""
                }, true);

                await logger!.LogCriticalAsync($"Exception caught in in {System.Reflection.MethodBase.GetCurrentMethod()} : Generation Failed");
            }
        }

        private async Task OnUploadToSN(MouseEventArgs args)
        {
            try
            {
                _ = args;

                if (String.IsNullOrEmpty(configService?.getConfigFileName()))
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = "No File Opened!!",
                        Description = "Please open config xml file"
                    }, true);

                    return;
                }

                (bool isAllowed, string message) = AppState!.CheckIfProcessAllowed();

                if (!isAllowed)
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = message,
                        Description = "Please wait for the operation to finish",
                    }, true);

                    return;
                }

                await OnGenerateClicked(null!);

                AppState!.UploadToSNInProgress = true;

                StateHasChanged();

                bool bStatus = await ConfigCommsService.OnUploadToSN();
                string description = "";

                AppState!.ConfigUploadState = EUplaodState.Uploaded;

                foreach (var node in ConfigCommsService?.GeneratedData?.UploadStatus ?? [])
                {
                    string name = node.Key.ToLower();

                    if (string.Equals(node.Key, "master", StringComparison.OrdinalIgnoreCase))
                    {
                        description += node.Value ? "Master uploaded successfully.\n" : "Master upload Failed.\n";
                    }
                    else if (string.Equals(node.Key, "ControlStation", StringComparison.OrdinalIgnoreCase))
                    {
                        description += node.Value ? "ControlStation uploaded successfully.\n" : "ControlStation upload Failed.\n";
                    }
                    else if (node.Key.StartsWith("lcu", StringComparison.OrdinalIgnoreCase))
                    {
                        description += node.Value ? $"{node.Key.ToUpper()} uploaded successfully.\n" : $"{node.Key.ToUpper()} upload Failed.\n";
                    }
                    else if (node.Key.StartsWith("ioc", StringComparison.OrdinalIgnoreCase))
                    {
                        description += node.Value ? $"{node.Key.ToUpper()} uploaded successfully.\n" : $"{node.Key.ToUpper()} upload Failed.\n";
                    }
                    else if (node.Key.StartsWith("xc", StringComparison.OrdinalIgnoreCase))
                    {
                        description += node.Value ? $"{node.Key.ToUpper()} uploaded successfully.\n" : $"{node.Key.ToUpper()} upload Failed.\n";
                    }/*switch (node.Key)
                    {
                        case "master":
                            description += node.Value ? "Master uploaded successfully." : "Master upload Failed.";
                            description += "\n";
                            break;
                        case "lcu":
                            description += node.Value ? "LCU uploaded successfully." : "LCU upload Failed.";
                            description += "\n";
                            break;
                        case "xc":
                            description += node.Value ? "EX uploaded successfully." : "EX upload Failed.";
                            description += "\n";
                            break;
                        case "ioc":
                            description += node.Value ? "IOC uploaded successfully." : "IOC upload Failed.";
                            description += "\n";
                            break;
                        //case "ioc2":
                        //    description += node.Value ? "IOC2 uploaded successfully." : "IOC2 upload Failed.";
                        //    description += "\n";
                        //    break;
                        case "ControlStation":
                            description += node.Value ? "ControlStation uploaded successfully." : "ControlStation upload Failed.";
                            description += "\n";
                            break;
                    }*/
                }

                if (bStatus)
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Success,
                        Title = "Upload Successful",
                        Description = description
                    }, true);
                }
                else
                {
                    await AlertService!.AddNotificationMessage(Snackbar, new NotificationSchema
                    {
                        Alert = Alerts.Error,
                        Title = "Upload Failed!!",
                        Description = description
                    }, true);
                }

                AppState!.UploadToSNInProgress = false;

                refreshService!.CallRequestRefresh();
            }
            catch (Exception ex)
            {
                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }

        public async Task onApplicationClose()
        {
            if (configService?.isFileUpdated ?? false)
            {
                var parameters = new DialogParameters<CloseDialog>();
                parameters.Add(x => x.ContentText, "Changes not saved. Would you like to save them before closing?");
                parameters.Add(x => x.ButtonText, "Save");
                parameters.Add(x => x.Color, MudBlazor.Color.Success);

                var options = new DialogOptions() { CloseButton = true, MaxWidth = MaxWidth.ExtraSmall };

                var dialog = await dialogService!.ShowAsync<CloseDialog>("Save Changes", parameters, options);
                var result = await dialog.Result;

                if (!result.Canceled)
                {
                    await OnExportClicked(null!);
                }
            }
            await InvokeAsync(StateHasChanged);

            App.Current!.Quit();
        }

        public async Task onApplicationNewFileOpen()
        {
            if (configService?.isFileUpdated ?? false)
            {
                var parameters = new DialogParameters<CloseDialog>();
                parameters.Add(x => x.ContentText, "Changes not saved. Would you like to save them before closing this configuration?");
                parameters.Add(x => x.ButtonText, "Save");
                parameters.Add(x => x.Color, MudBlazor.Color.Success);

                var options = new DialogOptions() { CloseButton = true, MaxWidth = MaxWidth.ExtraSmall };

                var dialog = await dialogService!.ShowAsync<CloseDialog>("Save Changes", parameters, options);
                var result = await dialog.Result;

                if (!result.Canceled)
                {
                    await OnExportClicked(null!);
                }
            }
            await InvokeAsync(StateHasChanged);
        }
    }
}