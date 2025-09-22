using global::System;
using global::System.Collections.Generic;
using global::System.Linq;
using global::System.Threading.Tasks;
using global::Microsoft.AspNetCore.Components;
using Microsoft.AspNetCore.Components.Web;
using MudBlazor;
using UIParser.Models;

namespace UIParser.Components
{
    /// <summary>
    /// 
    /// </summary>
    /// <seealso cref="Microsoft.AspNetCore.Components.ComponentBase" />
    public partial class ComboButton
    {
        /// <summary>
        /// Gets or sets the main button label.
        /// </summary>
        /// <value>
        /// The main button label.
        /// </value>
        [Parameter]
        public string? MainButtonLabel { get; set; }

        /// <summary>
        /// Gets or sets the options.
        /// </summary>
        /// <value>
        /// The options.
        /// </value>
        [Parameter]
        public List<string>? Options { get; set; }

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
        /// The show options
        /// </summary>
        private bool showOptions;
        /// <summary>
        /// Toggles the options.
        /// </summary>
        private void ToggleOptions()
        {
            showOptions = !showOptions;
        }
    }
}