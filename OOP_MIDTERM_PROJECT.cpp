#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <string>
#include <thread>
#include <vector>
#include <cmath>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

// Game constants
const int TTT_BOARD_SIZE = 9;
const int WIN_SCORE = 3;
string space = "           "; // 14 spaces

const string RED_BG = "\033[41m";
const string WHITE = "\033[37m";
const string BLACK = "\033[30m";
const string RESET = "\033[0m";

// Function prototypes
void clear_input_buffer();
void clear_screen();
void gotoxy(int x, int y);
void cursor_hide();
void cursor_show();
void terminal_pause(const string &prompt);
void delay(int milliseconds);
int getch();

string interpolateColor(int r1, int g1, int b1, int r2, int g2, int b2, float ratio);
void displayTicTacToeTitle();
void displaySnakeLadderTitle();

void arrow_options_animation(const vector<string> &options, int selected);
int display_options(const vector<string> &options, const string &title);

void arrow_options_animation(const vector<string> &options, int selected, int x, int y);
int display_options(const vector<string> &options, const string &title, int x, int y);

// Tic-Tac-Toe functions
void tictactoe_game_menu();
void tictactoe_game(bool vsAI = false, bool hardMode = false);
void print_tictactoe_board(const string board_mark[], int player1_score,
                           int player2_score, const string &player1_name,
                           const string &player2_name);
void tictactoe_how_to_play();
void tictactoe_developer_section();

int evaluate_board(const string board[]);
bool is_moves_left(const string board[]);
int minimax(string board[], bool isMax);
int find_best_move(string board[], bool hardMode);

// Snake and Ladder functions (stubs)
void snake_and_ladder_game_menu();
void snake_and_ladder_game();
void print_table_boarder();
void print_snake_and_ladder_board(string board_tile[], int difficulty, string player_avatars[], int player_tile_placement[], int total_players);
int dice_roller();
void display_dice_face(int dice_number);
int player_tile_placement_checker(int player_tile_placement, int difficulty, bool is_immune);
void snake_and_ladder_how_to_play();
void displayThankYou();
void snake_and_ladder_developer_section();
void print_big_DEVELOPERS();

