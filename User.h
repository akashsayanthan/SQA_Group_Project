#ifndef USER_H
#define USER_H

#include <string>
#include <iomanip>
#include <sstream>

/**
 * A user account in the distribution system.
 *
 * Each account has a name (up to 15 characters), a type code that
 * controls what the user may do, and a credit balance.
 *
 * Type codes:
 *   AA - Administrator (may create and delete users)
 *   FS - Full standard (may buy and sell)
 *   BS - Buy standard  (may buy only)
 *   SS - Sell standard (may sell only)
 */
class User {
private:
    std::string name;   // Unique account name, max 15 chars
    std::string type;   // One of "AA", "FS", "BS", "SS"
    double credit;      // Current account balance

public:
    /**
     * Creates a user with the given name, type code, and credit balance.
     * @param n  account name
     * @param t  type code (AA, FS, BS or SS)
     * @param c  starting credit balance
     */
    User(const std::string& n, const std::string& t, double c)
        : name(n), type(t), credit(c) {}

    /** Creates an empty placeholder user (used by std::map default construction). */
    User() : name(""), type(""), credit(0.0) {}

    /** @return the account name. */
    std::string getName() const { return name; }

    /** @return the two-letter type code. */
    std::string getType() const { return type; }

    /** @return the credit balance as a number. */
    double getCredit() const { return credit; }

    /**
     * Formats the credit balance as it appears in the accounts and
     * transaction files: exactly 9 characters, two decimal places,
     * left-padded with zeros (e.g. 5000.0 -> "005000.00").
     * @return the 9-character credit string
     */
    std::string formatCredit() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << credit;
        std::string result = oss.str();
        while (result.length() < 9) {
            result = "0" + result;
        }
        return result;
    }
};

#endif
