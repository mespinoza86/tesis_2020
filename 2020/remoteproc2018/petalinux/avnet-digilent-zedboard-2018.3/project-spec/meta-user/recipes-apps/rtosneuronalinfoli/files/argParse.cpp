int argParse(int argc, char** argv,std::string &config,std::string &server, std::string &port) {

    CLI::App app{"App description"};

    app.add_option("-c,--config", config, "Config JSON file");
    app.add_option("-s,--server", server, "Server IP");
    app.add_option("-p,--port", port, "Communication Port");
    CLI11_PARSE(app, argc, argv);

    return 0;
}