void delay(int milliseconds)
{
 this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

int getch()
{
#ifdef _WIN32
  return _getch();
#else
  struct termios oldt, newt;
  int ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
#endif
}

int main()
{
#ifdef _WIN32
  SetConsoleOutputCP(65001);
#endif
  srand(time(0)); // Add this line for random number generation
  cursor_hide();
  vector<string> game_options = {"Tictactoe ❌🔵", "Snake and Ladder 🪜🐍"};
  
  while (true)
  {
    int game_selected = display_options(game_options, "Please Select a Game");

    switch (game_selected)
    {
    case 0:
      tictactoe_game_menu();
      break;
    case 1:
      snake_and_ladder_game_menu();
      break;
    }
  }

  return 0;
}

string interpolateColor(int r1, int g1, int b1, int r2, int g2, int b2,
  float ratio)
{
int r = r1 + (int)((r2 - r1) * ratio);
int g = g1 + (int)((g2 - g1) * ratio);
int b = b1 + (int)((b2 - b1) * ratio);
return "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) +
"m";
}

void displayTicTacToeTitle()
{
cout << endl;
string asciiArt[] = {
"████████╗██╗ ██████╗████████╗ █████╗  ██████╗████████╗ ██████╗███████╗",
"╚══██╔══╝██║██╔════╝╚══██╔══╝██╔══██╗██╔════╝╚══██╔══╝██╔═══██╗██╔════╝",
"   ██║   ██║██║        ██║   ███████║██║        ██║   ██║   ██║█████╗  ",
"   ██║   ██║██║        ██║   ██╔══██║██║        ██║   ██║   ██║██╔══╝  ",
"   ██║   ██║╚██████╗   ██║   ██║  ██║╚██████╗   ██║   ╚██████╔╝███████╗",
"   ╚═╝   ╚═╝ ╚═════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝   ╚═╝    ╚═════╝ "
"╚══════╝"};

// Blue to cyan gradient (similar intensity to snake and ladder gradients)
for (size_t i = 0; i < 6; ++i)
{
float ratio = static_cast<float>(i) / 5.0f;
// Dark blue to bright cyan
string color = interpolateColor(0, 0, 150, 0, 255, 255, ratio);
cout << color << asciiArt[i] << "\033[0m" << endl;
}
}

void displaySnakeLadderTitle()
{
cout << endl;
string snakeArt[] = {"███████╗███╗   ██╗ █████╗ ██╗  ██╗███████╗",
 "██╔════╝████╗  ██║██╔══██╗██║ ██╔╝██╔════╝",
 "███████╗██╔██╗ ██║███████║█████╔╝ █████╗  ",
 "╚════██║██║╚██╗██║██╔══██║██╔═██╗ ██╔══╝  ",
 "███████║██║ ╚████║██║  ██║██║  ██╗███████╗",
 "╚══════╝╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝"};

string andArt[] = {
" █████╗ ███╗   ██╗██████╗ ", "██╔══██╗████╗  ██║██╔══██╗",
"███████║██╔██╗ ██║██║  ██║", "██╔══██║██║╚██╗██║██║  ██║",
"██║  ██║██║ ╚████║██████╔╝", "╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ "};

string ladderArt[] = {"██╗      █████╗ ██████╗ ██████╗ ███████╗██████╗ ",
  "██║     ██╔══██╗██╔══██╗██╔══██╗██╔════╝██╔══██╗",
  "██║     ███████║██║  ██║██║  ██║█████╗  ██████╔╝",
  "██║     ██╔══██║██║  ██║██║  ██║██╔══╝  ██╔══██╗",
  "███████╗██║  ██║██████╔╝██████╔╝███████╗██║  ██║",
  "╚══════╝╚═╝  ╚═╝╚═════╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝"};

// SNAKE gradient (green to light green)
for (size_t i = 0; i < 6; ++i)
{
float ratio = static_cast<float>(i) / 5.0f;
string color = interpolateColor(0, 150, 0, 144, 238, 144, ratio);
cout << string(4, ' ') << color << snakeArt[i] << "\033[0m" << endl;
}

// AND gradient (gold to yellow)
for (size_t i = 0; i < 6; ++i)
{
float ratio = static_cast<float>(i) / 5.0f;
string color = interpolateColor(218, 165, 32, 255, 255, 0, ratio);
cout << string(12, ' ') << color << andArt[i] << "\033[0m" << endl;
}

// LADDER gradient (red to orange)
for (size_t i = 0; i < 6; ++i)
{
float ratio = static_cast<float>(i) / 5.0f;
string color = interpolateColor(178, 34, 34, 255, 165, 0, ratio);
cout << string(2, ' ') << color << ladderArt[i] << "\033[0m" << endl;
}
cout << endl;
}

void clear_input_buffer()
{
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void clear_screen()
{
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void gotoxy(int x, int y) {
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void cursor_hide()
{
#ifdef _WIN32
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 100;
  info.bVisible = FALSE;
  SetConsoleCursorInfo(consoleHandle, &info);
#else
  cout << "\033[?25l";
#endif
}

void cursor_show()
{
#ifdef _WIN32
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 100;
  info.bVisible = TRUE;
  SetConsoleCursorInfo(consoleHandle, &info);
#else
  cout << "\033[?25h";
#endif
}



void terminal_pause(const string &prompt)
{
  system("pause");
}

void arrow_options_animation(const vector<string> &options, int selected)
{
  for (size_t i = 0; i < options.size(); i++)
  {
    cout << (i == selected ? "➡️" : " ") << " " << options[i] << " " << endl;
  }
}

int display_options(const vector<string> &options, const string &title)
{
  int selected = 0;
  int key;

  while (true)
  {
    gotoxy(0,0);
    cursor_hide();

    // Print title with border
    int title_length = title.length();
    cout << string(title_length + 16, '*') << "\n"
         << string(8, ' ') << title << "\n"
         << string(title_length + 16, '*') << "\n\n";

    gotoxy(0,4);     
    arrow_options_animation(options, selected);
    key = getch();

#ifdef _WIN32
    if (key == 0 || key == 0xE0)
    {
      key = getch();
      if (key == 72)
      { // Up arrow
        selected = (selected - 1 + options.size()) % options.size();
      }
      else if (key == 80)
      { // Down arrow
        selected = (selected + 1) % options.size();
      }
    }
#else
    if (key == 27 && getch() == 91)
    {
      key = getch();
      if (key == 65)
      { // Up arrow
        selected = (selected - 1 + options.size()) % options.size();
      }
      else if (key == 66)
      { // Down arrow
        selected = (selected + 1) % options.size();
      }
    }
#endif
    else if (key == '\r' || key == '\n')
    {
      clear_screen();
      break;
    }
  }

  return selected;
}

void arrow_options_animation(const vector<string> &options, int selected, int x, int y){
  for (int i = 0; i < options.size(); i++)
  {
    gotoxy(x, y + 4 + i);
    cout << (i == selected ? " ➡️" : "  ") << options[i] << "  " << endl;
  }
}
int display_options(const vector<string> &options, const string &title, int x, int y){
  int selected = 0;
  int key;

  while (true)
  {
    cursor_hide();

    // Print title with border
    int title_length = title.length();
    gotoxy(x,y);
    cout << string(title_length + 16, '*') << "\n";
    gotoxy(x,y + 1);
    cout << string(8, ' ') << title << "\n";
    gotoxy(x,y+ 2);
    cout << string(title_length + 16, '*') << "\n\n";
   
    arrow_options_animation(options, selected, x , y);
    key = getch();

#ifdef _WIN32
    if (key == 0 || key == 0xE0)
    {
      key = getch();
      if (key == 72)
      { // Up arrow
        selected = (selected - 1 + options.size()) % options.size();
      }
      else if (key == 80)
      { // Down arrow
        selected = (selected + 1) % options.size();
      }
    }
#else
    if (key == 27 && getch() == 91)
    {
      key = getch();
      if (key == 65)
      { // Up arrow
        selected = (selected - 1 + options.size()) % options.size();
      }
      else if (key == 66)
      { // Down arrow
        selected = (selected + 1) % options.size();
      }
    }
#endif
    else if (key == '\r' || key == '\n')
    {
      clear_screen();
      break;
    }
  }

  return selected;
}

void tictactoe_game_menu()
{
  clear_screen();
  displayTicTacToeTitle();
  terminal_pause("\nENTER to continue...\n");
  clear_screen();
  cout << "*****************************************\n"
       << "    👋 WELCOME TO ❌TICTACTOE GAME🔵\n"
       << "*****************************************\n";

  terminal_pause("Press Enter to Continue...");

  while (true)
  {
    clear_screen();
    vector<string> main_menu_options = {" Play Game 🕹️", " How to Play ❔",
                                        " Developers ⌨️", " Exit ➡️"};

    int menu_decision =
        display_options(main_menu_options, "❌ TICTACTOE GAME 🔵");

    switch (menu_decision)
    {
    case 0:
    {
      vector<string> mode_options = {"Player vs Player 👥",
                                     "Player vs AI 🤖 (Easy)",
                                     "Player vs AI 🤖 (Hard)"};

      int mode_choice = display_options(mode_options, "Select Game Mode");

      switch (mode_choice)
      {
      case 0:
        tictactoe_game(false);
        break;
      case 1:
        tictactoe_game(true, false);
        break;
      case 2:
        tictactoe_game(true, true);
        break;
      }
      break;
    }
    case 1:
      tictactoe_how_to_play();
      break;
    case 2:
      tictactoe_developer_section();
      break;
    case 3:
      clear_screen();
      displayThankYou();
      terminal_pause("\n\tPRESS ENTER TO GO BACK TO GAME SELECTION MENU...");
      clear_screen();
      return;
      break;
    }
  }
}

void tictactoe_game(bool vsAI, bool hardMode)
{
  string board_mark[TTT_BOARD_SIZE] = {" 1", " 2", " 3", " 4", " 5",
                                       " 6", " 7", " 8", " 9"};
  string player1_name, player2_name = vsAI ? "AI" : "";
  int player1_score = 0, player2_score = 0;

  // Game instructions
  char read_instruction_response;
  do
  {
    clear_screen();
    cout << "**********************************************************\n"
         << "        PLEASE READ THE HOW TO PLAY MENU FIRST\n"
         << "**********************************************************\n"
         << "Reading this game instructions before playing is a must if \n"
         << "you already read the instructions press 'p' to proceed and  \n"
         << "       if not press 'r' to read the instructions\n"
         << "\n\n\tProceed [p]\t\tRead Instructions [r]\n";

    read_instruction_response = tolower(getch());

    if (read_instruction_response == 'r')
    {
      tictactoe_how_to_play();
      return;
    }
    else if (read_instruction_response != 'p')
    {
      terminal_pause("Invalid input please read the prompt carefully,\nPress "
                     "enter to continue...");
    }
  } while (read_instruction_response != 'p');

  // Player registration
  clear_screen();
  cout << "---Before starting the game lets register each player's name "
          "first---\n\n";
  cursor_show();

  cout << "    Enter the 1st player name: ";
  cin >> player1_name;

  if (!vsAI)
  {
    cout << "\n    Enter the 2nd player name: ";
    cin >> player2_name;
  }
  cursor_hide();

  clear_screen();
  clear_input_buffer();
  cout << "Welcome " << player1_name << (vsAI ? "" : " and " + player2_name)
       << " please enjoy the game 😊\n";
  cout << endl << player1_name << " will take the first move. The player who reach 3 points first, will win 🏆" << endl;
  terminal_pause("Press ENTER and the game will begin...");

  // Main game loop
  while (player1_score < WIN_SCORE && player2_score < WIN_SCORE)
  {
    int current_move = 0;
    bool game_over = false;

    // Reset board
    for (int i = 0; i < TTT_BOARD_SIZE; i++)
    {
      board_mark[i] = " " + to_string(i + 1);
    }

    // Round loop
    while (!game_over && current_move < TTT_BOARD_SIZE)
    {
      // Player 1's move
      int move;
      bool valid_move = false;

      print_tictactoe_board(board_mark, player1_score, player2_score,
                            player1_name, player2_name);

      do
      {
        // cout << player1_name << "'s move (1-9): ";
        cout << player1_name << "'s move ❌ (1-9): ";

        if (cin >> move)
        {
          if (move < 1 || move > 9)
          {
            cout << "Invalid input. Please choose a number between 1-9.\n";
          }
          else if (board_mark[move - 1] == "❌" || board_mark[move - 1] == "🔵")
          {
            cout << "That position is already taken. Please choose another.\n";
          }
          else
          {
            valid_move = true;
          }
        }
        else
        {
          cout << "Invalid input. Please enter a number.\n";
          cin.clear();
          clear_input_buffer();
        }
      } while (!valid_move);

      board_mark[move - 1] = "❌";
      current_move++;

      // Check if player 1 won
      if (evaluate_board(board_mark))
      {
        print_tictactoe_board(board_mark, player1_score, player2_score,
                              player1_name, player2_name);
        cout << "CONGRATULATIONS!!! " << player1_name << " Wins!" << endl;
        player1_score++;
        clear_input_buffer();
        terminal_pause("Press enter to proceed to next round...");
        game_over = true;
        break;
      }

      if (current_move == TTT_BOARD_SIZE)
        break;

      // Player 2's move (either human or AI)
      if (vsAI)
      {
        // AI's turn

        print_tictactoe_board(board_mark, player1_score, player2_score,
                              player1_name, player2_name);

        cout << "AI is thinking";
        for (int i = 0; i < 5; i++)
        {
          cout << ".";
          cout.flush();
          delay(250); // 0.5 second delay
        }

        int ai_move = find_best_move(board_mark, hardMode);
        board_mark[ai_move] = "🔵";
        current_move++;

        // cout << "AI chose position " << (ai_move + 1) << endl;

        print_tictactoe_board(board_mark, player1_score, player2_score,
                              player1_name, player2_name);

        cout << "AI chose position " << (ai_move + 1) << endl;

        delay(900);

        // Check if AI won
        if (evaluate_board(board_mark))
        {
          cout << endl << "AI Wins! Better luck next time!" << endl;
          player2_score++;
          clear_input_buffer();
          terminal_pause("Press enter to proceed to next round...");
          game_over = true;
        }
      }
      else
      {
        // Human player 2's turn
        print_tictactoe_board(board_mark, player1_score, player2_score,
                              player1_name, player2_name);

        valid_move = false;
        do
        {
          // cout << player2_name << "'s move (1-9): ";
          cout << player2_name << "'s move 🔵 (1-9): ";
          if (cin >> move)
          {
            if (move < 1 || move > 9)
            {
              cout << "Invalid input. Please choose a number between 1-9.\n";
            }
            else if (board_mark[move - 1] == "❌" ||
                     board_mark[move - 1] == "🔵")
            {
              cout
                  << "That position is already taken. Please choose another.\n";
            }
            else
            {
              valid_move = true;
            }
          }
          else
          {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            clear_input_buffer();
          }
        } while (!valid_move);

        board_mark[move - 1] = "🔵";
        current_move++;

        // Check if player 2 won
        if (evaluate_board(board_mark))
        {
          print_tictactoe_board(board_mark, player1_score, player2_score,
                                player1_name, player2_name);
          cout << "CONGRATULATIONS!!! " << player2_name << " Wins!" << endl;
          player2_score++;
          clear_input_buffer();
          terminal_pause("Press enter to proceed to next round...");
          game_over = true;
        }
      }
    }

    // Check for draw
    if (!game_over && current_move == TTT_BOARD_SIZE)
    {
      print_tictactoe_board(board_mark, player1_score, player2_score,
                            player1_name, player2_name);
      clear_input_buffer();
      cout << "⚠️ It's a DRAW!!!" << endl;
      terminal_pause("\nPress enter to start a new round..");
    }

    clear_screen();
    print_tictactoe_board(board_mark, player1_score, player2_score,
    player1_name, player2_name);
    char exit_decision, exit_decision_confirmation;

    cout << "Press E [e] if you want end the game and go back to game menu if not just press any other key...";
    //exit_decision = _getch();
exit_decision = getch();
    if(exit_decision == 'e'){
      clear_screen();
      cout << "Are you sure you want to EXIT? Press Y [y] if yes and if no press any other key";
      exit_decision_confirmation = getch();

      if(exit_decision_confirmation == 'y'){
        return;
      }
    }
  }

  // Game over - determine winner
  clear_screen();
  if (player1_score == WIN_SCORE)
  {
    cout << endl
         << "Congratulations!🎊 " << player1_name
         << " You Won!🎉, You OUTSMARTED " << (vsAI ? "The AI" : player2_name)
         << " 🍾\n";
  }
  else
  {
    cout << endl
         << "Congratulations!🎊 " << (vsAI ? "AI" : player2_name)
         << " You Won!🎉, You OUTSMARTED " << player1_name << " 🍾\n";
  }

  terminal_pause("Press ENTER to go back to TICTACTOE main menu...");
}

void print_tictactoe_board(const string board_mark[], int player1_score,
                           int player2_score, const string &player1_name,
                           const string &player2_name)
{
  clear_screen();

  // Gradient color function
  auto get_color = [](int step, int total_steps)
  {
    float ratio = static_cast<float>(step) / total_steps;
    int r = static_cast<int>(255 * (1 - ratio));
    int g = static_cast<int>(255 * ratio);
    int b = static_cast<int>(128 + 127 * sin(3.14159 * ratio));
    return "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" +
           to_string(b) + "m";
  };

  // Board template with gradient
  const vector<string> board_lines = {
      "╔═══════╦═══════╦═══════╗",
      "║       ║       ║       ║",
      "║  " + board_mark[0] + "   ║  " + board_mark[1] + "   ║  " +
          board_mark[2] + "   ║",
      "║       ║       ║       ║",
      "╠═══════╬═══════╬═══════╣",
      "║       ║       ║       ║",
      "║  " + board_mark[3] + "   ║  " + board_mark[4] + "   ║  " +
          board_mark[5] + "   ║",
      "║       ║       ║       ║",
      "╠═══════╬═══════╬═══════╣",
      "║       ║       ║       ║",
      "║  " + board_mark[6] + "   ║  " + board_mark[7] + "   ║  " +
          board_mark[8] + "   ║",
      "║       ║       ║       ║",
      "╚═══════╩═══════╩═══════╝"};

  // Scoreboard template
  const vector<string> score_lines = {
      "╔═════════════════════════╗",
      "║       SCORE BOARD       ║",
      "╠═════════════════════════╣",
      "║ " + player1_name + ": " + to_string(player1_score) +
          string(20 - player1_name.length() - to_string(player1_score).length(),
                 ' ') +
          "  ║",
      "║ " + player2_name + ": " + to_string(player2_score) +
          string(20 - player2_name.length() - to_string(player2_score).length(),
                 ' ') +
          "  ║",
      "╚═════════════════════════╝"};

  // Print board and scoreboard side by side
  size_t max_lines = max(board_lines.size(), score_lines.size());
  for (size_t i = 0; i < max_lines; i++)
  {
    // Print board line (with gradient)
    if (i < board_lines.size())
    {
      cout << get_color(i, max_lines) << board_lines[i] << "\033[0m";
    }
    else
    {
      cout << string(board_lines[0].length(), ' '); // Maintain spacing
    }

    // Add spacing between board and scoreboard
    cout << "   ";

    // Print scoreboard line (with gradient)
    if (i < score_lines.size())
    {
      cout << get_color(i, max_lines) << score_lines[i] << "\033[0m";
    }

    cout << endl;
  }
  cout << endl;
}

// AI functions implementation
int evaluate_board(const string board[])
{
  // Check all possible winning combinations
  const int win_patterns[8][3] = {
      {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // rows
      {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // columns
      {0, 4, 8}, {2, 4, 6}             // diagonals
  };

  for (auto &pattern : win_patterns)
  {
    if (board[pattern[0]] == board[pattern[1]] &&
        board[pattern[0]] == board[pattern[2]])
    {
      if (board[pattern[0]] == "❌")
        return -10;
      if (board[pattern[0]] == "🔵")
        return 10;
    }
  }
  return 0; // No winner yet
}

bool is_moves_left(const string board[])
{
  for (int i = 0; i < TTT_BOARD_SIZE; i++)
  {
    if (board[i] != "❌" && board[i] != "🔵")
    {
      return true;
    }
  }
  return false;
}

int minimax(string board[], bool isMax)
{
  int score = evaluate_board(board);

  // If maximizer or minimizer has won
  if (score == 10)
    return score;
  if (score == -10)
    return score;

  // If no moves left (draw)
  if (!is_moves_left(board))
    return 0;

  // If maximizing player's turn (AI)
  if (isMax)
  {
    int best = -1000;
    for (int i = 0; i < TTT_BOARD_SIZE; i++)
    {
      if (board[i] != "❌" && board[i] != "🔵")
      {
        string original = board[i];
        board[i] = "🔵"; // AI's move
        best = max(best, minimax(board, !isMax));
        board[i] = original;
      }
    }
    return best;
  }
  // If minimizing player's turn (human)
  else
  {
    int best = 1000;
    for (int i = 0; i < TTT_BOARD_SIZE; i++)
    {
      if (board[i] != "❌" && board[i] != "🔵")
      {
        string original = board[i];
        board[i] = "❌"; // Human's move
        best = min(best, minimax(board, !isMax));
        board[i] = original;
      }
    }
    return best;
  }
}

int find_best_move(string board[], bool hardMode)
{
  if (!hardMode)
  {
    // Easy mode - make random moves
    vector<int> available;
    for (int i = 0; i < TTT_BOARD_SIZE; i++)
    {
      if (board[i] != "❌" && board[i] != "🔵")
      {
        available.push_back(i);
      }
    }
    if (!available.empty())
    {
      return available[rand() % available.size()];
    }
    return -1;
  }
  else
  {
    // Hard mode - use minimax for optimal play
    int best_val = -1000;
    int best_move = -1;

    for (int i = 0; i < TTT_BOARD_SIZE; i++)
    {
      if (board[i] != "❌" && board[i] != "🔵")
      {
        string original = board[i];
        board[i] = "🔵";
        int move_val = minimax(board, false);
        board[i] = original;

        if (move_val > best_val)
        {
          best_move = i;
          best_val = move_val;
        }
      }
    }
    return best_move;
  }
}

void tictactoe_how_to_play()
{
  clear_screen();
  cout << "\nMechanics in playing TicTacToe Game: \n"
       << "\n1. **Players**: Two players are required to play (or play against "
          "AI). \n"
       << "2. **Game Grid**: The game is played on a 3x3 grid, making up 9 "
          "spaces in total.\n"
       << "3. **Turn Order**: Players take turns marking an empty space on the "
          "grid.\n"
       << "4. **Objective**: The goal is to be the first player to get three "
          "of their symbols in a row, column, or diagonal.\n"
       << "5. **Winning Condition**: A player wins if they have three of their "
          "symbols in a horizontal, vertical, or diagonal line.\n"
       << "6. **Tie Condition**: The game ends in a draw if all 9 spaces are "
          "filled and no player has won.\n"
       << "7. **Valid Move**: A player can only mark an empty square. If a "
          "square is already filled, they must choose another one.\n"
       << "8. **Game Start**: The game begins with player 1 taking the first "
          "move.\n"
       << "9. **End of Game**: The game ends when either a player wins or all "
          "spaces on the grid are filled.\n"
       << "10. **Game Modes**:\n"
       << "    - Player vs Player: Two human players compete\n"
       << "    - Player vs AI (Easy): AI makes random moves\n"
       << "    - Player vs AI (Hard): AI uses advanced strategy\n";

  char instruction_decision;
  do
  {
    cout << "\n\nPress [m] to go back to main menu 📃; \t [p] if you want to "
            "play the game 🎮";
    instruction_decision = tolower(getch());

    if (instruction_decision != 'm' && instruction_decision != 'p')
    {
      cout << "\n\nInvalid Input, Just input [m] or [p]!" << endl;
    }
  } while (instruction_decision != 'm' && instruction_decision != 'p');

  if (instruction_decision == 'm')
  {
    return;
  }
  else
  {
    clear_screen();
    vector<string> mode_options = {" Player vs Player 👥",
                                   " Player vs AI 🤖 (Easy)",
                                   " Player vs AI 🤖 (Hard)"};

    int mode_choice = display_options(mode_options, "Select Game Mode");
    tictactoe_game(mode_choice > 0, mode_choice == 2);
  }
}

void tictactoe_developer_section()
{
  clear_screen();
  cout << "*****************************************\n"
       << "        TIC-TAC-TOE DEVELOPERS\n"
       << "*****************************************\n"
       << "\nThis game was developed as a C++ project.\n"
       << "\nFeatures:\n"
       << "- Cross-platform support (Windows/Linux)\n"
       << "- Player vs Player mode\n"
       << "- Player vs AI with two difficulty levels\n"
       << "- Clean terminal interface\n"
       << "- Unicode symbol support\n";

  terminal_pause("\nPress ENTER to go back to main menu...");
}

// Snake and Ladder stubs (unchanged)
void snake_and_ladder_game_menu()
{
  int menu_decision = 0;
  clear_screen();
  displaySnakeLadderTitle();
  terminal_pause("\nENTER to continue...\n");
  clear_screen();
  cout << "*******************************************\n"
       << "   👋 WELCOME TO SNAKE 🐍 AND LADDER 🪜\n"
       << "*******************************************\n";
  terminal_pause("Press Enter to Continue...");

  vector<string> main_menu_options = {" Play Game 🕹️", " How to Play ❔",
                                      " Developers ⌨️", " Exit ➡️"};
  
  clear_screen();
  menu_decision = display_options(main_menu_options, "🐍 SNAKE AND LADDER 🪜");

  switch (menu_decision)
  {
  case 0:
    snake_and_ladder_game();
    break;
  case 1:
    snake_and_ladder_how_to_play();
    break;
  case 2:
    snake_and_ladder_developer_section();
    break;
  case 3:
    clear_screen();
    displayThankYou();
    terminal_pause("\n\tPRESS ENTER TO GO BACK TO GAME SELECTION MENU...");
    clear_screen();
    break;
  }
}

void snake_and_ladder_game() {
  
  string board_tile[3][101] = {{"00",
    " 01", " 02", " 03", " 04", " 05", " 06", " 07"," 09", " 09", " 10",
    " 11", " 12", " 13", " 14", " 15", " 16", " 17"," 🪜", " 19", " 20",
    " 21", " 22", " 23", " 🐍", " 25", " 26", " 27", " 28", " 29", " 30",
    " 31", " 32", " 33", " 34", " 35", " 36", " 37", " 38", " 39", " 40",
    " 41", " 42", " 43", " 44", " 🐍", " 46", " 47", " 48", " 49", " 50",
    " 51", " 52", " 53", " 🪜", " 55", " 56", " 57", " 58", " 59", " 60",
    " 🪜", " 62", " 63", " 🪜", " 65", " 66", " 67", " 68", " 69", " 70",
    " 71", " 72", " 73", " 74", " 75", " 🐍", " 77", " 78", " 79", " 80",
    " 81", " 🪜", " 83", " 84", " 85", " 86", " 87", " 88", " 89",  " 🐍",
    " 91", " 92", " 93", " 94", " 95", " 96", " 97", " 98", " 99", "100"}, 
    
    {"00",
      " 01", " 02", " 03", " 04", " 05", " 06", " 07"," 🪜", " 09", " 10",
      " 🐍", " 12", " 🪜", " 14", " 15", " 16", " 17"," 🪜", " 19", " 20",
      " 21", " 22", " 🪜", " 🐍", " 25", " 26", " 27", " 28", " 29", " 30",
      " 31", " 32", " 33", " 34", " 35", " 36", " 37", " 38", " 39", " 40",
      " 41", " 42", " 43", " 44", " 🐍", " 46", " 47", " 🪜", " 49", " 🐍",
      " 51", " 52", " 🪜", " 54", " 55", " 56", " 57", " 🪜", " 59", " 60",
      " 🪜", " 62", " 🪜", " 64", " 65", " 66", " 67", " 68", " 69", " 70",
      " 71", " 72", " 73", " 74", " 75", " 🐍", " 77", " 78", " 79", " 🪜",
      " 🪜", " 82", " 83", " 84", " 🐍", " 86", " 87", " 🐍", " 89",  " 🐍",
      " 91", " 92", " 🐍", " 94", " 95", " 🐍", " 97", " 98", " 🐍", "100"},
    
    {"00",
    " 🪜", " 02", " 03", " 04", " 05", " 06", " 07"," 08", " 🪜", " 10",
    " 🐍", " 🪜", " 13", " 🐍", " 15", " 16", " 17"," 18", " 🪜", " 🪜",
    " 🪜", " 🪜", " 23", " 🐍", " 25", " 26", " 27", " 28", " 29", " 30",
    " 31", " 32", " 33", " 34", " 🐍", " 36", " 37", " 38", " 39", " 40",
    " 41", " 42", " 43", " 44", " 🐍", " 46", " 🪜", " 48", " 49", " 🐍",
    " 51", " 52", " 53", " 🪜", " 55", " 56", " 🪜", " 58", " 59", " 60",
    " 🐍", " 62", " 🐍", " 🪜", " 65", " 66", " 67", " 68", " 69", " 70",
    " 71", " 72", " 🐍", " 74", " 75", " 🐍", " 77", " 🐍", " 79", " 80",
    " 🪜", " 🐍", " 🐍", " 🐍", " 85", " 🐍", " 87", " 🐍", " 🐍",  " 🐍",
    " 91", " 92", " 🐍", " 🐍", " 95", " 🐍", " 🐍", " 98", " 🐍", "100"}};

  
  vector<string> difficulty_options = {"EASY😊", "MEDIUM😕", "HARD🤢"};

  int choosen_board_difficulty = display_options(difficulty_options, "DECIDE THE DIFFICULTY OF THE BOARD💪");

  vector<string> number_of_player_options = {"1 player","2 players", "3 players", "4 players", "5 players", "6 players"};

  string player_names[] = {"PLAYER 1", "Pikselito (COMP)", "Bitoy Byte (COMP)", "Ctrl-Z Santos (COMP)", "AI Delas Alas (COMP)", "Giga Bites (COMP)"};
  string player_avatars[] = {">👺", ">👻", ">😈", ">👽", ">🤖", ">😼"};
  char player_key_roll[] = {'q', 'w', 'e', 'r', 't', 'y'};

  int number_of_players = display_options(number_of_player_options, "DECIDE THE NUMBER OF REAL PLAYERS TO JOIN THE GAME🤼") + 1;
  int number_of_ai_players = 0;
  
  switch(number_of_players){
    case 1:
      {
        vector<string> number_of_ai_player_options = {"1 AI ", "2 AI ", "3 AI ", "4 AI ", "5 AI "};
        number_of_ai_players = display_options(number_of_ai_player_options, "DECIDE THE NUMBER OF AI PLAYERS TO JOIN THE GAME") + 1;
        break;
      }

    case 2:
      {
        vector<string> number_of_ai_player_options = {"NO AI PLAYERS ", "1 AI ", "2 AI ", "3 AI ", "4 AI "};
        number_of_ai_players = display_options(number_of_ai_player_options, "DECIDE THE NUMBER OF AI PLAYERS TO JOIN THE GAME");
        break;
      }
    
    case 3:
      {
        vector<string> number_of_ai_player_options = {"NO AI PLAYERS ", "1 AI ", "2 AI ", "3 AI "};
        number_of_ai_players = display_options(number_of_ai_player_options, "DECIDE THE NUMBER OF AI PLAYERS TO JOIN THE GAME");
        break;
      }
    
    case 4:
      {
        vector<string> number_of_ai_player_options = {"NO AI PLAYERS ", "1 AI ", "2 AI "};
        number_of_ai_players = display_options(number_of_ai_player_options, "DECIDE THE NUMBER OF AI PLAYERS TO JOIN THE GAME");
        break;
      }
    case 5:
      {
        vector<string> number_of_ai_player_options = {"NO AI PLAYERS ", "1 AI "};
        number_of_ai_players = display_options(number_of_ai_player_options, "DECIDE THE NUMBER OF AI PLAYERS TO JOIN THE GAME");
        break;
      }
    case 6:
      number_of_ai_players = 0;
  }

  clear_screen();
  cout<<"********************************************************\n" 
      <<"     BEFORE PLAYING LETS REGISTER PLAYER NAME FIRST\n"
      <<"********************************************************\n\n";
       
  for(int i = 0; i < number_of_players; i++){
    cout << "Player " << i + 1<< " Name: ";
    getline(cin, player_names[i]);
  }

  clear_screen();
  cout << "PLAYER NAMES AND THEIR DESIGNATED AVATARS IN THE BOARD:\n"
       << "▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n\n";

  for(int i = 0; i < (number_of_players + number_of_ai_players); i++){
    cout << "PLAYER " << i+1 << ": " << player_names[i] << " " << player_avatars[i] << endl;
  }
  cout << "\n\n";
  cout << player_names[0] << player_avatars[0] << " will take the first roll 🎲 ,  ";
  terminal_pause("Press ENTER and the game will begins...");

  
  int recent_tile_placement;
  bool player_wins;
  char player_press;
  int player_tile_placement[6] = {0};
  bool is_snake_immune[6] = {false};
  bool is_level_25[6] = {false};
  bool is_level_50[6] = {false};
  bool is_level_75[6] = {false};
  string skill_name[5] = {"💉Immunity💉", "🔄Player Swap🔄", "🎲Additional Dice Roll🎲", "🚀Player Teleport🚀", "🪨Earthquake🪨"};

  string skill_emoji[5] = {"💉", "🔄", "🎲", "🚀", "🪨"};
  
  do{
    player_wins = false;


    for(int i = 0; i < (number_of_players + number_of_ai_players); i++){
      print_snake_and_ladder_board(board_tile[choosen_board_difficulty], choosen_board_difficulty, player_avatars, player_tile_placement, number_of_players + number_of_ai_players);
      
      if(i < number_of_players){
        do{
          cout << "\n\n";
          cout << player_avatars[i] << " " << player_names[i] << "'s Turn Press [" << player_key_roll[i] << "] to roll the dice...\n";  
          player_press = _getch();
  
          if(player_press != player_key_roll[i]){
            cout << "INVALID KEY! PRESS THE CORRECT KEY PLEASE...\n";
  
          }
        }while(player_press != player_key_roll[i]);
      }else{
        gotoxy(65, 12);
        cout << player_avatars[i] <<" "<<player_names[i] << " will roll the dice";
        for(int j = 0; j < 3; j++){
          cout<< ".";
          delay(800);
        }
      }
      recent_tile_placement = player_tile_placement[i];
      player_tile_placement[i] += dice_roller();
      gotoxy(63,22);
      cout << player_names[i] << " " << player_avatars[i] << ": ";
      gotoxy(68,23);
      cout << "    🔹 Recent Tile : " << recent_tile_placement;
      gotoxy(68,24); 
      cout << "    🎲 Tile After Dice Roll: " << player_tile_placement[i] << endl;
      gotoxy(68,26);
      terminal_pause("Press ENTER to continue...");
      
      if(board_tile[choosen_board_difficulty][player_tile_placement[i]] == " 🐍"){
        clear_screen();
        cout << player_names[i] << player_avatars[i] << endl;
        cout << "▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n"; 

        if(is_snake_immune[i]){
          cout << "You are immune to snake🛡️" << endl;
          cout << "Current Tile: " << player_tile_placement[i] << "\t\tTile After Bitten by SNAKE🐍: " << player_tile_placement_checker(player_tile_placement[i], choosen_board_difficulty, is_snake_immune[i]) << endl;
        }else{
          cout << "OH NO YOU GOT BITTEN BY A SNAKE!!! 🤢\n";
          cout << "Current Tile: " << player_tile_placement[i] << "\t\tTile After Bitten by SNAKE🐍: " << player_tile_placement_checker(player_tile_placement[i], choosen_board_difficulty, is_snake_immune[i]) << endl;
        }

        
        
        terminal_pause("\nPress ENTER to continue...");
      }else if(board_tile[choosen_board_difficulty][player_tile_placement[i]] == " 🪜"){
        clear_screen();
        cout << player_names[i] << player_avatars[i] << endl;
        cout << "▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n"; 

        if(is_snake_immune[i]){
          cout << "You are immune to ladder🛡️" << endl;
          cout << "Current Tile: " << player_tile_placement[i] << "\t\tTile After Using the LADDER🪜: " << player_tile_placement_checker(player_tile_placement[i], choosen_board_difficulty, is_snake_immune[i]) << endl;
        }else{
          cout << "YOU'RE LUCKY YOU FOUND A LADDER!!! 🤑\n";
          cout << "Current Tile: " << player_tile_placement[i] << "\t\tTile After Using the LADDER🪜: " << player_tile_placement_checker(player_tile_placement[i], choosen_board_difficulty, is_snake_immune[i]) << endl;
        }

        terminal_pause("\nPress ENTER to continue...");
      }

      player_tile_placement[i] = player_tile_placement_checker(player_tile_placement[i], choosen_board_difficulty, is_snake_immune[i]);

      if(((player_tile_placement[i] > 25) && is_level_25[i] == false) || ((player_tile_placement[i] > 50) && is_level_50[i] == false) || ((player_tile_placement[i] > 75) && is_level_75[i] == false)){
        clear_screen();
        print_snake_and_ladder_board(board_tile[choosen_board_difficulty], choosen_board_difficulty, player_avatars, player_tile_placement, number_of_players + number_of_ai_players);
        

        if(player_tile_placement[i] >= 25 && player_tile_placement[i] < 50){
          is_level_25[i] = true;
          gotoxy(65,12);
          cout << player_names[i] << player_avatars[i]; 
          gotoxy(65,13);
          cout << "You reached tile 25! Claim a one-time skill reward!\n" << endl;
        }else if(player_tile_placement[i] >= 50 && player_tile_placement[i] < 75){
          is_level_50[i] = true;
          gotoxy(65,12);
          cout << player_names[i] << player_avatars[i]; 
          gotoxy(65,13);
          cout << "You reached tile 50! Claim a one-time skill reward!\n" << endl;

        }else if(player_tile_placement[i] >= 75){
          is_level_75[i] = true;
          gotoxy(65,12);
          cout << player_names[i] << player_avatars[i]; 
          gotoxy(65,13);
          cout << "You reached tile 75! Claim a one-time skill reward!\n" << endl;
        }
        
        if(is_level_25[i] || is_level_50[i] || is_level_75){
          int skill_option_1, skill_option_2, skill_option_3;
          
          gotoxy(65, 14);
          terminal_pause("");
          gotoxy(65, 14);
          cout << "               Random Skill Generator                " << endl;
          
          for(int j = 0; j < 8; j++){
            do{
              skill_option_1 = rand() % 5;
              skill_option_2 = rand() % 5;
              skill_option_3 = rand() % 5;
            }while(skill_option_1 == skill_option_2 || skill_option_1 == skill_option_3 || skill_option_2 == skill_option_3);

            gotoxy(87,15);
            cout << skill_emoji[skill_option_1]  << "  " << skill_emoji[skill_option_2]  << "  " << skill_emoji[skill_option_3];
            delay(700);
          }
          gotoxy(63, 17);
          terminal_pause("");
          vector<string> random_skill_options = {skill_name[skill_option_1], skill_name[skill_option_2], skill_name[skill_option_3]};

          int choosen_skill;

          if(i < number_of_players){
            choosen_skill = display_options(random_skill_options, "Choose One Skill", 63, 17);
          }else{
            choosen_skill = rand() % 3;
          }
        
          switch(choosen_skill){
            case 0:
                  choosen_skill = skill_option_1;
                  break;

            case 1: 
                  choosen_skill = skill_option_2;
                  break;

            case 2:
                  choosen_skill = skill_option_3;
                  break;
          }

          if(i >= number_of_players){
            gotoxy(63, 17);
            cout << player_names[i] << player_avatars[i] << " choose " << skill_name[choosen_skill];
            cout <<"\n";
            gotoxy(63, 19);
            terminal_pause("");
          }
          

          if(choosen_skill == 0){
            clear_screen();
            cout << "💉IMMUNITY💉" << endl;
            cout << "▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
            cout <<player_names[i] << player_avatars[i] <<  " choose immunity \nPerks: Immune to Snake\nPower Curse: Immune to ladder\n";
            is_snake_immune[i] = true;
            terminal_pause("");
          }else if(choosen_skill == 1){
            int choosen_player_to_swap;

            if(i < number_of_players){
              vector<string> player_to_swap_options;
              for(int j = 0; j < (number_of_ai_players + number_of_players); j++){
                if(i == j){
                  continue;
                }
  
                player_to_swap_options.push_back(player_avatars[j]);
              }
  
              choosen_player_to_swap = display_options(player_to_swap_options, "CHOOSE PLAYER TO SWAP", 63, 17);
  
              if(choosen_player_to_swap >= i){
                choosen_player_to_swap++;
              }
            }else{
              int best_player_to_swap = 0;

                for(int j = 0; j < (number_of_ai_players + number_of_players); j++){
                  if(i==j) continue;

                  if(best_player_to_swap < player_tile_placement[j]){
                    choosen_player_to_swap = j;
                    best_player_to_swap = player_tile_placement[j];
                  }
                }
            }        
            int temp;

            temp = player_tile_placement[i];
            player_tile_placement[i] = player_tile_placement[choosen_player_to_swap];
            player_tile_placement[choosen_player_to_swap] = temp;

            
            clear_screen();
            print_snake_and_ladder_board(board_tile[choosen_board_difficulty], choosen_board_difficulty, player_avatars, player_tile_placement, number_of_players + number_of_ai_players);
            cout << "🔄Player Swap🔄" << endl;
            cout << "▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n\n";
            cout << "Perks: Can SWAP with the any player\n";
            cout << "Power Curse: If you are the leading player\n\n";

            cout << player_names[i] << player_avatars[i] << endl;
            cout << "▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
            if(i >= number_of_players) cout << "Choosen player to swap: " << player_names[choosen_player_to_swap] << player_avatars[choosen_player_to_swap] << "       TILE: " << player_tile_placement[choosen_player_to_swap] << endl;
            cout << "    🔹 Recent Tile : " << temp;
            cout << "    🔄 Tile After Swap: " << player_tile_placement[i] << endl;
            terminal_pause("");

          }else if(choosen_skill == 2){
            clear_screen();
            print_snake_and_ladder_board(board_tile[choosen_board_difficulty], choosen_board_difficulty, player_avatars, player_tile_placement, number_of_players + number_of_ai_players);

            gotoxy(63,10);
            cout << "🎲ADDITIONAL DICE ROLL🎲" << endl;
            gotoxy(63,11);
            cout << "▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
            recent_tile_placement = player_tile_placement[i];

            player_tile_placement[i] += dice_roller();
            gotoxy(63,22);
            cout << player_names[i] << " " << player_avatars[i] << ": ";
            gotoxy(68,23);
            cout << "    🔹 Recent Tile : " << recent_tile_placement;
            gotoxy(68,24); 
            cout << "    🎲 Tile After Dice Roll: " << player_tile_placement[i] << endl;
            gotoxy(68,26);
            terminal_pause("");

          }else if(choosen_skill == 3){
            clear_screen();
            cout << "PLAYER TELEPORT: " <<  player_names[i] << player_avatars[i] <<endl;
            cout << "▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n";
            terminal_pause("");

            gotoxy(0, 2);
            cout << "                                                             ";
            int number_to_teleport;
            int random_number;
            recent_tile_placement = player_tile_placement[i];

            for(int j = 0; j < 6; j++){
              gotoxy(0, 5);
              number_to_teleport = (player_tile_placement[i] - 5) + (rand() % 20);
              cout << "TILE NUMBER TO TELEPORT: " << number_to_teleport;
              delay(700);
            }

            player_tile_placement[i] = number_to_teleport;

            gotoxy(0,7);
            cout << "    🚀 Tile After Teleport: " << player_tile_placement[i] << endl;
            
            gotoxy(0,8);
            cout << "    🔹 Recent Tile : " << recent_tile_placement;
            gotoxy(0,10);
            terminal_pause("");

          }else if(choosen_skill == 4){
            int choosen_player_to_cast;

            if(i < number_of_players){
              vector<string> player_to_cast_options;
              for(int j = 0; j < (number_of_ai_players + number_of_players); j++){
                if(i == j){
                  continue;
                }
  
                player_to_cast_options.push_back(player_avatars[j]);
              }
  
              choosen_player_to_cast = display_options(player_to_cast_options, "CHOOSE PLAYER TO SWAP", 63, 17);
  
              if(choosen_player_to_cast >= i){
                choosen_player_to_cast++;
              }
            }else{
              int best_player_to_cast = 0;

                for(int j = 0; j < (number_of_ai_players + number_of_players); j++){
                  if(i==j) continue;

                  if(best_player_to_cast < player_tile_placement[j]){
                    choosen_player_to_cast = j;
                    best_player_to_cast = player_tile_placement[j];
                  }
                }
            }

            clear_screen();
            cout << player_names[i] << player_avatars[i] << " choose 🪨Earthquake🪨 and targeted " << player_names[choosen_player_to_cast] << player_avatars[choosen_player_to_cast] << endl;
            cout << "Perks: Targeted player will be down to their current tile up to (0 - 20) number of tile\nCurse: The Caster will also feel the 1/5 of the earthquake\n\n";
          }
          
        }
      }


      if(player_tile_placement[i] == 100){
        player_wins = true;
        cout << "CONGRATULATIONS " << player_names[i] << " YOU ARE THE FIRST TO FINISH THE RACE!!!🎉🍾🎊\n\n\n" << endl;
        int leading_player;
        int second_leading;
        int third_leading;

        leading_player = second_leading = third_leading = INT_MIN;

        for (int j = 0; j < (number_of_players + number_of_ai_players); j++) {
            if (player_tile_placement[j] > leading_player) {
              third_leading = second_leading;
              second_leading = leading_player;
              leading_player = player_tile_placement[j];
            } else if (player_tile_placement[j] > second_leading && player_tile_placement[j] < leading_player) {
                third_leading = second_leading;
                second_leading = player_tile_placement[j];
            } else if (player_tile_placement[j] > third_leading && player_tile_placement[j] < second_leading) {
                third_leading = player_tile_placement[j];
            }
        }

        if((number_of_players + number_of_ai_players) < 3){
          cout << "🌟🌟🌟FINALIST🌟🌟🌟" << endl;
          cout << "🥇     " << player_names[leading_player] << player_avatars[leading_player] << endl;
          cout << "🥈     " << player_names[second_leading] << player_avatars[second_leading] << endl;
        }else{
          cout << "🌟🌟🌟FINALIST🌟🌟🌟" << endl;
          cout << "🥇     " << player_names[leading_player] << player_avatars[leading_player] << endl;
          cout << "🥈     " << player_names[second_leading] << player_avatars[second_leading] << endl;
          cout << "🥉     " << player_names[third_leading] << player_avatars[third_leading] << endl;
        }
        
        terminal_pause("Press ENTER to go back to main menu...");
        clear_screen();
        break;
      }

      if(player_tile_placement[i] > 100){
        clear_screen();
        cout<< "AIGHT YOU GOTTA LAND ON TILE 100 WITH THE EXACT NUMBER OF MOVES🐾🐾\n\n" 
            <<"CURRENT TILE: " << player_tile_placement[i] << "\t\tTILE FOR GOING BACKWARDS FOR EXCESS STEPS: " << 100 - (player_tile_placement[i] % 100) << endl;
        player_tile_placement[i] = 100 - (player_tile_placement[i] % 100);
      }
      
      if(board_tile[choosen_board_difficulty][player_tile_placement[i]] == " 🐍"){
        clear_screen();
        cout << player_names[i] << player_avatars[i] << endl;
        cout << "▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n"; 

        if(is_snake_immune[i]){
          cout << "You are immune to snake🛡️" << endl;
          cout << "Current Tile: " << player_tile_placement[i] << "\t\tTile After Bitten by SNAKE🐍: " << player_tile_placement_checker(player_tile_placement[i], choosen_board_difficulty, is_snake_immune[i]) << endl;
        }else{
          cout << "OH NO YOU GOT BITTEN BY A SNAKE!!! 🤢\n";
          cout << "Current Tile: " << player_tile_placement[i] << "\t\tTile After Bitten by SNAKE🐍: " << player_tile_placement_checker(player_tile_placement[i], choosen_board_difficulty, is_snake_immune[i]) << endl;
        }

        
        
        terminal_pause("\nPress ENTER to continue...");
      }else if(board_tile[choosen_board_difficulty][player_tile_placement[i]] == " 🪜"){
        clear_screen();
        cout << player_names[i] << player_avatars[i] << endl;
        cout << "▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬▬\n"; 

        if(is_snake_immune[i]){
          cout << "You are immune to ladder🛡️" << endl;
          cout << "Current Tile: " << player_tile_placement[i] << "\t\tTile After Using the LADDER🪜: " << player_tile_placement_checker(player_tile_placement[i], choosen_board_difficulty, is_snake_immune[i]) << endl;
        }else{
          cout << "YOU'RE LUCKY YOU FOUND A LADDER!!! 🤑\n";
          cout << "Current Tile: " << player_tile_placement[i] << "\t\tTile After Using the LADDER🪜: " << player_tile_placement_checker(player_tile_placement[i], choosen_board_difficulty, is_snake_immune[i]) << endl;
          terminal_pause("\nPress ENTER to continue...");
        }
        
      }

      player_tile_placement[i] = player_tile_placement_checker(player_tile_placement[i], choosen_board_difficulty, is_snake_immune[i]);

    }


    if(player_wins){
      return;
    }
  }while(!player_wins);
  


  

}
void print_table_boarder(){
  cout << "+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+";
}



void print_snake_and_ladder_board(string board_tile[], int difficulty, string player_avatars[], int player_tile_placement[], int total_players) {
  clear_screen();

  string avatar_board_tile[101];

  for(int i = 0; i < 101; i++){
    avatar_board_tile[i] = board_tile[i];
  }

  for(int i = 0; i < 6; i++){
    avatar_board_tile[player_tile_placement[i]] = player_avatars[i];
  }

  int leading_player;
  int second_leading;
  int third_leading;

  leading_player = second_leading = third_leading = INT_MIN;

  for (int i = 0; i < 6; i++) {
      if (player_tile_placement[i] > leading_player) {
        third_leading = second_leading;
        second_leading = leading_player;
        leading_player = player_tile_placement[i];
    } else if (player_tile_placement[i] > second_leading && player_tile_placement[i] < leading_player) {
        third_leading = second_leading;
        second_leading = player_tile_placement[i];
    } else if (player_tile_placement[i] > third_leading && player_tile_placement[i] < second_leading) {
        third_leading = player_tile_placement[i];
  }
  }




  switch(difficulty){
    case 0:
      cout<< "            **************************************\n" 
          << "                 🐍SNAKE AND LADDER🪜 (EASY)\n"
          << "            **************************************\n";
          break;

    case 1:
      cout<< "          ****************************************\n" 
          << "               🐍SNAKE AND LADDER🪜 (MEDIUM)\n"
          << "          ****************************************\n";
          break;

    case 2:
      cout<< "            **************************************\n" 
          << "                 🐍SNAKE AND LADDER🪜 (HARD)\n"
          << "            **************************************\n";
  }



  int tile_end = 100;

  for(int i = 1; i <= 21  ; i++){
    if(i % 2 == 1){
        print_table_boarder();
        if(i > (total_players + 1)){
          cout << "\n";
        }
    }else if(i % 2 == 0 && i % 4 != 0){
        for(int j = tile_end; j > (tile_end - 10); j--){
            cout << "| " << avatar_board_tile[j] << " ";
        }
        cout << "|";
        if(i > (total_players + 1)){
          cout << "\n";
        }
        tile_end -= 10;

    }else{
        for(int j = (tile_end - 9); j <= tile_end; j++){
            cout << "| " << avatar_board_tile[j] << " ";
        }
        cout << "|";
        if(i > (total_players + 1)){
          cout << "\n";
        }
        tile_end -= 10;

    }

    
    if(i == 1){
      cout<< "\t\tCURRENT PLACEMENT\n";
    }

    if(i == 2 && i <= total_players + 1){
      cout << "\t\t" << player_avatars[0] << " TILE: " << player_tile_placement[0];
      if(player_tile_placement[0] > 0){
        if(player_tile_placement[0] == leading_player){
          cout << "\t🥇";
        }else if(player_tile_placement[0] == second_leading){
          cout << "\t🥈";
        }else if(player_tile_placement[0] == third_leading){
          cout << "\t🥉";
        }
      }
  
      cout << "\n";
    }
    
    if(i == 3 && i <= total_players + 1){
      cout << "\t\t" << player_avatars[1] << " TILE: " << player_tile_placement[1];
      if(player_tile_placement[1] > 0){
        if(player_tile_placement[1] == leading_player){
          cout << "\t🥇";
        }else if(player_tile_placement[1] == second_leading){
          cout << "\t🥈";
        }else if(player_tile_placement[1] == third_leading){
          cout << "\t🥉";
        } 
      }
      cout << "\n";
    }
    

    if(i == 4 && i <= total_players + 1){
      cout << "\t\t" << player_avatars[2] << " TILE: " << player_tile_placement[2];
      if(player_tile_placement[2] > 0){
        if(player_tile_placement[2] == leading_player){
          cout << "\t🥇";
        }else if(player_tile_placement[2] == second_leading){
          cout << "\t🥈";
        }else if(player_tile_placement[2] == third_leading){
          cout << "\t🥉";
        }
      }
      cout << "\n";
    }
    

    if(i == 5 && i <= total_players + 1){
      cout << "\t\t" << player_avatars[3] << " TILE: " << player_tile_placement[3];
      if(player_tile_placement[3] > 0){
        if(player_tile_placement[3] == leading_player){
          cout << "\t🥇";
        }else if(player_tile_placement[3] == second_leading){
          cout << "\t🥈";
        }else if(player_tile_placement[3] == third_leading){
          cout << "\t🥉";
        }
      }
      
      cout << "\n";
    }
    

    if(i == 6 && i <= total_players + 1){
      cout << "\t\t" << player_avatars[4] << " TILE: " << player_tile_placement[4];
      if(player_tile_placement[4] > 0){
        if(player_tile_placement[4] == leading_player){
          cout << "\t🥇";
        }else if(player_tile_placement[4] == second_leading){
          cout << "\t🥈";
        }else if(player_tile_placement[4] == third_leading){
          cout << "\t🥉";
        }
      }
      cout << "\n";
    }

    if(i == 7 && i <= total_players + 1){
      cout << "\t\t" << player_avatars[5] << " TILE: " << player_tile_placement[5];
      if(player_tile_placement[5] > 0){
        if(player_tile_placement[5] == leading_player){
          cout << "\t🥇";
        }else if(player_tile_placement[5] == second_leading){
          cout << "\t🥈";
        }else if(player_tile_placement[5] == third_leading){
          cout << "\t🥉";
        }
      }
      cout << "\n";
    }
    

}
  
}

int dice_roller() {
  int dice_number;

  for(int i = 0; i < 10; i++){
    gotoxy(65,12);
    cout << "Rolling the dice...                               \n";
    dice_number = (rand() % 6) + 1;
    display_dice_face(dice_number);
    this_thread::sleep_for(chrono::milliseconds(200));
  }

  for(int i = 0; i < 3; i++){ 
    gotoxy(65,12);
    cout << "Rolling the dice...                               \n";
    dice_number = (rand() % 6) + 1;
    display_dice_face(dice_number);
    this_thread::sleep_for(chrono::milliseconds(500));
  }
  
  gotoxy(65,12);  
  cout << "Final Dice Roll 🏁:                                 \n";
  display_dice_face(dice_number);
  gotoxy(65,19);
  cout << "You got 🎲 " << dice_number << endl;
  return dice_number;
}
void display_dice_face(int dice_number) {
  string dice_faces[] = {
      "\t\t\t\t\t\t\t\t\t\t" + RED_BG + WHITE + "┌─────────┐\n" +
          "\t\t\t\t\t\t\t\t\t\t│         │\n" + "\t\t\t\t\t\t\t\t\t\t│    " +
          WHITE + "●" + WHITE + "    │\n" +
          "\t\t\t\t\t\t\t\t\t\t│         │\n" +
          "\t\t\t\t\t\t\t\t\t\t└─────────┘" + RESET,

      "\t\t\t\t\t\t\t\t\t\t" + RED_BG + WHITE + "┌─────────┐\n" +
          "\t\t\t\t\t\t\t\t\t\t│ " + WHITE + "●" + WHITE + "       │\n" +
          "\t\t\t\t\t\t\t\t\t\t│         │\n" + "\t\t\t\t\t\t\t\t\t\t│       " +
          WHITE + "●" + WHITE + " │\n" + "\t\t\t\t\t\t\t\t\t\t└─────────┘" +
          RESET,

      "\t\t\t\t\t\t\t\t\t\t" + RED_BG + WHITE + "┌─────────┐\n" +
          "\t\t\t\t\t\t\t\t\t\t│ " + WHITE + "●" + WHITE + "       │\n" +
          "\t\t\t\t\t\t\t\t\t\t│    " + WHITE + "●" + WHITE + "    │\n" +
          "\t\t\t\t\t\t\t\t\t\t│       " + WHITE + "●" + WHITE + " │\n" +
          "\t\t\t\t\t\t\t\t\t\t└─────────┘" + RESET,

      "\t\t\t\t\t\t\t\t\t\t" + RED_BG + WHITE + "┌─────────┐\n" +
          "\t\t\t\t\t\t\t\t\t\t│ " + WHITE + "●" + WHITE + "     " + WHITE +
          "●" + WHITE + " │\n" + "\t\t\t\t\t\t\t\t\t\t│         │\n" +
          "\t\t\t\t\t\t\t\t\t\t│ " + WHITE + "●" + WHITE + "     " + WHITE +
          "●" + WHITE + " │\n" + "\t\t\t\t\t\t\t\t\t\t└─────────┘" + RESET,

      "\t\t\t\t\t\t\t\t\t\t" + RED_BG + WHITE + "┌─────────┐\n" +
          "\t\t\t\t\t\t\t\t\t\t│ " + WHITE + "●" + WHITE + "     " + WHITE +
          "●" + WHITE + " │\n" + "\t\t\t\t\t\t\t\t\t\t│    " + WHITE + "●" +
          WHITE + "    │\n" + "\t\t\t\t\t\t\t\t\t\t│ " + WHITE + "●" + WHITE +
          "     " + WHITE + "●" + WHITE + " │\n" +
          "\t\t\t\t\t\t\t\t\t\t└─────────┘" + RESET,

      "\t\t\t\t\t\t\t\t\t\t" + RED_BG + WHITE + "┌─────────┐\n" +
          "\t\t\t\t\t\t\t\t\t\t│ " + WHITE + "●" + WHITE + "     " + WHITE +
          "●" + WHITE + " │\n" + "\t\t\t\t\t\t\t\t\t\t│ " + WHITE + "●" +
          WHITE + "     " + WHITE + "●" + WHITE + " │\n" +
          "\t\t\t\t\t\t\t\t\t\t│ " + WHITE + "●" + WHITE + "     " + WHITE +
          "●" + WHITE + " │\n" + "\t\t\t\t\t\t\t\t\t\t└─────────┘" + RESET};

  cout << dice_faces[dice_number - 1] << endl;
}

int player_tile_placement_checker(int player_tile_placement, int difficulty, bool is_immune){
  if(is_immune){
    return player_tile_placement;
  }else if(difficulty == 0){
      if(player_tile_placement == 18){
          return 23;
      }else if(player_tile_placement == 24){
          return 17;
      }else if(player_tile_placement == 45){
          return 36;
      }else if(player_tile_placement == 54){
          return 67;
      }else if(player_tile_placement == 64){
          return 77;
      }else if(player_tile_placement == 61){
          return 80;
      }else if(player_tile_placement == 76){
          return 65;
      }else if(player_tile_placement == 82){
          return 99;
      }else if(player_tile_placement == 90){
          return 71;
      }
  }else if(difficulty == 1){
      if(player_tile_placement == 8 || player_tile_placement == 13){
          return 28;
      }else if(player_tile_placement == 11){
          return 10;
      }else if(player_tile_placement == 18 || player_tile_placement == 23){
          return 38;
      }else if(player_tile_placement == 24){
          return 17;
      }else if(player_tile_placement == 50){
          return 31;
      }else if(player_tile_placement == 48 || player_tile_placement == 53){
          return 68;
      }else if(player_tile_placement == 45){
          return 36;
      }else if(player_tile_placement == 58 || player_tile_placement == 63){
          return 78;
      }else if(player_tile_placement == 61 || player_tile_placement == 80 || player_tile_placement == 81){
          return 100;
      }else if(player_tile_placement == 90){
          return 71;
      }else if(player_tile_placement == 93 || player_tile_placement == 88){
          return 73;
      }else if(player_tile_placement == 96 || player_tile_placement == 85 || player_tile_placement == 76){
          return 65;
      }else if(player_tile_placement == 99){
          return 82;
      }
  }else if (difficulty == 2){
      if(player_tile_placement == 99 ||  player_tile_placement == 82){
        return 79;
      }else if(player_tile_placement == 97 || player_tile_placement == 84){
        return 77;
      }else if(player_tile_placement == 96){
        return 85;
      }else if(player_tile_placement == 94){
        return 87;
      }else if(player_tile_placement == 93 || player_tile_placement == 88 || player_tile_placement == 73){
        return 68;
      }else if(player_tile_placement == 90){
        return 71;
      }else if(player_tile_placement == 89){
        return 72;
      }else if(player_tile_placement == 86){
        return 75;
      }else if(player_tile_placement == 81){
        return 100;
      }else if(player_tile_placement == 76){
        return 65;
      }else if(player_tile_placement == 61){
        return 60;
      }else if(player_tile_placement == 83 || player_tile_placement == 78 || player_tile_placement == 63){
        return 58;
      }else if(player_tile_placement == 64 || player_tile_placement == 57){
        return 77;
      }else if(player_tile_placement == 54 || player_tile_placement == 47){
        return 67;
      }else if(player_tile_placement == 50){
        return 31;
      }else if(player_tile_placement == 45){
        return 36;
      }else if(player_tile_placement == 35){
        return 26;
      }else if(player_tile_placement == 24){
        return 17;
      }else if(player_tile_placement == 14){
        return 7;
      }else if(player_tile_placement == 11){
        return 10;
      }else if(player_tile_placement == 9 || player_tile_placement == 12){
        return 29;
      }else if(player_tile_placement == 19 || player_tile_placement == 22){
        return 39;
      }else if(player_tile_placement == 1 || player_tile_placement == 20 || player_tile_placement == 21){
        return 40;
      }
  
  }else{
    return player_tile_placement; 
  }


  return player_tile_placement;
}
void print_big_DEVELOPERS() {
  string developers[] = {
      "DDDD   EEEEE  V   V  EEEEE  L       OOO   PPPP   EEEEE  RRRR    SSSS",
      "D   D  E      V   V  E      L      O   O  P   P  E      R   R  S    ",
      "D   D  EEEE   V   V  EEEE   L      O   O  PPPP   EEEE   RRRR    SSS ",
      "D   D  E      V   V  E      L      O   O  P      E      R  R      S  ",
      "DDDD   EEEEE   VVV   EEEEE  LLLLL   OOO   P      EEEEE  R   R   SSSS",
  };

}

void snake_and_ladder_how_to_play() {
  clear_screen();
  cout <<"OBJECTIVE OF THE GAME: \n\n";
    cout << "The objective of the game is to be the first player to reach the\n"
      "end of the board (square 100) by rolling a die\n and moving "
      "your game piece accordingly.\n\n";

cout << "BASIC SET-UP:\n\n";
    cout << "1. The game is played on a board with numbered squares from 1 to "
      "100.\n";
    cout << "2. Each player has a game piece that starts at square 1.\n";
    cout << "3. Players take turns rolling a die to determine how many squares "
      "to move.\n\n";

cout << "GAME PLAY:\n\n";
    cout << "1. On your turn, roll the die and move your game piece forward the "
      "number of squares rolled.\n";
    cout << "2. If you land on a square with the bottom of a ladder, you can "
      "climb up to the top of the ladder.\n";
    cout << "3. If you land on a square with the head of a snake, you must slide "
      "down to the tail of the snake.\n";
    cout << "4. The first player to reach square 100 wins the game.\n\n";

cout << "WINNING THE GAME: \n\n";
    cout << "1. The first player to reach square 100 wins the game.\n";
    cout << "2. If a player rolls a number that would take them past square "
      "100,\nthey must stay on their current square until they roll the exact number to land on the square 100.\n\n";
cout << "ADDITIONAL RULES:\n\n";
    cout << "**If you reach 25 tiles or up, 50 tiles or up and 75 tiles or up, you can choose one skill "
            "out of 3 randomized options**\n\n";

cout << "SKILLS:\n\n";

cout << "IMMUNITY:\n"
"Effective: Whole rounds\n"
"perks: Immune to snake\n"
"cons: immune also to ladder\n\n";

cout << "PLAYER SWAP:\n"
"Effective: One time use.\n"
"perks: you can swap with any player you want.\n"
"cons: if you are the leading player.\n\n";

//cout << "EARTHQUAKE:\n"
//"Effective: One time use\n"
//"perks: you can force a player to step down with a random number of tiles.\n"
//"cons: you're going also to step down 1/4 to how many tiles the player you chose to step down.\n\n";

cout << "PLAYER TELEPORT(Self-cast):\n"
"Effective: One time use\n"
"Perks: you can step up with 10 tiles.\n"
"Cons: There's also a chance you can step down 5 tiles.\n";
    
    terminal_pause("Press ENTER to go back to main menu...");
    clear_screen();
    snake_and_ladder_game_menu();
    return;
}
void snake_and_ladder_developer_section() {
  clear_screen();

   //calling the function to print the DEVELOPERS in big font
  print_big_DEVELOPERS();
  cout <<"\n\n";
  //Print the developer names
cout << "Andrie Timothy R. Cabuguas\nStatus: Double\n";
cout << "- Talk is cheap. Show me the code.\n\n";
  cout << "Asdi V. Amamence\nStatus: Double\n";
cout << "- Computers are fast; developers keep them slow.\n\n";
  cout << "John Kennith A. Madera\nStatus: Importante Buhi\n";
cout << "- It's not a BUG, it's a FEATURE.\n\n";
  cout << "Krisna Hojland\nStatus: Double\n";
cout << "- If debugging is the process of removing bugs, then programming must be the process of putting them in.\n";

terminal_pause("\nPress ENTER to go back to main menu...");
clear_screen();
snake_and_ladder_game_menu();
return;
}

void displayThankYou()
{
  cout << endl;
  string thankYouArt[] = {"████████╗██╗  ██╗ █████╗ ███╗   ██╗██╗  ██╗    ██╗  "
                          " ██╗ ██████╗ ██╗   ██╗",
                          "╚══██╔══╝██║  ██║██╔══██╗████╗  ██║██║ ██╔╝    ╚██╗ "
                          "██╔╝██╔═══██╗██║   ██║",
                          "   ██║   ███████║███████║██╔██╗ ██║█████╔╝      "
                          "╚████╔╝ ██║   ██║██║   ██║",
                          "   ██║   ██╔══██║██╔══██║██║╚██╗██║██╔═██╗       "
                          "╚██╔╝  ██║   ██║██║   ██║",
                          "   ██║   ██║  ██║██║  ██║██║ ╚████║██║  ██╗       "
                          "██║   ╚██████╔╝╚██████╔╝",
                          "   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝       "
                          "╚═╝    ╚═════╝  ╚═════╝ "};

  for (size_t i = 0; i < 6; ++i)
  {
    float ratio = static_cast<float>(i) / 5.0f;
    string color;
    switch (i)
    {
    case 0:
      color =
          interpolateColor(75, 0, 130, 255, 0, 255, ratio); // Purple → Magenta
      break;
    case 1:
      color = interpolateColor(255, 0, 255, 255, 20, 147,
                               ratio); // Magenta → Hot Pink
      break;
    case 2:
      color = interpolateColor(255, 20, 147, 255, 182, 193,
                               ratio); // Hot Pink → Light Pink
      break;
    case 3:
      color = interpolateColor(255, 182, 193, 255, 218, 185,
                               ratio); // Light Pink → Peach
      break;
    case 4:
      color =
          interpolateColor(255, 218, 185, 255, 165, 0, ratio); // Peach → Orange
      break;
    case 5:
      color = interpolateColor(255, 165, 0, 255, 69, 0,
                               ratio); // Orange → Deep Orange
      break;
    }
    cout << color << thankYouArt[i] << "\033[0m" << endl;
  }
  cout << endl;
}
