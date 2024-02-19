modded class PluginManager 
{
	override void Init() 
	{
		super.Init();
		//----------------------------------------------------------------------
		// Register modules
		//----------------------------------------------------------------------
		//				Module Class Name 						Client	Server
		//----------------------------------------------------------------------
		RegisterPlugin("SRV_ConfigManagerPlugin", true, true);
	}
}