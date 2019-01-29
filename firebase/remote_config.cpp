#include "remote_config.h"

bool FirebaseRemoteConfig::inited = false;
bool FirebaseRemoteConfig::data_loaded = false;

FirebaseRemoteConfig::FirebaseRemoteConfig() {
    if(!inited) {
        firebase::App* app = Firebase::AppId();
        if(app != NULL) {
            if(firebase::remote_config::Initialize(*app) == firebase::kInitResultSuccess) {
                firebase::remote_config::Fetch().OnCompletion([](const firebase::Future<void>& completed_future, void* user_data) {
                                                                  FirebaseRemoteConfig *frc = (FirebaseRemoteConfig*)user_data;
                                                                  if(firebase::remote_config::ActivateFetched()) {
                                                                      // activate preloaded configs
                                                                      print_line("Fetched remote config data activated");
                                                                      data_loaded = true;
                                                                      frc->emit_signal("loaded");
                                                                  } else {
                                                                      print_line("Fetched remote config data was not activated");
                                                                  }
                                                              }, this);
                inited = true;
            }
        }
    }
}

void FirebaseRemoteConfig::set_defaults(const Dictionary& params)
{
    firebase::remote_config::ConfigKeyValueVariant pars[params.size()];
    for(int i=0; i<params.size(); i++) {
        Variant key = params.get_key_at_index(i);
        Variant val = params.get_value_at_index(i);
        firebase::remote_config::ConfigKeyValueVariant ckv;
        ckv.key = ((String)key).utf8().ptr();
        if(val.get_type() == Variant::INT)
            ckv.value = firebase::Variant((int)val);
        else if(val.get_type() == Variant::REAL)
            ckv.value = firebase::Variant((double)val);
        else
            ckv.value = firebase::Variant(((String)val).utf8().ptr());
        pars[i] = ckv;
    }
    firebase::remote_config::SetDefaults(pars, params.size());
}

bool FirebaseRemoteConfig::get_boolean(const String& param)
{
    return firebase::remote_config::GetBoolean(param.utf8().ptr());
}

double FirebaseRemoteConfig::get_double(const String& param)
{
    return firebase::remote_config::GetDouble(param.utf8().ptr());
}

int64_t FirebaseRemoteConfig::get_int(const String& param)
{
    return firebase::remote_config::GetLong(param.utf8().ptr());
}

String FirebaseRemoteConfig::get_string(const String& param)
{
    return String(firebase::remote_config::GetString(param.utf8().ptr()).c_str());
}

bool FirebaseRemoteConfig::loaded()
{
    return data_loaded;
}

void FirebaseRemoteConfig::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_defaults", "params"), &FirebaseRemoteConfig::set_defaults);
    ClassDB::bind_method(D_METHOD("get_boolean", "param"), &FirebaseRemoteConfig::get_boolean);
    ClassDB::bind_method(D_METHOD("get_double", "param"), &FirebaseRemoteConfig::get_double);
    ClassDB::bind_method(D_METHOD("get_int", "param"), &FirebaseRemoteConfig::get_int);
    ClassDB::bind_method(D_METHOD("get_string", "param"), &FirebaseRemoteConfig::get_string);
    ClassDB::bind_method(D_METHOD("loaded"), &FirebaseRemoteConfig::loaded);
    ADD_SIGNAL(MethodInfo("loaded"));
}
