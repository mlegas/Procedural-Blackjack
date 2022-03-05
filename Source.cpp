#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include "Windows.h"

struct Card
{
	char m_cardSuit[9];
	char m_cardName[6];
	int m_cardValue;
};

enum Result
{
	m_null, m_win, m_push, m_lose, m_blackjack
};

void CalculateOutcome(long long *_balanceptr, long long _bet, Result _outcome);
void CreateDeck(Card _deck[]);
void DrawCard(Card _deck[], Card _hand[], short *_deckCardCount, short *_handCardCount, short *_handStrength, short *_firstAceLocation, bool *_isFirstAceSet);
void GameLoop(long long *_balanceptr, Card _deck[]);
long long InputBet(long long *_balanceptr);
void PlayAgain(long long *_balanceptr, bool *_quitGame);
void PrintOutHUD(Card _dealersHand[], Card _playersHand[], short *_dealerHandStrength, short *_playerHandStrength, long long _bet, long long *_balanceptr, bool _standCheck);
void ShuffleCards(Card _deck[]);

int main()
{
	std::cout << "Welcome to... Blackjack!\n\n\n";
	std::cout << "== Basic rules of the casino ==\n\n";
	std::cout << "There is one deck of cards used.\n";
	std::cout << "The payout rate is 2:1.\n";
	std::cout << "A blackjack pays 3:2.\n";
	std::cout << "The dealer hits until his hand strength is above 16.\n";
	std::cout << "The dealer hits on a soft 17. (e.g. when the dealer has an Ace and a Six)\n";
	std::cout << "The value of aces changes automatically to 1 from 11 in case of a bust scenario.\n\n\n";
	system("PAUSE");

	long long balance = 10000;
	long long *balanceptr = &balance;

	Card deck[52];

	CreateDeck(deck);
	ShuffleCards(deck);

	GameLoop(balanceptr, deck);

	system("PAUSE");
	return 0;
}

void CalculateOutcome(long long *_balanceptr, long long _bet, Result _outcome)
{
	switch (_outcome)
	{
		case m_win:
		{
			*_balanceptr = *_balanceptr + (_bet * 2);
			std::cout << "Your winnings: $" << (_bet * 2) << "\n\n";
			std::cout << "Your balance: $" << *_balanceptr << "\n\n";
			break;
		}

		case m_push:
		{
			*_balanceptr += _bet;
			std::cout << "Your bet of $" << _bet << " has been returned.\n\n";
			std::cout << "Your balance: $" << *_balanceptr << "\n\n";
			break;
		}

		case m_lose:
		{
			std::cout << "You lost your bet of $" << _bet << ".\n\n";
			std::cout << "Your balance: $" << *_balanceptr << "\n\n";
			break;
		}

		case m_blackjack:
		{
			*_balanceptr = *_balanceptr + ((double)_bet * 1.5);
			std::cout << "Your winnings: $" << ((double)_bet * 1.5) << "\n\n";
			std::cout << "Your balance: $" << *_balanceptr << "\n\n";
			break;
		}

		default:
		{
			std::cout << "Exception found!\n";
			break;
		}
	}
}

void CreateDeck(Card _deck[])
{
	for (int suit = 0; suit < 52; suit += 13)
	{
		for (int card = 0; card < 13; card++)
		{
			if (card == 0)
			{
				_deck[suit + card].m_cardValue = 11;
			}

			if (card > 9)
			{
				_deck[suit + card].m_cardValue = 10;
			}

			if (card > 0 && card < 10)
			{
				_deck[suit + card].m_cardValue = card + 1;
			}

			switch (suit)
			{
				case 0:
				{
					strcpy_s(_deck[suit + card].m_cardSuit, "Clubs");
					break;
				}

				case 13:
				{
					strcpy_s(_deck[suit + card].m_cardSuit, "Hearts");
					break;
				}

				case 26:
				{
					strcpy_s(_deck[suit + card].m_cardSuit, "Diamonds");
					break;
				}

				case 39:
				{
					strcpy_s(_deck[suit + card].m_cardSuit, "Spades");
					break;
				}

				default:
				{
					std::cout << "Exception found!\n";
					break;
				}
			}

			switch (card)
			{
				case 0:
				{
					strcpy_s(_deck[suit + card].m_cardName, "Ace");
					break;
				}

				case 1:
				{
					strcpy_s(_deck[suit + card].m_cardName, "Two");
					break;
				}

				case 2:
				{
					strcpy_s(_deck[suit + card].m_cardName, "Three");
					break;
				}

				case 3:
				{
					strcpy_s(_deck[suit + card].m_cardName, "Four");
					break;
				}

				case 4:
				{
					strcpy_s(_deck[suit + card].m_cardName, "Five");
					break;
				}

				case 5:
				{
					strcpy_s(_deck[suit + card].m_cardName, "Six");
					break;
				}

				case 6:
				{
					strcpy_s(_deck[suit + card].m_cardName, "Seven");
					break;
				}

				case 7:
				{
					strcpy_s(_deck[suit + card].m_cardName, "Eight");
					break;
				}

				case 8:
				{
					strcpy_s(_deck[suit + card].m_cardName, "Nine");
					break;
				}

				case 9:
				{
					strcpy_s(_deck[suit + card].m_cardName, "Ten");
					break;
				}

				case 10:
				{
					strcpy_s(_deck[suit + card].m_cardName, "Jack");
					break;
				}

				case 11:
				{
					strcpy_s(_deck[suit + card].m_cardName, "Queen");
					break;
				}

				case 12:
				{
					strcpy_s(_deck[suit + card].m_cardName, "King");
					break;
				}

				default:
				{
					std::cout << "Exception found!\n";
					break;
				}
			}
		}
	}
}

