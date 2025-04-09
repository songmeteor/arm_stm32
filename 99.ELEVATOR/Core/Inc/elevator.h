#include "main.h"

enum CURRENT_FLOOR{  // 현재 엘베 층수
	first = 1,
	second,
	third,
	fourth
};

enum CURRENT_STATE{  // 현재 엘베 상태
	up = 1,
	down,
	stop
};

enum CURRENT_DOOR_STATE{  // 현재 엘베 문 상태
	open = 1,
	close
};
