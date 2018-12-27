def can_build(env, platform):
    if platform == "android":
	return True
    if platform == "ios":
	return True
    return False

def configure(env):
    if (env['platform'] == 'android'):
	env.android_add_gradle_classpath("com.google.gms:google-services:3.2.0")
	env.android_add_gradle_plugin("com.google.gms.google-services")
	env.android_add_dependency("implementation 'com.google.firebase:firebase-core:16.0.5'")
	env.android_add_dependency("implementation 'com.google.firebase:firebase-analytics:16.0.5'")
	env.android_add_dependency("implementation 'com.google.firebase:firebase-config:16.0.1'")
	env.android_add_dependency("implementation 'com.google.android.gms:play-services-base:16.0.1'")
    elif (env['platform'] == 'ios'):
	pass