void DrawCard(Card _deck[], Card _hand[], short *_deckCardCount, short *_handCardCount, short *_handStrength, short *_firstAceLocation, bool *_isFirstAceSet)
{
	if (*_deckCardCount >= 51)
	{
		std::cout << "All cards have been used.\n";
		ShuffleCards(_deck);
		*_deckCardCount = 0;
	}

	_hand[*_handCardCount].m_cardValue = _deck[*_deckCardCount].m_cardValue;
	strcpy_s(_hand[*_handCardCount].m_cardName, _deck[*_deckCardCount].m_cardName);
	strcpy_s(_hand[*_handCardCount].m_cardSuit, _deck[*_deckCardCount].m_cardSuit);

	if (_hand[*_handCardCount].m_cardValue == 11)
	{
		if (*_isFirstAceSet == false && *_handStrength < 10)
		{
			*_firstAceLocation = *_handCardCount;
			*_isFirstAceSet = true;
		}

		if (*_handStrength > 10)
		{
			_hand[*_handCardCount].m_cardValue = 1;
		}
	}

	if ((*_handStrength + _hand[*_handCardCount].m_cardValue) > 21 && *_isFirstAceSet == true && _hand[*_firstAceLocation].m_cardValue != 1)
	{
		_hand[*_firstAceLocation].m_cardValue = 1;
		*_handStrength -= 10;
	}

	*_handStrength += _hand[*_handCardCount].m_cardValue;
	*_deckCardCount = *_deckCardCount + 1;
	*_handCardCount = *_handCardCount + 1;
}

