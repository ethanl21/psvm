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

// This is less random than a true UUID, but for this project it should be good enough
namespace uuid
{
    static std::random_device rd;
    static std::mt19937 gen( rd() );
    static std::uniform_int_distribution<> dis( 0, 15 );
    static std::uniform_int_distribution<> dis2( 8, 11 );

    std::string generate_uuid_v4()
    {
        std::stringstream ss;
        int i;
        ss << std::hex;
        for ( i = 0; i < 8; i++ )
        {
            ss << dis( gen );
        }
        ss << "-";
        for ( i = 0; i < 4; i++ )
        {
            ss << dis( gen );
        }
        ss << "-4";
        for ( i = 0; i < 3; i++ )
        {
            ss << dis( gen );
        }
        ss << "-";
        ss << dis2( gen );
        for ( i = 0; i < 3; i++ )
        {
            ss << dis( gen );
        }
        ss << "-";
        for ( i = 0; i < 12; i++ )
        {
            ss << dis( gen );
        };
        return ss.str();
    }
}

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
        this->ctx_ = qjs::JS_NewContext( this->rt_ );

        // Add the response callback wrapper to the global scope in the JS context
        auto global = qjs::JS_GetGlobalObject( this->ctx_ );

        qjs::JS_SetPropertyStr(
            this->ctx_, global, "ResponseCallback",
            qjs::JS_NewCFunction( this->ctx_, impl::callback_wrapper_, "ResponseCallback", 3 ) );
        qjs::JS_FreeValue( this->ctx_, global );
    }

    ~impl()
    {
        // make sure all the JS events have been executed
        qjs::js_std_loop( this->ctx_ );

        // Free the JS context and runtime
        qjs::JS_FreeContext( this->ctx_ );
        qjs::JS_FreeRuntime( this->rt_ );
    }

    qjs::JSRuntime *rt_;
    qjs::JSContext *ctx_;

    static qjs::JSValue callback_wrapper_( qjs::JSContext *_ctx, qjs::JSValue this_val, int argc,
                                           qjs::JSValue *argv )
    {
        using namespace qjs; // JS_EXCEPTION and JS_UNDEFINED below are macros, so we need this

        if ( argc != 2 )
        {
            JS_ThrowTypeError( _ctx, "Expected 2 arguments" );
            return JS_EXCEPTION;
        }

        // Convert the JS strings to c-strings
        const char *id_cstr = JS_ToCString( _ctx, argv[0] );
        const char *msg_cstr = JS_ToCString( _ctx, argv[1] );

        std::string id = std::string( id_cstr );
        std::string msg = std::string( msg_cstr );

        // Clean up
        JS_FreeCString( _ctx, id_cstr );
        JS_FreeCString( _ctx, msg_cstr );

        // Call this instance's response callback if it exists using the pointer to this instance
        // stored in the file scope
        if ( g_ShowdownService->on_msg_received_callback_ )
        {
            g_ShowdownService->on_msg_received_callback_.value()( id, msg );
        }

        return JS_UNDEFINED;
    }
};

ShowdownService::ShowdownService() : pimpl( new impl )
{
    // Add ShowdownService class to global namespace
    qjs::js_std_eval_binary( this->pimpl->ctx_, psvmjs::qjsc_psvm, psvmjs::qjsc_psvm_size, 0 );
    qjs::js_std_eval_binary( this->pimpl->ctx_, psvmjs::qjsc_globalize, psvmjs::qjsc_globalize_size,
                             0 );

    // Initialize ShowdownService instance
    auto create_sim_eval = "globalThis.ShowdownServiceInstance = new psvm.ShowdownService();";
    auto r = qjs::JS_Eval( this->pimpl->ctx_, create_sim_eval, std::strlen( create_sim_eval ), "",
                           JS_EVAL_TYPE_GLOBAL );
    qjs::JS_FreeValue( this->pimpl->ctx_, r );

    // Execute JS event loop
    qjs::js_std_loop( this->pimpl->ctx_ );

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
    auto battle_id = uuid::generate_uuid_v4();

    // Construct the JS code used to create a new battle
    std::stringstream js_sstream;
    js_sstream << "ShowdownServiceInstance.startBattle('" << battle_id << "');";

    // Create a new battle using the generated uuid
    auto r = qjs::JS_Eval( this->pimpl->ctx_, js_sstream.str().c_str(), js_sstream.str().length(),
                           "", JS_EVAL_TYPE_GLOBAL );
    qjs::JS_FreeValue( this->pimpl->ctx_, r );
    qjs::js_std_loop( this->pimpl->ctx_ );

    return battle_id;
}

void ShowdownService::WriteMessage( const std::string &id, const std::string &message )
{
    // Construct the JS code used to write to a battle stream
    std::stringstream js_sstream;
    js_sstream << "ShowdownServiceInstance.writeToBattle('" << id << "', '" << message << "');";

    // Write to a battle stream using its uuid
    auto r = qjs::JS_Eval( this->pimpl->ctx_, js_sstream.str().c_str(), js_sstream.str().length(),
                           "", JS_EVAL_TYPE_GLOBAL );
    qjs::JS_FreeValue( this->pimpl->ctx_, r );

    qjs::js_std_loop( this->pimpl->ctx_ );
}

void ShowdownService::setSimulatorOnResponseCallback(
    const std::function<void( std::string, std::string )> &simRespCallback )
{
    on_msg_received_callback_ = simRespCallback;
}

void ShowdownService::clearSimulatorOnResponseCallback()
{
    this->on_msg_received_callback_.reset();
}
