#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "crow.h"

int main(void) {
    // delcare crow application
    crow::SimpleApp app; 

    // define endpoint at the root directory
    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    // set the port, set the app to run on multiple threads, and run the app
    app.port(18080).multithreaded().run();
}