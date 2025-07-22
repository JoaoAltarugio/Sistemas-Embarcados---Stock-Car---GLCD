#include <reg51.h>
#include <intrins.h>

// --- Configurações da tela ---
#define SCREEN_W 128
#define SCREEN_H 64
#define PLAYER_Y 56
#define PLAYER_CAR_WIDTH 8
#define PLAYER_CAR_HEIGHT 8
#define ROAD_LINE_WIDTH 4
#define ROAD_LINE_HEIGHT 8
#define MAX_ENEMIES 2
#define ROAD_SEGMENTS 16
#define nop() _nop_()



// --- Pinos do GLCD ---
#define Data_Port P1
sbit RS  = P2^0;
sbit RW  = P2^1;
sbit E   = P2^2;
sbit CS1 = P2^3;
sbit CS2 = P2^4;
sbit RST = P2^5;

// --- Botões ---
sbit BUTTON_LEFT    = P3^0;
sbit BUTTON_RESTART = P3^1;
sbit BUTTON_RIGHT   = P3^2;


// --- Sprites ---
const unsigned char solid_8x8[8] = { // Sprite do carro do jogador
  0x3C, // 00111100
  0x5A, // 01011010
  0x5A, // 01011010
  0x7E, // 01111110
  0x3C, // 00111100
  0x7E, // 01111110
  0x66, // 01100110
  0x3C  // 00111100
};


code unsigned char asterisk_8x8[8] = {0x3C, 0x24, 0x66, 0x3C, 0x3C, 0x3C, 0x24, 0x3C};// Sprite do carro inimigo
code unsigned char road_line_4x8[4] = {// Sprite da linha da estrada
  0xFF,0xFF,0xFF,0xFF
};
code unsigned char blank[4] = {
  0x00,0x00,0x00,0x00
};

code unsigned char letra_G[8] = {0x00, 0x78, 0x84, 0x04, 0xE4, 0x84, 0x84, 0x78};
code unsigned char letra_A[8] = {0x00, 0x21, 0x21, 0x21, 0x3F, 0x21, 0x12, 0x0C};
code unsigned char letra_M[8] = {0x00, 0x21, 0x21, 0x21, 0x21, 0x2D, 0x33, 0x21};
//code unsigned char letra_E[8] = {0x00, 0x3F, 0x20, 0x20, 0x3E, 0x20, 0x20, 0x3F};
//code unsigned char letra_O[8] = {0x00, 0x1E, 0x21, 0x21, 0x21, 0x21, 0x21, 0x1E};
code unsigned char letra_V[8] = {0x00, 0x0C, 0x12, 0x21, 0x21, 0x21, 0x21, 0x21};
//code unsigned char letra_R[8] = {0x00, 0x78, 0x84, 0x84, 0x78, 0x24, 0x44, 0x84};




code unsigned char letra_S[8] = {
  0x3E,
  0x20,
  0x20,
  0x3E,
  0x02,
  0x02,
  0x02,
  0x3E
};

code unsigned char letra_C[8] = {
  0x3E,
  0x02,
  0x02,
  0x02,
  0x02,
  0x02,
  0x02,
  0x3E
};

code unsigned char letra_O[8] = {
  0x3E,
  0x22,
  0x22,
  0x22,
  0x22,
  0x22,
  0x22,
  0x3E
};

code unsigned char letra_R[8] = {
  0x44,
  0x24,
  0x14,
  0x7C,
  0x44,
  0x44,
  0x44,
  0x7C
};
code unsigned char letra_E[8] = {
  0x3E,
  0x02,
  0x02,
  0x02,
  0x3E,
  0x02,
  0x02,
  0x3E
};

code unsigned char digito_0[8] = {0x3C, 0x66, 0x6E, 0x76, 0x66, 0x66, 0x3C, 0x00};
code unsigned char digito_1[8] = {0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
code unsigned char digito_2[8] = {0x1E, 0x02, 0x02, 0x1E, 0x10, 0x10, 0x10, 0x1E};
code unsigned char digito_3[8] = {0x0E, 0x10, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x0E};
code unsigned char digito_4[8] = {0x20, 0x20, 0x20, 0x20, 0x3C, 0x24, 0x24, 0x24};
code unsigned char digito_5[8] = {0x3C, 0x20, 0x20, 0x3C, 0x04, 0x04, 0x04, 0x3C};
code unsigned char digito_6[8] = {0x3C, 0x24, 0x24, 0x24, 0x3C, 0x04, 0x04, 0x3C};
code unsigned char digito_7[8] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3C};
code unsigned char digito_8[8] = {0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00};
code unsigned char digito_9[8] = {0x3C, 0x20, 0x20, 0x3C, 0x24, 0x24, 0x24, 0x3C};


