// @ts-ignore
import {BattleStreams} from "@pkmn/sim";
import {ObjectReadWriteStream} from "@pkmn/streams";

export class ShowdownService {
    battle_dict: Map<string, ObjectReadWriteStream<string>>;

    constructor() {
        this.battle_dict = new Map();
    }

    startBattle(id: string) {
        // Delete the battle stream if it already exists
        this.battle_dict.delete(id);

        // Create the battle streams
        const streams = BattleStreams.getPlayerStreams(
            new BattleStreams.BattleStream()
        );

        // Add the omniscient stream to the streams dict
        this.battle_dict.set(id, streams.omniscient);

        // Write the output to the output function
        void (async (id) => {
            for await(const chunk of streams.omniscient) {
                // todo add actual fn here
                // @ts-ignore
                ResponseCallback(id, chunk);
            }
        })(id);
    }

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
