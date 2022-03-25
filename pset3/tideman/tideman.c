#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool check_cycle(int cur_pair, int fixed_loser, int loser, int flag);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = candidate_count - 1; i < j; j--)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
        printf("%i\n", preferences[1][2]);
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i; j < candidate_count; j++)
        {
            if (i == j)
            {
                continue;
            }
            else if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int swap_counter = -1;
    pair buffer;
    
    for (int i = 0; swap_counter != 0 && i < pair_count - 1; i++)
    {
        swap_counter = 0;
        
        for (int j = 0; j < pair_count - 1; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] < preferences[pairs[j + 1].winner][pairs[j + 1].loser])
            {
                buffer = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = buffer;
                swap_counter++;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    locked[pairs[0].winner][pairs[0].loser] = true;
    for (int i = 0; i < pair_count; i++)
    {
        if (check_cycle(i, pairs[i].loser, pairs[i].loser, 0))
        {
            continue;
        }
        else
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

bool check_cycle(int cur_pair, int fixed_loser, int loser, int flag)
{
    if (!locked[fixed_loser][loser] && flag == 1)
    {
        return true;
    }
    
    for (int i = 0; i <= cur_pair; i++)
    {
        if (locked[loser][pairs[i].loser])
        {
            return check_cycle(cur_pair, fixed_loser, pairs[i].loser, 1);
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    int flag = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        flag = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                flag = 1;
            }
        }
        if (flag != 1)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

