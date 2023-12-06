/****************************************************************************************************
 * Name: sliding_puzzle.c                                                                           *
 * File creation date: 2022-04-17                                                                   *
 * 1.0 date: 2022-07-10                                                                             *
 * Last modification date: 2022-09-09                                                               *
 * Author: Ryan Wells                                                                               *
 * Purpose: A sliding-block puzzle CLI videogame                                                    *
 * Further work: see notes at end of file                                                           *
 ****************************************************************************************************/

/* Preprocessing Directives (#include) */
#include <string.h> // for strcpy(), strcat(), strcmp(), and strlen()
#include <stdlib.h> // for exit(), rand(), srand(), and atoi()
#include <stdio.h> // for printf(), scanf(), getchar(), fopen(), fclose(), fseek(),
                   //    fgetc(), ftell(), fread(), fprintf(),
                   //    the macros "NULL", "EOF", and "SEEK_SET",
                   //    and the types "FILE" and "size_t"
#include <stdbool.h> // for the macros "bool", "true", and "false"
#include <time.h> // for time()
#include <ctype.h> // for isdigit() and tolower()

/* Preprocessing Directives (#define) */
#define ROW_WIDTH 38
#define CLEAR_CONSOLE (void) printf("\033[H\033[2J\033[3J"); // ANSI escapes for clearing screen and scrollback.
#define BLANK_LINE "|                                    |"
#define PANEL_TOP " ____________________________________ "
#define GAP "                                      "
#define MAX_LINE 1000
#define NUM_ROWS 12
#define FILE_PANEL_ALL_CHAR_NUM 4254
#define SELECTED_PANEL (panel_number == 0 ? panel0 : panel_number == 1 ? panel1 : panel_number == 2 ? panel2 \
: panel_number == 3 ? panel3 : panel_number == 4 ? panel4 : panel_number == 5 ? panel5 \
: panel_number == 6 ? panel6 : panel_number == 7 ? panel7 : panel8)

/* Type Definitions */
typedef struct Panel {
    /*
     * Position diagram:
     *  0 1 2
     *  3 4 5
     *  6 7 8
     */
    int position;
    bool is_gap;
    char row0[ROW_WIDTH + 1]; // "+ 1" is for the null character
    char row1[ROW_WIDTH + 1];
    char row2[ROW_WIDTH + 1];
    char row3[ROW_WIDTH + 1];
    char row4[ROW_WIDTH + 1];
    char row5[ROW_WIDTH + 1];
    char row6[ROW_WIDTH + 1];
    char row7[ROW_WIDTH + 1];
    char row8[ROW_WIDTH + 1];
    char row9[ROW_WIDTH + 1];
    char row10[ROW_WIDTH + 1];
    char row11[ROW_WIDTH + 1];
} Panel;

typedef struct Panel_Row {
    char row0[ROW_WIDTH * 3 + 1]; // "* 3" for three panels, "+ 1" for the null char
    char row1[ROW_WIDTH * 3 + 1];
    char row2[ROW_WIDTH * 3 + 1];
    char row3[ROW_WIDTH * 3 + 1];
    char row4[ROW_WIDTH * 3 + 1];
    char row5[ROW_WIDTH * 3 + 1];
    char row6[ROW_WIDTH * 3 + 1];
    char row7[ROW_WIDTH * 3 + 1];
    char row8[ROW_WIDTH * 3 + 1];
    char row9[ROW_WIDTH * 3 + 1];
    char row10[ROW_WIDTH * 3 + 1];
    char row11[ROW_WIDTH * 3 + 1];
} Panel_Row;

typedef struct Panel_Row_Plus_Side_Panel {
    char row0[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1]; // "+ 3 + ROW_WIDTH" is for a 3-space gap and the final panel out to the side
    char row1[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1];
    char row2[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1];
    char row3[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1];
    char row4[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1];
    char row5[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1];
    char row6[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1];
    char row7[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1];
    char row8[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1];
    char row9[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1];
    char row10[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1];
    char row11[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1];
} Panel_Row_Plus_Side_Panel;

typedef struct Panel_All {
    Panel_Row top;
    Panel_Row middle;
    Panel_Row bottom;
    char final_row[ROW_WIDTH * 3 + 1];
} Panel_All;

typedef struct Panel_All_Plus_Side_Panel {
    Panel_Row top;
    Panel_Row_Plus_Side_Panel middle;
    Panel_Row_Plus_Side_Panel bottom;
    char final_row[ROW_WIDTH * 3 + 3 + ROW_WIDTH + 1];
} Panel_All_Plus_Side_Panel;

/* Declarations of External Variables */
// none

/* Prototypes for non-main functions */
void play_game(FILE *picture_file, int selection);
bool check_formatting(FILE *picture_file, long *offset);
Panel_All store_picture_heart(Panel *panel0, Panel *panel1, Panel *panel2,
                              Panel *panel3, Panel *panel4, Panel *panel5,
                              Panel *panel6, Panel *panel7, Panel *panel8);
Panel blank_panel(void);
Panel_Row assemble_panel_row(Panel panel0, Panel panel1, Panel panel2);
Panel_All assemble_panel_all(Panel_Row top, Panel_Row middle, Panel_Row bottom);
Panel_All store_picture_from_file(FILE *picture_file, long *offset,
                                  Panel *panel0, Panel *panel1, Panel *panel2,
                                  Panel *panel3, Panel *panel4, Panel *panel5,
                                  Panel *panel6, Panel *panel7, Panel *panel8);
void print_panel_all(Panel_All pa);
void print_panel_row(Panel_Row pr);
Panel_All_Plus_Side_Panel scramble_puzzle(Panel *panel0, Panel *panel1, Panel *panel2,
                                          Panel *panel3, Panel *panel4, Panel *panel5,
                                          Panel *panel6, Panel *panel7, Panel *panel8,
                                          Panel_Row *top, Panel_Row *middle, Panel_Row *bottom,
                                          Panel_Row_Plus_Side_Panel *middle_and_side, Panel_Row_Plus_Side_Panel *bottom_and_side,
                                          Panel *final_piece_text, Panel *final_piece);
Panel flip_panel_over_x(Panel p);
Panel flip_panel_over_y(Panel p);
char *reverse(char *reversed_string_holder, char *string);
Panel_Row_Plus_Side_Panel add_side_panel(Panel_Row pr, Panel side_panel);
Panel_All_Plus_Side_Panel assemble_all_plus_side(Panel_Row top, Panel_Row_Plus_Side_Panel middle, Panel_Row_Plus_Side_Panel bottom, int gap);
void print_all_plus_side(Panel_All_Plus_Side_Panel paplus);
void print_row_plus_side(Panel_Row_Plus_Side_Panel prplus);
int read_line(char *input, int n);
bool parse_command(char *command, int n, Panel_All solution,
                   Panel *panel0, Panel *panel1, Panel *panel2,
                   Panel *panel3, Panel *panel4, Panel *panel5,
                   Panel *panel6, Panel *panel7, Panel *panel8,
                   bool *submit);
bool caseless_cmp(char str1[], char str2[]);
void print_command_listing(void);
void print_numbers(void);
bool caseless_skip_digit_cmp_no9(char str1[], char str2[]);
Panel_All_Plus_Side_Panel update_display(Panel *panel0, Panel *panel1, Panel *panel2,
                                         Panel *panel3, Panel *panel4, Panel *panel5,
                                         Panel *panel6, Panel *panel7, Panel *panel8,
                                         Panel_Row *top, Panel_Row *middle, Panel_Row *bottom,
                                         Panel_Row_Plus_Side_Panel *middle_and_side, Panel_Row_Plus_Side_Panel *bottom_and_side,
                                         Panel *final_piece_text, Panel *final_piece);
bool check_answer(Panel *panel0, Panel *panel1, Panel *panel2,
                  Panel *panel3, Panel *panel4, Panel *panel5,
                  Panel *panel6, Panel *panel7, Panel *panel8,
                  Panel *solution_panel0, Panel *solution_panel1, Panel *solution_panel2,
                  Panel *solution_panel3, Panel *solution_panel4, Panel *solution_panel5,
                  Panel *solution_panel6, Panel *solution_panel7, Panel *solution_panel8);
bool compare_panels(Panel a, Panel b);
void export_template(void);
Panel_All make_template(void);
int print_panel_all_to_file(FILE *filename, Panel_All pa);
int print_panel_row_to_file(FILE *filename, Panel_Row pr);

/* Definition of main */
/********************************************************************************************************
 * main():              Purpose: Run main menu loop, handle file opening/exporting, and run play_game() *
 *                      Parameters: none                                                                *
 *                      Return value: int                                                               *
 *                      Side effects: - prints to stdout                                                *
 *                                    - reads from stdin                                                *
 *                                    - terminates program                                              *
 *                                    - clears CLI screen and scrollback                                *
 *                                    - reads and writes external files                                 *
 ********************************************************************************************************/
int main(void)
{
    int selection;
    FILE *picture_file = NULL;
    char user_text[MAX_LINE] = {0};
    int fclose_return;

    // Main menu loop:
    do
    {
        do
        {
            CLEAR_CONSOLE;
            selection = 0; // Without this line, nonnumeric input would cause the selection from the previous loop to be rerun,
                           //       due to scanf()'s call ignoring nonnumeric input.
            (void) printf("Select a menu option:\n");
            (void) printf("\t1 = Play default puzzle\n\t2 = Load custom puzzle\n\t3 = Export custom puzzle template\n\t4 = Quit\n");
            (void) printf("SELECTION: ");
            (void) scanf("%d", &selection); while (getchar() != '\n');
        } while (selection < 1 || selection > 4);
        if (selection == 1)
        {
            picture_file = NULL;
            play_game(picture_file, selection);
            selection = 4; // exit program after game to prevent 2nd puzzle bug I haven't been able to solve
        }
        if (selection == 2)
        {
            // Prompt for custom file:
            (void) printf("Enter filename (including extension) of custom puzzle.\n");
            (void) printf("Custom puzzle file must be located in the same directory as this program.\n");
            (void) printf("FILENAME: ");
            (void) read_line(user_text, MAX_LINE + 1);

            // Test whether a file with given name exists:
            picture_file = fopen(user_text, "r");
            if (picture_file != NULL)
            {
                play_game(picture_file, selection);
                selection = 4; // exit program after game to prevent 2nd puzzle bug I haven't been able to solve
            }
            else
            {
                (void) printf("Unable to locate file. Please ensure file exists and is located in the same directory as this program.\n");
                (void) printf("\n\n----press ENTER----\n\n");
                while (getchar() != '\n');
            }
        }
        if (selection == 3)
            export_template();
    } while (selection != 4);

    CLEAR_CONSOLE;

    return 0;
}


