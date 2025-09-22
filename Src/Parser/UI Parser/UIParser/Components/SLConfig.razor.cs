using ConfigParser.Services;
using Microsoft.AspNetCore.Components;
using MudBlazor;
using Radzen;
using UIParser.Components.Dialogs;
using UIParser.Services;

namespace UIParser.Components
{
    /// <summary>
    /// 
    /// </summary>
    /// <seealso cref="Microsoft.AspNetCore.Components.ComponentBase" />
    public partial class SLConfig
    {
        /// <summary>
        /// Gets or sets the configuration native service.
        /// </summary>
        /// <value>
        /// The configuration native service.
        /// </value>
        [Inject]
        ConfigNativeService? configNativeService { get; set; }

        /// <summary>
        /// Gets or sets the refresh service.
        /// </summary>
        /// <value>
        /// The refresh service.
        /// </value>
        [Inject]
        RefreshService? refreshService { get; set; }

        [Inject]
        StateManager? AppState { get; set; }

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

        /// <summary>
        /// Called when [initialized].
        /// </summary>
        protected override void OnInitialized()
        {
            refreshService!.RefreshRequested += () => StateHasChanged();
        }
    }
}