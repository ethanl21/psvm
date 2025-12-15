#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace psvm
{
/**
 * @brief Interface for managing Pokémon Showdown battle streams.
 *
 * This class wraps a QuickJS runtime internally to run JS simulations of
 * battles. QuickJS types are fully hidden from the user.
 */
class ShowdownService
{
public:
  /**
   * @brief Construct a new ShowdownService instance.
   *
   * Creates a new QuickJS runtime and context, evaluates the embedded JS
   * bytecode, and sets up the battle service instance.
   */
  ShowdownService ();

  /**
   * @brief Destroy the ShowdownService instance.
   *
   * Frees all QuickJS objects, context, and runtime.
   */
  ~ShowdownService ();

  /**
   * @brief Set a user-defined callback for receiving battle output.
   *
   * @param cb A function that takes the battle ID and a string chunk from
   * the battle stream.
   *
   * This callback will be called whenever the JS ShowdownService writes
   * output to a battle stream.
   */
  void setCallback (
      std::function<void (const std::string &id, const std::string &chunk)>
          cb);

  /**
   * @brief Write a message to a battle stream.
   *
   * @param id The battle ID to send the message to.
   * @param message The message to send.
   *
   * If the battle stream exists, the message will be written and the
   * QuickJS event loop will be run to process the message.
   */
  void writeToBattle (const std::string &id, const std::string &message);

  /**
   * @brief Start a new battle simulation.
   *
   * @param id The unique battle ID.
   *
   * If a battle with the given ID exists, it will be overwritten.
   * Starts reading the output asynchronously and forwards it to the
   * user callback.
   */
  void startBattle (const std::string &id);

  /**
   * @brief Kill a battle simulation.
   *
   * @param id The battle ID to kill.
   *
   * Deletes the corresponding battle stream if it exists.
   */
  void killBattle (const std::string &id);

  /**
   * @brief Kill all active battles.
   *
   * Clears all active battle streams and stops their output.
   */
  void killAllBattles ();

private:
  // Pimpl idiom: full definition hidden in .cpp
  struct Impl;
  std::unique_ptr<Impl> pimpl_;
};

} // namespace psvm
