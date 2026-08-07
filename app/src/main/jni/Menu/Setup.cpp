#include "Includes/obfuscate.h"
#include "Includes/Logger.h"
#include "Menu/Menu.hpp"
#include "Utils.hpp"

JavaVM *g_vm;
jclass g_mainClazz;

static void RegisterMethod(JNIEnv *env, jclass clazz, JNINativeMethod method) {
    if (env->RegisterNatives(clazz, &method, 1) != 0) {
        LOGE(OBFUSCATE("Failed to register native method %s%s"), method.name, method.signature);
        env->ExceptionClear();
    }
}

int RegisterMenu(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("Icon"),            OBFUSCATE(
                                                   "()Ljava/lang/String;"),                                                           reinterpret_cast<void *>(Icon)},
            {OBFUSCATE("IconWebViewData"), OBFUSCATE(
                                                   "()Ljava/lang/String;"),                                                           reinterpret_cast<void *>(IconWebViewData)},
            {OBFUSCATE("IsGameLibLoaded"), OBFUSCATE(
                                                   "()Z"),                                                                            reinterpret_cast<void *>(isGameLibLoaded)},
            {OBFUSCATE("Init"),            OBFUSCATE(
                                                   "(Landroid/content/Context;Landroid/widget/TextView;Landroid/widget/TextView;)V"), reinterpret_cast<void *>(Init)},
            {OBFUSCATE("SettingsList"),    OBFUSCATE(
                                                   "()[Ljava/lang/String;"),                                                          reinterpret_cast<void *>(SettingsList)},
            {OBFUSCATE("GetFeatureList"),  OBFUSCATE(
                                                    "()[Ljava/lang/String;"),                                                          reinterpret_cast<void *>(GetFeatureList)},
            {OBFUSCATE("GetDeckInfo"),     OBFUSCATE(
                                                    "()Ljava/lang/String;"),                                                          reinterpret_cast<void *>(GetDeckInfo)},
    };

    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/Menu"));
    if (!clazz) {
        LOGE(OBFUSCATE("FindClass failed: com/android/support/Menu"));
        env->ExceptionClear();
        return JNI_OK;
    }
    for (JNINativeMethod method : methods)
        RegisterMethod(env, clazz, method);
    return JNI_OK;
}

int RegisterPreferences(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("Changes"), OBFUSCATE("(Landroid/content/Context;ILjava/lang/String;IJZLjava/lang/String;)V"), reinterpret_cast<void *>(Changes)},
    };
    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/Preferences"));
    if (!clazz) {
        LOGE(OBFUSCATE("FindClass failed: com/android/support/Preferences"));
        env->ExceptionClear();
        return JNI_OK;
    }
    for (JNINativeMethod method : methods)
        RegisterMethod(env, clazz, method);
    return JNI_OK;
}

int RegisterMain(JNIEnv *env) {
    JNINativeMethod methods[] = {
            {OBFUSCATE("CheckOverlayPermission"), OBFUSCATE("(Landroid/content/Context;)V"),
             reinterpret_cast<void *>(CheckOverlayPermission)},
    };
    jclass clazz = env->FindClass(OBFUSCATE("com/android/support/Main"));
    if (!clazz) {
        LOGE(OBFUSCATE("FindClass failed: com/android/support/Main"));
        env->ExceptionClear();
        return JNI_OK;
    }
    for (JNINativeMethod method : methods)
        RegisterMethod(env, clazz, method);
    return JNI_OK;
}

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    g_vm = vm;
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        LOGE(OBFUSCATE("JNI_OnLoad: GetEnv failed"));
        return JNI_ERR;
    }
    
    jclass localClassRef = env->FindClass("com/android/support/Main");
    if (localClassRef != NULL) {
        g_mainClazz = (jclass)env->NewGlobalRef(localClassRef);
    } else {
        env->ExceptionClear();
    }
    
    RegisterMenu(env);
    RegisterPreferences(env);
    RegisterMain(env);
    return JNI_VERSION_1_6;
}