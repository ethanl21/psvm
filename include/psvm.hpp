#ifndef PSVM_HPP
#define PSVM_HPP

#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <unistd.h>
#include "quickjs-libc.h"
#include "psvm-js.h"

const std::string CREATE_SIM_JS{"globalThis.SimulatorInstance = new psvm.ShowdownSimulator();"};
const std::string SETUP_SIM_JS{"import * as os from \"os\";\n"
 "import * as std from \"std\";\n"
 "\n"
 "const inBuf = new Uint8Array(2048);\n"
 "const encoder = new psvm.TextEncoding.TextEncoder();\n"
 "const decoder = new psvm.TextEncoding.TextDecoder(\"utf-8\");\n"
 "\n"
 "os.setReadHandler(host.readFd, () => {\n"
 "  const bytes = os.read(host.readFd, inBuf.buffer, 0, 2048);\n"
 "\n"
 "    if (bytes > 0){\n"
 "      let msg = decoder.decode(inBuf);\n"
 "\n"
 "      const lines = msg.split(\"\\0\");\n"
 "      const lines_trimmed = lines.map((line) => line.replaceAll(\"\\0\", \"\"));\n"
 "\n"
 "      (async () => {\n"
 "        for(const msg of lines_trimmed) {\n"
 "          if (msg.startsWith(\">\")) {\n"
 "            await SimulatorInstance.writeToOmniscient(msg);\n"
 "          }\n"
 "        }\n"
 "      })();\n"
 "    }\n"
 "});\n"
 "\n"
 "function waitForEmptyArray(arr) {\n"
 "  return new Promise((resolve) => {\n"
 "    const checkEmpty = () => {\n"
 "      if (arr.length === 0) {\n"
 "        resolve(); // Resolve the Promise when the array is empty\n"
 "      } else {\n"
 "        setTimeout(checkEmpty, 100); // Check again after a delay\n"
 "      }\n"
 "    };\n"
 "\n"
 "    checkEmpty(); // Start checking immediately\n"
 "  });\n"
 "}\n"
 "\n"
 "os.setWriteHandler(host.writeFd, () => {\n"
 "  if (SimulatorInstance.output.length > 0) {\n"
 "    let line = SimulatorInstance.output.shift();\n"
 "    let msg = encoder.encode(line + '\\0');\n"
 "\n"
 "    let bytes = os.write(host.writeFd, msg.buffer, 0, msg.length);\n"
 "\n"
 "    if (line.startsWith(\"|win\") || line.startsWith(\"|tie\")) {\n"
 "      // wait for the output buffer to empty\n"
 "      waitForEmptyArray(SimulatorInstance.output).then(() => {\n"
 "        os.setReadHandler(host.readFd, null);\n"
 "        os.setWriteHandler(host.writeFd, null);\n"
 "        std.exit(0);\n"
 "      });\n"
 "    }\n"
 "  }\n"
 "});"};



class ShowdownRuntime {
public:
    ShowdownRuntime();

    ~ShowdownRuntime();

    void insert(std::string &input);

    std::optional<std::string> readResult();

    std::optional<std::function<void()>> onSimulatorResponse;

    void wait_for_child();

    bool isParent;

    std::vector<std::string> output;

private:
    JSRuntime *rt;
    JSContext *ctx;

    JSContext *JS_NewCustomContext(JSRuntime *rt);

    int parent_to_child[2];
    int child_to_parent[2];
    pid_t child_pid;
};

#endif