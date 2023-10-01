import * as os from "os";

var inBuf = new Uint8Array(2048);
var decoder = new psvm.TextEncoding.TextDecoder("utf-8");

os.setReadHandler(host.readFd, () => {
  var bytes = os.read(host.readFd, inBuf.buffer, 0, 2048);
  var msg = decoder.decode(inBuf);

  var lines = msg.split("\0");
  lines.forEach((line) => {
    line = line.trim();
    line += "\n";
  });

  lines = lines.filter(function (entry) {
    return entry.trim() != "";
  });

  (async function loop() {
    for (let i = 0; i < lines.length; i++) {
      if (lines[i].startsWith(">")) {
        SimulatorInstance.writeToOmniscient(lines[i]);
      }
    }
  })();
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
    const encoder = new psvm.TextEncoding.TextEncoder();

    let line = SimulatorInstance.output.shift();
    let msg = encoder.encode(line + "\0");

    let bytes = os.write(host.writeFd, msg.buffer, 0, line.length + 1);

    if (line.startsWith("|win") || line.startsWith("|tie")) {
      // wait for the output buffer to empty
      waitForEmptyArray(SimulatorInstance.output).then(() => {
        os.setReadHandler(host.readFd, null);
        os.setWriteHandler(host.writeFd, null);
      });
    }
  }
});
