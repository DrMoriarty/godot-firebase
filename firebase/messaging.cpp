#include "messaging.h"

bool FirebaseMessaging::inited = false;
FirebaseMessagingListener *FirebaseMessaging::listener = NULL;
String FirebaseMessaging::_token;

void FirebaseMessagingListener::OnMessage(const firebase::messaging::Message& message)
{
    print_line("FCM Message arrived");
    this->singleton->setMessage(message);
}

void FirebaseMessagingListener::OnTokenReceived(const char* token)
{
    String str;
    str += token;
    print_line(String("Get FCM Token: ") + str);
    this->singleton->setToken(str);
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

Variant FirebaseMessaging::token()
{
    if(_token.size() > 0) {
        return Variant(_token);
    } else {
        return Variant();
    }
}

void FirebaseMessaging::setToken(String token)
{
    _token = token;
    //this->emit_signal("token");
}

void FirebaseMessaging::setMessage(const firebase::messaging::Message& message)
{
    //this->emit_signal("message");
}

void FirebaseMessaging::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_token"), &FirebaseMessaging::token);
    ADD_SIGNAL(MethodInfo("token"));
    ADD_SIGNAL(MethodInfo("message"));
}
