#include "_script_port_headers.h"

namespace scripts {

    // Some X-Ray block
    bool DebugLog = true;
    bool DebuggerMode = false;

    void log(const std::string& str)
    {
        if (DebugLog) {
            SemiLog(str);
        }
        if (DebuggerMode) {
            ///LuaPanda::printToVSCode(str, 1, 2);
        }
    }

    void printf(const char* fmt, ...) 
    {
        va_list args;
        va_start(args, fmt);
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
        log(buffer);
    }


    bool empty(const std::function<void()>& container) 
    {
        return true; 
    }

    bool empty(const std::unordered_map<std::string, std::string>& container) 
    {
        if (!container.empty()) {
            return false;
        }
        return true;
    }



    std::unordered_map<std::string, std::string> schemes; // соответствие схем модулям
    std::unordered_map<std::string, std::string> stypes; // типы схем

    void load_scheme(const std::string& filename, const std::string& scheme, const std::string& stype) 
    {
        schemes[scheme] = filename;
        stypes[scheme] = stype;
    }

    bool dev_debug = false;
    bool sim_debug = false;
    int mus_vol = 0;
    int amb_vol = 0;
    bool b_discard_settings_shown = false;

    void wait_game(long long time_to_wait) {
        // Placeholder for actual thread verification
        if (time_to_wait == 0) {
            std::this_thread::yield();
        } else {
            auto time_to_stop = std::chrono::system_clock::now() + std::chrono::milliseconds(time_to_wait);
            while (std::chrono::system_clock::now() < time_to_stop) {
                std::this_thread::yield();
            }
        }
    }

    void wait(long long time_to_wait) {
        // Placeholder for actual thread verification
        if (time_to_wait == 0) {
            std::this_thread::yield();
        } else {
            auto time_to_stop = std::chrono::system_clock::now() + std::chrono::milliseconds(time_to_wait);
            while (std::chrono::system_clock::now() < time_to_stop) {
                std::this_thread::yield();
            }
        }
    }


    class statictest {
    public:
        statictest() {

        }
    };
    static statictest statictest_;


    // 

    ////

    //double distance_between(const SomeType& obj1, const SomeType& obj2) {
    //    // Placeholder for actual position and distance calculation
    //    return 0.0;
    //}

    //double distance_between_safe(const SomeType* obj1, const SomeType* obj2) {
    //    if (obj1 == nullptr || obj2 == nullptr) return 100000;
    //    return distance_between(*obj1, *obj2);
    //}

    //bool has_alife_info(int info_id) {
    //    // Placeholder for actual ALife system check
    //    return false;
    //}

    //void reset_action(SomeType& npc, const std::string& script_name) {
    //    // Placeholder for actual script handling
    //    npc.script(false, script_name);
    //    npc.script(true, script_name);
    //}



    //// Constants
    //const int time_infinite = 100000000;

    //// Function to interrupt an action and disable script mode
    //void interrupt_action(CObject* who, const string& script_name) {
    //    if (who->get_script()) {
    //        who->script(false, script_name);
    //    }
    //}

    //// Function to randomly choose an element from a list
    //template <typename T>
    //T random_choice(const std::vector<T>& args) {
    //    int r = rand() % args.size() + 1;
    //    return args[r - 1];
    //}

    //// Function to implement if-then-else logic
    //template <typename T, typename U>
    //T if_then_else(bool cond, T if_true, U if_false) {
    //    return cond ? if_true : static_cast<T>(if_false);
    //}

    //// Function to set the current time
    //void set_current_time(int hour, int min, int sec) {
    //    float current_time_factor = level.get_time_factor();
    //    printf("Need time : %d:%d:%d", hour, min, sec);
    //    u32 current_time = game.time();
    //    u32 c_day = current_time / 86400000;
    //    u32 c_time = current_time - c_day * 86400000;
    //    u32 n_time = (sec + min * 60 + hour * 3600) * 1000;

    //    if (c_time > n_time) c_day++;
    //    n_time += c_day * 86400000;
    //    level.set_time_factor(10000);
    //    while (game.time() < n_time) {
    //        wait();
    //    }
    //    level.set_time_factor(current_time_factor);
    //}

