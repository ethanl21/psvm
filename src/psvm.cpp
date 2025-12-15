#include "psvm/psvm.hpp"

extern "C"
{
#include <quickjs-libc.h>
#include <quickjs.h>
}

#include "psvmjs.h" // compiled bytecode

namespace psvm
{
// Callback function container
struct CallbackData
{
  std::function<void (const std::string &, const std::string &)> user_callback;
};

// Private implementation struct
struct ShowdownService::Impl
{
  JSRuntime *rt = nullptr;
  JSContext *ctx = nullptr;
  JSValue js_showdownService = JS_UNDEFINED;
  JSValue js_writeToBattle = JS_UNDEFINED;

  std::unique_ptr<CallbackData> callback_data;

  ~Impl ()
  {
    // Free JS values first
    JS_FreeValue (ctx, js_writeToBattle);
    JS_FreeValue (ctx, js_showdownService);

    // Free context and runtime
    delete callback_data.release ();
    JS_FreeContext (ctx);
    JS_FreeRuntime (rt);
  }
};

// Callback function wrapper
static JSValue
ResponseCallback (JSContext *ctx, JSValueConst, int argc, JSValueConst *argv)
{
  auto *cbdata = static_cast<CallbackData *> (JS_GetContextOpaque (ctx));
  if (!cbdata || !cbdata->user_callback)
    return JS_UNDEFINED;

  const char *id_c = JS_ToCString (ctx, argv[0]);
  const char *chunk_c = JS_ToCString (ctx, argv[1]);

  if (id_c && chunk_c)
    {
      cbdata->user_callback (id_c, chunk_c);
    }

  JS_FreeCString (ctx, id_c);
  JS_FreeCString (ctx, chunk_c);
  return JS_UNDEFINED;
}

ShowdownService::ShowdownService () : pimpl_ (new Impl)
{
  pimpl_->rt = JS_NewRuntime ();
  if (!pimpl_->rt)
    throw std::runtime_error ("Failed to create JS runtime");

  pimpl_->ctx = JS_NewContext (pimpl_->rt);
  if (!pimpl_->ctx)
    throw std::runtime_error ("Failed to create JS context");

  // Allocate callback data
  pimpl_->callback_data = std::make_unique<CallbackData> ();
  JS_SetContextOpaque (pimpl_->ctx, pimpl_->callback_data.get ());

  // Initialize standard helpers
  js_std_set_worker_new_context_func (JS_NewContext);
  js_std_init_handlers (pimpl_->rt);

  // Register the callback
  JSValue global = JS_GetGlobalObject (pimpl_->ctx);
  JS_SetPropertyStr (
      pimpl_->ctx, global, "ResponseCallback",
      JS_NewCFunction (pimpl_->ctx, ResponseCallback, "ResponseCallback", 2));
  JS_FreeValue (pimpl_->ctx, global);

  // Evaluate compiled JS bytecode
  js_std_eval_binary (pimpl_->ctx, qjsc_psvm, qjsc_psvm_size, 0);

  // Create the ShowdownService instance
  JSValue global_obj = JS_GetGlobalObject (pimpl_->ctx);
  JSValue ctor
      = JS_GetPropertyStr (pimpl_->ctx, global_obj, "showdownService");
  pimpl_->js_showdownService
      = JS_CallConstructor (pimpl_->ctx, ctor, 0, nullptr);
  pimpl_->js_writeToBattle = JS_GetPropertyStr (
      pimpl_->ctx, pimpl_->js_showdownService, "writeToBattle");
  JS_FreeValue (pimpl_->ctx, ctor);
  JS_FreeValue (pimpl_->ctx, global_obj);
}

ShowdownService::~ShowdownService () = default;

void
ShowdownService::setCallback (
    std::function<void (const std::string &, const std::string &)> cb)
{
  pimpl_->callback_data->user_callback = std::move (cb);
}

void
ShowdownService::writeToBattle (const std::string &id,
                                const std::string &message)
{
  JSContext *ctx = pimpl_->ctx;
  JSValue args[2] = { JS_NewString (ctx, id.c_str ()),
                      JS_NewString (ctx, message.c_str ()) };

  JSValue ret = JS_Call (ctx, pimpl_->js_writeToBattle,
                         pimpl_->js_showdownService, 2, args);
  if (JS_IsException (ret))
    {
      JSValue ex = JS_GetException (ctx);
      const char *err = JS_ToCString (ctx, ex);
      std::cerr << "JS Exception: " << err << "\n";
      JS_FreeCString (ctx, err);
      JS_FreeValue (ctx, ex);
    }
  JS_FreeValue (ctx, ret);
  js_std_loop (ctx);

  JS_FreeValue (ctx, args[0]);
  JS_FreeValue (ctx, args[1]);
}

void
ShowdownService::startBattle (const std::string &id)
{
  JSContext *ctx = pimpl_->ctx;
  JSValue func
      = JS_GetPropertyStr (ctx, pimpl_->js_showdownService, "startBattle");
  if (!JS_IsUndefined (func))
    {
      JSValue arg = JS_NewString (ctx, id.c_str ());
      JSValue ret = JS_Call (ctx, func, pimpl_->js_showdownService, 1, &arg);
      JS_FreeValue (ctx, arg);
      JS_FreeValue (ctx, ret);
      js_std_loop (ctx);
    }
  JS_FreeValue (ctx, func);
}

void
ShowdownService::killBattle (const std::string &id)
{
  JSContext *ctx = pimpl_->ctx;
  JSValue func
      = JS_GetPropertyStr (ctx, pimpl_->js_showdownService, "killBattle");
  if (!JS_IsUndefined (func))
    {
      JSValue arg = JS_NewString (ctx, id.c_str ());
      JSValue ret = JS_Call (ctx, func, pimpl_->js_showdownService, 1, &arg);
      JS_FreeValue (ctx, arg);
      JS_FreeValue (ctx, ret);
      js_std_loop (ctx);
    }
  JS_FreeValue (ctx, func);
}

void
ShowdownService::killAllBattles ()
{
  JSContext *ctx = pimpl_->ctx;
  JSValue func
      = JS_GetPropertyStr (ctx, pimpl_->js_showdownService, "killAllBattles");
  if (!JS_IsUndefined (func))
    {
      JSValue ret
          = JS_Call (ctx, func, pimpl_->js_showdownService, 0, nullptr);
      JS_FreeValue (ctx, ret);
      js_std_loop (ctx);
    }
  JS_FreeValue (ctx, func);
}

} // namespace psvm
