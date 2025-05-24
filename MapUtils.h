#pragma once
#include <SFML/System/Vector2.hpp>
#include <functional>

namespace std {
    template <>
    struct hash<sf::Vector2i> {
        std::size_t operator()(const sf::Vector2i& v) const noexcept {
            std::size_t h1 = std::hash<int>()(v.x);
            std::size_t h2 = std::hash<int>()(v.y);
            return h1 ^ (h2 << 1); // or use boost::hash_combine logic
        }
    };
}
