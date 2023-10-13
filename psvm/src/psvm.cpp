/*
 * @file psvm.cpp
 *
 * @brief implementation of ShowdownService
 */

#include "psvm.hpp"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

// wrap QuickJS in a namespace just in case
namespace qjs {
    extern "C"
    {

#include "quickjs-libc.h"
#include "quickjs.h"

    }
}

// qjsc compiled bytecode
namespace psvmjs {
    extern "C" {

#include "psvmjs.c"

    };
}


// HACK: C can only call static functions (which have no access to "this"),
//       so store the instance as a file-scoped variable. Note that this means
//       there can only be one ShowdownService active at a time. Fix this later
static ShowdownService *g_ShowdownService;

ShowdownService::ShowdownService() {
    // Create JS runtime and context
    this->rt_ = qjs::JS_NewRuntime();
    js_std_init_handlers(this->rt_);
    JS_SetModuleLoaderFunc(this->rt_, nullptr, qjs::js_module_loader, nullptr);
    this->ctx_ = qjs::JS_NewContext(this->rt_);

    // Add the response callback wrapper to the global scope in the JS context
    auto global = qjs::JS_GetGlobalObject(this->ctx_);
    qjs::JS_SetPropertyStr(this->ctx_, global, "ResponseCallback",
                           qjs::JS_NewCFunction(this->ctx_, ShowdownService::callback_wrapper_, "ResponseCallback", 3));
    qjs::JS_FreeValue(this->ctx_, global);

    // Add ShowdownService class to global namespace
    qjs::js_std_eval_binary(this->ctx_, psvmjs::qjsc_psvm, psvmjs::qjsc_psvm_size, 0);
    qjs::js_std_eval_binary(this->ctx_, psvmjs::qjsc_globalize, psvmjs::qjsc_globalize_size, 0);

    // Initialize ShowdownService instance
    auto create_sim_eval = "globalThis.ShowdownServiceInstance = new psvm.ShowdownService();";
    qjs::JS_Eval(this->ctx_, create_sim_eval, strlen(create_sim_eval), "<input>", JS_EVAL_TYPE_MODULE);

    // Execute JS event loop
    qjs::js_std_loop(this->ctx_);

    // TODO: throw an exception if there's already an existing ShowdownService instance
    // Add this instance to the file scope
    g_ShowdownService = this;
}

ShowdownService::~ShowdownService() {
    // make sure all the JS events have been executed
    qjs::js_std_loop(this->ctx_);

    // Free the JS context and runtime
    qjs::JS_FreeContext(this->ctx_);
    qjs::JS_FreeRuntime(this->rt_);
}

std::string ShowdownService::CreateBattle() {
    auto battle_id = boost::uuids::to_string(boost::uuids::random_generator()());

    // Construct the JS code used to create a new battle
    std::stringstream js_sstream;
    js_sstream << "ShowdownServiceInstance.startBattle('" << battle_id << "');";

    // Create a new battle using the generated uuid
    qjs::JS_Eval(this->ctx_, js_sstream.str().c_str(), js_sstream.str().length(), "<input>", JS_EVAL_TYPE_MODULE);
    qjs::js_std_loop(this->ctx_);

    return battle_id;
}

void ShowdownService::WriteMessage(const std::string &id, const std::string &message) {
    // Construct the JS code used to write to a battle stream
    std::stringstream js_sstream;
    js_sstream << "ShowdownServiceInstance.writeToBattle('" << id << "', '" << message << "');";

    // Write to a battle stream using its uuid
    qjs::JS_Eval(this->ctx_, js_sstream.str().c_str(), js_sstream.str().length(), "<input>", JS_EVAL_TYPE_MODULE);
    qjs::js_std_loop(this->ctx_);
}

qjs::JSValue ShowdownService::callback_wrapper_(qjs::JSContext *_ctx, qjs::JSValue this_val, int argc,
                                                qjs::JSValue *argv) {
    using namespace qjs; // JS_EXCEPTION and JS_UNDEFINED below are macros, so we need this

    if (argc != 2) {
        JS_ThrowTypeError(_ctx, "Expected 2 arguments");
        return JS_EXCEPTION;
    }

    // Convert the JS strings to c-strings
    const char *id_cstr = JS_ToCString(_ctx, argv[0]);
    const char *msg_cstr = JS_ToCString(_ctx, argv[1]);

    // Call this instance's response callback if it exists using the pointer to this instance stored in
    // the file scope
    if (g_ShowdownService->on_msg_received_callback_) {
        g_ShowdownService->on_msg_received_callback_.value()(std::string(id_cstr), std::string(msg_cstr));
    }

    // Clean up
    JS_FreeCString(_ctx, id_cstr);
    JS_FreeCString(_ctx, msg_cstr);

    return JS_UNDEFINED;
}

void ShowdownService::setSimulatorOnResponseCallback(
        const std::optional<std::function<void(std::string, std::string)>> &simRespCallback) {
    on_msg_received_callback_ = simRespCallback;
}

void ShowdownService::clearSimulatorOnResponseCallback() {
    this->on_msg_received_callback_.reset();
}
