/* 
 * File:   main.cpp
 * Author: Mariam Mikhail
 *Purpose: Poker Card Game 
 * Created on February 2, 2024, 9:53 AM
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

// Define card structure
struct Card {
    string suit;
    string rank;
    int value;
};

// Define hand structure
struct Hand {
    vector<Card> cards;
    int totalValue;
    bool isBusted;
};

// Function to initialize a standard deck of 52 cards
vector<Card> initializeDeck() {
    vector<Card> deck;
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King", "Ace"};
    int values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11}; // Ace value initially set to 11

    for (const string &suit : suits) {
        for (int i = 0; i < 13; ++i) {
            deck.push_back({suit, ranks[i], values[i]});
        }
    }

    return deck;
}

// Function to shuffle the deck
void shuffleDeck(vector<Card> &deck) {
    srand(time(nullptr));
    for (int i = 0; i < deck.size(); ++i) {
        int j = rand() % deck.size();
        swap(deck[i], deck[j]);
    }
}

// Function to deal a card from the deck
Card dealCard(vector<Card> &deck) {
    Card card = deck.back();
    deck.pop_back();
    return card;
}

// Function to calculate the total value of a hand
int calculateHandValue(const Hand &hand) {
    int totalValue = 0;
    for (const Card &card : hand.cards) {
        totalValue += card.value;
    }
    return totalValue;
}

// Function to display a hand
void displayHand(const Hand &hand, bool showAllCards = true) {
    cout << "Hand:\n";
    for (size_t i = 0; i < hand.cards.size(); ++i) {
        if (showAllCards || i == 0) {
            cout << hand.cards[i].rank << " of " << hand.cards[i].suit << endl;
        } else {
            cout << "[Hidden Card]" << endl;
        }
    }
    cout << "Total Value: " << hand.totalValue << endl << endl;
}

// Function to check if a hand is busted (total value exceeds 21)
bool isBusted(const Hand &hand) {
    return hand.totalValue > 21;
}

// Function to determine the winner
string determineWinner(const Hand &playerHand, const Hand &dealerHand) {
    if (isBusted(playerHand)) {
        return "Dealer wins (Player busted).";
    } else if (isBusted(dealerHand)) {
        return "Player wins (Dealer busted).";
    } else if (playerHand.totalValue > dealerHand.totalValue) {
        return "Player wins.";
    } else if (dealerHand.totalValue > playerHand.totalValue) {
        return "Dealer wins.";
    } else {
        return "It's a tie.";
    }
}

// Function to save game progress to file
void saveGame(const Hand &playerHand, const Hand &dealerHand) {
    ofstream file("poker_game_save.txt");
    if (file.is_open()) {
        file << "Player's Hand:\n";
        for (const Card &card : playerHand.cards) {
            file << card.rank << " of " << card.suit << endl;
        }
        file << "\nDealer's Hand:\n";
        for (const Card &card : dealerHand.cards) {
            file << card.rank << " of " << card.suit << endl;
        }
        cout << "Game progress saved successfully.\n";
        file.close();
    } else {
        cout << "Unable to open file for saving game progress.\n";
    }
}

// Function to display menu options
void displayMenu() {
    cout << "1. Hit\n";
    cout << "2. Stand\n";
    cout << "3. Save Game\n";
    cout << "4. Exit\n";
    cout << "Choose an option: ";
}

// Function to play the game
void playGame() {
    // Initialize and shuffle the deck
    vector<Card> deck = initializeDeck();
    shuffleDeck(deck);

    // Initialize player and dealer hands
    Hand playerHand;
    Hand dealerHand;

    // Deal two cards to each player
    playerHand.cards.push_back(dealCard(deck));
    playerHand.cards.push_back(dealCard(deck));
    dealerHand.cards.push_back(dealCard(deck));
    dealerHand.cards.push_back(dealCard(deck));

    // Calculate total value of hands
    playerHand.totalValue = calculateHandValue(playerHand);
    dealerHand.totalValue = calculateHandValue(dealerHand);

    // Display player's initial hand
    cout << "Player's Hand:\n";
    displayHand(playerHand);

    // Game loop
    bool gameOver = false;
    while (!gameOver) {
        displayMenu();

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: // Hit
                playerHand.cards.push_back(dealCard(deck));
                playerHand.totalValue = calculateHandValue(playerHand);
                cout << "Player's Hand:\n";
                displayHand(playerHand);
                if (isBusted(playerHand)) {
                    cout << "Player busted. Dealer wins.\n";
                    gameOver = true;
                }
                break;
            case 2: // Stand
                cout << "Dealer's Hand:\n";
                displayHand(dealerHand, true);
                while (dealerHand.totalValue < 17) {
                    dealerHand.cards.push_back(dealCard(deck));
                    dealerHand.totalValue = calculateHandValue(dealerHand);
                }
                cout << "Dealer's Hand:\n";
                displayHand(dealerHand);
                if (isBusted(dealerHand)) {
                    cout << "Dealer busted. Player wins.\n";
                } else {
                    cout << determineWinner(playerHand, dealerHand) << endl;
                }
                gameOver = true;
                break;
            case 3: // Save game
                saveGame(playerHand, dealerHand);
                break;
            case 4: // Exit
                gameOver = true;
                break;
            default:
                cout << "Invalid option. Please choose again.\n";
        }
    }
}

int main() {
    cout << "Welcome to Poker!\n";
    cout << "Let's play.\n\n";
    playGame();

    return 0;
}



