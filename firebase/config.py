def can_build(env, platform):
    if platform == "android":
	return True
    if platform == "ios":
	return True
    return False

def configure(env):
    if (env['platform'] == 'android'):
	env.android_add_default_config("applicationId 'free.games.match.three.puzzle.galactic.adventure.android'")
	env.android_add_default_config("testApplicationId 'free.games.match.three.puzzle.galactic.adventure.android'")
	env.android_add_dependency("implementation 'com.android.support:support-v4:28.0.0'")
	#env.android_add_dependency("implementation 'com.android.support:preference-v7:28.0.0'")
	#env.android_add_dependency("implementation 'com.android.support:appcompat-v7:28.0.0'")
	env.android_add_gradle_classpath("com.google.gms:google-services:4.1.0")
	env.android_add_gradle_plugin("com.google.gms.google-services")
	env.android_add_dependency("implementation 'com.google.firebase:firebase-core:16.0.5'")
	env.android_add_dependency("implementation 'com.google.firebase:firebase-analytics:16.0.5'")
	env.android_add_dependency("implementation 'com.google.firebase:firebase-config:16.0.1'")
	env.android_add_dependency("implementation 'com.google.android.gms:play-services-base:16.0.1'")
	# Crashlytics
	env.android_add_maven_repository("url 'https://maven.fabric.io/public'")
	env.android_add_gradle_classpath("io.fabric.tools:gradle:1.26.1")
	env.android_add_gradle_plugin("io.fabric")
	env.android_add_dependency("implementation 'com.crashlytics.sdk.android:crashlytics:2.9.7'")
	# Performance monitor
	#env.android_add_gradle_classpath("com.google.firebase:firebase-plugins:1.1.5")
	#env.android_add_gradle_plugin("com.google.firebase.firebase-perf")
	#env.android_add_dependency("implementation 'com.google.firebase:firebase-perf:16.0.0'")
	# Realtime Database
	env.android_add_dependency("implementation 'com.google.firebase:firebase-database:16.0.5'")
	# Auth
	env.android_add_dependency("implementation 'com.google.firebase:firebase-auth:16.0.+'")
    elif (env['platform'] == 'ios'):
	pass
