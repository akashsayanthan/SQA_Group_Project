#ifndef GAME_H
#define GAME_H

#include <string>
#include <iomanip>
#include <sstream>

/**
 * A game available for distribution.
 *
 * Holds the game title and its price. Ownership and seller information
 * are not tracked here yet.
 */
class Game {
private:
    std::string title;  // Game title
    double price;       // Price of the game

public:
    /**
     * Creates a game with the given title and price.
     * @param t  game title
     * @param p  price
     */
    Game(const std::string& t, double p)
        : title(t), price(p) {}

    /** Creates an empty placeholder game (used by std::map default construction). */
    Game() : title(""), price(0.0) {}

    /** @return the game title. */
    std::string getTitle() const { return title; }

    /** @return the price as a number. */
    double getPrice() const { return price; }

    /**
     * Formats the price as it appears in the games file: exactly 6
     * characters, two decimal places, left-padded with zeros
     * (e.g. 12.5 -> "012.50").
     * @return the 6-character price string
     */
    std::string formatPrice() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << price;
        std::string result = oss.str();
        while (result.length() < 6) {
            result = "0" + result;
        }
        return result;
    }
};

#endif
