import * as os from "os";
import * as std from "std";

const inBuf = new Uint8Array(2048);
const encoder = new psvm.TextEncoding.TextEncoder();
const decoder = new psvm.TextEncoding.TextDecoder("utf-8");

os.setReadHandler(host.readFd, () => {
  const bytes = os.read(host.readFd, inBuf.buffer, 0, 2048);

    if (bytes > 0){
      let msg = decoder.decode(inBuf);

      const lines = msg.split("\0");
      const lines_trimmed = lines.map((line) => line.replaceAll("\0", ""));

      (async () => {
        for(const msg of lines_trimmed) {
          if (msg.startsWith(">")) {
            await SimulatorInstance.writeToOmniscient(msg);
          }
        }
      })();
    }
});

function waitForEmptyArray(arr) {
  return new Promise((resolve) => {
    const checkEmpty = () => {
      if (arr.length === 0) {
        resolve(); // Resolve the Promise when the array is empty
      } else {
        setTimeout(checkEmpty, 100); // Check again after a delay
      }
    };

    checkEmpty(); // Start checking immediately
  });
}

os.setWriteHandler(host.writeFd, () => {
  if (SimulatorInstance.output.length > 0) {
    let line = SimulatorInstance.output.shift();
    let msg = encoder.encode(line + '\0');

    let bytes = os.write(host.writeFd, msg.buffer, 0, msg.length);

    if (line.startsWith("|win") || line.startsWith("|tie")) {
      // wait for the output buffer to empty
      waitForEmptyArray(SimulatorInstance.output).then(() => {
        os.setReadHandler(host.readFd, null);
        os.setWriteHandler(host.writeFd, null);
        std.exit(0);
      });
    }
  }
});
