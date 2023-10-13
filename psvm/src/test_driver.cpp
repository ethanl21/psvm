#include <iostream>
#include <vector>
#include "psvm.hpp"


const std::vector<std::string> SIM_TEST_LINES = {
        ">start {\"formatid\":\"gen9customgame\"}",
        ">player p1 {\"name\":\"Bot 1\",\"team\":\"Samurott||AssaultVest|Torrent|knockoff,flipturn,grassknot,hydropump||85,85,85,85,85,85||||88|,,,,,Dark]Azumarill||SitrusBerry|HugePower|liquidation,aquajet,playrough,bellydrum||85,85,85,85,85,85||||82|,,,,,Water]Meowscarada||ChoiceBand|Protean|knockoff,playrough,flowertrick,uturn||85,85,85,85,85,85||||78|,,,,,Grass]Iron Valiant||LifeOrb|QuarkDrive|swordsdance,spiritbreak,knockoff,closecombat||85,85,85,85,85,85|N|||79|,,,,,Fighting]Clodsire||Leftovers|Unaware|recover,curse,earthquake,gunkshot||85,85,85,85,85,85||||81|,,,,,Ground]Ampharos||ChoiceSpecs|Static|focusblast,voltswitch,thunderbolt,dazzlinggleam||85,,85,85,85,85||,0,,,,||88|,,,,,Fairy\"}",
        ">player p2 {\"name\":\"Bot 2\",\"team\":\"Jolteon||Leftovers|VoltAbsorb|substitute,terablast,calmmind,thunderbolt||85,,85,85,85,85||,0,,,,||84|,,,,,Ice]Greedent||SitrusBerry|CheekPouch|bodyslam,earthquake,psychicfangs,swordsdance||85,85,85,85,85,85||||87|,,,,,Psychic]Lurantis||Leftovers|Contrary|knockoff,leafstorm,synthesis,superpower||85,85,85,85,85,85||||91|,,,,,Fighting]Polteageist||WhiteHerb|CursedBody|shadowball,storedpower,gigadrain,shellsmash||85,,85,85,85,85|N|,0,,,,||79|,,,,,Psychic]Gurdurr||Eviolite|Guts|drainpunch,knockoff,machpunch,bulkup||85,85,85,85,85,85||||85|,,,,,Steel]Glastrier||ChoiceBand|ChillingNeigh|iciclecrash,highhorsepower,closecombat,heavyslam||81,85,85,85,85,85|N|||86|,,,,,Fighting\"}",
};


int main() {
    // Create a new ShowdownService
    ShowdownService ss;

    // Demo callback function, prints to stdout
    std::function<void(std::string, std::string)> callback = [](std::string id, std::string msg) {
        std::cout << "[" << id << "] " << msg << "\n";
    };
    ss.sim_resp_callback = callback;

    // Create a battle and store its id
    auto battle_id = ss.create_battle();

    // Write all the test lines to the battle stream
    for (const auto &line: SIM_TEST_LINES) {
        ss.write_to_battle(battle_id, line);
    }

    return 0;
}
