#include <iostream>
#include <string>
#include <conio.h>
#include <limits>
#include <cctype>
#include <windows.h>

//GUYS KINDLY CHECK THE SESSION CHAT FOR DOCUMENTATION INSTRUCTIONS THANK YOU; HAPPY CODING AsdiiIGOP and KennethSAKALAM😊!

using namespace std;


void clear_input_buffer();
void clear_screen();
void cursor_hide();
void terminal_pause(string prompt);
int mismatch_data();


void arrow_options_animation(string options[], int number_of_options, int selected);
int display_options(string options[], string title, int number_of_options);


void tictactoe_game_menu();
void tictactoe_game();
void print_tictactoe_board_score(string board_mark[], int player1_score, int player2_score, string player1_name, string player2_name);
void tictactoe_how_to_play();
void tictactoe_developer_section();


void snake_and_ladder_game_menu();
void snake_and_ladder_game();
void print_snake_and_ladder_board();
void snake_and_ladder_how_to_play();
void snake_and_ladder_developer_section();




int main(){
    SetConsoleOutputCP(65001);
    string game_options[] = {" Tictactoe ❌🔵", " Snake and Ladder 🪜🐍"};

    while (true)
    {
        int game_selected = display_options(game_options, "Please Select a Game", 2);

        switch (game_selected)
        {
            case  0:
                tictactoe_game_menu();
                break;
        
            case  1:
                snake_and_ladder_game_menu();
                break;
        }
    }
    
      
    terminal_pause("");
    return 0;
}






