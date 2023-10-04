#include <iostream>
#include <vector>
#include "psvm.hpp"

const std::vector<std::string> SIM_TEST_LINES = {
    ">start {\"formatid\":\"gen9customgame\"}",
    ">player p1 {\"name\":\"Bot 1\",\"team\":\"Samurott||AssaultVest|Torrent|knockoff,flipturn,grassknot,hydropump||85,85,85,85,85,85||||88|,,,,,Dark]Azumarill||SitrusBerry|HugePower|liquidation,aquajet,playrough,bellydrum||85,85,85,85,85,85||||82|,,,,,Water]Meowscarada||ChoiceBand|Protean|knockoff,playrough,flowertrick,uturn||85,85,85,85,85,85||||78|,,,,,Grass]Iron Valiant||LifeOrb|QuarkDrive|swordsdance,spiritbreak,knockoff,closecombat||85,85,85,85,85,85|N|||79|,,,,,Fighting]Clodsire||Leftovers|Unaware|recover,curse,earthquake,gunkshot||85,85,85,85,85,85||||81|,,,,,Ground]Ampharos||ChoiceSpecs|Static|focusblast,voltswitch,thunderbolt,dazzlinggleam||85,,85,85,85,85||,0,,,,||88|,,,,,Fairy\"}",
    ">player p2 {\"name\":\"Bot 2\",\"team\":\"Jolteon||Leftovers|VoltAbsorb|substitute,terablast,calmmind,thunderbolt||85,,85,85,85,85||,0,,,,||84|,,,,,Ice]Greedent||SitrusBerry|CheekPouch|bodyslam,earthquake,psychicfangs,swordsdance||85,85,85,85,85,85||||87|,,,,,Psychic]Lurantis||Leftovers|Contrary|knockoff,leafstorm,synthesis,superpower||85,85,85,85,85,85||||91|,,,,,Fighting]Polteageist||WhiteHerb|CursedBody|shadowball,storedpower,gigadrain,shellsmash||85,,85,85,85,85|N|,0,,,,||79|,,,,,Psychic]Gurdurr||Eviolite|Guts|drainpunch,knockoff,machpunch,bulkup||85,85,85,85,85,85||||85|,,,,,Steel]Glastrier||ChoiceBand|ChillingNeigh|iciclecrash,highhorsepower,closecombat,heavyslam||81,85,85,85,85,85|N|||86|,,,,,Fighting\"}",
    ">p1 default",
    ">p2 default",
    ">p1 default",
    ">p2 default",
    "aaa", // demo ignored input
    "aaa",
    "aaa",
    "aaa",
    ">forcetie"};

int main()
{
    ShowdownRuntime sr;

    for (auto i : SIM_TEST_LINES)
    {
        sr.insert(i);
    }

    sr.wait_for_child(); // dbg

    bool loop = true;
    while (loop)
    {
        auto val = sr.readResult();
        if (val.has_value())
        {
            std::string line = val.value();

            std::cout << line  << " [" << line.length() << "]\n\n";

            if (line == "|tie" || line.substr(0, 4) == "|win")
            {
                loop = false;
            }
        }
    }

    return 0;
}