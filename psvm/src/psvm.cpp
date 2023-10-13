#include "psvm.hpp"

// HACK: C can only call static functions (which have no access to "this"),
//       so store the instance as a file-scoped variable. Note that this means
//       there can only be one ShowdownService active at a time. Fix this later
static ShowdownService *g_ShowdownService;

ShowdownService::ShowdownService() {
    // Create JS runtime and context
    this->rt = qjs::JS_NewRuntime();
    js_std_init_handlers(this->rt);
    JS_SetModuleLoaderFunc(this->rt, nullptr, qjs::js_module_loader, nullptr);
    this->ctx = qjs::JS_NewContext(this->rt);

    // Add the response callback wrapper to the global scope in the JS context
    auto global = qjs::JS_GetGlobalObject(this->ctx);
    qjs::JS_SetPropertyStr(this->ctx, global, "ResponseCallback",
                           qjs::JS_NewCFunction(this->ctx, ShowdownService::callback_wrapper_, "ResponseCallback", 3));
    qjs::JS_FreeValue(this->ctx, global);

    // Add ShowdownService class to global namespace
    qjs::js_std_eval_binary(this->ctx, psvmjs::qjsc_psvm, psvmjs::qjsc_psvm_size, 0);
    qjs::js_std_eval_binary(this->ctx, psvmjs::qjsc_globalize, psvmjs::qjsc_globalize_size, 0);

    // Initialize ShowdownService instance
    auto create_sim_eval = "globalThis.ShowdownServiceInstance = new psvm.ShowdownService();";
    qjs::JS_Eval(this->ctx, create_sim_eval, strlen(create_sim_eval), "<input>", JS_EVAL_TYPE_MODULE);

    // Execute JS event loop
    qjs::js_std_loop(this->ctx);

    // TODO: throw an exception if there's already an existing ShowdownService instance
    // Add this instance to the file scope
    g_ShowdownService = this;
}

ShowdownService::~ShowdownService() {
    // make sure all the JS events have been executed
    qjs::js_std_loop(this->ctx);

    // Free the JS context and runtime
    qjs::JS_FreeContext(this->ctx);
    qjs::JS_FreeRuntime(this->rt);
}

std::string ShowdownService::get_uuid() {
    return boost::uuids::to_string(this->uuid_generator_());
}

std::string ShowdownService::create_battle() {
    auto battle_id = this->get_uuid();

    // Construct the JS code used to create a new battle
    std::stringstream js_sstream;
    js_sstream << "ShowdownServiceInstance.startBattle('" << battle_id << "');";

    // Create a new battle using the generated uuid
    qjs::JS_Eval(this->ctx, js_sstream.str().c_str(), js_sstream.str().length(), "<input>", JS_EVAL_TYPE_MODULE);
    qjs::js_std_loop(this->ctx);

    return battle_id;
}

void ShowdownService::write_to_battle(const std::string &id, const std::string &message) {
    // Construct the JS code used to write to a battle stream
    std::stringstream js_sstream;
    js_sstream << "ShowdownServiceInstance.writeToBattle('" << id << "', '" << message << "');";

    // Write to a battle stream using its uuid
    qjs::JS_Eval(this->ctx, js_sstream.str().c_str(), js_sstream.str().length(), "<input>", JS_EVAL_TYPE_MODULE);
    qjs::js_std_loop(this->ctx);
}

qjs::JSValue ShowdownService::callback_wrapper_(qjs::JSContext *_ctx, qjs::JSValueConst this_val, int argc,
                                                qjs::JSValueConst *argv) {
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
    if (g_ShowdownService->sim_resp_callback) {
        g_ShowdownService->sim_resp_callback.value()(std::string(id_cstr), std::string(msg_cstr));
    }

    // Clean up
    JS_FreeCString(_ctx, id_cstr);
    JS_FreeCString(_ctx, msg_cstr);

    return JS_UNDEFINED;
}