unsigned char road_left[ROAD_SEGMENTS];  // posição esquerda da estrada
unsigned char road_index = 0;            // qual segmento está no topo

code const unsigned char* digitos[] = {
  digito_0, digito_1, digito_2, digito_3, digito_4,
  digito_5, digito_6, digito_7, digito_8, digito_9
};

void Draw_Score_Label();
// --- Estado do jogo ---
unsigned char scroll_offset;//Usado para controlar o deslocamento vertical para o efeito de rolagem suave da estrada.
unsigned char player_car_x;
unsigned char enemy_x[MAX_ENEMIES];
unsigned char enemy_y[MAX_ENEMIES];
unsigned char enemy_active[MAX_ENEMIES];
unsigned char random_seed;
unsigned char pista_largura_total = 64;
bit game_over = 0;
int score = 0;
int velocity = 1;
// --- Funções auxiliares ---
void delay(unsigned int k) {
  unsigned int i,j;
  for(i=0;i<k;i++)
    for(j=0;j<120;j++);
}

unsigned char pseudo_rand() {
  random_seed ^= (random_seed << 3);
  random_seed ^= (random_seed >> 5);
  random_seed ^= (random_seed << 4);
  return random_seed;
}

void GLCD_SelectChip(unsigned char chip) {
  CS1 = CS2 = 0;
  if(chip == 0) CS1 = 1;
  else          CS2 = 1;
  nop();
}

void rotate_bitmap_8x8(const unsigned char *input, unsigned char *output) {
  unsigned char x, y;
  for (y = 0; y < 8; y++) {
    output[y] = 0;
    for (x = 0; x < 8; x++) {
      if (input[7 - x] & (1 << y)) { 
        output[y] |= (1 << x);        
      }
    }
  }
}


void GLCD_Command(char cmd) {
  RS = 0; RW = 0;
  Data_Port = cmd;
  E = 1;
  nop(); nop(); nop();
  E = 0; nop();
}

void GLCD_Data(char d) {
  RS = 1; RW = 0;
  Data_Port = d;
  E = 1;
  nop(); nop(); nop();
  E = 0; nop();
}

void GLCD_Init() {
  RST = 1;
  delay(20);

  GLCD_SelectChip(0);
  GLCD_Command(0x3E);
  GLCD_Command(0x40);
  GLCD_Command(0xB8);
  GLCD_Command(0xC0);
  GLCD_Command(0x3F);

  GLCD_SelectChip(1);
  GLCD_Command(0x3E);
  GLCD_Command(0x40);
  GLCD_Command(0xB8);
  GLCD_Command(0xC0);
  GLCD_Command(0x3F);
}

void GLCD_ClearAll() {
  unsigned char page;
  int col;
  for(page = 0; page < 8; page++) {
    GLCD_SelectChip(0);
    GLCD_Command(0xB8 + page);
    GLCD_Command(0x40);
    for(col = 0; col < 64; col++) GLCD_Data(0);

    GLCD_SelectChip(1);
    GLCD_Command(0xB8 + page);
    GLCD_Command(0x40);
    for(col = 0; col < 64; col++) GLCD_Data(0);
  }
}

void GLCD_Draw8x8(unsigned char x, unsigned char y, const unsigned char *bmp) {
  unsigned char rotated[8];
  unsigned char page;
  unsigned char chip1;
  unsigned char col1;
  unsigned char i;

  rotate_bitmap_8x8(bmp, rotated);

  page  = y / 8;
  chip1 = (x < 64 ? 1 : 0);
  col1  = (x < 64 ? x : x - 64);
  
  if (x <= 63 && x + 7 >= 64) {
    GLCD_SelectChip(1);
    GLCD_Command(0xB8 + page);
    GLCD_Command(0x40 + col1);
    for (i = 0; i < 64 - x; i++) {
      GLCD_Data(rotated[i]);
    }

    GLCD_SelectChip(0);
    GLCD_Command(0xB8 + page);
    GLCD_Command(0x40);
    for (; i < 8; i++) {
      GLCD_Data(rotated[i]);
    }

  } else {
    GLCD_SelectChip(chip1);
    GLCD_Command(0xB8 + page);
    GLCD_Command(0x40 + col1);
    for (i = 0; i < 8; i++) {
      GLCD_Data(rotated[i]);
    }
  }
}