/* Definitions of other functions */
/************************************************************************************************************************
 * play_game():         Purpose: Creates puzzle / stores puzzle in memory, runs game loop, runs winning sequence        *
 *                      Parameters: - FILE *picture_file --> pointer to the file containing the user's custom puzzle    *
 *                                      (or a NULL pointer if the user elected to play a default puzzle)                *
 *                                  - int selection --> value is either 1 (default puzzle) or 2 (custom puzzle)         *
 *                      Return value: none                                                                              *
 *                      Side effects: - prints to stdout                                                                *
 *                                    - reads from stdin                                                                *
 *                                    - terminates program                                                              *
 *                                    - clears CLI screen and scrollback                                                *
 *                                    - reads external files                                                            *
 ************************************************************************************************************************/
void play_game(FILE *picture_file, int selection)
{
    // Variable declarations:
    int default_picture;
    int fclose_return;
    Panel panel0, panel1, panel2, panel3, panel4, panel5, panel6, panel7, panel8;
    Panel solution_panel0, solution_panel1, solution_panel2,
          solution_panel3, solution_panel4, solution_panel5,
          solution_panel6, solution_panel7, solution_panel8;
    Panel_All solution;
    Panel_Row top, middle, bottom;
    Panel final_piece_text, final_piece;
    Panel_Row_Plus_Side_Panel middle_and_side;
    Panel_Row_Plus_Side_Panel bottom_and_side;
    Panel_All_Plus_Side_Panel display;
    bool unsolved = true;
    char command[MAX_LINE] = {0};
    int length; // Character length of user commands.
    bool valid;
    bool correct_format = false;
    bool submit = false;
    long offset;

    if (selection == 1)
    {
        do
        {
            CLEAR_CONSOLE;
            default_picture = 0; // Without this line, nonnumeric input would cause the selection from the previous loop to be rerun,
                                 //     due to scanf()'s call ignoring nonnumeric input.
            (void) printf("Which default puzzle would you like?\n");
            (void) printf("\t1 = HEART\n");
            (void) printf("SELECTION: ");
            (void) scanf("%d", &default_picture); while (getchar() != '\n');
        } while (default_picture != 1);
    }
    else if (selection == 2)
    {
        correct_format = check_formatting(picture_file, &offset);
        if (!correct_format)
        {
            CLEAR_CONSOLE;
            (void) printf("Error 3: Format error.\n");
            (void) printf("Specified file could not be read. Possible formatting error.\n");
            (void) printf("Ensure template instructions have been followed.\n");
            fclose_return = fclose(picture_file);
            if (fclose_return)
            {
                (void) printf("Error 4: Close error.\n");
                (void) printf("Specified file could not be closed correctly.\n");
                exit(4);
            }
            exit(3);
        }
        default_picture = 0; //prep for switch statement.
    }

    switch (default_picture)
    {
        case 1:
                solution = store_picture_heart(&panel0, &panel1, &panel2, &panel3, &panel4, &panel5, &panel6, &panel7, &panel8);
                break;
        default: // let default_picture be zero if the user loaded a file
            if (picture_file == NULL)
            {
                CLEAR_CONSOLE;
                (void) printf("Error 11: Unexpected switch case.\n");
                exit(11);
            }
            else
            {
                solution = store_picture_from_file(picture_file, &offset, &panel0, &panel1, &panel2, &panel3, &panel4, &panel5, &panel6, &panel7, &panel8);
                fclose_return = fclose(picture_file);
                if (fclose_return)
                {
                    CLEAR_CONSOLE;
                    (void) printf("Error 12: Close error after storing custom picture.\n");
                    (void) printf("Specified file could not be closed correctly.\n");
                    exit(12);
                }
                break;
            }
    }

    // Duplicate panels as solution panels to check against:
    solution_panel0 = panel0;
    solution_panel1 = panel1;
    solution_panel2 = panel2;
    solution_panel3 = panel3;
    solution_panel4 = panel4;
    solution_panel5 = panel5;
    solution_panel6 = panel6;
    solution_panel7 = panel7;

    CLEAR_CONSOLE;
    (void) printf("Solution:\n");
    print_panel_all(solution);
    (void) printf("\n\nPress ENTER to scramble puzzle and begin.\n\n");
    while (getchar() != '\n'); // Wait for Enter key.

    display = scramble_puzzle(&panel0, &panel1, &panel2,
                              &panel3, &panel4, &panel5,
                              &panel6, &panel7, &panel8,
                              &top, &middle, &bottom,
                              &middle_and_side, &bottom_and_side,
                              &final_piece_text, &final_piece);

    solution_panel8 = panel8; // This is here and not earlier because after scrambling, panel8 is just a gap panel--which is what the solution requires.

    // Main game loop:
    while (unsolved)
    {
        CLEAR_CONSOLE;
        (void) printf("Puzzle:\n");
        print_all_plus_side(display);
        (void) printf("\n\n");
        do
        {
            (void) printf("Enter command (\"help\" for help): ");
            length = read_line(command, MAX_LINE + 1);
            valid = parse_command(command, length, solution,
                                  &panel0, &panel1, &panel2,
                                  &panel3, &panel4, &panel5,
                                  &panel6, &panel7, &panel8,
                                  &submit);
        } while (!valid);
        display = update_display(&panel0, &panel1, &panel2,
                                 &panel3, &panel4, &panel5,
                                 &panel6, &panel7, &panel8,
                                 &top, &middle, &bottom,
                                 &middle_and_side, &bottom_and_side,
                                 &final_piece_text, &final_piece);
        if (submit)
        {
            unsolved = !check_answer(&panel0, &panel1, &panel2,
                                     &panel3, &panel4, &panel5,
                                     &panel6, &panel7, &panel8,
                                     &solution_panel0, &solution_panel1, &solution_panel2,
                                     &solution_panel3, &solution_panel4, &solution_panel5,
                                     &solution_panel6, &solution_panel7, &solution_panel8);
            submit = false;
            if (unsolved)
            {
                CLEAR_CONSOLE;
                (void) printf("\a");
                (void) printf("Submission incorrect. Press ENTER to keep playing.\n\n");
                while (getchar() != '\n');
            }
        }
    }

    // Winning sequence (from here to end of function):
    CLEAR_CONSOLE;
    (void) printf("\a\a\a");
    (void) printf(
           "************************************************************\n"
           "* __   __   ___    _   _    __        __  ___   _   _   _  *\n"
           "* \\ \\ / /  / _ \\  | | | |   \\ \\      / / |_ _| | \\ | | | | *\n"
           "*  \\ V /  | | | | | | | |    \\ \\ /\\ / /   | |  |  \\| | | | *\n"
           "*   | |   | |_| | | |_| |     \\ V  V /    | |  | |\\  | |_| *\n"
           "*   |_|    \\___/   \\___/       \\_/\\_/    |___| |_| \\_| (_) *\n"
           "************************************************************\n");

    (void) printf("\n\n\n\n\n----press ENTER----\n\n");
    while (getchar() != '\n');

    CLEAR_CONSOLE;
    (void) printf("Solution:\n");
    print_panel_all(solution);
    (void) printf("\n\n----press ENTER----\n\n");
    while (getchar() != '\n');

    CLEAR_CONSOLE;

    return;
}


/************************************************************************************************************************
 * check_formatting():  Purpose: Determines whether given file contains a validly formatted puzzle                      *
 *                      Parameters: - FILE *picture_file --> pointer to the file containing the user's custom puzzle    *
 *                                  - long *offset --> pointer to the variable in which to store the file offset        *
 *                                                          which indicates the beginning of the valid puzzle           *
 *                      Return value: bool --> true for validity, false for invalidity                                  *
 *                      Side effects: - reads external files                                                            *
 *                                    - alters external variable pointed to by "long *offset"                           *
 *                                    - prints to stdout                                                                *
 *                                    - terminates program                                                              *
 *                                    - clears CLI screen and scrollback                                                *
 ************************************************************************************************************************/
bool check_formatting(FILE *picture_file, long *offset)
{
    int fseek_return = 0;
    int fread_return = 0;
    int ch, i;
    bool found_topline = false;
    long told_pos = 0;
    bool correct_formatting = true;

    // topline_size is equal to the number of bytes in the top line of a panel row, minus the initial space and the automatic terminating null:
    size_t topline_size = (sizeof(PANEL_TOP PANEL_TOP PANEL_TOP) - sizeof(char) - sizeof(char));
    char potential_topline[(int) topline_size];
    char spaced_potential_topline[(int) topline_size + 1];

    // Ensure position indicator is at beginning of file:
    fseek_return = fseek(picture_file, 0, SEEK_SET);
    if (fseek_return)
    {
        CLEAR_CONSOLE;
        (void) printf("Error 5: File position indicator could not be set correctly.\n");
        exit(5);
    }

    // Find topline:
    do
    {
        while ((ch = fgetc(picture_file)) != ' ' && ch != EOF);
        if (ch == EOF)
        {
            CLEAR_CONSOLE;
            (void) printf("Error 6: EOF return while searching custom file. Possible formatting error.\n");
            (void) printf("Ensure template instructions have been followed.\n");
            exit(6);
        }
        told_pos = ftell(picture_file);
        if (told_pos == -1)
        {
            CLEAR_CONSOLE;
            (void) printf("Error 7: Failure to tell current file position.\n");
            exit(7);
        }
        fread_return = fread(potential_topline, topline_size, 1, picture_file);
        if (fread_return < 1)
        {
            CLEAR_CONSOLE;
            (void) printf("Error 8: Failure to read potential topline. Possible formatting error.\n");
            (void) printf("Ensure template instructions have been followed.\n");
            exit(8);
        }

        (void) strcat(strcpy(spaced_potential_topline, " "), potential_topline);


        if (strcmp(spaced_potential_topline, PANEL_TOP PANEL_TOP PANEL_TOP "\0"))
        {
            fseek_return = fseek(picture_file, told_pos, SEEK_SET);
            if (fseek_return)
            {
                CLEAR_CONSOLE;
                (void) printf("Error 9: File position indicator could not be set to previous position.\n");
                exit(9);
            }
        }
        else
        {
            found_topline = true;
            *offset = told_pos - (long) sizeof(char);
            fseek_return = fseek(picture_file, *offset, SEEK_SET);
            if (fseek_return)
            {
                CLEAR_CONSOLE;
                (void) printf("Error 10: File position indicator could not be set to adjusted position.\n");
                exit(10);
            }
        }
    } while (!found_topline);

    // Check formatting of panels, skipping interiors:
    for (i = 0, ch = fgetc(picture_file); i < FILE_PANEL_ALL_CHAR_NUM && ch != EOF; i++, ch = fgetc(picture_file))
    {
        // Check all needed spaces:
        if (i == 0 || i == 37 || i == 38 || i == 75 || i == 76 || i == 113 ||
            i == 1380 || i == 1417 || i == 1418 || i == 1455 || i == 1456 || i == 1493 ||
            i == 2760 || i == 2797 || i == 2798 || i == 2835 || i == 2836 || i == 2873 ||
            i == 4140 || i == 4177 || i == 4178 || i == 4215 || i == 4216 || i == 4253)
        {
            if (ch != ' ')
            {
                correct_formatting = false;
                break;
            }
        }
        // Check all needed underscores:
        if ((i > 0 && i < 37) || (i > 38 && i < 75) || (i > 76 && i < 113) ||
            (i > 1380 && i < 1417) || (i > 1418 && i < 1455) || (i > 1456 && i < 1493) ||
            (i > 2760 && i < 2797) || (i > 2798 && i < 2835) || (i > 2836 && i < 2873) ||
            (i > 4140 && i < 4177) || (i > 4178 && i < 4215) || (i > 4216 && i < 4253))
        {
            if (ch != '_')
            {
                correct_formatting = false;
                break;
            }
        }
        // Check all needed vertical bars:
        if (((i > 114 && i < 1380) || (i > 1494 && i < 2760) || (i > 2874 && i < 4140)) &&
                ((((i - 0) % 115) == 0) || (((i - 37) % 115) == 0) || (((i - 38) % 115) == 0) ||
                 (((i - 75) % 115) == 0) || (((i - 76) % 115) == 0) || (((i - 113) % 115) == 0)))
        {
            if (ch != '|')
            {
                correct_formatting = false;
                break;
            }
        }
    }

    return correct_formatting;
}

