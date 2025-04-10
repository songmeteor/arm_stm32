#include "elevator.h"
#include "button.h"
#include "extern.h"
#include "stepmotor.h"

enum CURRENT_FLOOR current_floor = first;
enum CURRENT_STATE current_state = stop;
enum CURRENT_DOOR_STATE current_door_state = close;

int target_floor[5] = {0};  //현재 가야하는 엘베 층

void elevator_main(void);

void elevator_main(void)
{
	if(current_door_state == close)
	{
		elevator_open_counter = 0;
		switch(current_floor){
		case first:
			if(target_floor[2]||target_floor[3]||target_floor[4]) current_state = up;
			else current_state = stop;
			break;
		case second:
			if((current_state == up) && (target_floor[3]||target_floor[4])) current_state = up;
			else if((current_state == up) && (target_floor[1])) current_state = down;
			else if((current_state == down) && (target_floor[1])) current_state = down;
			else if((current_state == down) && (target_floor[3]||target_floor[4])) current_state = up;
			else if((current_state == stop) && (target_floor[3]||target_floor[4])) current_state = up;
			else if((current_state == stop) && (target_floor[1])) current_state = down;
			else current_state = stop;
			break;
		case third:
			if((current_state == up) && (target_floor[4])) current_state = up;
			else if((current_state == up) && (target_floor[1]|| target_floor[2])) current_state = down;
			else if((current_state == down) && (target_floor[1] || target_floor[2])) current_state = down;
			else if((current_state == down) && (target_floor[4])) current_state = up;
			else if((current_state == stop) && (target_floor[1]||target_floor[2])) current_state = down;
			else if((current_state == stop) && (target_floor[4])) current_state = up;
			else current_state = stop;
			break;
		case fourth:
			if(target_floor[1]||target_floor[2]||target_floor[3]) current_state = down;
			else current_state = stop;
			break;
		}

		if(current_state == up)
		{
			stepmotor_state = FORWARD;
		}
		else if(current_state == down)
		{
			stepmotor_state = BACKWARD;
		}
		else if(current_state == stop)
		{
			stepmotor_state = IDLE;
		}

	}
	else if(current_door_state == open)
	{
		if(elevator_open_counter >= 5000)
		{
			current_door_state = close;
		}
	}
}