void GLCD_Draw4x8(unsigned char x, unsigned char y, const unsigned char *bmp) {
  unsigned char page = y / 8;
  unsigned char chip = (x < 64 ? 0 : 1);
  unsigned char col  = (x < 64 ? x : x - 64);
  unsigned char i;

  GLCD_SelectChip(chip);
  GLCD_Command(0xB8 + page);
  GLCD_Command(0x40 + col);
  for(i = 0; i < 4; i++) {
    GLCD_Data(bmp[i]);
  }
}

void Game_Init() {
  unsigned char i;
  player_car_x = (SCREEN_W / 2) - (PLAYER_CAR_WIDTH / 2);
  scroll_offset = 0;
  random_seed = 0xA5;
  game_over = 0;
  for(i = 0; i < MAX_ENEMIES; i++) {
    enemy_active[i] = 0;
    enemy_x[i] = 0;
    enemy_y[i] = 0;
  }
	
	for(i = 0; i < ROAD_SEGMENTS; i++) {
  road_left[i] = (SCREEN_W / 2) - (pista_largura_total / 2);
}

}

void Game_Update() {
  unsigned char i;
  unsigned char seg, left, right, index;
  unsigned char prev_left, new_left;
  signed char delta;
  unsigned char max_x;

  // Atualizar estrada dinâmica
  unsigned char r = pseudo_rand() % 10;
	if (r < 4)
		delta = -1;
	else if (r < 6)
		delta = 0;
	else
		delta = 1;
  index = (road_index + ROAD_SEGMENTS - 1) % ROAD_SEGMENTS;
  prev_left = road_left[index];
  new_left = prev_left + delta;

  if (new_left < 4)
    new_left = 4;
  if (new_left > SCREEN_W - pista_largura_total - 4)
    new_left = SCREEN_W - pista_largura_total - 4;

  road_left[road_index] = new_left;
  road_index = (road_index + 1) % ROAD_SEGMENTS;

  // Movimento do jogador
  seg = (road_index + (PLAYER_Y / ROAD_LINE_HEIGHT)) % ROAD_SEGMENTS;
  left  = road_left[seg];
  right = left + pista_largura_total - PLAYER_CAR_WIDTH;

  if (!BUTTON_LEFT && player_car_x > left + ROAD_LINE_WIDTH)
    player_car_x -= 2;
  if (!BUTTON_RIGHT && player_car_x < right - ROAD_LINE_WIDTH)
    player_car_x += 2;

  // Colisão com bordas
  if (player_car_x < left + ROAD_LINE_WIDTH || player_car_x + PLAYER_CAR_WIDTH > left + pista_largura_total - ROAD_LINE_WIDTH)
    game_over = 1;

  // Scroll
  scroll_offset++;
  if (scroll_offset >= ROAD_LINE_HEIGHT)
    scroll_offset = 0;

  // Atualizar inimigos

  for (i = 0; i < MAX_ENEMIES; i++) {
    if (enemy_active[i]) {
      enemy_y[i] += velocity;
      if (enemy_y[i] >= SCREEN_H)
        enemy_active[i] = 0;
    } else {
      if ((pseudo_rand() % 100) < 4) {
        seg = (road_index + 0) % ROAD_SEGMENTS;
        left = road_left[seg];
        max_x = pista_largura_total - 2 * ROAD_LINE_WIDTH - PLAYER_CAR_WIDTH;
        if (max_x > 0) {
          enemy_active[i] = 1;
          enemy_y[i] = 0;
          enemy_x[i] = left + ROAD_LINE_WIDTH + (pseudo_rand() % max_x);
        }
      }
    }

    // Colisão com inimigos
    if (enemy_active[i]) {
      if (player_car_x < enemy_x[i] + PLAYER_CAR_WIDTH &&
          player_car_x + PLAYER_CAR_WIDTH > enemy_x[i] &&
          PLAYER_Y < enemy_y[i] + PLAYER_CAR_HEIGHT &&
          PLAYER_Y + PLAYER_CAR_HEIGHT > enemy_y[i]) {
        game_over = 1;
      }
    }
  }
}


