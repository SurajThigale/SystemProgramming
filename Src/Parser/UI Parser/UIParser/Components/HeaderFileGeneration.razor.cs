using ConfigParser.Services;
using Microsoft.AspNetCore.Components;
using MudBlazor;
using UIParser.Components.Dialogs;
using UIParser.Services;

namespace UIParser.Components
{
    /// <summary>
    /// 
    /// </summary>
    /// <seealso cref="Microsoft.AspNetCore.Components.ComponentBase" />
    public partial class HeaderFileGeneration
    {

        enum CHeadersStructures
        {
            EMaster,
            ELCU,
            EXC,
            EIOC1,
            EIOC2
        }

        /// <summary>
        /// Gets or sets the configuration native service.
        /// </summary>
        /// <value>
        /// The configuration native service.
        /// </value>
        [Inject]
        ConfigNativeService? configNativeService { get; set; }

        [Inject]
        ConfigService? configService { get; set; }

        /// <summary>
        /// Gets or sets the refresh service.
        /// </summary>
        /// <value>
        /// The refresh service.
        /// </value>
        [Inject]
        RefreshService? refreshService { get; set; }

        /// <summary>
        /// Gets or sets the class.
        /// </summary>
        /// <value>
        /// The class.
        /// </value>
        [Parameter]
        public string? Class { get; set; }

        /// <summary>
        /// Gets or sets the style.
        /// </summary>
        /// <value>
        /// The style.
        /// </value>
        [Parameter]
        public string? Style { get; set; }


        [Inject]
        IDialogService? dialogService { get; set; }

        [Inject]
        LoggerService? logger { get; set; }

        /// <summary>
        /// Called when [initialized].
        /// </summary>
        protected override void OnInitialized()
        {
            refreshService!.RefreshRequested += () => StateHasChanged();
        }

        async Task ShowCHeadersByKey(string key)
        {
            try
            {
                var data = configNativeService?.GeneratedData?.data?.GetValueOrDefault(key);
                if (data == null)
                {
                    logger!.LogWarningAsync($"Data for {key} is null.");
                    return;
                }
                var parameters = new DialogParameters
                {
                    { "Title", key.ToUpper() },
                    { "JsonData", data.Data},
                    { "Color", MudBlazor.Color.Success }
                };

                var options = new MudBlazor.DialogOptions() { CloseButton = true, FullWidth = true };
                var dialog = await dialogService!.ShowAsync<CHeadersDialog>("Structure Insight", parameters, options);

                if (dialog != null)
                {
                    var result = await dialog.Result;
                    dialog.Close(result);
                }
            }
            catch (Exception ex)
            {
                logger!.LogErrorAsync($"Exception in ShowCHeadersByKey: {ex.Message}\n{ex.StackTrace}");
                await logger!.LogCriticalAsync($"Exception in ShowCHeadersByKey: {ex.Message}");
            }
        }
    }
}