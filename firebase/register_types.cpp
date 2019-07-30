/* register_types.cpp */

#include "register_types.h"
#include "core/class_db.h"
#include "firebase.h"
#include "analytics.h"
#include "remote_config.h"
#include "database.h"
#include "auth.h"
#include "admob.h"
#include "functions.h"
#include "messaging.h"

void register_firebase_types() {
    ClassDB::register_class<Firebase>();
    ClassDB::register_class<FirebaseAnalytics>();
    ClassDB::register_class<FirebaseRemoteConfig>();
    ClassDB::register_class<FirebaseDatabase>();
    ClassDB::register_class<FirebaseAuth>();
    ClassDB::register_class<FirebaseAdmob>();
    ClassDB::register_class<FirebaseFunctions>();
    ClassDB::register_class<FirebaseMessaging>();
}

void unregister_firebase_types() {
   //nothing to do here
}