    //// Function to generate a random number
    //int random_number(int min_value = 0, int max_value = RAND_MAX) {
    //    std::random_device rd;
    //    std::mt19937 gen(rd());
    //    std::uniform_int_distribution<> dis(min_value, max_value);
    //    return dis(gen);
    //}

    //// Function to parse names from a string
    //std::vector<std::string> parse_names(const std::string& s) {
    //    std::vector<std::string> t;
    //    std::regex pattern("[\\w_\\\\]+\\p*");
    //    std::smatch match;
    //    std::string str = s;
    //    while (std::regex_search(str, match, pattern)) {
    //        t.push_back(match[0]);
    //        str = match.suffix().str();
    //    }
    //    return t;
    //}

    //// Function to parse key-value pairs from a string
    //std::unordered_map<std::string, std::string> parse_key_value(const std::string& s) {
    //    std::unordered_map<std::string, std::string> t;
    //    if (s.empty()) {
    //        return t;
    //    }
    //    std::string key, name;
    //    std::regex pattern("[\\w_\\\\]+\\p*");
    //    std::smatch match;
    //    std::string str = s;
    //    while (std::regex_search(str, match, pattern)) {
    //        if (key.empty()) {
    //            key = match[0];
    //        } else {
    //            t[key] = match[0];
    //            key.clear();
    //        }
    //        str = match.suffix().str();
    //    }
    //    return t;
    //}

    //// Function to parse numbers from a string
    //std::vector<double> parse_nums(const std::string& s) {
    //    std::vector<double> t;
    //    std::regex pattern("[-\\d\\.]+\\,*");
    //    std::smatch match;
    //    std::string str = s;
    //    while (std::regex_search(str, match, pattern)) {
    //        t.push_back(std::stod(match[0]));
    //        str = match.suffix().str();
    //    }
    //    return t;
    //}

    //// Function to check if an object is online
    //bool is_object_online(u16 obj_id) {
    //    return level.object_by_id(obj_id) != nullptr;
    //}

    //// Function to get the class ID of an NPC
    //shared_str get_clsid(CObject* npc) {
    //    if (npc == nullptr) {
    //        return shared_str();
    //    }
    //    return npc->clsid();
    //}

    //// Function to calculate yaw in radians
    //float yaw(const Fvector& v1, const Fvector& v2) {
    //    return acosf(((v1.x * v2.x) + (v1.z * v2.z)) / (sqrtf(v1.x * v1.x + v1.z * v1.z) * sqrtf(v2.x * v2.x + v2.z * v2.z)));
    //}

    //// Function to calculate yaw in degrees
    //float yaw_degree(const Fvector& v1, const Fvector& v2) {
    //    return (acosf(((v1.x * v2.x) + (v1.z * v2.z)) / (sqrtf(v1.x * v1.x + v1.z * v1.z) * sqrtf(v2.x * v2.x + v2.z * v2.z))) * 57.2957f);
    //}

    //// Function to calculate yaw in degrees (3D)
    //float yaw_degree3d(const Fvector& v1, const Fvector& v2) {
    //    return (acosf((v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) / (sqrtf(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z) * sqrtf(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z))) * 57.2957f);
    //}

    //// Function to calculate the cross product of two vectors
    //Fvector vector_cross(const Fvector& v1, const Fvector& v2) {
    //    return Fvector(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
    //}

    //// Function to rotate a vector around the Y-axis counterclockwise
    //Fvector vector_rotate_y(const Fvector& v, float angle) {
    //    float c = cosf(angle * 0.017453292519943295769236907684886f);
    //    float s = sinf(angle * 0.017453292519943295769236907684886f);
    //    return Fvector(v.x * c - v.z * s, v.y, v.x * s + v.z * c);
    //}

    //// Function to clear a container
    //template <typename T>
    //void clear_table(T& t) {
    //    t.clear();
    //}

    //// Function to stop playing a sound on an object
    //void stop_play_sound(CObject* obj) {
    //    if (obj->alive()) {
    //        obj->set_sound_mask(-1);
    //        obj->set_sound_mask(0);
    //    }
    //}

