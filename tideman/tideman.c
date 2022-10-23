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
bool is_a_cycle(int winner, bool tracker[]);

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
            ranks[rank] = i; // The candidate number i in the array is now placed in position j (rank) in the ranks array
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < (candidate_count - 1); i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i < j)
            {
                preferences[ranks[i]][ranks[j]] = preferences[ranks[i]][ranks[j]] + 1;
            }
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < (candidate_count - 1); i++)
    {
        for (int j = i; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int swaps;
    pair placeholder;

    // Bubble sort
    do
    {
        swaps = 0;
        for (int i = 0; i < (pair_count - 1); i++)
        {
            if ((preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]) < 
                (preferences[pairs[i + 1].winner][pairs[i + 1].loser] - preferences[pairs[i + 1].loser][pairs[i + 1].winner]))
            {
                placeholder = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = placeholder;
                swaps ++;
            }
        }
    }
    while (swaps != 0);
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Inicialize cycle-tracker
        bool tracker[candidate_count];
        for (int j = 0; j < candidate_count; j++)
        {
            tracker[j] = false;
        }

        // Make sure it doesnt create a cycle
        locked[pairs[i].winner][pairs[i].loser] = true;

        if (is_a_cycle(pairs[i].winner, tracker))
        {
            locked[pairs[i].winner][pairs[i].loser] = false;
        }
    }
}

// Finds out if it makes a cycle
bool is_a_cycle(int winner, bool tracker[])
{
    if (tracker[winner]) // If winner was already in the cycle
    {
        return true;
    }
    
    tracker[winner] = true;
    
    for (int loser = 0; loser < candidate_count; loser++)
    {
        if (locked[winner][loser] && is_a_cycle(loser, tracker)) // If the next loser closes the cycle
        {
            return true;
        }
    }
    return false; // If it has no loser
}

// Print the winner of the election
void print_winner(void)
{
    int edge;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                break;
            }
            if (j == (candidate_count - 1))
            {
                edge = i;
            }
        }
    }
    printf("%s", candidates[edge]);
    printf("\n");
}