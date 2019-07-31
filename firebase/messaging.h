#ifndef FirebaseMessaging_h
#define FirebaseMessaging_h

#include "core/reference.h"
#include "firebase/messaging.h"
#include "firebase.h"

class FirebaseMessaging;

class FirebaseMessagingListener : public firebase::messaging::Listener {
    public:
    FirebaseMessaging *singleton;
    void OnMessage(const firebase::messaging::Message& message);
    void OnTokenReceived(const char* token);
};

class FirebaseMessaging : public Reference {
    GDCLASS(FirebaseMessaging, Reference);
    
    protected:
    static bool inited;
    static void _bind_methods();
    static FirebaseMessagingListener* listener;
    static String _token;

    public:
    
    FirebaseMessaging();
    Variant token();
    void setToken(String token);
    void setMessage(const firebase::messaging::Message& message);
};

#endif // FirebaseMessaging_h
