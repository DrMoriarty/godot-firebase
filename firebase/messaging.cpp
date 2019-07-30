#include "messaging.h"

bool FirebaseMessaging::inited = false;
FirebaseMessagingListener *FirebaseMessaging::listener = NULL;

void FirebaseMessagingListener::OnMessage(const firebase::messaging::Message& message)
{
    print_line("FCM Message arrived");
    this->singleton->emit_signal("message");
}

void FirebaseMessagingListener::OnTokenReceived(const char* token)
{
    print_line(String("FCM Token: ") + token);
    this->singleton->emit_signal("token", String(token));
}

FirebaseMessaging::FirebaseMessaging() {
    if(!inited) {
        firebase::App* app = Firebase::AppId();
        if(app != NULL) {
            listener = new FirebaseMessagingListener();
            listener->singleton = this;
            firebase::messaging::Initialize(*app, listener);
            inited = true;
        }
    }
}

void FirebaseMessaging::_bind_methods() {
    ADD_SIGNAL(MethodInfo("token"));
    ADD_SIGNAL(MethodInfo("message"));
}
