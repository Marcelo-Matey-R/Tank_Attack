#include "StructPosition.h"

const Position up      = {-1,  0};  // decrementa fila
const Position down    = { 1,  0};  // incrementa fila
const Position right   = { 0,  1};  // incrementa columna
const Position left    = { 0, -1};  // decrementa columna

const Position diagonal1 = up   + right;  // {-1,  1}  arriba-derecha
const Position diagonal2 = up   + left;   // {-1, -1}  arriba-izquierda
const Position diagonal3 = down + right;  // { 1,  1}  abajo-derecha
const Position diagonal4 = down + left;   // { 1, -1}  abajo-izquierda


inline Position directions[4] = {up, down, right, left};

inline Position allDirections[8] = {up, down, right, left, diagonal1, diagonal2, diagonal3, diagonal4};