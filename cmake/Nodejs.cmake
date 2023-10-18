# Find Node.js and npm
if (WIN32)
    find_program(NODE_EXECUTABLE
            NAMES node.exe
            HINTS "C:/Program Files/nodejs" "C:/Program Files (x86)/nodejs"
    )

    find_program(NPM_EXECUTABLE
            NAMES npm.cmd
            HINTS "C:/Program Files/nodejs" "C:/Program Files (x86)/nodejs"
    )

    # npm is a cmd file and not actually an executable
    if(${NPM_EXECUTABLE})
        set(${NPM_EXECUTABLE} "cmd ${NPM_EXECUTABLE}")
    endif ()
else ()
    find_program(NODE_EXECUTABLE
            NAMES node
            PATH_SUFFIXES bin
    )
    find_program(NPM_EXECUTABLE
            NAMES npm
            PATH_SUFFIXES bin
    )
endif ()

if (NOT NODE_EXECUTABLE)
    message(FATAL_ERROR "Node.js not found. Please make sure it is installed and specify the correct installation path.")
elseif (NOT NPM_EXECUTABLE)
    message(FATAL_ERROR "npm not found. Please make sure it is installed and specify the correct installation path.")
endif ()
