#pragma once

class Entity {
    public:
        Entity(std::size_t new_id);
        operator std::size_t() const;
};
