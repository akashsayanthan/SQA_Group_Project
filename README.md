# Games Distribution System - Front End

This is the front end for the games distribution system. It reads transaction
codes from standard input and runs them against account data loaded from files.

## Status

Implemented:

- login and logout, including loading accounts and writing the transaction log
- create (admin only)
- delete (admin only)
- Error messages for missing login, permission denied, invalid input, and unknown codes

Not implemented yet:

- sell, buy, refund, addcredit. Each prints "not yet implemented" after the login check.
- Parsing the games file and the collection file. The paths are accepted but not read.
- Credit changes from purchases or refunds.

## Files

- User.h: User class (name, type code, credit) and credit formatting
- Game.h: Game class (title, price) and price formatting
- DistributionSystem.h / DistributionSystem.cpp: transaction loop and handlers
- main.cpp: program entry point
- Makefile: build script
- DESIGN.md: design notes and file format details
- current_user_accounts.txt, available_games.txt, game_collection.txt: sample input files

## Build

    make

## Run

    ./distribution <accounts> <games> <collection> <transaction_out>

Example:

    ./distribution current_user_accounts.txt available_games.txt game_collection.txt out.txt < quick_test.txt

Transactions are read from standard input. The transaction log is written to the
last argument when the user logs out.

## Test Coverage

The login, logout, create, and delete paths were run end to end with the sample
files. The sell, buy, refund, and addcredit cases are not covered yet.
