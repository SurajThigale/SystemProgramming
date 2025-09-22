using ConfigParser.Services;
using Microsoft.AspNetCore.Components;
using MudBlazor;
using Radzen;
using Radzen.Blazor;
using System.Text.Json;

namespace UIParser.Components.Dialogs
{
    public partial class CHeadersDialog
    {
        [CascadingParameter]
        MudDialogInstance? MudDialog { get; set; }

        [Parameter]
        public string? Title { get; set; }

        [Parameter]
        public JsonElement? JsonData { get; set; }

        [Parameter]
        public Color Color { get; set; }

        RadzenDataGrid<JsonElementRow>? grid;

        public class JsonElementRow
        {
            public string? Property { get; set; }
            public string? DisplayName { get; set; }
            public JsonElement? Value { get; set; }
            public bool HasChild => Children != null && Children.Count > 0;

            public List<JsonElementRow> Children { get; set; } = new List<JsonElementRow>();
        }

        void RowRender(RowRenderEventArgs<JsonElementRow> args)
        {
            args.Expandable = args.Data.Children.Any();
        }

        void LoadChildData(DataGridLoadChildDataEventArgs<JsonElementRow> args)
        {
            args.Data = args.Item.Children;
        }

        protected override async Task OnAfterRenderAsync(bool firstRender)
        {
            await base.OnAfterRenderAsync(firstRender);

            if (firstRender)
            {
                await grid!.ExpandRow(data!.FirstOrDefault()!);
            }
        }

        private readonly object dataLock = new object();
        private List<JsonElementRow>? data = new List<JsonElementRow>();

        protected override async Task OnInitializedAsync()
        {
            try
            {
                await FlattenJsonAsync(JsonData, data!);
            }
            catch
            {
                Console.WriteLine("Error in Json Parsing");
            }
        }

        private async Task FlattenJsonAsync(JsonElement? element, List<JsonElementRow> rows, string prefix = "")
        {
            try
            {
                await Task.Run(() => FlattenJson(element, rows, prefix));
            }
            catch
            {
                Console.WriteLine("Error in Json Parsing");
            }
        }

        private void FlattenJson(JsonElement? element, List<JsonElementRow> rows, string prefix = "")
        {
            try
            {
                lock (dataLock)
                {
                    switch (element?.ValueKind)
                    {
                        case JsonValueKind.Object:
                            foreach (var property in element?.EnumerateObject()!)
                            {                              
                                var propertyName = String.IsNullOrEmpty(prefix) ? property.Name : $"{prefix}.{property.Name}";
                                var displayName = property.Name;
                                var row = new JsonElementRow { Property = propertyName, DisplayName=displayName, Value = property.Value };
                                FlattenJson(property.Value, row.Children, propertyName);
                                rows.Add(row);
                            }
                            break;

                        case JsonValueKind.Array:
                            for (var i = 0; i < element?.GetArrayLength(); i++)
                            {
                                var arrayElement = element?[i];
                                var arrayPropertyName = $"{prefix}[{i}]";
                                var displayName = $"[{i}]";
                                var arrayRow = new JsonElementRow { Property = arrayPropertyName, DisplayName = displayName, Value = arrayElement };
                                FlattenJson(arrayElement, arrayRow.Children, arrayPropertyName);
                                rows.Add(arrayRow);
                            }
                            break;

                        default:
                            //rows.Add(new JsonElementRow { Property = prefix, Value = element });
                            break;
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Exception in FlattenJson: {ex.Message}");
            }      
        }

        void Submit() => MudDialog?.Close(DialogResult.Ok(true));
    }
}
