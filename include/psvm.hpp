#ifndef PSVM_HPP
#define PSVM_HPP

#include <string>       // std::string
#include <sstream>      // std::stringstream
#include <optional>     // std::optional
#include <functional>   // std::function

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "psvmjs.hpp" // compiled JS bytecode containing simulator wrapper class

// wrap QuickJS in a namespace just in case
namespace qjs {
    extern "C"
    {

#include "quickjs-libc.h"
#include "quickjs.h"

    }
}

/// @brief A wrapper for smogon/pokemon-showdown's simulator
class ShowdownService {
public:
    /// @brief Create a new simulator JS context
    ShowdownService();

    /// @brief Free the simulator JS context
    ~ShowdownService();

    /// @brief Start a new battle stream
    /// @return UUID used to identify the new battle
    std::string create_battle();

    /// @brief Writes a line to a battle stream
    /// @param id UUID of the battle to write to
    /// @param message Line to write to the battle stream. '\n' will be appended to the end of the string if it is missing.
    void write_to_battle(const std::string &id, const std::string &message);

    /// @brief Callback function to call when a battle stream produces output
    std::optional<std::function<void(std::string, std::string)>> sim_resp_callback;

private:
    /// @brief Used to generate battle UUIDs
    boost::uuids::random_generator uuid_generator_;

    /// @brief Generate a UUID
    /// @return the generated UUID, as a std::string
    std::string get_uuid();

    /// @brief QuickJS Runtime
    qjs::JSRuntime *rt;

    /// @brief QuickJS COntext
    qjs::JSContext *ctx;

    /// @brief Calls the (non-static) callback function on simulator output
    /// @param _ctx QuickJS context
    /// @param this_val unused
    /// @param argc argument count (2)
    /// @param argv argument vector [std::string id, std::string msg]
    /// @return unused
    static qjs::JSValue
    callback_wrapper_(qjs::JSContext *_ctx, qjs::JSValueConst this_val, int argc, qjs::JSValueConst *argv);
};

#endif