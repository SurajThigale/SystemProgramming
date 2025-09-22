/**********************************************************************************************//**
 * @file    UIParser\Models\UIElement.cs.
 *
 * @brief   Implements the element class
 **************************************************************************************************/

using MudBlazor;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace UIParser.Models;


/// <summary>
/// 
/// </summary>
public class UI
{

}

/// <summary>
/// 
/// </summary>
public class UITheme
{
    /// <summary>
    /// Gets or sets the theme.
    // User can specify : Light, Dark or System
    /// </summary>
    /// <value>
    /// The theme.
    /// </value>
    public string Theme { get; set; } = "Light";
}

// Abstract base class for all UI components
/// <summary>
/// Base class for All UI Components
/// </summary>
public abstract class UIComponent
{
    /// <summary>
    /// Gets or sets the name of the type of Component.
    /// </summary>
    /// <value>
    /// The name of the type.
    /// </value>
    public string? TypeName { get; set; }

    /// <summary>
    /// Gets or sets the CSS class.
    /// </summary>
    /// <value>
    /// The class.
    /// </value>
    public string? Class { get; set; }

    /// <summary>
    /// Gets or sets the CSS style.
    /// </summary>
    /// <value>
    /// The style.
    /// </value>
    public string? Style { get; set; }
}

/// <summary>
/// Class for UIElement which will be used in Grid
/// </summary>
/// <seealso cref="UIParser.Models.UIComponent" />
public class UIElement : UIComponent
{
    /// <summary>
    /// Gets or sets the row.
    /// </summary>
    /// <value>
    /// The row.
    /// </value>
    public int Row { get; set; }
    /// <summary>
    /// Gets or sets the column.
    /// </summary>
    /// <value>
    /// The column.
    /// </value>
    public int Column { get; set; }
    /// <summary>
    /// Gets or sets the row span.
    /// </summary>
    /// <value>
    /// The row span.
    /// </value>
    public int RowSpan { get; set; }
    /// <summary>
    /// Gets or sets the column span.
    /// </summary>
    /// <value>
    /// The column span.
    /// </value>
    public int ColumnSpan { get; set; }
    /// <summary>
    /// Gets or sets the title.
    /// </summary>
    /// <value>
    /// The title.
    /// </value>
    public string? Title { get; set; }
    /// <summary>
    /// Gets or sets the components.
    /// </summary>
    /// <value>
    /// The components.
    /// </value>
    public List<UIComponent> Components { get; set; } = new List<UIComponent>();
}

/// <summary>
/// MetaData class for Grid Component
/// </summary>
/// <seealso cref="UIParser.Models.UIComponent" />
public class GridMetaData : UIComponent
{
    /// <summary>
    /// Gets or sets the rows gap.
    /// </summary>
    /// <value>
    /// The rows gap.
    /// </value>
    public string? RowsGap { get; set; }
    /// <summary>
    /// Gets or sets the columns gap.
    /// </summary>
    /// <value>
    /// The columns gap.
    /// </value>
    public string? ColumnsGap { get; set; }

    /// <summary>
    /// Gets or sets the row definition.
    // This will be similar to WPF RowDefinition
    /// </summary>
    /// <value>
    /// The row definition.
    /// </value>
    public List<string> RowDefinition { get; set; } = new();
    /// <summary>
    /// Gets or sets the column definition.
    //  This will be similar to WPF ColumnDefinition
    /// </summary>
    /// <value>
    /// The column definition.
    /// </value>
    public List<string> ColumnDefinition { get; set; } = new();

    /// <summary>
    /// Gets or sets the elements.
    /// </summary>
    /// <value>
    /// The elements.
    /// </value>
    public List<UIElement> Elements { get; set; } = new List<UIElement>();

}

/// <summary>
/// MetaData for ButtonGroup components
/// </summary>
/// <seealso cref="UIParser.Models.UIComponent" />
public class ButtonGroupMetaData : UIComponent
{
    /// <summary>
    /// Gets or sets the buttons.
    /// </summary>
    /// <value>
    /// The buttons.
    /// </value>
    public List<ButtonMetaData> Buttons { get; set; } = new();
}

// Specific derived class for buttons
/// <summary>
/// MetaData for Button components
/// </summary>
/// <seealso cref="UIParser.Models.UIComponent" />
public class ButtonMetaData : UIComponent
{
    /// <summary>
    /// Gets or sets the name.
    /// </summary>
    /// <value>
    /// The name.
    /// </value>
    public string? Name { get; set; }
}

/// <summary>
/// MetaData for UIColumn in DataGrid components
/// </summary>
/// <seealso cref="UIParser.Models.UIComponent" />
public class UIColumn : UIComponent
{
    /// <summary>
    /// Gets or sets the name.
    /// </summary>
    /// <value>
    /// The name.
    /// </value>
    public string? Name { get; set; }

    /// <summary>
    /// Gets or sets the title.
    /// </summary>
    /// <value>
    /// The title.
    /// </value>
    public string? Title { get; set; }

    /// <summary>
    /// Gets or sets the width.
    /// </summary>
    /// <value>
    /// The width.
    /// </value>
    public string? Width { get; set; }  
}

/// <summary>
/// MetaData for PropertyColumn components which will be used in DataGrid
/// </summary>
/// <seealso cref="UIParser.Models.UIColumn" />
public class PropertyColumn : UIColumn
{
    /// <summary>
    /// Gets or sets the type.
    /// </summary>
    /// <value>
    /// The type.
    /// </value>
    public string? Type { get; set; }

