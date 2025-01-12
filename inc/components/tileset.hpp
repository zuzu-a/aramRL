#ifndef TILESET_HPP
#define TILESET_HPP

class TileSet {
public:
    using TypeSize      = std::size_t;
    using TypeTile      = SDL_Rect;
    using TypeName      = std::string;
    using TypeTexture   = SDL_Texture;

    TypeTexture loadTileSet(std::filesystem::path &tilesetImage, const std::array<int, 2> &dimensions) {
        

        return tilesheet;
    };


private:

}

#endif