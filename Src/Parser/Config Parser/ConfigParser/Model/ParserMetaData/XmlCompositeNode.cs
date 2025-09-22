using Microsoft.AspNetCore.Components;
using System.Xml.Linq;

namespace ConfigParser.Model.ParserMetaData;

public class XmlCompositeNode : IXmlNode
{
    public List<IXmlNode> Children = new List<IXmlNode>();
    public string Name { get; set; }
    public XAttribute? Value { get; set; } // Composite nodes do not have a direct value

    private Dictionary<string, string> propertyValues = new Dictionary<string, string>();

    public bool bIsDataUpdate { get; set; }
    public XElement? CorrespondingElement { get; set; } // Reference to the corresponding XML element

    public XmlCompositeNode(string name)
    {
        Name = name;
    }

    public string this[string parameterName]
    {
        get
        {
            return Task.Run(() => GetAsync(parameterName)).Result;
        }
    }

    // Add a method to add a new array element to a nested node
    public void AddArrayElementToNestedNode(string nodeName, string nestedNodeName, string[] parameterNames, string[] parameterValues)
    {
        XElement newNestedNode = new XElement(nestedNodeName);


        // Add parameters to the new nested node
        for (int i = 0; i < parameterNames.Length && i < parameterValues.Length; i++)
        {
            newNestedNode.Add(new XElement("parameter", new XAttribute("name", parameterNames[i]), new XAttribute("value", parameterValues[i])));
        }

        XmlCompositeNode node = new XmlCompositeNode(nestedNodeName);
        foreach (var element in newNestedNode.Elements())
        {
            XAttribute attributeValue = element!.Attribute("value")!;
            XAttribute attributeName = element!.Attribute("name")!;

            var leafNode = new XmlLeafNode(attributeName!.Value, attributeValue);

            node.AddChild(leafNode);
        }

        // Find the corresponding XML element of the parent composite node
        var parentElement = CorrespondingElement;

        if (parentElement != null)
        {
            // Add the array element to the corresponding XML element
            parentElement.Add(newNestedNode);
        }

        AddChild(node);

    }

    private async Task<string> GetAsync(string parameterName)
    {
        if (!propertyValues.ContainsKey(parameterName))
        {
            var result = await SearchAsync(parameterName).ConfigureAwait(false);

            if (result.Result)
            {
                propertyValues[parameterName] = result!.Element!.Value!.Value;
            }
            else
            {
                propertyValues[parameterName] = "0";
            }
        }

        return propertyValues[parameterName];
    }


    public async Task SetValueAsync(string parameterName, string value)
    {
        var result = await SearchAsync(parameterName).ConfigureAwait(false);

        if (result.Result)
        {
            propertyValues[parameterName] = value;
            result!.Element!.Value!.Value = value;
        }
    }

    public async Task<ResultSet> SearchAsync(string parameterName, int index = -1)
    {
        int currentIndex = 0;
        foreach (var child in Children)
        {
            var result = await child.SearchAsync(parameterName, index);

            if (result.Result && index != -1 && currentIndex++ != index) continue;

            if (result.Result)
            {
                return result;
            }
        }
        return new ResultSet { Result = false, Element = null };
    }


    public void AddChild(IXmlNode child)
    {
        Children.Add(child);
        if (child is XmlCompositeNode compositeChild)
        {
            compositeChild.CorrespondingElement = CorrespondingElement; // Propagate the reference
        }
        else if (child is XmlLeafNode leafChild)
        {
            //CorrespondingElement?.Add(new XElement(leafChild.Name, leafChild.Value?.Value));
            leafChild.CorrespondingElement = CorrespondingElement; // Set the reference
        }
    }

    public void Remove(IXmlNode child)
    {
        if (Children.Contains(child))
        {
            Children.Remove(child);

            // Remove from the XML element
            if (CorrespondingElement != null)
            {
                if (child is XmlLeafNode)
                {
                    XmlLeafNode node = (XmlLeafNode) child;
                    node.Remove();
                }
                else if(child is XmlCompositeNode)
                {
                    var elementToRemove = CorrespondingElement.Elements().FirstOrDefault(e => e.Name.LocalName == child.Name);
                    elementToRemove?.Remove();
                }
            }
        }
    }

    public void RemoveById(IXmlNode child)
    {
        if (Children.Contains(child))
        {
            Children.Remove(child);

            // Remove from the XML element
            if (CorrespondingElement != null)
            {
                if (child is XmlLeafNode)
                {
                    XmlLeafNode node = (XmlLeafNode)child;
                    node.Remove();
                }
                else if (child is XmlCompositeNode)
                {
                    var elementsToRemove = CorrespondingElement.Elements().Where(e => e.Name.LocalName == child.Name).ToList();
                    var idToRemove = child.GetChildren().FirstOrDefault(c => c.Name == "Id")?.Value?.Value;
                    foreach (var element in elementsToRemove)
                    {
                        var paraElement = element.Element("parameter");
                        if (paraElement?.Attribute("name")?.Value == "Id" && paraElement.Attribute("value")?.Value == idToRemove)
                        {
                            element.Remove();
                            break;
                        }
                    }
                    //int id = 1;
                    //foreach (var subChild in Children)
                    //{
                    //    var idNode = subChild.GetChildren().FirstOrDefault(c => c.Name == "Id");
                    //    idNode?.Value?.SetValue(id++.ToString());
                    //}
                }
            }
        }
    }
    public List<IXmlNode> GetChildren()
    {
        return Children;
    }

    public void AddChild(string parameterName, string value)
    {
        // Find the corresponding XML element of the parent composite node
        var parentElement = CorrespondingElement;

        // Add parameters to the new nested node
        XElement child = new XElement("parameter", new XAttribute("name", parameterName), new XAttribute("value", value));
        parentElement?.Add(child);

        XAttribute attributeValue = child!.Attribute("value")!;
        XAttribute attributeName = child!.Attribute("name")!;

        var leafNode = new XmlLeafNode(attributeName!.Value, attributeValue);

        AddChild(leafNode);

    }

    public Task SetNameAsync(string Name)
    {
        throw new NotImplementedException();
    }
}


