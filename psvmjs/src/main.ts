import { BattleStreams, RandomPlayerAI, Teams } from "@pkmn/sim";
import { TeamGenerators } from "@pkmn/randoms";

type onResponseFunctionType = (chunk: string) => void;
export class ShowdownSimulator {
  streams: ReturnType<typeof BattleStreams.getPlayerStreams>;
  onResponseOmniscient: onResponseFunctionType | undefined;

  constructor() {
    this.streams = BattleStreams.getPlayerStreams(
      new BattleStreams.BattleStream()
    );

    void (async () => {
      for await (const chunk of this.streams.omniscient) {
        this.onResponseOmniscient && this.onResponseOmniscient(chunk);
      }
    })();
  }

  writeToOmniscient(chunk: string) {
    if (!chunk.endsWith("\n")) {
      chunk += "\n";
    }

    this.streams.omniscient.write(chunk);
  }
}