    // Function to print a table as a tree
    void print_table(const xr_hash_map<xr_string, int>& table, const std::string& subs) {
        for (const auto& [k, v] : table) {
            printf("%s%s:%d", subs.c_str(), k.c_str(), v);
        }
    }

    ///*void print_table(const xr_hash_map<xr_string, int>& table, const std::string& subs = "") {
    //    for (const auto& [k, v] : table) {
    //        if (v.type() == typeid(std::unordered_map<std::string, std::any>)) {
    //            printf("%s%s:", subs.c_str(), k.c_str());
    //            print_table(std::any_cast<std::unordered_map<std::string, std::any>>(v), subs + "    ");
    //        } else if (v.type() == typeid(std::function<void()>)) {
    //            printf("%s%s:function", subs.c_str(), k.c_str());
    //        } else if (v.type() == typeid(CObject*)) {
    //            printf("%s%s:userdata", subs.c_str(), k.c_str());
    //        } else if (v.type() == typeid(bool)) {
    //            printf("%s%s:%s", subs.c_str(), k.c_str(), std::any_cast<bool>(v) ? "true" : "false");
    //        } else {
    //            printf("%s%s:%s", subs.c_str(), k.c_str(), std::any_cast<std::string>(v).c_str());
    //        }
    //    }
    //}*/

    //// Function to store a table as a string
    //std::string store_table(const std::unordered_map<std::string, std::any>& table, const std::string& subs = "") {
    //    std::stringstream ss;
    //    ss << subs << "{";
    //    for (const auto& [k, v] : table) {
    //        if (v.type() == typeid(std::unordered_map<std::string, std::any>)) {
    //            ss << subs << k << " = ";
    //            ss << store_table(std::any_cast<std::unordered_map<std::string, std::any>>(v), subs + "    ");
    //        } else if (v.type() == typeid(std::function<void()>)) {
    //            ss << subs << k << " = \"func\",";
    //        } else if (v.type() == typeid(CObject*)) {
    //            ss << subs << k << " = \"userdata\",";
    //        } else if (v.type() == typeid(std::string)) {
    //            ss << subs << k << " = \"" << std::any_cast<std::string>(v) << "\",";
    //        } else {
    //            ss << subs << k << " = " << std::any_cast<std::string>(v) << ",";
    //        }
    //    }
    //    ss << subs << "},";
    //    return ss.str();
    //}

    //// -------------------------------------------------------------------------------------------

    //// Define classes for different object types
    //std::unordered_map<int, bool> monster_classes;
    //std::unordered_map<int, bool> stalker_classes;
    //std::unordered_map<int, bool> weapon_classes;
    //std::unordered_map<int, bool> artefact_classes;

    //// Function to check if an object is a monster
    //bool IsMonster(const void* object, int class_id) {
    //    int clsid = class_id != -1 ? class_id : get_clsid(object);
    //    return monster_classes.count(clsid) > 0;
    //}

    //// Function to check if an object is a stalker
    //bool IsStalker(const void* object, int class_id) {
    //    int clsid = class_id != -1 ? class_id : get_clsid(object);
    //    return stalker_classes.count(clsid) > 0;
    //}

    //// Function to check if an object is a weapon
    //bool isWeapon(const void* object, int class_id) {
    //    int clsid = class_id != -1 ? class_id : get_clsid(object);
    //    return weapon_classes.count(clsid) > 0;
    //}

    //// Function to check if an object is an artefact
    //bool isArtefact(const void* object, int class_id) {
    //    int clsid = class_id != -1 ? class_id : get_clsid(object);
    //    return artefact_classes.count(clsid) > 0;
    //}

    //// Function to get the community of an object
    //std::string get_object_community(const void* obj) {
    //    if (obj->id) {
    //        return character_community(obj);
    //    } else {
    //        return alife_character_community(obj);
    //    }
    //}

    //// Function to get the community of a character
    //std::string character_community(const void* obj) {
    //    if (IsStalker(obj)) {
    //        return obj->character_community();
    //    }
    //    return "monster";
    //}

    //// Function to get the community of an alife character
    //std::string alife_character_community(const void* obj) {
    //    if (IsStalker(obj, obj->clsid())) {
    //        return obj->community();
    //    }
    //    return "monster";
    //}

