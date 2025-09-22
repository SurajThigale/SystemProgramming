using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConfigParser.Services
{
    public enum EGenerationState
    {
        NotGenerated,
        Generated,
        None
    }

    public enum EUplaodState
    {
        Uploaded,
        None
    }

    public class StateManager
    {
        private EGenerationState appGenerationState = EGenerationState.None;

        private EUplaodState appUploadState = EUplaodState.None;

        public EGenerationState ConfigGenerationState { get => appGenerationState; set => appGenerationState = value; }

        public EUplaodState ConfigUploadState { get => appUploadState; set => appUploadState = value; }

        public bool GenerationInProgrss { get; set; } = false;

        public bool UploadToSNInProgress { get; set; } = false;

        public bool ExportInProgress { get; set; } = false;

        // Add the shared flag and version_no variable
        public bool IsGenerateClicked { get; set; } = false;
        
        public string VersionNo { get; set; } = "1.8.6";

        public (bool, string) CheckIfProcessAllowed()
        {

            if (UploadToSNInProgress || ExportInProgress || GenerationInProgrss)
            {
                string message = UploadToSNInProgress ? "Upload to SN In Progress!" : (GenerationInProgrss ? "Generation of Binary is in Progress" : (ExportInProgress ? "Export of XML file is in Progress" : ""));
                return (false, message);
            }

            return (true, "");
        }
    }
}
