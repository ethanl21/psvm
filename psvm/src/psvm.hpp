/**
 * @file psvm.hpp
 * @brief Defines the ShowdownSimulator class used to simulate Pokemon battles.
 */
#ifndef PSVM_HPP
#define PSVM_HPP

#include <string>       // std::string
#include <sstream>      // std::stringstream
#include <optional>     // std::optional
#include <functional>   // std::function

// forward declarations to make this header easier to link to
namespace qjs{
    class JSRuntime;
    class JSContext;
    class JSValue;
};

/**
 * @brief Manages Pokémon Showdown battle streams running in an embedded JavaScript context.
 */
class ShowdownService {
public:
    /**
     * @brief Creates a new simulator JS context
     */
    ShowdownService();

    /**
     * @brief Frees the simulator JS context
     */
    ~ShowdownService();

    /**
     * @brief Creates a new battle stream
     * @return UUID used to identify the new battle
     */
    std::string CreateBattle();

    /**
     * @brief Writes a line to a battle stream
     * @param id UUID of the battle to write to
     * @param message Line to write to the battle stream. '\n' will be appended to the end of the string if it is missing.
     */
    void WriteMessage(const std::string &id, const std::string &message);

    /**
     * @brief Sets the simulator message response callback
     * @param simulatorOnRespCallback a function that will be called whenever the simulator produces output
     */
    void setSimulatorOnResponseCallback(const std::optional<std::function<void(std::string, std::string)>> &simulatorOnRespCallback);

    /**
     * @brief Clears the simulator message response callback
     */
    void clearSimulatorOnResponseCallback();

private:
    /**
     * @brief QuickJS Runtime
     */
    qjs::JSRuntime *rt_;

    /**
     * @brief QuickJS Context
     */
    qjs::JSContext *ctx_;

    /**
     * @brief Calls the (non-static) callback function on simulator output
     * @param _ctx QuickJS context
     * @param this_val unused
     * @param argc argument count (2)
     * @param argv argument vector [std::string id, std::string msg]
     * @return unused
     */
    static qjs::JSValue
    callback_wrapper_(qjs::JSContext *_ctx, qjs::JSValue this_val, int argc, qjs::JSValue *argv);

    /**
    * @brief Callback function to call when a battle stream produces output
    */
    std::optional<std::function<void(std::string, std::string)>> on_msg_received_callback_;
};

#endif