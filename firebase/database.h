#ifndef FirebaseDatabase_h
#define FirebaseDatabase_h

#include "core/reference.h"
#include "firebase/database.h"
#include "firebase.h"

class FirebaseDatabase;

class FirebaseChildListener : public firebase::database::ChildListener {
private:
    FirebaseDatabase *database;
public:
    FirebaseChildListener(FirebaseDatabase *db);
    void OnCancelled(const firebase::database::Error & error, const char *error_message);
    void OnChildAdded(const firebase::database::DataSnapshot & snapshot, const char *previous_sibling_key);
    void OnChildChanged(const firebase::database::DataSnapshot & snapshot, const char *previous_sibling_key);
    void OnChildMoved(const firebase::database::DataSnapshot & snapshot, const char *previous_sibling_key);
    void OnChildRemoved(const firebase::database::DataSnapshot & snapshot);
};

class FirebaseDatabase : public Reference {
    GDCLASS(FirebaseDatabase, Reference);
    
    protected:
    static bool inited;
    static firebase::database::Database *database;
    static firebase::database::DatabaseReference dbref;
    static FirebaseChildListener *listener;
    static void _bind_methods();

    //Variant ConvertVariant(const firebase::Variant& val);
    firebase::database::DatabaseReference GetReferenceToPath(const Array& keys);

    public:
    
    FirebaseDatabase();
    void SetDBRoot(const Array& keys);
    void SetValue(const Array& keys, const Variant& value);
    String PushChild(const Array& keys);
    void UpdateChildren(const Array& paths, const Dictionary& params);
    void RemoveValue(const Array& keys);
    void GetValue(const Array& keys);
    void OnGetValue(const firebase::Future<firebase::database::DataSnapshot>& result, void* user_data);
};

#endif // FirebaseDatabase_h
