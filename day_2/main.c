#include <stdio.h>
#include <stdlib.h>

// 1 = rock
// 2 = paper
// 3 = scissors
int get_winner(int p1, int p2)
{
    if (p1 == p2)
        return 2; // tie
    if ((p1 == 1 && p2 == 3) ||
        (p1 == 3 && p2 == 2) ||
        (p1 == 2 && p2 == 1))
        return 0; // p1 won
    return 1; // p2 won
}

int get_player(char p)
{
    if (p == 'A' || p == 'X')
        return 1;
    if (p == 'B' || p == 'Y')
        return 2;
    return 3;
}

int get_shape(int p1, int exp)
{
    if (exp == 2) // tie
        return p1;
    if (exp == 1)
    {
        if (p1 == 3)
            return 1;
        if (p1 == 2)
            return 3;
        return 2;
    }
    if (p1 == 1)
        return 3;
    if (p1 == 3)
        return 2;
    return 1;
}

// player 0 = A B C
// player 1 = X Y Z
int get_score(int symbol, int winner)
{
    if (winner == 0)
        return symbol;
    if (winner == 1)
        return symbol + 6;
    return symbol + 3;
}

int get_exp(char p)
{
    if (p == 'X')
        return 0;
    if (p == 'Y')
        return 2;
    return 1;
}

int main(void)
{

    FILE *f = fopen("input.txt", "r");
    if (!f)
        return 1;

    int sum = 0;

    size_t len = 0;
    char *line = NULL;

    while (getline(&line, &len, f) != -1)
    {
        int p1 = get_player(line[0]);
        int exp = get_exp(line[2]);

        int p2 = get_shape(p1, exp);

        int winner = get_winner(p1, p2);

        sum += get_score(p2, winner);
    }

    free(line);
    fclose(f);

    printf("res = %d\n", sum);

    return 0;
}