// Blank puzzle, for reference:
//  ____________________________________  ____________________________________  ____________________________________ 
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
//  ____________________________________  ____________________________________  ____________________________________ 
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
//  ____________________________________  ____________________________________  ____________________________________ 
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
// |                                    ||                                    ||                                    |
//  ____________________________________  ____________________________________  ____________________________________ 


/**************************************************************************************************************************************
 * store_picture_heart():   Purpose: Stores, in passed panel variable pointers, the strings making up the "heart" puzzle's graphics,  *
 *                                      and returns the combined, completed picture                                                   *
 *                          Parameters: - Panel *panel0 --> pointer to the variable containing the 0th panel                          *
 *                                      - Panel *panel1 --> pointer to the variable containing the 1st panel                          *
 *                                      - Panel *panel2 --> pointer to the variable containing the 2nd panel                          *
 *                                      - Panel *panel3 --> pointer to the variable containing the 3rd panel                          *
 *                                      - Panel *panel4 --> pointer to the variable containing the 4th panel                          *
 *                                      - Panel *panel5 --> pointer to the variable containing the 5th panel                          *
 *                                      - Panel *panel6 --> pointer to the variable containing the 6th panel                          *
 *                                      - Panel *panel7 --> pointer to the variable containing the 7th panel                          *
 *                                      - Panel *panel8 --> pointer to the variable containing the 8th panel                          *
 *                          Return value: Panel_All                                                                                   *
 *                          Side effects: - alters the panel variables pointed to by the parameters                                   *
 **************************************************************************************************************************************/
Panel_All store_picture_heart(Panel *panel0, Panel *panel1, Panel *panel2,
                              Panel *panel3, Panel *panel4, Panel *panel5,
                              Panel *panel6, Panel *panel7, Panel *panel8)
{
    *panel0 = (Panel) {
                        .row0 = PANEL_TOP "\0",
                        .row1 = "|                                 ** |\0",
                        .row2 = "|                               **   |\0",
                        .row3 = "|                             **     |\0",
                        .row4 = "|                           **       |\0",
                        .row5 = "|                         **         |\0",
                        .row6 = "|                       **           |\0",
                        .row7 = "|                     **             |\0",
                        .row8 = "|                   **               |\0",
                        .row9 = "|                 **                 |\0",
                        .row10 = "|                 **                 |\0",
                        .row11 = "|                 **                 |\0"
                      };
    
    *panel1 = (Panel) {
                        .row0 = PANEL_TOP "\0",
                        .row1 = "|***                              ***|\0",
                        .row2 = "|   ***                        ***   |\0",
                        .row3 = "|      **                    **      |\0",
                        .row4 = "|        **                **        |\0",
                        .row5 = "|          **            **          |\0",
                        .row6 = "|            **        **            |\0",
                        .row7 = "|              **    **              |\0",
                        .row8 = "|                *  *                |\0",
                        .row9 = "|                 **                 |\0",
                        .row10 = BLANK_LINE "\0",
                        .row11 = BLANK_LINE "\0",
                      };

    *panel2 = (Panel) {
                        .row0 = PANEL_TOP "\0",
                        .row1 = "| **                                 |\0",
                        .row2 = "|   **                               |\0",
                        .row3 = "|     **                             |\0",
                        .row4 = "|       **                           |\0",
                        .row5 = "|         **                         |\0",
                        .row6 = "|           **                       |\0",
                        .row7 = "|             **                     |\0",
                        .row8 = "|               **                   |\0",
                        .row9 = "|                 **                 |\0",
                        .row10 = "|                 **                 |\0",
                        .row11 = "|                 **                 |\0",
                      };

    *panel3 = (Panel) {
                        .row0 = PANEL_TOP "\0",
                        .row1 = "|                 **                 |\0",
                        .row2 = "|                 **                 |\0",
                        .row3 = "|                 **                 |\0",
                        .row4 = "|                 **                 |\0",
                        .row5 = "|                   **               |\0",
                        .row6 = "|                     **             |\0",
                        .row7 = "|                       **           |\0",
                        .row8 = "|                         **         |\0",
                        .row9 = "|                           **       |\0",
                        .row10 = "|                             **     |\0",
                        .row11 = "|                               **   |\0",
                      };

    *panel4 = blank_panel();

    *panel5 = (Panel) {
                        .row0 = PANEL_TOP "\0",
                        .row1 = "|                 **                 |\0",
                        .row2 = "|                 **                 |\0",
                        .row3 = "|                 **                 |\0",
                        .row4 = "|                 **                 |\0",
                        .row5 = "|               **                   |\0",
                        .row6 = "|             **                     |\0",
                        .row7 = "|           **                       |\0",
                        .row8 = "|         **                         |\0",
                        .row9 = "|       **                           |\0",
                        .row10 = "|     **                             |\0",
                        .row11 = "|   **                               |\0",
                      };

    *panel6 = (Panel) {
                        .row0 = PANEL_TOP "\0",
                        .row1 = "|                                   *|\0",
                        .row2 = BLANK_LINE "\0",
                        .row3 = BLANK_LINE "\0",
                        .row4 = BLANK_LINE "\0",
                        .row5 = BLANK_LINE "\0",
                        .row6 = BLANK_LINE "\0",
                        .row7 = BLANK_LINE "\0",
                        .row8 = BLANK_LINE "\0",
                        .row9 = BLANK_LINE "\0",
                        .row10 = BLANK_LINE "\0",
                        .row11 = BLANK_LINE "\0",
                      };

    *panel7 = (Panel) {
                        .row0 = PANEL_TOP "\0",
                        .row1 = BLANK_LINE "\0",
                        .row2 = "|*                                  *|\0",
                        .row3 = "| **                              ** |\0",
                        .row4 = "|   **                          **   |\0",
                        .row5 = "|     **                      **     |\0",
                        .row6 = "|       **                  **       |\0",
                        .row7 = "|         **              **         |\0",
                        .row8 = "|           **          **           |\0",
                        .row9 = "|             **      **             |\0",
                        .row10 = "|               **  **               |\0",
                        .row11 = "|                 **                 |\0",
                      };

    *panel8 = (Panel) {
                        .row0 = PANEL_TOP "\0",
                        .row1 = "|*                                   |\0",
                        .row2 = BLANK_LINE "\0",
                        .row3 = BLANK_LINE "\0",
                        .row4 = BLANK_LINE "\0",
                        .row5 = BLANK_LINE "\0",
                        .row6 = BLANK_LINE "\0",
                        .row7 = BLANK_LINE "\0",
                        .row8 = BLANK_LINE "\0",
                        .row9 = BLANK_LINE "\0",
                        .row10 = BLANK_LINE "\0",
                        .row11 = BLANK_LINE "\0",
                      };
    
    Panel_Row top = assemble_panel_row(*panel0, *panel1, *panel2);
    Panel_Row middle = assemble_panel_row(*panel3, *panel4, *panel5);
    Panel_Row bottom = assemble_panel_row(*panel6, *panel7, *panel8);

    return assemble_panel_all(top, middle, bottom);
}


/********************************************************************************************
 * blank_panel():    Purpose: Returns by value a panel variable with blanked out graphics   *
 *                   Parameters: none                                                       *
 *                   Return value: Panel                                                    *
 *                   Side effects: none                                                     *
 ********************************************************************************************/
Panel blank_panel(void)
{
    Panel p = {.row0 = PANEL_TOP "\0",
               .row1 = BLANK_LINE "\0",
               .row2 = BLANK_LINE "\0",
               .row3 = BLANK_LINE "\0",
               .row4 = BLANK_LINE "\0",
               .row5 = BLANK_LINE "\0",
               .row6 = BLANK_LINE "\0",
               .row7 = BLANK_LINE "\0",
               .row8 = BLANK_LINE "\0",
               .row9 = BLANK_LINE "\0",
               .row10 = BLANK_LINE "\0",
               .row11 = BLANK_LINE "\0"
              };

    return p;
}


/********************************************************************************************************************
 * assemble_panel_row():    Purpose: Takes 3 passed panel variables and assembles them into a 1x3 unit for return.  *
 *                          Parameters: - Panel panel0 --> value of the desired leftmost panel                      *
 *                                      - Panel panel1 --> value of the desired middle panel                        *
 *                                      - Panel panel2 --> value of the desired rightmost panel                     *
 *                          Return value: Panel_Row                                                                 *
 *                          Side effects: none                                                                      *
 ********************************************************************************************************************/
Panel_Row assemble_panel_row(Panel panel0, Panel panel1, Panel panel2)
{
    Panel_Row pr = {.row0 = *strcat(strcat(strcpy(pr.row0, panel0.row0), panel1.row0), panel2.row0),
                    .row1 = *strcat(strcat(strcpy(pr.row1, panel0.row1), panel1.row1), panel2.row1),
                    .row2 = *strcat(strcat(strcpy(pr.row2, panel0.row2), panel1.row2), panel2.row2),
                    .row3 = *strcat(strcat(strcpy(pr.row3, panel0.row3), panel1.row3), panel2.row3),
                    .row4 = *strcat(strcat(strcpy(pr.row4, panel0.row4), panel1.row4), panel2.row4),
                    .row5 = *strcat(strcat(strcpy(pr.row5, panel0.row5), panel1.row5), panel2.row5),
                    .row6 = *strcat(strcat(strcpy(pr.row6, panel0.row6), panel1.row6), panel2.row6),
                    .row7 = *strcat(strcat(strcpy(pr.row7, panel0.row7), panel1.row7), panel2.row7),
                    .row8 = *strcat(strcat(strcpy(pr.row8, panel0.row8), panel1.row8), panel2.row8),
                    .row9 = *strcat(strcat(strcpy(pr.row9, panel0.row9), panel1.row9), panel2.row9),
                    .row10 = *strcat(strcat(strcpy(pr.row10, panel0.row10), panel1.row10), panel2.row10),
                    .row11 = *strcat(strcat(strcpy(pr.row11, panel0.row11), panel1.row11), panel2.row11)
                   };
    return pr;
}


