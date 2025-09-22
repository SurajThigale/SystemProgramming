using System.Xml.Linq;
using UIParser.Models;

namespace UIParser.Services
{
    public class XmlCompatibilityCheckerService
    {
        public async Task<string[]> CheckXmlBindings(XDocument xmlDocument)
        {
            await Task.Delay(0);

            // Extract all attribute names from the XML
            var xmlAttributes = xmlDocument.Descendants("parameter")
              .Select(param => GetFullAttributeName(param));

            // Extract all XPaths used in UI components
            var uiXPaths = ExtractXPathsFromUI();

            List<string> errors = new();

            // Check for missing attribute bindings in the XML
            foreach (var attributeName in xmlAttributes)
            {
                if (!string.IsNullOrEmpty(attributeName) &&
                    !uiXPaths.Contains(attributeName) && !attributeName.Contains("config.sl-config"))
                {
                    errors.Add(attributeName);
                }
            }

            // Handle Special condition where we have added "*" after xpath. 
            string wildcard = "*";

            List<string> filteredArray2 = new List<string>();

            foreach (string xpath1 in uiXPaths)
            {
                if (xpath1.Contains(wildcard))
                {
                    // If the element in array1 contains "*", ignore matching elements in array2
                    string prefix = xpath1.Substring(0, xpath1.IndexOf(wildcard));
                    errors.RemoveAll(xpath2 => xpath2.StartsWith(prefix));
                }
            }

            return errors.Distinct().ToArray();
        }

        private string GetFullAttributeName(XElement parameterElement)
        {
            // Build the full attribute name including parent elements
            string attributeName = parameterElement.Attribute("name")?.Value ?? "";
            if (attributeName != null)
            {
                XElement parent = parameterElement.Parent!;
                while (parent != null)
                {
                    attributeName = parent.Name.LocalName + "." + attributeName;
                    parent = parent.Parent!;
                }
            }
            return attributeName!;
        }

        private List<string> ExtractXPathsFromUI()
        {
            return ParserHelper.xPathArray;
        }
    }
}