void GLCD_ClearPartial() {
  unsigned char page, col;
  unsigned char seg;
  unsigned char x_start, x_end;
  

  for (page = 0; page < SCREEN_H / 8; page++) {
    if (page == 0)
      continue;  // HUD

    seg = (road_index + page) % ROAD_SEGMENTS;
    x_start = road_left[seg];
    x_end = x_start + pista_largura_total;

    // Limpa chip 1 (col 0–63)
    if (x_start < 64 || x_end <= 64) {
      GLCD_SelectChip(1);
      GLCD_Command(0xB8 + page);
      GLCD_Command(0x40);

      for (col = 0; col < 64; col++) {
        GLCD_Data(0x00);  // limpa tudo no chip 1
      }
    }

    // Limpa chip 0 (col 64–127)
    if (x_end > 64) {
      GLCD_SelectChip(0);
      GLCD_Command(0xB8 + page);
      GLCD_Command(0x40);

      for (col = 0; col < 64; col++) {
        GLCD_Data(0x00);  // limpa tudo no chip 0
      }
    }
  }
}





void Game_Draw() {
  unsigned char i, j;
  unsigned char left_x  = (SCREEN_W / 2) - (pista_largura_total / 2);
  unsigned char right_x = (SCREEN_W / 2) + (pista_largura_total / 2) - ROAD_LINE_WIDTH;
  GLCD_ClearPartial();

  if (game_over) {
    GLCD_ClearAll();
    //Draw_Score_Label();  // Reexibe SCORE após limpar
    return;
  }

  // Desenhar as linhas da estrada 
  for (j = 0; j < ROAD_SEGMENTS; j++) {
    unsigned char index = (road_index + j) % ROAD_SEGMENTS;
    unsigned char y = (j * ROAD_LINE_HEIGHT) - scroll_offset;
    unsigned char left, right;
		
    if (y < 16 || y >= SCREEN_H) continue;  // Ignora page 0 e além da tela
		

    left = road_left[index];
    right = left + pista_largura_total - ROAD_LINE_WIDTH;
		
    GLCD_Draw4x8(left,  y, road_line_4x8);
    GLCD_Draw4x8(right, y, road_line_4x8);
  }

  // Desenhar o carro do jogador (sempre abaixo da page 0)
  if (PLAYER_Y >= 8)
    GLCD_Draw8x8(player_car_x, PLAYER_Y, solid_8x8);

  // Desenhar os inimigos
  for (i = 0; i < MAX_ENEMIES; i++) {
    if (enemy_active[i] && enemy_y[i] >= 8) {
      GLCD_Draw8x8(enemy_x[i], enemy_y[i], asterisk_8x8);
    }
  }

  // (Opcional) Redesenhar SCORE se desejar manter "à prova de bugs"
  //Draw_Score_Label(); // Garantir que SCORE sempre apareça corretamente
}

void Draw_Score_Label(int score) {
    unsigned char i;
    unsigned char digits[4] = {0, 0, 0, 0};
    unsigned int val = score;

    // Desenha a palavra SCORE
    GLCD_Draw8x8(0, 0, letra_S);
    GLCD_Draw8x8(8, 0, letra_C);
    GLCD_Draw8x8(16, 0, letra_O);
    GLCD_Draw8x8(24, 0, letra_R);
    GLCD_Draw8x8(32, 0, letra_E);

    // Extrai até 4 dígitos do número
    digits[0] = (val / 1000) % 10;
    digits[1] = (val / 100) % 10;
    digits[2] = (val / 10) % 10;
    digits[3] = val % 10;

    // Desenha os dígitos ao lado da palavra
    for (i = 0; i < 4; i++) {
        GLCD_Draw8x8(48 + (i * 8), 0, digitos[digits[i]]);
    }
}

void main() {
  int frame = 0;
  GLCD_Init();
  GLCD_ClearAll();
  delay(50);
  Game_Init();
  Draw_Score_Label(score);

  while (1) {
    frame++;

    if (!game_over) {
      Game_Update();

      if (frame % 3 == 0) score++;
      if (score % 15 == 0 && velocity < 9) velocity++;

    } else {
      // Wait for restart button release to reinit game
      if (!BUTTON_RESTART) {
        GLCD_ClearAll();
        Game_Init();
        score = 0;
        velocity = 1;
        frame = 0;
        continue;
      }

      // Show GAME OVER while button is held
      while (BUTTON_RESTART) {
        Draw_Score_Label(score);
        GLCD_Draw8x8(20, 24, letra_G);
        GLCD_Draw8x8(28, 24, letra_A);
        GLCD_Draw8x8(36, 24, letra_M);
        GLCD_Draw8x8(44, 24, letra_E);
        GLCD_Draw8x8(64, 24, letra_O);
        GLCD_Draw8x8(72, 24, letra_V);
        GLCD_Draw8x8(80, 24, letra_E);
        GLCD_Draw8x8(88, 24, letra_R);
        delay(50); // avoid flickering or button bouncing
      }

      continue;
    }

    Draw_Score_Label(score);
    Game_Draw();
    
  }
}