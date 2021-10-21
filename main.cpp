#include <iostream>
#include "subprocess.h"
#include <reproc++/run.hpp>
#include <reproc++/drain.hpp>
#include <reproc++/reproc.hpp>

int main(int argc, const char **argv) {

    reproc::options options;
    options.redirect.parent = true;
    reproc::process process;
    std::error_code ec = process.start(argv + 1, options);

    if (ec == std::errc::no_such_file_or_directory) {
        std::cerr << "Program not found. Make sure it's available from the PATH.";
        return ec.value();
    } else if (ec) {
        return ec.value();
    }


    return 0;

}