void GameLoop(long long *_balanceptr, Card _deck[])
{
	char choice;
	bool *quitGame = new bool;
	*quitGame = false;
	short *playerHandStrength = new short;
	short *dealerHandStrength = new short;
	short *deckCardCount = new short;
	*deckCardCount = 0;
	short *playerHandCount = new short;
	short *dealerHandCount = new short;
	short *playerFirstAceLocation = new short;
	short *dealerFirstAceLocation = new short;
	bool *playerIsFirstAceSet = new bool;
	bool *dealerIsFirstAceSet = new bool;

	while (!*quitGame)
	{
		Card playersHand[11] = { 0 };
		Card dealersHand[10] = { 0 };
		Result outcome = m_null;

		long long bet = InputBet(_balanceptr);
		bool playerIsDeciding = true;
		bool standCheck = false;
		bool roundOver = false;

		*playerHandStrength = 0;
		*dealerHandStrength = 0;
		*playerHandCount = 0;
		*dealerHandCount = 0;
		*playerFirstAceLocation = 0;
		*dealerFirstAceLocation = 0;
		*playerIsFirstAceSet = false;
		*dealerIsFirstAceSet = false;

		DrawCard(_deck, dealersHand, deckCardCount, dealerHandCount, dealerHandStrength, dealerFirstAceLocation, dealerIsFirstAceSet);
		DrawCard(_deck, dealersHand, deckCardCount, dealerHandCount, dealerHandStrength, dealerFirstAceLocation, dealerIsFirstAceSet);
		DrawCard(_deck, playersHand, deckCardCount, playerHandCount, playerHandStrength, playerFirstAceLocation, playerIsFirstAceSet);
		DrawCard(_deck, playersHand, deckCardCount, playerHandCount, playerHandStrength, playerFirstAceLocation, playerIsFirstAceSet);

		system("CLS");
		std::cout << "Drawing initial cards for the dealer and the player...\n\n";

		PrintOutHUD(dealersHand, playersHand, dealerHandStrength, playerHandStrength, bet, _balanceptr, standCheck);

		if (*playerHandStrength == 21)
		{
			system("CLS");
			std::cout << "Natural blackjack!\n\n\n";

			standCheck = true;
			PrintOutHUD(dealersHand, playersHand, dealerHandStrength, playerHandStrength, bet, _balanceptr, standCheck);

			if (*dealerHandStrength == *playerHandStrength)
			{
				std::cout << "And the dealer has a blackjack as well! Push!\n\n";
				outcome = m_push;
			}

			else
			{
				std::cout << "The dealer did not have a blackjack! You win!\n\n";
				outcome = m_blackjack;
			}
			roundOver = true;
		}

		while (*playerHandStrength != 21 && *playerHandStrength < 22 && playerIsDeciding)
		{
			std::cout << "Would you like to [H]it or [S]tand?\n";
			std::cin >> choice;

			switch (choice)
			{
				case 'H':
				case 'h':
				{
					DrawCard(_deck, playersHand, deckCardCount, playerHandCount, playerHandStrength, playerFirstAceLocation, playerIsFirstAceSet);
					system("CLS");

					std::cout << "You draw a card.\n\n";
					std::cout << "You drew a(n) " << playersHand[*playerHandCount - 1].m_cardName << " of " << playersHand[*playerHandCount - 1].m_cardSuit << ".\n\n";

					standCheck = false;
					PrintOutHUD(dealersHand, playersHand, dealerHandStrength, playerHandStrength, bet, _balanceptr, standCheck);
					break;
				}

				case 'S':
				case 's':
				{
					playerIsDeciding = false;
					system("CLS");

					std::cout << "You stand.\n\n";
					std::cout << "The dealer reveals his first card.\n\n\n";

					standCheck = true;
					PrintOutHUD(dealersHand, playersHand, dealerHandStrength, playerHandStrength, bet, _balanceptr, standCheck);
					Sleep(2500);
					break;
				}

				default:
				{
					std::cout << "Sorry, I did not understand. Try again.\n\n";
					break;
				}
			}
		}

		if (*playerHandStrength == 21 && !roundOver)
		{
			system("CLS");
			std::cout << "21 hand strength!\n\n";
			std::cout << "You automatically stand.\n\n";
			std::cout << "The dealer reveals his first card.\n\n\n";

			standCheck = true;
			PrintOutHUD(dealersHand, playersHand, dealerHandStrength, playerHandStrength, bet, _balanceptr, standCheck);
			Sleep(2500);
		}

		if (*playerHandStrength > 21 && !roundOver)
		{
			std::cout << "You bust!\n\n";
			outcome = m_lose;
			roundOver = true;
		}

		while ((*dealerHandStrength < 17 || *dealerIsFirstAceSet && (*dealerHandStrength == 17 && dealersHand[*dealerFirstAceLocation].m_cardValue == 11)) && !roundOver)
		{
			system("CLS");
			std::cout << "The dealer hits.\n\n";

			DrawCard(_deck, dealersHand, deckCardCount, dealerHandCount, dealerHandStrength, dealerFirstAceLocation, dealerIsFirstAceSet);
			PrintOutHUD(dealersHand, playersHand, dealerHandStrength, playerHandStrength, bet, _balanceptr, standCheck);

			Sleep(2000);
		}

		if (*dealerHandStrength == 21 && !roundOver)
		{
			std::cout << "The dealer has 21!\n\n";
		}

		if (*dealerHandStrength > 21 && !roundOver)
		{
			std::cout << "The dealer busts!\n\n";
			outcome = m_win;
			roundOver = true;
		}

		if (*playerHandStrength == *dealerHandStrength && !roundOver)
		{
			std::cout << "The dealer has the same card strength! Push!\n\n";
			outcome = m_push;
			roundOver = true;
		}

		if (*playerHandStrength > *dealerHandStrength && !roundOver)
		{
			std::cout << "Your hand is stronger than the dealer's!\n\n";
			outcome = m_win;
			roundOver = true;
		}

		if (*playerHandStrength < *dealerHandStrength && !roundOver)
		{
			std::cout << "Your hand is weaker than the dealer's.\n\n";
			outcome = m_lose;
			roundOver = true;
		}
		CalculateOutcome(_balanceptr, bet, outcome);
		PlayAgain(_balanceptr, quitGame);
	}
	delete quitGame, playerHandStrength, dealerHandStrength, deckCardCount, playerHandCount, dealerHandCount, playerFirstAceLocation, dealerFirstAceLocation, playerIsFirstAceSet, dealerIsFirstAceSet;
}

