#include "_script_port_headers.h"

namespace scripts {
    namespace story_objects {
        //------------------------------------------------------------------------------------------------------------
        StoryObjectsRegistry::StoryObjectsRegistry() {}

        void StoryObjectsRegistry::Register(int obj_id, const xr_string& story_obj_id, bool on_register) {
            auto it = id_by_story_id.find(story_obj_id);
            if (it != id_by_story_id.end()) {
                if (obj_id != it->second) {
                    auto alife = ai().get_alife();
                    auto& alife_objects = alife->objects();
                    auto  exist_obj = alife_objects.object(it->second, true);
                    xr_string exist_obj_name = exist_obj->name();
                    auto  adding_obj = alife_objects.object(obj_id);
                    xr_string adding_obj_name = adding_obj->name();
                    auto errstr = make_string<const char*>("You are trying to spawn two or more objects with the same story_id: [%s] --> [%s] try to add:[%s]", story_obj_id.c_str(), exist_obj_name.c_str(), adding_obj_name.c_str());
                    R_ASSERT2(false, errstr);
                }
            }
            else {
                auto find = story_id_by_id.find(obj_id);
                if (find != story_id_by_id.end() && find->second != story_obj_id) {
                    auto errstr = make_string<const char*>("Object [%d] is already in story_objects_registry with story_id[%s]", obj_id, story_obj_id.c_str());
                    R_ASSERT2(false, errstr);
                }
            }
            id_by_story_id[story_obj_id] = obj_id;
            story_id_by_id[obj_id] = story_obj_id;
        }

        void StoryObjectsRegistry::unregister_by_id(int obj_id) {
            auto it = story_id_by_id.find(obj_id);
            if (it != story_id_by_id.end()) {
                id_by_story_id.erase(it->second);
                story_id_by_id.erase(obj_id);
            }
        }

        void StoryObjectsRegistry::unregister_by_story_id(const xr_string& story_id) {
            auto it = id_by_story_id.find(story_id);
            if (it != id_by_story_id.end()) {
                story_id_by_id.erase(it->second);
                id_by_story_id.erase(story_id);
            }
        }

        int StoryObjectsRegistry::get(const xr_string& story_obj_id) {
            auto it = id_by_story_id.find(story_obj_id);
            if (it == id_by_story_id.end()) {
                auto errstr = make_string<const char*>("Story_obj_id [%s] not found in id_by_story_id", story_obj_id.c_str());
                R_ASSERT2(false, errstr);
            }
            return it->second;
        }

        const xr_string& StoryObjectsRegistry::get_story_id(int obj_id) {
            auto it = story_id_by_id.find(obj_id);
            if (it == story_id_by_id.end()) {
                auto errstr = make_string<const char*>("obj_id [%d] not found in story_id_by_id", obj_id);
                R_ASSERT2(false, errstr);
            }
            return it->second;
        }

        void StoryObjectsRegistry::save(NET_Packet* packet) {
            set_save_marker(packet, "save", false, "story_objects");
            size_t count = id_by_story_id.size();
            if (count > 65534) {
                print_table(id_by_story_id, "");
                R_ASSERT2(false, "There is too many story_ids!!!");
            }
            packet->w_u16(count);
            for (const auto& pair : id_by_story_id) {
                packet->w_stringZ(pair.first.c_str());
                packet->w_u16(pair.second);
            }
            set_save_marker(packet, "save", true, "story_objects");
        }

        void StoryObjectsRegistry::load(NET_Packet* packet) {
            set_save_marker(packet, "load", false, "story_objects");
            int count = packet->r_u16();
            for (int i = 0; i < count; ++i) {
                xr_string story_id;
                packet->r_stringZ(story_id);

                int obj_id = packet->r_u16();
                id_by_story_id[story_id] = obj_id;
                story_id_by_id[obj_id] = story_id;
            }
            set_save_marker(packet, "load", true, "story_objects");
        }

        //------------------------------------------------------------------------------------------------------------

        StoryObjectsRegistry* get_story_objects_registry() {
            static StoryObjectsRegistry* story_obj_registry = nullptr;
            if (story_obj_registry == nullptr) {
                story_obj_registry = new StoryObjectsRegistry();
            }
            return story_obj_registry;
        }

        void check_spawn_ini_for_story_id(CSE_Abstract* se_obj) {
            CInifile& spawn_ini = se_obj->spawn_ini();
            if (spawn_ini.section_exist("story_object")) {

                xr_string id = "", value= "";
                {
                    LPCSTR	n, v;
                    if (spawn_ini.r_line("story_object", 0, &n, &v)) {
                        id = n;
                        if (v) 
                            value = v;
                    }
                }

                if (id != "story_id") {
                    auto errstr = make_string<const char*>("There is no 'story_id' field in [story_object] section :object [%s]", se_obj->name_replace());
                    R_ASSERT2(false, errstr);
                }

                if (value.empty()) {
                    auto errstr = make_string<const char*>("Field 'story_id' in [story_object] section got no value :object [%s]", se_obj->name_replace());
                    R_ASSERT2(false, errstr);
                }

                get_story_objects_registry()->Register(se_obj->ID, value, true);

                return;
            }
            xr_string spawn_sect = se_obj->name();
            xr_string story_id = utils::cfg_get_string(pSettings, spawn_sect, "story_id", nullptr, false, "", "");
            if (!story_id.empty()) {
                get_story_objects_registry()->Register(se_obj->ID, story_id, true);
            }
        }
    }
}