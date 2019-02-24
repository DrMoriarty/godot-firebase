#ifndef FirebaseAdmob_h
#define FirebaseAdmob_h

#include "core/reference.h"
#include "firebase/admob.h"
#include "firebase/admob/banner_view.h"
#include "firebase/admob/interstitial_ad.h"
#include "firebase.h"

class FirebaseAdmob : public Reference {
    GDCLASS(FirebaseAdmob, Reference);
    
    protected:

    ///////////////////////////////////////
    //  Banner

    class MyBannerViewListener : public firebase::admob::BannerView::Listener {
        FirebaseAdmob *controller;
    public:
        MyBannerViewListener(FirebaseAdmob *c) {
            controller = c;
        }

        void OnPresentationStateChanged(firebase::admob::BannerView* banner_view, firebase::admob::BannerView::PresentationState state) override {
            // send signal with int state
            controller->emit_signal("banner_state", state);
        }

        void OnBoundingBoxChanged(firebase::admob::BannerView* banner_view, firebase::admob::BoundingBox box) override {
        }

        ~MyBannerViewListener() {
        }
    };

    typedef struct BannerSettings {
        firebase::admob::BannerView *bannerView;
        FirebaseAdmob *controller;
        BannerSettings(firebase::admob::BannerView *ad, FirebaseAdmob* c) {
            bannerView = ad;
            controller = c;
        };
    } BannerSettings;

    static void BannerLoadCallback(const firebase::Future<void>& future, void* user_data);
    static void BannerInitCallback(const firebase::Future<void>& future, void* user_data);
    static void BannerHideCallback(const firebase::Future<void>& future, void* user_data);


    ///////////////////////////////////////
    //  Interstitial Ad

    class MyInterstitialAdListener: public firebase::admob::InterstitialAd::Listener {
        FirebaseAdmob *controller;
    public:
        MyInterstitialAdListener(FirebaseAdmob *c) {
            controller = c;
        };

        void OnPresentationStateChanged(firebase::admob::InterstitialAd* interstitialAd, firebase::admob::InterstitialAd::PresentationState state) override {
            // send signal with int state value
            controller->emit_signal("interstitial_state", state);
        }
    
        ~MyInterstitialAdListener() {
        }
    };

    typedef struct InterstitialSettings {
        firebase::admob::InterstitialAd *interstitial_ad;
        FirebaseAdmob *controller;
        InterstitialSettings(firebase::admob::InterstitialAd *ad, FirebaseAdmob* c) {
            interstitial_ad = ad;
            controller = c;
        };
    } InterstitialSettings;

    static void InterstitialLoadCallback(const firebase::Future<void>& future, void* user_data);
    static void InterstitialInitCallback(const firebase::Future<void>& future, void* user_data);

    ///////////////////////////////////////
    //  Rewarded Ad
    class MyRewardedVideoListener: public firebase::admob::rewarded_video::Listener {
        FirebaseAdmob *controller;
    public:
        MyRewardedVideoListener(FirebaseAdmob *c) {
            controller = c;
        };

        void OnRewarded(firebase::admob::rewarded_video::RewardItem item) override {
            // send signal on rewarded
            controller->emit_signal("rewarded_completed");
        }

        void OnPresentationStateChanged(firebase::admob::rewarded_video::PresentationState state) override {
            // send signal with int state
            controller->emit_signal("rewarded_state", state);
        }
    
        ~MyRewardedVideoListener() {
        }
    };

    typedef struct RewardedSettings {
        std::string adId;
        FirebaseAdmob *controller;
        RewardedSettings(std::string& _adId, FirebaseAdmob *c) {
            adId = _adId;
            controller = c;
        };
    } RewardedSettings;

    static void RewardedLoadedCallback(const firebase::Future<void>& future, void* user_data);
    static void RewardedInitCallback(const firebase::Future<void>& future, void* user_data);



    static bool inited;
    static std::string ApplicationId;
    static std::vector<std::string> testDeviceIds;
    static const char* testingDevices[100];
    static firebase::admob::AdRequest my_ad_request;
    static firebase::admob::BannerView *sharedBannerView;
    static firebase::admob::InterstitialAd *sharedInterstitialAd;
    static int rewarded_inited;

    static void _bind_methods();
    firebase::admob::AdParent getAdParent();
    public:
    
    FirebaseAdmob();
    void Init(String advertisingId);
    void AddTestDevice(String deviceId);

    void LoadBanner(String bannerId);
    bool IsBannerLoaded();
    void ShowBanner();
    void CloseBanner();

    void LoadInterstitial(String bannerId);
    bool IsInterstitialLoaded();
    void ShowInterstitial();

    void LoadRewarded(String bannerId);
    bool IsRewardedLoaded();
    void ShowRewarded();

};

#endif // FirebaseAdmob_h
