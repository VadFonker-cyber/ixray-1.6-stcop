#pragma once

namespace scripts {
    namespace story_objects {

        class StoryObjectsRegistry {
        private:
            xr_hash_map<xr_string, int> id_by_story_id;
            xr_hash_map<int, xr_string> story_id_by_id;

        public:
            StoryObjectsRegistry();

            void                Register(int obj_id, const xr_string& story_obj_id, bool on_register); // renamed 'register'
            void                unregister_by_id(int obj_id);
            void                unregister_by_story_id(const xr_string& story_id);
            int                 get(const xr_string& story_obj_id);
            const xr_string&    get_story_id(int obj_id);
            void                save(NET_Packet* packet);
            void                load(NET_Packet* packet);
        };

        StoryObjectsRegistry*   get_story_objects_registry();

        void                    check_spawn_ini_for_story_id(CSE_Abstract* se_obj);

    }
}
