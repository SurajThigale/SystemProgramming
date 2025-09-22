

#include "ExportData.h"
#include "pugixml.hpp"
#include "Utility.h"
#include <bitset>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <local_inc/UtilBase_SATs.h>
#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_set>
#include <vector>
#include <regex>

#define clue_LOG_TO_FILE "Logs//NativeLogger.log.txt"
#define LOG_MODULE_NAME "ConfigExporter.Native"
#include "clue.hpp"

#include <memory>

using json = nlohmann::json;
namespace fs = std::filesystem;

#include "StructToJsonConverter.hpp"


#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Shlwapi.lib")
#define AREA_WIDTH(a) ((a)->tBottomRight.lY - (a)->tTopLeft.lY)
#define AREA_HEIGHT(a) ((a)->tTopLeft.lZ - (a)->tBottomRight.lZ)

namespace
{
	using namespace std;

	std::vector<string> xpathFromStructures;

	pugi::xml_document doc;

	std::vector<std::string> lcu;
	std::vector<std::string> ioc;
	std::vector<std::string> exchangercfg;
	std::string master;

	void GetXPaths(const pugi::xml_node& node, const std::string& currentPath, std::vector<std::string>& xpaths)
	{
		// Build the XPath for the current node
		std::string xpath = currentPath.empty() ? node.name() : currentPath + "/" + node.name();

		// Add attributes to XPath
		for (const pugi::xml_attribute& attribute : node.attributes())
		{
			auto name = attribute.name();
			if (std::strcmp(name, "value") == 0)
				continue;
			xpaths.push_back("/" + xpath + "/@" + attribute.as_string());
		}

		// Add current node's XPath to the vector
		//xpaths.push_back(xpath);

		// Recursively process child nodes
		for (const pugi::xml_node& child : node.children())
		{
			GetXPaths(child, xpath, xpaths);
		}
	}

	auto findParamValue = [](pugi::xml_node node, const std::string& paramNameToFind) {
		try
		{
			pugi::xml_node child = node.find_child_by_attribute("parameter", "name", paramNameToFind.c_str());
			if (child)
			{
				std::string parentPath = child.parent().path();
				std::string xpath = parentPath + "/parameter/@" + child.attribute("name").as_string();
				xpathFromStructures.push_back(xpath);
				return child.attribute("value");
			}
		}
		catch (...)
		{
			LOG_ERROR("Exception in findParamValue");
		}

		return pugi::xml_attribute();
		};


	void processFileInPlace(const std::string& filePath) {
		try {
			// Open the file for read and write in binary mode
			std::ifstream file(filePath, std::ios::binary);
			if (!file.is_open()) {
				throw std::runtime_error("Error opening the file.");
			}

			// Read the entire file into a string
			std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

			// Use regex to replace " />" with "/>"
			std::regex pattern(R"(\s+/>)");
			fileContent = std::regex_replace(fileContent, pattern, "/>");

			// Close the file
			file.close();

			// Open the file for writing
			std::ofstream outputFile(filePath, std::ios::binary | std::ios::trunc);
			if (!outputFile.is_open()) {
				throw std::runtime_error("Error opening the output file.");
			}

			// Write the modified content back to the file
			outputFile << fileContent;

		}
		catch (const std::exception& e) {
			LOG_ERROR("Error while removing extra space from XML node" << e.what());
		}
	}
}

