#pragma once
#include <string>
#include <utility>
#include <memory>

enum { ITEM_NONE, ITEM_GOLD, ITEM_FLASHDRIVE, ITEM_LETTER, ITEM_BOOK, ITEM_COLLAR, ITEM_BOUQUET, ITEM_RAT, ITEM_PAINTING};
struct Item {
    int id;
    std::string name;
    std::string flavorText;

    Item() : id(ITEM_NONE), name("Empty"), flavorText("") {}

    Item(int id, std::string name, std::string flavor)
        : id(id), name(std::move(name)), flavorText(std::move(flavor)) {
    }

    Item(Item&& other) noexcept
        : id(other.id), name(std::move(other.name)), flavorText(std::move(other.flavorText)) {
        other.id = ITEM_NONE;
    }

    Item& operator=(Item&& other) noexcept {
        if (this != &other) {
            id = other.id;
            name = std::move(other.name);
            flavorText = std::move(other.flavorText);
            other.id = ITEM_NONE;
        }
        return *this;
    }

    Item(const Item&) = delete;
    Item& operator=(const Item&) = delete;
};

inline Item GenerateItem(int itemId) {
    switch (itemId) {
    case ITEM_GOLD:       return Item(ITEM_GOLD, "A sack of gold", "A small pouch with 10 pieces of gold.");
    case ITEM_PAINTING:       return Item(ITEM_PAINTING, "A Painting", "It's titled 'A Passing Ship.' by Reign Stumpen.");
    case ITEM_RAT:       return Item(ITEM_RAT, "Taxidermied Rat", "Something about the eyes.. it's unnerving.");
    case ITEM_BOUQUET:    return Item(ITEM_BOUQUET, "A bouquet of roses", "'Forgive me' is written on a card taped to the front. Smells lovely.");
    case ITEM_COLLAR:    return Item(ITEM_COLLAR, "A pet collar", "A tattered pet collar, inscribed with 'Emperor Jiji'");
    case ITEM_LETTER:     return Item(ITEM_LETTER, "Perfumed Letter", "'My dear Helene, if you're reading this, it means I have passed..");
    case ITEM_FLASHDRIVE: return Item(ITEM_FLASHDRIVE, "Flashdrive", "I wonder what's on this... It must be encrypted");
    case ITEM_BOOK:    return Item(ITEM_BOOK, "Worn Leather Diary", "Inside is a faded photo of two people on a bench, feeding pigeons.");
    default:              return Item();
    }
}