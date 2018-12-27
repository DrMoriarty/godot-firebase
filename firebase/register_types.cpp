/* register_types.cpp */

#include "register_types.h"
#include "core/class_db.h"
#include "firebase.h"
#include "analytics.h"
#include "remote_config.h"

void register_firebase_types() {
    ClassDB::register_class<Firebase>();
    ClassDB::register_class<FirebaseAnalytics>();
    ClassDB::register_class<FirebaseRemoteConfig>();
}

void unregister_firebase_types() {
   //nothing to do here
}
