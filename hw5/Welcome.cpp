#include "Welcome.h"

using namespace std;

Welcome::Welcome(QApplication *app, Board* scrabble_board, Bag* scrabble_bag, Dictionary* scrabble_dict, unsigned int scrabble_tiles) {
	this->app = app; // copying app address
	board_pointer = scrabble_board; // pointer to Board
	bag_pointer = scrabble_bag; // pointer to Bag
	dict_pointer = scrabble_dict; // pointer to Dict
	num_tiles = scrabble_tiles;

	// Overall layout
	overallLayout = new QVBoxLayout();

	// Banner label
	banner = new QLabel("Welcome to my Scrabble game, see README file for full directions");
	overallLayout->addWidget(banner);

	// Making banner's font bold and larger
	QFont bannerFont;
    bannerFont.setBold(true);
    bannerFont.setPointSize(14);
    banner->setFont(bannerFont);

    // Player name's Label
	names = new QLabel("Please enter the name of a player and press 'Add Player' to add him/her to the game");
	overallLayout->addWidget(names);

	// Player name input
	nameInput = new QLineEdit();
	overallLayout->addWidget(nameInput);

	// Add Button
	addButton = new QPushButton("Add Player");
	connect(addButton, SIGNAL(clicked()), this, SLOT(addPlayer()));
	overallLayout->addWidget(addButton);

	// Start Button
	startButton = new QPushButton("Start Game");
	connect(startButton, SIGNAL(clicked()), this, SLOT(openNewWindow()));
	overallLayout->addWidget(startButton);

	// Set overall layout
	setLayout(overallLayout);

}

Welcome::~Welcome() {

}

void Welcome::addPlayer() {
	// Do nothing if user left iput blank
	if(nameInput->text().isEmpty())
	{
		return;
	}

	// Push name to peole vector
	player_names.push_back(nameInput->text().toStdString());

	// Clear input line
	nameInput->setText("");
}

void Welcome::openNewWindow() { // opening new window
	// Do nothing if user entered no players
	if(player_names.size() < 1)
	{
		return;
	}

	mainWindow = new Scrabble(app, board_pointer, bag_pointer, dict_pointer, num_tiles);
	mainWindow->setPlayers(player_names); // passing on player's names to new window
	mainWindow->setHandLabel(0); // setting intial hand to display first player's hand
	mainWindow->setPlayerLabel(0); //setting intial player label display to first player's name
	mainWindow->setScoreLabel(); //setting up player score's label
	mainWindow->show();
	this->close(); // closing old window
}