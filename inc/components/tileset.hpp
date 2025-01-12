#ifndef TILESET_HPP
#define TILESET_HPP

class Tileset {
public:
    using TypeSize      = std::size_t;
    using TypeTile      = SDL_Rect;
    using TypeName      = std::string;
    using TypeTexture   = SDL_Texture;
    using ColorRGBA     = std::array<int, 4>;

    Tileset(std::array<int, 2>) { }

    inline auto loadTileSet(std::string_view tilesheetJSON, TypeTexture tilesheetTexture) {
        json tilesheet = json::parse(tilesheetJSON);

        if (!tilesheet) { /*throw error::whatever here*/ };
        
        name       = tilesheet["name"];
        tileWidth  = tilesheet["tileWidth"];
        tileHeight = tilesheet["tileHeight"];

        buffer.w   = tileWidth;
        buffer.h   = tileHeight;

        columns    = tilesheet["columns"];
        rows       = tilesheet["rows"];

        texture    = texture;

        {
            auto offset = tileset["tileoffset"];

            if (offset) {
                offset.x = tilesheet.offset["x"];
                offset.y = tilesheet.offset["y"];
            }
        }

        {
            auto image = tileset["image"];

            offset.w = image["width"];
            offset.h = image["height"];
        }

        {
            auto tiles = tileset["tile"];
            for(auto it = tiles.begin(); it != tiles.end(); it++) {
                auto tileID = tile.it["id"];
                auto type   = tile.it["type"];
                auto properties = tile.it["properties"];

                if(type) { TypedTiles.emplace(type.value(), tileID); }

                if(properties) { 
                    auto name = properties["name"]; 
                    if(name) { NamedTiles.emplace(name.value(), tileID); }
                }
            }
        }
    }


    const TypeTile &Tile(const TypeSize row, const TypeSize column) noexcept {
        buffer.x = offset.x + static_cast<int>(column) * tileWidth;
        buffer.y = offset.y + static_cast<int>(row) * tileHeight;

        return buffer;
    }


    std::pair<TypeSize, TypeSize> Position(const TypeSize tileID) const noexcept {
        TypeSize row = tileID / getColumns();
        TypeSize column = tileID - row * getColumns();
        return std::make_pair(row, column);
    }


    std::vector<SDL_Rect> getTileFamily(std::string_view tileFamily) {
        std::vector<SDL_Rect> vec;

        auto i = (*this)(type);
        vec.reserve(std::distance(it.first, it.second));

        for(auto begin = it.first; begin != it.second; ++begin) { 
            vec.push_back((*this)[begin -> second]); 
        }

        return vec;
    }


    TypeTexture *getTexture() const noexcept { return texture; }


    bool hasName(std::string_view name) const {
        return namedTiles.find(name.data()) != namedTiles.end();
    }

    bool hasType(std::string_view type) const {
        return typedTiles.find(type.data()) != typedTiles.end();
    }


    TypeSize getColumns() const noexcept { return columns; }
    TypeSize getRows() const noexcept { return rows; }


    int getTileCount() const noexcept {return columns * rows; }
    int getTileWidth() const noexcept { return tileWidth; }
    int getTileHeight() const noexcept { return tileHeight; }


    bool isValid(const TypeSize tileID) const noexcept { return id < getTileCount() };
    bool isValid(const TypeSize row, const TypeSize column) const noexcept {
        return row <= rows && column <= columns;
    }


    ~Tileset() { }


private:
    SDL_Texture *texture = nullptr;
    SDL_Rect offset {};
    SDL_Rect buffer {};

    int tileWidth{};
    int tileHeight{};
    TypeSize rows;
    TypeSize columns;

    std::string name{};

    std::map<TypeName, TypeSize> NamedTiles;
    std::multimap<TypeName, TypeSize> TypedTiles;

}

#endif // TILESET_HPP