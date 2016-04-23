#ifndef __CURRENT_H_GAME_
#define __CURRENT_H_GAME_



namespace current {

	template<int SIZE_W, int SIZE_H>
	class Current {
	public:

		enum direction { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };
		enum direction current_direction;

		int speed;
		unsigned int score, num;

		Current();
		void initgraph();
		void update();
		void timer_update();
		int operator()(int x, int y);
		void setGraphItem(int x, int y, unsigned char col);

		inline unsigned int getScore() const { return score; }
		inline unsigned int getClears() const { return num; }
		bool gameOver() const { return game_over; }

		void addLine();
		void moveLeft();
		void moveRight();
		void switchColors();

		unsigned char graph[SIZE_W][SIZE_H];
	protected:

		bool game_over;

		enum { HALF_SIZE = SIZE_H/2 };
		enum { MAX_COLORS = 8 };

		void mergeBlock();
		void moveBlocksDown();
		void procBlocks();

		class Block {
		public:

			int x, y, blocks[3];

			Block();
			Block(const Block &b);
			inline Block &operator=(const Block &b) { x=b.x, y=b.y; blocks[0]=b.blocks[0]; blocks[1]=b.blocks[1]; blocks[2]=b.blocks[2]; return *this; }
			void reset();
			void setRand(const int max);
			void move(direction d);
			void shift();
		};

	public:
		Block nextblock;
		Block gameblock;
	};

}

#ifndef CURRENT_SOURCE_H_
#define CURRENT_SOURCE_H_
#include "current.cc"
#endif


#endif

