
#define __ITEM_H__

#include "GameObject.h" 

#define ITEMS_BBOX_WIDTH  15
#define ITEMS_BBOX_HEIGHT 15

#define ITEMS_TYPE_POWER			601
#define ITEMS_TYPE_HOVER			602
#define ITEMS_TYPE_GUN				603
#define ITEMS_TYPE_THUNDER_BREAK	604
#define ITEMS_TYPE_CRUSHER_BEAM		605
#define ITEMS_HAS_TAKEN_X			1000

class CItems : public CGameObject
{
	protected:
		DWORD timeLive;
		int type;
		
	public:
		CItems(float x = 0.0f, float y = 0.0f);
		CItems(int itemType, float x = 0.0f, float y = 0.0f);
		virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* listObject = NULL);
		virtual void Render();
		virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
		void setItemType(int type);
		void countDownTime(int time);
		void hasTaken();
		int getItemType();
		~CItems();
};
