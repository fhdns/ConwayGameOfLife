#pragma once

//-------------------------------------------------------------
#define DEFAULT_MAP_SIZE_W  10
#define DEFAULT_MAP_SIZE_H  10
//-------------------------------------------------------------
enum    GAME_STATE { STATE_READY, STATE_RUNNING, STATE_ERROR };
//-------------------------------------------------------------
typedef bool**  G_MAP;
#define life    true
#define death   false
//-------------------------------------------------------------
#define BOOL_TO_INT(bool_value) ((bool_value) ? 1 : 0)
//-------------------------------------------------------------

class Game
{
public:
  Game(void);
  ~Game(void);
  
  GAME_STATE CheckState();

  void Init(int width = DEFAULT_MAP_SIZE_W, int height = DEFAULT_MAP_SIZE_H);
  void Step();
  void Destroy();
  bool isOver() const;
  
  void SetValue(int i, int j, bool state) { map[i][j] = state; }
  void SetTestValues();

  G_MAP  GetMap()    const { return map; }
  int    GetWidth()  const { return width; }
  int    GetHeight() const { return height; }

private:
  int     width;
  int     height;
  G_MAP   map;
  G_MAP   next_map;

  inline bool*  cell_state(int i, int j);
  inline int    living_around(int i, int j);
};
