#include "psvm.hpp"

ShowdownRuntime::ShowdownRuntime()
{
    // Create pipes
    if (pipe(this->parent_to_child) == -1 || pipe(this->child_to_parent) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE); // todo exceptions instead of exit
    }

    // Fork process
    this->child_pid = fork();
    if (this->child_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE); // todo exceptions instead of exit
    }

    if (this->child_pid == 0)
    {
        // inside child
        this->isParent = false;

        // close unused pipe ends
        close(this->parent_to_child[1]);
        close(this->child_to_parent[0]);

        // Init the JS runtime
        this->rt = JS_NewRuntime();
        js_std_init_handlers(this->rt);
        JS_SetModuleLoaderFunc(this->rt, NULL, js_module_loader, NULL);
        this->ctx = JS_NewCustomContext(this->rt);

        // Create the simulator
        JS_Eval(this->ctx, CREATE_SIM_JS.c_str(), CREATE_SIM_JS.size(), "<module>", JS_EVAL_TYPE_MODULE);

        // Setup the simulator
        JS_Eval(this->ctx, SETUP_SIM_JS.c_str(), SETUP_SIM_JS.size(), "<module>", JS_EVAL_TYPE_MODULE);

        // JS runtime event loop
        js_std_loop(this->ctx);

        JS_FreeContext(this->ctx);
        JS_FreeRuntime(this->rt);
    }
    else
    {
        // inside parent
        this->isParent = true;

        // close unused pipe ends
        close(this->parent_to_child[0]);
        close(this->child_to_parent[1]);
    }
}

ShowdownRuntime::~ShowdownRuntime()
{
    // Clean up
    if (this->isParent)
    {
        close(this->parent_to_child[1]);
        close(this->child_to_parent[0]);

        wait(NULL);
    }
    else
    {
        close(this->parent_to_child[0]);
        close(this->child_to_parent[1]);
    }
}

void ShowdownRuntime::insert(std::string &input)
{
    if (!this->isParent)
    {
        return;
    }

    // std::cout << "writing: " << input.substr(0, 10) << "..."
    //           << "\n";
    ssize_t bytesWritten = write(this->parent_to_child[1], input.c_str(), strlen(input.c_str()) + 1);

    if (bytesWritten == -1)
    {
        perror("write");
        exit(1); // todo exceptions instead of exit
    }
}

std::optional<std::string> ShowdownRuntime::readResult()
{
    if (!this->isParent)
    {
        return {};
    }

    char buffer[1024];
    ssize_t bytes_read = read(this->child_to_parent[0], buffer, sizeof(buffer));

    switch (bytes_read)
    {
    case -1:
        perror("read");
        exit(1); // todo exception
        break;
    case 0:
        return {};
    default:
        return std::string(buffer, bytes_read-1); // trim null terminator
    }

    return {};
}

JSContext *ShowdownRuntime::JS_NewCustomContext(JSRuntime *rt)
{
    JSContext *ctx = JS_NewContextRaw(rt);
    if (!ctx)
        return nullptr;
    JS_AddIntrinsicBaseObjects(ctx);
    JS_AddIntrinsicDate(ctx);
    JS_AddIntrinsicEval(ctx);
    JS_AddIntrinsicStringNormalize(ctx);
    JS_AddIntrinsicRegExp(ctx);
    JS_AddIntrinsicJSON(ctx);
    JS_AddIntrinsicProxy(ctx);
    JS_AddIntrinsicMapSet(ctx);
    JS_AddIntrinsicTypedArrays(ctx);
    JS_AddIntrinsicPromise(ctx);
    JS_AddIntrinsicBigInt(ctx);
    js_init_module_os(ctx, "os");
    js_init_module_std(ctx, "std");

    // Add compiled JS bundle to global namespace
    js_std_eval_binary(ctx, qjsc_psvm, qjsc_psvm_size, 0);
    js_std_eval_binary(ctx, qjsc_globalize, qjsc_globalize_size, 0);

    // Expose pipes to JS context
    JSValue global_obj = JS_GetGlobalObject(ctx);
    JSValue host = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, global_obj, "host", host);
    JS_SetPropertyStr(ctx, host, "readFd", JS_NewInt32(ctx, parent_to_child[0]));
    JS_SetPropertyStr(ctx, host, "writeFd", JS_NewInt32(ctx, child_to_parent[1]));

    JS_FreeValue(ctx, global_obj);
    return ctx;
}
