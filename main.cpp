#include "DistributionSystem.h"

/**
 * Program entry point.
 *
 * Usage: distribution <accounts> <games> <collection> <transaction_out>
 *   accounts         accounts input file
 *   games            games input file 
 *   collection       game collection input file
 *   transaction_out  file the transaction log is written to on logout
 *
 * Transactions are read from standard input.
 *
 * @return 0 on success, 1 if the wrong number of arguments is given
 */
int main(int argc, char* argv[]) {
    if (argc != 5) {
        return 1;
    }

    DistributionSystem system(argv[1], argv[2], argv[3], argv[4]);
    system.run();

    return 0;
}
