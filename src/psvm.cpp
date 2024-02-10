/*
 * @file psvm.cpp
 *
 * @brief implementation of ShowdownService
 */

#include <psvm/psvm.hpp>

// qjsc compiled bytecode
namespace psvmjs
{
    extern "C"
    {

#include "psvmjs.c"
    };
}

namespace qjs
{
    extern "C"
    {

#include "quickjs-libc.h"
#include "quickjs.h"
    }
};

// HACK: C can only call static functions (which have no access to "this"),
//       so store the instance as a file-scoped variable. Note that this means
//       there can only be one ShowdownService active at a time. Fix this later
static ShowdownService *g_ShowdownService;

class ShowdownService::impl
{
public:
    impl()
    {
        this->rt_ = qjs::JS_NewRuntime();
        this->ctx_ = qjs::JS_NewContext(this->rt_);

        // store reference to globalThis
        this->js_globalThis = qjs::JS_GetGlobalObject(this->ctx_);

        // Add callback wrapper to the global namespace
        // TODO change this to set a callback member in the class
        qjs::JS_SetPropertyStr(
            this->ctx_, this->js_globalThis, "ResponseCallback",
            qjs::JS_NewCFunction(this->ctx_, impl::callback_wrapper_, "ResponseCallback", 3));

        // Add ShowdownService class to global namespace
        qjs::js_std_eval_binary(this->ctx_, psvmjs::qjsc_psvm, psvmjs::qjsc_psvm_size, 0);
        qjs::js_std_eval_binary(this->ctx_, psvmjs::qjsc_globalize, psvmjs::qjsc_globalize_size,
                                0);

        qjs::JSValue showdownServiceCtor =
            qjs::JS_GetPropertyStr(this->ctx_, this->js_globalThis, "ShowdownService");
        this->js_showdownService =
            qjs::JS_CallConstructor(this->ctx_, showdownServiceCtor, 0, nullptr);

        qjs::JS_FreeValue(this->ctx_, showdownServiceCtor);

        // Store references to the ShowdownService JS class instance's methods
        this->js_startBattle =
            qjs::JS_GetPropertyStr(this->ctx_, this->js_showdownService, "startBattle");
        this->js_killBattle =
            qjs::JS_GetPropertyStr(this->ctx_, this->js_showdownService, "killBattle");
        this->js_killAllBattles =
            qjs::JS_GetPropertyStr(this->ctx_, this->js_showdownService, "killAllBattles");
        this->js_writeToBattle =
            qjs::JS_GetPropertyStr(this->ctx_, this->js_showdownService, "writeToBattle");
    }

    ~impl()
    {
        // Free function refs
        qjs::JS_FreeValue(this->ctx_, this->js_startBattle);
        qjs::JS_FreeValue(this->ctx_, this->js_killBattle);
        qjs::JS_FreeValue(this->ctx_, this->js_killAllBattles);
        qjs::JS_FreeValue(this->ctx_, this->js_writeToBattle);

        // free ShowdownService instance red
        qjs::JS_FreeValue(this->ctx_, this->js_showdownService);

        // Free globalThis JSValue
        qjs::JS_FreeValue(this->ctx_, this->js_globalThis);

        // Free the JS context and runtime
        qjs::JS_FreeContext(this->ctx_);
        qjs::JS_FreeRuntime(this->rt_);
    }

    // qjs context and runtime ptrs
    qjs::JSRuntime *rt_;
    qjs::JSContext *ctx_;

    // JS globalThis
    qjs::JSValue js_globalThis;

    // JS ShowdownService instance
    qjs::JSValue js_showdownService;

    // ShowdownService JS class instance methods
    qjs::JSValue js_startBattle;
    qjs::JSValue js_killBattle;
    qjs::JSValue js_killAllBattles;
    qjs::JSValue js_writeToBattle;

