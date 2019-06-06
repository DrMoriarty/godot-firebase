#include "analytics.h"
#include "convertor.h"

bool FirebaseAnalytics::inited = false;

FirebaseAnalytics::FirebaseAnalytics() {
    if(!inited) {
        firebase::App* app = Firebase::AppId();
        if(app != NULL) {
            firebase::analytics::Initialize(*app);
            inited = true;
        }
    }
}

void FirebaseAnalytics::log_event(const String& event)
{
    std::string ev(event.utf8().get_data());
    firebase::analytics::LogEvent(ev.c_str());
}

void FirebaseAnalytics::log_int(const String& event, const String& parameter, int value)
{
    std::string ev(event.utf8().get_data());
    std::string par(parameter.utf8().get_data());
    firebase::analytics::LogEvent(ev.c_str(), par.c_str(), value);
}

void FirebaseAnalytics::log_long(const String& event, const String& parameter, int64_t value)
{
    std::string ev(event.utf8().get_data());
    std::string par(parameter.utf8().get_data());
    firebase::analytics::LogEvent(ev.c_str(), par.c_str(), value);
}

void FirebaseAnalytics::log_double(const String& event, const String& parameter, double value)
{
    std::string ev(event.utf8().get_data());
    std::string par(parameter.utf8().get_data());
    firebase::analytics::LogEvent(ev.c_str(), par.c_str(), value);
}

void FirebaseAnalytics::log_string(const String& event, const String& parameter, const String& value)
{
    std::string ev(event.utf8().get_data());
    std::string par(parameter.utf8().get_data());
    std::string val(value.utf8().get_data());
    firebase::analytics::LogEvent(ev.c_str(), par.c_str(), val.c_str());
}

void FirebaseAnalytics::log_params(const String& event, const Dictionary& params)
{
    std::string ev(event.utf8().get_data());
    firebase::analytics::Parameter pars[params.size()];
    for(int i=0; i<params.size(); i++) {
        Variant key = params.get_key_at_index(i);
        Variant val = params.get_value_at_index(i);
        std::string strKey;
        if(key.get_type() == Variant::STRING) {
            strKey = std::string(((String)key).utf8().get_data());
        } else {
            print_line(String("Unknown key type: ") + itos(val.get_type()));
            continue;
        }
        if(val.get_type() == Variant::INT)
            pars[i] = firebase::analytics::Parameter(strKey.c_str(), firebase::Variant((int)val));
        else if(val.get_type() == Variant::REAL)
            pars[i] = firebase::analytics::Parameter(strKey.c_str(), firebase::Variant((double)val));
        else if (val.get_type() == Variant::STRING)
            pars[i] = firebase::analytics::Parameter(strKey.c_str(), Convertor::toFirebaseVariant((String)val));
        else if(val.get_type() == Variant::BOOL)
            pars[i] = firebase::analytics::Parameter(strKey.c_str(), firebase::Variant((bool)val));
        else {
            pars[i] = firebase::analytics::Parameter(strKey.c_str(), firebase::Variant(false));
            print_line(String("Unknown variant type: ") + itos(val.get_type()));
        }
    }
    firebase::analytics::LogEvent(ev.c_str(), pars, params.size());
}

void FirebaseAnalytics::user_property(const String& name, const String& property)
{
    std::string n(name.utf8().get_data());
    std::string p(property.utf8().get_data());
    firebase::analytics::SetUserProperty(n.c_str(), p.c_str());
}

void FirebaseAnalytics::user_id(const String& user_id)
{
    std::string uid(user_id.utf8().get_data());
    firebase::analytics::SetUserId(uid.c_str());
}

void FirebaseAnalytics::screen_name(const String& screen, const String& screen_class)
{
    std::string sc(screen.utf8().get_data());
    std::string cl(screen_class.utf8().get_data());
    firebase::analytics::SetCurrentScreen(sc.c_str(), cl.c_str());
}

void FirebaseAnalytics::_bind_methods() {
    ClassDB::bind_method(D_METHOD("log_event", "event"), &FirebaseAnalytics::log_event);
    ClassDB::bind_method(D_METHOD("log_int", "event", "parameter", "value"), &FirebaseAnalytics::log_int);
    ClassDB::bind_method(D_METHOD("log_long", "event", "parameter", "value"), &FirebaseAnalytics::log_long);
    ClassDB::bind_method(D_METHOD("log_double", "event", "parameter", "value"), &FirebaseAnalytics::log_double);
    ClassDB::bind_method(D_METHOD("log_string", "event", "parameter", "value"), &FirebaseAnalytics::log_string);
    ClassDB::bind_method(D_METHOD("log_params", "event", "params"), &FirebaseAnalytics::log_params);
    ClassDB::bind_method(D_METHOD("user_property", "name", "property"), &FirebaseAnalytics::user_property);
    ClassDB::bind_method(D_METHOD("user_id", "user_id"), &FirebaseAnalytics::user_id);
    ClassDB::bind_method(D_METHOD("screen_name", "screen", "screen_class"), &FirebaseAnalytics::screen_name);
}
