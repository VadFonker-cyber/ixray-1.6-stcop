

namespace scripts {

	extern void SemiLog(const std::string& Msg);

	inline int round(double value) { return std::round(value); }
	
	extern long long time_global();


	using class_info_data = luabind::class_info;

	//////using header			= CALifeSimulatorHeader;
	//////using spawns			= CALifeSpawnRegistry;
	//////using objects			= CALifeObjectRegistry;
	///using story_objects		= CALifeStoryRegistry;
	//////using smart_terrains	= CALifeSmartTerrainRegistry;
	//////using groups			= CALifeGroupRegistry;

}