    /// <summary>
    /// Gets or sets the is automatic increment.
    /// </summary>
    /// <value>
    /// The is automatic increment.
    /// </value>
    public bool? isAutoIncrement { get; set; }
}

/// <summary>
/// MetaData for ComboBoxColumn components which will be used in DataGrid
/// </summary>
/// <seealso cref="UIParser.Models.UIColumn" />
public class ComboBoxColumn : UIColumn
{
    /// <summary>
    /// Gets or sets the type.
    /// </summary>
    /// <value>
    /// The type.
    /// </value>
    public string? Type { get; set; }
}

// Specific derived class for tables
/// <summary>
/// 
/// </summary>
/// <seealso cref="UIParser.Models.UIComponent" />
public class DataGridMetaData : UIComponent
{
    // Add properties specific to tables here
    /// <summary>
    /// Gets or sets the columns.
    /// </summary>
    /// <value>
    /// The columns.
    /// </value>
    public List<UIColumn> Columns { get; set; } = new();

    /// <summary>
    /// Defines the XML XPath used to retrieve and bind elements within the DataGrid.
    /// Users can specify XPath as an array, such as Items="iocs.WellTemp[0]". In this case, only
    /// the array at the 0th index will be considered in the DataGrid.
    /// </summary>
    /// <value>
    /// The XML-bound XPath.
    /// </value>
    public string? Items { get; internal set; }

    /// <summary>
    /// Gets the limit.
    /// </summary>
    /// <value>
    /// The limit.
    /// </value>
    public string? Limit { get; internal set; }
    /// <summary>
    /// Gets the description.
    /// </summary>
    /// <value>
    /// The description.
    /// </value>
    public string? Description { get; internal set; }
    /// <summary>
    /// Gets the show add delete buttons.
    /// </summary>
    /// <value>
    /// The show add delete buttons.
    /// </value>
    public bool? ShowAddDeleteButtons {  get; internal set; }

    /// <summary>
    /// Gets the buttons CSS style.
    /// </summary>
    /// <value>
    /// The buttons style.
    /// </value>
    public string? ButtonsStyle { get; internal set; }

    /// <summary>
    /// Gets the buttons CSS class.
    /// </summary>
    /// <value>
    /// The buttons class.
    /// </value>
    public string? ButtonsClass { get; internal set; }
}

/// <summary>
/// 
/// </summary>
/// <seealso cref="UIParser.Models.UIComponent" />
public class NameValuePairGridMetaData : UIComponent
{
    // Add properties specific to tables here
    /// <summary>
    /// Gets the items.
    /// </summary>
    /// <value>
    /// The items.
    /// </value>
    public string? Items { get; internal set; }
    /// <summary>
    /// Gets or sets the type.
    /// </summary>
    /// <value>
    /// The type.
    /// </value>
    public string? Type { get; set; }
    /// <summary>
    /// Gets the show add delete buttons.
    /// </summary>
    /// <value>
    /// The show add delete buttons.
    /// </value>
    public bool? ShowAddDeleteButtons { get; internal set; }
    /// <summary>
    /// Gets the buttons style.
    /// </summary>
    /// <value>
    /// The buttons style.
    /// </value>
    public string? ButtonsStyle { get; internal set; }
    /// <summary>
    /// Gets the buttons class.
    /// </summary>
    /// <value>
    /// The buttons class.
    /// </value>
    public string? ButtonsClass { get; internal set; }
    /// <summary>
    /// Gets the description.
    /// </summary>
    /// <value>
    /// The description.
    /// </value>
    public string? Description { get; internal set; }
}

// Specific derived class for text edits
/// <summary>
/// 
/// </summary>
/// <seealso cref="UIParser.Models.UIComponent" />
public class TextEditMetaData : UIComponent
{
    /// <summary>
    /// Gets or sets the name.
    /// </summary>
    /// <value>
    /// The name.
    /// </value>
    public string? Name { get; set; }
    /// <summary>
    /// Gets or sets the remark.
    /// </summary>
    /// <value>
    /// The remark.
    /// </value>
    public string? Remark { get; set; }
    /// <summary>
    /// Gets or sets the value.
    /// </summary>
    /// <value>
    /// The value.
    /// </value>
    public string? Value { get; set; }
    /// <summary>
    /// Gets or sets the type.
    /// </summary>
    /// <value>
    /// The type.
    /// </value>
    public string? Type { get; set; }
}

/// <summary>
/// 
/// </summary>
/// <seealso cref="UIParser.Models.UIComponent" />
public class TabItem : UIComponent
{
    /// <summary>
    /// Gets or sets the source.
    /// </summary>
    /// <value>
    /// The source.
    /// </value>
    public string? Source { get; set; }

    /// <summary>
    /// Gets or sets the name.
    /// </summary>
    /// <value>
    /// The name.
    /// </value>
    public string? Name { get; set; }
}

/// <summary>
/// 
/// </summary>
/// <seealso cref="UIParser.Models.UIComponent" />
public class TabMetaData : UIComponent
{
    /// <summary>
    /// Gets or sets the name.
    /// </summary>
    /// <value>
    /// The name.
    /// </value>
    public string? Name { get; set; }

    /// <summary>
    /// Gets or sets the tabs.
    /// </summary>
    /// <value>
    /// The tabs.
    /// </value>
    public List<TabItem> Tabs { get; set; } = new();
}

/// <summary>
/// 
/// </summary>
/// <seealso cref="UIParser.Models.UIComponent" />
public class SLConfigMetaData : UIComponent
{

}

/// <summary>
/// 
/// </summary>
/// <seealso cref="UIParser.Models.UIComponent" />
public class HeaderFileGenMetaData : UIComponent
{

}