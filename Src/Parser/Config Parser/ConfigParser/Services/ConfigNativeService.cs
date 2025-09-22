using ConfigParser.dllmap;
using System.Net.Http.Json;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Text.RegularExpressions;
using Windows.Networking.Sockets;

namespace ConfigParser.Services;

public class StructureMetaData
{
    public string? BinaryData { get; set; }
    public string? SLConfig { get; set; }
    public int? Size { get; set; }
    public string? Status { get; set; }
    public JsonElement? Data { get; set;}
}

public class ConfigData
{
    public Dictionary<string, StructureMetaData>? data { get; set; }

    public Dictionary<string, bool>? UploadStatus { get; set; }
}

public class ConfigNativeService
{
    private ConfigService? configService;

    private LoggerService? logger;

    public ConfigData? GeneratedData { get; set; }

    private readonly object lockObject = new object(); // Used for synchronization

    public ConfigNativeService(ConfigService config, LoggerService logger)
    {
        DllMap.Register(Assembly.GetExecutingAssembly());
        configService = config;
        this.logger = logger;

        Task.Run(async () => await ExtractLimits());
    }

    public Dictionary<string, int>? ArrayLimits { get; set; }

    public async Task<bool> Generate()
    {
        try
        {
            string? result = default!;
            lock (lockObject)
            {
                ArgumentNullException.ThrowIfNull(configService);

                var xml = configService.getXMLDoc()?.ToString();

                IntPtr ptr = Generate(xml!);

                if (ptr == IntPtr.Zero) return false;

                result = Marshal.PtrToStringAnsi(ptr);

                FreeMemory(ptr);
            }

            await Task.Run(() =>
            {
                File.WriteAllText($"{logger!.getConfigLogFolderPath()}//CHeaders.json", result!);
            });

            Dictionary<string, StructureMetaData>? generatedData = null;

            await Task.Run(() =>
            {
                generatedData = JsonSerializer.Deserialize<Dictionary<string, StructureMetaData>>(result!);
            });

            lock (lockObject)
            {
                if (GeneratedData is null)
                    GeneratedData = new();

                GeneratedData.data = generatedData;
            }

            configService!.isGenerationModified = false;

            return true;
        }
        catch (Exception ex)
        {
            await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
        }

        return false;
    }

    public async Task<string> WriteXMLAsync(string fileName)
    {
        try
        {
            lock (lockObject)
            {
                ArgumentNullException.ThrowIfNull(configService);

                if (fileName == null)
                {
                    var assemplyPath = AppDomain.CurrentDomain.BaseDirectory;
                    fileName = Path.Combine(Path.GetDirectoryName(assemplyPath)!,
                         "config.xml");
                }

                WriteXML(configService!.getXMLDoc().ToString(), fileName);
            }           
        }
        catch (Exception ex)
        {
            await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
        }

        return fileName;
    }

    public async Task<bool> OnUploadToSN()
    {
        try
        {
            ArgumentNullException.ThrowIfNull(configService);
            var assemplyPath = Assembly.GetExecutingAssembly().Location!;
            string toUploadXML = Path.Combine(Path.GetDirectoryName(assemplyPath)!,
                     "Upload\\config.xml");

            System.IO.Directory.CreateDirectory($"{Path.GetDirectoryName(assemplyPath)}\\Upload");

            toUploadXML = await WriteXMLAsync(toUploadXML);

            IntPtr ptr = await Task.Run(() => UploadToSN(toUploadXML));

            if (ptr == IntPtr.Zero) return false;

            var result = Marshal.PtrToStringAnsi(ptr);
            FreeMemory(ptr);

            Dictionary<string, bool>? uploadStatus = new();

            await Task.Run(() =>
            {
                uploadStatus = JsonSerializer.Deserialize<Dictionary<string, bool>>(result!);
            });

            lock (lockObject)
            {
                if (GeneratedData is null)
                    GeneratedData = new();

                GeneratedData.UploadStatus = uploadStatus;
            }

            // Check if any value is false, ignoring keys starting with "xc" as we are not uplaoding to XC in this version
            // Needs to remove this check once we are in Version 2 of this application
            return !uploadStatus!.Where(kv => !kv.Key.StartsWith("xc")).Any(kv => !kv.Value);
        }
        catch (Exception ex)
        {
            await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            return false;
        }
    }

    public async Task<bool> ExtractLimits()
    {
        try
        {
            string? result = default!;
            lock (lockObject)
            {
                IntPtr ptr = GetLimits();

                if (ptr == IntPtr.Zero) return false;

                result = Marshal.PtrToStringAnsi(ptr);
                FreeMemory(ptr);
            }

            if (ArrayLimits is null)
                ArrayLimits = new();

            ArrayLimits = await Task.Run(() => JsonSerializer.Deserialize<Dictionary<string, int>>(result!));
            return true;
        }
        catch (Exception ex)
        {
            await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            return false;
        }
    }

    public async Task<string[]> GetNonCompatibileNodes()
    {
        try
        {
            lock (lockObject)
            {
                if (String.IsNullOrEmpty(configService?.getConfigFileName())) return null!;

                var xml = configService.getXMLDoc()?.ToString();

                IntPtr ptr = GetNonCompatibileNodes(xml!);

                if (ptr == IntPtr.Zero) return null!;

                string? result = Marshal.PtrToStringAnsi(ptr);
                FreeMemory(ptr);

                Dictionary<string, object>? jsonDictionary = JsonSerializer.Deserialize<Dictionary<string, object>>(result!);

                // Extract only the keys
                List<string> keys = new List<string>(jsonDictionary?.Keys!);

                return keys.ToArray()!;
            }

        }
        catch (Exception ex)
        {
            await logger!.LogCriticalAsync($"Exception caught in {System.Reflection.MethodBase.GetCurrentMethod()} : {ex.Message}");
            return null!;
        }
    }

    [DllImport("ConfigExporter")]
    static extern IntPtr Generate(string xml);

    [DllImport("ConfigExporter")]
    static extern void WriteXML(string xml, string path);

    [DllImport("ConfigExporter")]
    static extern IntPtr UploadToSN(string path);

    [DllImport("ConfigExporter")]
    static extern IntPtr GetLimits();

    [DllImport("ConfigExporter")]
    static extern IntPtr GetNonCompatibileNodes(string xml);

    [DllImport("ConfigExporter")]
    public static extern void FreeMemory(IntPtr ptr);

    public void ClearGeneratedData()
    {
        GeneratedData = null;
    }
}

