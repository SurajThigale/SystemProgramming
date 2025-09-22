using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace ConfigParser.Model.ParserMetaData;

public interface IXmlNode
{
    string Name { get; set; }

    bool bIsDataUpdate { get; set; }

    XAttribute? Value { get; set; }

    public XElement? CorrespondingElement { get; set; }

    string this[string parameterName] { get; }

    Task SetNameAsync(string Name);

    Task SetValueAsync(string parameterName, string value);

    void AddChild(string parameterName, string value);

    List<IXmlNode> GetChildren();

    void Remove(IXmlNode child);

    void RemoveById(IXmlNode child);

    Task<ResultSet> SearchAsync(string parameterName, int index = -1);

    public void AddArrayElementToNestedNode(string nodeName, string nestedNodeName, string[] parameterNames, string[] parameterValues);
}