using ConfigParser.Model.ParserMetaData;
using ConfigParser.Services;
using global::Microsoft.AspNetCore.Components;
using MudBlazor;
using System.Net;
using System.Reflection.Metadata;
using System.Xml.Linq;
using UIParser.Models;
using UIParser.Services;
using static MudBlazor.CategoryTypes;
using static MudBlazor.Colors;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace UIParser.Components
{
    /// <summary>
    /// 
    /// </summary>
    /// <seealso cref="Microsoft.AspNetCore.Components.ComponentBase" />
    public partial class NameValuePairGrid
    {
        /// <summary>
        /// Gets or sets the items.
        /// </summary>
        /// <value>
        /// The items.
        /// </value>
        [Parameter]
        public List<IXmlNode>? Items { get; set; }

        /// <summary>
        /// Gets or sets the class.
        /// </summary>
        /// <value>
        /// The class.
        /// </value>
        [Parameter]
        public string Class { get; set; } = "mt-5";

        /// <summary>
        /// Gets or sets the style.
        /// </summary>
        /// <value>
        /// The style.
        /// </value>
        [Parameter]
        public string? Style { get; set; }

        /// <summary>
        /// Gets or sets the type.
        /// </summary>
        /// <value>
        /// The type.
        /// </value>
        [Parameter]
        public string? Type { get; set; }

        /// <summary>
        /// Gets or sets the parent.
        /// </summary>
        /// <value>
        /// The parent.
        /// </value>
        [Parameter]
        public IXmlNode? Parent { get; set; }

        /// <summary>
        /// Gets or sets the name of the x path.
        /// </summary>
        /// <value>
        /// The name of the x path.
        /// </value>
        [Parameter]
        public string? XPathName { get; set; }

        /// <summary>
        /// Gets or sets the show add delete buttons.
        /// </summary>
        /// <value>
        /// The show add delete buttons.
        /// </value>
        [Parameter]
        public bool? ShowAddDeleteButtons { get; set; }

        /// <summary>
        /// Gets or sets the buttons class.
        /// </summary>
        /// <value>
        /// The buttons class.
        /// </value>
        [Parameter]
        public string? ButtonsClass { get; set; }

        /// <summary>
        /// Gets or sets the buttons style.
        /// </summary>
        /// <value>
        /// The buttons style.
        /// </value>
        [Parameter]
        public string? ButtonsStyle { get; set; }

        /// <summary>
        /// Gets or sets the description.
        /// </summary>
        /// <value>
        /// The description.
        /// </value>
        [Parameter]
        public string? Description { get; set; }


        /// <summary>
        /// Gets or sets the refresh service.
        /// </summary>
        /// <value>
        /// The refresh service.
        /// </value>
        [Inject]
        public RefreshService? refreshService { get; set; }

        /// <summary>
        /// Gets or sets the logger.
        /// </summary>
        /// <value>
        /// The logger.
        /// </value>
        [Inject]
        public LoggerService? logger { get; set; }

        /// <summary>
        /// Gets or sets the snackbar.
        /// </summary>
        /// <value>
        /// The snackbar.
        /// </value>
        [Inject]
        ISnackbar? Snackbar { get; set; }

        /// <summary>
        /// Gets or sets the configuration service.
        /// </summary>
        /// <value>
        /// The configuration service.
        /// </value>
        [Inject]
        public ConfigService? configService { get; set; }

        /// <summary>
        /// The selected item
        /// </summary>
        private IList<IXmlNode>? selectedItem;

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
        private string ValidateDoubleType(double? value)
        {
            if (!value.HasValue)
            {
                return "Value is required.";
            }

            if (!double.TryParse(value.ToString(), out _))
            {
                return "Value must be a valid integer.";
            }

            return null!; // No validation errors
        }


        /// <summary>
        /// Adds the elements.
        /// </summary>
        async Task AddElements()
        {
            try
            {
                List<string> parameterNames = new();

                List<string> parameterValues = new();

                string arrayName = XPathName?.Split('.').Last() ?? "";

                Parent!.AddChild("", "");


                if (!configService!.isFileUpdated)
                    configService!.isFileUpdated = true;

                if (!configService!.isGenerationModified)
                    configService!.isGenerationModified = true;

                refreshService?.CallRequestRefresh();

            }
            catch (Exception ex)
            {
                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }

        }

        /// <summary>
        /// Deletes the elements.
        /// </summary>
        async Task DeleteElements()
        {
            try
            {
                Parent!.Remove(selectedItem!.First());

                refreshService?.CallRequestRefresh();
            }
            catch (Exception ex)
            {
                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }

        /// <summary>
        /// Handles the text changed.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="newValue">The new value.</param>
        /// <param name="cell">The cell.</param>
        /// <param name="previousValue">The previous value.</param>
        /// <param name="currentValue">The current value.</param>
        /// <param name="isParamNameUpdate">if set to <c>true</c> [is parameter name update].</param>
        async Task HandleTextChanged<T>(T newValue, IXmlNode cell, T? previousValue, T? currentValue, bool isParamNameUpdate = false)
        {
            try
            {
                if (typeof(T) == typeof(string))
                {
                    // Handle string type
                    string? newValueString = newValue as string;
                    if (newValueString != null)
                    {
                        // Value is a valid string, update `currentValue`.
                        currentValue = (T)(object)newValueString;
                        cell.bIsDataUpdate = true;
                    }
                    else
                    {
                        // Value is not valid, revert to `previousValue`.
                        currentValue = previousValue;

                        Snackbar?.Add($"Invalid string value added for {cell.Name}. Reverting to previous value.", Severity.Error);
                    }
                }
                else if (typeof(T) == typeof(int?))
                {
                    // Handle int type
                    if (int.TryParse(newValue!.ToString(), out int parsedValue))
                    {
                        // Value is a valid integer, update `currentValue`.
                        currentValue = (T)(object)parsedValue;
                        cell.bIsDataUpdate = true;
                    }
                    else
                    {
                        // Value is not valid, revert to `previousValue`.
                        currentValue = previousValue;
                        Snackbar?.Add($"Invalid integer value added for {cell.Name}. Reverting to previous value.", Severity.Error);
                    }
                }
                else if (typeof(T) == typeof(double?))
                {
                    // Handle double type
                    if (double.TryParse(newValue!.ToString(), out double parsedValue))
                    {
                        // Value is a valid double, update `currentValue`.
                        currentValue = (T)(object)parsedValue;
                        cell.bIsDataUpdate = true;
                    }
                    else
                    {
                        // Value is not valid, revert to `previousValue`.
                        currentValue = previousValue;
                        Snackbar?.Add($"Invalid decimal value added for {cell.Name}. Reverting to previous value.", Severity.Error);
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
                        cell.bIsDataUpdate = true;
                    }
                    else
                    {
                        // Value is not valid, revert to `previousValue`.
                        currentValue = previousValue;
                        Snackbar?.Add($"Invalid binary value added for {cell.Name}. Reverting to previous value.", Severity.Error);
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
                            cell.bIsDataUpdate = true;
                        }
                        else
                        {
                            // Value is not valid, revert to `previousValue`.
                            currentValue = previousValue;
                            Snackbar?.Add($"Invalid IP value added for {cell.Name}. Reverting to previous value.", Severity.Error);
                        }
                    }
                }
                else
                {
                    // Handle unsupported types or throw an exception.
                    // You may want to add additional handling for other types.
                }

                // Handle the text change here.

                if (isParamNameUpdate)
                {
                    cell.Name = currentValue?.ToString() ?? "";
                    await cell.SetNameAsync(currentValue?.ToString() ?? "");
                }
                else
                {
                    await cell.SetValueAsync(cell.Name, currentValue?.ToString() ?? "");
                }

                if(!configService!.isFileUpdated && cell.bIsDataUpdate)
                    configService!.isFileUpdated = cell.bIsDataUpdate;

                if (!configService!.isGenerationModified && cell.bIsDataUpdate)
                    configService!.isGenerationModified = cell.bIsDataUpdate;

                refreshService!.CallRequestRefresh();

            }
            catch (Exception ex)
            {
                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }

        /// <summary>
        /// Validates the ip.
        /// </summary>
        /// <param name="ip">The ip.</param>
        /// <returns></returns>
        string validateIP(IPAddress ip)
        {
            if (ip is null) return "Valid";

            return "Invalid IP";
        }

        /// <summary>
        /// Method invoked when the component is ready to start, having received its
        /// initial parameters from its parent in the render tree.
        /// Override this method if you will perform an asynchronous operation and
        /// want the component to refresh when that operation is completed.
        /// </summary>
        protected override async Task OnInitializedAsync()
        {
            try
            {
                refreshService!.RefreshRequested += () =>
                {
                    StateHasChanged();
                };
            }
            catch (Exception ex)
            {
                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }


        /// <summary>
        /// Values the changed.
        /// </summary>
        /// <param name="e">The e.</param>
        public void ValueChanged(object e)
        {
            _ = e;

        }
    }
}