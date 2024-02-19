class CfgPatches
{
	class ServerRulesViewer_Scripts
	{
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
		requiredAddons[] = 
		{
			"DZ_Data"
		};
	};
};
class CfgMods 
{
	class ServerRulesViewer
	{
		dir = "ServerRulesViewer";
		credits = "";
		author = "";
		creditsJson = "ServerRulesViewer/ServerRulesViewer/Data/Credits.json";
		versionPath = "ServerRulesViewer/ServerRulesViewer/Data/Version.hpp";
		inputs = "ServerRulesViewer/ServerRulesViewer/Data/Inputs.xml";
		type = "mod";
		dependencies[] =
		{
			"Game", "World", "Mission"
		};
		class defs
		{
			class imageSets
			{
				files[]=
				{
					"ServerRulesViewer/ServerRulesViewer/GUI/imagesets/srv_ui.imageset"
				};
			};
			class gameScriptModule
			{
				value="";
				files[] = 
				{
					"ServerRulesViewer/ServerRulesViewer/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] = 
				{
					"ServerRulesViewer/ServerRulesViewer/scripts/4_World"
				};
			};

			class missionScriptModule 
			{
				value="";
				files[] = 
				{
					"ServerRulesViewer/ServerRulesViewer/scripts/5_Mission"
				};
			};
		};
	};
};
