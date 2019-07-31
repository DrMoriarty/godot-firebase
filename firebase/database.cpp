#include "database.h"
#include "convertor.h"

bool FirebaseDatabase::inited = false;
firebase::database::Database *FirebaseDatabase::database = NULL;
firebase::database::DatabaseReference FirebaseDatabase::dbref;
FirebaseChildListener *FirebaseDatabase::listener = NULL;

////////////////////////////
//
// FirebaseChildListener
//

FirebaseChildListener::FirebaseChildListener(FirebaseDatabase* db): database(db)
{
}

void FirebaseChildListener::OnCancelled(const firebase::database::Error & error, const char *error_message)
{
    print_line(String("[RTDB] ChildListener: OnCancelled"));
}

void FirebaseChildListener::OnChildAdded(const firebase::database::DataSnapshot & snapshot, const char *previous_sibling_key)
{
    //print_line(String("[RTDB] ChildListener: OnChildAdded ") + snapshot.key());

    String key(snapshot.key());
    firebase::Variant val = snapshot.value();
    Variant value = Convertor::fromFirebaseVariant(val);
    database->emit_signal("child_added", key, value);
}

void FirebaseChildListener::OnChildChanged(const firebase::database::DataSnapshot & snapshot, const char *previous_sibling_key)
{
    //print_line(String("[RTDB] ChildListener: OnChildChanged ") + snapshot.key());

    String key(snapshot.key());
    firebase::Variant val = snapshot.value();
    Variant value = Convertor::fromFirebaseVariant(val);
    database->emit_signal("child_changed", key, value);
}

void FirebaseChildListener::OnChildMoved(const firebase::database::DataSnapshot & snapshot, const char *previous_sibling_key)
{
    //print_line(String("[RTDB] ChildListener: OnChildMoved ") + snapshot.key());

    String key(snapshot.key());
    firebase::Variant val = snapshot.value();
    Variant value = Convertor::fromFirebaseVariant(val);
    database->emit_signal("child_moved", key, value);
}

void FirebaseChildListener::OnChildRemoved(const firebase::database::DataSnapshot & snapshot)
{
    //print_line(String("[RTDB] ChildListener: OnChildRemoved ") + snapshot.key());

    String key(snapshot.key());
    firebase::Variant val = snapshot.value();
    Variant value = Convertor::fromFirebaseVariant(val);
    database->emit_signal("child_removed", key, value);
}

////////////////////////////
//
// FirebaseDatabase
//

FirebaseDatabase::FirebaseDatabase()
{
    if(!inited) {
        firebase::App* app = Firebase::AppId();
        if(app != NULL) {
            database = firebase::database::Database::GetInstance(app);
            database->set_persistence_enabled(true);
            dbref = database->GetReference();
            inited = true;
        }
    }
}

void FirebaseDatabase::SetDBRoot(const Array& keys)
{
    if(dbref.is_valid()) {
        dbref.RemoveAllChildListeners();
        dbref.RemoveAllValueListeners();
    }
    dbref = database->GetReference();
    dbref = GetReferenceToPath(keys);
    print_line(String("[RTDB] Set DB root: ")+dbref.key());
    if(listener == NULL) {
        listener = new FirebaseChildListener(this);
    }
    dbref.AddChildListener(listener);
}

firebase::database::DatabaseReference FirebaseDatabase::GetReferenceToPath(const Array& keys)
{
    firebase::database::DatabaseReference ref = dbref;
    for(int i=0; i<keys.size(); i++) {
        Variant key = keys[i];
        if(key.get_type() == Variant::STRING) {
            ref = ref.Child(((String)key).utf8().get_data());
        }
    }
    return ref;
}

void FirebaseDatabase::SetValue(const Array& keys, const Variant& value)
{
    firebase::database::DatabaseReference ref = GetReferenceToPath(keys);
    if(value.get_type() == Variant::INT)
        ref.SetValue(firebase::Variant((int)value));
    else if(value.get_type() == Variant::REAL)
        ref.SetValue(firebase::Variant((double)value));
    else
        ref.SetValue(Convertor::toFirebaseVariant((String)value));
}

String FirebaseDatabase::PushChild(const Array& keys)
{
    firebase::database::DatabaseReference ref = GetReferenceToPath(keys);
    ref = ref.PushChild();
    return String(ref.key());
}

