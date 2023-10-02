#ifndef PSVM_HPP
#define PSVM_HPP

// TODO
//  add exceptions to handle errors
//  compile as shared library
//  use thread instead of fork to prevent methods from being called
//      in the simulator process
//  add demo CLI executable

#include <iostream>
#include <string>
#include <fstream>
#include <optional>
#include <unistd.h>
#include <signal.h>
#include "quickjs-libc.h"
#include "psvm-js.h"

const std::string CREATE_SIM_JS = "globalThis.SimulatorInstance = new psvm.ShowdownSimulator();";
const std::string SETUP_SIM_JS = "import * as os from \"os\";\r\nimport * as std from \"std\";\r\n\r\nvar inBuf = new Uint8Array(2048);\r\nvar decoder = new psvm.TextEncoding.TextDecoder(\"utf-8\");\r\n\r\nos.setReadHandler(host.readFd, () => {\r\n  var bytes = os.read(host.readFd, inBuf.buffer, 0, 2048);\r\n  var msg = decoder.decode(inBuf);\r\n\r\n  var lines = msg.split(\"\\0\");\r\n  lines.forEach((line) => {\r\n    line = line.trim();\r\n    line += \"\\n\";\r\n  });\r\n\r\n  lines = lines.filter(function (entry) {\r\n    return entry.trim() != \"\";\r\n  });\r\n\r\n  (async function loop() {\r\n    for (let i = 0; i < lines.length; i++) {\r\n      if (lines[i].startsWith(\">\")) {\r\n        SimulatorInstance.writeToOmniscient(lines[i]);\r\n      }\r\n    }\r\n  })();\r\n});\r\n\r\nfunction waitForEmptyArray(arr) {\r\n  return new Promise((resolve) => {\r\n    const checkEmpty = () => {\r\n      if (arr.length === 0) {\r\n        resolve(); // Resolve the Promise when the array is empty\r\n      } else {\r\n        setTimeout(checkEmpty, 100); // Check again after a delay\r\n      }\r\n    };\r\n\r\n    checkEmpty(); // Start checking immediately\r\n  });\r\n}\r\n\r\nos.setWriteHandler(host.writeFd, () => {\r\n  if (SimulatorInstance.output.length > 0) {\r\n    const encoder = new psvm.TextEncoding.TextEncoder();\r\n\r\n    let line = SimulatorInstance.output.shift();\r\n    let msg = encoder.encode(line + \"\\0\");\r\n\r\n    let bytes = os.write(host.writeFd, msg.buffer, 0, line.length + 1);\r\n\r\n    if (line.startsWith(\"|win\") || line.startsWith(\"|tie\")) {\r\n      // wait for the output buffer to empty\r\n      waitForEmptyArray(SimulatorInstance.output).then(() => {\r\n        os.setReadHandler(host.readFd, null);\r\n        os.setWriteHandler(host.writeFd, null);\r\n        std.exit(0);\r\n      });\r\n    }\r\n  }\r\n});\r\n";

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

class ShowdownRuntime
{
public:
    ShowdownRuntime();
    ~ShowdownRuntime();
    bool isParent;
    void insert(std::string &input);
    std::optional<std::string> readResult();
private:
    JSRuntime *rt;
    JSContext *ctx;
    JSContext *JS_NewCustomContext(JSRuntime *rt);
    int parent_to_child[2];
    int child_to_parent[2];
    pid_t child_pid;
};

#endif