/****************************************************************************************************
 * assemble_panel_all():    Purpose: Takes 3 passed Panel_Rows combines them into a 9x9 set.        *
 *                          Parameters: - Panel_Row top --> the value of the desired top row        *
 *                                      - Panel_Row middle --> the value of the desired middle row  *
 *                                      - Panel_Row bottom --> the value of the desired bottom row  *
 *                          Return value: Panel_All                                                 *
 *                          Side effects: none                                                      *
 ****************************************************************************************************/
Panel_All assemble_panel_all(Panel_Row top, Panel_Row middle, Panel_Row bottom)
{
    Panel_All pa = {
                    .top = top,
                    .middle = middle,
                    .bottom = bottom,
                    .final_row = PANEL_TOP PANEL_TOP PANEL_TOP "\0"
                   };
    return pa;
}


/***************************************************************************************************************************************
 * store_picture_from_file():   Purpose: Stores, in passed panel variable pointers, the strings making up a custom puzzle's graphics,  *
 *                                          and returns the combined, completed picture                                                *
 *                              Parameters: - FILE *picture_file --> pointer to the file containing the user's custom puzzle           *
 *                                          - long *offset --> pointer to the variable containing the file offset                      *
 *                                                          which indicates the beginning of the custom puzzle                         *
 *                                          - Panel *panel0 --> pointer to the variable containing the 0th panel                       *
 *                                          - Panel *panel1 --> pointer to the variable containing the 1st panel                       *
 *                                          - Panel *panel2 --> pointer to the variable containing the 2nd panel                       *
 *                                          - Panel *panel3 --> pointer to the variable containing the 3rd panel                       *
 *                                          - Panel *panel4 --> pointer to the variable containing the 4th panel                       *
 *                                          - Panel *panel5 --> pointer to the variable containing the 5th panel                       *
 *                                          - Panel *panel6 --> pointer to the variable containing the 6th panel                       *
 *                                          - Panel *panel7 --> pointer to the variable containing the 7th panel                       *
 *                                          - Panel *panel8 --> pointer to the variable containing the 8th panel                       *
 *                              Return value: Panel_All                                                                                *
 *                              Side effects: - reads external files                                                                   *
 *                                            - alters the panel variables pointed to by the Panel * parameters                        *
 *                                            - prints to stdout                                                                       *
 *                                            - terminates program                                                                     *
 *                                            - clears CLI screen and scrollback                                                       *
 ***************************************************************************************************************************************/
Panel_All store_picture_from_file(FILE *picture_file, long *offset,
                                  Panel *panel0, Panel *panel1, Panel *panel2,
                                  Panel *panel3, Panel *panel4, Panel *panel5,
                                  Panel *panel6, Panel *panel7, Panel *panel8)
{
    size_t line_size = sizeof(PANEL_TOP) - sizeof(char); // subtracting the automatic terminating null
    long temp_offset;
    long told_pos = 0;
    int inner_counter = 0, outer_counter = 0;
    int fseek_return = 0;
    int fread_return = 0;
    char buffer[line_size];
    char *interlaced_rows[108] = {panel0->row0, panel1->row0, panel2->row0,
                                  panel0->row1, panel1->row1, panel2->row1,
                                  panel0->row2, panel1->row2, panel2->row2,
                                  panel0->row3, panel1->row3, panel2->row3,
                                  panel0->row4, panel1->row4, panel2->row4,
                                  panel0->row5, panel1->row5, panel2->row5,
                                  panel0->row6, panel1->row6, panel2->row6,
                                  panel0->row7, panel1->row7, panel2->row7,
                                  panel0->row8, panel1->row8, panel2->row8,
                                  panel0->row9, panel1->row9, panel2->row9,
                                  panel0->row10, panel1->row10, panel2->row10,
                                  panel0->row11, panel1->row11, panel2->row11,
                                  panel3->row0, panel4->row0, panel5->row0,
                                  panel3->row1, panel4->row1, panel5->row1,
                                  panel3->row2, panel4->row2, panel5->row2,
                                  panel3->row3, panel4->row3, panel5->row3,
                                  panel3->row4, panel4->row4, panel5->row4,
                                  panel3->row5, panel4->row5, panel5->row5,
                                  panel3->row6, panel4->row6, panel5->row6,
                                  panel3->row7, panel4->row7, panel5->row7,
                                  panel3->row8, panel4->row8, panel5->row8,
                                  panel3->row9, panel4->row9, panel5->row9,
                                  panel3->row10, panel4->row10, panel5->row10,
                                  panel3->row11, panel4->row11, panel5->row11,
                                  panel6->row0, panel7->row0, panel8->row0,
                                  panel6->row1, panel7->row1, panel8->row1,
                                  panel6->row2, panel7->row2, panel8->row2,
                                  panel6->row3, panel7->row3, panel8->row3,
                                  panel6->row4, panel7->row4, panel8->row4,
                                  panel6->row5, panel7->row5, panel8->row5,
                                  panel6->row6, panel7->row6, panel8->row6,
                                  panel6->row7, panel7->row7, panel8->row7,
                                  panel6->row8, panel7->row8, panel8->row8,
                                  panel6->row9, panel7->row9, panel8->row9,
                                  panel6->row10, panel7->row10, panel8->row10,
                                  panel6->row11, panel7->row11, panel8->row11};
    
    fseek_return = fseek(picture_file, *offset, SEEK_SET);
    if (fseek_return)
    {
        CLEAR_CONSOLE;
        (void) printf("Error 13: File position indicator could not be set in preparation for storing custom picture in memory.\n");
        exit(13);
    }

    while (outer_counter < 108)
    {
        // Skipping the new-lines present after every three buffer-lengths:
        if (inner_counter == 3)
        {
            inner_counter = 0;
            told_pos = ftell(picture_file);
            if (told_pos == -1)
            {
                CLEAR_CONSOLE;
                (void) printf("Error 14: Failure to tell current file position when skipping new-lines.\n");
                exit(14);
            }

            temp_offset = told_pos + (long) sizeof(char);

            fseek_return = fseek(picture_file, temp_offset, SEEK_SET);
            if (fseek_return)
            {
                CLEAR_CONSOLE;
                (void) printf("Error 15: File position indicator could not be set to adjusted position when skipping new-lines.\n");
                exit(15);
            }
        }

        fread_return = fread(buffer, line_size, 1, picture_file);
        if (fread_return < 1)
        {
            CLEAR_CONSOLE;
            (void) printf("Error 16: Failure to read into buffer.\n");
            exit(16);
        }

        (void) strcat(strcpy(interlaced_rows[outer_counter], buffer), "\0");

        inner_counter++;
        outer_counter++;
    }
    
    Panel_Row top = assemble_panel_row(*panel0, *panel1, *panel2);
    Panel_Row middle = assemble_panel_row(*panel3, *panel4, *panel5);
    Panel_Row bottom = assemble_panel_row(*panel6, *panel7, *panel8);

    return assemble_panel_all(top, middle, bottom);
}


/*************************************************************************************
 * print_panel_all():   Purpose: Receives a Panel_All and prints it to the screen.   *
 *                      Parameters: - Panel_All pa --> the Panel_All to be printed.  *
 *                      Return value: none                                           *
 *                      Side effects: - prints to stdout                             *
 *************************************************************************************/
void print_panel_all(Panel_All pa)
{
    print_panel_row(pa.top);
    print_panel_row(pa.middle);
    print_panel_row(pa.bottom);
    (void) printf("%s\n", pa.final_row);

    return;
}


/*************************************************************************************
 * print_panel_row():   Purpose: Receives a Panel_Row and prints it to the screen.   *
 *                      Parameters: - Panel_Row pr --> the Panel_Row to be printed.  *
 *                      Return value: none                                           *
 *                      Side effects: - prints to stdout                             *
 *************************************************************************************/
void print_panel_row(Panel_Row pr)
{
    (void) printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", pr.row0, pr.row1, pr.row2, pr.row3, pr.row4, pr.row5, pr.row6, pr.row7, pr.row8, pr.row9, pr.row10, pr.row11);
    return;
}


/***************************************************************************************************************************************
 * scramble_puzzle():   Purpose: Receives, in passed panel variable pointers, the strings making up a puzzle's graphics,               *
 *                                 randomly scrambles which panels go in which variables, randomly flips panels horizontally or        *
 *                                 vertically, stores the newly-scrambled panels in the original variables, stores the sidebar         *
 *                                 graphics in the passed pointers to final_piece and final_piece_text, assembles the puzzle into      *
 *                                 row pointers, and returns the combined, completed, scrambled puzzle for display.                    *
 *                      Parameters: - Panel *panel0 --> pointer to the variable containing the 0th panel                               *
 *                                  - Panel *panel1 --> pointer to the variable containing the 1st panel                               *
 *                                  - Panel *panel2 --> pointer to the variable containing the 2nd panel                               *
 *                                  - Panel *panel3 --> pointer to the variable containing the 3rd panel                               *
 *                                  - Panel *panel4 --> pointer to the variable containing the 4th panel                               *
 *                                  - Panel *panel5 --> pointer to the variable containing the 5th panel                               *
 *                                  - Panel *panel6 --> pointer to the variable containing the 6th panel                               *
 *                                  - Panel *panel7 --> pointer to the variable containing the 7th panel                               *
 *                                  - Panel *panel8 --> pointer to the variable containing the 8th panel                               *
 *                                  - Panel_Row *top --> pointer to the variable for storing the post-scramble top three panels        *
 *                                  - Panel_Row *middle --> pointer to the variable for storing the post-scramble middle three panels  *
 *                                  - Panel_Row *bottom --> pointer to the variable for storing the post-scramble bottom three panels  *
 *                                  - Panel_Row_Plus_Side_Panel *middle_and_side --> pointer to the variable for storing               *
 *                                      the post-scramble middle three panels plus the top portion of the sidebar graphics             *
 *                                  - Panel_Row_Plus_Side_Panel *bottom_and_side --> pointer to the variable for storing               *
 *                                      the post-scramble bottom three panels plus the bottom portion of the sidebar graphics          *
 *                                  - Panel *final_piece_text --> pointer to the variable for storing the top portion of the sidebar   *
 *                                  - Panel *final_piece --> pointer to the variable for storing the bottom portion of the sidebar     *
 *                      Return value: Panel_All_Plus_Side_Panel                                                                        *
 *                      Side effects: - alters the variables pointed to by every single parameter                                      *
 ***************************************************************************************************************************************/
