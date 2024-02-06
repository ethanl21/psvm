import { BattleStreams } from "@pkmn/sim";
import { ObjectReadWriteStream } from "@pkmn/streams";

/**
 * @file main.ts
 *
 * @brief JavaScript implementation of psvm used to simulate battles
 */

/**
 * ShowdownService
 * @brief Manages Pokémon Showdown BattleStream objects to simulate battles
 */
export class ShowdownService {
  battle_dict: Map<string, ObjectReadWriteStream<string>>;

  constructor() {
    this.battle_dict = new Map();
  }

  /**
   * @brief Starts a new battle
   * @param id UUID to assign to the battle. An existing battle with the same ID will be deleted
   */
  startBattle(id: string) {
    // Delete the battle stream if it already exists
    this.battle_dict.delete(id);

    // Create the battle streams
    const streams = BattleStreams.getPlayerStreams(
      new BattleStreams.BattleStream(),
    );

    // Add the omniscient stream to the streams dict
    this.battle_dict.set(id, streams.omniscient);

    // Write the output to the output function
    void (async (id) => {
      for await (const chunk of streams.omniscient) {
        ResponseCallback(id, chunk);
      }
    })(id);
  }

  /**
   * @brief Kill a battle
   * @param id UUID of the battle to kill
   */
  killBattle(id: string) {
    // delete the battle from the dict
    this.battle_dict.delete(id);
  }

  /**
   * @brief Kill all the currently active battles
   */
  killAllBattles() {
    this.battle_dict.clear();
  }

  /**
   * @brief Write a message to a battle stream if it exists
   * @param id UUID of the battle
   * @param message Message to write to the battle stream
   */
  writeToBattle(id: string, message: string) {
    const battleStream = this.battle_dict.get(id);

    if (battleStream) {
      let msgCopy = message.slice();
      if (!msgCopy.endsWith("\n")) {
        msgCopy += "\n";
      }

      battleStream.write(msgCopy);
    }
  }
}
