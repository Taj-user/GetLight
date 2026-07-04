#pragma once

struct Color {
        int r, g, b;
};

struct Config {
        int x;
        int y;
        int height;
        int width;
        int opacity;
        Color color;

        bool load();
        bool save();
};
