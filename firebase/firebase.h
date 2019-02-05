#ifndef Firebase_h
#define Firebase_h

#include "core/reference.h"
#include "firebase/app.h"

#if defined(__ANDROID__)
/// An Android Activity from Java.
typedef jobject AppActivity;
#elif defined(TARGET_OS_IPHONE)
/// A pointer to an iOS UIView.
typedef id AppActivity;
#else
/// A void pointer for stub classes.
typedef void *AppActivity;
#endif  // __ANDROID__, TARGET_OS_IPHONE


class Firebase : public Reference {
    GDCLASS(Firebase, Reference);
    
    protected:
    static firebase::App* app_ptr;
    static void _bind_methods();
    static void createApplication();

    public:
    
    Firebase();
    static firebase::App* AppId();
    static AppActivity GetAppActivity();

};

#endif // Firebase_h
