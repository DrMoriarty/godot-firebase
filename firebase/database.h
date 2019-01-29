#ifndef FirebaseDatabase_h
#define FirebaseDatabase_h

#include "core/reference.h"
#include "firebase/database.h"
#include "firebase.h"

class FirebaseDatabase : public Reference {
    GDCLASS(FirebaseDatabase, Reference);
    
    protected:
    static bool inited;
    static firebase::database::Database *database;
    static firebase::database::DatabaseReference dbref;
    static void _bind_methods();

    Variant ConvertVariant(const firebase::Variant& val);
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
