valgrind --undef-value-errors=no --leak-check=full --track-fds=yes --time-stamp=yes --suppressions=suppresions ./solar_interface_win input.txt > valgrind_error_run 2>&1