Panel_All_Plus_Side_Panel scramble_puzzle(Panel *panel0, Panel *panel1, Panel *panel2,
                                          Panel *panel3, Panel *panel4, Panel *panel5,
                                          Panel *panel6, Panel *panel7, Panel *panel8,
                                          Panel_Row *top, Panel_Row *middle, Panel_Row *bottom,
                                          Panel_Row_Plus_Side_Panel *middle_and_side, Panel_Row_Plus_Side_Panel *bottom_and_side,
                                          Panel *final_piece_text, Panel *final_piece)
{
    int new_position;
    bool position_options[8] = {true, true, true, true, true, true, true, true}; // Array represents whether a position (0-7) is available.
    bool should_flip_over_x = false, should_flip_over_y = false;
    Panel *panel_set[8] = {panel0, panel1, panel2, panel3, panel4, panel5, panel6, panel7};
    Panel assembly_panel0, assembly_panel1, assembly_panel2,
          assembly_panel3, assembly_panel4, assembly_panel5,
          assembly_panel6, assembly_panel7, assembly_panel8;

    // Creating the display's side panel:
    *final_piece = *panel8;
    *final_piece_text = (Panel) {
                                    .row0 = GAP "\0",
                                    .row1 = GAP "\0",
                                    .row2 = GAP "\0",
                                    .row3 = GAP "\0",
                                    .row4 = GAP "\0",
                                    .row5 = GAP "\0",
                                    .row6 = GAP "\0",
                                    .row7 = GAP "\0",
                                    .row8 = GAP "\0",
                                    .row9 = GAP "\0",
                                    .row10 = GAP "\0",
                                    .row11 = "  Final Piece:                        \0"
                                };

    // Seeding rand():
    srand(time(NULL));

    // Randomizing panel positions:
    for (int i = 0; i < 8; i++)
    {
        do
        {
            new_position = rand() % (9 - 1); // 9 positions, but the 9th (position 8) is where the blank space will be, so it's invalid.
        } while (position_options[new_position] == false); // Select new unused position.
        position_options[new_position] = false; // Mark position as used.
        panel_set[i]->position = new_position; // Assign position to panel.
    }

    // Randomizing panel orientations:
    for (int i = 0; i < 8; i++)
    {
        should_flip_over_x = rand() % 2;
        if (should_flip_over_x)
            *panel_set[i] = flip_panel_over_x(*panel_set[i]);
        should_flip_over_y = rand() % 2;
        if (should_flip_over_y)
            *panel_set[i] = flip_panel_over_y(*panel_set[i]);
    }

    // Loop to rename (through duplication) panel variables for assembly.
    for (int i = 0; i < 8; i++)
    {
        if (panel_set[i]->position == 0)
            assembly_panel0 = *panel_set[i];
        else if (panel_set[i]->position == 1)
            assembly_panel1 = *panel_set[i];
        else if (panel_set[i]->position == 2)
            assembly_panel2 = *panel_set[i];
        else if (panel_set[i]->position == 3)
            assembly_panel3 = *panel_set[i];
        else if (panel_set[i]->position == 4)
            assembly_panel4 = *panel_set[i];
        else if (panel_set[i]->position == 5)
            assembly_panel5 = *panel_set[i];
        else if (panel_set[i]->position == 6)
            assembly_panel6 = *panel_set[i];
        else
            assembly_panel7 = *panel_set[i];
    }
    assembly_panel8 = (Panel) {
                                .position = 8,
                                .row0 = PANEL_TOP "\0",
                                .row1 = GAP "\0",
                                .row2 = GAP "\0",
                                .row3 = GAP "\0",
                                .row4 = GAP "\0",
                                .row5 = GAP "\0",
                                .row6 = GAP "\0",
                                .row7 = GAP "\0",
                                .row8 = GAP "\0",
                                .row9 = GAP "\0",
                                .row10 = GAP "\0",
                                .row11 = GAP "\0"
                              }; //  assembly_panel8 is just a blank space (plus a top line, which serves as the bottom for the panel above it),
                                 //     because it's the 'gap' used to slide other panels into.
    
    // This assignment section updates the actual panel variables with the scrambled versions to allow the player to manipulate them during play:
    *panel0 = assembly_panel0;
    *panel1 = assembly_panel1;
    *panel2 = assembly_panel2;
    *panel3 = assembly_panel3;
    *panel4 = assembly_panel4;
    *panel5 = assembly_panel5;
    *panel6 = assembly_panel6;
    *panel7 = assembly_panel7;
    *panel8 = assembly_panel8;

    panel0->is_gap = false;
    panel1->is_gap = false;
    panel2->is_gap = false;
    panel3->is_gap = false;
    panel4->is_gap = false;
    panel5->is_gap = false;
    panel6->is_gap = false;
    panel7->is_gap = false;
    panel8->is_gap = true;

    // Assemble and return:
    *top = assemble_panel_row(assembly_panel0, assembly_panel1, assembly_panel2);
    *middle = assemble_panel_row(assembly_panel3, assembly_panel4, assembly_panel5);
    *bottom = assemble_panel_row(assembly_panel6, assembly_panel7, assembly_panel8);
    *middle_and_side = add_side_panel(*middle, *final_piece_text);
    *bottom_and_side = add_side_panel(*bottom, *final_piece);

    return assemble_all_plus_side(*top, *middle_and_side, *bottom_and_side, 8);
}


/*****************************************************************************************************************************
 * flip_panel_over_x():   Purpose: Flips a panel vertically (meaning, 'across the x-axis') and returns the flipped version.  *
 *                        Parameters: - Panel p --> a passed-by-value copy of the panel to be flipped                        *
 *                        Return value: Panel                                                                                *
 *                        Side effects: none                                                                                 *
 *****************************************************************************************************************************/
Panel flip_panel_over_x(Panel p)
{
    Panel flipped_p = blank_panel();
    flipped_p.position = p.position;
    flipped_p.is_gap = p.is_gap;
    (void) strcpy(flipped_p.row1, p.row11);
    (void) strcpy(flipped_p.row2, p.row10);
    (void) strcpy(flipped_p.row3, p.row9);
    (void) strcpy(flipped_p.row4, p.row8);
    (void) strcpy(flipped_p.row5, p.row7);
    (void) strcpy(flipped_p.row6, p.row6);
    (void) strcpy(flipped_p.row7, p.row5);
    (void) strcpy(flipped_p.row8, p.row4);
    (void) strcpy(flipped_p.row9, p.row3);
    (void) strcpy(flipped_p.row10, p.row2);
    (void) strcpy(flipped_p.row11, p.row1);
    return flipped_p;
}


/*******************************************************************************************************************************
 * flip_panel_over_y():   Purpose: Flips a panel horizontally (meaning, 'across the y-axis') and returns the flipped version.  *
 *                        Parameters: - Panel p --> a passed-by-value copy of the panel to be flipped                          *
 *                        Return value: Panel                                                                                  *
 *                        Side effects: none                                                                                   *
 *******************************************************************************************************************************/
Panel flip_panel_over_y(Panel p) // flips a panel horizontally (meaning, 'across the y-axis')
{
    Panel flipped_p = blank_panel();
    char reversed[ROW_WIDTH + 1] = {0};
    flipped_p.position = p.position;
    flipped_p.is_gap = p.is_gap;
    (void) strcpy(flipped_p.row1, reverse(reversed, p.row1));
    (void) strcpy(flipped_p.row2, reverse(reversed, p.row2));
    (void) strcpy(flipped_p.row3, reverse(reversed, p.row3));
    (void) strcpy(flipped_p.row4, reverse(reversed, p.row4));
    (void) strcpy(flipped_p.row5, reverse(reversed, p.row5));
    (void) strcpy(flipped_p.row6, reverse(reversed, p.row6));
    (void) strcpy(flipped_p.row7, reverse(reversed, p.row7));
    (void) strcpy(flipped_p.row8, reverse(reversed, p.row8));
    (void) strcpy(flipped_p.row9, reverse(reversed, p.row9));
    (void) strcpy(flipped_p.row10, reverse(reversed, p.row10));
    (void) strcpy(flipped_p.row11, reverse(reversed, p.row11));
    return flipped_p;
}


/*******************************************************************************************************************************************
 * reverse():   Purpose: Reverses a passed string and returns a pointer to the reverse (like how strcpy() returns a pointer to the copy).  *
 *              Parameters: - char *reversed_string_holder --> a pointer to the array to hold the reversed string                          *
 *                          - char *string --> a pointer to the string to be reversed                                                      *
 *              Return value: char *                                                                                                       *
 *              Side effects: - alters the variable pointed to by char *reversed_string_holder                                             *
 *******************************************************************************************************************************************/
char *reverse(char *reversed_string_holder, char *string)
{
    int length = strlen(string);
    for (int i = 0; i < length; i++)
    {
        reversed_string_holder[i] = string[(length - 1) - i];
    }
    reversed_string_holder[length] = '\0';
    return reversed_string_holder;
}


/**************************************************************************************************************************************************
 * add_side_panel():   Purpose: Accepts a Panel_Row and a desired extra Panel, and appends the extra Panel to the Panel_Row with a gap inbetween. *
 *                     Parameters: - Panel_Row pr --> the Panel_Row to be added to                                                                *
 *                                 - Panel side_panel --> the Panel to be appended                                                                *
 *                     Return value: Panel_Row_Plus_Side_Panel                                                                                    *
 *                     Side effects: none                                                                                                         *
 **************************************************************************************************************************************************/
Panel_Row_Plus_Side_Panel add_side_panel(Panel_Row pr, Panel side_panel)
{
    Panel_Row_Plus_Side_Panel prplus = {
                                        .row0 = *strcat(strcat(strcpy(prplus.row0, pr.row0), "   "), side_panel.row0),
                                        .row1 = *strcat(strcat(strcpy(prplus.row1, pr.row1), "   "), side_panel.row1),
                                        .row2 = *strcat(strcat(strcpy(prplus.row2, pr.row2), "   "), side_panel.row2),
                                        .row3 = *strcat(strcat(strcpy(prplus.row3, pr.row3), "   "), side_panel.row3),
                                        .row4 = *strcat(strcat(strcpy(prplus.row4, pr.row4), "   "), side_panel.row4),
                                        .row5 = *strcat(strcat(strcpy(prplus.row5, pr.row5), "   "), side_panel.row5),
                                        .row6 = *strcat(strcat(strcpy(prplus.row6, pr.row6), "   "), side_panel.row6),
                                        .row7 = *strcat(strcat(strcpy(prplus.row7, pr.row7), "   "), side_panel.row7),
                                        .row8 = *strcat(strcat(strcpy(prplus.row8, pr.row8), "   "), side_panel.row8),
                                        .row9 = *strcat(strcat(strcpy(prplus.row9, pr.row9), "   "), side_panel.row9),
                                        .row10 = *strcat(strcat(strcpy(prplus.row10, pr.row10), "   "), side_panel.row10),
                                        .row11 = *strcat(strcat(strcpy(prplus.row11, pr.row11), "   "), side_panel.row11)
                                       };
    return prplus;
}


