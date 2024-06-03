#pragma once

namespace scripts {
    namespace utils {

        // not used!!
        xr_string cfg_get_string(CScriptIniFile* char_ini, const xr_string& section, const xr_string& field, void* object, bool mandatory, const xr_string& gulag_name, const xr_string& default_val);
        
        xr_string cfg_get_string(CInifile* char_ini, const xr_string& section, const xr_string& field, void* object, bool mandatory, const xr_string& gulag_name, const xr_string& default_val);
    
    }
}
