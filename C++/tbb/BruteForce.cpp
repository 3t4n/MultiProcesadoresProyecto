#include "BruteForce.h"
int main(int argc, unsigned char* argv[])
{
    unsigned char* prefixes;

    unsigned char hash[33] = "eba021d91b44a97dec2588bbea58a447";
    unsigned char password[MAX_LENGTH];
    prefixes = (unsigned char*)malloc(SIZEP * sizeof(char) * MAX_LENGTH);
    memset(prefixes, '\0', SIZEP * sizeof(unsigned char) * MAX_LENGTH);



    uint v1, v2, v3, v4;
    md5_to_ints(hash, &v1, &v2, &v3, &v4);
    for (int i = 0; i < CHARSETLEN; i++) prefixes[i * MAX_LENGTH] = charset[i];


    double ms;
    printf("Starting...\n");
    ms = 0;
    for (int h = 0; h < N; h++) {
        memset(password, '\0', sizeof(unsigned char) * MAX_LENGTH);
        long prefix_n = 26;
        int length = 5;
        long offset_start = 0;
        long offest_end = 26;

        start_timer();
        for (int i = 2; i <= length; i++) {
            prefix_n = prefix_n * CHARSETLEN;
            BruteForce obj(prefixes, i, offset_start, offest_end, prefix_n, v1, v2, v3, v4, password);
            parallel_for(blocked_range<int>(0, prefix_n), obj);

            if (password[0] != '\0')
            {
                printf("We found %s as %s  ", hash, password);
                break;
            }
            offset_start = offest_end;
            offest_end = offset_start + prefix_n;

        }
        ms += stop_timer();

    }
    printf("avg time = %.5lf ms\n", (ms / N));
    free(prefixes);
}

