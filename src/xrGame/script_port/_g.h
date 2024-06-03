#pragma once

namespace scripts {

    // Some X-Ray block
    extern bool DebugLog;
    extern bool DebuggerMode;

    extern void log(const std::string& str);
    extern void printf(const char* fmt, ...);



    using entity_action = CScriptEntityAction;
    using look          = CScriptWatchAction;
    using anim          = CScriptAnimationAction;
    using sound         = CScriptSoundAction;
    using particle      = CScriptParticleAction;
    using object        = CScriptObjectAction;
    using cond          = CScriptActionCondition;
    using act           = CScriptMonsterAction;

    struct Action_Object {
        virtual void command(entity_action* act, bool unknown) = 0;
    };

    template <typename T>
    void action_internal(entity_action& act, T action) {
        if (action == nullptr) 
            return;
        act.SetAction(action);
    }

    template <typename T, typename... Types>
    void action_internal(entity_action& act, T action, Types... actions) {
        if (action == nullptr) 
            return;
        act.SetAction(action);
        action_internal(act, actions...);
    }

    template <typename T, typename... Types>
    entity_action& action(Action_Object* obj, Types... actions) {
        entity_action act;
        action_internal(act, actions...);

        if (obj) 
            obj->command(act, false);

        return act;
    }

    template <typename T, typename... Types>
    entity_action& action_first(Action_Object* obj, Types... actions) {
        entity_action act;
        action_internal(act, actions...);

        if (obj) 
            obj->command(act, true);

        return act;
    }


    extern void print_table(const xr_hash_map<xr_string, int>& table, const std::string& subs = "");

    extern void set_save_marker(NET_Packet* p, const std::string& mode, bool check, const std::string& prefix);

    extern const xr_string& get_object_story_id(u16 obj_id);
    extern u16              get_story_object_id(const xr_string& story_obj_id);
    extern void             unregister_story_object_by_id(u16 obj_id);
    extern void             unregister_story_id(const xr_string& story_id);

}