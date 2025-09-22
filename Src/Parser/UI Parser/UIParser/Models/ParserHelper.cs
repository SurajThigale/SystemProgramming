using System.Xml;
using System.Collections.Generic;
using System.IO;
using System.Xml.Linq;
using UIParser.Components;
using static MudBlazor.CategoryTypes;

namespace UIParser.Models;


public static class ParserHelper
{
    public static string? uiRootPath { get; set; }

    public static string uiTheme { get; set; } = string.Empty;

    public static List<string> xPathArray { get; set; } = new();

    public static async Task<GridMetaData> ParseXml(string? xmlContent, string uiFolderPath)
    {
        if (xmlContent == null) return null!;

        uiRootPath = uiFolderPath ?? string.Empty;
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(xmlContent);
        return await ParseElement(doc!.DocumentElement!);
    }

    private static async Task<GridMetaData> ParseElement(XmlElement? element)
    {

        XmlNode themeNode = element!.SelectSingleNode("UITheme")!;

        if (themeNode is not null)
        {
            uiTheme = themeNode?.Attributes?["Type"]?.Value ?? "";
        }



        GridMetaData gridRoot = new GridMetaData();

        XmlNode gridNode = element?.SelectSingleNode("Grid")!;

        if (gridNode is null)
        {
            gridNode = element!.ParentNode!;
        }

        gridRoot.RowsGap = gridNode.Attributes!["RowGap"]?.Value;
        gridRoot.ColumnsGap = gridNode.Attributes!["ColumnGap"]?.Value;
        gridRoot.Class = gridNode.Attributes["Class"]?.Value;
        gridRoot.Style = gridNode.Attributes["Style"]?.Value;

        XmlNodeList rowsDefinitionNode = gridNode!.SelectNodes("RowDefinition")!;
        foreach (XmlElement elementNode in rowsDefinitionNode)
        {
            gridRoot.RowDefinition.Add(elementNode.GetAttribute("Height"));
        }

        XmlNodeList columnDefinitionNode = gridNode.SelectNodes("ColumnDefinition")!;
        foreach (XmlElement elementNode in columnDefinitionNode)
        {
            gridRoot.ColumnDefinition.Add(elementNode.GetAttribute("Width"));
        }


        // Get all "Element" nodes
        XmlNodeList elementNodes = element?.SelectNodes("Grid/Element")!;

        if (elementNodes.Count == 0)
            elementNodes = element?.SelectNodes("Element")!;

        foreach (XmlElement elementNode in elementNodes)
        {
            UIElement uiElement = new UIElement();

            int row = int.Parse(elementNode.GetAttribute("Row"));
            int column = int.Parse(elementNode.GetAttribute("Column"));

            string spanString = elementNode.Attributes["RowSpan"]?.Value ?? "0";
            int rowSpan = int.Parse(spanString);

            spanString = elementNode.Attributes["ColumnSpan"]?.Value ?? "0";
            int columnSpan = int.Parse(spanString);

            uiElement.Class = elementNode.Attributes["Class"]?.Value;
            uiElement.Style = elementNode.Attributes["Style"]?.Value;
            uiElement.Row = row;
            uiElement.Column = column;

            uiElement.RowSpan = rowSpan;
            uiElement.ColumnSpan = columnSpan;
            uiElement.Title = elementNode.Attributes["Title"]?.Value;

            // Get the child components within the Element
            XmlNodeList componentNodes = elementNode.ChildNodes;
            foreach (XmlNode componentNode in componentNodes)
            {
                string classAttribute = componentNode.Attributes!["Class"]?.Value ?? String.Empty;
                string styleAttribute = componentNode.Attributes!["Style"]?.Value ?? String.Empty;
                switch (componentNode.Name)
                {
                    case "ButtonGroup":
                        ButtonGroupMetaData buttonGroup = new ButtonGroupMetaData();
                        // Process ButtonGroup and add Buttons to the UIComponents list
                        XmlNodeList buttonNodes = componentNode.SelectNodes("Button")!;
                        buttonGroup.TypeName = "ButtonGroup";
                        buttonGroup.Class = classAttribute;
                        buttonGroup.Style = styleAttribute;
                        foreach (XmlElement buttonNode in buttonNodes)
                        {
                            buttonGroup.Buttons.Add(new ButtonMetaData
                            {
                                TypeName = "Button",
                                Name = buttonNode.GetAttribute("Name"),
                                Class = buttonNode.GetAttribute("Class"),
                            });
                        }
                        uiElement.Components.Add(buttonGroup);
                        break;
                    case "Button":
                        var nameAttribute = componentNode.Attributes["Name"];
                        var button = new ButtonMetaData
                        {
                            TypeName = "Button",
                            Name = nameAttribute?.Value,
                            Class = classAttribute,
                            Style = styleAttribute,
                        };

                        uiElement.Components.Add(button);
                        break;
                    case "DataGrid":
                        {
                            // Process Table and add Table to the UIComponents list
                            // Add table-specific properties as needed
                            DataGridMetaData dataGridMetaData = new DataGridMetaData();
                            var items = componentNode.Attributes["Items"];
                            dataGridMetaData.Items = items?.Value!;
                            dataGridMetaData.TypeName = "DataGrid";
                            dataGridMetaData.Class = classAttribute;
                            dataGridMetaData.Style = styleAttribute;
                            dataGridMetaData.Limit = componentNode.Attributes["Limit"]?.Value ?? "-1";
                            dataGridMetaData.Description = componentNode.Attributes["Description"]?.Value ?? "";
                            dataGridMetaData.ButtonsStyle = componentNode.Attributes["ButtonsStyle"]?.Value ?? "";
                            dataGridMetaData.ButtonsClass = componentNode.Attributes["ButtonsClass"]?.Value ?? "";

                            string showAddDeleteButtonsStr = componentNode.Attributes["ShowAddDeleteButtons"]?.Value ?? "true";
                            bool.TryParse(showAddDeleteButtonsStr, out bool showAddDeleteButtons);
                            dataGridMetaData.ShowAddDeleteButtons = showAddDeleteButtons;

                            xPathArray.Add("config." + items?.Value);

                            XmlNodeList columnsNodes = componentNode.SelectNodes("Columns")!;

                            if (columnsNodes != null && columnsNodes.Count > 0)
                            {
                                foreach (XmlNode columnNode in columnsNodes?[0]!.ChildNodes!)
                                {
                                    XmlElement columnElement = (XmlElement)columnNode;
                                    string columnClassAttribute = columnElement.Attributes["Class"]?.Value ?? String.Empty;
                                    string columnStyleAttribute = columnElement.Attributes["Style"]?.Value ?? String.Empty;

                                    var typeName = columnElement.GetAttribute("Type") ?? String.Empty;

                                    if (String.IsNullOrEmpty(typeName))
                                    {
                                        typeName = "int";
                                    }

                                    bool columnAutoIncrement = false;
                                    if (String.Compare(typeName, "int", StringComparison.OrdinalIgnoreCase) == 0)
                                    {
                                        string columnAutoIncrementAttribute = columnElement.Attributes["AutoIncrement"]?.Value ?? String.Empty;

                                        if (String.Compare(columnAutoIncrementAttribute, "true", StringComparison.OrdinalIgnoreCase) == 0)
                                        {
                                            columnAutoIncrement = true;
                                        }
                                    }

                                    string elementName = columnElement.Name;

                                    if (elementName == "PropertyColumn")
                                    {
                                        dataGridMetaData.Columns.Add(new PropertyColumn()
                                        {
                                            Name = columnElement.GetAttribute("Name"),
                                            Title = columnElement.GetAttribute("Title"),
                                            Type = typeName,
                                            Class = columnClassAttribute,
                                            Style = columnStyleAttribute,
                                            Width = columnElement.GetAttribute("Width"),
                                            isAutoIncrement = columnAutoIncrement,
                                        });

                                    }
                                    else if (elementName == "ComboBoxColumn")
                                    {
                                        dataGridMetaData.Columns.Add(new ComboBoxColumn()
                                        {
                                            Name = columnElement.GetAttribute("Name"),
                                            Title = columnElement.GetAttribute("Title"),
                                            Type = typeName,
                                            Class = columnClassAttribute,
                                            Style = columnStyleAttribute,
                                            Width = columnElement.GetAttribute("Width"),
                                        });
                                    }


                                    xPathArray.Add("config." + items?.Value + "." + columnElement.GetAttribute("Name"));
                                }
                            }


                            uiElement.Components.Add(dataGridMetaData);
                            break;
                        }
                    case "NameValuePairGrid":
                        {
                            // Process Table and add Table to the UIComponents list
                            // Add table-specific properties as needed
                            NameValuePairGridMetaData nameValueGridData = new NameValuePairGridMetaData();
                            var itemsSet = componentNode.Attributes["Items"];
                            nameValueGridData.Items = itemsSet?.Value!;
                            nameValueGridData.TypeName = "NameValuePairGrid";
                            nameValueGridData.Class = classAttribute;
                            nameValueGridData.Style = styleAttribute;
                            nameValueGridData.Type = componentNode.Attributes["Type"]?.Value ?? "int";
                            nameValueGridData.Description = componentNode.Attributes["Description"]?.Value ?? "";
                            nameValueGridData.ButtonsStyle = componentNode.Attributes["ButtonsStyle"]?.Value ?? "";
                            nameValueGridData.ButtonsClass = componentNode.Attributes["ButtonsClass"]?.Value ?? "";

                            string showAddDeleteButtonsStr = componentNode.Attributes["ShowAddDeleteButtons"]?.Value ?? "true";
                            bool.TryParse(showAddDeleteButtonsStr, out bool showAddDeleteButtons);
                            nameValueGridData.ShowAddDeleteButtons = showAddDeleteButtons;

                            //As we are showing all the properties from this set, so xpath is added as "*"
                            //This special condition will be handled while finding mismatched entries
                            xPathArray.Add("config." + itemsSet?.Value + ".*");

                            uiElement.Components.Add(nameValueGridData);
                            break;
                        }

                    case "TextEdit":
                        ArgumentNullException.ThrowIfNull(componentNode);
                        // Process TextEdit and add TextEdit to the UIComponents list   
                        nameAttribute = componentNode.Attributes["Name"];

                        var remarkAttribute = componentNode.Attributes["Remark"];

                        var valueAttribute = componentNode.Attributes["Value"];

                        var type = componentNode.Attributes["Type"]?.Value ?? String.Empty;

                        if (String.IsNullOrEmpty(type))
                        {
                            type = "double";
                        }


                        var textEdit = new TextEditMetaData
                        {
                            TypeName = "TextEdit",
                            Name = nameAttribute?.Value ?? "",
                            Remark = remarkAttribute?.Value ?? "",
                            Value = valueAttribute?.Value,
                            Class = classAttribute,
                            Style = styleAttribute,
                            Type = type
                        };

                        xPathArray.Add("config." + textEdit.Value);

                        uiElement.Components.Add(textEdit);
                        break;

                    case "Tab":

                        TabMetaData tabMetaData = new TabMetaData();

                        nameAttribute = componentNode.Attributes["Name"];

                        tabMetaData.TypeName = "Tab";
                        tabMetaData.Class = classAttribute;
                        tabMetaData.Style = styleAttribute;

                        XmlNodeList tabItemsNodes = componentNode.SelectNodes("TabItem")!;
                        foreach (XmlElement tabNode in tabItemsNodes)
                        {
                            tabMetaData.Tabs.Add(new TabItem()
                            {
                                Name = tabNode.GetAttribute("Name"),
                                Source = tabNode.GetAttribute("Source"),
                                Class = tabNode.GetAttribute("Class"),
                                Style = tabNode.GetAttribute("Style")
                            });
                        }

                        uiElement.Components.Add(tabMetaData);

                        break;

                    case "Grid":

                        if (componentNode is null) continue;

                        var xmlEle = await ParseElement(componentNode as XmlElement);
                        xmlEle.TypeName = "Grid";

                        uiElement.Components.Add(xmlEle);

                        break;
                    case "HeaderFileGeneration":

                        if (componentNode is null) continue;

                        var headerFileGen = new HeaderFileGenMetaData
                        {
                            TypeName = "HeaderFileGeneration",
                            Class = classAttribute,
                            Style = styleAttribute,
                        };

                        uiElement.Components.Add(headerFileGen);

                        break;
                    case "SLConfig":

                        if (componentNode is null) continue;

                        var slconfig = new SLConfigMetaData
                        {
                            TypeName = "SLConfig",
                            Class = classAttribute,
                            Style = styleAttribute,
                        };

                        uiElement.Components.Add(slconfig);

                        break;

                    default:
                        break;
                }
            }

            gridRoot.Elements.Add(uiElement);
        }

        return gridRoot;
    }
}
