#include <iostream>
#include <vector>
#include <psvm/psvm.hpp>

const std::vector<std::string> SIM_TEST_LINES = {
    R"(>start {"formatid":"gen9customgame"})",
    R"(>player p1 {"name":"Bot 1","team":"Samurott||AssaultVest|Torrent|knockoff,flipturn,grassknot,hydropump||85,85,85,85,85,85||||88|,,,,,Dark]Azumarill||SitrusBerry|HugePower|liquidation,aquajet,playrough,bellydrum||85,85,85,85,85,85||||82|,,,,,Water]Meowscarada||ChoiceBand|Protean|knockoff,playrough,flowertrick,uturn||85,85,85,85,85,85||||78|,,,,,Grass]Iron Valiant||LifeOrb|QuarkDrive|swordsdance,spiritbreak,knockoff,closecombat||85,85,85,85,85,85|N|||79|,,,,,Fighting]Clodsire||Leftovers|Unaware|recover,curse,earthquake,gunkshot||85,85,85,85,85,85||||81|,,,,,Ground]Ampharos||ChoiceSpecs|Static|focusblast,voltswitch,thunderbolt,dazzlinggleam||85,,85,85,85,85||,0,,,,||88|,,,,,Fairy"})",
    R"(>player p2 {"name":"Bot 2","team":"Jolteon||Leftovers|VoltAbsorb|substitute,terablast,calmmind,thunderbolt||85,,85,85,85,85||,0,,,,||84|,,,,,Ice]Greedent||SitrusBerry|CheekPouch|bodyslam,earthquake,psychicfangs,swordsdance||85,85,85,85,85,85||||87|,,,,,Psychic]Lurantis||Leftovers|Contrary|knockoff,leafstorm,synthesis,superpower||85,85,85,85,85,85||||91|,,,,,Fighting]Polteageist||WhiteHerb|CursedBody|shadowball,storedpower,gigadrain,shellsmash||85,,85,85,85,85|N|,0,,,,||79|,,,,,Psychic]Gurdurr||Eviolite|Guts|drainpunch,knockoff,machpunch,bulkup||85,85,85,85,85,85||||85|,,,,,Steel]Glastrier||ChoiceBand|ChillingNeigh|iciclecrash,highhorsepower,closecombat,heavyslam||81,85,85,85,85,85|N|||86|,,,,,Fighting"})",
};

int main() {
    try {
        // Create the service
        psvm::ShowdownService service;

        service.setCallback([](const std::string &id, const std::string &chunk) {
            std::cout << "Battle " << id << ": " << chunk << "\n";
        });

        // Start a battle
        std::string const battle_id_str = "battle_0";
        service.startBattle (battle_id_str);

        // Send some messages to the battle
        for(const auto& line : SIM_TEST_LINES) {
            service.writeToBattle(battle_id_str, line);
        }

        // Kill the battle
        service.killBattle(battle_id_str);
        // Optional: kill all active battles
        service.killAllBattles();

        std::cout << "Battle simulation completed successfully.\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
