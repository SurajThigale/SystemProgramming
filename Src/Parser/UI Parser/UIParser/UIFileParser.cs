

using UIParser.Models;

namespace UIParser;

public class UIFileParser
{
    public static string? uiRootPath { get; set; }

    public async Task<GridMetaData> parseXML(string fileName)
    {
        var file = uiRootPath + "//" + fileName;

        var xmlContent = File.ReadAllText(file);

        // Parse the XML into UI model
        return await ParserHelper.ParseXml(xmlContent ?? null, uiRootPath!);
    }
}