void FirebaseDatabase::UpdateChildren(const Array& paths, const Dictionary& params)
{
    std::map<std::string, firebase::Variant> entryValues;
    for(int i=0; i<params.size(); i++) {
        Variant key = params.get_key_at_index(i);
        Variant val = params.get_value_at_index(i);
        std::string strKey = std::string(((String)key).utf8().get_data());
        if(val.get_type() == Variant::INT)
            entryValues[strKey] = firebase::Variant((int)val);
        else if(val.get_type() == Variant::REAL)
            entryValues[strKey] = firebase::Variant((double)val);
        else
            entryValues[strKey] = Convertor::toFirebaseVariant((String)val);
    }
    
    std::map<std::string, firebase::Variant> childUpdates;// = new std::map<std::string, firebase::Variant>();
    for(int i=0; i<paths.size(); i++) {
        Variant path = paths[i];
        if(path.get_type() == Variant::STRING) {
            std::string strPath = std::string(((String)path).utf8().get_data());
            childUpdates[strPath] = entryValues;
        }
    }
    dbref.UpdateChildren(childUpdates);
}

void FirebaseDatabase::RemoveValue(const Array& keys)
{
    firebase::database::DatabaseReference ref = GetReferenceToPath(keys);
    ref.RemoveValue();
}

void FirebaseDatabase::GetValue(const Array& keys)
{
    firebase::database::DatabaseReference ref = GetReferenceToPath(keys);
    firebase::Future<firebase::database::DataSnapshot> result = ref.GetValue();
    result.OnCompletion([](const firebase::Future<firebase::database::DataSnapshot>& result, void* user_data) {
                            ((FirebaseDatabase*)user_data)->OnGetValue(result, user_data);
                        }, this);
}

void FirebaseDatabase::OnGetValue(const firebase::Future<firebase::database::DataSnapshot>& result, void* user_data)
{
    assert(result.status() == firebase::kFutureStatusComplete);
    if (result.error() == firebase::database::kErrorNone) {
        String key(result.result()->key());
        firebase::Variant val = result.result()->value();
        Variant value = Convertor::fromFirebaseVariant(val);
        emit_signal("get_value", key, value);
    } else {
        print_line(String("[RTDB] Reading DB failed with error ") + result.error_message());
    }
}

/*
Variant FirebaseDatabase::ConvertVariant(const firebase::Variant& val)
{
    if(val.is_null()) {
        return Variant((void*)NULL);
    } else if(val.is_vector()) {
        const std::vector<firebase::Variant>& vector = val.vector();
        Vector<Variant> vecRes;
        for(int i=0; i<vector.size(); i++) {
            vecRes.push_back(ConvertVariant(vector[i]));
        }
        return Variant(vecRes);
    } else if(val.is_map()) {
        const std::map<firebase::Variant, firebase::Variant>& map = val.map();
        Dictionary dictRes;
        for(std::map<firebase::Variant, firebase::Variant>::const_iterator i=map.begin(); i!=map.end(); i++) {
            firebase::Variant first = i->first;
            firebase::Variant second = i->second;
            String key = String(first.string_value());
            dictRes[key] = ConvertVariant(second);
        }
        return Variant(dictRes);
    } else if(val.is_int64()) {
        return Variant(val.int64_value());
    } else if(val.is_double()) {
        return Variant(val.double_value());
    } else if(val.is_bool()) {
        return Variant(val.bool_value());
    } else if(val.is_string()) {
        return Variant(val.string_value());
    } else {
        return Variant((void*)NULL);
    }
}
*/

void FirebaseDatabase::_bind_methods() {

    ClassDB::bind_method(D_METHOD("set_db_root", "keys"), &FirebaseDatabase::SetDBRoot);
    ClassDB::bind_method(D_METHOD("set_value", "keys", "value"), &FirebaseDatabase::SetValue);
    ClassDB::bind_method(D_METHOD("push_child", "keys"), &FirebaseDatabase::PushChild);
    ClassDB::bind_method(D_METHOD("update_children", "paths", "params"), &FirebaseDatabase::UpdateChildren);
    ClassDB::bind_method(D_METHOD("remove_value", "keys"), &FirebaseDatabase::RemoveValue);
    ClassDB::bind_method(D_METHOD("get_value", "keys"), &FirebaseDatabase::GetValue);

    ADD_SIGNAL(MethodInfo("get_value"));
    ADD_SIGNAL(MethodInfo("child_added"));
    ADD_SIGNAL(MethodInfo("child_changed"));
    ADD_SIGNAL(MethodInfo("child_moved"));
    ADD_SIGNAL(MethodInfo("child_removed"));
}
