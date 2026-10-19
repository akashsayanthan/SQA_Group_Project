#include "DistributionSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>

/**
 * Stores the file paths. No files are opened here.
 */
DistributionSystem::DistributionSystem(const std::string& accounts, const std::string& games,
                                       const std::string& collection, const std::string& transaction)
    : currentUser(nullptr), accountsFile(accounts), gamesFile(games),
      collectionFile(collection), transactionFile(transaction) {}

/**
 * Main loop. Reads whitespace-separated tokens from standard input.
 * Each token is a transaction code, and the matching process* handler
 * reads any further input it needs (username, type, etc.) itself.
 * Ends when standard input is exhausted.
 */
void DistributionSystem::run() {
    std::string code;
    while (std::cin >> code) {
        std::cout << "Enter transaction:" << std::endl;

        if (code == "login") {
            processLogin();
        } else if (code == "logout") {
            processLogout();
        } else if (code == "create") {
            processCreate();
        } else if (code == "delete") {
            processDelete();
        } else if (code == "sell") {
            processSell();
        } else if (code == "buy") {
            processBuy();
        } else if (code == "refund") {
            processRefund();
        } else if (code == "addcredit") {
            processAddCredit();
        } else {
            std::cout << "Error: '" << code << "' is not a valid transaction." << std::endl;
        }
    }
}

/**
 * Reads the accounts file into the users map.
 *
 * Each line is 28 characters, fixed-width:
 *   columns 0-14   username (space-padded)
 *   column  15     space
 *   columns 16-17  type code
 *   column  18     space
 *   columns 19-27  credit balance
 * The line "END" marks the end of the data and is skipped, as are blank lines.
 * Any existing entries in users are overwritten.
 */
void DistributionSystem::readAccountsFile() {
    std::ifstream file(accountsFile);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line.find("END") == 0) continue;

        // Parse: username(15) + space + type(2) + space + credit(9)
        std::string username = line.substr(0, 15);
        std::string type = line.substr(16, 2);
        std::string creditStr = line.substr(19, 9);

        // Strip the space padding from the end of the username
        while (!username.empty() && username.back() == ' ') username.pop_back();

        double credit = std::stod(creditStr);
        users[username] = User(username, type, credit);
    }
    file.close();
}

/**
 * Writes every entry in transactionLog to transactionFile, one per line.
 * Overwrites the file if it already exists.
 */
void DistributionSystem::writeTransactionFile() {
    std::ofstream file(transactionFile);
    for (const auto& tx : transactionLog) {
        file << tx << std::endl;
    }
    file.close();
}

/**
 * Handles "login". Prompts for a username, reloads the accounts file,
 * and sets currentUser if the account exists. Fails if someone is
 * already logged in.
 */
void DistributionSystem::processLogin() {
    if (currentUser != nullptr) {
        std::cout << "Error: a user is already logged in." << std::endl;
        return;
    }

    std::cout << "Enter username:" << std::endl;
    std::string username;
    std::cin >> username;

    readAccountsFile();

    if (users.find(username) == users.end()) {
        std::cout << "Error: user '" << username << "' does not exist." << std::endl;
        return;
    }

    currentUser = &users[username];
    std::cout << "Login successful. Welcome, " << username << "." << std::endl;
}

/**
 * Handles "logout". Adds an end-of-session record ("00") holding the
 * user's final credit, writes the whole transaction log to the output
 * file, and clears the session (current user, loaded users, log).
 */
void DistributionSystem::processLogout() {
    if (currentUser == nullptr) {
        std::cout << "Error: you must log in first." << std::endl;
        return;
    }

    // Write end-of-session record
    std::string username = currentUser->getName();
    std::string type = currentUser->getType();
    std::string credit = currentUser->formatCredit();
    transactionLog.push_back("00 " + username + " " + type + " " + credit);

    writeTransactionFile();

    std::cout << "Logout successful." << std::endl;

    // Clear state
    currentUser = nullptr;
    users.clear();
    transactionLog.clear();
}

