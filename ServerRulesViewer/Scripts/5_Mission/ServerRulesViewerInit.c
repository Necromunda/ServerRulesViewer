modded class MissionServer
{	
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity) 
	{
        super.InvokeOnConnect(player, identity);

        if (!player)
			return;
		
        auto configParams = new Param1<SRV_Config>(GetSRVConfigManagerPlugin().GetConfig());
        GetGame().RPCSingleParam(player, SRV_RPC.RPC_RECEIVE_CONFIG, configParams, true, identity);
    }
};


modded class MissionBase 
{
    override UIScriptedMenu CreateScriptedMenu(int id) 
	{
        UIScriptedMenu menu = NULL;
        menu = super.CreateScriptedMenu(id);
		
        if (!menu) 
		{
            switch (id) 
			{
	            case SRV_MENU_ID:
	                menu = new SRV_RulesUI;
	                break;
            }
			
            if (menu) 
			{
                menu.SetID(id);
            }
        }
		
        return menu;
    }
};

modded class MissionGameplay 
{
	UAInput m_SRV_Keybind = GetUApi().GetInputByName("ServerRulesViewer");
	
	/*
	override void OnKeyPress( int key )
    {
        super.OnKeyPress( key );
		
		if (key == "ServerRulesViewer")
		{
			SRV_RulesUI ui = SRV_RulesUI.Cast(GetGame().GetUIManager().EnterScriptedMenu(SRV_MENU_ID, NULL));
			SRV_Config config = GetSRVConfigManagerPlugin().GetConfig();
			ui.SetData(config);
		}
    }
	*/
	
	override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
		
		if (m_SRV_Keybind.LocalPress())
		{
			GetGame().GetUIManager().EnterScriptedMenu(SRV_MENU_ID, NULL);
		}
    }
};