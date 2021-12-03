#ifndef HNS_NTCIP_MIB_STRINGS_H
#define HNS_NTCIP_MIB_STRINGS_H

#include <string>

//handy tree strings
//Lv1
const std::string gk_iso = "1";
//Lv2
const std::string gk_org = gk_iso + ".3";
//Lv3
const std::string gk_dod = gk_org + ".6";
//LV4
const std::string gk_internet = gk_dod + ".1";
//LV5
const std::string gk_mgmt = gk_internet + ".2";
//LV6
const std::string gk_mib_2 = gk_mgmt + ".1";
//LV7
const std::string gk_system = gk_mib_2 + ".1";
//LV8
const std::string gk_sysDescr = gk_system + ".1.0";
const std::string gk_sysObjectId = gk_system + ".2.0";
const std::string gk_sysUpTime = gk_system + ".3.0";
const std::string gk_sysContact = gk_system + ".4.0";
const std::string gk_sysName = gk_system + ".5.0";
const std::string gk_sysLocation = gk_system + ".6.0";
const std::string gk_sysServices = gk_system + ".7.0";
//LV7
const std::string gk_transmission = "10";
//LV8
const std::string gk_rs232 = gk_mib_2 + ".33";
//LV9
const std::string gk_rs232Number = gk_rs232 + ".1.0";
const std::string gk_rs232PortTable = gk_rs232 + ".2";
//LV10
const std::string gk_rs232PortEntry = gk_rs232PortTable + ".1";
//LV11
const std::string gk_rs232PortIndex = gk_rs232PortEntry + ".1";
const std::string gk_rs232PortType = gk_rs232PortEntry + ".2";
const std::string gk_rs232PortInSigNumber = gk_rs232PortEntry + ".3";
const std::string gk_rs232PortOutSigNumber = gk_rs232PortEntry + ".4";
const std::string gk_rs232PortInSpeed = gk_rs232PortEntry + ".5";
const std::string gk_rs232PortOutSpeed = gk_rs232PortEntry + ".6";
//LV9
const std::string gk_rs232AsyncTable = gk_rs232 + ".3";
//LV10
const std::string gk_rs232AsyncPortEntry = gk_rs232AsyncTable + ".1";
//LV11
const std::string gk_rs232AsyncPortIndex = gk_rs232AsyncPortEntry + ".1";
const std::string gk_rs232AsyncPortBits = gk_rs232AsyncPortEntry + ".2";
const std::string gk_rs232AsyncPortStopBits = gk_rs232AsyncPortEntry + ".3";
const std::string gk_rs232AsyncPortParity = gk_rs232AsyncPortEntry + ".4";
const std::string gk_rs232AsyncPortAutobaud = gk_rs232AsyncPortEntry + ".5";
const std::string gk_rs232AsyncPortParityErrs = gk_rs232AsyncPortEntry + ".6";
const std::string gk_rs232AsyncPortFramingErrs = gk_rs232AsyncPortEntry + ".7";
const std::string gk_rs232AsyncPortOverrunErrs = gk_rs232AsyncPortEntry + ".8";
//LV7
const std::string gk_snmp = "11";
//LV8
const std::string gk_snmpInPkts = gk_snmp + ".1.0";
const std::string gk_snmpOutPkts = gk_snmp + ".2.0";
const std::string gk_snmpInBadVersions = gk_snmp + ".3.0";
const std::string gk_snmpInBadCommunityNames = gk_snmp + ".4.0";
const std::string gk_snmpInBadCommunityUses = gk_snmp + ".5.0";
const std::string gk_snmpInASNParseErrs = gk_snmp + ".6.0";
const std::string gk_snmpInTooBigs = gk_snmp + ".8.0";
const std::string gk_snmpInNoSuchNames = gk_snmp + ".9.0";
const std::string gk_snmpInBadValues = gk_snmp + ".10.0";
const std::string gk_snmpInReadOnlys = gk_snmp + ".11.0";
const std::string gk_snmpInGenErrs = gk_snmp + ".12.0";
const std::string gk_snmpInTotalReqVars = gk_snmp + ".13.0";
const std::string gk_snmpInTotalSetVars = gk_snmp + ".14.0";
const std::string gk_snmpInGetRequests = gk_snmp + ".15.0";
const std::string gk_snmpInGetNexts = gk_snmp + ".16.0";
const std::string gk_snmpInSetRequests = gk_snmp + ".17.0";
const std::string gk_snmpInGetResponses = gk_snmp + ".18.0";
const std::string gk_snmpInTraps = gk_snmp + ".19.0";
const std::string gk_snmpOutTooBigs = gk_snmp + ".20.0";
const std::string gk_snmpOutNoSuchNames = gk_snmp + ".21.0";
const std::string gk_snmpOutBadValues = gk_snmp + ".22.0";
const std::string gk_snmpOutGenErrs = gk_snmp + ".24.0";
const std::string gk_snmpOutGenRequests = gk_snmp + ".25.0";
const std::string gk_snmpOutGenNexts = gk_snmp + ".26.0";
const std::string gk_snmpOutSetRequests = gk_snmp + ".27.0";
const std::string gk_snmpOutGetRepsonses = gk_snmp + ".28.0";
const std::string gk_snmpOutTraps = gk_snmp + ".29.0";
const std::string gk_snmpEnableAuthenTraps = gk_snmp + ".30.0";
//LV5
const std::string gk_private = gk_internet + ".4";
//LV6
const std::string gk_enterprises = gk_private + ".1";
//LV7
const std::string gk_nema = gk_enterprises + ".1206";
//LV8
const std::string gk_nemaPrivate = gk_nema + ".3";
//LV9
const std::string gk_WAP = gk_nemaPrivate + ".34";
//LV10
const std::string gk_vendor = gk_WAP + ".4";
//LV11
const std::string gk_SolarVolts = gk_vendor + ".1.0";
const std::string gk_SolarCurrent = gk_vendor + ".2.0";
const std::string gk_maxChargeCurrent = gk_vendor + ".3.0";
const std::string gk_ChargerStatus = gk_vendor + ".4.0";
const std::string gk_SMCountSensor = gk_vendor + ".5.0";
const std::string gk_SMCountFob = gk_vendor + ".6.0";
const std::string gk_radar = gk_vendor + ".7";
//LV12
const std::string gk_RadarMode = gk_radar + ".1.0";
const std::string gk_LegacyTripSpeed = gk_radar + ".2.0";
const std::string gk_PoliceMinSpeed = gk_radar + ".3.0";
const std::string gk_PoliceViolatorSpeed = gk_radar + ".4.0";
const std::string gk_PoliceMaxSpeed = gk_radar + ".5.0";
//LV8
const std::string gk_transportation = gk_nema + ".4";
//LV9
const std::string gk_protocols = gk_transportation + ".1";
//LV10
const std::string gk_layers = gk_protocols + ".1";
//LV11
const std::string gk_application = gk_protocols + ".7";
//LV12
const std::string gk_snmpConfig = gk_application + ".1";
//LV13
const std::string gk_snmpMaxPacketSize = gk_snmpConfig + ".1.0";
//LV9
const std::string gk_devices = gk_transportation + ".2";
//LV10
const std::string gk_dms = gk_devices + ".3";
//LV11
const std::string gk_dmsSignCfg = gk_dms + ".1";
//LV12
const std::string gk_dmsSignAccess = gk_dmsSignCfg + ".1.0";
const std::string gk_dmsSignType = gk_dmsSignCfg + ".2.0";
const std::string gk_dmsSignHeight = gk_dmsSignCfg + ".3.0";
const std::string gk_dmsSignWidth = gk_dmsSignCfg + ".4.0";
const std::string gk_dmsHorizontalBorder = gk_dmsSignCfg + ".5.0";
const std::string gk_dmsVerticalBorder = gk_dmsSignCfg + ".6.0";
const std::string gk_dmsLegend = gk_dmsSignCfg + ".7.0";
const std::string gk_dmsBeaconType = gk_dmsSignCfg + ".8.0";
const std::string gk_dmsSignTechnology = gk_dmsSignCfg + ".9.0";
//LV11
const std::string gk_vmsCfg = gk_dms + ".2";
//LV12
const std::string gk_vmsCharacterHeightPixels = gk_vmsCfg + ".1.0";
const std::string gk_vmsCharacterWidthPixels = gk_vmsCfg + ".2.0";
const std::string gk_vmsSignHeightPixels = gk_vmsCfg + ".3.0";
const std::string gk_vmsSignWidthPixels = gk_vmsCfg + ".4.0";
const std::string gk_vmsHorizontalPitch = gk_vmsCfg + ".5.0";
const std::string gk_vmsVerticalPitch = gk_vmsCfg + ".6.0";
const std::string gk_monochromeColor = gk_vmsCfg + ".7.0";
//LV11
const std::string gk_fontDefinition = gk_dms + ".3";
//LV12
const std::string gk_numFonts = gk_fontDefinition + ".1.0";
const std::string gk_fontTable = gk_fontDefinition + ".2";
//LV13
const std::string gk_fontEntry = gk_fontTable + ".1";
//LV14
const std::string gk_fontIndex = gk_fontEntry + ".1";
const std::string gk_fontNumber = gk_fontEntry + ".2";
const std::string gk_fontName = gk_fontEntry + ".3";
const std::string gk_fontHeight = gk_fontEntry + ".4";
const std::string gk_fontCharSpacing = gk_fontEntry + ".5";
const std::string gk_fontLineSpacing = gk_fontEntry + ".6";
const std::string gk_fontVersionID = gk_fontEntry + ".7";
const std::string gk_fontStatus = gk_fontEntry + ".8";
//LV12
const std::string gk_maxFontCharacters = gk_fontDefinition + ".3.0";
const std::string gk_characterTable = gk_fontDefinition + ".4";
//LV13
const std::string gk_characterEntry = gk_characterTable + ".1";
//LV14
const std::string gk_characterNumber = gk_characterEntry + ".1";
const std::string gk_characterWidth = gk_characterEntry + ".2";
const std::string gk_characterBitmap = gk_characterEntry + ".3";
//LV12
const std::string gk_fontMaxCharacterSize = gk_fontDefinition + ".5.0";
//LV11
const std::string gk_multiCfg = gk_dms + ".4";
//LV12
const std::string gk_defaultBackgroundColor = gk_multiCfg + ".1.0";
const std::string gk_defaultForegroundColor = gk_multiCfg + ".2.0";
const std::string gk_defaultFlashOn = gk_multiCfg + ".3.0";
const std::string gk_defaultFlashOnActivate = gk_multiCfg + ".17.0";
const std::string gk_defaultFlashOff = gk_multiCfg + ".4.0";
const std::string gk_defaultFlashOffActivate = gk_multiCfg + ".18.0";
const std::string gk_defaultFont = gk_multiCfg + ".5.0";
const std::string gk_defaultFontActivate = gk_multiCfg + ".19.0";
const std::string gk_defaultJustificationLine = gk_multiCfg + ".6.0";
const std::string gk_defaultJustificationLineActivate = gk_multiCfg + ".20.0";
const std::string gk_defaultJustificationPage = gk_multiCfg + ".7.0";
const std::string gk_defaultJustificationPageActivate = gk_multiCfg + ".21.0";
const std::string gk_defaultPageOnTime = gk_multiCfg + ".8.0";
const std::string gk_defaultPageOnTimeActivate = gk_multiCfg + ".22.0";
const std::string gk_defaultPageOffTime = gk_multiCfg + ".9.0";
const std::string gk_defaultPageOffTimeActivate = gk_multiCfg + ".23.0";
const std::string gk_defaultBackgroundRGB = gk_multiCfg + ".12.0";
const std::string gk_defaultBackgroundRGBActivate = gk_multiCfg + ".24.0";
const std::string gk_defaultForeGroundRGB = gk_multiCfg + ".13.0";
const std::string gk_defaultForegroundRGBActivate = gk_multiCfg + ".25.0";
const std::string gk_defaultCharacterSet = gk_multiCfg + ".10.0";
const std::string gk_dmsColorScheme = gk_multiCfg + ".11.0";
const std::string gk_dmsSupportedMULTITags = gk_multiCfg + ".14.0";
const std::string gk_dmsMaxNumberPages = gk_multiCfg + ".15.0";
const std::string gk_dmsMaxSupportedLength = gk_multiCfg + ".16.0";
//LV11
const std::string gk_dmsMessage = gk_dms + ".5";
//LV12
const std::string gk_dmsNumPermanentMsg = gk_dmsMessage + ".1.0";
const std::string gk_dmsNumChangeableMsg = gk_dmsMessage + ".2.0";
const std::string gk_dmsMaxChangeableMsg = gk_dmsMessage + ".3.0";
const std::string gk_dmsFreeChangeableMemory = gk_dmsMessage + ".4.0";
const std::string gk_dmsNumVolatileMsg = gk_dmsMessage + ".5.0";
const std::string gk_dmsMaxVolatileMsg = gk_dmsMessage + ".6.0";
const std::string gk_dmsFreeVolatileMemory = gk_dmsMessage + ".7.0";
const std::string gk_dmsMessageTable = gk_dmsMessage + ".8";
//LV13
const std::string gk_dmsMessageEntry = gk_dmsMessageTable + ".1";
//LV14
const std::string gk_dmsMessageMemoryType = gk_dmsMessageEntry + ".1";
const std::string gk_dmsMessageNumber = gk_dmsMessageEntry + ".2";
const std::string gk_dmsMessageMultiString = gk_dmsMessageEntry + ".3";
const std::string gk_dmsMessageOwner = gk_dmsMessageEntry + ".4";
const std::string gk_dmsMessageCRC = gk_dmsMessageEntry + ".5";
const std::string gk_dmsMessageBeacon = gk_dmsMessageEntry + ".6";
const std::string gk_dmsMessagePixelService = gk_dmsMessageEntry + ".7";
const std::string gk_dmsMessageRunTimePriority = gk_dmsMessageEntry + ".8";
const std::string gk_dmsMessageStatus = gk_dmsMessageEntry + ".9";
//LV12
const std::string gk_dmsValidateMessageError = gk_dmsMessage + ".9.0";
//LV11
const std::string gk_signControl = gk_dms + ".6";
//LV12
const std::string gk_dmsControlMode = gk_signControl + ".1.0";
const std::string gk_dmsSWReset = gk_signControl + ".2.0";
const std::string gk_dmsActivateMessage = gk_signControl + ".3.0";
const std::string gk_dmsMessageTimeRemaining = gk_signControl + ".4.0";
const std::string gk_dmsMsgTableSource = gk_signControl + ".5.0";
const std::string gk_dmsMsgRequesterID = gk_signControl + ".6.0";
const std::string gk_dmsMsgSourceMode = gk_signControl + ".7.0";
const std::string gk_dmsShortPowerRecoveryMessage = gk_signControl + ".8.0";
const std::string gk_dmsLongPowerRecoveryMessage = gk_signControl + ".9.0";
const std::string gk_dmsShortPowerLossTime = gk_signControl + ".10.0";
const std::string gk_dmsResetMessage = gk_signControl + ".11.0";
const std::string gk_dmsCommunicationsLossMessage = gk_signControl + ".12.0";
const std::string gk_dmsTimeCommLoss = gk_signControl + ".13.0";
const std::string gk_dmsPowerLossMessage = gk_signControl + ".14.0";
const std::string gk_dmsEndDurationMessage = gk_signControl + ".15.0";
const std::string gk_dmsMemoryMgmt = gk_signControl + ".16.0";
const std::string gk_dmsActivateMsgError = gk_signControl + ".17.0";
const std::string gk_dmsMultiSyntaxError = gk_signControl + ".18.0";
const std::string gk_dmsMultiSyntaxErrorPosition = gk_signControl + ".19.0";
const std::string gk_dmsMultiOtherErrorDescription = gk_signControl + ".20.0";
const std::string gk_vmsPixelServiceDuration = gk_signControl + ".21.0";
const std::string gk_vmsPixelServiceFrequency = gk_signControl + ".22.0";
const std::string gk_vmsPixelServiceTime = gk_signControl + ".23.0";
const std::string gk_dmsActivateErrorMsgCode = gk_signControl + ".24.0";
const std::string gk_dmsActivateMessageState = gk_signControl + ".25.0";
//LV11
const std::string gk_illum = gk_dms + ".7";
//LV12
const std::string gk_dmsIllumControl = gk_illum + ".1.0";
const std::string gk_dmsIllumMaxPhotocellLevel = gk_illum + ".2.0";
const std::string gk_dmsIllumPhotocellLevelStatus = gk_illum + ".3.0";
const std::string gk_dmsIllumNumBrightLevels = gk_illum + ".4.0";
const std::string gk_dmsIllumBrightLevelStatus = gk_illum + ".5.0";
const std::string gk_dmsIllumManLevel = gk_illum + ".6.0";
const std::string gk_dmsIllumBrightnessValues = gk_illum + ".7.0";
const std::string gk_dmsIllumBrightnessValuesError = gk_illum + ".8.0";
const std::string gk_dmsIllumLightOutputStatus = gk_illum + ".9.0";
//LV11
const std::string gk_dmsSchedule = gk_dms + ".8";
//LV12
const std::string gk_numActionTableEntries = gk_dmsSchedule + ".1.0";
const std::string gk_dmsActionTable = gk_dmsSchedule + ".2";
//LV13
const std::string gk_dmsActionEntry = gk_dmsActionTable + ".1";
//LV14
const std::string gk_dmsActionIndex = gk_dmsActionEntry + ".1";
const std::string gk_dmsActionMsgCode = gk_dmsActionEntry + ".2";
//LV11
const std::string gk_dmsStatus = gk_dms + ".9";
//LV12
const std::string gk_statMultiFieldRows = gk_dmsStatus + ".1";
const std::string gk_statMultiFieldTable = gk_dmsStatus + ".2";
//LV13
const std::string gk_statMultiFieldEntry = gk_statMultiFieldTable + ".1";
//LV14
const std::string gk_statMultiFieldIndex = gk_statMultiFieldEntry + ".1";
const std::string gk_statMultiFieldCode = gk_statMultiFieldEntry + ".2";
const std::string gk_statMultiCurrentFieldValue = gk_statMultiFieldEntry + ".3";
//LV12
const std::string gk_dmsCurrentSpeed = gk_dmsStatus + ".3";
const std::string gk_dmsCurrentSpeedLimit = gk_dmsStatus + ".4";
const std::string gk_watchdogFailureCount = gk_dmsStatus + ".5";
const std::string gk_dmsStatDoorOpen = gk_dmsStatus + ".6";
const std::string gk_statError = gk_dmsStatus + ".7";
//LV13
const std::string gk_shortErrorStatus = gk_statError + ".1.0";
const std::string gk_controllerErrorStatus = gk_statError + ".10.0";
const std::string gk_dmsPowerFailureStatusMap = gk_statError + ".11.0";
const std::string gk_dmsPowerNumRows = gk_statError + ".12.0";
const std::string gk_dmsPowerStatusTable = gk_statError + ".13";
//LV14
const std::string gk_dmsPowerStatusEntry = gk_dmsPowerStatusTable + ".1";
//LV15
const std::string gk_dmsPowerIndex = gk_dmsPowerStatusEntry + ".1";
const std::string gk_dmsPowerDescription = gk_dmsPowerStatusEntry + ".2";
const std::string gk_dmsPowerMfrStatus = gk_dmsPowerStatusEntry + ".3";
const std::string gk_dmsPowerStatus = gk_dmsPowerStatusEntry + ".4";
const std::string gk_dmsPowerVoltage = gk_dmsPowerStatusEntry + ".5";
const std::string gk_dmsPowerType = gk_dmsPowerStatusEntry + ".6";
//LV12
const std::string gk_statPower = gk_dmsStatus + ".8";
//LV13
const std::string gk_signVolts = gk_statPower + ".1.0";
const std::string gk_lowFuelThreshold = gk_statPower + ".2.0";
const std::string gk_fuelLevel = gk_statPower + ".3.0";
const std::string gk_engineRPM = gk_statPower + ".4.0";
const std::string gk_lineVolts = gk_statPower + ".5.0";
const std::string gk_powerSource = gk_statPower + ".6.0";
//LV13
const std::string gk_dmsClimateCtrlStatusMap = gk_statError + ".14.0";
const std::string gk_dmsClimateCtrlNumRows = gk_statError + ".16.0";
const std::string gk_dmsClimateCtrlStatusTable = gk_statError + ".17";
//LV14
const std::string gk_dmsClimateCtrlStatusEntry = gk_dmsClimateCtrlStatusTable + ".1";
//LV15
const std::string gk_dmsClimateCtrlIndex = gk_dmsClimateCtrlStatusEntry + ".1";
const std::string gk_dmsClimateCtrlDescription = gk_dmsClimateCtrlStatusEntry + ".2";
const std::string gk_dmsClimateCtrlMfrStatus = gk_dmsClimateCtrlStatusEntry + ".3";
const std::string gk_dmsClimateCtrlErrorStatus = gk_dmsClimateCtrlStatusEntry + ".4";
const std::string gk_dmsClimateCtrlOnStatus = gk_dmsClimateCtrlStatusEntry + ".5";
const std::string gk_dmsClimateCtrlTestActivation = gk_dmsClimateCtrlStatusEntry + ".6";
const std::string gk_dmsClimateCtrlAbortReason = gk_dmsClimateCtrlStatusEntry + ".7";
const std::string gk_dmsClimateCtrlType = gk_dmsClimateCtrlStatusEntry + ".8";
//LV13
const std::string gk_pixelFailureTableNumRows = gk_statError + ".2.0";
const std::string gk_pixelFailureTable = gk_statError + ".3";
//LV14
const std::string gk_pixelFailureEntry = gk_pixelFailureTable + ".1";
//LV15
const std::string gk_pixelFailureDetectionType = gk_pixelFailureEntry + ".1";
const std::string gk_pixelFailureIndex = gk_pixelFailureEntry + ".2";
const std::string gk_pixelFailureXLocation = gk_pixelFailureEntry + ".3";
const std::string gk_pixelFailureYLocation = gk_pixelFailureEntry + ".4";
const std::string gk_pixelFailureStatus = gk_pixelFailureEntry + ".5";
//LV13
const std::string gk_pixelTestActivation = gk_statError + ".4.0";
const std::string gk_pixelStatusTable = gk_statError + ".18";
//LV14
const std::string gk_pixelStatusEntry = gk_pixelStatusTable + ".1";
//LV15
const std::string gk_dmsPixelStatusIndex = gk_pixelStatusEntry + ".1";
const std::string gk_dmsPixelStatus = gk_pixelStatusEntry + ".2";
//LV13
const std::string gk_dmsPixelFailureTestRows = gk_statError + ".19.0";
const std::string gk_dmsPixelFailureMessageRows = gk_statError + ".20.0";
const std::string gk_lampFailureStuckOn = gk_statError + ".5.0";
const std::string gk_lampFailureStuckOff = gk_statError + ".6.0";
const std::string gk_lampTestActivation = gk_statError + ".7.0";
const std::string gk_dmsLampNumRows = gk_statError + ".23.0";
const std::string gk_dmsDrumStatusMap = gk_statError + ".25.0";
const std::string gk_dmsDrumNumRows = gk_statError + ".26.0";
const std::string gk_dmsLightSensorStatusMap = gk_statError + ".28.0";
const std::string gk_dmsLightSensorNumRows = gk_statError + ".29.0";
const std::string gk_dmsLightSensorStatusTable = gk_statError + ".30";
//LV14
const std::string gk_dmsLightSensorStatusEntry = gk_dmsLightSensorStatusTable + ".1";
//LV15
const std::string gk_dmsLightSensorIndex = gk_dmsLightSensorStatusEntry + ".1";
const std::string gk_dmsLightSensorDescription = gk_dmsLightSensorStatusEntry + ".2";
const std::string gk_dmsLightSensorCurrentReading = gk_dmsLightSensorStatusEntry + ".3";
const std::string gk_dmsLightSensorStatus = gk_dmsLightSensorStatusEntry + ".4";
//LV13
const std::string gk_dmsHumiditySensorStatusMap = gk_statError + ".31.0";
const std::string gk_dmsHumiditySensorNumRows = gk_statError + ".32.0";
const std::string gk_dmsTempSensorStatusMap = gk_statError + ".34.0";
const std::string gk_dmsTempSensorNumRows = gk_statError + ".35.0";
//LV14
const std::string gk_dmsTempSensorStatusTable = gk_statError + ".36";
//LV15
const std::string gk_dmsTempSensorStatusEntry = gk_dmsTempSensorStatusTable + ".1";
//LV16
const std::string gk_dmsTempSensorIndex = gk_dmsTempSensorStatusEntry + ".1";
const std::string gk_dmsTempSensorDescription = gk_dmsTempSensorStatusEntry + ".2";
const std::string gk_dmsTempSensorCurrentReading = gk_dmsTempSensorStatusEntry + ".3";
const std::string gk_dmsTempSensorHighWarningTemperature = gk_dmsTempSensorStatusEntry + ".4";
const std::string gk_dmsTempSensorLowWarningTemperature = gk_dmsTempSensorStatusEntry + ".5";
const std::string gk_dmsTempSensorHighCriticalTemperature = gk_dmsTempSensorStatusEntry + ".6";
const std::string gk_dmsTempSensorLowCriticalTemperature = gk_dmsTempSensorStatusEntry + ".7";
const std::string gk_dmsTempSensorStatus = gk_dmsTempSensorStatusEntry + ".8";
//LV13
const std::string gk_dmsTempSensorHighestCriticalTempThreshold = gk_statError + ".37.0";
const std::string gk_dmsTempSensorLowestCriticalTempThreshold = gk_statError + ".38.0";
//LV12
const std::string gk_statTemp = gk_dmsStatus + ".9";
//LV13
const std::string gk_tempMinctrlCabinet = gk_statTemp + ".1.0";
const std::string gk_tempMaxCtrlCabinet = gk_statTemp + ".2.0";
const std::string gk_tempMinAmbient = gk_statTemp + ".3.0";
const std::string gk_tempMaxAmbient = gk_statTemp + ".4.0";
const std::string gk_tempMinSignHousing = gk_statTemp + ".5.0";
const std::string gk_tempMaxSignHousing = gk_statTemp + ".6.0";
const std::string gk_tempSensorWarningMap = gk_statTemp + ".7.0";
const std::string gk_tempSensorCriticalTempMap = gk_statTemp + ".8.0";
//LV11
const std::string gk_graphicsDefinition = gk_dms + ".10";
//LV12
const std::string gk_dmsGraphicMaxEntries = gk_graphicsDefinition + ".1.0";
const std::string gk_dmsGraphicNumEntries = gk_graphicsDefinition + ".2.0";
const std::string gk_dmsGraphicMaxSize = gk_graphicsDefinition + ".3.0";
const std::string gk_availableGraphicMemory = gk_graphicsDefinition + ".4.0";
const std::string gk_dmsGraphicBlockSize = gk_graphicsDefinition + ".5.0";
const std::string gk_dmsGraphicTable = gk_graphicsDefinition + ".6";
//LV13
const std::string gk_dmsGraphicEntry = gk_dmsGraphicTable + ".1";
//LV14
const std::string gk_dmsGraphicIndex = gk_dmsGraphicEntry + ".1";
const std::string gk_dmsGraphicNumber = gk_dmsGraphicEntry + ".2";
const std::string gk_dmsGraphicName = gk_dmsGraphicEntry + ".3";
const std::string gk_dmsGraphicHeight = gk_dmsGraphicEntry + ".4";
const std::string gk_dmsGraphicWidth = gk_dmsGraphicEntry + ".5";
const std::string gk_dmsGraphicType = gk_dmsGraphicEntry + ".6";
const std::string gk_dmsGraphicID = gk_dmsGraphicEntry + ".7";
const std::string gk_dmsGraphicTransparentEnabled = gk_dmsGraphicEntry + ".8";
const std::string gk_dmsGraphicTransparentColor = gk_dmsGraphicEntry + ".9";
const std::string gk_dmsGraphicStatus = gk_dmsGraphicEntry + ".10";
//LV12
const std::string gk_dmsGraphicBitmapTable = gk_graphicsDefinition + ".7";
//LV13
const std::string gk_dmsGraphicBitmapEntry = gk_dmsGraphicBitmapTable + ".1";
//LV14
const std::string gk_dmsGraphicBitmapIndex = gk_dmsGraphicBitmapEntry + ".1";
const std::string gk_dmsGraphicBlockNumber = gk_dmsGraphicBitmapEntry + ".2";
const std::string gk_dmsGraphicBlockBitmap = gk_dmsGraphicBitmapEntry + ".3";
//LV10
const std::string gk_ess = gk_devices + ".5";
//LV11
const std::string gk_essNtcip = gk_ess + ".2";
//LV12
const std::string gk_essNtcipLocation = gk_essNtcip + ".2";
//LV13
const std::string gk_essLatitude = gk_essNtcipLocation + ".1.0";
const std::string gk_essLongitude = gk_essNtcipLocation + ".2.0";
const std::string gk_essVehicleSpeed = gk_essNtcipLocation + ".3.0";
const std::string gk_essVehicleBearing = gk_essNtcipLocation + ".4.0";
const std::string gk_essOdometer = gk_essNtcipLocation + ".5.0";
//LV10
const std::string gk_global = gk_devices + ".6";
//LV11
const std::string gk_globalConfiguration = gk_global + ".1";
//LV12
const std::string gk_globalSetIDParameter = gk_globalConfiguration + ".1.0";
const std::string gk_globalMaxModules = gk_globalConfiguration + ".2.0";
const std::string gk_globalModuleTable = gk_globalConfiguration + ".3";
//LV13
const std::string gk_moduleTableEntry = gk_globalModuleTable + ".1";
//LV14
const std::string gk_moduleNumber = gk_moduleTableEntry + ".1";
const std::string gk_moduleDeviceNode = gk_moduleTableEntry + ".2";
const std::string gk_moduleMake = gk_moduleTableEntry + ".3";
const std::string gk_moduleModel = gk_moduleTableEntry + ".4";
const std::string gk_moduleVersion = gk_moduleTableEntry + ".5";
const std::string gk_moduleType = gk_moduleTableEntry + ".6";
//LV12
const std::string gk_controllerBaseStandards = gk_globalConfiguration + ".4.0";
//LV11
const std::string gk_globalTimeManagement = gk_global + ".3";
//LV12
const std::string gk_globalTime = gk_globalTimeManagement + ".1.0";
const std::string gk_globalDaylightSaving = gk_globalTimeManagement + ".2.0";
const std::string gk_timebase = gk_globalTimeManagement + ".3";
//LV13
const std::string gk_maxTimeBaseScheduleEntries = gk_timebase + ".1.0";
const std::string gk_timeBaseScheduleTable = gk_timebase + ".2";
//LV14
const std::string gk_timeBaseScheduleEntry = gk_timeBaseScheduleTable + ".1";
//LV15
const std::string gk_timeBaseScheduleNumber = gk_timeBaseScheduleEntry + ".1";
const std::string gk_timeBaseScheduleMonth = gk_timeBaseScheduleEntry + ".2";
const std::string gk_timeBaseScheduleDay = gk_timeBaseScheduleEntry + ".3";
const std::string gk_timeBaseScheduleDate = gk_timeBaseScheduleEntry + ".4";
const std::string gk_timeBaseScheduleDayPlan = gk_timeBaseScheduleEntry + ".5";
//LV13
const std::string gk_maxDayPlans = gk_timebase + ".3.0";
const std::string gk_maxDayPlanEvents = gk_timebase + ".4.0";
const std::string gk_timeBaseDayPlanTable = gk_timebase + ".5";
//LV14
const std::string gk_timeBaseDayPlanEntry = gk_timeBaseDayPlanTable + ".1";
//LV15
const std::string gk_dayPlanNumber = gk_timeBaseDayPlanEntry + ".1";
const std::string gk_dayPlanEventNumber = gk_timeBaseDayPlanEntry + ".2";
const std::string gk_dayPlanHour = gk_timeBaseDayPlanEntry + ".3";
const std::string gk_dayPlanMinute = gk_timeBaseDayPlanEntry + ".4";
const std::string gk_dayPlanActionNumberOID = gk_timeBaseDayPlanEntry + ".5";
//LV13
const std::string gk_dayPlanStatus = gk_timebase + ".6.0";
const std::string gk_timeBaseScheduleTableStatus = gk_timebase + ".7.0";
//LV12
const std::string gk_controllerStandardTimeZone = gk_globalTimeManagement + ".5.0";
const std::string gk_controllerLocalTime = gk_globalTimeManagement + ".6.0";
const std::string gk_daylightSavingNode = gk_globalTimeManagement + ".7.0";
//LV11
const std::string gk_globalReport = gk_global + ".4";
//LV12
const std::string gk_maxEventLogConfigs = gk_globalReport + ".1.0";
const std::string gk_eventLogConfigTable = gk_globalReport + ".2";
//LV13
const std::string gk_eventLogConfigEntry = gk_eventLogConfigTable + ".1";
//LV12
const std::string gk_maxEventLogSize = gk_globalReport + ".3.0";
const std::string gk_eventLogTable = gk_globalReport + ".4";
//LV13
const std::string gk_eventLogEntry = gk_eventLogTable + ".1";
//LV12
const std::string gk_maxEventClasses = gk_globalReport + ".5.0";
const std::string gk_eventClassTable = gk_globalReport + ".6";
//LV13
const std::string gk_eventClassEntry = gk_eventClassTable + ".1";
//Lv12
const std::string gk_numEvents = gk_globalReport + ".7.0";
//LV11
const std::string gk_security = gk_global + ".5";
//LV12
const std::string gk_communityNameAdmin = gk_security + ".1.0";
const std::string gk_communityNamesMax = gk_security + ".2.0";
const std::string gk_communityNameTable = gk_security + ".3";
//LV13
const std::string gk_communityNameTableEntry = gk_communityNameTable + ".1";
//LV14
const std::string gk_communityNameIndex = gk_communityNameTableEntry + ".1";
const std::string gk_communityNameUser = gk_communityNameTableEntry + ".2";
const std::string gk_communityNameAccessMask = gk_communityNameTableEntry + ".3";

#endif // HNS_NTCIP_MIB_STRINGS_H
