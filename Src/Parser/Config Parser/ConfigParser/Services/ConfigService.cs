using ConfigParser.Model.ParserMetaData;
using ConfigParser.ViewModels;
using System.Xml.Linq;

namespace ConfigParser.Services;

public class ConfigService
{
    private XMLConfigParserViewModel ViewModel { get; set; }

    private LoggerService logger;

    private StateManager stateManager;

    private string? file { get; set; }

    public bool isFileUpdated { get; set; }

    private bool isGenerationExpired = false;

    public bool isGenerationModified
    {
        get
        {
            return isGenerationExpired;
        }
        set
        {
            isGenerationExpired = value;   
            if (isGenerationExpired)
                stateManager.ConfigGenerationState = EGenerationState.NotGenerated;
        }
    }


    public ConfigService(LoggerService logger, StateManager stateManager)
    {
        ViewModel = new XMLConfigParserViewModel(logger);
        this.logger = logger;
        this.stateManager = stateManager;
    }

    public async Task<bool> LoadConfigFile(string xmlPath)
    {
        try
        {
            bool bResult = await ViewModel.LoadConfigFile(xmlPath);

            if (bResult)
            {
                if (!String.IsNullOrEmpty(file))
                {
                    await logger.DeleteOldLogFilesForNewConfig(file);
                }
                file = xmlPath;
            }
            else
                file = null;

            return bResult;
        }
        catch (Exception ex)
        {
            await logger.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
        }
        return false;
    }

    #region Search and Get Key
    public async Task<ResultSet> SearchInSetAsync(string searchParameter, IXmlNode nodeToLook = null!, int index = -1)
    {
        try
        {
            return await ViewModel.SearchInSetAsync(searchParameter);
        }
        catch (Exception ex)
        {
            await logger.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
        }
        return null!;
    }

    public async Task<(List<IXmlNode>, IXmlNode)> GetListInSetAsync(string searchParameter)
    {
        try
        {
            return await ViewModel.GetListInSetAsync(searchParameter);
        }
        catch (Exception ex)
        {
            await logger.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
        }
        return (null!, null!);
    }

    #endregion

    public XDocument getXMLDoc()
    {
        return ViewModel.getXMLDoc();
    }

    public string getConfigFileName()
    {
        return file!;
    }
}
