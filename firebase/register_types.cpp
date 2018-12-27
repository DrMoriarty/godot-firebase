/* register_types.cpp */

#include "register_types.h"
#include "core/class_db.h"
#include "firebase.h"
#include "analytics.h"

void register_firebase_types() {
    ClassDB::register_class<Firebase>();
    ClassDB::register_class<FirebaseAnalytics>();
}

void unregister_firebase_types() {
   //nothing to do here
}
