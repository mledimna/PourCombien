#include "PourCombien.hpp"
#include "Animation.hpp"

/**
 * @brief Constructs PourCombien object
 * @param _plus_btn Button to increment counters
 * @param _minus_btn Button to decrement counters
 * @param _validate_btn Button to validate counters
 * @param _animations Vector containing Animation pointers
*/
PourCombien::PourCombien(PushButton &_plus_btn, PushButton &_minus_btn, PushButton &_validate_btn,
		std::vector<Animation*> &_animations) :
		plus_btn(_plus_btn), minus_btn(_minus_btn), validate_btn(_validate_btn),
		animations(_animations){}


/**
 * @brief Runs finite state machine
*/
void PourCombien::run(void) {
	switch (this->state) {
	case START:
		this->state = this->start_state();
		break;

	case GET_MAX_BET:
		this->state = this->get_max_bet_state();
		break;

	case GET_GM_BET:
		this->state = this->get_gm_bet_state();
		break;

	case GET_PLAYER_BET:
		this->state = this->get_player_bet_state();
		break;

	case GUESSING:
		this->state = this->guessing_state();
		break;

	case DISPLAY_WINNER:
		this->state = this->display_winner_state();
	}
}

/**
 * @brief Returns max_bet member value
 * @return uint32_t
 * @retval max_bet member
*/
uint32_t PourCombien::get_max_bet(void){
	return this->max_bet;
}

/**
 * @brief Returns game_master_bet member value
 * @return uint32_t
 * @retval game_master_bet member
*/
uint32_t PourCombien::get_gm_bet(void){
	return this->game_master_bet;
}

/**
 * @brief Returns player_bet member value
 * @return uint32_t
 * @retval player_bet member
*/
uint32_t PourCombien::get_player_bet(void){
	return this->player_bet;
}

/**
 * @brief Returns winner member value
 * @return Player
 * @retval winner member
*/
Player PourCombien::get_winner(void){
	return this->winner;
}

/**
 * @brief Runs animation, handles empty vector & NULL pointers
 * @param _fsm_state Finite state machine state from FSM_State enumeration
 * @return Animation_State
 * @retval Animation state
*/
Animation_State PourCombien::run_animation(FSM_State _fsm_state){

	// Check whether user filled animations vector enough
	if ((std::vector<Animation*>::size_type)(_fsm_state + 1) > this->animations.size()) return ANIMATION_ENDED;

	// Get animation pointer
	Animation * animation = this->animations.at(_fsm_state);

	// Check NULL exception
	if (animation == NULL) return ANIMATION_ENDED;

	// Start animation
	Animation_State animation_state = animation->get_state();
	if (animation_state == ANIMATION_STOPPED) animation->start();

	//Run animation
	animation->worker();

	// Return actual state
	return animation->get_state();
}

/**
 * @brief Returns animation state, handles empty vector & NULL pointers
 * @param _fsm_state Finite state machine state from FSM_State enumeration
 * @return Animation_State
 * @retval Animation state
*/
Animation_State PourCombien::get_animation_state(FSM_State _fsm_state){
	// Check whether user filled animations vector enough
	if ((std::vector<Animation*>::size_type)(_fsm_state + 1) > this->animations.size()) return ANIMATION_ENDED;

	// Get animation pointer
	Animation * animation = this->animations.at(_fsm_state);

	// Check NULL exception
	if (animation == NULL) return ANIMATION_ENDED;

	// Start animation
	return animation->get_state();
}

/**
 * @brief Stops animation, handles empty vector & NULL pointers
 * @param _fsm_state Finite state machine state from FSM_State enumeration
*/
void PourCombien::stop_animation(FSM_State _fsm_state){
	// Check whether user filled animations vector enough
	if ((std::vector<Animation*>::size_type)(_fsm_state + 1) > this->animations.size()) return;

	// Get animation pointer
	Animation * animation = this->animations.at(_fsm_state);

	// Check NULL exception
	if (animation == NULL) return;

	// Stop animation
	animation->stop();

	// Run worker to apply changes
	animation->worker();
}

/**
 * @brief START state callback
 * @return FSM_State
 * @retval Next finite state machine state
*/
FSM_State PourCombien::start_state(void){
	// Run START animation
	this->run_animation(START);

	// Check animation END
	if (this->get_animation_state(START) == ANIMATION_ENDED){
		// Stop animation for future use
		this->stop_animation(START);

		// Next state
		return GET_MAX_BET;
	}

	// Stay in START state
	return START;
}

