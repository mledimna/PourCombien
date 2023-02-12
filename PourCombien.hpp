#ifndef POURCOMBIEN_HPP_
#define POURCOMBIEN_HPP_


#ifdef __cplusplus

#include "PushButton.hpp"
#include "Animation.hpp"
#include <vector>

typedef enum {
	START,
	GET_MAX_BET,
	GET_GM_BET,
	GET_PLAYER_BET,
	GUESSING,
	DISPLAY_WINNER,
} FSM_State;

typedef enum {
	NONE,
	GAME_MASTER,
	PLAYER,
} Player;

class PourCombien {
public:
	PourCombien(PushButton &_plus_btn, PushButton &_minus_btn, PushButton &_validate_btn,
		std::vector<Animation*> &_animations);
	void run(void);
	uint32_t get_gm_bet(void);
	uint32_t get_max_bet(void);
	uint32_t get_player_bet(void);
	Player get_winner(void);

private:
	FSM_State start_state(void);
	FSM_State get_max_bet_state(void);
	FSM_State get_gm_bet_state(void);
	FSM_State get_player_bet_state(void);
	FSM_State guessing_state(void);
	FSM_State display_winner_state(void);

	Animation_State run_animation(FSM_State _fsm_state);
	Animation_State get_animation_state(FSM_State _fsm_state);
	void stop_animation(FSM_State _fsm_state);

	FSM_State state = START;
	Player winner = NONE;

	uint32_t max_bet = 0;
	uint32_t game_master_bet = 0;
	uint32_t player_bet = 0;

	PushButton &plus_btn;
	PushButton &minus_btn;
	PushButton &validate_btn;

	std::vector<Animation*> & animations;
};
#endif


#endif /* POURCOMBIEN_HPP_ */
