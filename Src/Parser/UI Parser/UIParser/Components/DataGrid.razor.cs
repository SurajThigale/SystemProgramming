using ConfigParser.Model.ParserMetaData;
using ConfigParser.Services;
using Excubo.Generators.Blazor.ExperimentalDoNotUseYet;
using global::Microsoft.AspNetCore.Components;
using MudBlazor;
using System;
using System.Net;
using System.Reflection.Metadata;
using System.Xml.Linq;
using UIParser.Models;
using UIParser.Services;
using static MudBlazor.Colors;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace UIParser.Components
{
    /// <summary>
    /// 
    /// </summary>
    /// <seealso cref="Microsoft.AspNetCore.Components.ComponentBase" />
    public partial class DataGrid
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
        /// Gets or sets the columns.
        /// </summary>
        /// <value>
        /// The columns.
        /// </value>
        [Parameter]
        public List<UIColumn>? Columns { get; set; }

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
        /// Gets or sets the name of the x path.
        /// </summary>
        /// <value>
        /// The name of the x path.
        /// </value>
        [Parameter]
        public string? XPathName { get; set; }

        /// <summary>
        /// Gets or sets the parent.
        /// </summary>
        /// <value>
        /// The parent.
        /// </value>
        [Parameter]
        public IXmlNode? Parent { get; set; }

        /// <summary>
        /// Gets or sets the description.
        /// </summary>
        /// <value>
        /// The description.
        /// </value>
        [Parameter]
        public string? Description { get; set; }

        /// <summary>
        /// Gets or sets the limit.
        /// </summary>
        /// <value>
        /// The limit.
        /// </value>
        [Parameter]
        public int? Limit { get; set; }

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
        /// Gets or sets the snackbar.
        /// </summary>
        /// <value>
        /// The snackbar.
        /// </value>
        [Inject]
        ISnackbar? Snackbar { get; set; }

        /// <summary>
        /// Gets or sets the ComboBox list items.
        /// </summary>
        /// <value>
        /// The ComboBox list items.
        /// </value>
        public List<string> ComboBoxListItems { get; set; } = new();

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
                return "Value must be a valid double.";
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
                IXmlNode? item = Items?.LastOrDefault();

                bool bCheckLimit = false;
                if (Limit is not null && Limit != -1)
                {
                    bCheckLimit = true;
                }

                List<string> parameterNames = new();

                List<string> parameterValues = new();

                foreach (var column in Columns!)
                {
                    parameterNames.Add(column.Name!);

                    if (column is PropertyColumn proColumn)
                    {
                        if (proColumn.isAutoIncrement ?? false)
                        {
                            int? currentValue = int.TryParse(item?[column.Name!].ToString(), out int parsedCurrentValue) ? (int?)parsedCurrentValue : null;

                            if (currentValue != null)
                            {
                                parameterValues.Add((++currentValue).ToString() ?? "1");
                            }
                            else
                            {
                                parameterValues.Add("1");
                            }
                        }
                        else
                        {
                            parameterValues.Add("0");
                        }
                    }
                    else
                    {
                        parameterValues.Add("0");
                    }
                }

                string arrayName = XPathName?.Split('.').Last() ?? "";

                if (arrayName != "")
                {
                    if (bCheckLimit)
                    {
                        var currentCount = Items?.Count;
                        if (currentCount >= Limit)
                        {
                            Snackbar?.Add($"Unable to add a new element as it exceeds the limit of {Limit}.", Severity.Error);

                            StateHasChanged();

                            return;
                        }
                    }

                    Parent!.AddArrayElementToNestedNode(Parent.Name, arrayName, parameterNames.ToArray(), parameterValues.ToArray());

                    if (!configService!.isFileUpdated)
                        configService!.isFileUpdated = true;

                    if (!configService!.isGenerationModified)
                        configService!.isGenerationModified = true;

                    CheckIdSequence(arrayName);
                    refreshService?.CallRequestRefresh();
                }

                StateHasChanged();
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
                string arrayName = XPathName?.Split('.').Last() ?? "";

                if (arrayName != "")
                {
                    Parent!.RemoveById(selectedItem!.First());

                    if (!configService!.isFileUpdated)
                        configService!.isFileUpdated = true;

                    if (!configService!.isGenerationModified)
                        configService!.isGenerationModified = true;

                    CheckIdSequence(arrayName);
                    refreshService?.CallRequestRefresh();
                }
            }
            catch (Exception ex)
            {
                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }

        void CheckIdSequence(string arrayName)
        {
            var id = 1;
            foreach (var child in Parent!.GetChildren())
            {
                if (child.Name == arrayName)
                {
                    var idNode = child.GetChildren().FirstOrDefault(c => c.Name == "Id");
                    if (idNode?.Value?.Value != id++.ToString())
                    {
                        Snackbar?.Add($"Id not in sequence.", Severity.Error);
                        StateHasChanged();
                        break;
                    }
                }
            }
        }

        /// <summary>
        /// Handles the text changed.
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="columnName">Name of the column.</param>
        /// <param name="newValue">The new value.</param>
        /// <param name="cell">The cell.</param>
        /// <param name="previousValue">The previous value.</param>
        /// <param name="currentValue">The current value.</param>
        async Task HandleTextChanged<T>(string columnName, T newValue, IXmlNode cell, T? previousValue, T? currentValue)
        {
            try
            {
                // Get the type without using typeof on nullable reference types
                Type nonNullableType = Nullable.GetUnderlyingType(typeof(T)) ?? typeof(T);

                if (nonNullableType == typeof(string))
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

                        Snackbar?.Add($"Invalid string value added for {columnName}. Reverting to previous value.", Severity.Error);

                        StateHasChanged();
                    }
                }
                else if (nonNullableType == typeof(int))
                {
                    // Handle int type
                    if (int.TryParse(newValue?.ToString(), out int parsedValue))
                    {
                        // Value is a valid integer, update `currentValue`.
                        currentValue = (T)(object)parsedValue;
                        cell.bIsDataUpdate = true;
                    }
                    else
                    {
                        // Value is not valid, revert to `previousValue`.
                        currentValue = previousValue;
                        Snackbar?.Add($"Invalid integer value added for {columnName}. Reverting to previous value.", Severity.Error);
                    }
                }
                else if (nonNullableType == typeof(double))
                {
                    // Handle double type
                    if (double.TryParse(newValue?.ToString(), out double parsedValue))
                    {
                        // Value is a valid double, update `currentValue`.
                        currentValue = (T)(object)parsedValue;
                        cell.bIsDataUpdate = true;
                    }
                    else
                    {
                        // Value is not valid, revert to `previousValue`.
                        currentValue = previousValue;
                        Snackbar?.Add($"Invalid decimal value added for {columnName}. Reverting to previous value.", Severity.Error);
                    }
                }
                else if (nonNullableType == typeof(Binary))
                {
                    // Handle binary type
                    Binary newBinValue = new Binary(newValue?.ToString() ?? "Invalid");
                    if (newBinValue.IsValid)
                    {
                        // Value is a valid binary, update `currentValue`.
                        currentValue = (T)(object)newBinValue;
                        cell.bIsDataUpdate = true;
                    }
                    else
                    {
                        // Value is not valid, revert to `previousValue`.
                        currentValue = previousValue;
                        Snackbar?.Add($"Invalid binary value added for {columnName}. Reverting to previous value.", Severity.Error);
                    }
                }
                else if (nonNullableType == typeof(IPAddress))
                {
                    // Handle IPAddress type
                    if (newValue != null && IPAddress.TryParse(newValue.ToString(), out IPAddress? parsedValue))
                    {
                        // Value is a valid IP address, update `currentValue`.
                        currentValue = (T)(object)parsedValue;
                        cell.bIsDataUpdate = true;
                    }
                    else if (newValue == null)
                    {
                        // Value is null, revert to `previousValue`.
                        currentValue = previousValue;
                        Snackbar?.Add($"IP address cannot be null for {columnName}. Reverting to previous value.", Severity.Error);
                    }
                    else
                    {
                        // Value is not valid, revert to `previousValue`.
                        currentValue = previousValue;
                        Snackbar?.Add($"Invalid IP address for {columnName}. Reverting to previous value.", Severity.Error);
                    }
                }
                else
                {
                    // Handle unsupported types or throw an exception.
                    // You may want to add additional handling for other types.
                }

                if (!configService!.isFileUpdated && cell.bIsDataUpdate)
                    configService!.isFileUpdated = cell.bIsDataUpdate;

                if (!configService!.isGenerationModified && cell.bIsDataUpdate)
                    configService!.isGenerationModified = cell.bIsDataUpdate;

                // Handle the text change here.
                var column = Columns!.First(c => c.Name == columnName);

                refreshService!.CallRequestRefresh();

                if (nonNullableType == typeof(Binary))
                {
                    Binary? bin = currentValue as Binary;
                    await cell.SetValueAsync(columnName, bin?.ToDecimal().ToString() ?? "");
                }
                else
                {
                    await cell.SetValueAsync(columnName, currentValue?.ToString() ?? "");
                }

                string arrayName = XPathName?.Split('.').Last() ?? "";
                if (arrayName != "")
                    CheckIdSequence(arrayName);
            }
            catch (Exception ex)
            {
                await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
        }

        /// <summary>
        /// Validates the IP.
        /// </summary>
        /// <param name="ip">The IP.</param>
        /// <returns>String indicating validation result</returns>
        string validateIP(IPAddress ip)
        {
            if (ip is null) return "IP address cannot be null";

            try
            {
                // Additional validation if needed
                return string.Empty; // Valid IP address
            }
            catch
            {
                return "Invalid IP address format";
            }
        }

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

        /// <summary>
        /// Searches for ComboBox items.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns></returns>
        public async Task<IEnumerable<string>> SearchForComboBoxItems(string value)
        {
            await Task.Delay(0);
            if (string.IsNullOrEmpty(value))
            {
                return ComboBoxListItems.Distinct();
            }

            return ComboBoxListItems.Distinct().Where(x => x.Contains(value, StringComparison.InvariantCultureIgnoreCase));
        }
    }
}
