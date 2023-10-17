/**
 * @file psvm.hpp
 * @brief Defines the ShowdownSimulator class used to simulate Pokemon battles.
 */
#ifndef PSVM_HPP
#define PSVM_HPP

#include <functional> // std::function
#include <memory>     // std::unique_ptr
#include <optional>   // std::optional
#include <random>     // std::random_device, std::mt19937, std::uniform_int_distribution
#include <sstream>    // std::stringstream
#include <string>     // std::string

/**
 * @brief Manages Pokémon Showdown battle streams running in an embedded JavaScript context.
 */
class ShowdownService
{
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
     * @param message Line to write to the battle stream. '\\n' will be appended to the end of the
     * string if it is missing.
     */
    void WriteMessage( const std::string &id, const std::string &message );

    /**
     * @brief Sets the simulator message response callback
     * @param simulatorOnRespCallback a function that will be called whenever the simulator produces
     * output
     */
    void setSimulatorOnResponseCallback(
        const std::optional<std::function<void( std::string, std::string )>>
            &simulatorOnRespCallback );

    /**
     * @brief Clears the simulator message response callback
     */
    void clearSimulatorOnResponseCallback();

private:
    /// \cond HIDDEN_SYMBOLS
    // pImpl used so the end user doesn't have to link QuickJS or Boost::uuid
    class impl;

    std::unique_ptr<impl> pimpl;

    /// \endcond

    /**
     * @brief Callback function to call when a battle stream produces output
     */
    std::optional<std::function<void( std::string, std::string )>> on_msg_received_callback_;
};

#endif