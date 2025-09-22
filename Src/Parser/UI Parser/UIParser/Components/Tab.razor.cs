using global::System;
using global::System.Collections.Generic;
using global::System.Linq;
using global::System.Threading.Tasks;
using global::Microsoft.AspNetCore.Components;
using Microsoft.AspNetCore.Components.Web;
using MudBlazor;
using UIParser.Models;
using UIParser;

namespace UIParser.Components
{
    /// <summary>
    /// 
    /// </summary>
    /// <seealso cref="Microsoft.AspNetCore.Components.ComponentBase" />
    public partial class Tab
    {
        /// <summary>
        /// The tabs
        /// </summary>
        MudTabs? tabs;

        /// <summary>
        /// Activates all tabs.
        /// </summary>
        public void ActivateAllTabs()
        {
            int tabCount = 1;

            foreach (var items in TabItems!)
            {
                tabs?.ActivatePanel(tabCount);
                tabCount++;
            }

            tabs?.ActivatePanel(0);
        }

        /// <summary>
        /// Gets or sets the tab items.
        /// </summary>
        /// <value>
        /// The tab items.
        /// </value>
        [Parameter]
        public List<TabItem>? TabItems { get; set; }

        /// <summary>
        /// Gets or sets the name.
        /// </summary>
        /// <value>
        /// The name.
        /// </value>
        [Parameter]
        public string? Name { get; set; }

        /// <summary>
        /// Gets or sets the class.
        /// </summary>
        /// <value>
        /// The class.
        /// </value>
        [Parameter]
        public string Class { get; set; } = "pa-6";

        /// <summary>
        /// Gets or sets the style.
        /// </summary>
        /// <value>
        /// The style.
        /// </value>
        [Parameter]
        public string? Style { get; set; }
    }
}