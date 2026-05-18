#include "StructPosition.h"

const Position up = {0, -1};

const Position down = {0, 1};

const Position right = {1, 0};

const Position left = {-1, 0};

const Position diagonal1 = up + Position{1, 0};

const Position diagonal2 = up + Position{-1, 0};

const Position diagonal3 = down + Position{1, 0};

const Position diagonal4 = down + Position{-1, 0};


Position directions[4] = {up, down, right, left};

Position allDirections[8] = {up, down, right, left, diagonal1, diagonal2, diagonal3, diagonal4};