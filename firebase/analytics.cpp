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
    firebase::analytics::LogEvent(event.utf8().ptr());
}

void FirebaseAnalytics::log_int(const String& event, const String& parameter, int value)
{
    firebase::analytics::LogEvent(event.utf8().ptr(), parameter.utf8().ptr(), value);
}

void FirebaseAnalytics::log_long(const String& event, const String& parameter, int64_t value)
{
    firebase::analytics::LogEvent(event.utf8().ptr(), parameter.utf8().ptr(), value);
}

void FirebaseAnalytics::log_double(const String& event, const String& parameter, double value)
{
    firebase::analytics::LogEvent(event.utf8().ptr(), parameter.utf8().ptr(), value);
}

void FirebaseAnalytics::log_string(const String& event, const String& parameter, const String& value)
{
    firebase::analytics::LogEvent(event.utf8().ptr(), parameter.utf8().ptr(), value.utf8().ptr());
}

void FirebaseAnalytics::log_params(const String& event, const Dictionary& params)
{
    firebase::analytics::Parameter pars[params.size()];
    for(int i=0; i<params.size(); i++) {
        Variant key = params.get_key_at_index(i);
        Variant val = params.get_value_at_index(i);
        if(val.get_type() == Variant::INT)
            pars[i] = firebase::analytics::Parameter(((String)key).utf8().ptr(), firebase::Variant((int)val));
        else if(val.get_type() == Variant::REAL)
            pars[i] = firebase::analytics::Parameter(((String)key).utf8().ptr(), firebase::Variant((double)val));
        else if (val.get_type() == Variant::STRING)
            pars[i] = firebase::analytics::Parameter(((String)key).utf8().ptr(), Convertor::toFirebaseVariant((String)val));
        else if(val.get_type() == Variant::BOOL)
            pars[i] = firebase::analytics::Parameter(((String)key).utf8().ptr(), firebase::Variant((bool)val));
        else {
            pars[i] = firebase::analytics::Parameter(((String)key).utf8().ptr(), firebase::Variant(false));
            print_line(String("Unknown variant type: ") + itos(val.get_type()));
        }
    }
    firebase::analytics::LogEvent(event.utf8().ptr(), pars, params.size());
}

void FirebaseAnalytics::user_property(const String& name, const String& property)
{
    firebase::analytics::SetUserProperty(name.utf8().ptr(), property.utf8().ptr());
}

void FirebaseAnalytics::user_id(const String& user_id)
{
    firebase::analytics::SetUserId(user_id.utf8().ptr());
}

void FirebaseAnalytics::screen_name(const String& screen, const String& screen_class)
{
    firebase::analytics::SetCurrentScreen(screen.utf8().ptr(), screen_class.utf8().ptr());
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