/**
 * Handles "create" (admin only). Prompts for a new username and type,
 * validates both, adds the user with zero credit, and logs a "01" record.
 *
 * Username rules: non-empty, at most 15 characters, not the reserved
 * word "END", and not already in use.
 * Type rules: must be AA, FS, BS or SS.
 */
void DistributionSystem::processCreate() {
    if (currentUser == nullptr) {
        std::cout << "Error: you must log in first." << std::endl;
        return;
    }

    if (currentUser->getType() != "AA") {
        std::cout << "Error: permission denied." << std::endl;
        return;
    }

    std::cout << "Enter new username:" << std::endl;
    std::string username;
    std::cin >> username;

    if (username.empty()) {
        std::cout << "Error: username cannot be empty." << std::endl;
        return;
    }

    if (username.length() > 15) {
        std::cout << "Error: username must be at most 15 characters." << std::endl;
        return;
    }

    if (username == "END") {
        std::cout << "Error: 'END' is a reserved name." << std::endl;
        return;
    }

    if (users.find(username) != users.end()) {
        std::cout << "Error: user '" << username << "' already exists." << std::endl;
        return;
    }

    std::cout << "Enter user type (AA, FS, BS, SS):" << std::endl;
    std::string type;
    std::cin >> type;

    if (type != "AA" && type != "FS" && type != "BS" && type != "SS") {
        std::cout << "Error: invalid user type." << std::endl;
        return;
    }

    users[username] = User(username, type, 0.0);
    transactionLog.push_back("01 " + username + " " + type + " 000000.00");

    std::cout << "User '" << username << "' created." << std::endl;
}

/**
 * Handles "delete" (admin only). Prompts for a username, removes that
 * user from the session, and logs a "02" record with the balance at the
 * time of deletion. An admin cannot delete their own account.
 */
void DistributionSystem::processDelete() {
    if (currentUser == nullptr) {
        std::cout << "Error: you must log in first." << std::endl;
        return;
    }

    if (currentUser->getType() != "AA") {
        std::cout << "Error: permission denied." << std::endl;
        return;
    }

    std::cout << "Enter username to delete:" << std::endl;
    std::string username;
    std::cin >> username;

    if (users.find(username) == users.end()) {
        std::cout << "Error: user '" << username << "' does not exist." << std::endl;
        return;
    }

    if (username == currentUser->getName()) {
        std::cout << "Error: you cannot delete your own account." << std::endl;
        return;
    }

    User& user = users[username];
    std::string type = user.getType();
    std::string credit = user.formatCredit();
    transactionLog.push_back("02 " + username + " " + type + " " + credit);

    users.erase(username);

    std::cout << "User '" << username << "' deleted." << std::endl;
}

/**
 * Placeholder for loading the games file. The 49-character line format
 * is not parsed yet, so this currently reads nothing into the games map.
 */
void DistributionSystem::readGamesFile() {
    std::ifstream file(gamesFile);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line.find("END") == 0) continue;
        // TODO: Parse 49-char game lines
    }
    file.close();
}

/**
 * Handles "sell". Not yet implemented; only checks that someone is logged in.
 */
void DistributionSystem::processSell() {
    if (currentUser == nullptr) {
        std::cout << "Error: you must log in first." << std::endl;
        return;
    }
    std::cout << "Error: sell not yet implemented." << std::endl;
}

/**
 * Handles "buy". Not yet implemented; only checks that someone is logged in.
 */
void DistributionSystem::processBuy() {
    if (currentUser == nullptr) {
        std::cout << "Error: you must log in first." << std::endl;
        return;
    }
    std::cout << "Error: buy not yet implemented." << std::endl;
}

/**
 * Handles "refund". Not yet implemented; only checks that someone is logged in.
 */
void DistributionSystem::processRefund() {
    if (currentUser == nullptr) {
        std::cout << "Error: you must log in first." << std::endl;
        return;
    }
    std::cout << "Error: refund not yet implemented." << std::endl;
}

/**
 * Handles "addcredit". Not yet implemented; only checks that someone is logged in.
 */
void DistributionSystem::processAddCredit() {
    if (currentUser == nullptr) {
        std::cout << "Error: you must log in first." << std::endl;
        return;
    }
    std::cout << "Error: addcredit not yet implemented." << std::endl;
}
