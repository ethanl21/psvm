import { BattleStreams } from "@pkmn/sim";
import { ObjectReadWriteStream } from "@pkmn/streams";

/**
 * ShowdownService
 * Manages Pokémon Showdown BattleStream objects to simulate battles.
 */
class ShowdownService {
  private battleDict: Map<string, ObjectReadWriteStream<string>>;

  constructor() {
    this.battleDict = new Map();
  }

  /**
   * Start a new battle.
   * @param id UUID of the battle
   */
  startBattle(id: string): string {
    // Delete if already exists
    this.battleDict.delete(id);

    // Create the battle streams
    const streams = BattleStreams.getPlayerStreams(
      new BattleStreams.BattleStream(),
    );

    // Store the omniscient stream
    this.battleDict.set(id, streams.omniscient);

    // Write each chunk to the global ResponseCallback
    void (async (battleId: string) => {
      for await (const chunk of streams.omniscient) {
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
        if (typeof (globalThis as any).ResponseCallback === "function") {
          // eslint-disable-next-line @typescript-eslint/no-explicit-any
          (globalThis as any).ResponseCallback(battleId, chunk);
        }
      }
    })(id);

    return id;
  }

  /**
   * Kill a specific battle
   * @param id UUID of the battle
   */
  killBattle(id: string): void {
    this.battleDict.delete(id);
  }

  /**
   * Kill all active battles
   */
  killAllBattles(): void {
    this.battleDict.clear();
  }

  /**
   * Write a message to a battle
   * @param id UUID of the battle
   * @param message message to write
   */
  writeToBattle(id: string, message: string): void {
    const battleStream = this.battleDict.get(id);
    if (!battleStream) return;

    let msgCopy = message;
    if (!msgCopy.endsWith("\n")) msgCopy += "\n";
    battleStream.write(msgCopy);
  }
}

// Expose globally for C++ QuickJS bindings
// eslint-disable-next-line @typescript-eslint/no-explicit-any
(globalThis as any).showdownService = ShowdownService;