long long InputBet(long long *_balanceptr)
{
	system("CLS");

	long long bet = 0;
	bool correctBetGiven = false;

	while (correctBetGiven == false)
	{
		std::cout << "Your balance: $" << *_balanceptr << "\n";
		std::cout << "How much would you like to bet?\n";
		std::cin >> bet;

		if (bet > *_balanceptr)
		{
			system("CLS");
			std::cout << "Your bet is higher than your balance! Try again.\n\n";
		}

		if (bet <= 0)
		{
			system("CLS");
			std::cout << "Your bet is an invalid value! Try again.\n\n";
		}

		if (bet > 0 && bet <= *_balanceptr)
		{
			system("CLS");
			std::cout << "Your bet is: $" << bet << "\n";

			*_balanceptr -= bet;

			std::cout << "The amount of $" << bet << " has been taken from your balance.\n\n";
			correctBetGiven = true;
		}
		Sleep(1000);
	}
	return bet;
}

void PlayAgain(long long *_balanceptr, bool *_quitGame)
{
	char choice;
	bool correctOption = false;

	if (*_balanceptr <= 0)
	{
		std::cout << "You have lost all of your money!\n\n";
		std::cout << "G A M E  O V E R\n\n";
		*_quitGame = true;
	}

	else
	{
		while (correctOption == false)
		{
			std::cout << "Would you like to play again? [Y/N] (Inputting N will quit the game)\n";
			std::cin >> choice;

			switch (choice)
			{
				case 'Y':
				case 'y':
				{
					correctOption = true;
					break;
				}

				case 'N':
				case 'n':
				{
					std::cout << "\nThank you for playing! Goodbye!\n\n";
					correctOption = true;
					*_quitGame = true;
					break;
				}

				default:
				{
					std::cout << "\nSorry, I did not understand.\n\n";
					break;
				}
			}
		}
	}
}

void PrintOutHUD(Card _dealersHand[], Card _playersHand[], short *_dealerHandStrength, short *_playerHandStrength, long long _bet, long long *_balanceptr, bool _standCheck)
{
	if (_standCheck == false)
	{
		std::cout << "Dealer's Hand:\n\n";
		std::cout << "??? of ??? || " << _dealersHand[1].m_cardName << " of " << _dealersHand[1].m_cardSuit << "\n\n";
		std::cout << "Strength: ??? + " << _dealersHand[1].m_cardValue << "\n\n\n";

		std::cout << "Your Hand:\n\n";
		std::cout << _playersHand[0].m_cardName << " of " << _playersHand[0].m_cardSuit;

		for (short i = 1; _playersHand[i].m_cardValue != 0; i++)
		{
			std::cout << " || " << _playersHand[i].m_cardName << " of " << _playersHand[i].m_cardSuit;
		}

		std::cout << "\n\nStrength: " << _playersHand[0].m_cardValue;

		for (short j = 1; _playersHand[j].m_cardValue != 0; j++)
		{
			std::cout << " + " << _playersHand[j].m_cardValue;
		}

		std::cout << " = " << *_playerHandStrength;

		std::cout << "\n\n\nYour bet: $" << _bet << "\t Your balance: $" << *_balanceptr << "\n\n\n";
	}

	if (_standCheck == true)
	{
		std::cout << "Dealer's Hand:\n\n";
		std::cout << _dealersHand[0].m_cardName << " of " << _dealersHand[0].m_cardSuit;

		for (short k = 1; _dealersHand[k].m_cardValue != 0; k++)
		{
			std::cout << " || " << _dealersHand[k].m_cardName << " of " << _dealersHand[k].m_cardSuit;
		}

		std::cout << "\n\nStrength: " << _dealersHand[0].m_cardValue;

		for (short l = 1; _dealersHand[l].m_cardValue != 0; l++)
		{
			std::cout << " + " << _dealersHand[l].m_cardValue;
		}

		std::cout << " = " << *_dealerHandStrength;

		std::cout << "\n\n\nYour Hand:\n\n";
		std::cout << _playersHand[0].m_cardName << " of " << _playersHand[0].m_cardSuit;

		for (short m = 1; _playersHand[m].m_cardValue != 0; m++)
		{
			std::cout << " || " << _playersHand[m].m_cardName << " of " << _playersHand[m].m_cardSuit;
		}

		std::cout << "\n\nStrength: " << _playersHand[0].m_cardValue;

		for (short n = 1; _playersHand[n].m_cardValue != 0; n++)
		{
			std::cout << " + " << _playersHand[n].m_cardValue;
		}

		std::cout << " = " << *_playerHandStrength;

		std::cout << "\n\n\nYour bet: $" << _bet << "\t Your balance: $" << *_balanceptr << "\n\n\n";
	}
}

void ShuffleCards(Card _deck[])
{
	srand(time(NULL));
	Card temp = { 0 };

	for (short i = 0; i < 10; i++)
	{
		for (short j = 0; j < 52; j++)
		{
			temp = _deck[j];
			short random = rand() % 52;
			_deck[j] = _deck[random];
			_deck[random] = temp;
		}
	}
	std::cout << "Cards have been shuffled.\n";
}