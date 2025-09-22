using ConfigParser.Model.ParserMetaData;
using ConfigParser.Services;
using global::Microsoft.AspNetCore.Components;
using MudBlazor;
using System.Net;
using UIParser.Models;
using UIParser.Services;

namespace UIParser.Components;

/// <summary>
/// 
/// </summary>
/// <seealso cref="Microsoft.AspNetCore.Components.ComponentBase" />
public partial class TextEdit
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
    /// Gets or sets the configuration service.
    /// </summary>
    /// <value>
    /// The configuration service.
    /// </value>
    [Inject]
    public ConfigService? configService { get; set; }

    /// <summary>
    /// Gets or sets the logger.
    /// </summary>
    /// <value>
    /// The logger.
    /// </value>
    [Inject]
    public LoggerService? logger { get; set; }
    
    /// <summary>
    /// Gets or sets the state manager.
    /// </summary>
    /// <value>
    /// The state manager.
    /// </value>
    [Inject]
    public StateManager? AppState { get; set; }

    /// <summary>
    /// Gets or sets the snackbar.
    /// </summary>
    /// <value>
    /// The snackbar.
    /// </value>
    [Inject]
    ISnackbar? Snackbar { get; set; }

    /// <summary>
    /// Gets or sets the name.
    /// </summary>
    /// <value>
    /// The name.
    /// </value>
    [Parameter]
    public string? Name { get; set; }

    /// <summary>
    /// Gets or sets the element found.
    /// </summary>
    /// <value>
    /// The element found.
    /// </value>
    [Parameter]
    public bool? ElementFound { get; set; } = false;

    /// <summary>
    /// Gets or sets the type.
    /// </summary>
    /// <value>
    /// The type.
    /// </value>
    [Parameter]
    public string? Type { get; set; }

    /// <summary>
    /// The current time
    /// </summary>
    private string currentTime = DateTime.Now.ToString();

    private MudTextField<double?>? decimalTextBoxRef { get; set; }

    private MudTextField<int?>? intTextBoxRef { get; set; }

    private MudTextField<string>? stringTextBoxRef { get; set; }

    private MudTextField<IPAddress>? ipTextBoxRef { get; set; }

    /// <summary>
    /// The CTS
    /// </summary>
    private CancellationTokenSource cts = new();

    /// <summary>
    /// Method invoked when the component is ready to start, having received its
    /// initial parameters from its parent in the render tree.
    /// </summary>
    protected override void OnInitialized()
    {
        //TODO: This thread was created as a temporary solution to address issues with styles not being applied to the
        //          component and difficulties in reverting incorrect values on the UI. A better alternative should be found.
        Task.Run(async () =>
        {

            while (!cts.IsCancellationRequested)
            {
                try
                {
                    if (!String.IsNullOrEmpty(configService!.getConfigFileName()))
                    {

                        currentTime = DateTime.Now.ToString();
                        await InvokeAsync(StateHasChanged);
                    }

                }
                catch
                {
                    await logger!.LogErrorAsync("Exception occured in Refresh Thread of TextEdit");
                }
                await Task.Delay(500);
            }
        });
    }

    /// <summary>
    /// Releases unmanaged and - optionally - managed resources.
    /// </summary>
    public void Dispose()
    {
        cts.Cancel();
    }

    /// <summary>
    /// Parses the specified value.
    /// </summary>
    /// <param name="value">The value.</param>
    public void Parse(IPAddress value)
    {
        ElementValue!.Value!.Value = value.ToString();
    }

    /// <summary>
    /// The converter
    /// </summary>
    Converter<IPAddress> converter = new Converter<IPAddress>
    {
        SetFunc = value => value!.ToString(),
        GetFunc = text =>
        {
            try
            {
                return IPAddress.Parse(text!);
            }
            catch
            {
                return null;
            }
        }
    };

    /// <summary>
    /// Validates the ip.
    /// </summary>
    /// <param name="ip">The ip.</param>
    /// <returns></returns>
    string validateIP(IPAddress ip)
    {
        if (ip is null) return "Invalid IP";

        return "";
    }

    /// <summary>
    /// Validates the type of the int.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns></returns>
    private bool ValidateIntType(string? value)
    {
        if (!Int32.TryParse(value!.ToString(), out _))
        {

            return false;
        }

        return true;
    }

    /// <summary>
    /// Validates the type of the binary.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns></returns>
    private bool ValidateBinaryType(string? value)
    {
        Binary newVal = new Binary(value ?? "Invalid");
        if (!newVal?.IsValid ?? false)
        {
            return false;
        }

        return true;
    }

    /// <summary>
    /// Validates the type of the double.
    /// </summary>
    /// <param name="value">The value.</param>
    /// <returns></returns>
    private bool ValidateDoubleType(string? value)
    {
        if (!Double.TryParse(value!.ToString(), out _))
        {

            return false;
        }

        return true;
    }

    /// <summary>
    /// Updates the attribute.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="newValue">The new value.</param>
    /// <param name="previousValue">The previous value.</param>
    /// <param name="currentValue">The current value.</param>
    private async Task UpdateAttribute<T>(T newValue, T? previousValue, T? currentValue)
    {
        try
        {
            if (ElementValue is null)
            {
                return;
            }

            if (typeof(T) == typeof(string))
            {
                // Handle string type
                string? newValueString = newValue as string;
                if (newValueString != null)
                {
                    // Value is a valid string, update `currentValue`.
                    currentValue = (T)(object)newValueString;
                    ElementValue.bIsDataUpdate = true;
                }
                else
                {
                    // Value is not valid, revert to `previousValue`.
                    currentValue = previousValue;
                    stringTextBoxRef!.Reset();
                    Snackbar?.Add($"Invalid string value added for {Name}. Reverting to previous value.", Severity.Error);
                }
            }
            else if (typeof(T) == typeof(int?))
            {
                // Handle int type
                if (int.TryParse(newValue!.ToString(), out int parsedValue))
                {
                    // Value is a valid integer, update `currentValue`.
                    currentValue = (T)(object)parsedValue;
                    ElementValue.bIsDataUpdate = true;
                }
                else
                {
                    // Value is not valid, revert to `previousValue`.
                    currentValue = previousValue;
                    intTextBoxRef!.Reset();
                    Snackbar?.Add($"Invalid integer value added for {Name}. Reverting to previous value.", Severity.Error);
                }
            }
            else if (typeof(T) == typeof(double?))
            {
                // Handle double type
                if (double.TryParse(newValue!.ToString(), out double parsedValue))
                {
                    // Value is a valid double, update `currentValue`.
                    currentValue = (T)(object)parsedValue;
                    ElementValue.bIsDataUpdate = true;
                }
                else
                {
                    // Value is not valid, revert to `previousValue`.
                    currentValue = previousValue;
                    decimalTextBoxRef!.Reset();
                    Snackbar?.Add($"Invalid decimal value added for {Name}. Reverting to previous value.", Severity.Error);
                }
            }
            else if (typeof(T) == typeof(IPAddress))
            {
                if (newValue == null)
                {
                    currentValue = previousValue;
                }
                else
                {
                    // Handle double type
                    if (IPAddress.TryParse(newValue.ToString(), out IPAddress? parsedValue))
                    {
                        // Value is a valid double, update `currentValue`.
                        currentValue = (T)(object)parsedValue;
                        ElementValue.bIsDataUpdate = true;
                    }
                    else
                    {
                        // Value is not valid, revert to `previousValue`.
                        currentValue = previousValue;
                        ipTextBoxRef!.Reset();
                        Snackbar?.Add($"Invalid IP value added for {Name}. Reverting to previous value.", Severity.Error);
                    }
                }
            }
            if (typeof(T) == typeof(Binary))
            {
                // Handle int type
                Binary newBinValue = new Binary(newValue?.ToString() ?? "Invalid");
                if (newBinValue.IsValid)
                {
                    // Value is a valid integer, update `currentValue`.
                    currentValue = (T)(object)newBinValue;
                    ElementValue.bIsDataUpdate = true;
                }
                else
                {
                    // Value is not valid, revert to `previousValue`.
                    currentValue = previousValue;
                    stringTextBoxRef!.Reset();
                    Snackbar?.Add($"Invalid binary value added for {Name}. Reverting to previous value.", Severity.Error);
                }
            }
            else
            {
                // Handle unsupported types or throw an exception.
                // You may want to add additional handling for other types.
            }



            if (typeof(T) == typeof(Binary))
            {
                Binary bin = (Binary)(object)currentValue!;
                await ElementValue.SetValueAsync(ElementValue.Name, bin.ToDecimal().ToString() ?? "");
            }
            else
            {
                // Handle the text change here.
                await ElementValue.SetValueAsync(ElementValue.Name, currentValue?.ToString() ?? "");
            }

            if (!configService!.isFileUpdated && ElementValue.bIsDataUpdate)
                configService!.isFileUpdated = ElementValue.bIsDataUpdate;

            if (!configService!.isGenerationModified && ElementValue.bIsDataUpdate)
                configService!.isGenerationModified = ElementValue.bIsDataUpdate;

            StateHasChanged();

        }
        catch (Exception ex)
        {
            await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
        }
    }

    // Define a parameter with [Parameter] attribute
    /// <summary>
    /// Gets or sets the element value.
    /// </summary>
    /// <value>
    /// The element value.
    /// </value>
    [Parameter]
    public IXmlNode? ElementValue { get; set; }

    /// <summary>
    /// Gets or sets the remark.
    /// </summary>
    /// <value>
    /// The remark.
    /// </value>
    [Parameter]
    public string? Remark { get; set; }

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
    /// Shows a read-only message when user clicks on a read-only field
    /// </summary>
    private void ShowReadOnlyMessage(bool isReadOnly)
    {
        if (isReadOnly && Snackbar != null)
        {
            Snackbar.Add("This field is read-only and cannot be modified.", Severity.Warning, config =>
            {
                config.ShowTransitionDuration = 500;  // Slower fade-in (0.5 seconds)
                config.HideTransitionDuration = 800;  // Even slower fade-out (0.8 seconds)
                config.VisibleStateDuration = 2000;  // Visible for 2 seconds
            });
        }
    }
}