/**************************************************************************************************************************************************
 * asssemble_all_plus_side():   Purpose: assembles a Panel_Row variable and two Panel_Row_Plus_Side_Panel variables into a single unit.           *
 *                              Parameters: - Panel_Row top --> the Panel_Row intended to go on top                                               *
 *                                          - Panel_Row_Plus_Side_Panel middle --> the Panel_Row intended to go in the middle, plus a sidebar     *
 *                                          - Panel_Row_Plus_Side_Panel bottom --> the Panel_Row intended to go on the bottom, plus a sidebar     *
 *                                          - int gap --> the current position of the gap in the puzzle (needed to choose the correct final line) *
 *                     Return value: Panel_All_Plus_Side_Panel                                                                                    *
 *                     Side effects: none                                                                                                         *
 **************************************************************************************************************************************************/
Panel_All_Plus_Side_Panel assemble_all_plus_side(Panel_Row top, Panel_Row_Plus_Side_Panel middle, Panel_Row_Plus_Side_Panel bottom, int gap)
{
    Panel_All_Plus_Side_Panel paplus = {
                                        .top = top,
                                        .middle = middle,
                                        .bottom = bottom
                                       };
    if (gap == 6)
        (void) strcpy(paplus.final_row, GAP PANEL_TOP PANEL_TOP "   " PANEL_TOP "\0");
    else if (gap == 7)
        (void) strcpy(paplus.final_row, PANEL_TOP GAP PANEL_TOP "   " PANEL_TOP "\0");
    else if (gap == 8)
        (void) strcpy(paplus.final_row, PANEL_TOP PANEL_TOP GAP "   " PANEL_TOP "\0");
    else
        (void) strcpy(paplus.final_row, PANEL_TOP PANEL_TOP PANEL_TOP "   " PANEL_TOP "\0");
    return paplus;
}


/****************************************************************************************************************************
 * print_all_plus_side():   Purpose: prints a Panel_All_Plus_Side_Panel to the screen                                       *
 *                          Parameters: - Panel_All_Plus_Side_Panel paplus --> the Panel_All_Plus_Side_Panel to be printed  *
 *                          Return value: none                                                                              *
 *                          Side effects: - prints to stdout                                                                  *
 ****************************************************************************************************************************/
void print_all_plus_side(Panel_All_Plus_Side_Panel paplus)
{
    print_panel_row(paplus.top);
    print_row_plus_side(paplus.middle);
    print_row_plus_side(paplus.bottom);
    (void) printf("%s\n", paplus.final_row);

    return;
}


/****************************************************************************************************************************
 * print_row_plus_side():   Purpose: prints a Panel_Row_Plus_Side_Panel to the screen                                       *
 *                          Parameters: - Panel_Row_Plus_Side_Panel prplus --> the Panel_Row_Plus_Side_Panel to be printed  *
 *                          Return value: none                                                                              *
 *                          Side effects: - prints to stdout                                                                  *
 ****************************************************************************************************************************/
void print_row_plus_side(Panel_Row_Plus_Side_Panel prplus)
{
    (void) printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", prplus.row0, prplus.row1, prplus.row2,
                                                                      prplus.row3, prplus.row4, prplus.row5,
                                                                      prplus.row6, prplus.row7, prplus.row8,
                                                                      prplus.row9, prplus.row10, prplus.row11);
    return;
}


/********************************************************************************************************
 * read_line():   Purpose: reads and stores user input, and returns the number of characters stored     *
 *                Parameters: - char input[] --> the array in which to store user input                 *
 *                            - int n --> the maximum amount of characters that can be stored           *
 *                Return value: int                                                                     *
 *                Side effects: - fetches from stdin                                                    *
 *                              - modifies the array input[]                                            *
 ********************************************************************************************************/
int read_line(char input[], int n)
{
    int i = 0, ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        if (i < n)
            input[i++] = ch;
    input[i] = '\0';

    return i;
}


/*****************************************************************************************************************************************
 * parse_command():   Purpose: Receives and acts upon the command previously entered by the user, and returns whether command was valid  *
 *                    Parameters: - char *command --> the string containing the user's command                                           *
 *                                - int n --> the length (in characters) of the user's command                                           *
 *                                - Panel_All solution --> the completed puzzle, to be shown on the screen                               *
 *                                - Panel *panel0 --> pointer to the variable containing the 0th panel                                   *
 *                                - Panel *panel1 --> pointer to the variable containing the 1st panel                                   *
 *                                - Panel *panel2 --> pointer to the variable containing the 2nd panel                                   *
 *                                - Panel *panel3 --> pointer to the variable containing the 3rd panel                                   *
 *                                - Panel *panel4 --> pointer to the variable containing the 4th panel                                   *
 *                                - Panel *panel5 --> pointer to the variable containing the 5th panel                                   *
 *                                - Panel *panel6 --> pointer to the variable containing the 6th panel                                   *
 *                                - Panel *panel7 --> pointer to the variable containing the 7th panel                                   *
 *                                - Panel *panel8 --> pointer to the variable containing the 8th panel                                   *
 *                                - bool *submit --> pointer to the variable stating whether the user wishes to submit the puzzle        *
 *                                      for win/loss verification                                                                        *
 *                    Return value: bool                                                                                                 *
 *                    Side effects: - alters the variables pointed to by the Panel * parameters and the bool * parameter                 *
 *                                  - prints to stdout                                                                                   *
 *                                  - clears CLI screen and scrollback                                                                   *
 *                                  - terminates program                                                                                 *
 *****************************************************************************************************************************************/
bool parse_command(char *command, int n, Panel_All solution,
                   Panel *panel0, Panel *panel1, Panel *panel2,
                   Panel *panel3, Panel *panel4, Panel *panel5,
                   Panel *panel6, Panel *panel7, Panel *panel8,
                   bool *submit)
{
    bool valid = true;
    int panel_number;
    Panel *gap_panel = NULL;
    Panel *panel_set[9] = {panel0, panel1, panel2, panel3, panel4, panel5, panel6, panel7, panel8};

    if (caseless_cmp(command, "help"))
    {
        CLEAR_CONSOLE;
        print_command_listing();
    }
    else if (caseless_cmp(command, "quit") || caseless_cmp(command, "q"))
    {
        CLEAR_CONSOLE;
        exit(0);
    }
    else if (caseless_cmp(command, "show numbering"))
    {
        CLEAR_CONSOLE;
        print_numbers();
    }
    else if (caseless_cmp(command, "show solution"))
    {
        CLEAR_CONSOLE;
        (void) printf("Solution:\n");
        print_panel_all(solution);
        (void) printf("\n\n----PRESS ENTER----\n\n");
        while (getchar() != '\n');
    }
    else if (caseless_skip_digit_cmp_no9(command, "flip panel 0 horizontally") || caseless_skip_digit_cmp_no9(command, "0 h"))
    {
        for (int i = 0; i < n; i++)
            if (isdigit(*(command + i)))
            {
                panel_number = atoi(command + i);
                break;
            }
        if (SELECTED_PANEL->is_gap)
        {
            (void) printf("Cannot flip gap.\n");
            valid = !valid;
        }
        if (valid)
        {
            *SELECTED_PANEL = flip_panel_over_y(*SELECTED_PANEL);
        }
    }
    else if (caseless_skip_digit_cmp_no9(command, "flip panel 0 vertically") || caseless_skip_digit_cmp_no9(command, "0 v"))
    {
        for (int i = 0; i < n; i++)
            if (isdigit(*(command + i)))
            {
                panel_number = atoi(command + i);
                break;
            }
        if (SELECTED_PANEL->is_gap)
        {
            (void) printf("Cannot flip gap.\n");
            valid = !valid;
        }
        if (valid)
        {
            *SELECTED_PANEL = flip_panel_over_x(*SELECTED_PANEL);
        }
    }
    else if (caseless_skip_digit_cmp_no9(command, "rotate panel 0") || caseless_skip_digit_cmp_no9(command, "0 r"))
    {
        for (int i = 0; i < n; i++)
            if (isdigit(*(command + i)))
            {
                panel_number = atoi(command + i);
                break;
            }
        if (SELECTED_PANEL->is_gap)
        {
            (void) printf("Cannot rotate gap.\n");
            valid = !valid;
        }
        if (valid)
        {
            *SELECTED_PANEL = flip_panel_over_y(*SELECTED_PANEL);
            *SELECTED_PANEL = flip_panel_over_x(*SELECTED_PANEL);
        }
    }
    else if (caseless_cmp(command, "down") || caseless_cmp(command, "s"))
    {
        for (int i = 0; i < 9; i++)
        {
            if (panel_set[i]->is_gap)
            {
                gap_panel = panel_set[i];
                if (gap_panel->position == 0 || gap_panel->position == 1 || gap_panel->position == 2)
                {
                    (void) printf("Cannot comply--no panel exists above the gap.\n");
                    valid = !valid;
                }
                break;
            }
        }
        if (valid)
        {
            for (int i = 0; i < 9; i++)
            {
                if (panel_set[i]->position == gap_panel->position - 3) // "gap_panel->position - 3" equates to the position of the panel above the gap.
                {
                    panel_set[i]->position = gap_panel->position;
                    gap_panel->position = gap_panel->position - 3;
                    if (gap_panel->position == 0 || gap_panel->position == 1 || gap_panel->position == 2)
                        (void) strcpy(gap_panel->row0, GAP "\0");
                    break;
                }
            }
        }
    }
    else if (caseless_cmp(command, "right") || caseless_cmp (command, "d"))
    {
        for (int i = 0; i < 9; i++)
        {
            if (panel_set[i]->is_gap)
            {
                gap_panel = panel_set[i];
                if (gap_panel->position == 0 || gap_panel->position == 3 || gap_panel->position == 6)
                {
                    (void) printf("Cannot comply--no panel exists to the left of the gap.\n");
                    valid = !valid;
                }
                break;
            }
        }
        if (valid)
        {
            for (int i = 0; i < 9; i++)
            {
                if (panel_set[i]->position == gap_panel->position - 1)
                {
                    panel_set[i]->position = gap_panel->position;
                    gap_panel->position = gap_panel->position - 1;
                    break;
                }
            }
        }
    }
    else if (caseless_cmp(command, "up") || caseless_cmp(command, "w"))
    {
        for (int i = 0; i < 9; i++)
        {
            if (panel_set[i]->is_gap)
            {
                gap_panel = panel_set[i];
                if (gap_panel->position == 6 || gap_panel->position == 7 || gap_panel->position == 8)
                {
                    (void) printf("Cannot comply--no panel exists below the gap.\n");
                    valid = !valid;
                }
                break;
            }
        }
        if (valid)
        {
            for (int i = 0; i < 9; i++)
            {
                if (panel_set[i]->position == gap_panel->position + 3)
                {
                    panel_set[i]->position = gap_panel->position;
                    gap_panel->position = gap_panel->position + 3;
                    (void) strcpy(gap_panel->row0, PANEL_TOP "\0");
                    break;
                }
            }
        }
    }
    else if (caseless_cmp(command, "left") || caseless_cmp(command, "a"))
    {
        for (int i = 0; i < 9; i++)
        {
            if (panel_set[i]->is_gap)
            {
                gap_panel = panel_set[i];
                if (gap_panel->position == 2 || gap_panel->position == 5 || gap_panel->position == 8)
                {
                    (void) printf("Cannot comply--no panel exists to the right of the gap.\n");
                    valid = !valid;
                }
                break;
            }
        }
        if (valid)
        {
            for (int i = 0; i < 9; i++)
            {
                if (panel_set[i]->position == gap_panel->position + 1)
                {
                    panel_set[i]->position = gap_panel->position;
                    gap_panel->position = gap_panel->position + 1;
                    break;
                }
            }
        }
    }
    else if (caseless_cmp(command, "submit"))
        *submit = true;
    else
    {
        (void) printf("Command not recognized.\n");
        valid = !valid;
    }

    return valid;
}


