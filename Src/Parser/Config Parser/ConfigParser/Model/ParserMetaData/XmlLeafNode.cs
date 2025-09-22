using System.Xml.Linq;

namespace ConfigParser.Model.ParserMetaData;

public class XmlLeafNode : IXmlNode
{
    public string Name { get; set; }
    public XAttribute? Value { get; set; }
    public bool bIsDataUpdate { get; set; }
    public XElement? CorrespondingElement { get; set; } // Reference to the corresponding XML element

    public string this[string parameterName]
    {
        get { return Value?.Value!; }
    }

    public XmlLeafNode(string name, XAttribute value)
    {
        Name = name;
        Value = value;
    }

    // Add a method to add a new array element to a nested node (no action for leaf nodes)
    public void AddArrayElementToNestedNode(string nodeName, string nestedNodeName, string[] parameterNames, string[] parameterValues)
    {
        // Leaf nodes cannot have nested nodes
    }

    public async Task<ResultSet> SearchAsync(string parameterName, int index = -1)
    {
        await Task.Delay(0);
        if (Name.ToLower() == parameterName.ToLower())
        {
            return new ResultSet { Result = true, Element = this };
        }
        return new ResultSet { Result = false, Element = null };
    }

    public void Display(int depth)
    {
        Console.WriteLine($"{new string('-', depth)} {Name}: {Value}");
    }

    public async Task SetValueAsync(string parameterName, string value)
    {
        await Task.Delay(0);
        Name = parameterName;
        Value!.Value = value;
    }

    public void Remove()
    {
        // Remove the XAttribute from its parent
        Value?.Parent?.Remove();
    }

    public List<IXmlNode> GetChildren()
    {
        return null!;
    }

    public void AddChild(string parameterName, string value)
    {
        throw new NotImplementedException();
    }

    public async Task SetNameAsync(string Name)
    {
        this.Name = Name;
        Value!.Parent?.SetAttributeValue("name", Name);
        await Task.Delay(0);
    }

    public void Remove(IXmlNode child)
    {
        throw new NotImplementedException();
    }

    public void RemoveById(IXmlNode child)
    {
        throw new NotImplementedException();
    }
}

