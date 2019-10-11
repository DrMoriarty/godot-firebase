def can_build(env, platform):
    if platform == "android":
	return True
    if platform == "iphone":
	return True
    return False

def configure(env):
    if (env['platform'] == 'android'):
	#env.android_add_default_config("applicationId 'free.games.match.three.puzzle.galactic.adventure.android'")
	#env.android_add_default_config("testApplicationId 'free.games.match.three.puzzle.galactic.adventure.android'")
	env.android_add_dependency("implementation 'com.android.support:support-v4:28.0.0'")
	#env.android_add_dependency("implementation 'com.android.support:preference-v7:28.0.0'")
	#env.android_add_dependency("implementation 'com.android.support:appcompat-v7:28.0.0'")
	env.android_add_gradle_classpath("com.google.gms:google-services:4.3.2")
	env.android_add_gradle_plugin("com.google.gms.google-services")
	#env.android_add_dependency("implementation 'com.google.firebase:firebase-core:16.0.6'")
	env.android_add_dependency("implementation 'com.google.firebase:firebase-analytics:17.2.0'")
	env.android_add_dependency("implementation 'com.google.firebase:firebase-config:19.0.2'")
	env.android_add_dependency("implementation 'com.google.android.gms:play-services-base:16.0.1'")
	# Crashlytics
	env.android_add_maven_repository("url 'https://maven.fabric.io/public'")
	env.android_add_gradle_classpath("io.fabric.tools:gradle:1.26.1")
	env.android_add_gradle_plugin("io.fabric")
	env.android_add_dependency("implementation 'com.crashlytics.sdk.android:crashlytics:2.10.1'")
	# Performance monitor
	#env.android_add_gradle_classpath("com.google.firebase:firebase-plugins:1.1.5")
	#env.android_add_gradle_plugin("com.google.firebase.firebase-perf")
	#env.android_add_dependency("implementation 'com.google.firebase:firebase-perf:19.0.0'")
	# Realtime Database
	env.android_add_dependency("implementation 'com.google.firebase:firebase-database:19.1.0'")
	# Auth
	env.android_add_dependency("implementation 'com.google.firebase:firebase-auth:19.1.0'")
	# AdMob
	env.android_add_dependency("implementation 'com.google.firebase:firebase-ads:18.2.0'")
	env.android_add_dependency("implementation 'com.google.android.gms:play-services-ads:17.1.2'")
	env.android_add_to_manifest("AndroidManifest.xml")
	# Functions
	env.android_add_dependency("implementation 'com.google.firebase:firebase-functions:19.0.1'")
	# Messaging
	env.android_add_dependency("implementation 'com.google.firebase:firebase-messaging:20.0.0'")
	env.android_add_flat_dir('../../../modules/firebase/libs/android')
	env.android_add_dependency("implementation 'com.google.firebase.messaging.cpp:firebase_messaging_cpp@aar'")
	# In-App Messaging
	env.android_add_dependency("implementation 'com.google.firebase:firebase-inappmessaging-display:19.0.1'")
    elif (env['platform'] == 'iphone'):
	pass
