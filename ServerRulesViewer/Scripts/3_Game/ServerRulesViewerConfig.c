const int SRV_VERSION = 119022024;

class SRV_Config
{
	int version;
	string title;
	string donateLink;
	ref array<ref SRV_Category> categories;
	
	void CheckVersion()
	{
		if (version != SRV_VERSION)
		{
			Print("[ServerRulesViewer] Updating version... " + version + " -> " + SRV_VERSION);
			version = SRV_VERSION;
		}
	}
}

class SRV_Category
{
	string category;
	ref TStringArray rules;
}
