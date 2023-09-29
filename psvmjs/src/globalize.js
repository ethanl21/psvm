// Moves objects into the global scope so they can be
// compiled into bytecode and included in the binary

import * as psvmModule from "./psvm.js";

globalThis.psvm = psvmModule;