ExportData::ExportData()
{
	char path[MAX_PATH];
	if (GetModuleFileName(NULL, path, MAX_PATH) != 0)
	{
		std::cout << "DLL path: " << path << std::endl;
	}
	else
	{
		std::cerr << "Error getting DLL path" << std::endl;
	}

	std::filesystem::path directoryPath = std::filesystem::path(path).parent_path();

	std::string fullPath = directoryPath.string() + "//" + clue_LOG_TO_FILE;

	if (fs::exists(fullPath))
	{
		try
		{
			fs::remove(fullPath);
			std::cout << "File deleted successfully." << std::endl;
		}
		catch (const std::filesystem::filesystem_error& e)
		{
			std::cerr << "Error deleting file: " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << "File does not exist." << std::endl;
	}
}
static int NumberOfLCUInstalled;
static int NumberOfIOCInstalled;
static int NumberOfXCInstalled;
std::string ExportData::createMetaDataJson()
{
	json j;

	pugi::xml_node node = doc.child("config").child("general");

	NumberOfLCUInstalled = findParamValue(node, "NumberOfLCUInstalled").as_int();
	NumberOfIOCInstalled = findParamValue(node, "NumberOfIOCInstalled").as_int();
	NumberOfXCInstalled = findParamValue(node, "NumberOfXCInstalled").as_int();

	/*
	 * Master Configuration
	 */

	std::ostringstream tempStringMaster;
	tempStringMaster.str().clear();

	//SLConfig Generation
	Utility::convert_to_hex_slConfig(tempStringMaster, reinterpret_cast<const unsigned char*>(&mstCfg), sizeof(masterCfg_t));
	master = tempStringMaster.str();

	//Binary Generation
	std::ostringstream tempStringMasterH;
	Utility::convert_to_hex_string(tempStringMasterH, reinterpret_cast<const unsigned char*>(&mstCfg), sizeof(masterCfg_t));
	std::string binaryValueMstH = tempStringMasterH.str();

	//Verify
	masterCfg_t mstCfgCheck;
	istringstream ip(binaryValueMstH);
	Utility::convert_to_structTest(ip, reinterpret_cast<unsigned char*>(&mstCfgCheck), sizeof(masterCfg_t));

	std::string status;
	if (memcmp(&mstCfgCheck, &mstCfg, sizeof(masterCfg_t)) == 0)
		status = "Master mstCfg matches";
	else
		status = "Master mstCfg not matches";

	j["master"]["SLConfig"] = master;
	j["master"]["BinaryData"] = binaryValueMstH;
	j["master"]["Size"] = sizeof(mstCfg);
	j["master"]["Status"] = status;
	j["master"]["Data"] = to_json(mstCfgCheck);

	/*
	 * LCU Configuration
	 */
	for (int i = 0; i < NumberOfLCUInstalled; ++i)
	{		
		std::ostringstream tempStringSl, tempStringHex;

		// Set LCU ID here!
		//lcuCfg[i].tLcuId = static_cast<tLcuId_t>(i);

		//1. Recalculate CRC before serialization(excluding ulCrc only)
			//lcuCfg[i].ulCrc = Utility::crc32Checksum(
			//	sizeof(lcuCfg_t) - sizeof(uint32_t),                          // Exclude CRC
			//	reinterpret_cast<const uint8_t*>(&lcuCfg[i]) + sizeof(uint32_t),  // Start after CRC field
			//	0
			//);

		// SLConfig Generation
		Utility::convert_to_hex_slConfig(tempStringSl, reinterpret_cast<const unsigned char*>(&lcuCfg[i]), sizeof(lcuCfg_t));
		lcu.push_back(tempStringSl.str());

		// Binary Generation
		Utility::convert_to_hex_string(tempStringHex, reinterpret_cast<const unsigned char*>(&lcuCfg[i]), sizeof(lcuCfg_t));
		std::string binaryData = tempStringHex.str();

		// Verify
		lcuCfg_t lcuCheck{};
		std::istringstream input(binaryData);
		Utility::convert_to_structTest(input, reinterpret_cast<unsigned char*>(&lcuCheck), sizeof(lcuCfg_t));

		std::string status = (memcmp(&lcuCheck, &lcuCfg[i], sizeof(lcuCfg_t)) == 0)
			? "Lcu" + std::to_string(i) + " lcuCfg matches"
			: "Lcu" + std::to_string(i) + " lcuCfg not matches";

		std::string key = "lcu" + std::to_string(i);

		// Build JSON
		j[key]["SLConfig"] = lcu[i];
		j[key]["BinaryData"] = binaryData;
		j[key]["Size"] = sizeof(lcuCfg_t);
		j[key]["Status"] = status;
		j[key]["Data"] = to_json(lcuCheck);
	}

	/*
	 * xccfg Configuration
	 */

	for (int i = 0; i < NumberOfXCInstalled; ++i)
	{
		std::ostringstream tempStringSl, tempStringHex;

		// SLConfig Generation
		Utility::convert_to_hex_slConfig(tempStringSl, reinterpret_cast<const unsigned char*>(&xcCfg[i]), sizeof(xcCfg_t));
		exchangercfg.push_back(tempStringSl.str());

		// Binary Generation
		Utility::convert_to_hex_string(tempStringHex, reinterpret_cast<const unsigned char*>(&xcCfg[i]), sizeof(xcCfg_t));
		std::string binaryData = tempStringHex.str();

		// Verify
		xcCfg_t xcCheck{};
		std::istringstream input(binaryData);
		Utility::convert_to_structTest(input, reinterpret_cast<unsigned char*>(&xcCheck), sizeof(xcCfg_t));

		std::string status = (memcmp(&xcCheck, &xcCfg[i], sizeof(xcCfg_t)) == 0)
			? "Xc" + std::to_string(i) + " xcCfg matches"
			: "Xc" + std::to_string(i) + " xcCfg not matches";

		std::string key = "xc" + std::to_string(i);

		// Build JSON
		j[key]["SLConfig"] = exchangercfg[i];
		j[key]["BinaryData"] = binaryData;
		j[key]["Size"] = sizeof(xcCfg_t);
		j[key]["Status"] = status;
		j[key]["Data"] = to_json(xcCheck);
	}

	/*
	 * IOC Configuration
	 */

	for (int i = 0; i < NumberOfIOCInstalled; ++i)
	{
		std::ostringstream tempStringSl, tempStringHex;

		// SLConfig Generation
		Utility::convert_to_hex_slConfig(tempStringSl, reinterpret_cast<const unsigned char*>(&iocCfg[i]), sizeof(iocCfg_t));
		ioc.push_back(tempStringSl.str());

		// Binary Generation
		Utility::convert_to_hex_string(tempStringHex, reinterpret_cast<const unsigned char*>(&iocCfg[i]), sizeof(iocCfg_t));
		std::string binaryData = tempStringHex.str();

		// Verify
		iocCfg_t iocCheck{};
		std::istringstream input(binaryData);
		Utility::convert_to_structTest(input, reinterpret_cast<unsigned char*>(&iocCheck), sizeof(iocCfg_t));

		std::string status = (memcmp(&iocCheck, &iocCfg[i], sizeof(iocCfg_t)) == 0)
			? "ioc" + std::to_string(i) + " iocCfg matches"
			: "ioc" + std::to_string(i) + " iocCfg not matches";

		std::string key = "ioc" + std::to_string(i);

		// Build JSON
		j[key]["SLConfig"] = ioc[i];
		j[key]["BinaryData"] = binaryData;
		j[key]["Size"] = sizeof(iocCfg_t);
		j[key]["Status"] = status;
		j[key]["Data"] = to_json(iocCheck);
	}

	return j.dump();
}

std::string ExportData::generate(const std::string xml, bool exportMetaData)
{
	using namespace std;

	pugi::xml_parse_result result = doc.load_string(xml.c_str(), pugi::parse_full);

	pugi::xml_node node = doc.child("config").child("general");

	NumberOfLCUInstalled = findParamValue(node, "NumberOfLCUInstalled").as_int();
	NumberOfIOCInstalled = findParamValue(node, "NumberOfIOCInstalled").as_int();
	NumberOfXCInstalled = findParamValue(node, "NumberOfXCInstalled").as_int();

	if (!result)
	{
		LOG_ERROR("Error description: " << result.description() << "\n");
		LOG_ERROR("Error offset: " << result.offset << " (error at [..." << (xml.c_str() + result.offset) << "]\n\n");
		return "";
	}

	memset(&mstCfg, 0, sizeof(mstCfg));
	memset(&xcCfg, 0, sizeof(xcCfg));
	memset(&lcuCfg, 0, sizeof(lcuCfg));
	memset(&iocCfg, 0, sizeof(iocCfg));

	buildMasterConfig(&mstCfg);

	// FIXED: Initialize each LCU config
	lcuCfg.clear();
	lcuCfg.resize(NumberOfLCUInstalled);
	lcu.clear();
	for (int i = 0; i < NumberOfLCUInstalled; ++i)
	{
		buildLcuConfig(&lcuCfg[i], i); //Fills each LCU
		std::cout << "LCU " << i << " configuration: ";
		for (int j = 0; j < sizeof(lcuCfg_t); ++j)
		{
			std::cout << reinterpret_cast<unsigned char*>(&lcuCfg[i])[j] << " ";
		}
		std::cout << std::endl;

	}
	iocCfg.clear();
	iocCfg.resize(NumberOfIOCInstalled);
	ioc.clear();
	for (int i = 0; i < NumberOfIOCInstalled; ++i)
	{
		buildIocConfig(&iocCfg[i], i); //Fills each LCU
		std::cout << "IOC " << i << " configuration: ";
		for (int j = 0; j < sizeof(iocCfg_t); ++j)
		{
			std::cout << reinterpret_cast<unsigned char*>(&iocCfg[i])[j] << " ";
		}
		std::cout << std::endl;

	}

	xcCfg.clear();
	xcCfg.resize(NumberOfXCInstalled);
	exchangercfg.clear();
	for (int i = 0; i < NumberOfXCInstalled; ++i)
	{
		buildXcConfig(&xcCfg[i], i); //Fills each LCU
		std::cout << "XC " << i << " configuration: ";
		for (int j = 0; j < sizeof(xcCfg_t); ++j)
		{
			std::cout << reinterpret_cast<unsigned char*>(&xcCfg[i])[j] << " ";
		}
		std::cout << std::endl;

	}

	if (exportMetaData)
	{
		return createMetaDataJson();
	}

	return "";
}

void ExportData::buildXcConfig(xcCfg_t* xcCfg, tXcId_t tXcId) {
	using namespace std;

	memset(xcCfg, 0, sizeof(xcCfg_t));
	/* populate xcCfg using values from mstCfg */
	buildMasterConfig(&mstCfg);

	pugi::xml_node node = doc.child("config").child("general");

	// Access the attribute by its name (e.g., "attribute_name") if the node exists
	xcCfg->tConstants.tConfigVersion = findParamValue(node, "ConfigVersion").as_int();
	xcCfg->tConstants.ulConfigJobSpecificId = mstCfg.tConstants.ulConfigJobSpecificId;
	xcCfg->tXcId = tXcId;

	/* fill SR related constants
	 * See also Main Architecture Design Specification, section Cfg SR
	 *
	 */

	 // Find the root <xcs> element
	pugi::xml_node xcsNode = doc.child("config").child("xcs");

	auto xcGroupNode = xcsNode.child("xc-group");


	xcCfg->tSr.lTCBSS_MaxUnblockActuationTime = findParamValue(xcGroupNode, "TCBSS_MaxUnblockActuationTime").as_int(); /* unit: milliseconds */
	xcCfg->tSr.lTCBSS_MaxBlockActuationTime = findParamValue(xcGroupNode, "TCBSS_MaxBlockActuationTime").as_int(); /* unit: milliseconds */
	xcCfg->tSr.lXCSARE_MaxFailureTime = findParamValue(xcGroupNode, "SARE_MaxFailureTime").as_int(); /* unit: milliseconds */
	xcCfg->tSr.lXCGBSS_MaxAllowedActivationTime = findParamValue(xcGroupNode, "GBSS_MaxAllowedActivationTime").as_int(); /* unit: milliseconds */
	xcCfg->tSr.lXCGBSS_MaxAllowedReleaseTime = findParamValue(xcGroupNode, "GBSS_MaxAllowedReleaseTime").as_int(); /* unit: milliseconds */
	xcCfg->tSr.lXCGBSS_MaxMaintenanceTime = findParamValue(xcGroupNode, "GBSS_MaxMaintenanceTime").as_int(); /* unit: seconds */

	xcCfg->tSr.lXCISTO_ActuationTimeDuration = findParamValue(xcGroupNode, "ISTO_ActuationTimeDuration").as_int(); /* unit: milliseconds */
	xcCfg->tSr.lXCISTO_DeActuationTimeDuration = findParamValue(xcGroupNode, "ISTO_DeActuationTimeDuration").as_int(); /* unit: milliseconds */

	xcCfg->tXc.lXcSPRotationThreshold = findParamValue(xcGroupNode, "SPRotationThreshold").as_int();
	xcCfg->tXc.lXcClockwiseOuterEdgeAngle = findParamValue(xcGroupNode, "ClockwiseOuterEdgeAngle").as_int();
	xcCfg->tXc.lXcCounterclockwiseOuterEdgeAngle = findParamValue(xcGroupNode, "CounterclockwiseOuterEdgeAngle").as_int();
	xcCfg->tXc.lXcClockwiseClearanceAngle = findParamValue(xcGroupNode, "ClockwiseClearanceAngle").as_int();
	xcCfg->tXc.lXcCounterclockwiseClearanceAngle = findParamValue(xcGroupNode, "CounterclockwiseClearanceAngle").as_int();
	xcCfg->tXc.sLockingAngleTolerance = static_cast<int16_t>(findParamValue(xcGroupNode, "LockingAngleTolerance").as_int());
	xcCfg->tXc.lXcSAREVelocityIntegrationTime = findParamValue(xcGroupNode, "SAREVelocityIntegrationTime").as_int();
	xcCfg->tXc.lXcSARE_MultiturnInitialValue = findParamValue(xcGroupNode, "SARE_MultiturnInitialValue").as_int();
	xcCfg->tXc.lXcSARE_ZeroDegreePosition = findParamValue(xcGroupNode, "SARE_ZeroDegreePosition").as_int();
	xcCfg->tXc.lXcSARE_NinetyDegreePosition = findParamValue(xcGroupNode, "SARE_NinetyDegreePosition").as_int();
	xcCfg->tXc.lXcAngularSpeedMax = findParamValue(xcGroupNode, "AngularSpeedMax").as_int();
	xcCfg->tXc.lXcAngularSpeedInManualOpModeMax = findParamValue(xcGroupNode, "AngularSpeedInManualOpModeMax").as_int();
	xcCfg->tXc.lXC_MAR_BoundaryClockwise = findParamValue(xcGroupNode, "MAR_BoundaryClockwise").as_int();
	xcCfg->tXc.lXC_MAR_BoundaryCounterclockwise = findParamValue(xcGroupNode, "MAR_BoundaryCounterclockwise").as_int();		

	xcCfg->tSr.lSCTS_MaxErrorTime = findParamValue(xcGroupNode, "SCTS_MaxErrorTime").as_int();
	xcCfg->tSr.lSCTS_ErrorOverwriteErrorTemp = findParamValue(xcGroupNode, "SCTS_ErrorOverwriteErrorTemp").as_int();
	xcCfg->tSr.lSCTS_ErrorOverwriteWarnTemp = findParamValue(xcGroupNode, "SCTS_ErrorOverwriteWarnTemp").as_int();

	xcCfg->tSr.lTCLSS_MaxCabinLockingTime = findParamValue(xcGroupNode, "TCLSS_MaxCabinLockingTime").as_int();
	xcCfg->tSr.lTCLSS_LockingStatusUndefinedMaxTime = findParamValue(xcGroupNode, "TCLSS_LockingStatusUndefinedMaxTime").as_int();
	xcCfg->tSr.lTCLSS_MaxCabinUnlockingTime = findParamValue(xcGroupNode, "TCLSS_MaxCabinUnlockingTime").as_int();
	xcCfg->tSr.lTCLSS_MaxUndefinedTime = findParamValue(xcGroupNode, "TCLSS_MaxUndefinedTime").as_int();

	xcCfg->tSr.lXCL_MaxLockActuationTime = findParamValue(xcGroupNode, "XCL_MaxLockActuationTime").as_int();
	xcCfg->tSr.lXCL_MaxUnlockActuationTime = findParamValue(xcGroupNode, "XCL_MaxUnlockActuationTime").as_int();
	xcCfg->tSr.lXCL_MaxBlockActuationTime = findParamValue(xcGroupNode, "XCL_MaxBlockActuationTime").as_int();
	xcCfg->tSr.lXCL_MaxUnblockActuationTime = findParamValue(xcGroupNode, "XCL_MaxUnblockActuationTime").as_int();
	xcCfg->tSr.lXCL_MaxUndefinedTime = findParamValue(xcGroupNode, "XCL_MaxUndefinedTime").as_int();

	xcCfg->tSr.lXCSN_WarnTemp = findParamValue(xcGroupNode, "XCSN_WarnTemp").as_int();


	pugi::xml_node stoppingDistanceNode = doc.child("config").child("stoppingDistanceParamXc");
	// ---------- SET [0]: clockwise-clockwise ----------
	{
		auto nodes = stoppingDistanceNode.children("XC_SD_Clockwise");
		auto it = nodes.begin();

		// Clockwise for ParamSet[0]
		if (it != nodes.end()) {
			pugi::xml_node node = *it;
			xcCfg->tXc.tXC_SD_ParamSet[0].tXC_SD_clockwise.lXC_K1 = UPSCALE_K1(findParamValue(node, "K1").as_double());
			xcCfg->tXc.tXC_SD_ParamSet[0].tXC_SD_clockwise.lXC_K2 = UPSCALE_K2(findParamValue(node, "K2").as_double());
			xcCfg->tXc.tXC_SD_ParamSet[0].tXC_SD_clockwise.lXC_Offset = UPSCALE_KOFFSET(findParamValue(node, "Offset").as_double());
			++it;
		}

		// CounterClockwise for ParamSet[0]
		if (it != nodes.end()) {
			pugi::xml_node node = *it;
			xcCfg->tXc.tXC_SD_ParamSet[0].tXC_SD_counterclockwise.lXC_K1 = UPSCALE_K1(findParamValue(node, "K1").as_double());
			xcCfg->tXc.tXC_SD_ParamSet[0].tXC_SD_counterclockwise.lXC_K2 = UPSCALE_K2(findParamValue(node, "K2").as_double());
			xcCfg->tXc.tXC_SD_ParamSet[0].tXC_SD_counterclockwise.lXC_Offset = UPSCALE_KOFFSET(findParamValue(node, "Offset").as_double());
		}
	}

	// ---------- SET [1]: counter-counter ----------
	{
		auto nodes = stoppingDistanceNode.children("XC_SD_CounterClockwise");
		auto it = nodes.begin();

		// Clockwise for ParamSet[1]
		if (it != nodes.end()) {
			pugi::xml_node node = *it;
			xcCfg->tXc.tXC_SD_ParamSet[1].tXC_SD_clockwise.lXC_K1 = UPSCALE_K1(findParamValue(node, "K1").as_double());
			xcCfg->tXc.tXC_SD_ParamSet[1].tXC_SD_clockwise.lXC_K2 = UPSCALE_K2(findParamValue(node, "K2").as_double());
			xcCfg->tXc.tXC_SD_ParamSet[1].tXC_SD_clockwise.lXC_Offset = UPSCALE_KOFFSET(findParamValue(node, "Offset").as_double());
			++it;
		}

		// CounterClockwise for ParamSet[1]
		if (it != nodes.end()) {
			pugi::xml_node node = *it;
			xcCfg->tXc.tXC_SD_ParamSet[1].tXC_SD_counterclockwise.lXC_K1 = UPSCALE_K1(findParamValue(node, "K1").as_double());
			xcCfg->tXc.tXC_SD_ParamSet[1].tXC_SD_counterclockwise.lXC_K2 = UPSCALE_K2(findParamValue(node, "K2").as_double());
			xcCfg->tXc.tXC_SD_ParamSet[1].tXC_SD_counterclockwise.lXC_Offset = UPSCALE_KOFFSET(findParamValue(node, "Offset").as_double());
		}
	}

	int i = 0;
	for (auto xcSfNode : doc.child("config").child("SF_enable").children("XC_SF"))
	{
		if (i >= 8) break;

		int position = findParamValue(xcSfNode, "Id").as_int();
		position = position - 1;
		int Segment = findParamValue(xcSfNode, "value").as_int();

		xcCfg->usXCSN_Sf_Enable[position] = static_cast<uint16_t>(Segment);
		++i;
	}


	i = 0;
	for (auto xcSRNode : doc.child("config").child("SR_enable").children("XC_SR"))
	{
		if (i >= NUMBER_OF_XC_SSR_ON_SN) break;

		int position = findParamValue(xcSRNode, "Id").as_int();
		position = position - 1;
		int Segment = findParamValue(xcSRNode, "value").as_int();
		xcCfg->tSr.usXCSN_Ssr_Enable[position] = static_cast<uint16_t>(Segment);
		++i;
	}

	xcCfg->Spare1[0] = 0;
	xcCfg->Spare1[1] = 0;
	xcCfg->Spare1[2] = 0;

	xcCfg->ulCrc = Utility::crc32Checksum(sizeof(xcCfg_t) - sizeof(uint32_t), (uint8_t*)&xcCfg->tConstants, 0);
	

}

void ExportData::buildLcuConfig(lcuCfg_t* lcuCfg, tLcuId_t tLcuId) {
	using namespace std;

	/* build a master config to clone from */
	buildMasterConfig(&mstCfg);

	memset(lcuCfg, 0, sizeof(lcuCfg_t));

	pugi::xml_node node = doc.child("config").child("general");

	lcuCfg->tConstants.tConfigVersion = findParamValue(node, "ConfigVersion").as_int();

	lcuCfg->tLcuId = tLcuId;
	//lcuCfg->tLcuId = static_cast<tLcuId_t>(tLcuId);

	/* populate lcuCfg using values from mstCfg */

	// Find the root <lcus> element
	pugi::xml_node lcusNode = doc.child("config").child("lcus");
	// Create a map to store parameter values
	std::map<std::string, std::string> parameterMap;


	pugi::xml_node lcusGroupNode = lcusNode.child("lcu-group");

	lcuCfg->tConstants.lMULTI_NumberOfXCInstalled = mstCfg.tConstants.lMULTI_NumberOfXCInstalled;
	lcuCfg->tConstants.lMULTI_NumberOfGarageAreasInstalled = mstCfg.tConstants.lMULTI_NumberOfGarageAreasInstalled;
	lcuCfg->tConstants.lMULTI_NumberOfVerticalTracksInstalled = mstCfg.tConstants.lMULTI_NumberOfVerticalTracksInstalled;
	lcuCfg->tConstants.lMULTI_NumberOfHorizontalTracksInstalled = mstCfg.tConstants.lMULTI_NumberOfHorizontalTracksInstalled;
	lcuCfg->tConstants.lMULTI_NumberOfMaintenanceAreasInstalled = mstCfg.tConstants.lMULTI_NumberOfMaintenanceAreasInstalled;
	lcuCfg->tConstants.lMULTI_NumberOfWellDoorsInstalled = mstCfg.tConstants.lMULTI_NumberOfWellDoorsInstalled;
	lcuCfg->tConstants.lMULTI_NumberOfWellAccessPortsInstalled = mstCfg.tConstants.lMULTI_NumberOfWellAccessPortsInstalled;
	lcuCfg->tConstants.ulConfigJobSpecificId = mstCfg.tConstants.ulConfigJobSpecificId;
	lcuCfg->tConstants.lLutFn_NumberOfEntriesMax = findParamValue(lcusGroupNode, "LutFn_NumberOfEntriesMax").as_int();
	lcuCfg->tConstants.lLutXtarget_NumberOfEntriesMax = findParamValue(lcusGroupNode, "LutXtarget_NumberOfEntriesMax").as_int();
	lcuCfg->tConstants.lLCUBrake_CalibrationCountRisingThreshold = findParamValue(lcusGroupNode, "LCUBrake_CalibrationCountRisingThreshold").as_int();
	lcuCfg->tConstants.lLCUBrake_CalibrationCountFallingThreshold = findParamValue(lcusGroupNode, "LCUBrake_CalibrationCountFallingThreshold").as_int();
	lcuCfg->tConstants.lLCUBrake_CalibrationDeltaP = findParamValue(lcusGroupNode, "LCUBrake_CalibrationDeltaP").as_int();

	lcuCfg->tLcu.lLCUBrake_x_c_trg_min_area_of_evaluation_parameter = findParamValue(lcusGroupNode, "LCUBrake_x_c_trg_min_area_of_evaluation_parameter").as_int();
	lcuCfg->tLcu.lLCUBrake_x_c_trg_max_area_of_evaluation_parameter = findParamValue(lcusGroupNode, "LCUBrake_x_c_trg_max_area_of_evaluation_parameter").as_int();

	lcuCfg->tConstants.ucSpare[0] = 0;
	lcuCfg->tConstants.ucSpare[1] = 0;

	//lcuCfg->tLcu.cType = mstCfg.tLcu.cType[lcuCfg->tLcuId];
	lcuCfg->tLcu.cType = 0;

	memcpy(lcuCfg->tLcu.cPositionTranslation_TrackIsVertical, mstCfg.tLcu.cPositionTranslation_TrackIsVertical,
		sizeof(lcuCfg->tLcu.cPositionTranslation_TrackIsVertical));
	memcpy(lcuCfg->tLcu.lPositionTranslation_CenterAlignmentPosAlongTrack,
		mstCfg.tLcu.lPositionTranslation_CenterAlignmentPosAlongTrack,
		sizeof(lcuCfg->tLcu.lPositionTranslation_CenterAlignmentPosAlongTrack));
	memcpy(lcuCfg->tLcu.tPhysicalConstraints, mstCfg.tLcu.tPhysicalConstraints,
		sizeof(lcuCfg->tLcu.tPhysicalConstraints));

	memcpy(lcuCfg->tLcu.tStoppingDistanceParamSet.LoSD_upwards_BS, mstCfg.tLcu.tStoppingDistanceParamSet.LoSD_upwards_BS, sizeof(lcuCfg->tLcu.tStoppingDistanceParamSet.LoSD_upwards_BS));
	memcpy(lcuCfg->tLcu.tStoppingDistanceParamSet.LoSD_downwards_BS, mstCfg.tLcu.tStoppingDistanceParamSet.LoSD_downwards_BS, sizeof(lcuCfg->tLcu.tStoppingDistanceParamSet.LoSD_downwards_BS));
	memcpy(lcuCfg->tLcu.tStoppingDistanceParamSet.USD_downwards_BS, mstCfg.tLcu.tStoppingDistanceParamSet.USD_downwards_BS, sizeof(lcuCfg->tLcu.tStoppingDistanceParamSet.USD_downwards_BS));
	memcpy(lcuCfg->tLcu.tStoppingDistanceParamSet.USD_upwards_BS, mstCfg.tLcu.tStoppingDistanceParamSet.USD_upwards_BS, sizeof(lcuCfg->tLcu.tStoppingDistanceParamSet.USD_upwards_BS));
	memcpy(lcuCfg->tLcu.tStoppingDistanceParamSet.LeSD_rightwards_BS, mstCfg.tLcu.tStoppingDistanceParamSet.LeSD_rightwards_BS, sizeof(lcuCfg->tLcu.tStoppingDistanceParamSet.LeSD_rightwards_BS));
	memcpy(lcuCfg->tLcu.tStoppingDistanceParamSet.LeSD_leftwards_BS, mstCfg.tLcu.tStoppingDistanceParamSet.LeSD_leftwards_BS, sizeof(lcuCfg->tLcu.tStoppingDistanceParamSet.LeSD_leftwards_BS));
	memcpy(lcuCfg->tLcu.tStoppingDistanceParamSet.RSD_rightwards_BS, mstCfg.tLcu.tStoppingDistanceParamSet.RSD_rightwards_BS, sizeof(lcuCfg->tLcu.tStoppingDistanceParamSet.RSD_rightwards_BS));
	memcpy(lcuCfg->tLcu.tStoppingDistanceParamSet.RSD_leftwards_BS, mstCfg.tLcu.tStoppingDistanceParamSet.RSD_leftwards_BS, sizeof(lcuCfg->tLcu.tStoppingDistanceParamSet.RSD_leftwards_BS));

	/* just copy fix protection areas from master */
	memcpy(lcuCfg->tProtectionAreas.tTranslationalMovementAreas,
		mstCfg.tProtectionAreas.tTranslationalMovementAreas,
		sizeof(lcuCfg->tProtectionAreas.tTranslationalMovementAreas));

	memcpy(lcuCfg->tProtectionAreas.tMaintenanceAreas,
		mstCfg.tProtectionAreas.tMaintenanceAreas,
		sizeof(lcuCfg->tProtectionAreas.tMaintenanceAreas));

	memcpy(lcuCfg->tProtectionAreas.tGarageAreas,
		mstCfg.tProtectionAreas.tGarageAreas,
		sizeof(lcuCfg->tProtectionAreas.tGarageAreas));
	/*...*/

	std::stringstream stream;

	// Set the output format to fixed-point with 7 decimal places
	stream << std::fixed << std::setprecision(7) << findParamValue(lcusGroupNode, "LoadweighingFactor").as_string();

	// Convert the stringstream content to a float

	lcuCfg->tLcu.lParkingBrakeForceThreshold = findParamValue(lcusGroupNode, "ParkingBrakeForceThreshold").as_int();
	lcuCfg->tLcu.lPrimaryBrakeStepFullActivationThreshold = findParamValue(lcusGroupNode, "PrimaryBrakeStepFullActivationThreshold").as_int();
	lcuCfg->tLcu.lSpeedHorizNominal = findParamValue(lcusGroupNode, "SpeedHorizNominal").as_int();
	lcuCfg->tLcu.lSpeedVertNominal = findParamValue(lcusGroupNode, "SpeedVertNominal").as_int();
	lcuCfg->tLcu.lUpAccelNominal = findParamValue(lcusGroupNode, "UpAccelNominal").as_int();
	lcuCfg->tLcu.lDownAccelNominal = findParamValue(lcusGroupNode, "DownAccelNominal").as_int();
	lcuCfg->tLcu.lHorizAccelNominal = findParamValue(lcusGroupNode, "HorizAccelNominal").as_int();
	lcuCfg->tLcu.lEmergencyAutonomVertDecelMin = findParamValue(lcusGroupNode, "EmergencyAutonomVertDecelMin").as_int();
	lcuCfg->tLcu.lEmergencyAutonomVertDecelMax = findParamValue(lcusGroupNode, "EmergencyAutonomVertDecelMax").as_int();
	lcuCfg->tLcu.lControlledHorizDecelMin = findParamValue(lcusGroupNode, "ControlledHorizDecelMin").as_int();
	lcuCfg->tLcu.lControlledHorizDecelModerate = findParamValue(lcusGroupNode, "ControlledHorizDecelModerate").as_int();
	lcuCfg->tLcu.lForcedHorizDecelMin = findParamValue(lcusGroupNode, "ForcedHorizDecelMin").as_int();
	lcuCfg->tLcu.lForcedDownDecelMin = findParamValue(lcusGroupNode, "ForcedDownDecelMin").as_int();
	lcuCfg->tLcu.lPayloadDetectionThreshold = findParamValue(lcusGroupNode, "PayloadDetectionThreshold").as_int();
	lcuCfg->tLcu.lUpAccelMax = findParamValue(lcusGroupNode, "UpAccelMax").as_int();
	lcuCfg->tLcu.lDownAccelMax = findParamValue(lcusGroupNode, "DownAccelMax").as_int();
	lcuCfg->tLcu.lHorizAccelMax = findParamValue(lcusGroupNode, "HorizAccelMax").as_int();
	lcuCfg->tLcu.lUpDecelMax = findParamValue(lcusGroupNode, "UpDecelMax").as_int();
	lcuCfg->tLcu.lDownDecelMax = findParamValue(lcusGroupNode, "DownDecelMax").as_int();
	lcuCfg->tLcu.lHorizDecelMax = findParamValue(lcusGroupNode, "HorizDecelMax").as_int();

	lcuCfg->tLcu.lVelocityEcasPreBrakeMax = findParamValue(lcusGroupNode, "VelocityEcasPreBrakeMax").as_int();
	lcuCfg->tLcu.lVelocityNcasPreBrakeMax = findParamValue(lcusGroupNode, "VelocityNcasPreBrakeMax").as_int();
	lcuCfg->tLcu.lVelocitySfoPreBrakeMax = findParamValue(lcusGroupNode, "VelocitySfoPreBrakeMax").as_int();
	lcuCfg->tLcu.lVelocityNcasMin = findParamValue(lcusGroupNode, "VelocityNcasMin").as_int();
	lcuCfg->tLcu.lVelocityEcasMin = findParamValue(lcusGroupNode, "VelocityEcasMin").as_int();
	lcuCfg->tLcu.lVelocityFES = findParamValue(lcusGroupNode, "VelocityFES").as_int();

	lcuCfg->tLcu.lOverloadThreshold = findParamValue(lcusGroupNode, "OverloadThreshold").as_int();
	lcuCfg->tLcu.lCrushloadThreshold = findParamValue(lcusGroupNode, "CrushloadThreshold").as_int();
	lcuCfg->tLcu.lPayload75kg = findParamValue(lcusGroupNode, "Payload75kg").as_int();
	lcuCfg->tLcu.lPayload225kg = findParamValue(lcusGroupNode, "Payload225kg").as_int();
	lcuCfg->tLcu.lPayload375kg = findParamValue(lcusGroupNode, "Payload375kg").as_int();
	lcuCfg->tLcu.lPayload525kg = findParamValue(lcusGroupNode, "Payload525kg").as_int();
	lcuCfg->tLcu.lPayload675kg = findParamValue(lcusGroupNode, "Payload675kg").as_int();
	lcuCfg->tLcu.lPayload825kg = findParamValue(lcusGroupNode, "Payload825kg").as_int();
	lcuCfg->tLcu.lPayloadMax_EconomyLCU = findParamValue(lcusGroupNode, "PayloadMax_EconomyLCU").as_int();
	lcuCfg->tLcu.lPayloadMax_PremiumLCU = findParamValue(lcusGroupNode, "PayloadMax_PremiumLCU").as_int();
	lcuCfg->tLcu.lPayloadMaxHyst = findParamValue(lcusGroupNode, "PayloadMaxHyst").as_int();
	lcuCfg->tLcu.lPayloadCrushed = findParamValue(lcusGroupNode, "PayloadCrushed").as_int();
	lcuCfg->tLcu.lPayloadCrushedHyst = findParamValue(lcusGroupNode, "PayloadCrushedHyst").as_int();
	lcuCfg->tLcu.lLoadedWeightMax = findParamValue(lcusGroupNode, "LoadedWeightMax").as_int();
	lcuCfg->tLcu.lLoadedWeightMaxHyst = findParamValue(lcusGroupNode, "LoadedWeightMaxHyst").as_int();
	lcuCfg->tLcu.lEmptyWeightMax_EconomyLCU = findParamValue(lcusGroupNode, "EmptyWeightMax_EconomyLCU").as_int();
	lcuCfg->tLcu.lEmptyWeightMax_PremiumLCU = findParamValue(lcusGroupNode, "EmptyWeightMax_PremiumLCU").as_int();
	lcuCfg->tLcu.lAmbientTempMax = findParamValue(lcusGroupNode, "AmbientTempMax").as_int();
	lcuCfg->tLcu.lAmbientTempMaxHyst = findParamValue(lcusGroupNode, "AmbientTempMaxHyst").as_int();
	lcuCfg->tLcu.lAvgEmptyWeightDigit_EconomyLCU = findParamValue(lcusGroupNode, "AvgEmptyWeightDigit_EconomyLCU").as_int();
	lcuCfg->tLcu.lAvgEmptyWeightDigit_PremiumLCU = findParamValue(lcusGroupNode, "AvgEmptyWeightDigit_PremiumLCU").as_int();
	lcuCfg->tLcu.lAvgEmptyWeightKg_EconomyLCU = findParamValue(lcusGroupNode, "AvgEmptyWeightKg_EconomyLCU").as_int();
	lcuCfg->tLcu.lAvgEmptyWeightKg_PremiumLCU = findParamValue(lcusGroupNode, "AvgEmptyWeightKg_PremiumLCU").as_int();
	lcuCfg->tLcu.lAvgOverloadWeightDigit_EconomyLCU = findParamValue(lcusGroupNode, "AvgOverloadWeightDigit_EconomyLCU").as_int();
	lcuCfg->tLcu.lAvgOverloadWeightDigit_PremiumLCU = findParamValue(lcusGroupNode, "AvgOverloadWeightDigit_PremiumLCU").as_int();
	lcuCfg->tLcu.lAvgOverloadWeightKg_EconomyLCU = findParamValue(lcusGroupNode, "AvgOverloadWeightKg_EconomyLCU").as_int();
	lcuCfg->tLcu.lAvgOverloadWeightKg_PremiumLCU = findParamValue(lcusGroupNode, "AvgOverloadWeightKg_PremiumLCU").as_int();

	lcuCfg->tLcu.lBrakePositionOpenThreshold = findParamValue(lcusGroupNode, "BrakePositionOpenThreshold").as_int();

	/* ... for testing ... */
	lcuCfg->tLcu.lAps_maxAccelDeviation = findParamValue(lcusGroupNode, "APS_MaxAccelDeviation").as_int();
	lcuCfg->tLcu.lAps_gravityOffset_1A = findParamValue(lcusGroupNode, "APS_gravityOffset_1A").as_int();
	lcuCfg->tLcu.lAps_gravityOffset_2A = findParamValue(lcusGroupNode, "APS_gravityOffset_2A").as_int();
	lcuCfg->tLcu.lAps_gravityOffset_1B = findParamValue(lcusGroupNode, "APS_gravityOffset_1B").as_int();
	lcuCfg->tLcu.lAps_gravityOffset_2B = findParamValue(lcusGroupNode, "APS_gravityOffset_2B").as_int();
	lcuCfg->tLcu.lAps_AccSensorOffset = findParamValue(lcusGroupNode, "Aps_AccSensorOffset").as_int();

	lcuCfg->tLcu.lSpeedHorizMin = findParamValue(lcusGroupNode, "SpeedHorizMin").as_int();
	lcuCfg->tLcu.lSpeedVertMin = findParamValue(lcusGroupNode, "SpeedVertMin").as_int();

	/* brake escalation thresholds */
	lcuCfg->tLcu.lControlledDownDecelMin = findParamValue(lcusGroupNode, "ControlledDownDecelMin").as_int();
	lcuCfg->tLcu.lControlledDownDecelModerate = findParamValue(lcusGroupNode, "ControlledDownDecelModerate").as_int();

	lcuCfg->tLcu.lNormalAutonomDownDecelMin = findParamValue(lcusGroupNode, "NormalAutonomDownDecelMin").as_int();
	lcuCfg->tLcu.lNormalAutonomDownDecelMax = findParamValue(lcusGroupNode, "NormalAutonomDownDecelMax").as_int();

	lcuCfg->tLcu.lNormalAutonomHorizDecelMin = findParamValue(lcusGroupNode, "NormalAutonomHorizDecelMin").as_int();
	lcuCfg->tLcu.lNormalAutonomHorizDecelMax = findParamValue(lcusGroupNode, "NormalAutonomHorizDecelMax").as_int();

	lcuCfg->tLcu.lNormalAutonomUpDecelMin = findParamValue(lcusGroupNode, "NormalAutonomUpDecelMin").as_int();
	lcuCfg->tLcu.lNormalAutonomUpDecelMax = findParamValue(lcusGroupNode, "NormalAutonomUpDecelMax").as_int();

	/*
	 *  horizontal movement --> Target deceleration: 0.2g
	 *  vertical movement --> Target deceleration: 0.7g
	 */
	lcuCfg->tLcu.lTargetDeceleration_Horizontal = findParamValue(lcusGroupNode, "TargetDecelerationHorizontal").as_int(); /* unit is mm/s/s */
	lcuCfg->tLcu.lTargetDeceleration_Down = findParamValue(lcusGroupNode, "TargetDecelerationDown").as_int(); /* unit is mm/s/s */

	/* brake actuation times (in milliseconds) */
	lcuCfg->tLcu.lNormalCasActuationTime = findParamValue(lcusGroupNode, "NormalCASActuationTime").as_int();
	lcuCfg->tLcu.lEmergencyCasActuationTime = findParamValue(lcusGroupNode, "EmergencyCASActuationTime").as_uint();
	lcuCfg->tLcu.lControlledPrimaryBrakeActuationTime = findParamValue(lcusGroupNode, "ControlledPrimaryBrakeActuationTime").as_int();
	lcuCfg->tLcu.lForcedPrimaryBrakeActuationTime = findParamValue(lcusGroupNode, "ForcedPrimaryBrakeActuationTime").as_int();
	lcuCfg->tLcu.lForcedRedundantBrakeActuationTime = findParamValue(lcusGroupNode, "ForcedRedundantBrakeActuationTime").as_int();
	lcuCfg->tLcu.lSfoActuationTime = findParamValue(lcusGroupNode, "SFOActuationTime").as_int();
	lcuCfg->tLcu.lLCU_NoMovementTime = findParamValue(lcusGroupNode, "LCU_NoMovementTime").as_int();
	lcuCfg->tLcu.lLCU_LoadBrakeCollectiveNextEventCountTime = findParamValue(lcusGroupNode, "LCU_LoadBrakeCollectiveNextEventCountTime").as_int();
	lcuCfg->tLcu.lLCU_LoadBrakeCollectiveDebouncingCyclesTime = findParamValue(lcusGroupNode, "LCU_LoadBrakeCollectiveDebouncingCyclesTime").as_int();
	lcuCfg->tLcu.lPBS_SafetyValvesDeEnergizedTime = findParamValue(lcusGroupNode, "LCU_PBS_SafetyValvesDeEnergizedTime").as_int();

	/* config values for Speed limits (see SF_LCU_036)
	 * Just set a dummy value of 50 to be above speed values from Lcu*/
	lcuCfg->tLcu.lSpeedLoadedHorizMax = findParamValue(lcusGroupNode, "SpeedLoadedHorizMax").as_int();
	lcuCfg->tLcu.lSpeedUnloadedHorizMax = findParamValue(lcusGroupNode, "SpeedUnloadedHorizMax").as_int();
	lcuCfg->tLcu.lSpeedVertMax = findParamValue(lcusGroupNode, "SpeedVertMax").as_int();
	lcuCfg->tLcu.lSpeedInManualOpHorizMax = findParamValue(lcusGroupNode, "SpeedInManualOpHorizMax").as_int();
	lcuCfg->tLcu.lSpeedInManualOpVertMax = findParamValue(lcusGroupNode, "SpeedInManualOpVertMax").as_int();
	lcuCfg->tLcu.lSlidingBrakeSpeedMax = findParamValue(lcusGroupNode, "SlidingBrakeSpeedMax").as_int();

	/* Only dummy-values to enable local operation */
	lcuCfg->tSr.lLCUAPS_MaxTravelDistanceWithCalculatedPosition = findParamValue(lcusGroupNode, "APS_MaxTravelDistanceWithCalculatedPosition").as_int();
	lcuCfg->tSr.lLCUAPS_MaxTravelTimeWithCalculatedPosition = findParamValue(lcusGroupNode, "APS_MaxTravelTimeWithCalculatedPosition").as_int();
	lcuCfg->tSr.lLCUAPS_MaxTravelTimeWithCalculatedVelocity = findParamValue(lcusGroupNode, "APS_MaxTravelTimeWithCalculatedVelocity").as_int();
	lcuCfg->tSr.lLCUAPS_MaxTravelTimeWithInjectedVelocity = findParamValue(lcusGroupNode, "APS_MaxTravelTimeWithInjectedVelocity").as_int();
	lcuCfg->tSr.lLCUAPS_MinSpeedWithExtrapolation = findParamValue(lcusGroupNode, "APS_MinSpeedWithExtrapolation").as_int();

	lcuCfg->tSr.lLCUBRK_WearoutLevelWarn = findParamValue(lcusGroupNode, "BRK_WearoutLevelWarn").as_int();
	lcuCfg->tSr.lLCUBRK_WearoutLevelError = findParamValue(lcusGroupNode, "BRK_WearoutLevelError").as_int();
	lcuCfg->tSr.lLCUBRK_FrictionLevelWarn = findParamValue(lcusGroupNode, "BRK_FrictionLevelWarn").as_int();
	lcuCfg->tSr.lLCUBRK_FrictionLevelError = findParamValue(lcusGroupNode, "BRK_FrictionLevelError").as_int();
	lcuCfg->tSr.lLCUBRK_StepForceDelta = findParamValue(lcusGroupNode, "BRK_StepForceDelta").as_int();
	lcuCfg->tSr.lLCUBRK_MaxRedundantBrakeFailureTime = findParamValue(lcusGroupNode, "BRK_MaxRedundantBrakeFailureTime").as_int();
	lcuCfg->tSr.lLCUBRK_MaxUpwardBrakeOpCount = findParamValue(lcusGroupNode, "BRK_MaxUpwardBrakeOpCount").as_int();
	lcuCfg->tSr.lLCUBRK_MaxPrimaryUBPLockingTime = findParamValue(lcusGroupNode, "BRK_MaxPrimaryUBPLockingTime").as_int();
	lcuCfg->tSr.lLCUBRK_MaxRedundantUBPLockingTime = findParamValue(lcusGroupNode, "BRK_MaxRedundantUBPLockingTime").as_int();
	lcuCfg->tSr.lLCUBRK_MaxPrimaryUBPUnlockingTime = findParamValue(lcusGroupNode, "BRK_MaxPrimaryUBPUnlockingTime").as_int();
	lcuCfg->tSr.lLCUBRK_MaxRedundantUBPUnlockingTime = findParamValue(lcusGroupNode, "BRK_MaxRedundantUBPUnlockingTime").as_int();
	lcuCfg->tSr.lLCUBSS_MaxBrakeSafetySensorFailureTime = findParamValue(lcusGroupNode, "BSS_MaxBrakeSafetySensorFailureTime").as_int();
	lcuCfg->tSr.lLCUBSS_MaxBrakeReleaseTimeWarn = findParamValue(lcusGroupNode, "BSS_MaxBrakeReleaseTimeWarn").as_int();
	lcuCfg->tSr.lLCUBSS_MaxBrakeReleaseTimeError = findParamValue(lcusGroupNode, "BSS_MaxBrakeReleaseTimeError").as_int();
	lcuCfg->tSr.lLCUBSS_MaxBrakeActuationTimeWarn = findParamValue(lcusGroupNode, "BSS_MaxBrakeActuationTimeWarn").as_int();
	lcuCfg->tSr.lLCUBSS_MaxBrakeActuationTimeError = findParamValue(lcusGroupNode, "BSS_MaxBrakeActuationTimeError").as_int();
	lcuCfg->tSr.lLCUBSS_dXtargetTolerance = findParamValue(lcusGroupNode, "BSS_dXtargetTolerance").as_int();
	lcuCfg->tSr.lLCUBSS_dXcontactTolerance = findParamValue(lcusGroupNode, "BSS_dXcontactTolerance").as_int();
	lcuCfg->tSr.lLCUBSS_PcontactMin = findParamValue(lcusGroupNode, "BSS_PcontactMin").as_int();
	lcuCfg->tSr.lLCUBSS_PcontactMax = findParamValue(lcusGroupNode, "BSS_PcontactMax").as_int();
	lcuCfg->tSr.lLCUCBSS_CabinLockingMaxBlockingTime = findParamValue(lcusGroupNode, "CBSS_CabinLockingMaxBlockingTime").as_int();
	lcuCfg->tSr.lLCUCBSS_CabinLockingMaxUnblockingTime = findParamValue(lcusGroupNode, "CBSS_CabinLockingMaxUnblockingTime").as_int();
	lcuCfg->tSr.lLCUCLSS_MaxLCUCabinLockingTime = findParamValue(lcusGroupNode, "CLSS_MaxLCUCabinLockingTime").as_int();
	lcuCfg->tSr.lLCUCLSS_MaxLCUCabinUnlockingTime = findParamValue(lcusGroupNode, "CLSS_MaxLCUCabinUnlockingTime").as_int();
	lcuCfg->tSr.lLCUCLSS_CabinLockingStatusUndefinedMaxTime = findParamValue(lcusGroupNode, "CLSS_CabinLockingStatusUndefinedMaxTime").as_int();
	lcuCfg->tSr.lLCUCTS_DiffOverwriteErrorTemp = findParamValue(lcusGroupNode, "CTS_DiffOverwriteErrorTemp").as_int();
	lcuCfg->tSr.lLCUCTS_DiffOverwriteWarnTemp = findParamValue(lcusGroupNode, "CTS_DiffOverwriteWarnTemp").as_int();
	lcuCfg->tSr.lLCUCTS_MaxDiffValue = findParamValue(lcusGroupNode, "CTS_MaxDiffValue").as_int();
	lcuCfg->tSr.lLCUCTS_MaxDiffTime = findParamValue(lcusGroupNode, "CTS_MaxDiffTime").as_int();
	lcuCfg->tSr.lLCUCTS_MaxZeroTime = findParamValue(lcusGroupNode, "CTS_MaxZeroTime").as_int();
	lcuCfg->tSr.lLCUCTS_ZeroOverwriteErrorTemp = findParamValue(lcusGroupNode, "CTS_ZeroOverwriteErrorTemp").as_int();
	lcuCfg->tSr.lLCUCTS_ZeroOverwriteWarnTemp = findParamValue(lcusGroupNode, "CTS_ZeroOverwriteWarnTemp").as_int();
	lcuCfg->tSr.lLCULW_MaxDiffValue = findParamValue(lcusGroupNode, "LW_MaxDiffValue").as_int();
	lcuCfg->tSr.lLCULW_MinValue = findParamValue(lcusGroupNode, "LW_MinValue").as_int();
	lcuCfg->tSr.lLCULW_MaxValue = findParamValue(lcusGroupNode, "LW_MaxValue").as_int();
	lcuCfg->tSr.lLCULW_CrushLoadValue = findParamValue(lcusGroupNode, "LW_CrushLoadValue").as_int();
	lcuCfg->tSr.lLCUSN_MaxTemperature = findParamValue(lcusGroupNode, "SN_MaxTemperature").as_int();
	lcuCfg->tSr.lLCUSN_PrimaryBrakeActiveWithoutCommandMaxTime = findParamValue(lcusGroupNode, "SN_PrimaryBrakeActiveWithoutCommandMaxTime").as_int();
	lcuCfg->tSr.lLCUSN_ReactionModuleSignalDelayTime = findParamValue(lcusGroupNode, "SN_ReactionModuleSignalDelayTime").as_int();
	lcuCfg->tSr.lSCTS_MaxErrorTime = findParamValue(lcusGroupNode, "SCTS_MaxErrorTime").as_int();
	lcuCfg->tSr.lSCTS_ErrorOverwriteErrorTemp = findParamValue(lcusGroupNode, "SCTS_ErrorOverwriteErrorTemp").as_int();
	lcuCfg->tSr.lSCTS_ErrorOverwriteWarnTemp = findParamValue(lcusGroupNode, "SCTS_ErrorOverwriteWarnTemp").as_int();

	pugi::xml_node lcusLoadNode = lcusNode.child("lcu-load");

	lcuCfg->tLcu.tCollectiveLoadMax.usCASVerticalNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "CASVerticalNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usCASHorizontalNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "CASHorizontalNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usECASVerticalNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "ECASVerticalNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usCESBrakingVerticalNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "CESBrakingVerticalNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usCESBrakingHorizontalNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "CESBrakingHorizontalNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usFESBrakingExtremeVerticalNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "FESBrakingExtremeVerticalNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usFESBrakingVerticalNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "FESBrakingVerticalNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usFESBrakingHorizontalNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "FESBrakingHorizontalNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usOverloadNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "OverloadNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usCrushloadNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "CrushloadNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usJumpingPassengerNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "JumpingPassengerNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.ulPayloadChangeNrOfEvents = findParamValue(lcusLoadNode, "PayloadChangeNrOfEvents").as_uint();
	lcuCfg->tLcu.tCollectiveLoadMax.usLcuTooFastInBufferAreaNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "LcuTooFastInBufferAreaNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.ulBrk1ActivationWhenStoppedNrOfEvents = findParamValue(lcusLoadNode, "Brk1ActivationWhenStoppedNrOfEvents").as_uint();
	lcuCfg->tLcu.tCollectiveLoadMax.ulBrk2ActivationWhenStoppedNrOfEvents = findParamValue(lcusLoadNode, "Brk2ActivationWhenStoppedNrOfEvents").as_uint();
	lcuCfg->tLcu.tCollectiveLoadMax.ulBrk3ActivationWhenStoppedNrOfEvents = findParamValue(lcusLoadNode, "Brk3ActivationWhenStoppedNrOfEvents").as_uint();
	lcuCfg->tLcu.tCollectiveLoadMax.ulBrk4ActivationWhenStoppedNrOfEvents = findParamValue(lcusLoadNode, "Brk4ActivationWhenStoppedNrOfEvents").as_uint();
	lcuCfg->tLcu.tCollectiveLoadMax.usBrk1ActivationWhenMovingUpNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "Brk1ActivationWhenMovingUpNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usBrk2ActivationWhenMovingUpNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "Brk2ActivationWhenMovingUpNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usBrk3ActivationWhenMovingUpNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "Brk3ActivationWhenMovingUpNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usBrk4ActivationWhenMovingUpNrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "Brk4ActivationWhenMovingUpNrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usCESBrakingBrk1NrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "CESBrakingBrk1NrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usCESBrakingBrk2NrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "CESBrakingBrk2NrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usCESBrakingBrk3NrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "CESBrakingBrk3NrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usCESBrakingBrk4NrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "CESBrakingBrk4NrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usFESBrakingBrk1NrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "FESBrakingBrk1NrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usFESBrakingBrk2NrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "FESBrakingBrk2NrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usFESBrakingBrk3NrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "FESBrakingBrk3NrOfEvents").as_uint());
	lcuCfg->tLcu.tCollectiveLoadMax.usFESBrakingBrk4NrOfEvents = static_cast<uint16_t>(findParamValue(lcusLoadNode, "FESBrakingBrk4NrOfEvents").as_uint());

	pugi::xpath_node_set lcusBrakeNode = lcusNode.select_nodes("//lcu-brake");

	int i = 0;
	for (auto& lcusBrakeNode : lcusNode.children("lcu-brake"))
	{
		if (i < MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_LCU)
		{
			lcuCfg->tLcu.lBrakeStepActiveHydraulicPressureThreshold[i] = findParamValue(lcusBrakeNode, "BrakeStepActiveHydraulicPressureThreshold").as_int();
			lcuCfg->tLcu.lLCUBrake_initialFnMax[i] = findParamValue(lcusBrakeNode, "LCUBrake_initialFnMax").as_int();
			lcuCfg->tLcu.lLCUBrake_initialPcontact[i] = findParamValue(lcusBrakeNode, "LCUBrake_initialPcontact").as_int();
			lcuCfg->tLcu.lLCUBrake_initialXcontact[i] = findParamValue(lcusBrakeNode, "LCUBrake_initialXcontact").as_int();
			lcuCfg->tLcu.lLCUBrake_initialPosition0[i] = findParamValue(lcusBrakeNode, "LCUBrake_initialPosition0").as_int();
			lcuCfg->tLcu.lLCUBrake_x_c_trg_max[i] = findParamValue(lcusBrakeNode, "LCUBrake_x_c_trg_max").as_int();
			lcuCfg->tLcu.lLCUBrake_x_c_trg_min[i] = findParamValue(lcusBrakeNode, "LCUBrake_x_c_trg_min").as_int();
			lcuCfg->tLcu.lLCUBrake_p_c_trg_max[i] = findParamValue(lcusBrakeNode, "LCUBrake_p_c_trg_max").as_int();
			lcuCfg->tLcu.lLCUBrake_p_c_trg_min[i] = findParamValue(lcusBrakeNode, "LCUBrake_p_c_trg_min").as_int();
		}
		++i;
	}


	i = 0;
	for (auto& lcusBrakeLutNode : lcusNode.children("lcu-brakeLut"))
	{
		if (i < LCUBRAKE_LUT_NUMBER_OF_ENTRIES_MAX_LIMIT)
		{
			lcuCfg->tLcu.tLutFn[i].sArg = static_cast<int16_t>(findParamValue(lcusBrakeLutNode, "LutFnArg").as_int());
			lcuCfg->tLcu.tLutFn[i].sValue = static_cast<int16_t>(findParamValue(lcusBrakeLutNode, "LutFnValue").as_int());
			lcuCfg->tLcu.tLutXtarget[i].sArg = static_cast<int16_t>(findParamValue(lcusBrakeLutNode, "LutXtargetArg").as_int());
			lcuCfg->tLcu.tLutXtarget[i].sValue = static_cast<int16_t>(findParamValue(lcusBrakeLutNode, "LutXtargetValue").as_int());
		}
		++i;
	}


	/* APS Config */
	/* minimum config to keep simple system running: */
	uint16_t indexAps;

	pugi::xml_node apsNode = doc.child("config").child("aps");


	lcuCfg->tLcu.lAPS_1A_Offset_Z_left = findParamValue(apsNode, "lAPS_1A_Offset_Z_left").as_int();
	lcuCfg->tLcu.lAPS_1A_Offset_Y_left = findParamValue(apsNode, "lAPS_1A_Offset_Y_left").as_int();
	lcuCfg->tLcu.lAPS_1B_Offset_Z_left = findParamValue(apsNode, "lAPS_1B_Offset_Z_left").as_int();
	lcuCfg->tLcu.lAPS_1B_Offset_Y_left = findParamValue(apsNode, "lAPS_1B_Offset_Y_left").as_int();
	lcuCfg->tLcu.lAPS_2A_Offset_Z_right = findParamValue(apsNode, "lAPS_2A_Offset_Z_right").as_int();
	lcuCfg->tLcu.lAPS_2A_Offset_Y_right = findParamValue(apsNode, "lAPS_2A_Offset_Y_right").as_int();
	lcuCfg->tLcu.lAPS_2B_Offset_Z_right = findParamValue(apsNode, "lAPS_2B_Offset_Z_right").as_int();
	lcuCfg->tLcu.lAPS_2B_Offset_Y_right = findParamValue(apsNode, "lAPS_2B_Offset_Y_right").as_int();

	lcuCfg->tLcu.ulMULTI_NumberOfCodeTapeInstalled = findParamValue(apsNode, "MULTI_NumberOfCodeTapeInstalled").as_uint();


	indexAps = 0;
	for (auto& apsPositionInfoNode : apsNode.children("apspositioninfo")) {

		if (indexAps < APS_NUMBER_OF_CODETAPES_MAX)
		{
			auto indexApsPos = findParamValue(apsPositionInfoNode, "Id").as_uint();
			indexApsPos = indexApsPos - 1;
			lcuCfg->tLcu.tApsPositionInfo[indexApsPos].ucCodeTapeNumber = static_cast<uint8_t>(findParamValue(apsPositionInfoNode, "CodeTapeNumber").as_uint());  // Let the tracks start with 1
			lcuCfg->tLcu.tApsPositionInfo[indexApsPos].ulRawCodeTapeLBLeft = findParamValue(apsPositionInfoNode, "RawCodeTapeLBLeft").as_uint();
			lcuCfg->tLcu.tApsPositionInfo[indexApsPos].ulRawCodeTapeUBLeft = findParamValue(apsPositionInfoNode, "RawCodeTapeUBLeft").as_uint();
			lcuCfg->tLcu.tApsPositionInfo[indexApsPos].ulRawCodeTapeLBRight = findParamValue(apsPositionInfoNode, "RawCodeTapeLBRight").as_uint();
			lcuCfg->tLcu.tApsPositionInfo[indexApsPos].ulRawCodeTapeUBRight = findParamValue(apsPositionInfoNode, "RawCodeTapeUBRight").as_uint();
			lcuCfg->tLcu.tApsPositionInfo[indexApsPos].ulRawCodeTapeLBLeft_Z = findParamValue(apsPositionInfoNode, "RawCodeTapeLBLeft_Z").as_uint();
			lcuCfg->tLcu.tApsPositionInfo[indexApsPos].ulRawCodeTapeUBLeft_Z = findParamValue(apsPositionInfoNode, "RawCodeTapeUBLeft_Z").as_uint();
			lcuCfg->tLcu.tApsPositionInfo[indexApsPos].ulRawCodeTapeLBRight_Z = findParamValue(apsPositionInfoNode, "RawCodeTapeLBRight_Z").as_uint();
			lcuCfg->tLcu.tApsPositionInfo[indexApsPos].ulRawCodeTapeUBRight_Z = findParamValue(apsPositionInfoNode, "RawCodeTapeUBRight_Z").as_uint();

			lcuCfg->tLcu.tApsPositionInfo[indexApsPos].spare[0] = 0;
			lcuCfg->tLcu.tApsPositionInfo[indexApsPos].spare[1] = 0;
			lcuCfg->tLcu.tApsPositionInfo[indexApsPos].spare[2] = 0;
		}
		++indexAps;

	}


	/* copy XC positions and matching tolerance */
	memcpy(lcuCfg->tXc.tXcInfo, mstCfg.tXc.tXcInfo, sizeof(lcuCfg->tXc.tXcInfo));
	lcuCfg->tXc.lXcMatchToleranceY = mstCfg.tXc.lXcMatchToleranceY;
	lcuCfg->tXc.lXcMatchToleranceZ = mstCfg.tXc.lXcMatchToleranceZ;


	i = 0;
	for (auto lcuSfNode : doc.child("config").child("SF_enable").children("LCU_SF"))
	{
		if (i >= 15) break;

		int position = findParamValue(lcuSfNode, "Id").as_int();
		position = position - 1;
		int Segment = findParamValue(lcuSfNode, "value").as_int();

		lcuCfg->usLCUSN_Sf_Enable[position] = static_cast<uint16_t>(Segment);
		++i;
	}


	i = 0;
	for (auto lcuSRNode : doc.child("config").child("SR_enable").children("LCU_SR"))
	{
		if (i >= NUMBER_OF_LCU_SSR_ON_SN) break;

		int position = findParamValue(lcuSRNode, "Id").as_int();
		position = position - 1;
		int Segment = findParamValue(lcuSRNode, "value").as_int();
		lcuCfg->tSr.usLCUSN_Ssr_Enable[position] = static_cast<uint16_t>(Segment);
		++i;
	}

	i = 0;
	for (auto wellAccessPortInfo : doc.child("config").child("WellAccessPorts").children("WellAccessPortInfo"))
	{
		if (i >= MULTI_NUMBER_OF_WELLDOORS_MAX) break;
		auto indexWellAccessPort = findParamValue(wellAccessPortInfo, "Id").as_uint();
		indexWellAccessPort = indexWellAccessPort - 1;
		lcuCfg->tWellAccessPort.tWellAccessPortInfos[indexWellAccessPort].tDoorZone.tTopLeft.lY = findParamValue(wellAccessPortInfo, "DoorZone TopLeft Y").as_int();
		lcuCfg->tWellAccessPort.tWellAccessPortInfos[indexWellAccessPort].tDoorZone.tTopLeft.lZ = findParamValue(wellAccessPortInfo, "DoorZone TopLeft Z").as_int();
		lcuCfg->tWellAccessPort.tWellAccessPortInfos[indexWellAccessPort].tDoorZone.tBottomRight.lY = findParamValue(wellAccessPortInfo, "DoorZone BottomRight Y").as_int();
		lcuCfg->tWellAccessPort.tWellAccessPortInfos[indexWellAccessPort].tDoorZone.tBottomRight.lZ = findParamValue(wellAccessPortInfo, "DoorZone BottomRight Z").as_int();
		i++;
	}

	lcuCfg->ucSpare1[0] = 0;
	lcuCfg->ucSpare1[1] = 0;
	lcuCfg->ucSpare1[2] = 0;

	memcpy(lcuCfg->tWellDoor.tWellDoorInfos, mstCfg.tWellDoor.tWellDoorInfos,
		sizeof(lcuCfg->tWellDoor.tWellDoorInfos));

	lcuCfg->ulCrc = Utility::crc32Checksum(sizeof(lcuCfg_t) - sizeof(uint32_t), (uint8_t*)&lcuCfg->tLcuId, 0);
	//lcuCfg->ulCrc = Utility::crc32Checksum(sizeof(lcuCfg_t) - sizeof(uint32_t), (uint8_t*)&lcuCfg->tConstants, 0);
}

