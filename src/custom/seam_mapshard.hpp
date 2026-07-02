// Copyright (c) rAthena Dev Teams - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef SEAM_MAPSHARD_HPP
#define SEAM_MAPSHARD_HPP

// [SATHENA-SEAM interface] MapShardSeam — runtime map-sharding. keepMap fires in map_addmap for
// every map in the server's list; return false to DROP the map from THIS instance, so one
// binary serves any subset per launch (shard assignment = env / control message, read in the
// consumer). The char server routes players to the owner automatically (chrif_changemapserver);
// the expensive per-map data (spawns/NPCs) is already map-scoped, so dropping a map is cheap.
// Default keeps every map => plain single-server behavior.

struct MapShardSeam{
	virtual bool keepMap( const char* mapname ){ return true; }

	virtual ~MapShardSeam() = default;
};

inline MapShardSeam*& map_shard_seam(){
	static MapShardSeam s_default;
	static MapShardSeam* s_current = &s_default;
	return s_current;
}

#endif /* SEAM_MAPSHARD_HPP */
