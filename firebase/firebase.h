#ifndef Firebase_h
#define Firebase_h

#include "core/reference.h"
#include "firebase/app.h"

class Firebase : public Reference {
    GDCLASS(Firebase, Reference);
    
    protected:
    static firebase::App* app_ptr;
    static void _bind_methods();

    public:
    
    Firebase();
    static firebase::App* AppId();

};

#endif // Firebase_h