    static qjs::JSValue callback_wrapper_(qjs::JSContext *_ctx, qjs::JSValue this_val, int argc,
                                          qjs::JSValue *argv)
    {
        using namespace qjs; // JS_EXCEPTION and JS_UNDEFINED below are macros, so we need this

        if (argc != 2)
        {
            JS_ThrowTypeError(_ctx, "Expected 2 arguments");
            return JS_EXCEPTION;
        }

        // Convert the JS strings to c-strings
        const char *id_cstr = JS_ToCString(_ctx, argv[0]);
        const char *msg_cstr = JS_ToCString(_ctx, argv[1]);

        std::string id = std::string(id_cstr);
        std::string msg = std::string(msg_cstr);

        // Clean up
        JS_FreeCString(_ctx, id_cstr);
        JS_FreeCString(_ctx, msg_cstr);

        // Call this instance's response callback if it exists using the pointer to this instance
        // stored in the file scope
        if (g_ShowdownService->on_msg_received_callback_)
        {
            g_ShowdownService->on_msg_received_callback_.value()(id, msg);
        }

        return JS_UNDEFINED;
    }
};

ShowdownService::ShowdownService() : pimpl(new impl)
{
    // TODO: throw an exception if there's already an existing ShowdownService instance
    // Add this instance to the file scope
    g_ShowdownService = this;
}

ShowdownService::~ShowdownService()
{
    g_ShowdownService = nullptr;
}

std::string ShowdownService::CreateBattle()
{
    // call the "startBattle" fn and store the nanoid result
    qjs::JSValue result = qjs::JS_Call(this->pimpl->ctx_, this->pimpl->js_startBattle,
                                       this->pimpl->js_showdownService, 0, nullptr);

    std::string new_id;

    if (!qjs::JS_IsException(result))
    {
        const char *message = qjs::JS_ToCString(this->pimpl->ctx_, result);
        new_id = std::string(message);
        qjs::JS_FreeCString(this->pimpl->ctx_, message);
    }
    else
    {
        // debug, todo handle the exception instead
        new_id = std::string("-1");
    }

    return new_id;
}

void ShowdownService::DeleteBattle(const std::string &id)
{

    // arguments array (battle id to delete)
    qjs::JSValue args[1];
    args[0] = qjs::JS_NewString(this->pimpl->ctx_, id.c_str());

    // todo: return success or failure instead of nothing

    qjs::JS_Call(this->pimpl->ctx_, this->pimpl->js_killBattle, this->pimpl->js_showdownService, 1,
                 args);

    // free the arguments array
    qjs::JS_FreeValue(this->pimpl->ctx_, args[0]);
}

void ShowdownService::DeleteAllBattles()
{
    // call killAllBattles
    qjs::JS_Call(this->pimpl->ctx_, this->pimpl->js_killAllBattles,
                 this->pimpl->js_showdownService, 0, nullptr);

    // todo: return success or failure instead of nothing
}

void ShowdownService::WriteMessage(const std::string &id, const std::string &message)
{
    // construct args array
    qjs::JSValue args[2];
    args[0] = qjs::JS_NewString(this->pimpl->ctx_, id.c_str());
    args[1] = qjs::JS_NewString(this->pimpl->ctx_, message.c_str());

    qjs::JS_Call(this->pimpl->ctx_, this->pimpl->js_writeToBattle, this->pimpl->js_showdownService,
                 2, args);

    qjs::js_std_loop(this->pimpl->ctx_);

    // free the arguments array
    qjs::JS_FreeValue(this->pimpl->ctx_, args[0]);
    qjs::JS_FreeValue(this->pimpl->ctx_, args[1]);
}

void ShowdownService::setSimulatorOnResponseCallback(
    const std::function<void(std::string, std::string)> &simRespCallback)
{
    on_msg_received_callback_ = simRespCallback;
}

void ShowdownService::clearSimulatorOnResponseCallback()
{
    this->on_msg_received_callback_.reset();
}