/************************************************************************************************
 * caseless_cmp():   Purpose: compares two strings irrespective of case                         *
 *                   Parameters: - char str1[] --> the array containing the first string        *
 *                               - char str2[] --> the array containing the second string       *
 *                   Return value: bool                                                         *
 *                   Side effects: none                                                         *
 ************************************************************************************************/
bool caseless_cmp(char str1[], char str2[])
{
    if (strlen(str1) != strlen(str2))
        return false;
    for (int i = 0; str1[i]; i++)
        if (tolower(str1[i]) != tolower(str2[i]))
            return false;
    return true;
}


/************************************************************************************************
 * print_command_listing():   Purpose: prints a list of valid commands to the screen            *
 *                            Parameters: none                                                  *
 *                            Return value: none                                                *
 *                            Side effects: - prints to stdout                                  *
 ************************************************************************************************/
void print_command_listing(void)
{
    (void) printf("Valid Commands:\n\n");
    (void) printf("'Help': Prints this listing.\n");
    (void) printf("'Quit' or 'q': Terminates program.\n");
    (void) printf("'Show numbering': Displays which numbers correspond to the picture panels.\n");
    (void) printf("'Show solution': Displays the unscrambled image for reference.\n");
    (void) printf("'Flip panel <n> horizontally' or '<n> h': Mirrors the specified panel across the Y-axis.\n");
    (void) printf("'Flip panel <n> vertically' or '<n> v': Mirrors the specified panel across the X-axis.\n");
    (void) printf("'Rotate panel <n>' or '<n> r': Rotates the specified panel 180 degrees.\n");
    (void) printf("'Down' or 's': Shifts the panel above the gap downward to fill the gap.\n");
    (void) printf("'Right' or 'd': Shifts the panel left of the gap rightward to fill the gap.\n");
    (void) printf("'Up' or 'w': Shifts the panel below the gap upward to fill the gap.\n");
    (void) printf("'Left' or 'a': Shifts the panel right of the gap leftward to fill the gap.\n");
    (void) printf("'Submit': Checks puzzle against solution in order to win.\n");

    (void) printf("\n\n");
    (void) printf("Commands are not case-sensitive.\n\n");
    (void) printf("----PRESS ENTER----\n\n");
    while (getchar() != '\n');
    return;
}


/************************************************************************************
 * print_numbers():   Purpose: prints a display showing the panel numbering system  *
 *                    Parameters: none                                              *
 *                    Return value: none                                            *
 *                    Side effects: - prints to stdout                              *
 ************************************************************************************/
void print_numbers(void)
{
    Panel p0, p1, p2, p3, p4, p5, p6, p7, p8;
    Panel_Row top, middle, bottom;
    Panel_All number_display;
    p0 = p1 = p2 = p3 = p4 = p5 = p6 = p7 = p8 = blank_panel();
    (void) strcpy(p0.row6, "|              Panel 0               |");
    (void) strcpy(p1.row6, "|              Panel 1               |");
    (void) strcpy(p2.row6, "|              Panel 2               |");
    (void) strcpy(p3.row6, "|              Panel 3               |");
    (void) strcpy(p4.row6, "|              Panel 4               |");
    (void) strcpy(p5.row6, "|              Panel 5               |");
    (void) strcpy(p6.row6, "|              Panel 6               |");
    (void) strcpy(p7.row6, "|              Panel 7               |");
    (void) strcpy(p8.row6, "|              Panel 8               |");

    top = assemble_panel_row(p0, p1, p2);
    middle = assemble_panel_row(p3, p4, p5);
    bottom = assemble_panel_row(p6, p7, p8);

    number_display = assemble_panel_all(top, middle, bottom);
    (void) printf("Numbering:\n");
    print_panel_all(number_display);

    (void) printf("\n\n----PRESS ENTER----\n\n");
    while (getchar() != '\n');

    return;
}


/******************************************************************************************************************************
 * caseless_skip_digit_cmp_no9():   Purpose: Compares two strings irrespective of case, while ignoring differences in digits. *
 *                                             Returns false if str1[] contains the digit '9'                                 *
 *                                  Parameters: - char str1[] --> the first string to be compared ('9'-sensitive)             *
 *                                              - char str2[] --> the second string to be compared ('9'-insensitive)          *
 *                                  Return value: bool                                                                        *
 *                                  Side effects: none                                                                        *
 ******************************************************************************************************************************/
bool caseless_skip_digit_cmp_no9(char str1[], char str2[])
{
    if (strlen(str1) != strlen(str2))
        return false;
    for (int i = 0; str1[i]; i++)
    {
        if (isdigit(str1[i]))
        {
            if (str1[i] == '9') // This bizarre check prevents a bug wherein commands with the number 9 are interpreted as using the number 8 instead,
                                //      due to the implementation of the "SELECTED_PANEL" macro.
            {
                return false;
            }
            else if (isdigit(str2[i]))
            {
                continue;
            }
            else
            {
                return false;
            }
        }
        else if (tolower(str1[i]) != tolower(str2[i]))
            return false;
    }
    return true;
}


/***************************************************************************************************************************************
 * update_display():    Purpose: Updates the display based on the effects of the player's commands                                     *
 *                      Parameters: - Panel *panel0 --> pointer to the variable containing the 0th panel                               *
 *                                  - Panel *panel1 --> pointer to the variable containing the 1st panel                               *
 *                                  - Panel *panel2 --> pointer to the variable containing the 2nd panel                               *
 *                                  - Panel *panel3 --> pointer to the variable containing the 3rd panel                               *
 *                                  - Panel *panel4 --> pointer to the variable containing the 4th panel                               *
 *                                  - Panel *panel5 --> pointer to the variable containing the 5th panel                               *
 *                                  - Panel *panel6 --> pointer to the variable containing the 6th panel                               *
 *                                  - Panel *panel7 --> pointer to the variable containing the 7th panel                               *
 *                                  - Panel *panel8 --> pointer to the variable containing the 8th panel                               *
 *                                  - Panel_Row *top --> pointer to the variable for storing the top three panels                      *
 *                                  - Panel_Row *middle --> pointer to the variable for storing the middle three panels                *
 *                                  - Panel_Row *bottom --> pointer to the variable for storing the bottom three panels                *
 *                                  - Panel_Row_Plus_Side_Panel *middle_and_side --> pointer to the variable for storing               *
 *                                      the middle three panels plus the top portion of the sidebar graphics                           *
 *                                  - Panel_Row_Plus_Side_Panel *bottom_and_side --> pointer to the variable for storing               *
 *                                      the bottom three panels plus the bottom portion of the sidebar graphics                        *
 *                                  - Panel *final_piece_text --> pointer to the variable containing the top portion of the sidebar    *
 *                                  - Panel *final_piece --> pointer to the variable containing the bottom portion of the sidebar      *
 *                      Return value: Panel_All_Plus_Side_Panel                                                                        *
 *                      Side effects: - alters the variables pointed to by every single parameter save the "final_piece" parameters    *
 ***************************************************************************************************************************************/
Panel_All_Plus_Side_Panel update_display(Panel *panel0, Panel *panel1, Panel *panel2,
                                         Panel *panel3, Panel *panel4, Panel *panel5,
                                         Panel *panel6, Panel *panel7, Panel *panel8,
                                         Panel_Row *top, Panel_Row *middle, Panel_Row *bottom,
                                         Panel_Row_Plus_Side_Panel *middle_and_side, Panel_Row_Plus_Side_Panel *bottom_and_side,
                                         Panel *final_piece_text, Panel *final_piece)
{
    Panel *panel_set[9] = {panel0, panel1, panel2, panel3, panel4, panel5, panel6, panel7, panel8};
    Panel assembly_panel0, assembly_panel1, assembly_panel2,
          assembly_panel3, assembly_panel4, assembly_panel5,
          assembly_panel6, assembly_panel7, assembly_panel8;
    int gap;

    // Loop to rename (through duplication) panel variables for assembly.
    for (int i = 0; i < 9; i++)
    {
        if (panel_set[i]->position == 0)
            assembly_panel0 = *panel_set[i];
        else if (panel_set[i]->position == 1)
            assembly_panel1 = *panel_set[i];
        else if (panel_set[i]->position == 2)
            assembly_panel2 = *panel_set[i];
        else if (panel_set[i]->position == 3)
            assembly_panel3 = *panel_set[i];
        else if (panel_set[i]->position == 4)
            assembly_panel4 = *panel_set[i];
        else if (panel_set[i]->position == 5)
            assembly_panel5 = *panel_set[i];
        else if (panel_set[i]->position == 6)
            assembly_panel6 = *panel_set[i];
        else if (panel_set[i]->position == 7)
            assembly_panel7 = *panel_set[i];
        else
            assembly_panel8 = *panel_set[i];
    }

    *panel0 = assembly_panel0;
    *panel1 = assembly_panel1;
    *panel2 = assembly_panel2;
    *panel3 = assembly_panel3;
    *panel4 = assembly_panel4;
    *panel5 = assembly_panel5;
    *panel6 = assembly_panel6;
    *panel7 = assembly_panel7;
    *panel8 = assembly_panel8;

    for (int i = 0; i < 9; i++)
    {
        if (panel_set[i]->is_gap)
        {
            gap = i;
        }
    }

    // Assemble and return:
    *top = assemble_panel_row(assembly_panel0, assembly_panel1, assembly_panel2);
    *middle = assemble_panel_row(assembly_panel3, assembly_panel4, assembly_panel5);
    *bottom = assemble_panel_row(assembly_panel6, assembly_panel7, assembly_panel8);
    *middle_and_side = add_side_panel(*middle, *final_piece_text);
    *bottom_and_side = add_side_panel(*bottom, *final_piece);

    return assemble_all_plus_side(*top, *middle_and_side, *bottom_and_side, gap);
}


