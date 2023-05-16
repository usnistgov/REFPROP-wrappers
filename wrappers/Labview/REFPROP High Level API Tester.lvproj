<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="21008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="Refprop tester.vi" Type="VI" URL="../Refprop tester.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="BuildHelpPath.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/BuildHelpPath.vi"/>
				<Item Name="Check Special Tags.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Check Special Tags.vi"/>
				<Item Name="Clear Errors.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Clear Errors.vi"/>
				<Item Name="compatCalcOffset.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatCalcOffset.vi"/>
				<Item Name="compatFileDialog.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatFileDialog.vi"/>
				<Item Name="compatOpenFileOperation.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatOpenFileOperation.vi"/>
				<Item Name="Convert property node font to graphics font.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Convert property node font to graphics font.vi"/>
				<Item Name="Details Display Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Details Display Dialog.vi"/>
				<Item Name="DialogType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogType.ctl"/>
				<Item Name="DialogTypeEnum.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/DialogTypeEnum.ctl"/>
				<Item Name="Error Cluster From Error Code.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Cluster From Error Code.vi"/>
				<Item Name="Error Code Database.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Error Code Database.vi"/>
				<Item Name="ErrWarn.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/ErrWarn.ctl"/>
				<Item Name="eventvkey.ctl" Type="VI" URL="/&lt;vilib&gt;/event_ctls.llb/eventvkey.ctl"/>
				<Item Name="Find Tag.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Find Tag.vi"/>
				<Item Name="Format Message String.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Format Message String.vi"/>
				<Item Name="General Error Handler Core CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler Core CORE.vi"/>
				<Item Name="General Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/General Error Handler.vi"/>
				<Item Name="Get String Text Bounds.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Get String Text Bounds.vi"/>
				<Item Name="Get Text Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Get Text Rect.vi"/>
				<Item Name="GetHelpDir.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetHelpDir.vi"/>
				<Item Name="GetRTHostConnectedProp.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/GetRTHostConnectedProp.vi"/>
				<Item Name="Longest Line Length in Pixels.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Longest Line Length in Pixels.vi"/>
				<Item Name="LVBoundsTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVBoundsTypeDef.ctl"/>
				<Item Name="LVRectTypeDef.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/miscctls.llb/LVRectTypeDef.ctl"/>
				<Item Name="Not Found Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Not Found Dialog.vi"/>
				<Item Name="Open_Create_Replace File.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/Open_Create_Replace File.vi"/>
				<Item Name="Search and Replace Pattern.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Search and Replace Pattern.vi"/>
				<Item Name="Set Bold Text.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set Bold Text.vi"/>
				<Item Name="Set String Value.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Set String Value.vi"/>
				<Item Name="Simple Error Handler.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Simple Error Handler.vi"/>
				<Item Name="TagReturnType.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/TagReturnType.ctl"/>
				<Item Name="Three Button Dialog CORE.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog CORE.vi"/>
				<Item Name="Three Button Dialog.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Three Button Dialog.vi"/>
				<Item Name="Trim Whitespace.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/Trim Whitespace.vi"/>
				<Item Name="whitespace.ctl" Type="VI" URL="/&lt;vilib&gt;/Utility/error.llb/whitespace.ctl"/>
			</Item>
			<Item Name="ABFL1dll.vi" Type="VI" URL="../REFPROP/VIs/ABFL1dll.vi"/>
			<Item Name="ABFL2dll.vi" Type="VI" URL="../REFPROP/VIs/ABFL2dll.vi"/>
			<Item Name="ABFLSH_Test.vi" Type="VI" URL="../REFPROP/Tests/ABFLSH_Test.vi"/>
			<Item Name="ABFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/ABFLSHdll.vi"/>
			<Item Name="AGdll.vi" Type="VI" URL="../REFPROP/VIs/AGdll.vi"/>
			<Item Name="ALLPROP1_Test.vi" Type="VI" URL="../REFPROP/Tests/ALLPROP1_Test.vi"/>
			<Item Name="ALLPROPS0_Test.vi" Type="VI" URL="../REFPROP/Tests/ALLPROPS0_Test.vi"/>
			<Item Name="ALLPROPS0dll.vi" Type="VI" URL="../REFPROP/VIs/ALLPROPS0dll.vi"/>
			<Item Name="ALLPROPS1dll.vi" Type="VI" URL="../REFPROP/VIs/ALLPROPS1dll.vi"/>
			<Item Name="ALLPROPS20_Test.vi" Type="VI" URL="../REFPROP/Tests/ALLPROPS20_Test.vi"/>
			<Item Name="ALLPROPS20dll.vi" Type="VI" URL="../REFPROP/VIs/ALLPROPS20dll.vi"/>
			<Item Name="ALLPROPS_Test.vi" Type="VI" URL="../REFPROP/Tests/ALLPROPS_Test.vi"/>
			<Item Name="ALLPROPSdll.vi" Type="VI" URL="../REFPROP/VIs/ALLPROPSdll.vi"/>
			<Item Name="B12dll.vi" Type="VI" URL="../REFPROP/VIs/B12dll.vi"/>
			<Item Name="CHEMPOTdll.vi" Type="VI" URL="../REFPROP/VIs/CHEMPOTdll.vi"/>
			<Item Name="CP0dll.vi" Type="VI" URL="../REFPROP/VIs/CP0dll.vi"/>
			<Item Name="CRITPdll.vi" Type="VI" URL="../REFPROP/VIs/CRITPdll.vi"/>
			<Item Name="CSATKdll.vi" Type="VI" URL="../REFPROP/VIs/CSATKdll.vi"/>
			<Item Name="CSTARdll.vi" Type="VI" URL="../REFPROP/VIs/CSTARdll.vi"/>
			<Item Name="CV2PKdll.vi" Type="VI" URL="../REFPROP/VIs/CV2PKdll.vi"/>
			<Item Name="CVCPdll.vi" Type="VI" URL="../REFPROP/VIs/CVCPdll.vi"/>
			<Item Name="DBDTdll.vi" Type="VI" URL="../REFPROP/VIs/DBDTdll.vi"/>
			<Item Name="DBFL1dll.vi" Type="VI" URL="../REFPROP/VIs/DBFL1dll.vi"/>
			<Item Name="DDDPdll.vi" Type="VI" URL="../REFPROP/VIs/DDDPdll.vi"/>
			<Item Name="DEFL1dll.vi" Type="VI" URL="../REFPROP/VIs/DEFL1dll.vi"/>
			<Item Name="DEFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/DEFLSHdll.vi"/>
			<Item Name="DHD1dll.vi" Type="VI" URL="../REFPROP/VIs/DHD1dll.vi"/>
			<Item Name="DHFL1dll.vi" Type="VI" URL="../REFPROP/VIs/DHFL1dll.vi"/>
			<Item Name="DHFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/DHFLSHdll.vi"/>
			<Item Name="DIELECdll.vi" Type="VI" URL="../REFPROP/VIs/DIELECdll.vi"/>
			<Item Name="DLSATKdll.vi" Type="VI" URL="../REFPROP/VIs/DLSATKdll.vi"/>
			<Item Name="DPDD2dll.vi" Type="VI" URL="../REFPROP/VIs/DPDD2dll.vi"/>
			<Item Name="DPDDdll.vi" Type="VI" URL="../REFPROP/VIs/DPDDdll.vi"/>
			<Item Name="DPDTdll.vi" Type="VI" URL="../REFPROP/VIs/DPDTdll.vi"/>
			<Item Name="DPTSATKdll.vi" Type="VI" URL="../REFPROP/VIs/DPTSATKdll.vi"/>
			<Item Name="DQFL2dll.vi" Type="VI" URL="../REFPROP/VIs/DQFL2dll.vi"/>
			<Item Name="DSFL1dll.vi" Type="VI" URL="../REFPROP/VIs/DSFL1dll.vi"/>
			<Item Name="DSFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/DSFLSHdll.vi"/>
			<Item Name="DVSATKdll.vi" Type="VI" URL="../REFPROP/VIs/DVSATKdll.vi"/>
			<Item Name="ENTHALdll.vi" Type="VI" URL="../REFPROP/VIs/ENTHALdll.vi"/>
			<Item Name="ENTROdll.vi" Type="VI" URL="../REFPROP/VIs/ENTROdll.vi"/>
			<Item Name="ERRMSGdll.vi" Type="VI" URL="../REFPROP/VIs/ERRMSGdll.vi"/>
			<Item Name="ERRMSGdll_Test.vi" Type="VI" URL="../REFPROP/Tests/ERRMSGdll_Test.vi"/>
			<Item Name="ESFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/ESFLSHdll.vi"/>
			<Item Name="EXCESSdll.vi" Type="VI" URL="../REFPROP/VIs/EXCESSdll.vi"/>
			<Item Name="FGCTY2dll.vi" Type="VI" URL="../REFPROP/VIs/FGCTY2dll.vi"/>
			<Item Name="FGCTYdll.vi" Type="VI" URL="../REFPROP/VIs/FGCTYdll.vi"/>
			<Item Name="FLAGSdll.vi" Type="VI" URL="../REFPROP/VIs/FLAGSdll.vi"/>
			<Item Name="FLAGSdll_Test.vi" Type="VI" URL="../REFPROP/Tests/FLAGSdll_Test.vi"/>
			<Item Name="FLSH_Inputs.ctl" Type="VI" URL="../REFPROP/Ctl/FLSH_Inputs.ctl"/>
			<Item Name="FLSH_Outputs.ctl" Type="VI" URL="../REFPROP/Ctl/FLSH_Outputs.ctl"/>
			<Item Name="FLSHdll.vi" Type="VI" URL="../REFPROP/VIs/FLSHdll.vi"/>
			<Item Name="FPVdll.vi" Type="VI" URL="../REFPROP/VIs/FPVdll.vi"/>
			<Item Name="FUGCOFdll.vi" Type="VI" URL="../REFPROP/VIs/FUGCOFdll.vi"/>
			<Item Name="GERG04dll.vi" Type="VI" URL="../REFPROP/VIs/GERG04dll.vi"/>
			<Item Name="GETENUMdll.vi" Type="VI" URL="../REFPROP/VIs/GETENUMdll.vi"/>
			<Item Name="GETENUMdll_Test.vi" Type="VI" URL="../REFPROP/Tests/GETENUMdll_Test.vi"/>
			<Item Name="GETFIJdll.vi" Type="VI" URL="../REFPROP/VIs/GETFIJdll.vi"/>
			<Item Name="GETKTVdll.vi" Type="VI" URL="../REFPROP/VIs/GETKTVdll.vi"/>
			<Item Name="GIBBSdll.vi" Type="VI" URL="../REFPROP/VIs/GIBBSdll.vi"/>
			<Item Name="HEATdll.vi" Type="VI" URL="../REFPROP/VIs/HEATdll.vi"/>
			<Item Name="hrf_enum_d.ctl" Type="VI" URL="../REFPROP/Ctl/hrf_enum_d.ctl"/>
			<Item Name="HSFL1dll.vi" Type="VI" URL="../REFPROP/VIs/HSFL1dll.vi"/>
			<Item Name="HSFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/HSFLSHdll.vi"/>
			<Item Name="INFOdll.vi" Type="VI" URL="../REFPROP/VIs/INFOdll.vi"/>
			<Item Name="LIMITKdll.vi" Type="VI" URL="../REFPROP/VIs/LIMITKdll.vi"/>
			<Item Name="LIMITSdll.vi" Type="VI" URL="../REFPROP/VIs/LIMITSdll.vi"/>
			<Item Name="LIMITXdll.vi" Type="VI" URL="../REFPROP/VIs/LIMITXdll.vi"/>
			<Item Name="MAXPdll.vi" Type="VI" URL="../REFPROP/VIs/MAXPdll.vi"/>
			<Item Name="MAXTdll.vi" Type="VI" URL="../REFPROP/VIs/MAXTdll.vi"/>
			<Item Name="MELTPdll.vi" Type="VI" URL="../REFPROP/VIs/MELTPdll.vi"/>
			<Item Name="MELTTdll.vi" Type="VI" URL="../REFPROP/VIs/MELTTdll.vi"/>
			<Item Name="Mixture Composition cluster_d.ctl" Type="VI" URL="../REFPROP/Ctl/Mixture Composition cluster_d.ctl"/>
			<Item Name="MLTH2Odll.vi" Type="VI" URL="../REFPROP/VIs/MLTH2Odll.vi"/>
			<Item Name="NAMEdll.vi" Type="VI" URL="../REFPROP/VIs/NAMEdll.vi"/>
			<Item Name="PASSCMNdll.vi" Type="VI" URL="../REFPROP/VIs/PASSCMNdll.vi"/>
			<Item Name="PDFL1dll.vi" Type="VI" URL="../REFPROP/VIs/PDFL1dll.vi"/>
			<Item Name="PDFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/PDFLSHdll.vi"/>
			<Item Name="PEFL1dll.vi" Type="VI" URL="../REFPROP/VIs/PEFL1dll.vi"/>
			<Item Name="PEFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/PEFLSHdll.vi"/>
			<Item Name="PHFL1dll.vi" Type="VI" URL="../REFPROP/VIs/PHFL1dll.vi"/>
			<Item Name="PHFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/PHFLSHdll.vi"/>
			<Item Name="PHIXdll.vi" Type="VI" URL="../REFPROP/VIs/PHIXdll.vi"/>
			<Item Name="PQFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/PQFLSHdll.vi"/>
			<Item Name="PREOSdll.vi" Type="VI" URL="../REFPROP/VIs/PREOSdll.vi"/>
			<Item Name="PRESSdll.vi" Type="VI" URL="../REFPROP/VIs/PRESSdll.vi"/>
			<Item Name="PSATKdll.vi" Type="VI" URL="../REFPROP/VIs/PSATKdll.vi"/>
			<Item Name="PSFL1dll.vi" Type="VI" URL="../REFPROP/VIs/PSFL1dll.vi"/>
			<Item Name="PSFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/PSFLSHdll.vi"/>
			<Item Name="PUREFLDdll.vi" Type="VI" URL="../REFPROP/VIs/PUREFLDdll.vi"/>
			<Item Name="QMASSdll.vi" Type="VI" URL="../REFPROP/VIs/QMASSdll.vi"/>
			<Item Name="QMOLEdll.vi" Type="VI" URL="../REFPROP/VIs/QMOLEdll.vi"/>
			<Item Name="REDXdll.vi" Type="VI" URL="../REFPROP/VIs/REDXdll.vi"/>
			<Item Name="REFPROP.dll" Type="Document" URL="../REFPROP/REFPROP.dll"/>
			<Item Name="REFPROP1dll.vi" Type="VI" URL="../REFPROP/VIs/REFPROP1dll.vi"/>
			<Item Name="REFPROP1dll_Test.vi" Type="VI" URL="../REFPROP/Tests/REFPROP1dll_Test.vi"/>
			<Item Name="REFPROP2dll.vi" Type="VI" URL="../REFPROP/VIs/REFPROP2dll.vi"/>
			<Item Name="REFPROP2dll_Test.vi" Type="VI" URL="../REFPROP/Tests/REFPROP2dll_Test.vi"/>
			<Item Name="REFPROP_ErrorConverter.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/REFPROP_ErrorConverter.vi"/>
			<Item Name="Refprop_Example_main.vi" Type="VI" URL="../REFPROP/Refprop_Example/Refprop_Example_main.vi"/>
			<Item Name="REFPROPdll.vi" Type="VI" URL="../REFPROP/VIs/REFPROPdll.vi"/>
			<Item Name="REFPROPdll_Test.vi" Type="VI" URL="../REFPROP/Tests/REFPROPdll_Test.vi"/>
			<Item Name="RESIDUALdll.vi" Type="VI" URL="../REFPROP/VIs/RESIDUALdll.vi"/>
			<Item Name="RP_Version.vi" Type="VI" URL="../REFPROP/VIs/RP_Version.vi"/>
			<Item Name="SAT.vi" Type="VI" URL="../REFPROP/VIs/SAT.vi"/>
			<Item Name="SAT_Outputs.ctl" Type="VI" URL="../REFPROP/Ctl/SAT_Outputs.ctl"/>
			<Item Name="SATDdll.vi" Type="VI" URL="../REFPROP/VIs/SATDdll.vi"/>
			<Item Name="SATEdll.vi" Type="VI" URL="../REFPROP/VIs/SATEdll.vi"/>
			<Item Name="SATHdll.vi" Type="VI" URL="../REFPROP/VIs/SATHdll.vi"/>
			<Item Name="SATPdll.vi" Type="VI" URL="../REFPROP/VIs/SATPdll.vi"/>
			<Item Name="SATSdll.vi" Type="VI" URL="../REFPROP/VIs/SATSdll.vi"/>
			<Item Name="SATSPLNdll.vi" Type="VI" URL="../REFPROP/VIs/SATSPLNdll.vi"/>
			<Item Name="SATTdll.vi" Type="VI" URL="../REFPROP/VIs/SATTdll.vi"/>
			<Item Name="SATTPdll.vi" Type="VI" URL="../REFPROP/VIs/SATTPdll.vi"/>
			<Item Name="SETAGAdll.vi" Type="VI" URL="../REFPROP/VIs/SETAGAdll.vi"/>
			<Item Name="SETFLUIDSdll.vi" Type="VI" URL="../REFPROP/VIs/SETFLUIDSdll.vi"/>
			<Item Name="SETFLUIDSdll_Test.vi" Type="VI" URL="../REFPROP/Tests/SETFLUIDSdll_Test.vi"/>
			<Item Name="SETKTVdll.vi" Type="VI" URL="../REFPROP/VIs/SETKTVdll.vi"/>
			<Item Name="SETMIXdll.vi" Type="VI" URL="../REFPROP/VIs/SETMIXdll.vi"/>
			<Item Name="SETMIXTUREdll.vi" Type="VI" URL="../REFPROP/VIs/SETMIXTUREdll.vi"/>
			<Item Name="SETMIXTUREdll_Test.vi" Type="VI" URL="../REFPROP/Tests/SETMIXTUREdll_Test.vi"/>
			<Item Name="SETMODdll.vi" Type="VI" URL="../REFPROP/VIs/SETMODdll.vi"/>
			<Item Name="SETNCdll.vi" Type="VI" URL="../REFPROP/VIs/SETNCdll.vi"/>
			<Item Name="SETPATHdll.vi" Type="VI" URL="../REFPROP/VIs/SETPATHdll.vi"/>
			<Item Name="SETPATHdll_Test.vi" Type="VI" URL="../REFPROP/Tests/SETPATHdll_Test.vi"/>
			<Item Name="SETREFdll.vi" Type="VI" URL="../REFPROP/VIs/SETREFdll.vi"/>
			<Item Name="SETUPdll.vi" Type="VI" URL="../REFPROP/VIs/SETUPdll.vi"/>
			<Item Name="SUBLPdll.vi" Type="VI" URL="../REFPROP/VIs/SUBLPdll.vi"/>
			<Item Name="SUBLTdll.vi" Type="VI" URL="../REFPROP/VIs/SUBLTdll.vi"/>
			<Item Name="SURFTdll.vi" Type="VI" URL="../REFPROP/VIs/SURFTdll.vi"/>
			<Item Name="SURTENdll.vi" Type="VI" URL="../REFPROP/VIs/SURTENdll.vi"/>
			<Item Name="TDFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/TDFLSHdll.vi"/>
			<Item Name="TEFL1dll.vi" Type="VI" URL="../REFPROP/VIs/TEFL1dll.vi"/>
			<Item Name="TEFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/TEFLSHdll.vi"/>
			<Item Name="THERM0dll.vi" Type="VI" URL="../REFPROP/VIs/THERM0dll.vi"/>
			<Item Name="THERM2dll.vi" Type="VI" URL="../REFPROP/VIs/THERM2dll.vi"/>
			<Item Name="THERM3dll.vi" Type="VI" URL="../REFPROP/VIs/THERM3dll.vi"/>
			<Item Name="THERMdll.vi" Type="VI" URL="../REFPROP/VIs/THERMdll.vi"/>
			<Item Name="THFL1dll.vi" Type="VI" URL="../REFPROP/VIs/THFL1dll.vi"/>
			<Item Name="THFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/THFLSHdll.vi"/>
			<Item Name="TPFL2dll.vi" Type="VI" URL="../REFPROP/VIs/TPFL2dll.vi"/>
			<Item Name="TPFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/TPFLSHdll.vi"/>
			<Item Name="TPRHOdll.vi" Type="VI" URL="../REFPROP/VIs/TPRHOdll.vi"/>
			<Item Name="TQFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/TQFLSHdll.vi"/>
			<Item Name="Transfer_FLSH_Format.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Transfer_FLSH_Format.vi"/>
			<Item Name="Transfer_SAT_Format.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Transfer_SAT_Format.vi"/>
			<Item Name="TRNPRPdll.vi" Type="VI" URL="../REFPROP/VIs/TRNPRPdll.vi"/>
			<Item Name="TSATDdll.vi" Type="VI" URL="../REFPROP/VIs/TSATDdll.vi"/>
			<Item Name="TSFL1dll.vi" Type="VI" URL="../REFPROP/VIs/TSFL1dll.vi"/>
			<Item Name="TSFLSHdll.vi" Type="VI" URL="../REFPROP/VIs/TSFLSHdll.vi"/>
			<Item Name="UNSETAGAdll.vi" Type="VI" URL="../REFPROP/VIs/UNSETAGAdll.vi"/>
			<Item Name="Utility_Calculate.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Utility_Calculate.vi"/>
			<Item Name="Utility_Initialization.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Utility_Initialization.vi"/>
			<Item Name="Utility_Pad_String.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Utility_Pad_String.vi"/>
			<Item Name="Utility_Parse_Refrigerant_Mixture_File.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Utility_Parse_Refrigerant_Mixture_File.vi"/>
			<Item Name="Utility_Read_and_Format_Refrigerant_and_Mixtures.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Utility_Read_and_Format_Refrigerant_and_Mixtures.vi"/>
			<Item Name="Utility_Read_Fluid_Cluster.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Utility_Read_Fluid_Cluster.vi"/>
			<Item Name="Utility_Read_Fluid_XML.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Utility_Read_Fluid_XML.vi"/>
			<Item Name="Utility_Read_Mixture_Cluster.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Utility_Read_Mixture_Cluster.vi"/>
			<Item Name="Utility_Read_Mixture_XML.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Utility_Read_Mixture_XML.vi"/>
			<Item Name="Utility_Read_Refrigerant_File.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Utility_Read_Refrigerant_File.vi"/>
			<Item Name="Utility_Setup.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Utility_Setup.vi"/>
			<Item Name="Utility_Strip_and_Replace_File_Suffix.vi" Type="VI" URL="../REFPROP/UTILITY SUBROUTINES/Utility_Strip_and_Replace_File_Suffix.vi"/>
			<Item Name="variable_refprop.ctl" Type="VI" URL="../REFPROP/Ctl/variable_refprop.ctl"/>
			<Item Name="VIRBAdll.vi" Type="VI" URL="../REFPROP/VIs/VIRBAdll.vi"/>
			<Item Name="VIRBdll.vi" Type="VI" URL="../REFPROP/VIs/VIRBdll.vi"/>
			<Item Name="VIRCAdll.vi" Type="VI" URL="../REFPROP/VIs/VIRCAdll.vi"/>
			<Item Name="VIRCdll.vi" Type="VI" URL="../REFPROP/VIs/VIRCdll.vi"/>
			<Item Name="WMOLdll.vi" Type="VI" URL="../REFPROP/VIs/WMOLdll.vi"/>
			<Item Name="XMASSdll.vi" Type="VI" URL="../REFPROP/VIs/XMASSdll.vi"/>
			<Item Name="XMOLEdll.vi" Type="VI" URL="../REFPROP/VIs/XMOLEdll.vi"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
