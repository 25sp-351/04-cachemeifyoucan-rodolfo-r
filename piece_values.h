#ifndef PIECE_VALUES_H
#define PIECE_VALUES_H

#include "vec.h"

Vec read_piece_values_from_file(const char *filename);

void print_piece_values(Vec value_list);

#endif 