#pragma once

//Engine flags can be changed and accessed data, right now the only option is verbose logging
struct engine_flags {
    int verbose;
};

extern struct engine_flags ENGINE_FLAGS;

int start_engine(int width, int height, const char* title);
