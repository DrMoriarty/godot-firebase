#include "admob.h"

///////////////////////////////////////
//
//  Banner
//
///////////////////////////////////////

void FirebaseAdmob::BannerLoadCallback(const firebase::Future<void>& future, void* user_data) {
    BannerSettings *settings = static_cast<BannerSettings*>(user_data);
    //send signal with banner load status
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        print_line("Banner load complete");
        settings->controller->emit_signal("banner_loading_result", true);
    } else {
        print_line(String("Banner load error: ") + itos(future.error()));
        settings->controller->emit_signal("banner_loading_result", false);
    }
}

void FirebaseAdmob::BannerInitCallback(const firebase::Future<void>& future, void* user_data) {
    BannerSettings *settings = static_cast<BannerSettings*>(user_data);
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        print_line("Banner init complete");
        settings->bannerView->LoadAd(my_ad_request);
        settings->bannerView->LoadAdLastResult().OnCompletion(FirebaseAdmob::BannerLoadCallback, settings);
    } else {
        // send signal with banner init failed status
        print_line(String("Banner init error: ") + itos(future.error()));
        settings->controller->emit_signal("banner_loading_result", false);
    }
}

void FirebaseAdmob::BannerHideCallback(const firebase::Future<void>& future, void* user_data) {
    firebase::admob::BannerView *bannerView = static_cast<firebase::admob::BannerView*>(user_data);
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        print_line("Banner hide complete");
        bannerView->Destroy();
    } else {
        print_line(String("Banner hide error: ") + itos(future.error()));
    }
}

///////////////////////////////////////
//
//  Interstitial Ad
//
///////////////////////////////////////


void FirebaseAdmob::InterstitialLoadCallback(const firebase::Future<void>& future, void* user_data) {
    InterstitialSettings *settings = static_cast<InterstitialSettings*>(user_data);
    // send signal with loading status
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        print_line("Interstitial load complete");
        settings->controller->emit_signal("interstitial_loading_result", true);
    } else {
        print_line(String("Interstitial load error: ") + itos(future.error()));
        settings->controller->emit_signal("interstitial_loading_result", false);
    }
}

void FirebaseAdmob::InterstitialInitCallback(const firebase::Future<void>& future, void* user_data) {
    InterstitialSettings *settings = static_cast<InterstitialSettings*>(user_data);
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        settings->interstitial_ad->LoadAd(my_ad_request);
        settings->interstitial_ad->LoadAdLastResult().OnCompletion(InterstitialLoadCallback, settings);
        print_line("Interstitial init complete");
    } else {
        print_line(String("Interstitial init error: ") + itos(future.error()));
        // send signal with init status
        settings->controller->emit_signal("interstitial_loading_result", false);
    }
}

///////////////////////////////////////
//
//  Rewarded Ad
//
///////////////////////////////////////

void FirebaseAdmob::RewardedLoadedCallback(const firebase::Future<void>& future, void* user_data) {
    RewardedSettings *settings = static_cast<RewardedSettings*>(user_data);
    // send signal with loading status
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        print_line("Rewarded load complete");
        settings->controller->emit_signal("rewarded_loading_result", true);
    } else {
        print_line(String("Rewarded load error: ") + itos(future.error()));
        settings->controller->emit_signal("rewarded_loading_result", false);
    }
}

void FirebaseAdmob::RewardedInitCallback(const firebase::Future<void>& future, void* user_data) {
    RewardedSettings *settings = static_cast<RewardedSettings*>(user_data);
    if (future.error() == firebase::admob::kAdMobErrorNone) {
        rewarded_inited = true;
        firebase::admob::rewarded_video::LoadAd(settings->adId.c_str(), my_ad_request);
        firebase::admob::rewarded_video::LoadAdLastResult().OnCompletion(FirebaseAdmob::RewardedLoadedCallback, settings);
        print_line("Rewarded init complete");
    } else {
        print_line(String("Rewarded init error: ") + itos(future.error()));
        // send signal with inited error
        settings->controller->emit_signal("rewarded_loading_result", false);
    }
}

///////////////////////////////////////
//
//  FirebaseAdmob
//
///////////////////////////////////////