    //// Function to get a level object by story ID
    //void* level_object_by_sid(int sid) {
    //    auto sim = alife();
    //    if (sim) {
    //        auto se_obj = sim->story_object(sid);
    //        if (se_obj) {
    //            return level.object_by_id(se_obj->id);
    //        }
    //    }
    //    return nullptr;
    //}

    //// Function to get an object ID by story ID
    //int id_by_sid(int sid) {
    //    auto sim = alife();
    //    if (sim) {
    //        auto se_obj = sim->story_object(sid);
    //        if (se_obj) {
    //            return se_obj->id;
    //        }
    //    }
    //    return -1;
    //}

    //// Function to abort the game with an error message
    //void abort(const char* fmt, ...) {
    //    // Implementation for formatting and logging the error message
    //}

    //// Function to set the inactivate input time
    //void set_inactivate_input_time(float delta) {
    //    db.storage[db.actor->id()].disable_input_time = game.get_game_time();
    //    db.storage[db.actor->id()].disable_input_idle = delta;
    //    level.disable_input();
    //}

    //// Function to check if a number is odd
    //bool odd(float x) {
    //    return std::floor(x * 0.5) * 2 == std::floor(x);
    //}

    //// Function to check if an NPC is in the actor's frustum
    //bool npc_in_actor_frustrum(const void* npc) {
    //    auto actor_dir = device().cam_dir;
    //    auto npc_dir = npc->position() - db.actor->position();
    //    float yaw = yaw_degree3d(actor_dir, npc_dir);
    //    return yaw < 35;
    //}

    //// Containers for script callbacks
    //std::unordered_map<std::string, std::vector<std::function<void()>>> save_state_callbacks;
    //std::unordered_map<std::string, std::vector<std::function<void()>>> load_state_callbacks;

    //// Function to register a script callback
    //void RegisterScriptCallback(const std::string& name, const std::function<void()>& callback) {
    //    if (save_state_callbacks.count(name)) {
    //        save_state_callbacks[name].push_back(callback);
    //    }
    //    if (load_state_callbacks.count(name)) {
    //        load_state_callbacks[name].push_back(callback);
    //    }
    //}

    //// Function to unregister a script callback
    //void UnregisterScriptCallback(const std::string& name, const std::function<void()>& callback) {
    //    if (save_state_callbacks.count(name)) {
    //        auto& callbacks = save_state_callbacks[name];
    //        callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
    //    }
    //    if (load_state_callbacks.count(name)) {
    //        auto& callbacks = load_state_callbacks[name];
    //        callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
    //    }
    //}

    //// Function to send a script callback
    //void SendScriptCallback(const std::string& name) {
    //    if (save_state_callbacks.count(name)) {
    //        for (const auto& callback : save_state_callbacks[name]) {
    //            callback();
    //        }
    //    }
    //    if (load_state_callbacks.count(name)) {
    //        for (const auto& callback : load_state_callbacks[name]) {
    //            callback();
    //        }
    //    }
    //}

    //// Start game callback function
    //void start_game_callback() {
    //    auto aa = alife();

    //    std::unordered_map<std::string, std::string> squad_community_by_behaviour = {
    //        {"stalker", "stalker"},
    //        {"bandit", "bandit"},
    //        {"dolg", "dolg"},
    //        {"freedom", "freedom"},
    //        {"army", "army"},
    //        {"ecolog", "ecolog"},
    //        {"killer", "killer"},
    //        {"zombied", "zombied"},
    //        {"monolith", "monolith"},
    //        {"monster", "monster"},
    //        {"monster_predatory_day", "monster"},
    //        {"monster_predatory_night", "monster"},
    //        {"monster_vegetarian", "monster"},
    //        {"monster_zombied_day", "monster"},
    //        {"monster_zombied_night", "monster"},
    //        {"monster_special", "monster"}
    //    };

