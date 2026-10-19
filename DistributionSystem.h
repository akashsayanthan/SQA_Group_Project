#ifndef DISTRIBUTIONSYSTEM_H
#define DISTRIBUTIONSYSTEM_H

#include "User.h"
#include "Game.h"
#include <map>
#include <vector>
#include <string>

/**
 * The main controller for the games distribution system.
 *
 * Reads transaction codes from standard input one at a time, dispatches
 * each to its handler, and keeps a session-scoped transaction log that is
 * written to a file on logout.
 *
 * Implemented in full: login, logout, create, delete.
 * Stubbed (prints "not yet implemented"): sell, buy, refund, addcredit.
 *
 * Only one user may be logged in at a time. Logging in loads the accounts
 * file; logging out writes the transaction log and clears the session.
 */
class DistributionSystem {
private:
    // --- Session state ---
    User* currentUser;                          // Logged-in user, or nullptr when logged out
    std::map<std::string, User> users;          // Accounts loaded at login, keyed by username
    std::map<std::string, Game> games;          // Games, keyed by title (not populated yet)
    std::vector<std::string> transactionLog;    // Records for the current session, in order

    // --- File paths supplied on the command line ---
    std::string accountsFile;                   // Accounts input file
    std::string gamesFile;                      // Games input file (not read yet)
    std::string collectionFile;                 // Collection input file (not read yet)
    std::string transactionFile;                // Output file for the transaction log

    /** Loads every account from accountsFile into the users map. */
    void readAccountsFile();

    /** Loads games from gamesFile into the games map. Currently a placeholder. */
    void readGamesFile();

    /** Overwrites transactionFile with the lines in transactionLog. */
    void writeTransactionFile();

    // --- Transaction handlers (one per transaction code) ---
    void processLogin();        // "login"     - start a session
    void processLogout();       // "logout"    - end a session and save the log
    void processCreate();       // "create"    - add a user (admin only)
    void processDelete();       // "delete"    - remove a user (admin only)
    void processSell();         // "sell"      - not yet implemented
    void processBuy();          // "buy"       - not yet implemented
    void processRefund();       // "refund"    - not yet implemented
    void processAddCredit();    // "addcredit" - not yet implemented

public:
    /**
     * Stores the file paths used by the system. Does not open any files;
     * files are read when they are needed.
     * @param accounts     path to the accounts file
     * @param games        path to the games file
     * @param collection   path to the game collection file
     * @param transaction  path where the transaction log is written on logout
     */
    DistributionSystem(const std::string& accounts, const std::string& games,
                      const std::string& collection, const std::string& transaction);

    /**
     * Runs the main loop: reads one transaction code at a time from
     * standard input until end of input, dispatching each to its handler.
     * Unknown codes print an error.
     */
    void run();
};

#endif
