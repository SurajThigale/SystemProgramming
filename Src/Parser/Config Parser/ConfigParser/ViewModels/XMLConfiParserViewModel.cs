using ConfigParser.Model.ParserMetaData;
using ConfigParser.Services;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace ConfigParser.ViewModels
{
    public class XMLConfigParserViewModel
    {
        private readonly LoggerService logger;

        private IXmlNode? root { get; set; }

        private XDocument? doc { get; set; }

        public XMLConfigParserViewModel(LoggerService logger)
        {
            this.logger = logger ?? throw new ArgumentNullException(nameof(logger));
        }

        #region Search and Get Key

        public async Task<ResultSet> SearchInSetAsync(string searchParameter, IXmlNode nodeToLook = null!, int index = -1)
        {
            // Check for null parameters
            if (doc is null || searchParameter == null)
                return null!;

            // Set default value for nodeToLook if not provided
            if (nodeToLook == null)
                nodeToLook = root!;

            // Define regex patterns
            string pattern = @"(?<=[^.])\.(?=\D)";
            string patternForArray = @"(\w+\s*\w+)\[(\d+)\]";

            // Split the search parameter into parts
            string[] parts = Regex.Split(searchParameter, pattern);

            if (parts.Length >= 2)
            {
                string setName = parts[0];
                string subSearchParameter = string.Join(".", parts, 1, parts.Length - 1);

                if (nodeToLook is XmlCompositeNode rootCompositeNode)
                {
                    IXmlNode? setNode = GetSetNode(rootCompositeNode, setName, patternForArray);

                    if (setNode != null)
                    {
                        if (Regex.IsMatch(subSearchParameter, pattern))
                        {
                            return await SearchInSetAsync(subSearchParameter, setNode);
                        }

                        ResultSet result = GetSearchResult(subSearchParameter, setNode, patternForArray);

                        if (result.Result)
                        {
                            Console.WriteLine($"Set: {setName}, Parameter: {searchParameter}, Value: {result.Result}");
                            return result;
                        }
                        else
                        {
                            Console.WriteLine($"Parameter '{subSearchParameter}' not found in set '{setName}'.");
                        }
                    }
                    else
                    {
                        Console.WriteLine($"Set '{setName}' not found.");
                    }
                }
            }
            else
            {
                await logger.LogErrorAsync($"Invalid search parameter format: '{searchParameter}'.");
            }

            return null!;
        }

        public async Task<(List<IXmlNode>, IXmlNode)> GetListInSetAsync(string searchParameter, IXmlNode nodeToLook = null!)
        {
            // Check for null parameters
            if (doc is null || searchParameter == null)
                return (null!, null!);

            // Set default value for nodeToLook if not provided
            if (nodeToLook == null)
                nodeToLook = root!;

            // Define regex patterns
            string pattern = @"(?<=[^.])\.(?=\D)";
            string patternForArray = @"(\w+\s*\w+)\[(\d+)\]";

            // Split the search parameter into parts
            string[] parts = Regex.Split(searchParameter, pattern);

            if (parts.Length >= 2)
            {
                string setName = parts[0];
                string subSearchParameter = string.Join(".", parts, 1, parts.Length - 1);

                if (nodeToLook is XmlCompositeNode rootCompositeNode)
                {
                    IXmlNode? setNode = GetSetNode(rootCompositeNode, setName, patternForArray);

                    if (setNode != null)
                    {
                        if (Regex.IsMatch(subSearchParameter, pattern))
                            return await GetListInSetAsync(subSearchParameter, setNode);

                        List<IXmlNode?> resultList = GetListResult(subSearchParameter, setNode, patternForArray);

                        return (resultList!, setNode!)!;
                    }
                    else
                    {
                        Console.WriteLine($"Set '{setName}' not found.");
                    }
                }
            }
            else if(!searchParameter.Contains(".")) //In case of List needed from root node. Used for NameValuePairGrid. e.g,. logging
            {
                if (nodeToLook is XmlCompositeNode rootCompositeNode)
                {
                    IXmlNode? setNode = GetSetNode(rootCompositeNode, searchParameter, patternForArray);
                    if (setNode != null)
                    {
                        List<IXmlNode> list = new();
                        list.Add(setNode);
                        return (list!, setNode!);
                    }
                }  
            }
            else
            {
                await logger.LogErrorAsync($"Invalid search parameter format: '{searchParameter}'.");
            }

            return (null!, null!);
        }

        #endregion

        #region Parse Config XML

        private async Task<IXmlNode> ParseXmlToComposite(XElement element)
        {
            try
            {
                XmlCompositeNode compositeNode = new XmlCompositeNode(element.Name.LocalName);

                // Process attributes of the current element and create leaf nodes
                foreach (var attribute in element.Attributes())
                {
                    XAttribute attributeValue = element!.Attribute("value")!;
                    XAttribute attributeName = element!.Attribute("name")!;

                    var leafNode = new XmlLeafNode(attributeName!.Value, attributeValue);
                    leafNode.CorrespondingElement = element;
                    return leafNode;
                }

                // Process child elements recursively and add them to the composite node
                foreach (var childElement in element.Elements())
                {
                    IXmlNode childNode = await ParseXmlToComposite(childElement);
                    compositeNode.CorrespondingElement = childElement;
                    compositeNode.AddChild(childNode);
                }

                return compositeNode;
            }
            catch (Exception ex)
            {
                var ele = element;
                await logger.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            }
            return null!;
        }

        #endregion

        public XDocument getXMLDoc()
        {
            // Check for null document
            if (doc is null)
                return null!;

            return doc;
        }

        internal async Task<bool> LoadConfigFile(string xmlPath)
        {
            try
            {
                var xmlContent = File.ReadAllText(xmlPath);

                doc = XDocument.Parse(xmlContent);

                root = await ParseXmlToComposite(doc.Root!);
                return true;
            }
            catch (Exception e)
            {
                doc = null;
                root = null;
                _ = e;
                await logger.LogErrorAsync($"Corrupted File {xmlPath} Found !!");
                return false;
            }
        }

        #region Helper Methods

        private IXmlNode? GetSetNode(XmlCompositeNode rootCompositeNode, string setName, string patternForArray)
        {
            var match = Regex.Match(setName, patternForArray);
            if (match.Success)
            {
                return rootCompositeNode.Children
                    .Where(node => node.Name == match.Groups[1].Value)
                    .Skip(Int32.Parse(match.Groups[2].Value))
                    .FirstOrDefault();
            }
            else
            {
                return rootCompositeNode.Children.Find(node => node.Name == setName);
            }
        }

        private ResultSet GetSearchResult(string subSearchParameter, IXmlNode setNode, string patternForArray)
        {
            ResultSet result;
            var match = Regex.Match(subSearchParameter, patternForArray);
            if (match.Success)
            {
                result = setNode.SearchAsync(match.Groups[1].Value, Int32.Parse(match.Groups[2].Value)).Result;
            }
            else
            {
                result = setNode.SearchAsync(subSearchParameter).Result;
            }

            if (result.Result)
            {
                Console.WriteLine($"Set: {setNode.Name}, Parameter: {subSearchParameter}, Value: {result.Result}");
                return result;
            }
            else
            {
                Console.WriteLine($"Parameter '{subSearchParameter}' not found in set '{setNode.Name}'.");
            }

            return result;
        }

        private List<IXmlNode?> GetListResult(string subSearchParameter, IXmlNode setNode, string patternForArray)
        {
            List<IXmlNode?> resultList = null!;
            var match = Regex.Match(subSearchParameter, patternForArray);
            if (match.Success)
            {
                var node = ((XmlCompositeNode)setNode).Children
                    .FindAll(n => n.Name == match.Groups[1].Value)
                    .ToList().ElementAt(Int32.Parse(match.Groups[2].Value));

                resultList = new List<IXmlNode?>();
                resultList.Add(node);
            }
            else
            {
                resultList = ((XmlCompositeNode)setNode).Children
                    .FindAll(n => n.Name == subSearchParameter)!;
            }

            return resultList!;
        }

        #endregion
    }
}
