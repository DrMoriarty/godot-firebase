#include "functions.h"
#include "convertor.h"

bool FirebaseFunctions::inited = false;
firebase::functions::Functions* FirebaseFunctions::functions = NULL;

class FunctionContext {
public:
    std::string function_name;
    FirebaseFunctions* controller;
    
    FunctionContext(const std::string& fn, FirebaseFunctions* cn): function_name(fn), controller(cn) {}
};

FirebaseFunctions::FirebaseFunctions() {
    if(!inited) {
        firebase::App* app = Firebase::AppId();
        if(app != NULL) {
            firebase::InitResult result;
            functions = firebase::functions::Functions::GetInstance(app, &result);
            if(result == firebase::kInitResultSuccess) {
                inited = true;
            }
        }
    }
}

void FirebaseFunctions::CallFunction(const String& function_name)
{
    if(!inited) return;
    firebase::functions::HttpsCallableReference func = functions->GetHttpsCallable(function_name.utf8().get_data());
    firebase::Future<firebase::functions::HttpsCallableResult> future = func.Call();
    FunctionContext *fc = new FunctionContext(function_name.utf8().get_data(), this);
    future.OnCompletion([](const firebase::Future<firebase::functions::HttpsCallableResult> & completed_future, void* user_data) {
                            FunctionContext *fc = static_cast<FunctionContext*>(user_data);
                            fc->controller->ProcessFunctionResult(fc->function_name, completed_future.result()->data());
                            delete fc;
                        }, fc);
}

void FirebaseFunctions::CallFunctionWithId(const String& function_name, const String& callback_id)
{
    if(!inited) return;
    firebase::functions::HttpsCallableReference func = functions->GetHttpsCallable(function_name.utf8().get_data());
    firebase::Future<firebase::functions::HttpsCallableResult> future = func.Call();
    FunctionContext *fc = new FunctionContext(callback_id.utf8().get_data(), this);
    future.OnCompletion([](const firebase::Future<firebase::functions::HttpsCallableResult> & completed_future, void* user_data) {
                            FunctionContext *fc = static_cast<FunctionContext*>(user_data);
                            fc->controller->ProcessFunctionResult(fc->function_name, completed_future.result()->data());
                            delete fc;
                        }, fc);
}

void FirebaseFunctions::CallFunctionWithArg(const String& function_name, const Dictionary& params)
{
    if(!inited) return;
    firebase::Variant fp = Convertor::toFirebaseVariant(params);
    firebase::functions::HttpsCallableReference func = functions->GetHttpsCallable(function_name.utf8().get_data());
    firebase::Future<firebase::functions::HttpsCallableResult> future = func.Call(fp);
    FunctionContext *fc = new FunctionContext(function_name.utf8().get_data(), this);
    future.OnCompletion([](const firebase::Future<firebase::functions::HttpsCallableResult> & completed_future, void* user_data) {
                            FunctionContext *fc = static_cast<FunctionContext*>(user_data);
                            fc->controller->ProcessFunctionResult(fc->function_name, completed_future.result()->data());
                            delete fc;
                        }, fc);
}

void FirebaseFunctions::CallFunctionWithIdAndArg(const String& function_name, const String& callback_id, const Dictionary& params)
{
    if(!inited) return;
    firebase::Variant fp = Convertor::toFirebaseVariant(params);
    firebase::functions::HttpsCallableReference func = functions->GetHttpsCallable(function_name.utf8().get_data());
    firebase::Future<firebase::functions::HttpsCallableResult> future = func.Call(fp);
    FunctionContext *fc = new FunctionContext(callback_id.utf8().get_data(), this);
    future.OnCompletion([](const firebase::Future<firebase::functions::HttpsCallableResult> & completed_future, void* user_data) {
                            FunctionContext *fc = static_cast<FunctionContext*>(user_data);
                            fc->controller->ProcessFunctionResult(fc->function_name, completed_future.result()->data());
                            delete fc;
                        }, fc);
}

void FirebaseFunctions::ProcessFunctionResult(const std::string& function_name, const firebase::Variant & data)
{
    Variant godot_param = Convertor::fromFirebaseVariant(data);
    emit_signal("function_result", String(function_name.c_str()), godot_param);
}

void FirebaseFunctions::_bind_methods() {
    ClassDB::bind_method(D_METHOD("call_function"), &FirebaseFunctions::CallFunction);
    ClassDB::bind_method(D_METHOD("call_function_with_id"), &FirebaseFunctions::CallFunctionWithId);
    ClassDB::bind_method(D_METHOD("call_function_with_arg"), &FirebaseFunctions::CallFunctionWithArg);
    ClassDB::bind_method(D_METHOD("call_function_with_id_and_arg"), &FirebaseFunctions::CallFunctionWithIdAndArg);
    ADD_SIGNAL(MethodInfo("function_result"));
}