bool FirebaseAdmob::inited = false;
std::string FirebaseAdmob::ApplicationId;
std::vector<std::string> FirebaseAdmob::testDeviceIds;
const char* FirebaseAdmob::testingDevices[100];
firebase::admob::AdRequest FirebaseAdmob::my_ad_request = {};
firebase::admob::BannerView* FirebaseAdmob::sharedBannerView = NULL;
firebase::admob::InterstitialAd* FirebaseAdmob::sharedInterstitialAd = NULL;
bool FirebaseAdmob::rewarded_inited = false;

FirebaseAdmob::FirebaseAdmob() {
}

void FirebaseAdmob::Init(String advertisingId) {
    if(!inited) {
        firebase::App* app = Firebase::AppId();
        if(app != NULL) {
            ApplicationId = std::string(advertisingId.utf8().ptr());
            firebase::admob::Initialize(*app, advertisingId.utf8().ptr());
            inited = true;
        }
    }
}

firebase::admob::AdParent FirebaseAdmob::getAdParent() {
    return Firebase::GetAppActivity();
}

void FirebaseAdmob::AddTestDevice(String deviceId) {
    std::string did(deviceId.utf8().ptr());
    testDeviceIds.push_back(did);

    my_ad_request.test_device_id_count = testDeviceIds.size();
    for(int i=0; i<testDeviceIds.size(); i++) {
        testingDevices[i] = testDeviceIds[i].c_str();
    }
    my_ad_request.test_device_ids = testingDevices;
}

void FirebaseAdmob::LoadBanner(String bannerId) {
    firebase::admob::AdSize ad_size;
    ad_size.ad_size_type = firebase::admob::kAdSizeStandard;
    ad_size.width = 320;
    ad_size.height = 50;
    sharedBannerView = new firebase::admob::BannerView();
    BannerSettings *settings = new BannerSettings(sharedBannerView, this);
    sharedBannerView->Initialize(getAdParent(), bannerId.utf8().ptr(), ad_size);
    sharedBannerView->InitializeLastResult().OnCompletion(FirebaseAdmob::BannerInitCallback, settings);
}

bool FirebaseAdmob::IsBannerLoaded() {
    if (sharedBannerView != NULL &&
        sharedBannerView->LoadAdLastResult().status() == firebase::kFutureStatusComplete &&
        sharedBannerView->LoadAdLastResult().error() == firebase::admob::kAdMobErrorNone) {
        return true;
    } else {
        return false;
    }
}

void FirebaseAdmob::ShowBanner() {
    if(sharedBannerView != NULL &&
       sharedBannerView->LoadAdLastResult().status() == firebase::kFutureStatusComplete &&
       sharedBannerView->LoadAdLastResult().error() == firebase::admob::kAdMobErrorNone) {
        sharedBannerView->SetListener(new MyBannerViewListener(this));
        sharedBannerView->Show();
    } else {
        print_line("Banner not loaded");
    }
}

void FirebaseAdmob::CloseBanner() {
    if(sharedBannerView != NULL &&
       sharedBannerView->ShowLastResult().status() == firebase::kFutureStatusComplete &&
       sharedBannerView->ShowLastResult().error() == firebase::admob::kAdMobErrorNone) {
        sharedBannerView->Hide();
        sharedBannerView->HideLastResult().OnCompletion(FirebaseAdmob::BannerHideCallback, sharedBannerView);
        sharedBannerView = NULL;
    } else {
        print_line("Banner not shown");
    }
}

void FirebaseAdmob::LoadInterstitial(String bannerId) {
    sharedInterstitialAd = new firebase::admob::InterstitialAd();
    InterstitialSettings *settings = new InterstitialSettings(sharedInterstitialAd, this);
    sharedInterstitialAd->Initialize(getAdParent(), bannerId.utf8().ptr());
    sharedInterstitialAd->InitializeLastResult().OnCompletion(FirebaseAdmob::InterstitialInitCallback, settings);
}

bool FirebaseAdmob::IsInterstitialLoaded() {
    if (sharedInterstitialAd != NULL &&
        sharedInterstitialAd->LoadAdLastResult().status() == firebase::kFutureStatusComplete &&
        sharedInterstitialAd->LoadAdLastResult().error() == firebase::admob::kAdMobErrorNone) {
        return true;
    } else {
        return false;
    }
}

