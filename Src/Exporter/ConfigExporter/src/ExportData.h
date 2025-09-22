#pragma once

#include "local_inc/MasterCfgGeneralTypes.h"
#include "local_inc/IocCfgGeneralTypes.h"
#include "local_inc/LcuCfgGeneralTypes.h"
#include "local_inc/XcCfgGeneralTypes.h"
#include "local_inc/XcCfgXcTypes.h"
#include <local_inc/XcCfgGeneralTypes.h>
#include <vcruntime_string.h>
#include <sstream>
#include "Utility.h"
#include "pugixml.hpp"
#include <vector>

class ExportData
{
public:
	std::string generate(std::string xml, bool exportMetaData = true);

	void writeXML(std::string xml, std::string filePathToSave);

	std::string uploadToSNs(std::string filePath);

	std::string getLimits();

	std::string getNonCompatibileNodes(const std::string xml);

	static ExportData& getInstance()
	{
		static ExportData exportObj;
		return exportObj;  
	}

private:

	explicit ExportData();

	std::vector<xcCfg_t> xcCfg;
	xcCfg_t xcCfgReverse{};
	std::vector<lcuCfg_t> lcuCfg;
	lcuCfg_t lcuCfg2{};
	lcuCfg_t lcuCfgReverse{};
	masterCfg_t mstCfg{};;
	std::vector<iocCfg_t> iocCfg;
	iocCfg_t iocCfgReverse[2]{};
	xcCfgXc_t xCfgXc{};
	xcCfgXc_t xcCfgArr[100]{};
	cfgVersion_t CfgVersion{};
	masterCfg_t mstCfgReverse{};

	void buildXcConfig(xcCfg_t* xcCfg, tXcId_t tXcId);

	void buildLcuConfig(lcuCfg_t* lcuCfg, tLcuId_t tLcuId);

	void populateMasterConfig_stat_hil(masterCfg_t* mstCfg);

	void buildMasterConfig(masterCfg_t* mstCfg);

	void buildIocConfig(iocCfg_t* iocCfg, tIocId_t tId);

	void clearIOCConfig(masterCfg_t* mstCfg);

	void fixupTrackInfo(masterCfg_t* mstCfg);

	std::string createMetaDataJson();

	int SensorMovmentIndex[MULTI_NUMBER_OF_TRACKS_MAX];
	int SensorPropulsionIndex[MULTI_NUMBER_OF_PROPULSIONSECTIONS_MAX + MULTI_NUMBER_OF_XC_MAX];
	int SensorWellSegmentIndex[MULTI_NUMBER_OF_WELLSEGMENTS_MAX];
	int SensorFireIndex[MULTI_NUMBER_OF_FIRECOMPARTMENTS_MAX];
	int SensorGarageIndex[MULTI_NUMBER_OF_GARAGEAREAS_MAX];
	int SensorMaintenanceIndex[MULTI_NUMBER_OF_MAINTENANCEAREAS_MAX];
	int SensorSwivelIndex[MULTI_NUMBER_OF_XC_MAX];
	int SensorWalkwayIndex[MULTI_NUMBER_OF_HORIZONTAL_TRACKS_MAX];
	int SensorWallPanelIndex[MULTI_NUMBER_OF_WALLPANELAREAS_MAX];
	int SensorWellDoorIndex[MULTI_NUMBER_OF_WELLDOORS_MAX];
	int SensorWelllockIndex[MULTI_NUMBER_OF_XC_MAX];
	int SensorWellWaterIndex[MULTI_NUMBER_OF_WELLWATERAREAS_MAX];
	int SensorIocIndex[MULTI_NUMBER_OF_IOC_MAX];
};

#if defined(_WIN32) || defined(_WIN64)
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

extern "C"
{
	LIB_EXPORT const char* Generate(const char* xml);

	LIB_EXPORT void WriteXML(const char* xml, const char* filePathToSave);

	LIB_EXPORT const char* UploadToSN(const char* filePathToSave);

	LIB_EXPORT const char* GetLimits();

	LIB_EXPORT char* GetNonCompatibileNodes(const char* xml);

	LIB_EXPORT void FreeMemory(const char* ptr);
}


