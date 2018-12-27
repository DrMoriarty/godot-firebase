#ifndef FirebaseAnalytics_h
#define FirebaseAnalytics_h

#include "core/reference.h"
#include "firebase/analytics.h"
#include "firebase.h"

class FirebaseAnalytics : public Reference {
    GDCLASS(FirebaseAnalytics, Reference);
    
    protected:
    static bool inited;
    static void _bind_methods();

    public:
    
    FirebaseAnalytics();
    void log_event(const String& event);
    void log_int(const String& event, const String& parameter, int value);
    void log_long(const String& event, const String& parameter, int64_t value);
    void log_double(const String& event, const String& parameter, double value);
    void log_string(const String& event, const String& parameter, const String& value);
    void log_params(const String& event, const Dictionary& params);
    void user_property(const String& name, const String& property);
    void user_id(const String& user_id);
    void screen_name(const String& screen, const String& screen_class);

};

#endif // FirebaseAnalytics_h
