using ConfigParser;
using ConfigParser.Services;
using Microsoft.AspNetCore.Components;
using UIParser.Models;
using UIParser.Services;

namespace UIParser.Components;

/// <summary>
/// 
/// </summary>
/// <seealso cref="Microsoft.AspNetCore.Components.ComponentBase" />
public partial class Grid
{
    /// <summary>
    /// Gets or sets the refresh service.
    /// </summary>
    /// <value>
    /// The refresh service.
    /// </value>
    [Inject]
    public RefreshService? refreshService { get; set; }

    /// <summary>
    /// Gets or sets the configuration.
    /// </summary>
    /// <value>
    /// The configuration.
    /// </value>
    [Inject]
    public ConfigService? Config { get; set; }

    /// <summary>
    /// Gets or sets the configuration native service.
    /// </summary>
    /// <value>
    /// The configuration native service.
    /// </value>
    [Inject]
    public ConfigNativeService? configNativeService { get; set; }

    /// <summary>
    /// Gets or sets the rows.
    /// </summary>
    /// <value>
    /// The rows.
    /// </value>
    [Parameter]
    public int Rows { get; set; }

    /// <summary>
    /// Gets or sets the columns.
    /// </summary>
    /// <value>
    /// The columns.
    /// </value>
    [Parameter]
    public int Columns { get; set; }

    /// <summary>
    /// Gets or sets the elements.
    /// </summary>
    /// <value>
    /// The elements.
    /// </value>
    [Parameter]
    public List<UIElement>? Elements { get; set; }

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
    /// Method invoked when the component is ready to start, having received its
    /// initial parameters from its parent in the render tree.
    /// Override this method if you will perform an asynchronous operation and
    /// want the component to refresh when that operation is completed.
    /// </summary>
    protected override async Task OnInitializedAsync()
    {
        refreshService!.RefreshRequested += () => StateHasChanged();

        await Task.Delay(0);
    }
}