/***************************************************************************************************************************************
 * check_answer():      Purpose: Compares the panels of the player's submission to the panels containing the solution                  *
 *                      Parameters: - Panel *panel0 --> pointer to the variable containing the 0th panel                               *
 *                                  - Panel *panel1 --> pointer to the variable containing the 1st panel                               *
 *                                  - Panel *panel2 --> pointer to the variable containing the 2nd panel                               *
 *                                  - Panel *panel3 --> pointer to the variable containing the 3rd panel                               *
 *                                  - Panel *panel4 --> pointer to the variable containing the 4th panel                               *
 *                                  - Panel *panel5 --> pointer to the variable containing the 5th panel                               *
 *                                  - Panel *panel6 --> pointer to the variable containing the 6th panel                               *
 *                                  - Panel *panel7 --> pointer to the variable containing the 7th panel                               *
 *                                  - Panel *panel8 --> pointer to the variable containing the 8th panel                               *
 *                                  - Panel *solution_panel0 --> pointer to the variable containing the 0th solution panel             *
 *                                  - Panel *solution_panel1 --> pointer to the variable containing the 1st solution panel             *
 *                                  - Panel *solution_panel2 --> pointer to the variable containing the 2nd solution panel             *
 *                                  - Panel *solution_panel3 --> pointer to the variable containing the 3rd solution panel             *
 *                                  - Panel *solution_panel4 --> pointer to the variable containing the 4th solution panel             *
 *                                  - Panel *solution_panel5 --> pointer to the variable containing the 5th solution panel             *
 *                                  - Panel *solution_panel6 --> pointer to the variable containing the 6th solution panel             *
 *                                  - Panel *solution_panel7 --> pointer to the variable containing the 7th solution panel             *
 *                                  - Panel *solution_panel8 --> pointer to the variable containing the 8th solution panel             *
 *                      Return value: bool                                                                                             *
 *                      Side effects: none                                                                                             *
 ***************************************************************************************************************************************/
bool check_answer(Panel *panel0, Panel *panel1, Panel *panel2,
                  Panel *panel3, Panel *panel4, Panel *panel5,
                  Panel *panel6, Panel *panel7, Panel *panel8,
                  Panel *solution_panel0, Panel *solution_panel1, Panel *solution_panel2,
                  Panel *solution_panel3, Panel *solution_panel4, Panel *solution_panel5,
                  Panel *solution_panel6, Panel *solution_panel7, Panel *solution_panel8)
{
    bool correct;
    Panel *player_panels[9] = {panel0, panel1, panel2, panel3, panel4, panel5, panel6, panel7, panel8};
    Panel *solution_panels[9] = {solution_panel0, solution_panel1, solution_panel2,
                                 solution_panel3, solution_panel4, solution_panel5,
                                 solution_panel6, solution_panel7, solution_panel8};
    correct = true;

    for (int i = 0; i < 9; i++)
        if (!compare_panels(*solution_panels[i], *player_panels[i]))
        {
            correct = false;
            break;
        }

    return correct;
}


/************************************************************************************
 * compare_panels():    Purpose: Compares two panels                                *
 *                      Parameters: - Panel a --> the first Panel to be compared    *
 *                                  - Panel b --> the second Panel to be compared   *
 *                      Return value: bool                                          *
 *                      Side effects: none                                          *
 ************************************************************************************/
bool compare_panels(Panel a, Panel b)
{
    bool same = true;

    char *a_rows[NUM_ROWS] = {a.row0, a.row1, a.row2, a.row3, a.row4, a.row5, a.row6, a.row7, a.row8, a.row9, a.row10, a.row11};
    char *b_rows[NUM_ROWS] = {b.row0, b.row1, b.row2, b.row3, b.row4, b.row5, b.row6, b.row7, b.row8, b.row9, b.row10, b.row11};

    for (int i = 0; i < NUM_ROWS; i++)
        if (strcmp(a_rows[i], b_rows[i]) != 0)
        {
            same = false;
            break;
        }
    
    return same;
}


/********************************************************************************************************************
 * export_template():   Purpose: Creates a file containing a template for making custom puzzles, plus instructions  *
 *                      Parameters: none                                                                            *
 *                      Return value: none                                                                          *
 *                      Side effects: - creates external file                                                       *
 *                                    - opens external file                                                         *
 *                                    - writes to / overwrites external file                                        *
 *                                    - clears CLI screen and scrollback                                            *
 *                                    - terminates program                                                          *
 *                                    - fetches from stdin                                                          *
 *                                    - prints to stdout                                                            *
 ********************************************************************************************************************/
void export_template(void)
{
    FILE *template_file = NULL;
    char filename[13] = "template.txt\0";
    bool valid = false;
    char yn = 0;
    int returnval = 0;
    int fclose_return;

    // Test whether a file with the name "template.txt" already exists:
    template_file = fopen(filename, "r");
    if (template_file == NULL)
        valid = true;
    // If a file with this name already exists, confirm whether to save over it:
    else
    {
        (void) printf("A file named \"%s\" already exists. Overwrite file? (y/n)\n", filename);
        do
        {
            yn = tolower(getchar());
            while (getchar() != '\n');
        } while (yn != 'y' && yn != 'n');
        if (yn == 'n')
        {
            fclose_return = fclose(template_file);
            if (fclose_return)
            {
                (void) printf("Error 18: Close error after choosing not to overwrite template.txt.\n");
                (void) printf("Specified file could not be closed correctly.\n");
                exit(18);
            }
            return;
        }
    }
    // Create file:
    template_file = fopen(filename, "w+");

    returnval = fprintf(template_file, "Instructions:\n"
                                  "Replace spaces in the template below with desired ASCII characters to create a custom ASCII-art puzzle.\n"
                                  "\tDo not delete a space without inserting a replacement character.\n"
                                  "\tDo not add a character without deleting a space.\n"
                                  "\tDo not alter the sides, top, bottom, or spacing of the panels.\n"
                                  "When finished, rename this file.\n");
    if (returnval != 331)
    {
        CLEAR_CONSOLE;
        (void) printf("Error 1: Template instructions could not be written correctly.\n");
        (void) printf("returnval: %d\n", returnval);
        exit(1);
    }

    returnval = print_panel_all_to_file(template_file, make_template());
    if (returnval != FILE_PANEL_ALL_CHAR_NUM)
    {
        CLEAR_CONSOLE;
        (void) printf("Error 2: Template could not be written correctly.\n");
        (void) printf("returnval: %d\n", returnval);
        exit(2);
    }

    fclose_return = fclose(template_file);
    if (fclose_return)
    {
        (void) printf("Error 17: Close error after writing to template file.\n");
        (void) printf("Specified file could not be closed correctly.\n");
        exit(17);
    }

    return;
}


/********************************************************************************
 * make_template():     Purpose: creates and returns a blank puzzle template    *
 *                      Parameters: none                                        *
 *                      Return value: Panel_All                                 *
 *                      Side effects: none                                      *
 ********************************************************************************/
Panel_All make_template(void)
{
    Panel p0, p1, p2, p3, p4, p5, p6, p7, p8;
    Panel_Row top, middle, bottom;

    p0 = p1 = p2 = p3 = p4 = p5 = p6 = p7 = p8 = blank_panel();

    top = assemble_panel_row(p0, p1, p2);
    middle = assemble_panel_row(p3, p4, p5);
    bottom = assemble_panel_row(p6, p7, p8);

    return assemble_panel_all(top, middle, bottom);
}


/********************************************************************************************************
 * print_panel_all_to_file():   Purpose: prints a given Panel_All to a given file.                      *
 *                                         Returns the number of characters written.                    *
 *                              Parameters: - FILE *filename --> pointer to the file to be written to   *
 *                                          - Panel_All pa --> the Panel_All to be printed              *
 *                              Return value: int                                                       *
 *                              Side effects: - prints to file                                          *
 *                                            - modifies file pointed to by FILE *filename              *
 ********************************************************************************************************/
int print_panel_all_to_file(FILE *filename, Panel_All pa)
{
    int returnval = 0;

    returnval += print_panel_row_to_file(filename, pa.top);
    returnval += print_panel_row_to_file(filename, pa.middle);
    returnval += print_panel_row_to_file(filename, pa.bottom);
    returnval += fprintf(filename, "%s", pa.final_row);

    return returnval;
}


/********************************************************************************************************
 * print_panel_row_to_file():   Purpose: prints a given Panel_Row to a given file.                      *
 *                                         Returns the number of characters written.                    *
 *                              Parameters: - FILE *filename --> pointer to the file to be written to   *
 *                                          - Panel_Row pr --> the Panel_Row to be printed              *
 *                              Return value: int                                                       *
 *                              Side effects: - prints to file                                          *
 *                                            - modifies file pointed to by FILE *filename              *
 ********************************************************************************************************/
int print_panel_row_to_file(FILE *filename, Panel_Row pr)
{
    return fprintf(filename, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", pr.row0, pr.row1, pr.row2,
                                                                                 pr.row3, pr.row4, pr.row5,
                                                                                 pr.row6, pr.row7, pr.row8,
                                                                                 pr.row9, pr.row10, pr.row11);
}

/************************************************************************************************************************************************
 * Notes on further work:                                                                                                                       *
 * - It would be nice to expand this program in order to display color graphics, using ANSI escapes.                                            *
 * - This code could do with refactoring (or, more easily, rewriting) in order to separate out the display from the panels,                     *
 *      making it possible to manipulate them separately. Not strictly necessary, but makes things cleaner and easier to modify.                *
 * - At one point, I implemented a command to return to the main menu, as well as the ability to play multiple puzzles sequentially.            *
 *      This caused a bizarre segfault bug that, even with help, I was unable to track down. I would like to re-implement this functionality    *
 *      while preventing the bug. Likely, this would take large-scale refactoring, if not outright recreation of the program in a neater form   *
 *      (see bullets above and below).                                                                                                          *
 * - The file importing functionality would be better off reading line-by-line rather than character-by-character, as it does currently.        *
 * - It might be a good idea to combine the custom puzzle feature with the default puzzle feature by bundling the main code with a set of       *
 *      external default puzzles, rather than storing default puzzles within the code itself.                                                   *
 * - More default puzzles (currently there's only the one).                                                                                     *
 ************************************************************************************************************************************************/