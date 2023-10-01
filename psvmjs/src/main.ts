import { BattleStreams } from "@pkmn/sim";
import { WriteStream } from "@pkmn/streams";

type onResponseFunctionType = (chunk: string) => void;
export class ShowdownSimulator {
  streams: ReturnType<typeof BattleStreams.getPlayerStreams>;
  onResponseOmniscient: onResponseFunctionType | undefined;
  output: string[];

  constructor() {
    this.streams = BattleStreams.getPlayerStreams(
      new BattleStreams.BattleStream({ keepAlive: true })
    );

    this.output = new Array();

    void (async () => {
      for await (const chunk of this.streams.omniscient) {
        //this.onResponseOmniscient && this.onResponseOmniscient(chunk);
        chunk.split("\n").forEach((line) => this.output.push(line));
      }
    })();
  }

  async writeToOmniscient(chunk: string) {
    if (!chunk.endsWith("\n")) {
      chunk += "\n";
    }

    await this.streams.omniscient.write(chunk);
  }
}

import * as TextEncoding from "@zxing/text-encoding";
export { TextEncoding };