void ExportData::populateMasterConfig_stat_hil(masterCfg_t* mstCfg) {

	using namespace std;

	pugi::xml_node node = doc.child("config").child("general");

	mstCfg->tConstants.tConfigVersion = findParamValue(node, "ConfigVersion").as_int();

	mstCfg->tConstants.lMULTI_NumberOfLCUInstalled = findParamValue(node, "NumberOfLCUInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfXCInstalled = findParamValue(node, "NumberOfXCInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfIOCInstalled = findParamValue(node, "NumberOfIOCInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfVerticalTracksInstalled = findParamValue(node, "NumberOfVerticalTracksInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfHorizontalTracksInstalled = findParamValue(node, "NumberOfHorizontalTracksInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfFireCompartmentsInstalled = findParamValue(node, "NumberOfFireCompartmentsInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfGarageAreasInstalled = findParamValue(node, "NumberOfGarageAreasInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfMaintenanceAreasInstalled = findParamValue(node, "NumberOfMaintenanceAreasInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfPropulsionSectionsInstalled = findParamValue(node, "NumberOfPropulsionSectionsInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfWallPanelAreasInstalled = findParamValue(node, "NumberOfWallPanelAreasInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfWalkwayAreasInstalled = findParamValue(node, "NumberOfWalkwayAreasInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfWellWaterAreasInstalled = findParamValue(node, "NumberOfWellWaterAreasInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfWellDoorsInstalled = findParamValue(node, "NumberOfWellDoorsInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfWellAccessPortsInstalled = findParamValue(node, "NumberOfWellAccessPortsInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfWellLocksInstalled = findParamValue(node, "NumberOfWellLocksInstalled").as_int();
	mstCfg->tConstants.lMULTI_NumberOfWellSegmentsInstalled = findParamValue(node, "NumberOfWellSegmentsInstalled").as_int();
	mstCfg->tConstants.lMSMA_MaxSubsystemInitWaitingTime = findParamValue(node, "MaxSubsystemInitWaitingTime").as_int();
	mstCfg->tConstants.ulConfigJobSpecificId = findParamValue(node, "ConfigJobSpecificId").as_int();
	mstCfg->tConstants.ucSpare[0] = 0;
	mstCfg->tConstants.ucSpare[1] = 0;

	/* stopping distances */
//	mstCfg->tLcu.tStoppingDistanceParamSet = tStoppingDistanceParamSet;

	pugi::xml_node stoppingDistanceNode = doc.child("config").child("stoppingDistanceParam");

	for (auto& losdUpBsNode : stoppingDistanceNode.children("LoSD_upwards_BS"))
	{
		int position = findParamValue(losdUpBsNode, "Id").as_int();
		position = position - 1;
		mstCfg->tLcu.tStoppingDistanceParamSet.LoSD_upwards_BS[position].lK1 = UPSCALE_K1(findParamValue(losdUpBsNode, "K1").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.LoSD_upwards_BS[position].lK2 = UPSCALE_K2(findParamValue(losdUpBsNode, "K2").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.LoSD_upwards_BS[position].lOffset = UPSCALE_KOFFSET(findParamValue(losdUpBsNode, "Offset").as_double());
	}


	for (auto& losdDownBsNode : stoppingDistanceNode.children("LoSD_downwards_BS"))
	{
		int position = findParamValue(losdDownBsNode, "Id").as_int();
		position = position - 1;
		mstCfg->tLcu.tStoppingDistanceParamSet.LoSD_downwards_BS[position].lK1 = UPSCALE_K1(findParamValue(losdDownBsNode, "K1").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.LoSD_downwards_BS[position].lK2 = UPSCALE_K2(findParamValue(losdDownBsNode, "K2").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.LoSD_downwards_BS[position].lOffset = UPSCALE_KOFFSET(findParamValue(losdDownBsNode, "Offset").as_double());
	}

	for (auto& usdDownBsNode : stoppingDistanceNode.children("USD_downwards_BS"))
	{
		int position = findParamValue(usdDownBsNode, "Id").as_int();
		position = position - 1;
		mstCfg->tLcu.tStoppingDistanceParamSet.USD_downwards_BS[position].lK1 = UPSCALE_K1(findParamValue(usdDownBsNode, "K1").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.USD_downwards_BS[position].lK2 = UPSCALE_K2(findParamValue(usdDownBsNode, "K2").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.USD_downwards_BS[position].lOffset = UPSCALE_KOFFSET(findParamValue(usdDownBsNode, "Offset").as_double());
	}

	for (auto usdUpBsNode : stoppingDistanceNode.children("USD_upwards_BS"))
	{
		int position = findParamValue(usdUpBsNode, "Id").as_int();
		position = position - 1;
		mstCfg->tLcu.tStoppingDistanceParamSet.USD_upwards_BS[position].lK1 = UPSCALE_K1(findParamValue(usdUpBsNode, "K1").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.USD_upwards_BS[position].lK2 = UPSCALE_K2(findParamValue(usdUpBsNode, "K2").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.USD_upwards_BS[position].lOffset = UPSCALE_KOFFSET(findParamValue(usdUpBsNode, "Offset").as_double());
	}

	for (auto& lesdRightBsNode : stoppingDistanceNode.children("LeSD_rightwards_BS"))
	{
		int position = findParamValue(lesdRightBsNode, "Id").as_int();
		position = position - 1;
		mstCfg->tLcu.tStoppingDistanceParamSet.LeSD_rightwards_BS[position].lK1 = UPSCALE_K1(findParamValue(lesdRightBsNode, "K1").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.LeSD_rightwards_BS[position].lK2 = UPSCALE_K2(findParamValue(lesdRightBsNode, "K2").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.LeSD_rightwards_BS[position].lOffset = UPSCALE_KOFFSET(findParamValue(lesdRightBsNode, "Offset").as_double());
	}

	for (auto& lesdLeftBsNode : stoppingDistanceNode.children("LeSD_leftwards_BS"))
	{
		int position = findParamValue(lesdLeftBsNode, "Id").as_int();
		position = position - 1;
		mstCfg->tLcu.tStoppingDistanceParamSet.LeSD_leftwards_BS[position].lK1 = UPSCALE_K1(findParamValue(lesdLeftBsNode, "K1").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.LeSD_leftwards_BS[position].lK2 = UPSCALE_K2(findParamValue(lesdLeftBsNode, "K2").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.LeSD_leftwards_BS[position].lOffset = UPSCALE_KOFFSET(findParamValue(lesdLeftBsNode, "Offset").as_double());
	}

	for (auto& rsdRightBsNode : stoppingDistanceNode.children("RSD_rightwards_BS"))
	{
		int position = findParamValue(rsdRightBsNode, "Id").as_int();
		position = position - 1;
		mstCfg->tLcu.tStoppingDistanceParamSet.RSD_rightwards_BS[position].lK1 = UPSCALE_K1(findParamValue(rsdRightBsNode, "K1").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.RSD_rightwards_BS[position].lK2 = UPSCALE_K2(findParamValue(rsdRightBsNode, "K2").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.RSD_rightwards_BS[position].lOffset = UPSCALE_KOFFSET(findParamValue(rsdRightBsNode, "Offset").as_double());
	}

	for (auto& rsdLeftBsNode : stoppingDistanceNode.children("RSD_leftwards_BS"))
	{
		int position = findParamValue(rsdLeftBsNode, "Id").as_int();
		position = position - 1;
		mstCfg->tLcu.tStoppingDistanceParamSet.RSD_leftwards_BS[position].lK1 = UPSCALE_K1(findParamValue(rsdLeftBsNode, "K1").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.RSD_leftwards_BS[position].lK2 = UPSCALE_K2(findParamValue(rsdLeftBsNode, "K2").as_double());
		mstCfg->tLcu.tStoppingDistanceParamSet.RSD_leftwards_BS[position].lOffset = UPSCALE_KOFFSET(findParamValue(rsdLeftBsNode, "Offset").as_double());
	}


	area_t lcuSize;
	/* LCU size as per firstExample.xml, including equipment */

	for (auto lcuTypeNode : doc.child("config").child("lcus").children("lcu-type"))
	{
		int position = findParamValue(lcuTypeNode, "Id").as_int();
		position = position - 1;

		std::string type = findParamValue(lcuTypeNode, "Type").as_string();
		if (type == "Economy")
		{
			mstCfg->tLcu.cType[position] = LcuType_Econonmy;
		}
		else if (type == "Premium")
		{
			mstCfg->tLcu.cType[position] = LcuType_Premium;
		}

		lcuSize.tTopLeft.lY = findParamValue(lcuTypeNode, "TopLeftY").as_int();
		lcuSize.tTopLeft.lZ = findParamValue(lcuTypeNode, "TopLeftZ").as_int();
		lcuSize.tBottomRight.lY = findParamValue(lcuTypeNode, "BottomRightY").as_int();
		lcuSize.tBottomRight.lZ = findParamValue(lcuTypeNode, "BottomRightZ").as_int();
		mstCfg->tLcu.tPhysicalConstraints[position].tSize = lcuSize;
		mstCfg->tLcu.tPhysicalConstraints[position].sPivotBearingOffsetY = static_cast<int16_t>(findParamValue(lcuTypeNode, "BearingOffsetY").as_int());
		mstCfg->tLcu.tPhysicalConstraints[position].sPivotBearingOffsetZ = static_cast<int16_t>(findParamValue(lcuTypeNode, "BearingOffsetZ").as_int());
	}
	for (auto lcuGroupNode : doc.child("config").child("lcus").children("lcu-group"))
	{
		mstCfg->tLcu.lVelocityFES = findParamValue(lcuGroupNode, "VelocityFES").as_int();
	}

	for (auto apsPosNode : doc.child("config").child("aps").children("apsposition"))
	{
		int position = findParamValue(apsPosNode, "Id").as_int();
		position = position - 1;
		mstCfg->tLcu.cPositionTranslation_TrackIsVertical[position] = static_cast<int8_t>(findParamValue(apsPosNode, "PositionTranslation_TrackIsVertical").as_int());
		mstCfg->tLcu.lPositionTranslation_CenterAlignmentPosAlongTrack[position] = findParamValue(apsPosNode, "PositionTranslation_CenterAlignment").as_int();
	}

	/* left and right coordinate of track #0 used in many of the following definitions */
	const int32_t YL = 3000 - 200;
	const int32_t YR = 3000 + 3000 + 200;


	clearIOCConfig(mstCfg);
	tIocId_t tIocIndex;
	tIocIndex = 0;
	int movmentIndex = 0;
	int propulsionIndex = 0;
	int wellSegmentIndex = 0;
	int fireIndex = 0;
	int garageIndex = 0;
	int maintenanceIndex = 0;
	int swivelIndex = 0;
	int walkwayIndex = 0;
	int wallPanelIndex = 0;
	int wellDoorIndex = 0;
	int welllockIndex = 0;
	int wellWaterIndex = 0;

	for (auto propulsionNode : doc.child("config").child("protectionareas").children("protectionarea"))
	{
		std::string type = findParamValue(propulsionNode, "Type").as_string();
		if (type == "Translational-Movement")
		{
			//mstCfg->tConstants.lMULTI_NumberOfVerticalTracksInstalled++;
			mstCfg->tProtectionAreas.tTranslationalMovementAreas[movmentIndex].tType = ProtectionArea_TranslationalMovement;
			mstCfg->tProtectionAreas.tTranslationalMovementAreas[movmentIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tTranslationalMovementAreas[movmentIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tTranslationalMovementAreas[movmentIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tTranslationalMovementAreas[movmentIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();

			SensorMovmentIndex[movmentIndex] = movmentIndex;
			movmentIndex++;
		}
		if (type == "PropulsionSection")
		{
			//mstCfg->tConstants.lMULTI_NumberOfPropulsionSectionsInstalled++;
			mstCfg->tProtectionAreas.tPropulsionSections[propulsionIndex].tType = ProtectionArea_PropulsionSection;
			mstCfg->tProtectionAreas.tPropulsionSections[propulsionIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tPropulsionSections[propulsionIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tPropulsionSections[propulsionIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tPropulsionSections[propulsionIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();

			SensorPropulsionIndex[propulsionIndex] = propulsionIndex;
			propulsionIndex++;
		}
		if (type == "WellSegment")
		{
			mstCfg->tProtectionAreas.tWellSegmentAreas[wellSegmentIndex].tType = ProtectionArea_WellSegment;
			mstCfg->tProtectionAreas.tWellSegmentAreas[wellSegmentIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tWellSegmentAreas[wellSegmentIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tWellSegmentAreas[wellSegmentIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tWellSegmentAreas[wellSegmentIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();
			//mstCfg->tConstants.lMULTI_NumberOfWellSegmentsInstalled++;
			SensorWellSegmentIndex[wellSegmentIndex] = wellSegmentIndex;
			wellSegmentIndex++;
		}
		if (type == "IocAffectedArea")
		{
			mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tType = ProtectionArea_IocAffectedArea;
			mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tIocAffectedArea[tIocIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();
			//mstCfg->tConstants.lMULTI_NumberOfIOCInstalled++;
			SensorIocIndex[tIocIndex] = tIocIndex;
			tIocIndex++;
		}
		if (type == "FireCompartment")
		{
			mstCfg->tProtectionAreas.tFireCompartments[fireIndex].tType = ProtectionArea_FireCompartment;
			mstCfg->tProtectionAreas.tFireCompartments[fireIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tFireCompartments[fireIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tFireCompartments[fireIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tFireCompartments[fireIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();
			//mstCfg->tConstants.lMULTI_NumberOfFireCompartmentsInstalled++;
			SensorFireIndex[fireIndex] = fireIndex;
			fireIndex++;
		}
		if (type == "Garage")
		{
			mstCfg->tProtectionAreas.tGarageAreas[garageIndex].tType = ProtectionArea_Garage;
			mstCfg->tProtectionAreas.tGarageAreas[garageIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tGarageAreas[garageIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tGarageAreas[garageIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tGarageAreas[garageIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();
			//mstCfg->tConstants.lMULTI_NumberOfGarageAreasInstalled++;
			SensorGarageIndex[garageIndex] = garageIndex;
			garageIndex++;
		}
		if (type == "MaintenanceArea")
		{
			mstCfg->tProtectionAreas.tMaintenanceAreas[maintenanceIndex].tType = ProtectionArea_Maintenance;
			mstCfg->tProtectionAreas.tMaintenanceAreas[maintenanceIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tMaintenanceAreas[maintenanceIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tMaintenanceAreas[maintenanceIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tMaintenanceAreas[maintenanceIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();
			//mstCfg->tConstants.lMULTI_NumberOfMaintenanceAreasInstalled++;
			SensorMaintenanceIndex[maintenanceIndex] = maintenanceIndex;
			maintenanceIndex++;
		}
		if (type == "SwivelPlatform")
		{
			mstCfg->tProtectionAreas.tSwivelPlatforms[swivelIndex].tType = ProtectionArea_SwivelPlatform;
			mstCfg->tProtectionAreas.tSwivelPlatforms[swivelIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tSwivelPlatforms[swivelIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tSwivelPlatforms[swivelIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tSwivelPlatforms[swivelIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();
			SensorSwivelIndex[swivelIndex] = swivelIndex;
			swivelIndex++;
		}
		if (type == "WalkwayArea")
		{
			mstCfg->tProtectionAreas.tWalkwayAreas[walkwayIndex].tType = ProtectionArea_Walkway;
			mstCfg->tProtectionAreas.tWalkwayAreas[walkwayIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tWalkwayAreas[walkwayIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tWalkwayAreas[walkwayIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tWalkwayAreas[walkwayIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();
			//mstCfg->tConstants.lMULTI_NumberOfWalkwayAreasInstalled++;
			SensorWalkwayIndex[walkwayIndex] = walkwayIndex;
			walkwayIndex++;
		}
		if (type == "WallPanelArea")
		{
			mstCfg->tProtectionAreas.tWallPanelAreas[wallPanelIndex].tType = ProtectionArea_WallPanel;
			mstCfg->tProtectionAreas.tWallPanelAreas[wallPanelIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tWallPanelAreas[wallPanelIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tWallPanelAreas[wallPanelIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tWallPanelAreas[wallPanelIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();
			//mstCfg->tConstants.lMULTI_NumberOfWallPanelAreasInstalled++;
			SensorWallPanelIndex[wallPanelIndex] = wallPanelIndex;
			wallPanelIndex++;
		}
		if (type == "WellDoorArea")
		{
			mstCfg->tProtectionAreas.tWellDoorAreas[wellDoorIndex].tType = ProtectionArea_WellDoor;
			mstCfg->tProtectionAreas.tWellDoorAreas[wellDoorIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tWellDoorAreas[wellDoorIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tWellDoorAreas[wellDoorIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tWellDoorAreas[wellDoorIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();
			//mstCfg->tConstants.lMULTI_NumberOfWellDoorsInstalled++;
			SensorWellDoorIndex[wellDoorIndex] = wellDoorIndex;
			wellDoorIndex++;
		}
		if (type == "WellLock")
		{
			mstCfg->tProtectionAreas.tWellLocks[welllockIndex].tType = ProtectionArea_WellLock;
			mstCfg->tProtectionAreas.tWellLocks[welllockIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tWellLocks[welllockIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tWellLocks[welllockIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tWellLocks[welllockIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();
			//mstCfg->tConstants.lMULTI_NumberOfWellLocksInstalled++;
			SensorWelllockIndex[welllockIndex] = welllockIndex;
			welllockIndex++;
		}
		if (type == "WellWaterArea")
		{
			mstCfg->tProtectionAreas.tWellWaterAreas[wellWaterIndex].tType = ProtectionArea_WellWater;
			mstCfg->tProtectionAreas.tWellWaterAreas[wellWaterIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tWellWaterAreas[wellWaterIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tWellWaterAreas[wellWaterIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tWellWaterAreas[wellWaterIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();
			//mstCfg->tConstants.lMULTI_NumberOfWellWaterAreasInstalled++;
			SensorWellWaterIndex[wellWaterIndex] = wellWaterIndex;
			wellWaterIndex++;
		}
		if (type == "WellAccessPort")
		{
			mstCfg->tProtectionAreas.tWellDoorAreas[wellDoorIndex].tType = ProtectionArea_WellAccessPort;
			mstCfg->tProtectionAreas.tWellDoorAreas[wellDoorIndex].tArea.tBottomRight.lY = findParamValue(propulsionNode, "BottomRightY").as_int();
			mstCfg->tProtectionAreas.tWellDoorAreas[wellDoorIndex].tArea.tBottomRight.lZ = findParamValue(propulsionNode, "BottomRightZ").as_int();
			mstCfg->tProtectionAreas.tWellDoorAreas[wellDoorIndex].tArea.tTopLeft.lY = findParamValue(propulsionNode, "TopLeftY").as_int();
			mstCfg->tProtectionAreas.tWellDoorAreas[wellDoorIndex].tArea.tTopLeft.lZ = findParamValue(propulsionNode, "TopLeftZ").as_int();
			//mstCfg->tConstants.lMULTI_NumberOfWellWaterAreasInstalled++;
			SensorWellWaterIndex[wellDoorIndex] = wellDoorIndex;
			wellDoorIndex++;
		}
	}

	int i = 0;
	for (auto workingAreaNode : doc.child("config").child("protectionareas").children("workingarea"))
	{
		mstCfg->tProtectionAreas.lWorkingArea_ZPoint2Top[i] = findParamValue(workingAreaNode, "ZPoint2Top").as_int();;
		mstCfg->tProtectionAreas.lWorkingArea_ZPoint2Bottom[i] = findParamValue(workingAreaNode, "ZPoint2Bottom").as_int();;
		mstCfg->tProtectionAreas.lWorkingArea_YPoint2Right[i] = findParamValue(workingAreaNode, "YPoint2Right").as_int();;
		mstCfg->tProtectionAreas.lWorkingArea_YPoint2Left[i] = findParamValue(workingAreaNode, "YPoint2Left").as_int();;
		i++;
	}

	auto protectionAreasNode = doc.child("config").child("protectionareas");
	mstCfg->tProtectionAreas.lWorkingArea_ExtensionTop = findParamValue(protectionAreasNode, "ZPoint2Top").as_int();;
	mstCfg->tProtectionAreas.lWorkingArea_ExtensionBottom = findParamValue(protectionAreasNode, "ZPoint2Top").as_int();;
	mstCfg->tProtectionAreas.lWorkingArea_ExtensionRight = findParamValue(protectionAreasNode, "ZPoint2Top").as_int();;
	mstCfg->tProtectionAreas.lWorkingArea_ExtensionLeft = findParamValue(protectionAreasNode, "ZPoint2Top").as_int();;

	/* Start indices for track propulsion sections in tPropulsionSections */
	for (auto propulsionNode : doc.child("config").child("propulsionSectionStartIndex").children("propulsionSection"))
	{
		int selectionId = findParamValue(propulsionNode, "Id").as_int();;
		selectionId = selectionId - 1;
		mstCfg->tProtectionAreas.lPropulsionSectionTrackStartIndex[selectionId] = findParamValue(propulsionNode, "Start_Index").as_int();;
	}

	auto srConfigMasterNode = doc.child("config").child("sr-config").child("sr_master");
	mstCfg->tSr.lMSMA_MaxTemperature = findParamValue(srConfigMasterNode, "MSMA_MaxTemperature").as_int();;
	mstCfg->tSr.lIOCSN_MaxTemperature = findParamValue(srConfigMasterNode, "IOCSN_MaxTemperature").as_int();;
	mstCfg->tSr.lLCUSN_MaxTemperature = findParamValue(srConfigMasterNode, "LCUSN_MaxTemperature").as_int();;
	mstCfg->tSr.lSCTS_MaxErrorTime = findParamValue(srConfigMasterNode, "SCTS_MaxErrorTime").as_int();;
	mstCfg->tSr.lSCTS_ErrorOverwriteErrorTemp = findParamValue(srConfigMasterNode, "SCTS_ErrorOverwriteErrorTemp").as_int();;
	mstCfg->tSr.lSCTS_ErrorOverwriteWarnTemp = findParamValue(srConfigMasterNode, "SCTS_ErrorOverwriteWarnTemp").as_int();;
	mstCfg->tSr.lMSMA_FireCompartmentRetractionFeedbackTimeout = findParamValue(srConfigMasterNode, "MSMA_FireCompartmentRetractionFeedbackTimeout").as_int();;

	i = 0;
	for (auto wellDoorInfo : doc.child("config").child("WellDoors").children("WellDoorInfo"))
	{
		if (i >= MULTI_NUMBER_OF_WELLDOORS_MAX) break;

		mstCfg->tWellDoor.tWellDoorInfos[i].tDoorZone.tTopLeft.lY = findParamValue(wellDoorInfo, "DoorZone TopLeft Y").as_int();
		mstCfg->tWellDoor.tWellDoorInfos[i].tDoorZone.tTopLeft.lZ = findParamValue(wellDoorInfo, "DoorZone TopLeft Z").as_int();
		mstCfg->tWellDoor.tWellDoorInfos[i].tDoorZone.tBottomRight.lY = findParamValue(wellDoorInfo, "DoorZone BottomRight Y").as_int();
		mstCfg->tWellDoor.tWellDoorInfos[i].tDoorZone.tBottomRight.lZ = findParamValue(wellDoorInfo, "DoorZone BottomRight Z").as_int();
		mstCfg->tWellDoor.tWellDoorInfos[i].tUcmZone.tTopLeft.lY = findParamValue(wellDoorInfo, "UcmZone TopLeft Y").as_int();
		mstCfg->tWellDoor.tWellDoorInfos[i].tUcmZone.tTopLeft.lZ = findParamValue(wellDoorInfo, "UcmZone TopLeft Z").as_int();
		mstCfg->tWellDoor.tWellDoorInfos[i].tUcmZone.tBottomRight.lY = findParamValue(wellDoorInfo, "UcmZone BottomRight Y").as_int();
		mstCfg->tWellDoor.tWellDoorInfos[i].tUcmZone.tBottomRight.lZ = findParamValue(wellDoorInfo, "UcmZone BottomRight Z").as_int();
		mstCfg->tWellDoor.tWellDoorInfos[i].tUnlockingZone.tTopLeft.lY = findParamValue(wellDoorInfo, "UnlockingZone TopLeft Y").as_int();
		mstCfg->tWellDoor.tWellDoorInfos[i].tUnlockingZone.tTopLeft.lZ = findParamValue(wellDoorInfo, "UnlockingZone TopLeft Z").as_int();
		mstCfg->tWellDoor.tWellDoorInfos[i].tUnlockingZone.tBottomRight.lY = findParamValue(wellDoorInfo, "UnlockingZone BottomRight Y").as_int();
		mstCfg->tWellDoor.tWellDoorInfos[i].tUnlockingZone.tBottomRight.lZ = findParamValue(wellDoorInfo, "UnlockingZone BottomRight Z").as_int();

		int Segment = findParamValue(wellDoorInfo, "AffectedSegments").as_int();

		mstCfg->tWellDoor.tWellDoorInfos[i].usAffectedSegments = static_cast<uint16_t>(Segment);
		mstCfg->tWellDoor.tWellDoorInfos[i].ucSpare = 0;
		i++;
	}

	//TODO: From Here
	i = 0;
	for (auto xcNode : doc.child("config").child("xcs").children("xc"))
	{
		if (i >= MULTI_NUMBER_OF_XC_MAX) break;

		mstCfg->tXc.tXcInfo[i].tCenter.lY = findParamValue(xcNode, "CenterY").as_int();
		mstCfg->tXc.tXcInfo[i].tCenter.lZ = findParamValue(xcNode, "CenterZ").as_int();
		mstCfg->tXc.tXcInfo[i].tArea.tTopLeft.lY = findParamValue(xcNode, "AreaTopLeftY").as_int();
		mstCfg->tXc.tXcInfo[i].tArea.tTopLeft.lZ = findParamValue(xcNode, "AreaTopLeftZ").as_int();
		mstCfg->tXc.tXcInfo[i].tArea.tBottomRight.lY = findParamValue(xcNode, "AreaBottomRightY").as_int();
		mstCfg->tXc.tXcInfo[i].tArea.tBottomRight.lZ = findParamValue(xcNode, "AreaBottomRightZ").as_int();

		i++;
	}

	auto xcNode = doc.child("config").child("xcs");
	mstCfg->tXc.lXcMatchToleranceY = findParamValue(xcNode, "XcMatchToleranceY").as_int();
	mstCfg->tXc.lXcMatchToleranceZ = findParamValue(xcNode, "XcMatchToleranceZ").as_int();


	for (auto wellDoorNode : doc.child("config").child("iocs").children("WellDoor"))
	{
		int indexLoc = findParamValue(wellDoorNode, "IOC").as_int();
		int LogicalIdLock = findParamValue(wellDoorNode, "Index").as_int();

		mstCfg->tIoc.tDeviceMappings[indexLoc].tWellDoorLockedClosed[LogicalIdLock] = static_cast<uint16_t>(findParamValue(wellDoorNode, "Index").as_uint());
	}

	for (auto wellTempNode : doc.child("config").child("iocs").children("WellTemp"))
	{
		int indexTemp = findParamValue(wellTempNode, "IOC").as_int();
		int indexTempPort = findParamValue(wellTempNode, "Index").as_int();

		mstCfg->tIoc.tDeviceMappings[indexTemp].tWellTemperature[indexTempPort] = static_cast<uint8_t>(findParamValue(wellTempNode, "Id").as_uint());
	}

	for (auto wellSegNode : doc.child("config").child("iocs").children("WellSegmEmergStop"))
	{
		int indexEmr = findParamValue(wellSegNode, "IOC").as_int();
		int LogicalIdEmr = findParamValue(wellSegNode, "Index").as_int();

		int Segment = findParamValue(wellSegNode, "AffectedSegment").as_ullong();

		mstCfg->tIoc.tDeviceMappings[indexEmr].tWellSegmentEmergencyStopTriggered[LogicalIdEmr] = static_cast<uint16_t>(Segment);
	}

	for (auto wellAccessNode : doc.child("config").child("iocs").children("WellAccess"))
	{
		int indexAcc = findParamValue(wellAccessNode, "IOC").as_int();
		int LogicalIdAcc = findParamValue(wellAccessNode, "Index").as_int();


		int Segment = findParamValue(wellAccessNode, "AffectedSegment").as_ullong();

		mstCfg->tIoc.tDeviceMappings[indexAcc].tWellAccessPortClosed[LogicalIdAcc] = findParamValue(wellAccessNode, "Index").as_int();
		mstCfg->tIoc.tDeviceMappings[indexAcc].tWellSegmentAccessPortNotClosed[LogicalIdAcc] = static_cast<uint16_t>(Segment);
	}

	for (auto wellWaterNode : doc.child("config").child("iocs").children("WellWater"))
	{
		int indexWater = findParamValue(wellWaterNode, "IOC").as_int();
		int LogicalIdWater = findParamValue(wellWaterNode, "Index").as_int();

		mstCfg->tIoc.tDeviceMappings[indexWater].tWaterInWell[LogicalIdWater] = static_cast<uint8_t>(findParamValue(wellWaterNode, "Id").as_uint());
	}

	for (auto mainAreaNode : doc.child("config").child("iocs").children("MaintArea"))
	{
		int indexMaint = findParamValue(mainAreaNode, "IOC").as_int();
		int LogicalIdMaint = findParamValue(mainAreaNode, "Index").as_int();

		mstCfg->tIoc.tDeviceMappings[indexMaint].tPersonInMaintenanceArea[LogicalIdMaint] = static_cast<uint8_t>(findParamValue(mainAreaNode, "Id").as_uint());
	}

	for (auto garageAreaNode : doc.child("config").child("iocs").children("GarageArea"))
	{
		int indexGarage = findParamValue(garageAreaNode, "IOC").as_int();
		int LogicalIdGarage = findParamValue(garageAreaNode, "Index").as_int();

		mstCfg->tIoc.tDeviceMappings[indexGarage].tPersonInGarage[LogicalIdGarage] = static_cast<uint8_t>(findParamValue(garageAreaNode, "Id").as_uint());
	}

	for (auto walkwayAreaNode : doc.child("config").child("iocs").children("WalkwayArea"))
	{
		int indexWalkway = findParamValue(walkwayAreaNode, "IOC").as_int();
		int LogicalIdWalkway = findParamValue(walkwayAreaNode, "Index").as_int();

		mstCfg->tIoc.tDeviceMappings[indexWalkway].tPersonInWalkway[LogicalIdWalkway] = static_cast<uint8_t>(findParamValue(walkwayAreaNode, "Id").as_uint());
	}


	for (auto wallPanelNode : doc.child("config").child("iocs").children("WallPanel"))
	{
		int indexWallPanel = findParamValue(wallPanelNode, "IOC").as_int();
		int LogicalIdWallPanel = findParamValue(wallPanelNode, "Index").as_int();

		mstCfg->tIoc.tDeviceMappings[indexWallPanel].tWallPanelClosed[LogicalIdWallPanel] = static_cast<uint8_t>(findParamValue(wallPanelNode, "Id").as_uint());
	}

	for (auto fireCompNode : doc.child("config").child("iocs").children("FireCompartment"))
	{
		int indexFire = findParamValue(fireCompNode, "IOC").as_int();
		int LogicalIdFire = findParamValue(fireCompNode, "Index").as_int();

		mstCfg->tIoc.tDeviceMappings[indexFire].tFireCompartmentRetracted[LogicalIdFire] = static_cast<uint8_t>(findParamValue(fireCompNode, "Id").as_uint());
	}

	for (auto powerProtectionNode : doc.child("config").child("iocs").children("PowerDetection"))
	{
		int indexPower = findParamValue(powerProtectionNode, "IOC").as_int();
		int LogicalIdPower = findParamValue(powerProtectionNode, "Index").as_int();

		mstCfg->tIoc.tDeviceMappings[indexPower].ucUnderpowerDetectionUsed[LogicalIdPower] = static_cast<uint8_t>(findParamValue(powerProtectionNode, "Id").as_uint());
	}

	auto iocGroupNode = doc.child("config").child("iocs").child("ioc_group");
	mstCfg->tWell.usWellAmbientTempMax = findParamValue(iocGroupNode, "WellAmbientTempMax").as_int();
	mstCfg->tWell.usWellAmbientTempMaxHyst = findParamValue(iocGroupNode, "WellAmbientTempMaxHyst").as_int();
	mstCfg->tWell.usWellAmbientTempMin = findParamValue(iocGroupNode, "WellAmbientTempMin").as_int();
	mstCfg->tWell.ulWellTemperatureSensorBottom_Z = findParamValue(iocGroupNode, "WellTemperatureSensorBottom_Z").as_int();
	mstCfg->tWell.ulWellTemperatureSensorTop_Z = findParamValue(iocGroupNode, "WellTemperatureSensorTop_Z").as_int();
	mstCfg->tWell.ucSpare[0] = 0;
	mstCfg->tWell.ucSpare[1] = 0;

	i = 0;
	for (auto masterSfNode : doc.child("config").child("SF_enable").children("Master_SF"))
	{
		if (i >= 11) break;

		int position = findParamValue(masterSfNode, "Id").as_int();
		position = position - 1;
		int Segment = findParamValue(masterSfNode, "value").as_int();
		mstCfg->usMASN_Sf_Enable[position] = static_cast<uint16_t>(Segment);

		++i;
	}

	i = 0;
	for (auto masterSRNode : doc.child("config").child("SR_enable").children("Master_SR"))
	{
		if (i >= NUMBER_OF_MASTER_SSR_ON_SN) break;

		int position = findParamValue(masterSRNode, "Id").as_int();
		position = position - 1;
		int Segment = findParamValue(masterSRNode, "value").as_int();

		mstCfg->tSr.usMASN_Ssr_Enable[position] = static_cast<uint16_t>(Segment);
		++i;
	}

	fixupTrackInfo(mstCfg);
}

void ExportData::buildMasterConfig(masterCfg_t* mstCfg) {
	memset(mstCfg, 0, sizeof(masterCfg_t));
	populateMasterConfig_stat_hil(mstCfg);
	mstCfg->ulCrc = Utility::crc32Checksum(sizeof(masterCfg_t) - sizeof(uint32_t), (uint8_t*)&mstCfg->tConstants, 0);
}

void ExportData::buildIocConfig(iocCfg_t* iocCfg, tIocId_t tId) {
	using namespace std;

	/* build a master config to clone from */
	buildMasterConfig(&mstCfg);

	memset(iocCfg, 0, sizeof(iocCfg_t));

	pugi::xml_node node = doc.child("config").child("general");

	/* populate iocCfg using values from mstCfg */
	iocCfg->tConstants.tConfigVersion = findParamValue(node, "ConfigVersion").as_int();
	iocCfg->tConstants.ucSpare[0] = 0;
	iocCfg->tConstants.ucSpare[1] = 1;

	iocCfg->tIocId = tId;

	/* TODO add configuration values */

	//free(&iocCfg);

	iocCfg->ucSpare2[0] = 0;
	iocCfg->ucSpare2[1] = 0;
	iocCfg->ucSpare2[2] = 0;

	/* copy complete device mapping table */
	memcpy(&iocCfg->tIoc.tDeviceMappingTable, &mstCfg.tIoc.tDeviceMappings[tId], sizeof(iocCfg->tIoc.tDeviceMappingTable));

	auto iocGroupNode = doc.child("config").child("iocs").child("ioc_group");
	iocCfg->tSr.lSCTS_MaxErrorTime = findParamValue(iocGroupNode, "MaxErrorTime").as_int();
	iocCfg->tSr.lUDSS_MaxAllowedUnderpowerTime = findParamValue(iocGroupNode, "MaxAllowedUnderpowerTime").as_int();
	iocCfg->tSr.lWTSS_MaxInvalidTime = findParamValue(iocGroupNode, "MaxInvalidTime").as_int();
	iocCfg->tSr.lWTSS_OverwriteWarnTemp = findParamValue(iocGroupNode, "OverwriteWarnTemp").as_int();
	iocCfg->tSr.lWTSS_OverwriteErrTemp = findParamValue(iocGroupNode, "OverwriteErrTemp").as_int();
	iocCfg->tSr.lWTSS_MaxDiffValue = findParamValue(iocGroupNode, "MaxDiffValue").as_int();

	iocCfg->tWell.lWell_AmbientTempMax = mstCfg.tWell.usWellAmbientTempMax;
	iocCfg->tWell.lWell_AmbientTempMaxHyst = mstCfg.tWell.usWellAmbientTempMaxHyst;
	iocCfg->tWell.lWell_AmbientTempMin = mstCfg.tWell.usWellAmbientTempMin;

	int i = 0;
	for (auto iocSFNode : doc.child("config").child("SF_enable").children("IOC_SF"))
	{
		if (i >= 2) break;

		int position = findParamValue(iocSFNode, "Id").as_int();
		position = position - 1;
		int Segment = findParamValue(iocSFNode, "value").as_int();

		iocCfg->usIOCSN_Sf_Enable[position] = static_cast<uint16_t>(Segment);
		++i;
	}

	i = 0;
	for (auto iocSRNode : doc.child("config").child("SR_enable").children("IOC_SR"))
	{
		if (i >= NUMBER_OF_IOC_SSR_ON_SN) break;

		int position = findParamValue(iocSRNode, "Id").as_int();
		position = position - 1;
		int Segment = findParamValue(iocSRNode, "value").as_int();

		iocCfg->tSr.usIOCSN_Ssr_Enable[position] = static_cast<uint16_t>(Segment);
		++i;
	}

	iocCfg->ulCrc = Utility::crc32Checksum(sizeof(iocCfg_t) - sizeof(uint32_t), (uint8_t*)&iocCfg->tConstants, 0);
}

void ExportData::clearIOCConfig(masterCfg_t* mstCfg) {
	using namespace std;
	for (tIocId_t tIocIndex = 0; tIocIndex < MULTI_NUMBER_OF_IOC_MAX; tIocIndex++) {
		/* Initialize all IOC Sensors to IOC_ENTRY_NOT_POPULATED_XXX */
		for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WDSS_AVAILABLE; ucSensorIndex++) {
			mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellDoorLockedClosed[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT16;
		}
		for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WAPSS_AVAILABLE; ucSensorIndex++) {
			mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellAccessPortClosed[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT16;
		}
		for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WWLSS_AVAILABLE; ucSensorIndex++) {
			mstCfg->tIoc.tDeviceMappings[tIocIndex].tWaterInWell[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
		}
		for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_PIMASS_AVAILABLE; ucSensorIndex++) {
			mstCfg->tIoc.tDeviceMappings[tIocIndex].tPersonInMaintenanceArea[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
		}
		for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_PIGASS_AVAILABLE; ucSensorIndex++) {
			mstCfg->tIoc.tDeviceMappings[tIocIndex].tPersonInGarage[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
		}
		for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_PIWSS_AVAILABLE; ucSensorIndex++) {
			mstCfg->tIoc.tDeviceMappings[tIocIndex].tPersonInWalkway[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
		}
		for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WPSS_AVAILABLE; ucSensorIndex++) {
			mstCfg->tIoc.tDeviceMappings[tIocIndex].tWallPanelClosed[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
		}
		for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WSES_AVAILABLE; ucSensorIndex++) {
			mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellSegmentEmergencyStopTriggered[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT16;
		}
		for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_FCSS_AVAILABLE; ucSensorIndex++) {
			mstCfg->tIoc.tDeviceMappings[tIocIndex].tFireCompartmentRetracted[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
		}
		for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WTSS_AVAILABLE; ucSensorIndex++) {
			mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellTemperature[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT8;
		}
		for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_UDSS_AVAILABLE; ucSensorIndex++) {
			mstCfg->tIoc.tDeviceMappings[tIocIndex].ucUnderpowerDetectionUsed[ucSensorIndex] = 0;
		}
		for (uint8_t ucSensorIndex = 0; ucSensorIndex < MAX_WSSS_AVAILABLE; ucSensorIndex++) {
			mstCfg->tIoc.tDeviceMappings[tIocIndex].tWellSegmentAccessPortNotClosed[ucSensorIndex] = IOC_ENTRY_NOT_POPULATED_INT16;
		}
	}
}

void ExportData::fixupTrackInfo(masterCfg_t* mstCfg) {
	using namespace std;
	int32_t numTracks = mstCfg->tConstants.lMULTI_NumberOfHorizontalTracksInstalled +
		mstCfg->tConstants.lMULTI_NumberOfVerticalTracksInstalled;
	for (int32_t i = 0; i < numTracks; ++i) {
		const area_t* ptArea = &mstCfg->tProtectionAreas.tTranslationalMovementAreas[i].tArea;
		int32_t lWidth = AREA_WIDTH(ptArea);
		int32_t lHeight = AREA_HEIGHT(ptArea);
		if (lHeight > lWidth) {
			/* vertical track: relPosOnTrack from LCU is Z, Y is just in the center of the track */
			mstCfg->tLcu.cPositionTranslation_TrackIsVertical[i] = true;
			mstCfg->tLcu.lPositionTranslation_CenterAlignmentPosAlongTrack[i] =
				ptArea->tTopLeft.lY + lWidth / 2;
		}
		else {
			/* horizontal track: relPosOnTrack from LCU is Y, Z is just in the center of the track */
			mstCfg->tLcu.cPositionTranslation_TrackIsVertical[i] = false;
			mstCfg->tLcu.lPositionTranslation_CenterAlignmentPosAlongTrack[i] =
				ptArea->tBottomRight.lZ + lHeight / 2;
		}
	}
}

std::string ExportData::uploadToSNs(std::string filePath)
{
	using namespace std;
	json j;
	pugi::xml_parse_result result = doc.load_file(filePath.c_str(), pugi::parse_full);

	pugi::xml_node node = doc.child("config").child("general");

	NumberOfLCUInstalled = findParamValue(node, "NumberOfLCUInstalled").as_int();
	NumberOfIOCInstalled = findParamValue(node, "NumberOfIOCInstalled").as_int();
	NumberOfXCInstalled = findParamValue(node, "NumberOfXCInstalled").as_int();

	if (!result)
	{
		LOG_ERROR("Error description: " << result.description() << "\n");
		return "";
	}

	std::string userName = "admin";
	std::string password = "admin";

	auto config = doc.child("config").child("general");
	std::string ftpServer = findParamValue(config, "MasterIp").as_string();
	bool success = Utility::UploadFile(filePath, ftpServer, userName, password);
	j["master"] = success ? true : false;

	int lcuIndex = 0;
	for (auto lcuNode : doc.child("config").child("lcus").children("lcu"))
	{
		if (lcuIndex >= NumberOfLCUInstalled) break;
		ftpServer = findParamValue(lcuNode, "Ip").as_string();
		success = Utility::UploadFile(filePath, ftpServer, userName, password);

		std::string nodeName = "lcu" + std::to_string(lcuIndex);
		j[nodeName] = success ? true : false;

		++lcuIndex;
	}

	int xcIndex = 0;
	for (auto xcNode : doc.child("config").child("xcs").children("xc"))
	{
		if (xcIndex >= NumberOfXCInstalled) break;
		ftpServer = findParamValue(xcNode, "Ip").as_string();
		success = Utility::UploadFile(filePath, ftpServer, userName, password);

		std::string nodeName = "xc" + std::to_string(xcIndex);
		j[nodeName] = success ? true : false;

		++xcIndex;
	}

	int i = 0;
	for (auto iocNode : doc.child("config").child("iocs").children("ioc"))
	{
		if (i >= NumberOfIOCInstalled) break;
		ftpServer = findParamValue(iocNode, "Ip").as_string();
		success = Utility::UploadFile(filePath, ftpServer, userName, password);

		std::string nodeName = "ioc" + std::to_string(i);
		j[nodeName] = success ? true : false;

		++i;
	}

	ftpServer = findParamValue(config, "ControlStationIp").as_string();
	success = Utility::UploadFile(filePath, ftpServer, userName, password);
	j["ControlStation"] = success ? true : false;

	return j.dump().c_str();
}

std::string ExportData::getLimits()
{
	json j;

	j["MULTI_TRAVELDISTANCE_VERTICAL_MAX"] = MULTI_TRAVELDISTANCE_VERTICAL_MAX;
	j["MULTI_NUMBER_OF_HORIZONTAL_TRACKS_MAX"] = MULTI_NUMBER_OF_HORIZONTAL_TRACKS_MAX;
	j["MULTI_NUMBER_OF_MA_MAX"] = MULTI_NUMBER_OF_MA_MAX;
	j["MULTI_NUMBER_OF_XC_MAX"] = MULTI_NUMBER_OF_XC_MAX;

	j["MULTI_NUMBER_OF_IOC_MAX"] = MULTI_NUMBER_OF_IOC_MAX;
	j["MULTI_NUMBER_OF_VERTICAL_TRACKS_MAX"] = MULTI_NUMBER_OF_VERTICAL_TRACKS_MAX;
	j["MULTI_LENGTH_END_HORIZONTAL_MAX"] = MULTI_LENGTH_END_HORIZONTAL_MAX;
	j["MULTI_DISTANCE_VERTICAL_TRACKS_MAX"] = MULTI_DISTANCE_VERTICAL_TRACKS_MAX;

	j["MULTI_TRAVELDISTANCE_HORIZONTAL_MAX"] = MULTI_TRAVELDISTANCE_HORIZONTAL_MAX;
	j["MULTI_NUMBER_OF_LCU_MAX"] = MULTI_NUMBER_OF_LCU_MAX;
	j["MULTI_NUMBER_OF_LCU_VERTICAL_TRACK_MAX"] = MULTI_NUMBER_OF_LCU_VERTICAL_TRACK_MAX;
	j["MULTI_NUMBER_OF_LCU_HORIZONTAL_TRACK_MAX"] = MULTI_NUMBER_OF_LCU_HORIZONTAL_TRACK_MAX;

	j["MULTI_NUMBER_OF_FIRECOMPARTMENTS_MAX"] = MULTI_NUMBER_OF_FIRECOMPARTMENTS_MAX;
	j["MULTI_NUMBER_OF_GARAGEAREAS_MAX"] = MULTI_NUMBER_OF_GARAGEAREAS_MAX;
	j["MULTI_NUMBER_OF_MAINTENANCEAREAS_MAX"] = MULTI_NUMBER_OF_MAINTENANCEAREAS_MAX;
	j["MULTI_NUMBER_OF_PROPULSIONSECTIONS_MAX"] = MULTI_NUMBER_OF_PROPULSIONSECTIONS_MAX;

	j["MULTI_NUMBER_OF_WALLPANELAREAS_MAX"] = MULTI_NUMBER_OF_WALLPANELAREAS_MAX;
	j["MULTI_NUMBER_OF_WORKINGAREAS_MAX"] = MULTI_NUMBER_OF_WORKINGAREAS_MAX;
	j["MULTI_NUMBER_OF_WORKINGAREATYPES_MAX"] = MULTI_NUMBER_OF_WORKINGAREATYPES_MAX;
	j["MULTI_NUMBER_OF_WELLWATERAREAS_MAX"] = MULTI_NUMBER_OF_WELLWATERAREAS_MAX;

	j["MULTI_NUMBER_OF_WELLDOORS_MAX"] = MULTI_NUMBER_OF_WELLDOORS_MAX;
	j["MULTI_NUMBER_OF_WELLSEGMENTS_MAX"] = MULTI_NUMBER_OF_WELLSEGMENTS_MAX;
	j["MULTI_NUMBER_OF_WALKWAYS_MAX"] = MULTI_NUMBER_OF_WALKWAYS_MAX;
	j["MULTI_NUMBER_OF_UNDERPOWERSENSOR_MAX"] = MULTI_NUMBER_OF_HORIZONTAL_TRACKS_MAX + MULTI_NUMBER_OF_VERTICAL_TRACKS_MAX;
	j["MULTI_NUMBER_OF_WELLTEMPERATURESENSOR_MAX"] = MULTI_NUMBER_OF_WELLWATERAREAS_MAX;

	j["MULTI_NUMBER_OF_BRAKE_STEPS_PER_LCU"] = MULTI_NUMBER_OF_BRAKE_STEPS_PER_LCU;
	j["MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_STEP"] = MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_STEP;
	j["MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_LCU"] = MULTI_NUMBER_OF_BRAKE_STEPS_PER_LCU * MULTI_NUMBER_OF_BRAKE_CALIPERS_PER_STEP;
	j["MULTI_CONFIG_VERSION"] = MULTI_CONFIG_VERSION;

	j["NUMBER_OF_MASTER_SR_ON_SN"] = NUMBER_OF_MASTER_SSR_ON_SN;
	j["NUMBER_OF_LCU_SR_ON_SN"] = NUMBER_OF_LCU_SSR_ON_SN;
	j["NUMBER_OF_IOC_SR_ON_SN"] = NUMBER_OF_IOC_SSR_ON_SN;

	return j.dump().c_str();
}

std::string ExportData::getNonCompatibileNodes(const std::string xml)
{
	json j;

	pugi::xml_parse_result result = doc.load_string(xml.c_str(), pugi::parse_full);

	if (!result)
	{
		LOG_ERROR("Error description: " << result.description() << "\n");
		LOG_ERROR("Error offset: " << result.offset << " (error at [..." << (xml.c_str() + result.offset) << "]\n\n");
		return "";
	}

	// Vector to store XPaths
	std::vector<std::string> xpaths;

	// Start the recursive traversal
	GetXPaths(doc.document_element(), "", xpaths);

	generate(xml, false);

	// Check for missing attribute bindings in the XML
	std::unordered_set<std::string> errors;

	//We need to carefully examine this list, as this will supress the logs for compatibility
	//for these nodes. 
	//TODO: Remove this if necessary in v2.0
	std::vector<std::string> blackList;
	blackList.push_back("/config/lcus/LCUXServiceConfirmation");
	blackList.push_back("/config/lcus/lcu_actions");
	blackList.push_back("/config/logging/");
	blackList.push_back("/config/sl-config/");

	for (const auto& attributeName : xpaths)
	{
		if (!attributeName.empty() &&
			std::find(xpathFromStructures.begin(), xpathFromStructures.end(), attributeName) == xpathFromStructures.end())
		{
			// Check if attributeName does not contain any substring from blackList
			bool isBlackListed = false;
			for (const auto& blackListEntry : blackList) {
				if (attributeName.find(blackListEntry) != std::string::npos) {
					isBlackListed = true;
					break;
				}
			}

			// If not in blackList, add it to errors
			if (!isBlackListed) {
				errors.insert(attributeName);
			}
		}
	}

	// Print errors or take further actions
	if (!errors.empty())
	{
		for (const auto& error : errors)
		{
			j[error];
		}
	}
	else
	{
		LOG_INFO("No errors found in compatibility check.\n");
	}


	return j.dump();
}

void ExportData::writeXML(std::string xml, std::string filePathToSave)
{
	auto setParamValue = [](pugi::xml_node node, std::string paramNameToFind, std::string value) {
		auto xpath = "/parameter[@name == " + paramNameToFind + "]";
		auto foundNode = node.select_node(xpath.c_str());
		return foundNode.node().attribute("value");
		};

	pugi::xml_parse_result result = doc.load_string(xml.c_str(), pugi::parse_full);

	if (!result)
	{
		LOG_ERROR("Error description: " << result.description() << "\n");
		LOG_ERROR("Error offset: " << result.offset << " (error at [..." << (xml.c_str() + result.offset) << "]\n\n");
		return;
	}

	auto sl_config = doc.child("config").child("sl-config");
	sl_config.remove_children();

	std::string masterDevided;
	int startIndex = 0;
	int valueSize = 200;

	sl_config.remove_child("master");

	auto sl_master = sl_config.append_child("master");
	Utility::parameterSet(sl_master, "id", "1");

	while (startIndex < master.length())
	{
		if (startIndex + 200 > master.length())
		{
			valueSize = master.length() - startIndex;
		}
		masterDevided = master.substr(startIndex, valueSize);
		startIndex = startIndex + 200;


		std::string std_string601 = masterDevided;
		Utility::parameterSet(sl_master, "data", std_string601.c_str());
		memset(&masterDevided, 0, sizeof(masterDevided));
		memset(&std_string601, 0, sizeof(std_string601));
	}

	sl_config.remove_child("lcu");
	for (size_t i = 0; i < NumberOfLCUInstalled; i++)
	{
		auto sl_lcu = sl_config.append_child("lcu");

		std::string dynamicIdlcu = std::to_string(i+1);
		Utility::parameterSet(sl_lcu, "id", dynamicIdlcu.c_str());

		std::string& lcuData = lcu[i];
		int startIndexLcu = 0;
		int valueSizeLcu = 200;
		while (startIndexLcu < lcuData.length())
		{
			if (startIndexLcu + 200 > lcuData.length())
			{
				valueSizeLcu = lcuData.length() - startIndexLcu;
			}
			std::string lcuDevided = lcuData.substr(startIndexLcu, valueSizeLcu);
			startIndexLcu = startIndexLcu + 200;
			// Allocate memory for the chunk and set the data
			char* result = new char[lcuDevided.length() + 1];
			std::strcpy(result, lcuDevided.c_str());
			Utility::parameterSet(sl_lcu, "data", result);
			delete[] result;
		}
	}

	/*         XC write XML        */
	sl_config.remove_child("xc");
	for (size_t i = 0; i < NumberOfXCInstalled; i++)
	{
		auto sl_xc = sl_config.append_child("xc");

		std::string dynamicIdxc = std::to_string(i+1);
		Utility::parameterSet(sl_xc, "id", dynamicIdxc.c_str());

		std::string& xcData = exchangercfg[i];
		int startIndexXc = 0;
		int valueSizeXc = 200;
		while (startIndexXc < xcData.length())
		{
			if (startIndexXc + 200 > xcData.length())
			{
				valueSizeXc = xcData.length() - startIndexXc;
			}
			std::string xcDevided = xcData.substr(startIndexXc, valueSizeXc);
			startIndexXc = startIndexXc + 200;
			// Allocate memory for the chunk and set the data
			char* result = new char[xcDevided.length() + 1];
			std::strcpy(result, xcDevided.c_str());
			Utility::parameterSet(sl_xc, "data", result);
			delete[] result;
		}
	}

	sl_config.remove_child("ioc");
	for (size_t i = 0; i < NumberOfIOCInstalled; i++)
	{
		auto sl_ioc = sl_config.append_child("ioc");

		std::string dynamicIdioc = std::to_string(i+1);
		Utility::parameterSet(sl_ioc, "id", dynamicIdioc.c_str());

		std::string& iocData = ioc[i];
		int startIndexIoc = 0;
		int valueSizeIoc = 200;
		while (startIndexIoc < iocData.length())
		{
			if (startIndexIoc + 200 > iocData.length())
			{
				valueSizeIoc = iocData.length() - startIndexIoc;
			}
			std::string iocDevided = iocData.substr(startIndexIoc, valueSizeIoc);
			startIndexIoc = startIndexIoc + 200;
			// Allocate memory for the chunk and set the data
			char* result = new char[iocDevided.length() + 1];
			std::strcpy(result, iocDevided.c_str());
			Utility::parameterSet(sl_ioc, "data", result);
			delete[] result;
		}
	}
	
	std::filesystem::remove(filePathToSave);

	pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
	decl.append_attribute("version") = "1.0";
	decl.append_attribute("encoding") = "utf-8";
	doc.save_file(filePathToSave.c_str(), "\t", pugi::format_default, pugi::encoding_utf8);

	//Remove extra space from the parameter node at the end
	processFileInPlace(filePathToSave.c_str());
}

LIB_EXPORT const char* Generate(const char* xml)
{
	try
	{
		ExportData& data = ExportData::getInstance();
		auto xmlRet = data.generate(xml);
		char* result = new char[xmlRet.length() + 1];
		std::strcpy(result, xmlRet.c_str());
		return result;
	}
	catch (...)
	{
		LOG_ERROR("Failed in Generating the file");
	}
}

LIB_EXPORT void WriteXML(const char* xml, const char* filePathToSave)
{
	try
	{
		ExportData& data = ExportData::getInstance();
		data.writeXML(xml, filePathToSave);
	}
	catch (...)
	{
		LOG_ERROR("Failed in Writing the config file");
	}
}

LIB_EXPORT const char* UploadToSN(const char* filePathToSave)
{
	try
	{
		ExportData& data = ExportData::getInstance();
		auto xmlRet = data.uploadToSNs(filePathToSave);
		char* result = new char[xmlRet.length() + 1];
		std::strcpy(result, xmlRet.c_str());
		return result;
	}
	catch (...)
	{
		LOG_ERROR("Failed in uploading to SN");
	}
}

LIB_EXPORT const char* GetLimits()
{
	try
	{
		ExportData& data = ExportData::getInstance();
		auto xmlRet = data.getLimits();
		char* result = new char[xmlRet.length() + 1];
		std::strcpy(result, xmlRet.c_str());
		return result;
	}
	catch (...)
	{
		LOG_ERROR("Failed in uploading to SN");
	}
}

LIB_EXPORT char* GetNonCompatibileNodes(const char* xml)
{
	try
	{
		ExportData& data = ExportData::getInstance();
		auto xmlRet = data.getNonCompatibileNodes(xml);
		char* result = new char[xmlRet.length() + 1];
		std::strcpy(result, xmlRet.c_str());
		return result;
	}
	catch (...)
	{
		LOG_ERROR("Failed in uploading to SN");
	}
}

LIB_EXPORT void FreeMemory(const char* ptr) {
	try
	{
		if (ptr)
		{
			delete[] ptr;
			ptr = nullptr;
		}
	}
	catch (...)
	{
		LOG_ERROR("Failed in free memory");
	}
}