    //    monster_classes = {
    //        {clsid.bloodsucker_s, true},
    //        {clsid.boar_s, true},
    //        {clsid.dog_s, true},
    //        {clsid.flesh_s, true},
    //        {clsid.pseudodog_s, true},
    //        {clsid.burer_s, true},
    //        {clsid.chimera_s, true},
    //        {clsid.controller_s, true},
    //        {clsid.poltergeist_s, true},
    //        {clsid.gigant_s, true},
    //        {clsid.snork_s, true},
    //        {clsid.tushkano_s, true},
    //        {clsid.psy_dog_s, true},
    //        {clsid.psy_dog_phantom_s, true}
    //    };

    //    stalker_classes = {
    //        {clsid.script_actor, true},
    //        {clsid.script_stalker, true}
    //    };

    //    weapon_classes = {
    //        {clsid.wpn_vintorez_s, true},
    //        {clsid.wpn_ak74_s, true},
    //        {clsid.wpn_lr300_s, true},
    //        {clsid.wpn_hpsa_s, true},
    //        {clsid.wpn_pm_s, true},
    //        {clsid.wpn_shotgun_s, true},
    //        {clsid.wpn_auto_shotgun_s, true},
    //        {clsid.wpn_bm16_s, true},
    //        {clsid.wpn_svd_s, true},
    //        {clsid.wpn_svu_s, true},
    //        {clsid.wpn_rg6_s, true},
    //        {clsid.wpn_rpg7_s, true},
    //        {clsid.wpn_val_s, true},
    //        {clsid.wpn_walther_s, true},
    //        {clsid.wpn_usp45_s, true},
    //        {clsid.wpn_groza_s, true},
    //        {clsid.wpn_knife_s, true},
    //        {clsid.wpn_grenade_f1_s, true},
    //        {clsid.wpn_grenade_rgd5_s, true},
    //        {clsid.wpn_grenade_launcher, true},
    //        {clsid.wpn_grenade_fake, true}
    //    };

    //    artefact_classes = {
    //        {clsid.art_bast_artefact, true},
    //        {clsid.art_black_drops, true},
    //        {clsid.art_dummy, true},
    //        {clsid.art_electric_ball, true},
    //        {clsid.art_faded_ball, true},
    //        {clsid.art_galantine, true},
    //        {clsid.art_gravi, true},
    //        {clsid.art_gravi_black, true},
    //        {clsid.art_mercury_ball, true},
    //        {clsid.art_needles, true},
    //        {clsid.art_rusty_hair, true},
    //        {clsid.art_thorn, true},
    //        {clsid.art_zuda, true},
    //        {clsid.artefact, true},
    //        {clsid.artefact_s, true}
    //    };

    //    printf("start_game_callback called");
    //    smart_names.init_smart_names_table();
    //    task_manager.clear_task_manager();
    //    sound_theme.load_sound();
    //    xr_sound.start_game_callback();
    //    dialog_manager.fill_phrase_table();
    //    xr_s.init();
    //    sim_objects.clear();
    //    sim_board.clear();
    //    sr_light.clean_up();
    //    pda.add_quick_slot_items_on_game_start();
    //}



    //// Fatigue
    //void on_actor_critical_power() {
    //    // Implement function body
    //}

    //void on_actor_critical_max_power() {
    //    // Implement function body
    //}

    //// Bleeding
    //void on_actor_bleeding() {
    //    // Implement function body
    //}

    //void on_actor_satiety() {
    //    // Implement function body
    //}

    //// Radiation
    //void on_actor_radiation() {
    //    // Implement function body
    //}

    //// Weapon jammed
    //void on_actor_weapon_jammed() {
    //    // Implement function body
    //}

    //// Can't walk due to weight
    //void on_actor_cant_walk_weight() {
    //    // Implement function body
    //}

    //// Psy effect
    //void on_actor_psy() {
    //    // Implement function body
    //}

    //void give_info(const std::string& info) {
    //    db.actor->give_info_portion(info);
    //}

    //void disable_info(const std::string& info) {
    //    if (has_alife_info(info)) {
    //        printf("*INFO*: disabled npc='single_player' id='%s'", info.c_str());
    //        db.actor->disable_info_portion(info);
    //    }
    //}

    //std::vector<CObject*> create_ammo(const std::string& section, const Fvector& position, u16 lvi, u16 gvi, u16 pid, u32 num) {
    //    CIniFile* ini = pGameIni;

