<distribution version="13.0.2" name="WallE_testsoftware" type="MSI">
	<prebuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></prebuild>
	<postbuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></postbuild>
	<msi GUID="{EF036086-E16D-414B-8F76-87FA2813D1D8}">
		<general appName="WallE_testsoftware" outputLocation="c:\WallE\WallE_Test_src\cvidistkit.WallE_testsoftware" relOutputLocation="cvidistkit.WallE_testsoftware" outputLocationWithVars="c:\WallE\WallE_Test_src\cvidistkit.%name" relOutputLocationWithVars="cvidistkit.%name" upgradeBehavior="1" autoIncrement="true" version="1.0.5">
			<arp company="Active Mind Technology" companyURL="" supportURL="" contact="" phone="" comments=""/>
			<summary title="" subject="" keyWords="" comments="" author=""/></general>
		<userinterface language="English" showPaths="true" readMe="" license="">
			<dlgstrings welcomeTitle="WallE_testsoftware" welcomeText=""/></userinterface>
		<dirs appDirID="100">
			<installDir name="[Program Files]" dirID="2" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="WallE_testsoftware" dirID="100" parentID="2" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="C:" dirID="101" parentID="-1" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="walle_init_path" dirID="102" parentID="101" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="[Start&gt;&gt;Programs]" dirID="7" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="WallE_testsoftware" dirID="103" parentID="7" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="walle_test_results" dirID="104" parentID="101" isMSIDir="false" visible="true" unlock="false"/></dirs>
		<files>
			<simpleFile fileID="0" sourcePath="c:\WallE\WallE_Test_src\ConfigFiles\ModuleLevelTestItems_LabSat3.ini" relSourcePath="ConfigFiles\ModuleLevelTestItems_LabSat3.ini" relSourceBase="0" targetDir="102" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="1" sourcePath="c:\WallE\WallE_Test_src\ConfigFiles\TestStationOptions.ini" relSourcePath="ConfigFiles\TestStationOptions.ini" relSourceBase="0" targetDir="104" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="2" sourcePath="c:\WallE\WallE_Test_src\ConfigFiles\ModuleLevelTestItems_Prog.ini" relSourcePath="ConfigFiles\ModuleLevelTestItems_Prog.ini" relSourceBase="0" targetDir="102" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="3" sourcePath="c:\WallE\WallE_Test_src\ConfigFiles\PCBALevelTestItems.ini" relSourcePath="ConfigFiles\PCBALevelTestItems.ini" relSourceBase="0" targetDir="102" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="4" sourcePath="c:\WallE\WallE_Test_src\ConfigFiles\PCBALevelTestItems_GPS10.ini" relSourcePath="ConfigFiles\PCBALevelTestItems_GPS10.ini" relSourceBase="0" targetDir="102" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="5" sourcePath="c:\WallE\WallE_Test_src\ConfigFiles\ModuleLevelTestItems_GPS10.ini" relSourcePath="ConfigFiles\ModuleLevelTestItems_GPS10.ini" relSourceBase="0" targetDir="102" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="6" sourcePath="c:\WallE\WallE_Test_src\WallE.uir" relSourcePath="WallE.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="7" sourcePath="c:\WallE\WallE_Test_src\cvibuild.WallE_testsoftware\Release\WallE_testsoftware.exe" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="8" sourcePath="c:\WallE\WallE_Test_src\ConfigFiles\board_rev.ini" relSourcePath="ConfigFiles\board_rev.ini" relSourceBase="0" targetDir="102" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="9" sourcePath="c:\WallE\WallE_Test_src\ConfigFiles\ModuleLevelTestItems.ini" relSourcePath="ConfigFiles\ModuleLevelTestItems.ini" relSourceBase="0" targetDir="102" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/></files>
		<fileGroups>
			<projectOutput targetType="0" dirID="100" projectID="0">
				<fileID>7</fileID></projectOutput></fileGroups>
		<shortcuts>
			<shortcut name="WallE_testsoftware" targetFileID="7" destDirID="103" cmdLineArgs="" description="" runStyle="NORMAL"/></shortcuts>
		<mergemodules/>
		<products>
			<product name="NI LabWindows/CVI Shared Run-Time Engine" UC="{80D3D303-75B9-4607-9312-E5FC68E5BFD2}" productID="{D3C549FD-7DA5-440B-A1BC-DD92C898949A}" path="C:\ProgramData\National Instruments\MDF\ProductCache\" flavorID="_full_" flavorName="Full" verRestr="false" coreVer="">
				<cutSoftDeps>
					<softDep name="TDMS Support" UC="{5A8AF88D-486D-4E30-A7A5-8D8A039BBEBF}" depKey="CVI_Tdms.CVI1320.RTE"/>
					<softDep name="Visual C++ 2008 Redistributable" UC="{BD1E864A-2ACC-4F7E-9A7D-977C39952A52}" depKey="VC2008MSMs_Redist.CVI1320.RTE"/>
					<softDep name="Visual C++ 2010 Redistributable" UC="{3109870C-C165-4896-AC99-AFDD3CA3304A}" depKey="VC2010MSMs_Redist.CVI1320.RTE"/></cutSoftDeps></product>
			<product name="NI Measurement &amp; Automation Explorer 14.5" UC="{AE940F24-CC0E-4148-9A96-10FB04D9796D}" productID="{67BC0011-6FDE-4396-9B16-11AD6A9DE960}" path="C:\ProgramData\National Instruments\MDF\ProductCache\" flavorID="_full_" flavorName="Full" verRestr="false" coreVer="14.50.49152">
				<cutSoftDeps/></product>
			<product name="NI-488.2 Runtime 14.0" UC="{357F6618-C660-41A2-A185-5578CC876D1D}" productID="{E1C7086A-D7A4-453E-990E-7BBC3213B732}" path="C:\ProgramData\National Instruments\MDF\ProductCache\" flavorID="_full_" flavorName="NI-488.2" verRestr="false" coreVer="14.0.49153"/></products>
		<runtimeEngine installToAppDir="false" activeXsup="true" analysis="true" cvirte="true" dotnetsup="true" instrsup="true" lowlevelsup="true" lvrt="true" netvarsup="true" rtutilsup="true">
			<hasSoftDeps/></runtimeEngine><sxsRuntimeEngine>
			<selected>false</selected>
			<doNotAutoSelect>false</doNotAutoSelect></sxsRuntimeEngine>
		<advanced mediaSize="650">
			<launchConditions>
				<condition>MINOS_WINXP_SP0</condition>
			</launchConditions>
			<includeConfigProducts>true</includeConfigProducts>
			<maxImportVisible>silent</maxImportVisible>
			<maxImportMode>merge</maxImportMode>
			<custMsgFlag>false</custMsgFlag>
			<custMsgPath>msgrte.txt</custMsgPath>
			<signExe>false</signExe>
			<certificate></certificate>
			<signTimeURL></signTimeURL>
			<signDescURL></signDescURL></advanced>
		<Projects NumProjects="1">
			<Project000 ProjectID="0" ProjectAbsolutePath="c:\WallE\WallE_Test_src\WallE_testsoftware.prj" ProjectRelativePath="WallE_testsoftware.prj"/></Projects>
		<buildData progressBarRate="0.518107488041180">
			<progressTimes>
				<Begin>0.000000000000000</Begin>
				<ProductsAdded>1.842444406250000</ProductsAdded>
				<DPConfigured>5.570569250000000</DPConfigured>
				<DPMergeModulesAdded>6.272161656250000</DPMergeModulesAdded>
				<DPClosed>9.792445375000002</DPClosed>
				<DistributionsCopied>11.212868593749999</DistributionsCopied>
				<End>193.010142312500020</End></progressTimes></buildData>
	</msi>
</distribution>
