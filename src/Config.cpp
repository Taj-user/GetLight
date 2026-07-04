#include <fstream>
#include "Config.hpp"
#include "../vendor/nlohmann/json.hpp"

static void setDefaults(Config& cfg) {
        cfg.x           = 100;
        cfg.y           = 100;
        cfg.height      = 500;
        cfg.width       = 500;
        cfg.opacity     = 255;
        cfg.color       = { 255, 255, 255 };
}

using json = nlohmann::json;

bool Config::load() {
        std::ifstream file("config.json");
        if(!file.is_open()) {
                setDefaults(*this);
                return save();
        }

        try {
                json j;
                file >> j;

                x       = j["x"];
                y       = j["y"];
                height  = j["height"];
                width   = j["width"];
                opacity = j["opacity"];
                color   = {j["color"][0], j["color"][1], j["color"][2]};
        }
        catch (...){
                setDefaults(*this);
        }
        return true;
}

bool Config::save() {
        json j;

        j["x"]          = x;
        j["y"]          = y;
        j["height"]     = height;
        j["width"]      = width;
        j["opacity"]    = opacity;
        j["color"]      = {color.r, color.g, color.b};

        std::ofstream file("config.json");
        if(!file.is_open()) return false;

        file << j.dump(4);
        return true;
}