void FirebaseAdmob::ShowInterstitial() {
    if (sharedInterstitialAd != NULL &&
        sharedInterstitialAd->LoadAdLastResult().status() == firebase::kFutureStatusComplete &&
        sharedInterstitialAd->LoadAdLastResult().error() == firebase::admob::kAdMobErrorNone) {
        sharedInterstitialAd->SetListener(new MyInterstitialAdListener(this));
        sharedInterstitialAd->Show();
    } else {
        print_line("Interstitial not loaded");
    }
}

void FirebaseAdmob::LoadRewarded(String bannerId) {
    std::string bid(bannerId.utf8().ptr());
    RewardedSettings *settings = new RewardedSettings(bid, this);
    if(!rewarded_inited) {
        firebase::admob::rewarded_video::Initialize();
        firebase::admob::rewarded_video::InitializeLastResult().OnCompletion(FirebaseAdmob::RewardedInitCallback, settings);
    } else {
        firebase::admob::rewarded_video::LoadAd(settings->adId.c_str(), my_ad_request);
        firebase::admob::rewarded_video::LoadAdLastResult().OnCompletion(FirebaseAdmob::RewardedLoadedCallback, settings);
    }
}

bool FirebaseAdmob::IsRewardedLoaded() {
    if (rewarded_inited &&
        firebase::admob::rewarded_video::LoadAdLastResult().status() == firebase::kFutureStatusComplete &&
        firebase::admob::rewarded_video::LoadAdLastResult().error() == firebase::admob::kAdMobErrorNone) {
        return true;
    } else {
        return false;
    }
}

void FirebaseAdmob::ShowRewarded() {
    if (firebase::admob::rewarded_video::LoadAdLastResult().status() == firebase::kFutureStatusComplete &&
        firebase::admob::rewarded_video::LoadAdLastResult().error() == firebase::admob::kAdMobErrorNone) {

        firebase::admob::rewarded_video::SetListener(new MyRewardedVideoListener(this));
        firebase::admob::rewarded_video::Show(getAdParent());
    } else {
        print_line("Rewarded not loaded");
    }
}

void FirebaseAdmob::_bind_methods() {

    ClassDB::bind_method(D_METHOD("init", "advertisingId"), &FirebaseAdmob::Init);
    ClassDB::bind_method(D_METHOD("add_test_device", "deviceId"), &FirebaseAdmob::AddTestDevice);

    ClassDB::bind_method(D_METHOD("load_banner", "bannerId"), &FirebaseAdmob::LoadBanner);
    ClassDB::bind_method(D_METHOD("is_banner_loaded"), &FirebaseAdmob::IsBannerLoaded);
    ClassDB::bind_method(D_METHOD("show_banner"), &FirebaseAdmob::ShowBanner);
    ClassDB::bind_method(D_METHOD("close_banner"), &FirebaseAdmob::CloseBanner);

    ClassDB::bind_method(D_METHOD("load_interstitial", "bannerId"), &FirebaseAdmob::LoadInterstitial);
    ClassDB::bind_method(D_METHOD("is_interstitial_loaded"), &FirebaseAdmob::IsInterstitialLoaded);
    ClassDB::bind_method(D_METHOD("show_interstitial"), &FirebaseAdmob::ShowInterstitial);

    ClassDB::bind_method(D_METHOD("load_rewarded", "bannerId"), &FirebaseAdmob::LoadRewarded);
    ClassDB::bind_method(D_METHOD("is_rewarded_loaded"), &FirebaseAdmob::IsRewardedLoaded);
    ClassDB::bind_method(D_METHOD("show_rewarded"), &FirebaseAdmob::ShowRewarded);

    ADD_SIGNAL(MethodInfo("banner_loading_result"));
    ADD_SIGNAL(MethodInfo("interstitial_loading_result"));
    ADD_SIGNAL(MethodInfo("rewarded_loading_result"));
    ADD_SIGNAL(MethodInfo("banner_state"));
    ADD_SIGNAL(MethodInfo("interstitial_state"));
    ADD_SIGNAL(MethodInfo("rewarded_state"));
    ADD_SIGNAL(MethodInfo("rewarded_completed"));
}

