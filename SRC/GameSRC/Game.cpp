#include "Game.h"
#include <iostream>
//-------------------------------------------------------------
#define swap_map(map1, map2)  std::swap((map1),(map2))
//-------------------------------------------------------------

void NewMap(G_MAP *map, int width, int height)
{
  (*map) = new bool*[width];

  for (int i = 0; i < width; i++)
  {
    (*map)[i] = new bool[height];
    memset((*map)[i], 0, height);
  }
}

//-------------------------------------------------------------

void DeleteMap(G_MAP *map, int width)
{
  for (int i = 0; i < width; i++)
  {
    delete[] (*map)[i];
  }
  delete[] (*map);
  (*map) = 0;
}

//-------------------------------------------------------------

bool CompareMap(G_MAP map1, G_MAP map2, int width, int height)
{
  for (int i = 0; i < width; i++)
  {
    if (memcmp(map1[i], map2[i], height)) return false;
  }
  return true;
}

//-------------------------------------------------------------

bool isEmptyMap(G_MAP map, int width, int height)
{
  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < height; j++)
    {
      if (map[i][j]) return false;
    }
  }
  return true;
}

//-------------------------------------------------------------

Game::Game(void)
{
  width     = 0;
  height    = 0;
  map       = 0;
  next_map  = 0;
}

//-------------------------------------------------------------

Game::~Game(void)
{
  Destroy();
}

//-------------------------------------------------------------

bool Game::GetCellState(int i, int j) const
{
  i = (i >= width)  ? 0 : (i < 0) ? width - 1  : i;
  i = (i >= height) ? 0 : (i < 0) ? height - 1 : i;
  
  return map[i][j];
}

//-------------------------------------------------------------

int Game::GetLivingAround(int i, int j) const
{
  return  BOOL_TO_INT(GetCellState(i + 1, j - 1)) +
          BOOL_TO_INT(GetCellState(i + 1, j + 0)) +
          BOOL_TO_INT(GetCellState(i + 1, j + 1)) +

          BOOL_TO_INT(GetCellState(i + 0, j - 1)) +
          BOOL_TO_INT(GetCellState(i + 0, j + 1)) +

          BOOL_TO_INT(GetCellState(i - 1, j - 1)) +
          BOOL_TO_INT(GetCellState(i - 1, j + 0)) +
          BOOL_TO_INT(GetCellState(i - 1, j + 1)) ;
}

//-------------------------------------------------------------

GAME_STATE Game::CheckGameState()
{
  if (!map && !next_map)  return STATE_READY;
  if (map || next_map)    return STATE_RUNNING;
  
  return STATE_ERROR;
}

//-------------------------------------------------------------

void Game::Init(int width, int height)
{
  if (CheckGameState() != STATE_READY) Destroy();
  if (!width || !height) return;

  this->width   = width;
  this->height  = height;

  NewMap(&map, width, height);
  NewMap(&next_map, width, height);
}

//-------------------------------------------------------------

void Game::Step()
{
  if (CheckGameState() != STATE_RUNNING) return;

  for (int i = 0; i < width; i++)
  {
    for (int j = 0; j < height; j++)
    {
      // Game Rules

      if (map[i][j] == death && GetLivingAround(i, j) == 3)
        next_map[i][j] = life;

      else if (map[i][j] == life && (GetLivingAround(i, j) == 2 || GetLivingAround(i, j) == 3))
        next_map[i][j] = life;

      else
        next_map[i][j] = death;
    }
  }
  swap_map(map, next_map);
}

//-------------------------------------------------------------

void Game::Destroy()
{
  if (CheckGameState() != STATE_RUNNING) return;
  DeleteMap(&map, width);
  DeleteMap(&next_map, width);
}

//-------------------------------------------------------------

void Game::SetTestValues()
{
  if (height < 10 || width < 10 || CheckGameState() != STATE_RUNNING) return;
  SetValue(0, 0, 1);
  SetValue(1, 1, 1);  
  SetValue(2, 1, 1); 
  SetValue(2, 0, 1); 
  SetValue(1, 2, 1);  
  SetValue(9, 9, 1);
}

//-------------------------------------------------------------

bool Game::isOver() const
{  
  // compare current and next state
  if (CompareMap(map, next_map, width, height))
    return true;

  // if all cells are empty
  if (isEmptyMap(map, width, height))
    return true;

  return false;
}
