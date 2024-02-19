class SRV_ConfigManagerPlugin : PluginBase 
{
    private ref SRV_Config _config;

    void SRV_ConfigManagerPlugin() 
	{
        GetDayZGame().Event_OnRPC.Insert(OnRPC);
        
        if (!GetGame().IsServer() || !GetGame().IsMultiplayer() || GetGame().IsClient() || !GetGame().IsDedicatedServer()) return;
        string modDir = "$profile:/ServerRulesViewer";
        string configName = "RulesConfig.json";
        string configDir = modDir + "/" + configName;

        if (!FileExist(modDir)) 
		{
            MakeDirectory(modDir);
        }
        if (!FileExist(configDir)) 
		{
            _config = new SRV_Config();
        } 
		else 
		{
            JsonFileLoader<SRV_Config>.JsonLoadFile(configDir, _config);
            // _config.Validate();
        }
		_config.CheckVersion();
        JsonFileLoader<SRV_Config>.JsonSaveFile(configDir, _config);
    }

    void ~SRV_ConfigManagerPlugin() 
	{
        GetDayZGame().Event_OnRPC.Remove(OnRPC);
    }

    void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx) 
	{
        if (GetGame().IsServer() && GetGame().IsMultiplayer()) return;
        if (rpc_type != SRV_RPC.RPC_RECEIVE_CONFIG) return;
        Param1<SRV_Config> dataConfig;
        
        if (!ctx.Read(dataConfig)) return;
        SetConfig(dataConfig.param1);
    }

    void SetConfig(SRV_Config config) 
	{
        if (_config != null) 
			return;
        _config = config;
    }

    SRV_Config GetConfig() 
	{
        return _config;
    }
}

SRV_ConfigManagerPlugin GetSRVConfigManagerPlugin() 
{
    return SRV_ConfigManagerPlugin.Cast(GetPlugin(SRV_ConfigManagerPlugin));
}