    //    u32 num_in_box = ini->r_u32(section, "box_size");
    //    std::vector<CObject*> t;
    //    while (num > num_in_box) {
    //        CObject* obj = Level().spawn_item(section, position, lvi, gvi, pid, num_in_box);
    //        t.push_back(obj);
    //        num -= num_in_box;
    //    }
    //    CObject* obj = Level().spawn_item(section, position, lvi, gvi, pid, num);
    //    t.push_back(obj);
    //    return t;
    //}

    //std::unordered_set<std::string> ammo_section = {
    //    "ammo_9x18_fmj", "ammo_9x18_pmm", "ammo_9x19_fmj", "ammo_9x19_pbp",
    //    "ammo_5.45x39_fmj", "ammo_5.45x39_ap", "ammo_5.56x45_ss190", "ammo_5.56x45_ap",
    //    "ammo_5.7x28_fmj", "ammo_5.7x28_ap", "ammo_7.62x54_7h1", "ammo_9x39_pab9",
    //    "ammo_gauss", "ammo_9x39_ap", "ammo_11.43x23_fmj", "ammo_11.43x23_hydro",
    //    "ammo_12x70_buck", "ammo_12x76_zhekan", "ammo_pkm_100"
    //};

    //std::unordered_set<std::string> quest_section;

    //// Convert string based on object value
    //std::string get_param_string(const std::string& src_string, CObject* obj) {
    //    std::string out_string = src_string;
    //    ALife::_STORY_ID script_ids = obj->ID_Parent;
    //    size_t num = std::regex_replace(out_string, std::regex("\\$script_id\\$"), std::to_string(script_ids));
    //    if (num > 0) {
    //        return out_string;
    //    } else {
    //        return src_string;
    //    }
    //}

    std::unordered_map<std::string, u32> save_markers;

    // Functions for checking save/load correctness
    void set_save_marker(NET_Packet* p, const std::string& mode, bool check, const std::string& prefix) {
        std::string result = "_" + prefix;

        if (check) {
            if (save_markers.find(result) == save_markers.end()) {
                auto errstr = make_string<const char*>("! Trying to check without marker %s", result.c_str());
                R_ASSERT2(false, errstr);
                return;
            }

            if (mode == "save") {
                u32 dif = p->w_tell() - save_markers[result];
                printf("%s: SAVE DIF: %d", result.c_str(), dif);
                if (dif >= 8000) {
                    printf("WARNING! may be this is problem save point");
                }
                if (dif >= 10240) {
                    //printf("You are saving too much");
                }
                p->w_u16(dif);
            } else {
                u32 c_dif = p->r_tell() - save_markers[result];
                u16 dif = p->r_u16();
                if (dif != c_dif) {
                    auto errstr = make_string<const char*>("! INCORRECT LOAD [%s].[%d][%d]", result.c_str(), dif, c_dif);
                    R_ASSERT2(false, errstr);
                    return;
                } else {
                    printf("%s: LOAD DIF: %d", result.c_str(), dif);
                }
            }
            return;
        }

        if (mode == "save") {
            u32 w_tell = p->w_tell();
            printf("%s: set save marker: %d", result.c_str(), w_tell);
            save_markers[result] = w_tell;
            if (w_tell > 16000) {
                R_ASSERT2(false, "You are saving too much");
                return;
            }
        } else {
            u32 r_tell = p->r_tell();
            printf("%s: set load marker: %d", result.c_str(), r_tell);
            save_markers[result] = r_tell;
        }
    }

    //// Convert vector to string
    //std::string vec_to_str(const Fvector& vector) {
    //    if (vector == Fvector().set(0, 0, 0)) {
    //        return "nil";
    //    }
    //    return std::string("[") + std::to_string(vector.x) + ":" + std::to_string(vector.y) + ":" + std::to_string(vector.z) + "]";
    //}

    //// Print call stack
    //void callstack() {
    //    if (IsDebuggerPresent()) {
    //        std::string callstack = print_stack_trace();
    //        Log(callstack.c_str());
    //    }
    //}

    //// Change team, squad, and group of an object
    //void change_team_squad_group(CObject* obj, int team, int squad, int group) {
    //    obj->set_squad_team_group(team, squad, group);
    //}