void clear_input_buffer(){
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void clear_screen(){
    system("cls");
}

void cursor_hide(){
    cout << "\e[?25l";
}

void cursor_show(){
    cout << "\e[?25h";
}

void terminal_pause(string prompt){
    cout << prompt;
    clear_input_buffer();
    cin.get();
}

void arrow_options_animation(string options[], int number_of_options, int selected){
    for(int i = 0; i < number_of_options; i++){
        if(i == selected){
            cout << ">";
        }else{
            cout << " ";
        }

        cout << options[i] << endl;
    }
}

int mismatch_data(){
    if(cin.fail()){
        cin.clear();
        clear_input_buffer();

        cout << "INVALID INPUT!" << endl;
        return 1;
    }

    return 0;
}

int display_options(string options[], string title, int number_of_options){
    int title_lenght = title.length();
    int selected = 0;
    char key;

    while(true){
        clear_screen();
        cursor_hide();
        for(int i = 0; i < (title_lenght + 16); i++) cout <<"*";
        cout << "\n";
        for(int i = 0; i < 8; i++) cout <<" ";
        cout << title << endl;
        for(int i = 0; i < (title_lenght + 16); i++) cout <<"*";

        cout << "\n";

        arrow_options_animation(options, number_of_options, selected);
        key = _getch();  // Get user input

        if (key == -32) {  // Arrow keys return two values, first is -32
            key = _getch();  // Get actual key code
            if (key == 72) {  // Up arrow
                selected = (selected - 1 + number_of_options) % number_of_options;  // Wrap around
            } else if (key == 80) {  // Down arrow
                selected = (selected + 1) % number_of_options;  // Wrap around
            }
        } else if (key == '\r') {  // Enter key
            break;
        }
    }


    return selected;
}




void tictactoe_game_menu(){
    int menu_decision = 0;

    clear_screen();
    cout<< "*****************************************\n" 
        << "    👋 WELCOME TO ❌TICTACTOE GAME🔵\n"
        << "*****************************************\n";

    cout << "Press Enter to Continue...";
    cin.get();

    clear_screen();

    string main_menu_options[] = {" Play Game 🕹️", " How to Play ❔", " Developers ⌨️", " Exit ➡️"};
    menu_decision = display_options(main_menu_options, "❌ TICTACTOE GAME 🔵", 4);

    switch (menu_decision)
    {
    case 0:
        tictactoe_game();
        break;
    
    case 1:
        tictactoe_how_to_play();
        break;
    
    case 2:
        tictactoe_developer_section();
        break;
    
    case 3:
        clear_screen();
        cout << "\n\n\tTHANK YOU FOR PLAYING OUR GAME 🙏💖" << endl;
        terminal_pause("\n\tPRESS ENTER TO GO BACK TO GAME SELECTION MENU...");
        break;
    }

}

void tictactoe_game(){
    char read_instruction_response;
    int player1_move, player2_move;
    string player1_name, player2_name;
    int player1_score = 0, player2_score = 0;

    do{
        clear_screen();
        cout<< "**********************************************************\n" 
        << "        PLEASE READ THE ""HOW TO PLAY"" MENU FIRST\n"
        << "**********************************************************\n"
        << "Reading this game instructions before playing is a must if \n"
        << "you already read the instructions press 'p' to proceed and  \n"
        << "       if not press 'r' to read the instructions\n"
        << "\n\n\tProceed [p]\t\tRead Instructions [r]\n";

        read_instruction_response = _getch();

        read_instruction_response = tolower(read_instruction_response);

        if(read_instruction_response == 'r'){
            tictactoe_how_to_play();
        }

        if(read_instruction_response != 'r' && read_instruction_response != 'p'){
            terminal_pause("Invalid input please read the prompt carefully,\nPress enter to continue...");
        }

    }while( read_instruction_response != 'r' && read_instruction_response != 'p');

    clear_screen();

    cout << "---Before starting the game lets register each player's name first---\n\n";
    cursor_show();
    cout << "    Enter the 1st player name: ";
    cin >> player1_name;

    cout << "\n";

    cout << "    Enter the 2nd player name: ";
    cin >> player2_name;
    cursor_hide();

    clear_screen();

    cout << "Welcome " << player1_name << " and " << player2_name << " please enjoy the game😊\n";
    cout << player1_name << " will take the first move. ";
    terminal_pause("Press ENTER and the game will begin...");

    clear_screen();


    do{
        int current_number_of_move = 0;
        string board_mark[9] = {" 1", " 2", " 3", " 4", " 5", " 6", " 7", " 8", " 9"};


        print_tictactoe_board_score(board_mark, player1_score, player2_score, player1_name, player2_name);
        do{
            do{
                cout << player1_name << "'s move: ";
                cin >> player1_move;
    
                if(board_mark[player1_move - 1] == "❌"){
                    cout << "You already marked the tile number " << player1_move << " please choose another tile!" << endl;
                }else if(board_mark[player1_move - 1] == "🔵"){
                    cout << player2_name <<" already marked the tile number " << player1_move << " please choose another tile!" << endl;
                }else if(player1_move < 1 || player1_move > 9){
                    cout << "Invalid Input please choose unassigned tile ranging from 1-9" << endl;
                }
                
            }while(mismatch_data() == 1 || board_mark[player1_move - 1] == "❌" || board_mark[player1_move - 1] == "🔵" || (player1_move < 1 || player1_move > 9));
                clear_screen();
            
    
            board_mark[player1_move - 1] = "❌";
            current_number_of_move++;
    
            if( (board_mark[0] == board_mark[1] && board_mark[0] == board_mark[2]) || (board_mark[0] == board_mark[3] && board_mark[0] == board_mark[6]) || (board_mark[0] == board_mark[4] && board_mark[4] == board_mark[8]) || (board_mark[6] == board_mark[7] && board_mark[6] == board_mark[8]) || (board_mark[2] == board_mark[5] && board_mark[2] == board_mark[8]) || (board_mark[2] == board_mark[4] && board_mark[2] == board_mark[6]) || (board_mark[3] == board_mark[4] && board_mark[3] == board_mark[5] || (board_mark[1] == board_mark[4] && board_mark[1] == board_mark[7]))){
                print_tictactoe_board_score(board_mark, player1_score, player2_score, player1_name, player2_name);
                cout << "CONGRATULATIONS!!! " << player1_name << " Winsss!" << endl;
                terminal_pause("Press enter to proceed to next round...");
                player1_score++;
                clear_screen();
                break;
                }
            print_tictactoe_board_score(board_mark, player1_score, player2_score, player1_name, player2_name);

            if(current_number_of_move == 9){
                cout << "⚠️ DRAW!!!" << endl;
                terminal_pause("Press enter to start a new round..");
                clear_screen();
                break;
            }
    
    
            do{
                cout << player2_name << "'s move: ";
                cin >> player2_move;
    
                if(board_mark[player2_move - 1] == "🔵"){
                    cout << "You already marked the tile number " << player2_move << " please choose another tile!" << endl;
                }else if(board_mark[player2_move - 1] == "❌"){
                    cout << player1_name <<" already marked the tile number " << player2_move << " please choose another tile!" << endl;
                }else if(player2_move < 1 || player2_move > 9){
                    cout << "Invalid Input please choose unassigned tile ranging from 1-9" << endl;
                }
                
            }while(mismatch_data() == 1 || board_mark[player2_move - 1] == "❌" || board_mark[player2_move - 1] == "🔵" || (player2_move < 1 || player2_move > 9));
            clear_screen();
        
            board_mark[player2_move - 1] = "🔵";
            current_number_of_move++;
    
           
            if( (board_mark[0] == board_mark[1] && board_mark[0] == board_mark[2]) || (board_mark[0] == board_mark[3] && board_mark[0] == board_mark[6]) || (board_mark[0] == board_mark[4] && board_mark[4] == board_mark[8]) || (board_mark[6] == board_mark[7] && board_mark[6] == board_mark[8]) || (board_mark[2] == board_mark[5] && board_mark[2] == board_mark[8]) || (board_mark[2] == board_mark[4] && board_mark[2] == board_mark[6]) || (board_mark[3] == board_mark[4] && board_mark[3] == board_mark[5] || (board_mark[1] == board_mark[4] && board_mark[1] == board_mark[7])) ){
                print_tictactoe_board_score(board_mark, player1_score, player2_score, player1_name, player2_name);
                cout << "CONGRATULATIONS!!! " << player2_name << " Winsss!" << endl;
                terminal_pause("Press enter to proceed to next round...");
                player2_score++;
                clear_screen();
                break;
                }
            print_tictactoe_board_score(board_mark, player1_score, player2_score, player1_name, player2_name);
    
    
        }while(current_number_of_move < 9);

        if(player1_score == 3){
            cout << "CONGRATULATIONS🎊 " << player1_name << " YOU WIN🎉🎉🎉, YOU OUTSMARTED " << player2_name << " 🍾\n";
            terminal_pause("Press ENTER to go back to TICTACTOE main menu...");
            tictactoe_game_menu();
            break;
        }else if(player1_score == 3){
            cout << "CONGRATULATIONS🎊 " << player2_name << " YOU WIN🎉🎉🎉, YOU OUTSMARTED " << player1_name << " 🍾\n";
            terminal_pause("Press ENTER to go back to TICTACTOE main menu...");
            tictactoe_game_menu();
            break;
        }

    }while( player1_score < 3 && player2_score < 3 );

    
        
    terminal_pause("");

}

void print_tictactoe_board_score(string board_mark[], int player1_score, int player2_score, string player1_name, string player2_name){
    cout<<"   " << board_mark[0] << " | " << board_mark[1] << " | " << board_mark[2] <<    "\t\t\t===========================" << endl
            <<"  ----+----+----"<<                                                         "\t\t      🔢 SCORE BOARD 🔢"         << endl
            <<"   " << board_mark[3] << " | " << board_mark[4] << " | " << board_mark[5] <<"\t\t\t===========================" <<endl
            <<"  ----+----+----"<<                                                         "\t\t" << player1_name << ": " << player1_score << endl  
            <<"   " << board_mark[6] << " | " << board_mark[7] << " | " << board_mark[8] <<"\t\t\t" << player2_name << ": " << player2_score <<endl;
            cout << "\n\n";

};

void tictactoe_how_to_play(){
    clear_screen();
    char instruction_decision;
    
    cout << "\nMechanics in playing TicTacToe Game: " << endl;
	
	cout << "\n1. **Players**: Two players are required to play. " << endl;
  
    cout << "2. **Game Grid**: The game is played on a 3x3 grid, making up 9 spaces in total." << endl;
    
    cout << "3. **Turn Order**: Players take turns marking an empty space on the grid " << endl;
  
    cout << "4. **Objective**: The goal is to be the first player to get three of their symbols in a row, column, or diagonal." << endl;
    
    cout << "5. **Winning Condition**: A player wins if they have three of their symbols in a horizontal, vertical, or diagonal line. " << endl;
  
    cout << "6. **Tie Condition**: The game ends in a draw if all 9 spaces are filled and no player has won." << endl;
    
    cout << "7. **Valid Move**: A player can only mark an empty square. If a square is already filled, they must choose another one." << endl;
  
    cout << "8. **Game Start**: The game begins with player 1 taking the first move." << endl;

    cout << "9. **End of Game**: The game ends when either a player wins or all spaces on the grid are filled." << endl;

    do{
        cout << "\n\nPress [m] to go back to main menu 📃; \t [p] if you want to play the game 🎮";
        instruction_decision = _getch();

        if(instruction_decision != 'm' && instruction_decision != 'p'){
            cout << "\n\nInvalid Input, Just input [m] or [p]!" << endl;
        }

    }while(instruction_decision != 'm' && instruction_decision != 'p');

    if(instruction_decision == 'm'){
        tictactoe_game_menu();
    }else if(instruction_decision == 'p'){
        tictactoe_game();
    }
    
    terminal_pause("No instructions yet...");
    
}

void tictactoe_developer_section(){

}




void snake_and_ladder_game_menu(){
    int menu_decision = 0;

    clear_screen();
    cout<< "*******************************************\n" 
        << "   👋 WELCOME TO SNAKE 🐍 AND LADDER 🪜\n"
        << "*******************************************\n";

    cout << "Press Enter to Continue...";
    cin.get();

    clear_screen();

    do{
        string main_menu_options[] = {" Play Game 🕹️", " How to Play ❔", " Developers ⌨️", " Exit ➡️"};
        menu_decision = display_options(main_menu_options, "🐍 SNAKE AND LADDER 🪜", 4);

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
            cout << "\n\n\tTHANK YOU FOR PLAYING OUR GAME 🙏💖" << endl;
            terminal_pause("\n\tPRESS ENTER TO GO BACK TO GAME SELECTION MENU...");
            break;
        }

    }while(menu_decision != 3);

    
}

void snake_and_ladder_game(){

}

void print_snake_and_ladder_board(){
    
}

void snake_and_ladder_how_to_play(){

}

void snake_and_ladder_developer_section(){

}