/**
 * @brief GET_MAX_BET state callback
 * @return FSM_State
 * @retval Next finite state machine state
*/
FSM_State PourCombien::get_max_bet_state(void) {
	// Run START animation
	this->run_animation(GET_MAX_BET);

	uint32_t plus_cnt = this->plus_btn.get_press();
	uint32_t minus_cnt = this->minus_btn.get_press();

	if (minus_cnt > plus_cnt){
		// Reset press count
		this->plus_btn.reset();
		this->minus_btn.reset();
	}
	else{
		// Set max_bet variable
		this->max_bet = plus_cnt - minus_cnt;
	}

	// Players choose a max bet
	if (this->validate_btn.get_press()) {
		// Reset validate button
		this->validate_btn.reset();
		// Reset press count
		this->plus_btn.reset();
		this->minus_btn.reset();

		// Check that bet is not below 2
		if (this->max_bet >= 2){
			// Stop animation for future use
			this->stop_animation(GET_MAX_BET);

			// Next state
			return GET_GM_BET;
		}
	}

	// No change
	return GET_MAX_BET;
}

/**
 * @brief GET_GM_BET state callback
 * @return FSM_State
 * @retval Next finite state machine state
*/
FSM_State PourCombien::get_gm_bet_state(void) {
	// Run START animation
	this->run_animation(GET_GM_BET);

	uint32_t plus_cnt = this->plus_btn.get_press();
	uint32_t minus_cnt = this->minus_btn.get_press();

	if (minus_cnt > plus_cnt){
		// Reset press count
		this->plus_btn.reset();
		this->minus_btn.reset();
	}
	else{
		// Set game_master_bet variable
		this->game_master_bet = plus_cnt - minus_cnt;
	}

	// Player choose a bet
	if (this->validate_btn.get_press()) {
		// Reset validate button
		this->validate_btn.reset();
		// Reset press count
		this->plus_btn.reset();
		this->minus_btn.reset();

		// Go to next state if player bet is not over maximum bet
		if ((this->game_master_bet <= this->max_bet) && (this->game_master_bet > 0)){
			// Stop animation for future use
			this->stop_animation(GET_GM_BET);

			// Next state
			return GET_PLAYER_BET;
		}
	}

	// Stay in state
	return GET_GM_BET;
}

/**
 * @brief GET_PLAYER_BET state callback
 * @return FSM_State
 * @retval Next finite state machine state
*/
FSM_State PourCombien::get_player_bet_state(void) {
	// Run animation
	this->run_animation(GET_PLAYER_BET);

	uint32_t plus_cnt = this->plus_btn.get_press();
	uint32_t minus_cnt = this->minus_btn.get_press();

	if (minus_cnt > plus_cnt){
		// Reset press count
		this->plus_btn.reset();
		this->minus_btn.reset();
	}
	else{
		// Set player_bet variable
		this->player_bet = plus_cnt - minus_cnt;
	}

	// Player choose a bet
	if (this->validate_btn.get_press()) {
		// Reset validate button
		this->validate_btn.reset();
		// Reset press count
		this->plus_btn.reset();
		this->minus_btn.reset();

		// Go to next state if player bet is not over maximum bet
		if ((this->player_bet <= this->max_bet) && (this->player_bet > 0)) {
			// Stop animation for future use
			this->stop_animation(GET_PLAYER_BET);

			// Next state
			return GUESSING;
		}
	}

	// Stay in state
	return GET_PLAYER_BET;
}

/**
 * @brief GUESSING state callback
 * @return FSM_State
 * @retval Next finite state machine state
*/
FSM_State PourCombien::guessing_state(void) {
	// Run animation
	this->run_animation(GUESSING);

	// Guessing animation ended
	if (this->get_animation_state(GUESSING) == ANIMATION_ENDED) {
		// Special case
		if (max_bet == 2) {
			// If P1 matches P2, P1 wins. Else, P2 wins.
			if (this->game_master_bet == this->player_bet)
				this->winner = GAME_MASTER;
			else
				this->winner = PLAYER;
		} else if (this->game_master_bet == this->player_bet)
			this->winner = GAME_MASTER;

		// Stop animation for future use
		this->stop_animation(GUESSING);

		// Next state
		return DISPLAY_WINNER;
	}

	// Stay in state
	return GUESSING;
}

/**
 * @brief DISPLAY_WINNER state callback
 * @return FSM_State
 * @retval Next finite state machine state
*/
FSM_State PourCombien::display_winner_state(void) {
	this->run_animation(DISPLAY_WINNER);

	if (this->get_animation_state(DISPLAY_WINNER) == ANIMATION_ENDED){
		// Stop animation for future use
		this->stop_animation(DISPLAY_WINNER);

		// Reset members for next round
		this->winner = NONE;
		this->max_bet = 0;
		this->game_master_bet = 0;
		this->player_bet = 0;

		// Next state
		return GET_MAX_BET;
	}

	// Stay in state
	return DISPLAY_WINNER;
}
