#ifndef FirebaseAuth_h
#define FirebaseAuth_h

#include "core/reference.h"
#include "firebase/auth.h"
#include "firebase.h"

class FirebaseAuth : public Reference {
    GDCLASS(FirebaseAuth, Reference);
    
    protected:
    static bool inited;
    static firebase::auth::Auth* auth;
    static firebase::auth::User::UserProfile profile;
    static void _bind_methods();

    public:
    
    FirebaseAuth();
    void sign_in_anonymously();
    void sign_in_facebook(String token);
    void unlink_facebook();
    bool is_logged_in();

    String user_name();
    String email();
    String uid();
    String photo_url();
    void sign_out();

    void OnCreateUserCallback(const firebase::Future<firebase::auth::User*>& result, void* user_data);
};

#endif // FirebaseAuth_h