    //// Story ID
    //void add_story_object(u16 obj_id, ALife::_STORY_ID story_obj_id) {
    //    story_objects::get_story_objects_registry()->register_object(obj_id, story_obj_id);
    //}

    //CObject* get_story_object(ALife::_STORY_ID story_obj_id) {
    //    u16 obj_id = story_objects::get_story_objects_registry()->get_object_id(story_obj_id);
    //    if (obj_id == u16(-1)) {
    //        return nullptr;
    //    }
    //    return Level().Objects.net_Find(obj_id);
    //}

    const xr_string& get_object_story_id(u16 obj_id) {
        return story_objects::get_story_objects_registry()->get_story_id(obj_id);
    }

    u16 get_story_object_id(const xr_string& story_obj_id) {
        return story_objects::get_story_objects_registry()->get(story_obj_id);
    }

    void unregister_story_object_by_id(u16 obj_id) {
        story_objects::get_story_objects_registry()->unregister_by_id(obj_id);
    }

    void unregister_story_id(const xr_string& story_id) {
        story_objects::get_story_objects_registry()->unregister_by_story_id(story_id);
    }

    //// Get object squad
    //CObject* get_object_squad(CObject* object) {
    //    if (object == nullptr) {
    //        Msg("! You are trying to get squad_object from NIL object!!!");
    //        return nullptr;
    //    }

    //    u16 obj_id = object->ID();
    //    CSE_Abstract* se_obj = Level().Server->game->get_entity_from_eid(obj_id);
    //    if (se_obj && se_obj->ID_Parent != 0xffff) {
    //        return Level().Objects.net_Find(se_obj->ID_Parent);
    //    }
    //    return nullptr;
    //}

    //CObject* get_story_squad(ALife::_STORY_ID story_id) {
    //    u16 squad_id = get_story_object_id(story_id);
    //    return squad_id != u16(-1) ? Level().Objects.net_Find(squad_id) : nullptr;
    //}

    //// Check time interval
    //bool in_time_interval(u32 val1, u32 val2) {
    //    u32 game_hours = Level().GetGameHours();
    //    if (val1 >= val2) {
    //        return game_hours < val2 || game_hours >= val1;
    //    } else {
    //        return game_hours < val2 && game_hours >= val1;
    //    }
    //}

    //void show_all_ui(bool show) {
    //    if (show) {
    //        Level().ShowIndicators();
    //        Actor()->EnableHitMarks(true);
    //        HUD().ShowMessages();
    //    } else {
    //        if (Actor()->IsTalking()) {
    //            Actor()->StopTalk();
    //        }
    //        Level().HideIndicatorsExceptScope();
    //        HUD().HideActorMenu();
    //        HUD().HidePdaMenu();
    //        HUD().HideMessages();
    //        Actor()->EnableHitMarks(false);
    //    }
    //}

    //// LuaPandas
    //bool DebuggerMode = false;

    //void debug_jit_off() {
    //    if (DebuggerMode && msrDebugger) {
    //        msrDebugger->jit_off();
    //    }
    //}

    //void debug_jit_on() {
    //    if (DebuggerMode && msrDebugger) {
    //        msrDebugger->jit_on();
    //    }
    //}

    //void debugger_attach() {
    //    if (DebuggerMode) {
    //        debug_jit_off();
    //        msrDebugger->reConnect();
    //        debug_jit_on();
    //    } else {
    //        debug_jit_off();
    //        Log("LuaPanda starting...");
    //        msrDebugger->start("127.0.0.1", 8818);
    //        DebuggerMode = true;
    //        debug_jit_on();
    //    }
    //}





    //int main() {
    //    // Initialize random seed
    //    std::srand(std::time(nullptr));

    //    // Check for optional components
    //    bool isMarshal = false; // Assuming marshal is some external library or feature
    //    bool isLfs = false; // Assuming lfs is some external library or feature

    //    // Replace Lua string and math functions
    //    // string.gfind = string.gmatch; // Not applicable in C++
    //    // math.mod = math.fmod; // Not applicable in C++
    //    
    //    
    //    
    //    
    //    
    //    
    //    
    //    return 0;
    //}

}
