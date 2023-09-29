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

export function runTests(onSimResponse: onResponseFunctionType) {
  Teams.setGeneratorFactory(TeamGenerators);

  const showdownSim = new ShowdownSimulator();
  showdownSim.onResponseOmniscient = (chunk: string) => {
    onSimResponse(chunk);
  };

  const spec = { formatid: "gen9customgame" };

  const p1spec = {
    name: "Bot 1",
    team: Teams.pack(Teams.generate("gen9randombattle")),
  };
  const p2spec = {
    name: "Bot 2",
    team: Teams.pack(Teams.generate("gen9randombattle")),
  };

  const p1 = new RandomPlayerAI(showdownSim.streams.p1);
  const p2 = new RandomPlayerAI(showdownSim.streams.p2);

  void p1.start();
  void p2.start();

  void showdownSim.writeToOmniscient(`>start ${JSON.stringify(spec)}
>player p1 ${JSON.stringify(p1spec)}
>player p2 ${JSON.stringify(p2spec)}\n`);
}
