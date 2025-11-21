# UI Constructor
### Add this:
```cpp
UI(int cell_display_width) : cell_width(cell_display_width) {}
```
### instead of:
```cpp
UI(string message, int cell_display_width) : cell_width(cell_display_width) {
  cout << message << endl;
}
```
### Why?
To prevent printing every game message once the game starts

---------------------------------
# Make UI::setup_players() a pure virtual
### Add this:
```cpp 
virtual Player<T> **setup_players() = 0;
```
### Instead of:
```cpp
template <typename T>

Player<T>** UI<T>::setup_players() {

    Player<T>** players = new Player<T>*[2];

    vector<string> type_options = { "Human", "Computer" };

    string nameX = get_player_name("Player X");

    PlayerType typeX = get_player_type_choice("Player X", type_options);

    players[0] = create_player(nameX, static_cast<T>('X'), typeX);

    string nameO = get_player_name("Player O");

    PlayerType typeO = get_player_type_choice("Player O", type_options);

    players[1] = create_player(nameO, static_cast<T>('O'), typeO);

    return players;
}
```
### Why?
The default one written in Board game library is only used for XO game and in some games We will need to customize the way we setup the players, 
I know that I can override the function but I think for more clean, maintainable code we should make it pure virtual. 

i.e: we can extend it to be for more than two players.
